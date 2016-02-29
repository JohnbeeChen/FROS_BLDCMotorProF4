/********************************************************************************
  * @FileName   main.c
  * @Author     Johnbee
  * @Version    V1.0.0
  * @Date       2015/10/02
  * @Brief      ����FreeRTOS����ϵͳ��STM32F4xx����
  *******************************************************************************/
/*********************************************************************************
   ���ڳ����Ԥ����ѡ����Johnbee_dbg.h�ļ���
   ��static ���εĺ��������ڲ�����
   ÿ����������ĸ����д�ı���Ϊȫ�ֱ���,��MyValue
   ����ĸСдʣ�ڵ�������ĸ��д�ı���Ϊ�β�,��myValue
   ȫСд�ı���Ϊ�ֲ�����,��temp_value
*ע    ��:
       �����ļ��Ѿ����޸Ĺ���,���FreeRTOSConfig.h�����������������
           #define vPortSVCHandler SVC_Handler
           #define xPortPendSVHandler PendSV_Handler
           #define xPortSysTickHandler SysTick_Handler
       �����޸������ļ�.
       system_stm32f4xx.cĬ���ⲿ����25M��ĳ�8M,��Ȼ��RCC�ͻ������
 ********************************************************************************/
/*#includes**********************************************************************/
#include "bsp_init.h"
#include "app.h"

/*#define************************************************************************/
#define  MyDelay(n)  while(n--)
#define  OS_Debug

#ifndef  OS_Debug  //ʵ����

/*********************************************************************************
 * @��������  main()
 * @��������  ������
 * @��ڲ���  ��
 * @���ڲ���  ��
*********************************************************************************/
int main(void)
{
    xQueueHandle queue = xQueueCreate(1, sizeof( unsigned long ));

    if(queue != NULL)
    {
        Start_Task_Create();  //������ʼ����       
        vTaskStartScheduler();   //��������������ʼִ������
    }
    while(1);
}

#endif
#ifdef OS_Debug   //������
/*********************************************************************************
 * @��������  main()
 * @��������  ������
 * @��ڲ���  ��
 * @���ڲ���  ��
*********************************************************************************/
float temp_speed = 0;
s16 temp_encordee = 0;
int main(void)
{
    u32 ii = 0;
    u8 n = 0;
    float jj = 0;
//    StateLED_Init();
    Bsp_NVIC_Config();
    Bsp_ADC_Init();
    TIM1_PWM_Config();
    BlueToothInit();
    Bsp_Encoder_Config();    
    RCC_ClocksTypeDef mysystemclock;
    RCC_GetClocksFreq(&mysystemclock);
    temp_speed = 250;    
    Set_MotorSpeed(temp_speed);
    Hall_PrepareCommutation();/*Ԥ����,����ʱ��Ҫ*/
    Manual_COMevent();
    while (1)   //�����������ĳЩ���ܽ��е����Ĳ���
    {
        jj = 123;
        for(int aa = 0;aa<0x03ff;aa++)
        {
            ii = 0xffff;
            MyDelay(ii);

            temp_encordee = Get_Encoder_Value();
            Send_Data(Current_Value,ID_Osc_CH2);           
        }
        Set_MotorSpeed(temp_speed);
        n = !n;
        if(n)
        {
            temp_speed = -250;
        }
        else
        {
            temp_speed = 250;            
        }
    }
}
#endif

/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
