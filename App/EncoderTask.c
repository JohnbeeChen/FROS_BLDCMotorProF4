/********************************************************************************
  * @FileName   EncoderTask.c
  * @Author     Johnbee
  * @Version    V1.0.0
  * @Date       2015/12/14
  * @Brief      ����������
  *******************************************************************************/
/*#includes**********************************************************************/
#include "EncoderTask.h"

/*#define************************************************************************/
#define Encoder_TASK_STK_SIZE		   128u //����ջ��С
#define   Encoder_Resolution          (400u) //�������ֱ���

/*ȫ�ֱ�������*******************************************************************/
float Encoder_Lenth = 0,Encoder_Speed = 0;

/*
 *@<function name=> Encoder_Task_Create() </function>
 *@<summary>
    ��������
 *@</summary>
 *@<param name="myPrio">�������ȼ�</param>
 *@<returns> null </returns>
*/
void Encoder_Task_Create(u8 myPrio)
{
    xTaskCreate(Encoder_Task,
                "encoder_task",
                Encoder_TASK_STK_SIZE,
                ( void * ) NULL,
                myPrio,NULL);
}
/*
 *@<function name=> Encoder_Task() </function>
 *@<summary>
    ������
 *@</summary>
 *@<param name="pdata">null</param>
 *@<returns> null </returns>
*/
void Encoder_Task(void *pdata)
{
    s16 temp = 0;

    (void)pdata;                		// 'pdata' ��û���õ�����ֹ��������ʾ����

    while(1)
    {
        vTaskDelay(5*configTICK_RATE_HZ/1000);  //��ʱ����
        temp = Get_Encoder_Value();
        Encoder_Lenth += temp;
        Encoder_Speed = 3000.0*temp/Encoder_Resolution;//����r/min
    }
}
/*
 *@<function name=> Get_Encoder_Speed() </function>
 *@<summary>
    ��ȡ�������ٶ�
 *@</summary>
 *@<param name="void">null</param>
 *@<returns> speed </returns>
*/
float Get_Encoder_Speed(void)
{
    return Encoder_Speed;
}
/*
 *@<function name=> Get_Encoder_Lenth() </function>
 *@<summary>
    ��ȡ������λ��
 *@</summary>
 *@<param name="void">null</param>
 *@<returns> speed </returns>
*/
float Get_Encoder_Lenth(void)
{
    return Encoder_Lenth;
}
/*
 *@<function name=> Set_Encoder_Lenth() </function>
 *@<summary>
    ���ñ�����λ��,�������������
 *@</summary>
 *@<param name="void">null</param>
 *@<returns> speed </returns>
*/
void Set_Encoder_Lenth(float myLenth)
{
    Encoder_Lenth = myLenth;
}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
