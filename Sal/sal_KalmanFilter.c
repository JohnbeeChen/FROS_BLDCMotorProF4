/********************************************************************************
  * @FileName   sal_KalmanFilter.c
  * @Author     Johnbee
  * @Version    V1.0.0
  * @Date       2015/11/28
  * @Brief      �㷨�����,�������˲�(KF)
  *******************************************************************************/
/*#includes**********************************************************************/
#include "sal_KalmanFilter.h"

/*
 *@<function name=> KF_Simple_Init() </function>
 *@<summary>
    �򵥵�һά����Kalman�˲��Ĳ�����ʼ��,Ĭ��A=H=1
 *@</summary>
 *@<param name="myStruct">kalman�����ṹ��</param>
 *@<param name="myX"> ��ʼ��״̬��</param>
 *@<param name="myP">��ʼ��Ԥ������Э����</param>
 *@<param name="myQ"> ��ʼ����������Э����</param>
 *@<param name="myR"> ��ʼ����������Э����</param>
 *@<returns> null </returns>
*/
void KF_Simple_Init(KFParamStruct* myStruct,float myX,float myP,float myQ,float myR)
{
    myStruct->x = myX;
    myStruct->p = myP;
    myStruct->q = myQ;
    myStruct->r = myR;
    myStruct->A = 1;
    myStruct->H = 1;
    myStruct->kg = 0;
}
/*
 *@<function name=> KF_Simple_Calculate() </function>
 *@<summary>
    �򵥵�һά����Kalman�˲�����
 *@</summary>
 *@<param name="myStruct">kalman�����ṹ��</param>
 *@<param name="zMeasure">������</param>
 *@<returns> �˲�������Ź���ֵ </returns>
*/
float KF_Simple_Calculate(KFParamStruct* myStruct,float zMeasure)
{
    /*predict processing*/
    myStruct->x = myStruct->A * myStruct->x;
    myStruct->p = myStruct->A*myStruct->p*myStruct->A + myStruct->q;/* p(n|n-1)=A^2*p(n-1|n-1)+q */
    /*Measure processing*/
    myStruct->kg = myStruct->p * myStruct->H/(myStruct->H*myStruct->p*myStruct->H + myStruct->r);
    myStruct->x = myStruct->x +myStruct->kg*(zMeasure - myStruct->H*myStruct->x);
    myStruct->p = (1 - myStruct->kg*myStruct->H)*myStruct->p;
    return myStruct->x;
}
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */
                                                             
