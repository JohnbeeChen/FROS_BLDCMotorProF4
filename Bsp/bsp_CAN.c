/*******************************************************************************************************
  * @FileName    bsp_CAN.c
  * @Author      Johnbee
  * @Version     V0.0.1
  * @Date        2015/11/23
  * @Brief       CAN���������ļ�  
  ******************************************************************************************************/
/*#include*********************************************************************************************/
#include "bsp_Init.h"

/*#define**********************************************************************************************/
#ifdef  _STM32F10x_
    #define USED_CAN                     CAN1
    #define RCC_USED_CAN                 RCC_APB1Periph_CAN1
    #define USED_CAN_Rx_Port             GPIOA
    #define USED_CAN_Rx_Pin              GPIO_Pin_11
    #define RCC_USED_CAN_Rx_Port         RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO
    #define USED_CAN_Tx_Port             GPIOA
    #define USED_CAN_Tx_Pin              GPIO_Pin_12
    #define RCC_USED_CAN_Tx_Port         RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO
#endif
#ifdef  _STM32F4xx_  //δ�޸�
    #define USED_CAN                     CAN1
    #define RCC_USED_CAN                 RCC_APB1Periph_CAN1
    #define USED_CAN_Rx_Port             GPIOB
    #define USED_CAN_Rx_Pin              GPIO_Pin_8
    #define USED_CAN_Rx_PinSource        GPIO_PinSource8
    #define USED_CAN_Rx_AF               GPIO_AF_CAN1    
    #define RCC_USED_CAN_Rx_Port         RCC_AHB1Periph_GPIOB
    #define USED_CAN_Tx_Port             GPIOB
    #define USED_CAN_Tx_Pin              GPIO_Pin_9
    #define USED_CAN_Tx_PinSource        GPIO_PinSource9
    #define USED_CAN_Tx_AF               GPIO_AF_CAN1   
    #define RCC_USED_CAN_Tx_Port         RCC_AHB1Periph_GPIOA
#endif

/*ȫ�ֱ�������**************************************************************************************************/
u8 CAN1_TCP_Flag = 0;  //CAN1������ɱ�־λ
u8 CAN2_TCP_Flag = 0;  //CAN2������ɱ�־λ

union float_trans
{
	s32  s32_form;
	float float_form;
	unsigned char uchar_form[4];
};
/*
 *@<function name="">bsp_CAN1_Init()</function>
 *@<summary>
      CAN��������
 *@</summary>
 *@<param name="void"> null </param>
 *@<returns> NULL </returns>
*/
void Bsp_CAN_Init(void)
{
	My_CAN_Init(USED_CAN);
//	My_CAN_Init(CAN2);
}
void CAN1_RX_NVIC(u8 preemPriority,u8 subPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);  
}

/*
 *@<function name="">CAN1_RX0_IRQPendle()</function>
 *@<summary>
      CAN1�ж��Ӻ���,��stm32f10x_it.c��stm32f4xx_it.c�е���
 *@</summary>
 *@<param name="void"> null </param>
 *@<returns> NULL </returns>
*/
void CAN1_RX0_IRQPendle(void)
{
}
void CAN2_RX0_IRQPendle(void)
{
}
/*
 *@<function name="">My_CAN_Init()</function>
 *@<summary>
      CAN���ߵײ�����,�ڲ�����
 *@</summary>
 *@<param name="CANx"> CAN1��CAN2
    
   </param>
 *@<returns> NULL </returns>
*/
static void My_CAN_Init(CAN_TypeDef* CANx)
{
#ifdef _STM32F10x_
    GPIO_InitTypeDef       GPIO_Structure;
    CAN_InitTypeDef        CAN_Structure;
    CAN_FilterInitTypeDef  Can_Filter_Structure;
    RCC_APB2PeriphClockCmd( RCC_USED_CAN_Rx_Port|RCC_USED_CAN_Tx_Port, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_USED_CAN,  ENABLE);

    GPIO_Structure.GPIO_Pin = USED_CAN_Rx_Pin;
    GPIO_Structure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Structure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(USED_CAN_Rx_Port,&GPIO_Structure);
    GPIO_Structure.GPIO_Pin = USED_CAN_Tx_Pin;
    GPIO_Init(USED_CAN_Tx_Port,&GPIO_Structure);
    CAN_DeInit(CANx);
    CAN_Structure.CAN_TTCM = DISABLE;
    CAN_Structure.CAN_ABOM = ENABLE;
    CAN_Structure.CAN_AWUM = DISABLE;
    CAN_Structure.CAN_NART = DISABLE;
    CAN_Structure.CAN_RFLM = DISABLE;
    CAN_Structure.CAN_TXFP = ENABLE;
    CAN_Structure.CAN_Mode = CAN_Mode_Normal;
    CAN_Structure.CAN_SJW = CAN_SJW_1tq;
    CAN_Structure.CAN_BS1 = CAN_BS1_7tq;
    CAN_Structure.CAN_BS2 = CAN_BS2_4tq;
    CAN_Structure.CAN_Prescaler = 3; //CAN BaudRate 36/(1+7+4)/3=1Mbps
    CAN_Init(CANx,&CAN_Structure);
    Can_Filter_Structure.CAN_FilterNumber = 0;
    Can_Filter_Structure.CAN_FilterMode = CAN_FilterMode_IdMask;
    Can_Filter_Structure.CAN_FilterScale = CAN_FilterScale_32bit;
    Can_Filter_Structure.CAN_FilterIdHigh = 0;
    Can_Filter_Structure.CAN_FilterIdLow = 0;
    Can_Filter_Structure.CAN_FilterMaskIdHigh = 0;
    Can_Filter_Structure.CAN_FilterMaskIdLow = 0;
    Can_Filter_Structure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    Can_Filter_Structure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&Can_Filter_Structure);
    CAN_ITConfig(CANx,CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE); // fifo0�ж�
    CAN_ITConfig(CANx,CAN_IT_TME, ENABLE); // �����ж�   
#endif
#ifdef _STM32F4xx_
    GPIO_InitTypeDef       Gpio_Structure;
    CAN_InitTypeDef        Can_Structure;
    CAN_FilterInitTypeDef  Can_filter_Structure;
	
    if (CANx == USED_CAN)
    {
        RCC_AHB1PeriphClockCmd(RCC_USED_CAN_Rx_Port|RCC_USED_CAN_Tx_Port, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_USED_CAN, ENABLE);

        GPIO_PinAFConfig(USED_CAN_Rx_Port, USED_CAN_Rx_PinSource, USED_CAN_Rx_AF);        //CAN1 PB9  PB8 ����
        GPIO_PinAFConfig(USED_CAN_Tx_Port, USED_CAN_Tx_PinSource, USED_CAN_Tx_AF);        //CAN1 PB9  PB8 ����

        Gpio_Structure.GPIO_Pin = USED_CAN_Rx_Pin;
        Gpio_Structure.GPIO_Mode = GPIO_Mode_AF;
        Gpio_Structure.GPIO_OType = GPIO_OType_PP;
        Gpio_Structure.GPIO_PuPd = GPIO_PuPd_DOWN,
        Gpio_Structure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(USED_CAN_Rx_Port, &Gpio_Structure);
        Gpio_Structure.GPIO_Pin = USED_CAN_Tx_Pin;
        GPIO_Init(USED_CAN_Tx_Port, &Gpio_Structure);
		
		CAN_DeInit(CANx);
		CAN_StructInit(&Can_Structure);
		Can_Structure.CAN_TTCM = DISABLE;               //��ʱ�䴥��ͨ��ģʽ
		Can_Structure.CAN_ABOM = ENABLE;                //ʧ������Զ����߹���
		Can_Structure.CAN_AWUM = DISABLE;              //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
		Can_Structure.CAN_NART = DISABLE;               //�����Զ�����
		Can_Structure.CAN_RFLM = DISABLE;               //FIFO������,�±��ĸ��Ǿɱ���
		Can_Structure.CAN_TXFP = ENABLE;                //FIFO���ȼ��ɷ�������˳�����
		Can_Structure.CAN_Mode = CAN_Mode_Normal;       //ģʽ����
//		Can_Structure.CAN_Mode = CAN_Mode_LoopBack;       //ģʽ����

		Can_Structure.CAN_SJW  = CAN_SJW_1tq;           //����ͬ����Ծʱ�䵥Ԫ��0-3
		Can_Structure.CAN_BS1 = CAN_BS1_9tq;            //ʱ���1��ʱ�䵥Ԫ��0-15
		Can_Structure.CAN_BS2 = CAN_BS2_4tq;            //ʱ���F2��ʱ�䵥Ԫ��0-7
		Can_Structure.CAN_Prescaler = 3;               //PCLK1=42M��CAN BaudRate 42/(1+9+4)/3=1Mbps
//		Can_Structure.CAN_BS1 = CAN_BS1_9tq;            //ʱ���1��ʱ�䵥Ԫ��0-15
//		Can_Structure.CAN_BS2 = CAN_BS2_5tq;            //ʱ���F2��ʱ�䵥Ԫ��0-7
//		Can_Structure.CAN_Prescaler = 3;               //PCLK1=45M��CAN BaudRate 45/(1+9+5)/3=1Mbps
		
		CAN_Init(CANx, &Can_Structure);
			
		Can_filter_Structure.CAN_FilterNumber = 0;
		Can_filter_Structure.CAN_FilterMode = CAN_FilterMode_IdMask;
		Can_filter_Structure.CAN_FilterScale = CAN_FilterScale_32bit;
		Can_filter_Structure.CAN_FilterIdHigh = 0x0000;
		Can_filter_Structure.CAN_FilterIdLow = 0x0000;
		Can_filter_Structure.CAN_FilterMaskIdHigh = 0x0000;
		Can_filter_Structure.CAN_FilterMaskIdLow = 0x0000;
		Can_filter_Structure.CAN_FilterFIFOAssignment = 0;
		Can_filter_Structure.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&Can_filter_Structure);
		CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
		CAN_ITConfig(CANx,CAN_IT_TME,ENABLE);

    }
    else if (CANx == CAN2)
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);

        GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,GPIO_AF_CAN2);        //CAN2 PB6 PB5 ����
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);

        Gpio_Structure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
        Gpio_Structure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_Init(GPIOB, &Gpio_Structure);
        
//        NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init(&NVIC_InitStructure);
    
		CAN_DeInit(CANx);
		CAN_StructInit(&Can_Structure);
		Can_Structure.CAN_TTCM = DISABLE;               //��ʱ�䴥��ͨ��ģʽ
		Can_Structure.CAN_ABOM = ENABLE;                //ʧ������Զ����߹���
		Can_Structure.CAN_AWUM = DISABLE;              //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
		Can_Structure.CAN_NART = DISABLE;               //�����Զ�����
		Can_Structure.CAN_RFLM = DISABLE;               //FIFO������,�±��ĸ��Ǿɱ���
		Can_Structure.CAN_TXFP = ENABLE;                //FIFO���ȼ��ɷ�������˳�����
		Can_Structure.CAN_Mode = CAN_Mode_Normal;       //ģʽ����
//		Can_Structure.CAN_Mode = CAN_Mode_LoopBack;       //ģʽ����

		Can_Structure.CAN_SJW  = CAN_SJW_1tq;           //����ͬ����Ծʱ�䵥Ԫ��0-3
		Can_Structure.CAN_BS1 = CAN_BS1_9tq;            //ʱ���1��ʱ�䵥Ԫ��0-15
		Can_Structure.CAN_BS2 = CAN_BS2_4tq;            //ʱ���F2��ʱ�䵥Ԫ��0-7
		Can_Structure.CAN_Prescaler = 3;               //PCLK1=42M��CAN BaudRate 42/(1+9+4)/3=1Mbps
//		Can_Structure.CAN_BS1 = CAN_BS1_9tq;            //ʱ���1��ʱ�䵥Ԫ��0-15
//		Can_Structure.CAN_BS2 = CAN_BS2_5tq;            //ʱ���F2��ʱ�䵥Ԫ��0-7
//		Can_Structure.CAN_Prescaler = 3;               //PCLK1=45M��CAN BaudRate 45/(1+9+5)/3=1Mbps
		
		CAN_Init(CANx, &Can_Structure);
		Can_filter_Structure.CAN_FilterNumber = 14;
		Can_filter_Structure.CAN_FilterMode = CAN_FilterMode_IdMask;
		Can_filter_Structure.CAN_FilterScale = CAN_FilterScale_32bit;
		Can_filter_Structure.CAN_FilterIdHigh = 0x0000;
		Can_filter_Structure.CAN_FilterIdLow = 0x0000;
		Can_filter_Structure.CAN_FilterMaskIdHigh = 0x0000;
		Can_filter_Structure.CAN_FilterMaskIdLow = 0x0000;
		Can_filter_Structure.CAN_FilterFIFOAssignment = 0;
		Can_filter_Structure.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&Can_filter_Structure);
		CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
		CAN_ITConfig(CANx,CAN_IT_TME,ENABLE);
	}
#endif
}

/*
 *@<function name="">CAN1_TX_IRQPendler()</function>
 *@<summary>
      CAN1��������ж��Ӻ���,��stm32f4xx_it.c�е���
 *@</summary>
 *@<param name="void">null</param>
 *@<returns> NULL </returns>
*/
void CAN1_TX_IRQPendler(void)
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
	   CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
       CAN1_TCP_Flag = 1;   //�������
     }
	
}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
