/********************************************************************************
 *@FileName    bsp_Encoder.c
 *@Author      Johnbee
 *@Version     V0.1
 *@Date        2015/11/23
 *@Brief       ���ñ�����
 *******************************************************************************/
/*#includes*********************************************************************/
#include "bsp_Encoder.h"

/*#define***********************************************************************/
#ifdef  _STM32F10x_
#define Encoder_TIM                TIM2
#define RCC_Encoder_TIM            RCC_APB1Periph_TIM2
#define Encoder_A_Port             GPIOA
#define Encoder_A_Pin              GPIO_Pin_0
#define RCC_Encoder_A_Port         RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO
#define Encoder_B_Port             GPIOA
#define Encoder_B_Pin              GPIO_Pin_1
#define RCC_Encoder_B_Port         RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO
#endif
#ifdef  _STM32F4xx_
#define Encoder_TIM                TIM4
#define RCC_Encoder_TIM            RCC_APB1Periph_TIM4
#define Encoder_A_Port             GPIOB
#define Encoder_A_Pin              GPIO_Pin_6
#define Encoder_A_PinSource        GPIO_PinSource6
#define Encoder_A_AF               GPIO_AF_TIM4
#define RCC_Encoder_A_Port         RCC_AHB1Periph_GPIOB 
#define Encoder_B_Port             GPIOB
#define Encoder_B_Pin              GPIO_Pin_7
#define Encoder_B_PinSource        GPIO_PinSource7
#define Encoder_B_AF               GPIO_AF_TIM4
#define RCC_Encoder_B_Port         RCC_AHB1Periph_GPIOB
#endif
#define   Encoder_Init_Value     (30000u)
#define   Encoder_ARR_Value      (60000u)

/*ȫ�ֱ�������********************************************************************/
//float Encoder_Lenth = 0;//������λ��

/*********************************************************************************
 * @��������  Bsp_Encoder_Config()
 * @��������  ���ö�ʱ����׽������
 * @��ڲ���  ��
 * @���ڲ���  ��
*********************************************************************************/
void Bsp_Encoder_Config(void)
{
    RCC_APB1PeriphClockCmd(RCC_Encoder_TIM, ENABLE);  //ʹ�ܶ�ʱ��9��ʱ��
    Encoder_GPIO_Config();
    TIM_EncoderInterfaceConfig(Encoder_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    Encoder_TIM->ARR = Encoder_ARR_Value;  //��װ�ؼĴ���
    Encoder_TIM->CNT = Encoder_Init_Value;   //�����Ĵ���
    TIM_Cmd(Encoder_TIM, ENABLE);
}
/*
 *@ <function name=> Set_PID_Param() </function>
 *@ <summary>
    Get ������������ֵ
 *@ </summary>
 *@ <returns> null </returns>
*/
s16 Get_Encoder_Value(void)
{
    s16 temp = Encoder_TIM->CNT - Encoder_Init_Value;
    Encoder_TIM->CNT = Encoder_Init_Value;
    return temp;
}
/*********************************************************************************
 * @��������  Encoder_GPIO_Config()
 * @��������  ���ñ�����GPIO��
 * @��ڲ���  ��
 * @���ڲ���  ��
*********************************************************************************/
static void Encoder_GPIO_Config(void)
{
#ifdef _STM32F4xx_ 
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_Encoder_A_Port | RCC_Encoder_B_Port, ENABLE); //��ʱ��

    GPIO_InitStructure.GPIO_Pin  = Encoder_A_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   //����ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //�������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(Encoder_A_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = Encoder_B_Pin;
    GPIO_Init(Encoder_B_Port, &GPIO_InitStructure);

    GPIO_PinAFConfig(Encoder_A_Port, Encoder_A_PinSource, Encoder_A_AF); //���ö�ʱ�����ù���
    GPIO_PinAFConfig(Encoder_B_Port, Encoder_B_PinSource, Encoder_B_AF); //���ö�ʱ�����ù���
#endif
#ifdef _STM32F10x_
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_Encoder_A_Port | RCC_Encoder_B_Port, ENABLE); //��ʱ��

    GPIO_InitStructure.GPIO_Pin  = Encoder_A_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Encoder_A_Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = Encoder_B_Pin;
    GPIO_Init(Encoder_B_Port, &GPIO_InitStructure);

#endif
}

/****************** end of this file ********************************************


                                                             Copyright @JohnBee */

