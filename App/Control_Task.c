/********************************************************************************
  * @FileName   Control_Task.c
  * @Author     Johnbee
  * @Version    V1.0.0
  * @Date       2015/11/23
  * @Brief      �����������
                Iface_ǰ׺�ı�����Ϊ����ӿ�
  *******************************************************************************/
/*#includes**********************************************************************/
#include "Control_Task.h"
#include "sal_PID.h"
#include "sal_KalmanFilter.h"

/*#define************************************************************************/
#define   CONTROL_TASK_STK_SIZE		   512 //����ջ��С

/*ȫ�ֱ�������*******************************************************************/
PIDTypeDef         PID_Loction,PID_Speed,PID_Current;  //λ��,�ٶ�,������
MotorParamStruct   Iface_MyMotor;
KFParamStruct      KF_Speed;

/*
 *@<function name=> Control_Task_Create() </function>
 *@<summary>
    ������������
 *@</summary>
 *@<param name="myPrio"> �������ȼ�</param>
 *@<returns> null </returns>
*/
void Control_Task_Create(u8 myPrio)
{
    xTaskCreate(Control_Task,
                "control_task",
                CONTROL_TASK_STK_SIZE,
                ( void * ) NULL,
                myPrio,NULL); 
}
/*
 *@<function name=> Control_Task() </function>
 *@<summary>
    ��������
 *@</summary>
 *@<param name="pdata">no use</param>
 *@<returns> null </returns>
*/
static void Control_Task(void *pdata)
{
    float now_speed = 0,now_lenth = 0;//�ٶ�λ�Ƶķ���ֵ
    static u8 ii = 0,jj  = 0;
    (void)pdata;                		// 'pdata' ��û���õ�����ֹ��������ʾ����
    
    Set_PID_Param(&PID_Loction,0,1,0,0,250,-250);//��ʼ������
    Set_PID_Param(&PID_Speed,1,0.5,0,0,800,-800);
    Set_PID_Param(&PID_Current,0,1,0,0,10000,-10000);
    KF_Simple_Init(&KF_Speed,0,10,100,100);
    while(1)
    {
        vTaskDelay(1*configTICK_RATE_HZ/1000);  //��ʱ����        
        if(jj == 0)//�⻷
        {
            jj = 10;
            now_lenth = Get_Encoder_Lenth();
            PID_Cal(&PID_Loction,Iface_MyMotor.lenth,now_lenth,100);//PID����
            Iface_MyMotor.speed = PID_Loction.ref;
        }

        if(ii == 0)//�ڻ�
        {
            ii = 3;
            now_speed = Get_Encoder_Speed();
         //   now_speed = KF_Simple_Calculate(&KF_Speed,now_speed);//�ٶ�KF�˲�     

            PID_Cal(&PID_Speed,Iface_MyMotor.speed,now_speed,10);//PID����
            PID_Speed.ref += PID_Speed.fd * Iface_MyMotor.speed; //��ǰ��
    //        PID_Speed.ref = Iface_MyMotor.speed;//��������
            
            Set_MotorSpeed(PID_Speed.ref);
            Send_Data(now_lenth,ID_Osc_CH2);           
            Send_Data(now_speed,ID_Osc_CH3);                      
        }
        ii--,jj--;
    }
}

/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
