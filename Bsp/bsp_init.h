/********************************************************************************
 *@FileName    bsp_init.h
 *@Author      Johnbee
 *@Version     V0.1
 *@Date        2015/10/02
 *@Brief       ���ڸ���Ӳ����ʼ��,BspĿ¼�и��ֺ͵ײ������йص�ͷ�ļ�������������,
               �ϲ���õײ�ĺ���ֻ�ð������ͷ�ļ�
 *******************************************************************************/
#ifndef _BSP_INIT_H_
#define _BSP_INIT_H_

/*#includes*********************************************************************/
#include "includes.h"
#include "Johnbee_dbg.h"
#ifdef _STM32F10x_
    #include <stm32f10x.h>
#endif
#ifdef _STM32F4XX_
    #include <stm32f4xxx.h>
#endif

#include "bsp_TIM.h"
#include "bsp_CAN.h"
#include "bsp_ADC.h"

#include "bsp_Commutation.h"
#include "bsp_Bluetooth.h"
#include "bsp_Encoder.h"
#include "bsp_HallTIM.h"


void Bsp_Init(void);
void Bsp_NVIC_Config(void);
void SysTick_init(void);
void StateLED_Init(void);
void EXTI15_10_NVIC_Config(u8 preemPriority, u8 subPriority);
void Key_GPIO_Init(void);
void  TurnStateLED(u8 n);
#endif
/****************** end of this file ********************************************


                                                             Copyright @JohnBee */

