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
       �����ļ��Ѿ����޸Ĺ���
       system_stm32f4xx.cĬ���ⲿ����25M��ĳ�8M,��Ȼ��RCC�ͻ������
 ********************************************************************************/
/*#includes**********************************************************************/
#include "bsp_init.h"
#include "app.h"

/*#define************************************************************************/
#define Delay(n)  while(n--)
//#define OS_Debug

#ifndef OS_Debug  //ʵ����

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
        Bsp_Init();  //���ֳ�ʼ��

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
int main(void)
{
    float recieve_speed = 0;
    static s16 delta_encoder = 0;
    static s16 adc_value = 0;
    u16 ii = 0x0fff;
    Bsp_Init();  //���ֳ�ʼ��
    while (1)   //�����������ĳЩ���ܽ��е����Ĳ���
    {
        ii--;
        recieve_speed = ComData[ID_Loc_Kp3];
        #if 0  //�����Ƿ�ʧ��ʱ���ص�
        TIM1_Set_PWMDuty(3, recieve_speed);
        TIM1_Set_PWMOutput_EnableState(3, 1, 1); // ʹ�����
        TIM1_Set_PWMOutput_EnableState(3, 0, 0); // ʹ�����
        #endif
        #if 1 //��������
        Set_MotorSpeed(recieve_speed);
        #endif
        if(ii == 0)
        {
            #if 1 // ���Դ���
            adc_value = Get_ADC_Value();
            delta_encoder = Get_Encoder_Value();
            Send_Data(-delta_encoder/10.0,ID_Osc_CH2);
            Send_Data(-adc_value/100,ID_Osc_CH3);           
            #endif
            ii = 0x0fff;
        }
    }
}
#endif

/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
