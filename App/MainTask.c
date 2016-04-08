/********************************************************************************
  * @FileName   MainTask.c
  * @Author     Johnbee
  * @Version    V1.0.0
  * @Date       2015/10/02
  * @Brief      ������,һ�������߼���������
  *******************************************************************************/
/*#includes**********************************************************************/
#include "MainTask.h"

/*#define************************************************************************/
#define  MAIN_TASK_STK_SIZE		   512u //����ջ��С

/*
 *@<function name=> Main_Task_Create() </function>
 *@<summary>
    ��������
 *@</summary>
 *@<param name="myPrio">�������ȼ�</param>
 *@<returns> null </returns>
*/
void Main_Task_Create(u8 myPrio)
{
    xTaskCreate(Main_Task,
                "main_task",
                MAIN_TASK_STK_SIZE,
                ( void * ) NULL,
                myPrio,NULL);
}
/*
 *@<function name=> Main_Task() </function>
 *@<summary>
    ������
 *@</summary>
 *@<param name="pdata">null</param>
 *@<returns> null </returns>
*/
static void Main_Task(void *pdata)
{
    (void)pdata;   // 'pdata' ��û���õ�����ֹ��������ʾ����
    while(1)
    {
        vTaskDelay(5*configTICK_RATE_HZ/1000);  //��ʱ����
        Iface_MyMotor.speed = ComData[ID_Loc_Ki3];//�ٶȱջ�
        Iface_MyMotor.lenth = 5*ComData[ID_Loc_Kp3];//λ�Ʊջ�

        PID_Loction.kp = ComData[ID_Loc_Kp1]/10;
        PID_Loction.ki = ComData[ID_Loc_Ki1]/10;
        PID_Loction.kd = ComData[ID_Loc_Kd1]/10;
        
        PID_Speed.kp = ComData[ID_Psi_Kp1]/10;
        PID_Speed.ki = ComData[ID_Psi_Ki1]/10;
        PID_Speed.kd = ComData[ID_Psi_Kd1]/10;
        if(Iface_MyMotor.speed != 0)
        {
            Set_MotorState(1);
        }
        else
        {
            Set_MotorState(0);            
        }
        Send_Data(Current_Value,ID_Osc_CH1);      
    }
}

/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
