/***********************************************************************************************
 *@FileName   USART_Task.c
 *@Version    V0.0.0.1
 *@Author     Johnbee
 *@Date       2015/07/31
 *@Brief      ���ڷ�������
 ***********************************************************************************************/
/*#include**************************************************************************************/
#include "app.h"

/*#define**************************************************************************************/
#define USART_TASK_STK_SIZE		   512 //����ջ��С

/*�ⲿ��������********************************************************************/
extern USART_QUEUE BLE_USART_QUEUE;

/*
 *@<function name=> USART_Task_Create() </function>
 *@<summary>
    �������ڷ�������
 *@</summary>
 *@<param name="myPrio">�������ȼ�</param>
 *@<returns> null </returns>
*/
void USART_Task_Create(u8 myPrio)
{
    xTaskCreate(USART_Task,
                "main_task",
                USART_TASK_STK_SIZE,
                ( void * ) NULL,
                myPrio,NULL);
}

/*
 *@ <task name=> USART_Task() </task>
 *@ <summary>
    ���ڷ�����������,�û����ȼ����,ȡ����������ĵ�λ
 *@ </summary>
 *@ <param name="pdata"> no use< /param>
 *@ <returns> null </returns>
*/
static void USART_Task(void *pdata)
{
    (void)pdata;
	while(1)
	{
        vTaskDelay(1);
		for(u8 jj = 0;jj<30;jj++)
		{
			if(BLE_USART_QUEUE.Rear != BLE_USART_QUEUE.Front) //��������Ϊ��
			{
				Send_Array(BLE_USART_QUEUE.USART_BUF[BLE_USART_QUEUE.Front].DATA, 8);
				BLE_USART_QUEUE.Front++;   //����	
				if( BLE_USART_QUEUE.Front >= USART_SendBuf_Size)
				{
					BLE_USART_QUEUE.Front = 0;
				}
			}
	    }
	}
}

/**************************End of this file***************************************
 


                                          Copyright @JohnBee */
