/********************************************************************************
 *@FileName    bsp_Commutation.c
 *@Author      Johnbee
 *@Version     V0.1
 *@Date        2015/02/23
 *@Brief       �������
 *******************************************************************************/
/*#includes*********************************************************************/
#include "bsp_Commutation.h"

/*#define***********************************************************************/
#define  MyAbs(a)    (a>0?a:-a)
    
/*ȫ�ֱ�������******************************************************************/
static u8 State_TabIndex = 0; //����ʱ����״̬����±�
static float Motor_SetedSpeed = 0;  //����ٶ�-100~100
static s16 Middle_PWM_Duty = 500;

static const  s8 I_IN  = 1;  //��������,�����Ϊ��
static const  s8 I_OUT = -1; //��������,�����Ϊ��
static const  s8 I_OFF = 0;  //�޵���
/*����ʱA,B,C�����״̬״̬��,120�Ȼ���*/
/*1.B+A-    5.B+C-   4.A+C-     6.A+B-   2.C+B-    3.C+A-*///����
/*3.A+B-    2.A+C-   6.B+C-     4.B+A-   5.C+A-    1.C+B-*///����
/*3.B+C-    2.B+A-   6.C+A-     4.C+B-   5.A+B-    1.A+C-*///����

static const s8 BLDC_STATE_TAB[8][3] =  //�����
{
    {I_OFF, I_OFF, I_OFF},  //CBA = 000ʱABC�����������
    {I_OUT, I_IN , I_OFF},  //B+A-;
    {I_OFF, I_OUT, I_IN },  //C+B-;
    {I_OUT, I_OFF, I_IN },  //C+A-;
    {I_IN , I_OFF, I_OUT},  //A+C-;
    {I_OFF, I_IN , I_OUT},  //B+C-;
    {I_IN,  I_OUT, I_OFF},  //A+B-;
    {I_OFF, I_OFF, I_OFF},  //CBA = 111,120�Ȼ�����������������,���������һ��������,��������ɲ��

};
static const s8 BLDC_STATE_TAB_Fan[8][3] =  //�����
{
    {I_OFF, I_OFF, I_OFF},  //CBA = 000ʱABC�����������
    {I_OFF, I_OUT, I_IN },  //C+B-;
    {I_IN , I_OFF, I_OUT},  //A+C-;
    {I_IN,  I_OUT, I_OFF},  //A+B-;    
    {I_OUT, I_IN , I_OFF},  //B+A-;    
    {I_OUT, I_OFF, I_IN },  //C+A-;    
    {I_OFF, I_IN , I_OUT},  //B+C-;
    {I_OFF, I_OFF, I_OFF},  //CBA = 111,120�Ȼ�����������������,���������һ��������,��������ɲ��
};

/*
 *@ <function name=>Hall_PrepareCommutation() </function>
 *@ <summary>
     ���������Ļ����¼�
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void Hall_PrepareCommutation(void)
{
    static u8 lasthallpos = 0;
    float temp_duty = 0;  //��ʱռ�ձ�
    s8 hall_state[3] = {0, 0, 0}; //ABC����״̬����
    u8 newhallpos =  Get_HallInputValue();
    State_TabIndex = newhallpos;
#if 0   //����һ�ŵĻ���
    for (u8 ii = 0; ii < 3; ii++) /*���ҵ�off����������*/
    {
        #if 1 //˫����
        if(Motor_SetedSpeed>0)
        {
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//����         
        }
        else
        {
           hall_state[ii] = BLDC_STATE_TAB_Fan[newhallpos][ii]; //����                    
        }
        #endif
        #if 0 //������
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//����        
        #endif
        if (hall_state[ii] == 0)
        {
            TIM1_Set_PWMOutput_EnableState(ii + 1, 0, 0); //ȫ������
            //break;
        }
    }
    for (u8 ii = 0; ii < 3; ii++) /*�ٸı䲻��off����*/
    {
        #if 1 //˫����
        if(Motor_SetedSpeed>=0)
        {
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//����         
        }
        else
        {
           hall_state[ii] = BLDC_STATE_TAB_Fan[newhallpos][ii]; //����                    
        }
        #endif
        #if 0 //������
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//����        
        #endif
        if (hall_state[ii] != 0)
        {
            TIM1_Set_PWMOutput_EnableState(ii + 1, 1, 1); // ʹ�����
        }
    }
#endif
#if 1  //ʹ��COM�¼�����,���Ŀ�ͬʱ��Ч
    for (u8 ii = 0; ii < 3; ii++) /*ֱ�Ӳ����*/
    {
        #if 1 //˫����
        if(Motor_SetedSpeed>=0)
        {
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//����         
        }
        else
        {
           hall_state[ii] = BLDC_STATE_TAB_Fan[newhallpos][ii]; //����                    
        }       
        #endif
        #if 0 //������
           hall_state[ii] = BLDC_STATE_TAB[newhallpos][ii];//����        
        #endif
        if (hall_state[ii] == 0)
        {
            TIM1_Set_PWMOutput_EnableState(ii + 1, 0, 0); //ȫ������
        }
        else
        {
            TIM1_Set_PWMOutput_EnableState(ii + 1, 1, 1); //ʹ��           
        }                
        temp_duty = hall_state[ii] * Motor_SetedSpeed / 2;      
        temp_duty += Middle_PWM_Duty;
        TIM1_Set_PWMDuty(ii + 1, temp_duty); 
    }
    
#endif    
    lasthallpos = newhallpos;
}
/*
 *@ <function name=>Set_MotorSpeed() </function>
 *@ <summary>
    ���õ���ٶ�
 *@ </summary>
 *@ <param name="speed">�ٶȴ�-1000��1000</param>
 *@ <returns> null </returns>
 */
void Set_MotorSpeed(float speed)
{
#if 1  //ֻ����ı�ռ�ձ�,ʹ���ڻ����¼��иı�,˫����
    s8 hall_state[3] = {0, 0, 0}; //ABC����״̬����
    float temp_duty = 0;  //��ʱռ�ձ�
    static float last_speed = 0;
    Motor_SetedSpeed = speed;
    if(last_speed*Motor_SetedSpeed<=0)
    { 
        /*�ϴκ�����ٶȷ���ͬʱҪ����һ�λ����¼�*/
        Hall_PrepareCommutation();
        Manual_COMevent();
    }
    else
    {
        for(u8 ii = 0; ii<3;ii++)
        {  
            #if 1 //˫����
            if(Motor_SetedSpeed>=0)
            {
               hall_state[ii] = BLDC_STATE_TAB[State_TabIndex][ii];//����         
            }
            else
            {
               hall_state[ii] = BLDC_STATE_TAB_Fan[State_TabIndex][ii]; //����                    
            }

            #endif
            #if 0 //������
            hall_state[ii] = BLDC_STATE_TAB[State_TabIndex][ii];//����  
            #endif
            temp_duty = hall_state[ii] * Motor_SetedSpeed / 2;                  
            temp_duty += Middle_PWM_Duty;
            TIM1_Set_PWMDuty(ii + 1, temp_duty); 
        } 
        last_speed = Motor_SetedSpeed;
    }
#endif
}
/*
 *@ <function name=>GraytoDecimal() </function>
 *@ <summary>
    u16���͵�Gray��תʮ����
 *@ </summary>
 *@ <param name="x">Gray��</param>
 *@ <returns> Gray���Ӧ��ʮ������ </returns>
 */
static u16 GraytoDecimal(u16 x) 
{ 
    u16 y = x; 
    while(x>>=1) 
    {
        y ^= x;        
    }
    return y;        
}
/*
 *@ <function name=>HallCode2Decimal() </function>
 *@ <summary>
    ����������תʮ����
    hall����Ϊ5 4 6 2 3 1����6 5 4 3 2 1 
 *@ </summary>
 *@ <param name="x">������</param>
 *@ <returns> �������Ӧ��ʮ������ </returns>
 */
static u8 HallCode2Decimal(u8 x)
{
    u16 temp = x;
    temp = GraytoDecimal(temp);
    if(temp == 7)
    {
        temp = 5;
    }
    return temp;
}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */

