/********************************************************************************
 *@FileName    sal_PID.h
 *@Author      Johnbee
 *@Version     V0.0.1
 *@Date        2015/11/24
 *@Brief       �㷨�����,PID�㷨
 *******************************************************************************/
#ifndef   _SAL_PID_H_
#define   _SAL_PID_H_
/*#includes*********************************************************************/
#include "math.h"

typedef struct _PIDTypeDef  //λ��ʽPID
{
	float fd;   /*ǰ��*/
	float kp;
	float ki;
	float kd;
	float expect_value;  /*����ֵ,Ϊ0ʱ��ʾ�����м�*/
	float now_value;     /*ʵʱֵ*/
	float delta_value;   /*΢����*/
	float sum_value;     /*������*/
	float now_error;     /*���ε����ֵ*/
	float last_error;   /*��һ�ε����ֵ*/
	float max_value;
	float min_value;
	double ref;        /*���ֵ*/
}PIDTypeDef;


void PID_Cal(PIDTypeDef* my_PID,float myExpectValue,float myNowValue,float inteRegion);
void Set_PID_Param(PIDTypeDef* my_pid,float fd,float kp,float ki,float kd,float max,float min);

#endif
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
