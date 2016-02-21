/********************************************************************************
 *@FileName    bsp_init.c
 *@Author      Johnbee
 *@Version     V0.1
 *@Date        2015/10/02
 *@Brief       ���ڸ���Ӳ����ʼ��
 *******************************************************************************/
/*#includes*********************************************************************/
#include "bsp_init.h"

/*#define***********************************************************************/
#ifdef  _STM32F10x_
#define KeyA_Port              GPIOC
#define KeyA_Pin               GPIO_Pin_13
#define KeyA_Exit_Line         EXTI_Line13
#define KeyA_GPIO_PinSource    GPIO_PinSource13
#define KeyA_GPIO_PortSource   GPIO_PortSourceGPIOC
#define RCC_KeyA_Port          RCC_APB2Periph_GPIOC// | RCC_APB2Periph_AFIO

//#define KeyB_Port              GPIOA
//#define KeyB_Pin               GPIO_Pin_5
//#define KeyB_Exit_Line         EXTI_Line5
//#define KeyB_GPIO_PinSource    GPIO_PinSource5
//#define KeyB_GPIO_PortSource   GPIO_PortSourceGPIOA
//#define RCC_KeyB_Port          RCC_APB2Periph_GPIOA // | RCC_APB2Periph_AFIO

//#define KeyC_Port              GPIOA
//#define KeyC_Pin               GPIO_Pin_6
//#define KeyC_Exit_Line         EXTI_Line6
//#define KeyC_GPIO_PinSource    GPIO_PinSource6
//#define KeyC_GPIO_PortSource   GPIO_PortSourceGPIOA
//#define RCC_KeyC_Port          RCC_APB2Periph_GPIOA  //| RCC_APB2Periph_AFIO
#endif
#ifdef  _STM32F4xx_

#endif


/*ȫ�ֱ�������******************************************************************/


/********************************************************************************
 *@��������    BSP_Init()
 *@��������    ����Ӳ�����úͳ�ʼ��
 *@��ڲ���    ��
 *@���ڲ���    ��
 *******************************************************************************/
void Bsp_Init(void)
{

    TIM1_PWM_Config();
    BlueToothInit();
    Hall_GPIO_Init();
    Bsp_ADC_Init();
    Bsp_Encoder_Config();
//    Bsp_CAN_Init();

    Bsp_NVIC_Config();/*�ж�����*/
    
    Hall_PrepareCommutation();//Ԥ����

}
/********************************************************************************
 *@��������    Bsp_NVIC_Config()
 *@��������    �ж�����������
 *@��ڲ���    ��
 *@���ڲ���    ��
 *@˵    ��    ���еĹ��̵��ж϶�Ҫ�ڴ�����,
               Ϊ��ʵ�ֶ��˺�����д��,�Ƽ���ȡ���µ�ʾ��д��
 *******************************************************************************/
void Bsp_NVIC_Config(void)
{

    /*�жϷ�������*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    EXTI9_5_NVIC_Config(0,0);
	EXTI4_NVIC_Config(1,0);
    BLE_NVIC_Config(2,0);
}

/********************************************************************************
 *@��������    SysTick_init()
 *@��������    ��ʼ��SysTick��ʱ��
 *@��ڲ���    ��
 *@���ڲ���    ��
 *******************************************************************************/
void SysTick_init(void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks); /*��ȡϵͳ��Ƶ*/
    /*systick���жϺ����������л������*/
//    SysTick_Config(rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC);//��ʼ����ʹ��SysTick��ʱ��
}/*
 *@ <function name=>Key_GPIO_Init() </function>
 *@ <summary>
     config the gpio for the presskey
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void Key_GPIO_Init(void)
{

#ifdef  _STM32F10x_
    EXTI_InitTypeDef EXTI_InitStructure;
     GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_KeyA_Port , ENABLE);

    GPIO_InitStructure.GPIO_Pin = KeyA_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	 // ��������
    GPIO_Init(KeyA_Port, &GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Pin = KeyB_Pin;
//    GPIO_Init(KeyB_Port, &GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Pin = KeyC_Pin;
//    GPIO_Init(KeyC_Port, &GPIO_InitStructure);

    /* EXTI line mode config */
    GPIO_EXTILineConfig(KeyA_GPIO_PortSource, KeyA_GPIO_PinSource);
    EXTI_InitStructure.EXTI_Line = KeyA_Exit_Line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�������ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

//    GPIO_EXTILineConfig(KeyB_GPIO_PortSource, KeyB_GPIO_PinSource);
//    EXTI_InitStructure.EXTI_Line = KeyB_Exit_Line;
//    EXTI_Init(&EXTI_InitStructure);
//    GPIO_EXTILineConfig(KeyC_GPIO_PortSource, KeyC_GPIO_PinSource);
//    EXTI_InitStructure.EXTI_Line = KeyC_Exit_Line;
//    EXTI_Init(&EXTI_InitStructure);
#endif

#ifdef _STM32F4xx_
    //��ûд
#endif
}
/*
 *@ <function name=>EXTI15_10_NVIC_Config() </function>
 *@ <summary>
      �����ж�����
 *@ </summary>
 *@ <param name="preemPriority">��ռ���ȼ�</param>
 *@ <param name="subPriority">�������ȼ�</param>
 *@ <returns> null </returns>
 */
void EXTI15_10_NVIC_Config(u8 preemPriority, u8 subPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 *@ <function name=>EXTI15_10_IRQPander() </function>
 *@ <summary>
      �����ж��Ӻ���,��stm32f10x_it.c�е���
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void EXTI15_10_IRQPander(void)
{
//    if (EXTI_GetITStatus(KeyA_Exit_Line) != RESET) //ȷ���Ƿ������EXTI Line�ж�
//    {
//        EXTI_ClearITPendingBit(KeyA_Exit_Line);     //����жϱ�־λ
//        if(ComData[0] == 0 )
//        {
//            ComData[0] = 200;           
//        }
//        else
//        {
//            ComData[0] = 0;                       
//        }
//    }

}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
