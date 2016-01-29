/********************************************************************************
  * @FileName   Control_Task.h
  * @Author     Johnbee
  * @Version    V1.0.0
  * @Date       2015/11/23
  * @Brief      �����������
  *******************************************************************************/
#ifndef   _CONTROL_TASK_H_
#define   _CONTROL_TASK_H_
/*#includes**********************************************************************/
#include "app.h"
#include "sal_PID.h"

typedef struct _MotorParamStruct
{
    float speed;//�ٶ�
    float lenth;//λ��
}MotorParamStruct;  //��������ṹ��


extern MotorParamStruct   Iface_MyMotor;
extern PIDTypeDef  PID_Loction,PID_Speed,PID_Current;  //λ��,�ٶ�,������

void Control_Task_Create(u8 myPrio);
static void Control_Task(void *pdata);

#endif
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
