/********************************************************************************
 *@FileName    bsp_Bluetooth.h
 *@Author      Johnbee
 *@Version     V0.2
 *@Date        2016/02/28
 *@Brief       ��������,ȫ���ĳɵ�λ��ǰ���շ�ģʽ��
 *******************************************************************************/
/*#includes*********************************************************************/
#include "bsp_Bluetooth.h"

/*#define***********************************************************************/
#ifdef  _STM32F10x_
    #define BLE_USART                  USART3
    #define BLE_USART_IRQn             USART3_IRQn
    #define RCC_BLE_USART              RCC_APB1Periph_USART3
    #define BLE_Rx_Port                GPIOB
    #define BLE_Rx_Pin                 GPIO_Pin_11
    #define RCC_BLE_Rx_Port            RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
    #define BLE_Tx_Port                GPIOB
    #define BLE_Tx_Pin                 GPIO_Pin_10
    #define RCC_BLE_Tx_Port            RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO  
#endif
#ifdef  _STM32F4xx_
    #define BLE_USART                  UART5
    #define BLE_USART_IRQn             UART5_IRQn
    #define BLE_AF_USART               GPIO_AF_UART5
    #define RCC_BLE_USART              RCC_APB1Periph_UART5
    
    #define BLE_Rx_Port                GPIOD
    #define BLE_Rx_Pin                 GPIO_Pin_2
    #define BLE_Rx_PinSource           GPIO_PinSource2
    #define RCC_BLE_Rx_Port            RCC_AHB1Periph_GPIOD
    
    #define BLE_Tx_Port                GPIOC
    #define BLE_Tx_Pin                 GPIO_Pin_12
    #define BLE_Tx_PinSource           GPIO_PinSource12
    #define RCC_BLE_Tx_Port            RCC_AHB1Periph_GPIOC 
#endif

/*ȫ�ֱ�������******************************************************************/
float ComData[MyComId_Num] = {0}; //������λ�����ݵ�����

USART_QUEUE BLE_USART_QUEUE;

typedef union
{
	s32 myvalue;
	u8 mychar[4]; //��λ��ǰ
}MyUion;
/*
 *@ <function name=>BlueToothInit() </function>
 *@ <summary>
      ������ʼ��
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void BlueToothInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
#ifdef  _STM32F4xx_
    /* config USART3 clock */
    RCC_AHB1PeriphClockCmd(RCC_BLE_Tx_Port|RCC_BLE_Rx_Port, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_BLE_USART, ENABLE);

    /* USART3 GPIO config */
    /* Configure USART Rx (PB11) as input floating */
    GPIO_InitStructure.GPIO_Pin = BLE_Rx_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(BLE_Rx_Port, &GPIO_InitStructure);
    /* Configure USART Tx (PB10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = BLE_Tx_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(BLE_Tx_Port, &GPIO_InitStructure);

    GPIO_PinAFConfig(BLE_Rx_Port, BLE_Rx_PinSource, BLE_AF_USART);
    GPIO_PinAFConfig(BLE_Tx_Port, BLE_Tx_PinSource, BLE_AF_USART);

    /* USART mode config */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(BLE_USART, &USART_InitStructure);
    USART_ITConfig(BLE_USART, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(BLE_USART, USART_FLAG_TXE, ENABLE);

    USART_Cmd(BLE_USART, ENABLE);
//    USART_ClearFlag(BLE_USART, USART_FLAG_TC);
#endif
#ifdef  _STM32F10x_
    RCC_APB2PeriphClockCmd(RCC_BLE_Rx_Port|RCC_BLE_Tx_Port, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_BLE_USART, ENABLE);

    /* USART3 GPIO config */
    /* Configure USART3 Rx (PB11) as input floating */
    GPIO_InitStructure.GPIO_Pin = BLE_Rx_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BLE_Rx_Port, &GPIO_InitStructure);
    /* Configure USART3 Tx (PB10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = BLE_Tx_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BLE_Tx_Port, &GPIO_InitStructure);

    /* USART3 mode config */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(BLE_USART, &USART_InitStructure);
    USART_ITConfig(BLE_USART, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(BLE_USART, USART_FLAG_TXE, ENABLE);

    USART_Cmd(BLE_USART, ENABLE);
    USART_ClearFlag(BLE_USART, USART_FLAG_TC);

#endif
}
/*
 *@ <function name=>BLE_NVIC_Config() </function>
 *@ <summary>
      ���������ж�����
 *@ </summary>
 *@ <param name="preemPriority">��ռ���ȼ�</param>
 *@ <param name="subPriority">�������ȼ�</param>
 *@ <returns> null </returns>
 */
void BLE_NVIC_Config(u8 preemPriority,u8 subPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = BLE_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);  
}

/*
 *@ <function name=>Send_Array() </function>
 *@ <summary>
      ���ڷ�������
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void Send_Array(u8 * Array, u16 Len)
{
    u16 i = 0;
    u8 temp = 0;
    for (; i < Len; i++)
    {
        temp = *Array++;
        USART_SendData(BLE_USART, temp);
        while (USART_GetFlagStatus(BLE_USART, USART_FLAG_TC) == RESET);
    }
}
/*
 *@ <function name=>Send_Data() </function>
 *@ <summary>
      ���ڷ������ݵ���λ��
 *@ </summary>
 *@ <param name="myValue">Ҫ���͵�����</param>
 *@ <param name="myUUID">Ҫ���͵�����UUID</param>
 *@ <returns> null </returns>
 */
void Send_Data(float myValue, uint8_t myUUID)
{
#if 0  //ֱ�ӷ���
    s32 num;
    static u8 SEND_BUF[8] = {0x55, 0xaa,0, 0, 0, 0, 0, 0xff};
	num = (s32)(myValue*100);
	SEND_BUF[2] = myUUID;
    SEND_BUF[3] = (num)&0xff;/*��λ��ǰ*/
    SEND_BUF[4] = (num>>8)&0xff;   
    SEND_BUF[5] = (num>>16)&0xff;
    SEND_BUF[6] = (num>>24)&0xff;
    
    Send_Array(SEND_BUF, 8);
#endif
#if 1  //��ӵ����Ͷ���
	u16 temp_rear;
    s32 num;
	num = (s32)(myValue*100);	
	temp_rear = BLE_USART_QUEUE.Rear + 1;  //��βָ���1
	if(temp_rear >= USART_SendBuf_Size)
	{
		temp_rear = 0;
	}
	if(temp_rear == BLE_USART_QUEUE.Front)
	{
        temp_rear = temp_rear;
		return ; //����������
	}
	BLE_USART_QUEUE.USART_BUF[BLE_USART_QUEUE.Rear].DATA[0] = 0x55;
	BLE_USART_QUEUE.USART_BUF[BLE_USART_QUEUE.Rear].DATA[1] = 0xaa;
	BLE_USART_QUEUE.USART_BUF[BLE_USART_QUEUE.Rear].DATA[2] = myUUID;
	BLE_USART_QUEUE.USART_BUF[BLE_USART_QUEUE.Rear].DATA[3] = (num)&0xff;/*��λ��ǰ*/
	BLE_USART_QUEUE.USART_BUF[BLE_USART_QUEUE.Rear].DATA[4] = (num>>8)&0xff;
	BLE_USART_QUEUE.USART_BUF[BLE_USART_QUEUE.Rear].DATA[5] = (num>>16)&0xff;
	BLE_USART_QUEUE.USART_BUF[BLE_USART_QUEUE.Rear].DATA[6] = (num>>24)&0xff;    
	BLE_USART_QUEUE.USART_BUF[BLE_USART_QUEUE.Rear].DATA[7] = 0xff;
    
    BLE_USART_QUEUE.Rear++;  //��Ч���ݼ�1
    if(BLE_USART_QUEUE.Rear >= USART_SendBuf_Size)	
	{
		BLE_USART_QUEUE.Rear = 0;  //ѭ������
	}
#endif
}

/*
 *@ <function name=>BLE_IRQPandler() </function>
 *@ <summary>
      ���������ж��Ӻ�������stm32f10x_it.c�е�����
 *@ </summary>
 *@ <param name="void">null</param>
 *@ <returns> null </returns>
 */
void BLE_IRQPandler(void)
{
    static u8 ii = 2;
	static u8 USART[7];

	MyUion my_uinon;
    if (USART_GetFlagStatus(BLE_USART, USART_FLAG_RXNE) == SET)
    {
        USART_ClearFlag(BLE_USART, USART_FLAG_RXNE);
        if (ii == 2)
        {
            USART[0] = USART[1];
            USART[1] = USART[2];
        }
        *(USART + ii) = USART_ReceiveData(BLE_USART);	
        if (USART[0] == 0xaa && USART[1] == 0x55)
		{
			ii++;
			if( ii == 7)
			{
				my_uinon.mychar[0] = USART[2];  //stm32Ĭ��С��ģʽ
				my_uinon.mychar[1] = USART[3];
				my_uinon.mychar[2] = USART[4];
				my_uinon.mychar[3] = USART[5];  //��λ�ں�	
                				
                if(USART[6]<MyComId_Num)	
                {					
				    ComData[USART[6]] = my_uinon.myvalue/100.0;
				}
				ii = 2;
			}
		}		
	}
}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */

