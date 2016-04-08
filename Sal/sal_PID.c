/********************************************************************************
 *@FileName    sal_PID.c
 *@Author      Johnbee
 *@Version     V0.0.1
 *@Date        2015/11/24
 *@Brief       �㷨�����,PID�㷨
 *******************************************************************************/
/*#includes*********************************************************************/
#include "sal_PID.h"

/*
 *@ <function name=> Set_PID_Param() </function>
 *@ <summary>
    ����λ��ʽPID������ֵ
 *@ </summary>
 *@ <returns> null </returns>
*/
void Set_PID_Param(PIDTypeDef* my_pid,float fd,float kp,float ki,float kd,float max,float min)
{
	my_pid->fd = fd;
	my_pid->kp = kp;
	my_pid->ki = ki;
	my_pid->kd = kd;
	my_pid->max_value = max;
	my_pid->min_value = min;
}

/*
 *@ <function name=> PID_Cal() </function>
 *@ <summary>
    λ��ʽPID����
 *@ </summary>
 *@ <param name="my_PID"> Ҫ�����PID�Ľṹ��</param>
 *@ <param name="myExpectValue"> Ҫ���Ƶ�����ֵ,0��ʾ�����м�</param>
 *@ <param name="myNowValue">���ԭ���ֵ,�����ǳ�ʼֵ</param>
 *@ <param name="inteRegion">��������,��������ⷶΧ�ڲŻ���</param>
 *@ <returns> null </returns>
*/
void PID_Cal(PIDTypeDef* my_PID,float myExpectValue,float myNowValue,float inteRegion)
{
    float temp_region = inteRegion;
	my_PID->now_value = myNowValue;
	my_PID->expect_value = 	myExpectValue;
	my_PID->now_error =  my_PID->expect_value - my_PID->now_value;
	my_PID->delta_value = my_PID->now_error - my_PID->last_error;
	if(temp_region>0)
	{
		if((my_PID->now_error<temp_region)&&(my_PID->now_error>-temp_region))   //С���Ż���
		{
			my_PID->sum_value  += my_PID->now_error;
		}
    }
	my_PID->ref  = my_PID->kp*my_PID->now_error
				 + my_PID->ki*my_PID->sum_value/100
				 + my_PID->kd*my_PID->delta_value;
	my_PID->last_error = my_PID->now_error;	
	if(my_PID->ref > my_PID->max_value)
	{
		my_PID->ref = my_PID->max_value;
	}	
	else if(my_PID->ref < my_PID->min_value)
	{
		my_PID->ref = my_PID->min_value;
	}		
}

/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
