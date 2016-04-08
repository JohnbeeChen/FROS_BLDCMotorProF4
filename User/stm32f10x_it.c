/**
  ******************************************************************************
  * @file    CortexM3/BitBand/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "includes.h"


/* global variables ----------------------------------------------------------*/


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup CortexM3_BitBand
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
//void DebugMon_Handler(void)
//{
//}

/**
  * @brief  This function handles PendSV_Handler exception.
            �ú�����������ϵͳ���ж���os_cpu_a.asm�б���д��
             ����������Ͳ����ٶ�����
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
//	OSIntEnter();		//�����ж�,�ж�Ƕ�ײ���++
//    OSTimeTick();       //����ucos��ʱ�ӷ������               
//    OSIntExit();        //�˳��ж�,�ж�Ƕ�ײ���--
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

//extern void DMA_Channel1_IRQPendVer(void);//�жϴ�����
void DMA1_Channel1_IRQHandler(void)
{
//    DMA_Channel1_IRQPendVer();
}

/**
  * @}
  */
extern void EXTI9_5_IRQPander(void);
void EXTI9_5_IRQHandler(void)
{
    EXTI9_5_IRQPander();
}
/**
  * @}
  */
extern void EXTI4_IRQPander(void);
void EXTI4_IRQHandler(void)
{
    EXTI4_IRQPander();
}
/**
  * @}
  */
extern void BLE_IRQPandler(void);
void USART3_IRQHandler(void)
{
    BLE_IRQPandler();
}
/**
  * @}
  */
extern void CAN1_RX0_IRQPendle(void);
void CAN1_RX1_IRQHandler(void)
{
    CAN1_RX0_IRQPendle();
}

extern void EXTI15_10_IRQPander(void);
void EXTI15_10_IRQHandler(void)
{
    EXTI15_10_IRQPander();
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
