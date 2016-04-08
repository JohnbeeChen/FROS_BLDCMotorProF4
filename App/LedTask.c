/********************************************************************************
  * @FileName   LedTask.c
  * @Author     Johnbee
  * @Version    V1.0.0
  * @Date       2016/04/01
  * @Brief      Led����
  *******************************************************************************/
/*#includes**********************************************************************/
#include "LedTask.h"
/*#define************************************************************************/
#define  LED_TASK_STK_SIZE		   128u //����ջ��С


/*
 *@<function name=> LED_Task_Create() </function>
 *@<summary>
    ��������
 *@</summary>
 *@<param name="myPrio">�������ȼ�</param>
 *@<returns> null </returns>
*/
void LED_Task_Create(u8 myPrio)
{
    xTaskCreate(LED_Task,
                "main_task",
                LED_TASK_STK_SIZE,
                ( void * ) NULL,
                myPrio, NULL);
}
/*
 *@<function name=> LED_Task() </function>
 *@<summary
    led����
 *@</summary>
 *@<param name="pdata">null</param>
 *@<returns> null </returns>
*/
static void LED_Task(void *pdata)
{
    (void)pdata;   // 'pdata' ��û���õ�����ֹ��������ʾ����
    u8 bled_flag = Led_ON;
    while (1)
    {
        vTaskDelay(configTICK_RATE_HZ); //��ʱ����
        Set_LED(BlueLed,bled_flag);
        bled_flag = ~bled_flag;   
    }
}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
