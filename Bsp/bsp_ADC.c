/*********************************************************************************
 *@FileName   bsp_ADC.c
 *@Version    V0.0.0.2
 *@Author     Johnbee
 *@Date       2016/02/28
 *@Brief      ADC����,ch0-C�෴�綯�� ch1-B�� ch2-A��  CH3-���ߵ���
 *********************************************************************************/
/*#include************************************************************************/
#include "bsp_ADC.h"

/*#define*************************************************************************/
#ifdef  _STM32F10x_
    #define ADC_USED              ADC1
    #define ADC_USED_Channel      ADC_Channel_9
    #define RCC_ADC_USED          RCC_APB2Periph_ADC1
    #define ADC_USED_Port         GPIOB
    #define ADC_USED_Pin          GPIO_Pin_1
    #define RCC_ADC_USED_Port     RCC_APB2Periph_GPIOB//| RCC_APB2Periph_AFIO
    #define ADC_USED_DR           (uint32_t)(&ADC_USED->DR)
    #define ADC_DMA_Channel       DMA1_Channel1
    #define RCC_ADC_DMA           RCC_AHBPeriph_DMA1
#endif
#ifdef  _STM32F4xx_  //
    #define Phase_ADC                   ADC1 /*�ɼ��෴�綯��*/
    #define RCC_Phase_ADC               RCC_APB2Periph_ADC1
    #define Phase_ADC_Port              GPIOA
    #define Phase_ADC_Pin               GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2
    #define RCC_Phase_ADC_Port          RCC_AHB1Periph_GPIOA
    #define Phase_ADC_Addr              (uint32_t)(&Phase_ADC->DR)
    #define Phase_ADC_DMA_Stream        DMA2_Stream0
    #define Phase_ADC_DMA_Channel       DMA_Channel_0
    #define Phase_ADC_DMA_IRQn          DMA2_Stream0_IRQn
    #define RCC_Phase_ADC_DMA           RCC_AHB1Periph_DMA2   

    #define Current_ADC                   ADC2/*�ɼ�������ADC*/
    #define RCC_Current_ADC               RCC_APB2Periph_ADC2
    #define Current_ADC_Port              GPIOA
    #define Current_ADC_Pin               GPIO_Pin_3
    #define RCC_Current_ADC_Port          RCC_AHB1Periph_GPIOA
    #define Current_ADC_Addr              (uint32_t)(&Current_ADC->DR)
    #define Current_ADC_DMA_Stream        DMA2_Stream2
    #define Current_ADC_DMA_Channel       DMA_Channel_1
    #define Current_ADC_DMA_IRQn          DMA2_Stream2_IRQn
    #define RCC_Current_ADC_DMA           RCC_AHB1Periph_DMA2   

#endif
/*ȫ�ֱ�������********************************************************************/
uint16_t  ADC_USED_ConvertedValue1[ADC_Converted_Num1];//������1
uint16_t  ADC_USED_ConvertedValue2[ADC_Converted_Num2];//������2

float Current_Value = 0;
float Current_Zero = 0;   //�������
float Voltag_Zero  = 0;   //��ѹ���
s32   Current_AD_Value = 0;
u8    Current_CalibrateState = 0;  //����ʱ���������У׼��־
u8    DMA_TC_Flag = 0;//DMA������ɱ�־
u8    DMA_HT_Flag = 0; //DMA�봫����ɱ�־
/*
 *@ <function name=>Bsp_ADC_Init() </function>
 *@ <summary>
    ADC����
 *@ </summary>
 *@ <param name="xxx"></param>
 *@ <returns> </returns>
*/
void Bsp_ADC_Init(void)
{
    Phase_ADC_GPIO_Config();
    Current_ADC_GPIO_Config();
    
    Phase_ADC_DMA_Config();
    Current_ADC_DMA_Config();      
    
    Phase_ADC_Config();
    Current_ADC_Config(); 
    ADC_Start();    
}
/*�������ADC*/
static void ADC_Start(void)
{
    ADC_SoftwareStartConv(Phase_ADC);
    ADC_SoftwareStartConv(Current_ADC);    
}
/*
 *@ <function name=>Phase_ADC_GPIO_Config() </function>
 *@ <summary>
    ADC��������
 *@ </summary>
 *@ <param name="void"></param>
 *@ <returns> </returns>
*/
static void Phase_ADC_GPIO_Config(void)
{
#ifdef _STM32F10x_
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_ADC_USED_Port, ENABLE);

    /* Configure ADC3 Channel12 pin as analog input*/
    GPIO_InitStructure.GPIO_Pin = ADC_USED_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   
    GPIO_Init(ADC_USED_Port, &GPIO_InitStructure);
#endif
#ifdef _STM32F4xx_
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_Phase_ADC_Port, ENABLE);

    /* Configure ADC Channel pin as analog input*/
    GPIO_InitStructure.GPIO_Pin = Phase_ADC_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    
    GPIO_Init(Phase_ADC_Port, &GPIO_InitStructure);
#endif
}
/*
 *@ <function name=>Current_ADC_GPIO_Config() </function>
 *@ <summary>
    ADC��������
 *@ </summary>
 *@ <param name="void"></param>
 *@ <returns> </returns>
*/
static void Current_ADC_GPIO_Config(void)
{
#ifdef _STM32F4xx_
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_Current_ADC_Port, ENABLE);

    /* Configure ADC Channel pin as analog input*/
    GPIO_InitStructure.GPIO_Pin = Current_ADC_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    
    GPIO_Init(Current_ADC_Port, &GPIO_InitStructure);
    
#endif
}

/*
 *@ <function name=>Phase_ADC_Config() </function>
 *@ <summary>
    ADC����
 *@ </summary>
 *@ <param name="void"></param>
 *@ <returns> </returns>
*/
static void Phase_ADC_Config(void)
{
#ifdef _STM32F10x_
	ADC_InitTypeDef ADC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_ADC_USED, ENABLE);

    ADC_DeInit( ADC_USED );
	/* ADC1 configuration */	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 				//��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 								//Ҫת����ͨ����Ŀ1
	ADC_Init(ADC_USED, &ADC_InitStructure);
	
	/*����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*����ADC1��ͨ��11Ϊ55.	5���������ڣ�����Ϊ1 */ 
	ADC_RegularChannelConfig(ADC_USED, ADC_USED_Channel, 1, ADC_SampleTime_55Cycles5);	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC_USED, ENABLE);	
	/* Enable ADC1 */
	ADC_Cmd(ADC_USED, ENABLE);	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC_USED);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC_USED));	
	/* ADCУ׼ */
	ADC_StartCalibration(ADC_USED);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC_USED));
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC_USED, ENABLE);
    
#endif
#ifdef _STM32F4xx_

    ADC_InitTypeDef  ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_Phase_ADC, ENABLE);
//    ADC_DeInit();
    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//4��Ƶ
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//�ر�DMA��ȡ
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_9Cycles;//�������
    ADC_CommonInit(&ADC_CommonInitStructure);
    /* ADC Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λת����ʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ģʽ 
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 3;//��ͨ��ת��ʱӦ�޸Ĵ˴�        
    ADC_Init(Phase_ADC, &ADC_InitStructure);
    
    /* ADC regular channel12 configuration *************************************/
    ADC_RegularChannelConfig(Phase_ADC, ADC_Channel_0, 1, ADC_SampleTime_84Cycles);//����ADC����ͨ������
    ADC_RegularChannelConfig(Phase_ADC, ADC_Channel_1, 2, ADC_SampleTime_84Cycles);//����ADC����ͨ������
    ADC_RegularChannelConfig(Phase_ADC, ADC_Channel_2, 3, ADC_SampleTime_84Cycles);//����ADC����ͨ������
    /*����ͨ������Ƶ�� = (APB2clk/ADC_Prescaler_Div4)/(����ת��ͨ����*(ADC_TwoSamplingDelay_9Cycles + ADC_SampleTime_84Cycles + ת������))*/
    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(Phase_ADC, ENABLE);//ADCת����ɺ�������//��DMA����
    /* Enable ADC DMA */
    ADC_DMACmd(Phase_ADC, ENABLE);
    /* Enable ADC*/
    ADC_Cmd(Phase_ADC, ENABLE);     
    //�������ADC  
//    ADC_SoftwareStartConv(Phase_ADC);
#endif    
}
/*
 *@ <function name=>Current_ADC_Config() </function>
 *@ <summary>
    ADC ����
 *@ </summary>
 *@ <param name="xxx"></param>
 *@ <returns> </returns>
*/
static void Current_ADC_Config(void)
{
#ifdef _STM32F4xx_
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_Current_ADC, ENABLE);
//    ADC_DeInit();
    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//4��Ƶ
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//�ر�DMA��ȡ
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_9Cycles;//�������
    ADC_CommonInit(&ADC_CommonInitStructure);
    /* ADC Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λת����ʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ģʽ 
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;//��ͨ��ת��ʱӦ�޸Ĵ˴�    
    ADC_Init(Current_ADC, &ADC_InitStructure);
    
    /* ADC regular channel12 configuration *************************************/
    ADC_RegularChannelConfig(Current_ADC, ADC_Channel_3, 1, ADC_SampleTime_84Cycles);//����ADC����ͨ������
    /*����ͨ������Ƶ�� = (APB2clk/ADC_Prescaler_Div4)/(����ת��ͨ����*(ADC_TwoSamplingDelay_9Cycles + ADC_SampleTime_84Cycles + ת������))*/
    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(Current_ADC, ENABLE);//ADCת����ɺ�������//��DMA����  
    /* Enable ADC DMA */
    ADC_DMACmd(Current_ADC, ENABLE);  
    /* Enable ADC*/
    ADC_Cmd(Current_ADC, ENABLE);     
    //�������ADC  
//    ADC_SoftwareStartConv(Current_ADC);       
    
#endif        
}
/*
 *@ <function name=>Phase_ADC_DMA_Config() </function>
 *@ <summary>
    ADC DMA����
 *@ </summary>
 *@ <param name="xxx"></param>
 *@ <returns> </returns>
*/
static void Phase_ADC_DMA_Config(void)
{
#ifdef _STM32F10x_
	DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_ADC_DMA, ENABLE);

	/* DMA channel1 configuration */
	DMA_DeInit(ADC_DMA_Channel);	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_USED_DR;	 			//ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_USED_ConvertedValue1;	//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = ADC_Converted_Num;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				//�ڴ��ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										//ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(ADC_DMA_Channel, &DMA_InitStructure);
//    DMA_ITConfig(ADC_DMA_Channel,DMA_IT_TC,ENABLE);
	
	/* Enable DMA channel1 */
	DMA_Cmd(ADC_DMA_Channel, ENABLE);
    
#endif
#ifdef _STM32F4xx_
    DMA_InitTypeDef DMA_InitStructure;
    /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
    RCC_AHB1PeriphClockCmd(RCC_Phase_ADC_DMA, ENABLE);    
    DMA_DeInit(Phase_ADC_DMA_Stream);
    /* DMA2 Stream0 channel0 configuration **************************************/
    DMA_InitStructure.DMA_Channel = Phase_ADC_DMA_Channel;
    DMA_InitStructure.DMA_PeripheralBaseAddr = Phase_ADC_Addr;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_USED_ConvertedValue1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //����DMA����Ϊ����//���ڴ�
    DMA_InitStructure.DMA_BufferSize = ADC_Converted_Num1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�����ַ++
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //���ַ�//ʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    
    DMA_Init(Phase_ADC_DMA_Stream, &DMA_InitStructure);
        
//    DMA_DoubleBufferModeConfig(DMA2_Stream0, (uint32_t)ADC_USED_ConvertedValue2, DMA_Memory_0);
//    DMA_DoubleBufferModeCmd(Phase_ADC_DMA_Stream, ENABLE);
    
    DMA_ITConfig(Phase_ADC_DMA_Stream,DMA_IT_TC|DMA_IT_HT,ENABLE);    
    DMA_Cmd(Phase_ADC_DMA_Stream, ENABLE); 
#endif    
}
/*
 *@ <function name=>Current_ADC_DMA_Config() </function>
 *@ <summary>
    ADC DMA����
 *@ </summary>
 *@ <param name="xxx"></param>
 *@ <returns> </returns>
*/
static void Current_ADC_DMA_Config(void)
{
#ifdef _STM32F4xx_
    DMA_InitTypeDef DMA_InitStructure;
    /* Enable ADC, DMA clocks ****************************************/
    RCC_AHB1PeriphClockCmd(RCC_Current_ADC_DMA, ENABLE);   
    DMA_DeInit(Current_ADC_DMA_Stream);
    /* DMA2 Stream0 channel0 configuration **************************************/
    DMA_InitStructure.DMA_Channel = Current_ADC_DMA_Channel;
    DMA_InitStructure.DMA_PeripheralBaseAddr = Current_ADC_Addr;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_USED_ConvertedValue2;    
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //����DMA����Ϊ����//���ڴ�
    DMA_InitStructure.DMA_BufferSize = ADC_Converted_Num2;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�����ַ++
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //���ַ�//ʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;        
    DMA_Init(Current_ADC_DMA_Stream, &DMA_InitStructure);
    
//    DMA_DoubleBufferModeConfig(DMA2_Stream0, (uint32_t)ADC_USED_ConvertedValue2, DMA_Memory_0);
//    DMA_DoubleBufferModeCmd(Phase_ADC_DMA_Stream, ENABLE);
    
    DMA_ITConfig(Current_ADC_DMA_Stream,DMA_IT_TC|DMA_IT_HT,ENABLE);
    DMA_Cmd(Current_ADC_DMA_Stream, ENABLE);    
#endif    
}
/*
 *@ <function name=>DMA_Phase_ADC_NVIC() </function>
 *@ <summary>
    ADC DMA�ж�����
 *@ </summary>
 *@ <param name="prePriority">��ռ���ȼ�</param>
 *@ <param name="subPriority">�������ȼ�</param>
 *@ <returns> </returns>
*/
void DMA_Phase_ADC_NVIC(u8 prePriority,u8 subPriority)
{
#ifdef _STM32F4xx_
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = Phase_ADC_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prePriority;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
#endif
}
/*
 *@ <function name=>DMA_Current_ADC_NVIC() </function>
 *@ <summary>
    ADC DMA�ж�����
 *@ </summary>
 *@ <param name="prePriority">��ռ���ȼ�</param>
 *@ <param name="subPriority">�������ȼ�</param>
 *@ <returns> </returns>
*/
void DMA_Current_ADC_NVIC(u8 prePriority,u8 subPriority)
{
#ifdef _STM32F4xx_
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = Current_ADC_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prePriority;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
#endif
}
/*
 *@ <function name=>DMA_Phase_ADC_IRQPendle() </function>
 *@ <summary>
    ADC DMA�����ж��Ӻ���,��stm32f4xx_t.c�е���
 *@ </summary>
 *@ <param name="xxx"></param>
 *@ <returns> </returns>
*/
void DMA_Phase_ADC_IRQPendle(void)
{
#ifdef _STM32F4xx_
    if(DMA_GetITStatus(Phase_ADC_DMA_Stream,DMA_IT_TCIF0))
    {
        DMA_ClearITPendingBit(Phase_ADC_DMA_Stream,DMA_IT_TCIF0);
        DMA_TC_Flag = 1;
    }
    if(DMA_GetITStatus(Phase_ADC_DMA_Stream,DMA_IT_HTIF0))
    {
        DMA_ClearITPendingBit(Phase_ADC_DMA_Stream,DMA_IT_HTIF0);
        DMA_HT_Flag = 1;
    } 
#endif
#ifdef _STM32F10x_
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC1);
        DMA_TC_Flag = 1;
    }
#endif    
}
/*
 *@ <function name=>DMA_Current_ADC_IRQPendle() </function>
 *@ <summary>
    ADC DMA�����ж��Ӻ���,��stm32f4xx_t.c�е���
 *@ </summary>
 *@ <param name="xxx"></param>
 *@ <returns> </returns>
*/
void DMA_Current_ADC_IRQPendle(void)
{
    s8 times = 50;
    static s8 ii = 0;
    static float temp = 0;
    float now_ad_value = 0;
#ifdef _STM32F4xx_
    if(DMA_GetITStatus(Current_ADC_DMA_Stream,DMA_IT_TCIF2))
    {
        DMA_ClearITPendingBit(Current_ADC_DMA_Stream,DMA_IT_TCIF2);
        now_ad_value = Get_Current_ADC_Value();
        if(Current_CalibrateState == 0)  //�������У׼
        {
            if(ii < times)
            {
                temp += now_ad_value;
                ii++;
            }
            else
            {
                temp = temp/times;
                Current_Zero = ADvalue2Current(temp);
                Voltag_Zero = ADvalue2Voltag(temp);
                Current_CalibrateState = 1;
            }
        }
        else  //У׼��ɺ���ʽ�ɵ���
        {
            Current_Value = ADvalue2Current(now_ad_value) - Current_Zero;
        }
    }
    if(DMA_GetITStatus(Current_ADC_DMA_Stream,DMA_IT_HTIF2))
    {
        DMA_ClearITPendingBit(Current_ADC_DMA_Stream,DMA_IT_HTIF2);
    } 
#endif
}
/*
 *@ <function name=>Get_ADC_Value() </function>
 *@ <summary>
    ��ȡADC�Ĳ���ֵ��ƽ��ֵ
 *@ </summary>
 *@ <param name="xxx"></param>
 *@ <returns> ���ƽ��֮���ADֵ </returns>
*/
float Get_Current_ADC_Value(void)
{
    u32 temp = 0;
    float temp_sesult = 0;
    for(u8 ii = 0;ii<ADC_Converted_Num2;ii++)
    {
        temp += ADC_USED_ConvertedValue2[ii];
    }
    temp_sesult = 1.0f*temp/ADC_Converted_Num2;
    return temp_sesult;
//    Current_AD_Value = (s32)temp_sesult;
//    
////    Current_Value = ADvalue2Voltag(temp_sesult)/100.0f;//to mV    
//    Current_Value = ADvalue2Current(temp_sesult);//to mA
}
/*
 *@ <function name=>ADvalue2Voltag() </function>
 *@ <summary>
    ADC Value ת��Ϊ��ѹ
 *@ </summary>
 *@ <param name="adValue">�ɼ�����ADCԭʼֵ</param>
 *@ <returns> ����ɵ�ѹֵmV </returns>
*/
float ADvalue2Voltag(float adValue)
{
    float ref = 0;
    float temp = adValue;
    ref = 3.3f*1000*temp/4095;
    return ref;
}
/*
 *@ <function name=>ADvalue2Current() </function>
 *@ <summary>
    ADC Value  ת��Ϊ����mA
 *@ </summary>
 *@ <param name="voltagValue">�ɼ����ĵ�ѹֵ</param>
 *@ <returns> ����ɵ���ֵmA </returns>
*/
float ADvalue2Current(float adValue)
{
    float ref = 0;
    float temp = ADvalue2Voltag(adValue);
    ref = 10*(2500 - temp);//mA
    return ref;
}

/**************************End of this file***************************************



                                          Copyright @JohnBee */
