/********************************************************************************
 *@FileName    bsp_HallInput.c
 *@Author      Johnbee
 *@Version     V0.1
 *@Date        2015/10/19
 *@Brief       ������������   PA4 5 6 �����������
 *******************************************************************************/
/*#includes*********************************************************************/
#include "bsp_HallInput.h"

#ifdef  _STM32F10x_
    #define HallA_Port              GPIOA
    #define HallA_Pin               GPIO_Pin_4
    #define HallA_Exit_Line         EXTI_Line4
    #define HallA_GPIO_PinSource    GPIO_PinSource4
    #define HallA_GPIO_PortSource   GPIO_PortSourceGPIOA
    #define RCC_HallA_Port          RCC_APB2Periph_GPIOA // | RCC_APB2Periph_AFIO

    #define HallB_Port              GPIOA
    #define HallB_Pin               GPIO_Pin_5
    #define HallB_Exit_Line         EXTI_Line5
    #define HallB_GPIO_PinSource    GPIO_PinSource5
    #define HallB_GPIO_PortSource   GPIO_PortSourceGPIOA
    #define RCC_HallB_Port          RCC_APB2Periph_GPIOA // | RCC_APB2Periph_AFIO

    #define HallC_Port              GPIOA
    #define HallC_Pin               GPIO_Pin_6
    #define HallC_Exit_Line         EXTI_Line6
    #define HallC_GPIO_PinSource    GPIO_PinSource6
    #define HallC_GPIO_PortSource   GPIO_PortSourceGPIOA
    #define RCC_HallC_Port          RCC_APB2Periph_GPIOA  //| RCC_APB2Periph_AFIO
    #endif
#ifdef  _STM32F4xx_

#endif
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
///*1.A+B-    3.A+C-   2.B+C-     6.B+A-   4.C+A-    5.C+B-*///����

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
 *@ <function name=>Hall_GPIO_Init() </function>
 *@ <summary>
      ����������������������
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void Hall_GPIO_Init(void)
{

#ifdef  _STM32F10x_
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_HallA_Port | RCC_HallB_Port | RCC_HallC_Port, ENABLE);

    GPIO_InitStructure.GPIO_Pin = HallA_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
    GPIO_Init(HallA_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HallB_Pin;
    GPIO_Init(HallB_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = HallC_Pin;
    GPIO_Init(HallC_Port, &GPIO_InitStructure);

    /* EXTI line mode config */
    GPIO_EXTILineConfig(HallA_GPIO_PortSource, HallA_GPIO_PinSource);
    EXTI_InitStructure.EXTI_Line = HallA_Exit_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�������ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    GPIO_EXTILineConfig(HallB_GPIO_PortSource, HallB_GPIO_PinSource);
    EXTI_InitStructure.EXTI_Line = HallB_Exit_Line;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(HallC_GPIO_PortSource, HallC_GPIO_PinSource);
    EXTI_InitStructure.EXTI_Line = HallC_Exit_Line;
    EXTI_Init(&EXTI_InitStructure);
#endif

#ifdef _STM32F4xx_
    //��ûд
#endif
}

/*
 *@ <function name=>HallBC_NVIC_Config() </function>
 *@ <summary>
      ��������������BC�������ж����ȼ�����
 *@ </summary>
 *@ <param name="preemPriority">��ռ���ȼ�</param>
 *@ <param name="subPriority">�������ȼ�</param>
 *@ <returns> null </returns>
 */
void EXTI9_5_NVIC_Config(u8 preemPriority, u8 subPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 *@ <function name=>HallA_NVIC_Config() </function>
 *@ <summary>
      ��������������A�������ж����ȼ�����
 *@ </summary>
 *@ <param name="preemPriority">��ռ���ȼ�</param>
 *@ <param name="subPriority">�������ȼ�</param>
 *@ <returns> null </returns>
 */
void EXTI4_NVIC_Config(u8 preemPriority, u8 subPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 *@ <function name=>EXTI9_5_IRQPander() </function>
 *@ <summary>
      ��������������BC�������ж��Ӻ���,��stm32f10x_it.c�е���
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void EXTI9_5_IRQPander(void)
{
//    if (EXTI_GetITStatus(HallC_Exit_Line) != RESET) //ȷ���Ƿ������EXTI Line�ж�
//    {
//        EXTI_ClearITPendingBit(HallC_Exit_Line);     //����жϱ�־λ
//        Hall_PrepareCommutation();
//    }
//    if (EXTI_GetITStatus(HallB_Exit_Line) != RESET) //ȷ���Ƿ������EXTI Line�ж�
//    {
//        EXTI_ClearITPendingBit(HallB_Exit_Line);     //����жϱ�־λ
//        Hall_PrepareCommutation();
//    }

}
/*
 *@ <function name=>EXTI4_IRQPander() </function>
 *@ <summary>
      ��������������A�������ж��Ӻ���,��stm32f10x_it.c�е���
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void EXTI4_IRQPander(void)
{
//    if (EXTI_GetITStatus(HallA_Exit_Line) != RESET) //ȷ���Ƿ������EXTI Line�ж�
//    {
//        EXTI_ClearITPendingBit(HallA_Exit_Line);     //����жϱ�־λ
//        Hall_PrepareCommutation();
//    }
}
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

