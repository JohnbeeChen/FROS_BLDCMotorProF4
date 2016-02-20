/********************************************************************************
 *@FileName    bsp_TIM.c
 *@Author      Johnbee
 *@Version     V0.1
 *@Date        2015/10/19
 *@Brief       ����PWM���   TIM1 CH1~CH3 PA8_PA10 ΪPWM��� ABC����
                             TIM1 CH1N~CH3N PB13_P1510 ΪPWM�������
 *******************************************************************************/
/*#includes*********************************************************************/
#include "bsp_TIM.h"

/*#define***********************************************************************/
#ifdef _STM32F10x_
#define TIM1_PWM_Frequence	36000  //PWMƵ��
#define TIM1_PWM_Period     2000  //PWMһ��������
#endif
#ifdef  _STM32F4xx_
#define _UES_TIM1_HALL_  //Enable the hall's interface of timer1
#define Hall_TIM                TIM3
#define RCC_Hall_TIM            RCC_APB1Periph_TIM3
#define Hall_TIM_GPIO           GPIOC
#define RCC_Hall_TIM_GPIO       RCC_AHB1Periph_GPIOC
#define Hall_TIM_Pin            GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8
#define Hall_TIM_IRQn           TIM3_IRQn
#define TIM1_PWM_Frequence	(30000u)  //PWMƵ��30KHz
#define TIM1_PWM_Period     (5600u)  //PWMһ��������
#endif


/*
 *@ <function name=>TIM1_TRG_COM_NVIC_Config() </function>
 *@ <summary>
       TIM1 COM�¼��ж����ȼ�����
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void TIM1_TRG_COM_NVIC_Config(u8 preemPriority, u8 subPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;// TIM1_TRG_COM_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 *@ <function name=>BLE_IRQPandler() </function>
 *@ <summary>
     This function handles motor timer trigger and commutation interrupts
     can be used for calculation...
     ��stm32f10x_it.c�е�����
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void TIM1_TRG_COM_IRQPandler(void)
{
    TIM_ClearITPendingBit(TIM1, TIM_IT_COM);
    // commutationCount++;
}
/*
 *@ <function name=>Hall_TIM_NVIC_Config() </function>
 *@ <summary>
       �����ӿڶ�ʱ���ж����ȼ�����
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void Hall_TIM_NVIC_Config(u8 preemPriority, u8 subPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = Hall_TIM_IRQn;// ������ʱ���ж�;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 *@ <function name=> Hall_TIM_IRQPandler() </function>
 *@ <summary>
     hall��ʱ���жϴ����Ӻ���
     ��stm32f10x_it.c�е�����
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void Hall_TIM_IRQPandler(void)
{
    //TIM_ClearITPendingBit(TIM1, TIM_IT_COM);
}
/*
 *@ <function name=>TIM1_PWM_Config() </function>
 *@ <summary>
       ���ö�ʱ������PWM���
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void TIM1_PWM_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef        TIM_OCInitStructure;
    TIM_BDTRInitTypeDef      TIM_BDTRInitStructure;
    TIM1_GPIO_Config();
#ifdef _STM32F4xx_
    TIM_DeInit(TIM1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //ʹ�ܶ�ʱ����ʱ��
    TIM_TimeBaseStructure.TIM_Period = TIM1_PWM_Period - 1;              //����
    TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / TIM1_PWM_Period / TIM1_PWM_Frequence - 1; //��Ƶ��Ƶ����ʱ��,���Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Channel 1, 2,3 and 4 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_Pulse = 0;  //ռ�ձ�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�����������
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//����״̬ʱ����ͨ������֮�������ƽ
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�������ʹ��
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//����ͨ�������������
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//����ͨ������״̬ʱ����ͨ������֮�������ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//����ͨ�����ʹ��

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);					//ʹ��TIM1_Channel2
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ������Ƚ�
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);					//ʹ��TIM1_Channel3
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);         //
    /* DeadTime value n=1 bis 31: from 14ns to 1,7us
      DeadTime value n=129 bis 159: from 1,7�s to 3,5ms
      DeadTime value 7 => 98ns
       ... see programmers reference manual
       DeadTime[ns] = value * (1/SystemCoreFreq) (on 72MHz: 7 is 98ns)*/

    TIM_BDTRInitStructure.TIM_DeadTime = 80;  //����ʱ��ns���
    /* Automatic Output enable, Break, dead time and lock configuration*/
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//AOEʹ��
    TIM_BDTRInitStructure.TIM_OSSRState  = TIM_OSSRState_Disable;  //Run״̬��ʱ��������ʱ�����
    TIM_BDTRInitStructure.TIM_OSSIState  = TIM_OSSIState_Disable;  //Stop״̬��ʱ��������ʱ�����
    TIM_BDTRInitStructure.TIM_LOCKLevel  = TIM_LOCKLevel_OFF;  //����ʱ��������ʱ�޸�
    /* enabel this if you use emergency stop signal*/
//    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable; //ɲ��ʹ��
//    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High; //ɲ���ߵ�ƽ��Ч
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
//    TIM1->BDTR |= (1<<15);  //ʹ��MOEλ
#ifdef  _UES_TIM1_HALL_
    // preload ARR register
    TIM_CCPreloadControl(TIM1, ENABLE);
    TIM_SelectCOM(TIM1, ENABLE);
    Hall_TimerConfig();
    // Choose carefully from the following possible combination
    // check programmers reference manual
    // TIM_SelectInputTrigger(TIM1, TIM_TS_ITR0);// MotorTimer = TIM1, HallTimer = TIM5
    // TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);// MotorTimer = TIM1, HallTimer = TIM2
    TIM_SelectInputTrigger(TIM1, TIM_TS_ITR2);// MotorTimer = TIM1, HallTimer = TIM3
    // TIM_SelectInputTrigger(TIM1, TIM_TS_ITR3);// MotorTimer = TIM1, HallTimer = TIM4
    // TIM_SelectInputTrigger(TIM8, TIM_TS_ITR0);// MotorTimer = TIM8, HallTimer = TIM1
    // TIM_SelectInputTrigger(TIM8, TIM_TS_ITR1);// MotorTimer = TIM8, HallTimer = TIM2
    // TIM_SelectInputTrigger(TIM8, TIM_TS_ITR2);// MotorTimer = TIM8, HallTimer = TIM4
    // TIM_SelectInputTrigger(TIM8, TIM_TS_ITR3);// MotorTimer = TIM8, HallTimer = TIM5
#endif
    TIM_ARRPreloadConfig(TIM1, ENABLE);   //ʹ�ܶ�ʱ����װ��
    TIM_Cmd(TIM1, ENABLE);   //ʹ�ܶ�ʱ��1
    TIM_CtrlPWMOutputs(TIM1, ENABLE); //�߼���ʱ����Ҫ����һ��
#endif
    TIM1_Set_PWMOutput_EnableState(1, 0, 0);
    TIM1_Set_PWMOutput_EnableState(2, 0, 0);
    TIM1_Set_PWMOutput_EnableState(3, 0, 0);
    TIM1_Set_PWMDuty(1, 50);
    TIM1_Set_PWMDuty(2, 50);
    TIM1_Set_PWMDuty(3, 50);
}

/*
 *@ <function name=>Hall_TimerConfig() </function>
 *@ <summary>
       ���û������붨ʱ��
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
static void Hall_TimerConfig(void)
{
#ifdef  _STM32F4xx_
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef        TIM_ICInitStructure;
    TIM_OCInitTypeDef        TIM_OCInitStructure;
    GPIO_InitTypeDef         GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_Hall_TIM_GPIO, ENABLE); //��ʱ��
    RCC_APB1PeriphClockCmd(RCC_Hall_TIM, ENABLE);

//    GPIO_InitStructure.GPIO_Pin = Hall_TIM_Pin;//�����Ҫ��Ҫ����һ��2016/02/20 by Johnbee
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_Init(Hall_TIM_GPIO, &GPIO_InitStructure);

    TIM_DeInit(Hall_TIM);
    // timer base configuration
    // 126 => 3,5s till overflow ; 285,714kHz TimerClock [36MHz/Prescaler]
    TIM_TimeBaseStructure.TIM_Prescaler = 126;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(Hall_TIM, &TIM_TimeBaseStructure);

    // enable hall sensor
    // T1F_ED will be connected to  HallSensoren Imputs
    // TIM4_CH1,TIM4_CH2,TIM4_CH3
    TIM_SelectHallSensor(Hall_TIM, ENABLE);

    //  TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode,
    //  uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)

    // HallSensor event is delivered with singnal TI1F_ED
    // (this is XOR of the three hall sensor lines)
    // Signal TI1F_ED: falling and rising ddge of the inputs is used
    TIM_SelectInputTrigger(Hall_TIM, TIM_TS_TI1F_ED);

    // On every TI1F_ED event the counter is resetted and update is tiggered
    TIM_SelectSlaveMode(Hall_TIM, TIM_SlaveMode_Reset);

    // Channel 1 in input capture mode
    // on every TCR edge (build from TI1F_ED which is a HallSensor edge)
    // the timervalue is copied into ccr register and a CCR1 Interrupt
    // TIM_IT_CC1 is fired
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    // listen to T1, the  HallSensorEvent
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_TRC;
    // Div:1, every edge
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    // noise filter: 1111 => 72000kHz / factor (==1) / 32 / 8 -> 281kHz
    // input noise filter (reference manual page 322)
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(Hall_TIM, &TIM_ICInitStructure);

    // channel 2 can be use for commution delay between hallsensor edge
    // and switching the FET into the next step. if this delay time is
    // over the channel 2 generates the commutation signal to the motor timer
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 1; // 1 is no delay; 2000 = 7ms
    TIM_OC2Init(Hall_TIM, &TIM_OCInitStructure);

    // clear interrupt flag
    TIM_ClearFlag(Hall_TIM, TIM_FLAG_CC2);

    //TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
    // TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_OC1);
    // timer2 output compate signal is connected to TRIGO
    TIM_SelectOutputTrigger(Hall_TIM, TIM_TRGOSource_OC2Ref);
    // Enable channel 2 compate interrupt request
    // TIM_IT_CC1 | TIM_IT_CC2
    TIM_ITConfig(Hall_TIM, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

    // Enable output compare preload
    //TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    // Enable ARR preload
    //TIM_ARRPreloadConfig(TIM4, ENABLE);

    // Enable update event
    //TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    //TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);

    // we use preemption interrupts here,  BLDC Bridge switching and
    // Hall has highest priority
//    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

    // -------------------
    // HallSensor is now configured, if BLDC Timer is also configured
    // after enabling timer 4
    // the motor will start after next overflow of the hall timer because
    // this generates the first startup motor cummutation event
    TIM_Cmd(Hall_TIM, ENABLE);

#endif
}
/*
 *@ <function name=>TIM1_PWM_DutyChange() </function>
 *@ <summary>
     TIM1ͨ��PWM��ռ�ձȵ���
 *@ </summary>
 *@ <param name="myChannel">PWMͨ��1~4</param>
 *@ <param name="myDuty">ռ�ձ�,0~100</param>
 *@ <returns> null </returns>
 */
void TIM1_Set_PWMDuty(u8 myChannel, float myDuty)
{
    u16 tim1_perikd = TIM1_PWM_Period;
    u16 pulse = 0;
    if (myDuty < 0)
    {
        myDuty = 0;
    }
    else if (myDuty > 100)
    {
        myDuty = 100;
    }
    pulse = (u16)(tim1_perikd * myDuty / 100.0);
    switch (myChannel)
    {
        case 1:
            TIM_SetCompare1(TIM1, (u16)pulse);
            break;
        case 2:
            TIM_SetCompare2(TIM1, (u16)pulse);
            break;
        case 3:
            TIM_SetCompare3(TIM1, (u16)pulse);
            break;
        case 4:
            TIM_SetCompare4(TIM1, (u16)pulse);
            break;
    }
}
/*
 *@ <function name=>TIM1_Set_PWMOutput_EnableState() </function>
 *@ <summary>
     ͬʱ����PWM����ͨ�������ʹ��,��OSSR��OSSIΪͬʱΪDisableʱ
     ��ͬʱʧ������ͨ���ﵽ����ͨ��ͬʱ���Ϊ0������
 *@ </summary>
 *@ <param name="myChannel">PWMͨ��1~3</param>
 *@ <param name="pState">����ʹ�ܱ�־,0Ϊʧ��,!0Ϊʹ��</param>
 *@ <param name="nState">����ʹ�ܱ�־,0Ϊʧ��,!0Ϊʹ��</param>
 *@ <returns> null </returns>
 */
void TIM1_Set_PWMOutput_EnableState(u8 myChannel, u8 pState, u8 nState)
{
    u16 tim1_ccxp_state = TIM_CCx_Disable;
    u16 tim1_ccxn_state = TIM_CCxN_Disable;
    u16 tim_channel = TIM_Channel_1;
    if (pState)
    {
        tim1_ccxp_state = TIM_CCx_Enable;
    }
    if (nState)
    {
        tim1_ccxn_state = TIM_CCxN_Enable;
    }
    if (myChannel == 1)
    {
        tim_channel = TIM_Channel_1;
    }
    else if (myChannel == 2)
    {
        tim_channel = TIM_Channel_2;
    }
    else if (myChannel == 3)
    {
        tim_channel = TIM_Channel_3;
    }
    else
    {
        return;
    }
    TIM_CCxCmd(TIM1, tim_channel, tim1_ccxp_state);
    TIM_CCxNCmd(TIM1, tim_channel, tim1_ccxn_state);
}

/*
 *@ <function name=>TIM1_GPIO_Config() </function>
 *@ <summary>
       ���ö�ʱ��PWM�������
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
static void TIM1_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
#ifdef _STM32F4XX_
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE); //��ʱ��
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1); //���ö�ʱ��1���ù���
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1); //���ö�ʱ��1���ù���
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1); //���ö�ʱ��1���ù���

    //���TIM1PWM���=PA8:PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#ifdef _STM32F10x_
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); //��ʱ��
    //���TIM1PWM���=PA8:PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
