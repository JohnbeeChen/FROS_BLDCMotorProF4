/********************************************************************************
  * @FileName   app.c
  * @Author     Johnbee
  * @Version    V1.0.0
  * @Date       2015/10/02
  * @Brief      ��ʼ����,һ����˵ֻ����һ��
  *******************************************************************************/
/*#includes**********************************************************************/
#include "app.h"

/*#define************************************************************************/
#define STARTUP_TASK_PRIO		  10u //�����������ȼ�
#define MAIN_TASK_PRIO		      4u 
#define CONTROL_TASK_PRIO         2u 
#define USART_TASK_PRIO           9u
#define Encoder_TASK_PRIO         1u
#define LED_TASK_PRIO             8u

/*
 *@<function name=> Start_Task_Create() </function>
 *@<summary>
    ������ʼ����
 *@</summary>
 *@<param name="void"> no use</param>
 *@<returns> null </returns>
*/
void Start_Task_Create(void)
{
    xTaskCreate(Start_Task,          //������ʼ����
                "OSStatInit",
                128,
                (void *)NULL,
                STARTUP_TASK_PRIO,
                NULL);
}
/*********************************************************************************
 * @��������  Start_Task()
 * @��������  ��ʼ����
 * @��ڲ���  pdata
 * @���ڲ���  ��
*********************************************************************************/
static void Start_Task(void *pvParameters)
{
    (void)pvParameters; 
    vTaskDelay(60*configTICK_RATE_HZ);  //��ʱ60s�ȴ������ȶ����ٲ���        

    Bsp_Init();  //���ֳ�ʼ��      
    taskENTER_CRITICAL();             //���ж�    
    /*�����ﴴ����������*/
    Main_Task_Create(MAIN_TASK_PRIO);
    Control_Task_Create(CONTROL_TASK_PRIO);
    USART_Task_Create(USART_TASK_PRIO);
    Encoder_Task_Create(Encoder_TASK_PRIO);  
    LED_Task_Create(LED_TASK_PRIO);    
    taskEXIT_CRITICAL();         //���ж�
    vTaskDelete(NULL);   //ɾ���Լ�
}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
