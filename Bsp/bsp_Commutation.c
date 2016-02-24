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
static s16 Middle_PWM_Duty = 50;

static const  s8 I_IN  = 1;  //��������,�����Ϊ��
static const  s8 I_OUT = -1; //��������,�����Ϊ��
static const  s8 I_OFF = 0;  //�޵���
/*����ʱA,B,C�����״̬״̬��,120�Ȼ���*/
/*1.B+A-    5.B+C-   4.A+C-     6.A+B-   2.C+B-    3.C+A-*///����
/*3.A+B-    2.A+C-   6.B+C-     4.B+A-   5.C+A-    1.C+B-*///����

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
    static u16 lasthallpos = 0;
//    float temp_duty = 0;  //��ʱռ�ձ�
    s8 hall_state[3] = {0, 0, 0}; //ABC����״̬����
    u16 newhallpos = 0;
//    GPIO_ReadInputDataBit(HallA_Port, HallA_Pin)
//                     + GPIO_ReadInputDataBit(HallB_Port, HallB_Pin) * 2
//                     + GPIO_ReadInputDataBit(HallC_Port, HallC_Pin) * 4;
    State_TabIndex = newhallpos;

    if (newhallpos == lasthallpos)
    {
        return;  //�޻����¼�
    }
//    if(Motor_SetedSpeed == 0)
//    {
//        TIM1_Set_PWMOutput_EnableState(1, 0, 0); //ȫ������
//        TIM1_Set_PWMOutput_EnableState(2, 0, 0); //ȫ������
//        TIM1_Set_PWMOutput_EnableState(3, 0, 0); //ȫ������
//        return;  //       
//    }
#if 1   //����һ�ŵĻ���
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
    for (u8 ii = 0; ii < 3; ii++) /**/
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
    Motor_SetedSpeed = speed / 10.0;
    if(last_speed*Motor_SetedSpeed<=0)
    { 
        /*�ϴκ�����ٶȷ���ͬʱҪ����һ�λ����¼�*/
        Hall_PrepareCommutation();
    }
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
//        temp_duty = hall_state[ii] * MyAbs(Motor_SetedSpeed)/ 2.0;
        temp_duty = hall_state[ii] * Motor_SetedSpeed/ 2.0;      

        #endif
        #if 0 //������
        hall_state[ii] = BLDC_STATE_TAB[State_TabIndex][ii];//����  
        temp_duty = hall_state[ii] * Motor_SetedSpeed/ 2.0;      
        #endif
        temp_duty += Middle_PWM_Duty;
        TIM1_Set_PWMDuty(ii + 1, temp_duty); 
    } 
    last_speed = Motor_SetedSpeed;
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
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */

