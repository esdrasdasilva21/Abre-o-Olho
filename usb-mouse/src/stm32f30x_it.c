/**
 ******************************************************************************
 * @file    stm32f30x_it.c
 * @author  MCD Application Team (Atualizado)
 * @version V1.2.0
 * @date    2026
 * @brief   Main Interrupt Service Routines for Open DoVision USB HID.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define BUTTON_DEBOUNCE_MS      200U

/* Extern variables (definidas em main.c) ------------------------------------*/
extern __IO uint32_t timeSinceRenew;
extern __IO uint32_t timeSinceSend;
extern __IO uint32_t timeSinceRead;
extern __IO uint32_t timerDelay;
extern uint8_t odvState;

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t sysTickMillis = 0;
static uint32_t lastButtonPressMs  = 0;

/* Public helper functions ---------------------------------------------------*/

/**
 * @brief  Retorna o tempo decorrido em milissegundos desde o arranque.
 */
uint32_t Get_SystemTickMs(void) {
    return sysTickMillis;
}

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

void NMI_Handler(void) {
}

void HardFault_Handler(void) {
    /* Sinaliza erro crítico no LED antes de travar */
    STM_EVAL_LEDOn(LED10);
    while (1) {
    }
}

void MemManage_Handler(void) {
    STM_EVAL_LEDOn(LED10);
    while (1) {
    }
}

void BusFault_Handler(void) {
    STM_EVAL_LEDOn(LED10);
    while (1) {
    }
}

void UsageFault_Handler(void) {
    STM_EVAL_LEDOn(LED10);
    while (1) {
    }
}

void SVC_Handler(void) {
}

void DebugMon_Handler(void) {
}

void PendSV_Handler(void) {
}

/**
 * @brief  Handler do SysTick (executado a cada 1 ms).
 *         Gere todos os temporizadores de envio, leitura e delays do sistema.
 */
void SysTick_Handler(void) {
    sysTickMillis++;
    timeSinceRenew++;
    timeSinceSend++;
    timeSinceRead++;

    if (timerDelay > 0) {
        timerDelay--;
    }
}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/******************************************************************************/

/**
 * @brief  Interrupção de baixa prioridade do USB (envio/receção de pacotes HID).
 */
#if defined (USB_INT_DEFAULT)
void USB_LP_CAN1_RX0_IRQHandler(void)
#elif defined (USB_INT_REMAP)
void USB_LP_IRQHandler(void)
#endif
{
    USB_Istr();
}

/**
 * @brief  Interrupção de Wake-up do USB (retoma após modo de suspensão).
 */
#if defined (USB_INT_DEFAULT)
void USBWakeUp_IRQHandler(void)
#elif defined (USB_INT_REMAP)
void USBWakeUp_RMP_IRQHandler(void)
#endif
{
    Resume(RESUME_EXTERNAL);
    EXTI_ClearITPendingBit(EXTI_Line18);
}

/**
 * @brief  Interrupção do Botão de Utilizador (com proteção contra debounce).
 *         Alterna manualmente entre State_Idle e State_Track.
 */
void USER_BUTTON_EXTI_IRQHandler(void) {
    if (EXTI_GetITStatus(USER_BUTTON_EXTI_LINE) != RESET) {
        /* Ignora ruído mecânico dentro da janela de debounce */
        if ((sysTickMillis - lastButtonPressMs) >= BUTTON_DEBOUNCE_MS) {
            lastButtonPressMs = sysTickMillis;

            if (odvState == State_Idle) {
                timeSinceRenew = 0;
                odvState = State_Track;
            } else {
                odvState = State_Idle;
            }
        }

        EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
    }
}