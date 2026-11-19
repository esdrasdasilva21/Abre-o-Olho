/**
 ******************************************************************************
 * @file    stm32f30x_wwdg.h
 * @author  MCD Application Team (Melhorado e Comentado)
 * @version V1.1.0
 * @date    2026
 * @brief   Protótipos e constantes para configuração do Window Watchdog (WWDG).
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32F30X_WWDG_H
#define STM32F30X_WWDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"

/** @addtogroup STM32F30x_StdPeriph_Driver
 * @{
 */

/** @addtogroup WWDG
 * @{
 */ 

/* Exported constants --------------------------------------------------------*/

/** @defgroup WWDG_Exported_Constants
 * @{
 */ 
  
/** @defgroup WWDG_Prescaler 
 * @{
 */ 
  
#define WWDG_Prescaler_1    ((uint32_t)0x00000000U)  /* WWDG counter clock = (PCLK1/4096)/1 */
#define WWDG_Prescaler_2    ((uint32_t)0x00000080U)  /* WWDG counter clock = (PCLK1/4096)/2 */
#define WWDG_Prescaler_4    ((uint32_t)0x00000100U)  /* WWDG counter clock = (PCLK1/4096)/4 */
#define WWDG_Prescaler_8    ((uint32_t)0x00000180U)  /* WWDG counter clock = (PCLK1/4096)/8 */

#define IS_WWDG_PRESCALER(PRESCALER) (((PRESCALER) == WWDG_Prescaler_1) || \
                                      ((PRESCALER) == WWDG_Prescaler_2) || \
                                      ((PRESCALER) == WWDG_Prescaler_4) || \
                                      ((PRESCALER) == WWDG_Prescaler_8))

#define IS_WWDG_WINDOW_VALUE(VALUE)  ((VALUE) <= 0x7FU)

/* O contador WWDG tem 7 bits (0x40 a 0x7F). Abaixo de 0x40 gera reset automático. */
#define IS_WWDG_COUNTER(COUNTER)     (((COUNTER) >= 0x40U) && ((COUNTER) <= 0x7FUE))

/**
 * @}
 */ 

/**
 * @}
 */ 

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Restaura os registos do WWDG para os valores predefinidos de fábrica.
 */
void WWDG_DeInit(void);

/**
 * @brief  Define o divisor de clock (prescaler) do WWDG.
 */
void WWDG_SetPrescaler(uint32_t WWDG_Prescaler);

/**
 * @brief  Define o valor limite superior da janela de refrescamento.
 */
void WWDG_SetWindowValue(uint8_t WindowValue);

/**
 * @brief  Ativa a interrupção antecipada de acordo com o alerta EWI (Early Wakeup Interrupt).
 */
void WWDG_EnableIT(void);

/**
 * @brief  Atualiza o valor atual do contador do WWDG (alimentar o cão de guarda).
 */
void WWDG_SetCounter(uint8_t Counter);

/**
 * @brief  Ativa o WWDG e carrega o valor inicial do contador.
 */
void WWDG_Enable(uint8_t Counter);

/**
 * @brief  Verifica se o flag de Early Wakeup (EWI) foi disparado.
 */
FlagStatus WWDG_GetFlagStatus(void);

/**
 * @brief  Limpa o flag de interrupção pendente do WWDG.
 */
void WWDG_ClearFlag(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32F30X_WWDG_H */