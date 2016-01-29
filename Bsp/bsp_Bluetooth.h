/********************************************************************************
 *@FileName    bsp_Bluetooth.h
 *@Author      Johnbee
 *@Version     V0.1
 *@Date        2015/10/20
 *@Brief       ��������
 *******************************************************************************/
#ifndef       _BSP_BLUETOOTH_H_
#define       _BSP_BLUETOOTH_H_
/*#includes*********************************************************************/
#include "bsp_init.h"

#define USART_SendBuf_Size    (100u)  //���ڷ��Ͷ��г���


typedef struct _USART_DATA 
{
   u8  DATA[8];             // Data
}USART_DATA;

typedef struct _USART_QUEUE
{
   u16 Front;        
   u16 Rear;
   USART_DATA USART_BUF[USART_SendBuf_Size];

}USART_QUEUE;
typedef enum
{
            #if 1  //PIDFORM��pid����
            ID_Loc_Kp1,
            ID_Loc_Ki1,
            ID_Loc_Kd1,

            ID_Psi_Kp1,
            ID_Psi_Ki1,
            ID_Psi_Kd1,

            ID_Loc_Kp2,
            ID_Loc_Ki2,
            ID_Loc_Kd2,

            ID_Psi_Kp2,
            ID_Psi_Ki2,
            ID_Psi_Kd2,

            ID_Loc_Kp3,
            ID_Loc_Ki3,
            ID_Loc_Kd3,
            #endif
            #if 1  //PIDFORM2��pid����
            ID1_Loc_Kp1,
            ID1_Loc_Ki1,
            ID1_Loc_Kd1,

            ID1_Psi_Kp1,
            ID1_Psi_Ki1,
            ID1_Psi_Kd1,

            ID1_Loc_Kp2,
            ID1_Loc_Ki2,
            ID1_Loc_Kd2,

            ID1_Psi_Kp2,
            ID1_Psi_Ki2,
            ID1_Psi_Kd2,

            ID1_Loc_Kp3,
            ID1_Loc_Ki3,
            ID1_Loc_Kd3,
            #endif
            IDs_Remote,  
            ID_SavsFlag,


            ID_Osc_CH1,  //ʾ����ͨ��1
            ID_Osc_CH2,  //ʾ����ͨ��2
            ID_Osc_CH3,  //ʾ����ͨ��3

            ID1_Osc_CH1,  //ʾ����2ͨ��1
            ID1_Osc_CH2,  //ʾ����2ͨ��2
            ID1_Osc_CH3,  //ʾ����2ͨ��3

            ID_Acc_X,
            ID_Acc_Y,
            ID_Acc_Z,

            ID_Omega_X,
            ID_Omega_Y,
            ID_Omega_Z,

            ID_Mag_X,  //�شų�
            ID_Mag_Y,
            ID_Mag_Z,

            ID_Pitch,
            ID_Roll,
            ID_Yaw,
            ID_Pressure,
 
            ID_Pulse1,
            ID_Pulse2,
            ID_Pulse3,
            ID_Pulse4,

            ID_ExpectX,
            ID_ExpectY,

            MyComId_Num,          /*ָ����Ŀ���������*/
}MyComId;

extern float ComData[MyComId_Num];
extern USART_QUEUE USART3_QUEUE;

void Send_Array(uint8_t * Array, uint16_t Len);
void Send_Data(float num,uint8_t NUM_Type);
void BlueToothInit(void);
void BLE_NVIC_Config(u8 preemPriority,u8 subPriority);

#endif 
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */

