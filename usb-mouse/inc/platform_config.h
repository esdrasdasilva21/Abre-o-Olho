/**
 ******************************************************************************
 * @file    USB_Example/platform_config.h 
 * @author  MCD Application Team (Comentado e Otimizado)
 * @version V1.2.0
 * @date    2026
 * @brief   Configurações específicas da placa de avaliação para o stack USB.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"

/* Seleção da placa de desenvolvimento (Padrão: STM32F303C_EVAL / Discovery) */
#if !defined (USE_STM32303C_EVAL)
 #define USE_STM32303C_EVAL
#endif

#if defined (USE_STM32303C_EVAL)

/* Configuração do modo de interrupção USB */
#define USB_INT_DEFAULT  /* Utiliza o modo de interrupção padrão para o USB */

#if !defined (USB_INT_DEFAULT) && !defined (USB_INT_REMAP)
  #error "Erro: Defina o modo de interrupção USB em platform_config.h (ex: USB_INT_DEFAULT)."
#endif

/* Configuração do pino de Soft Disconnect do USB (Simula desligar/ligar o cabo) */
#define USB_DISCONNECT                      GPIOE  
#define USB_DISCONNECT_PIN                  GPIO_Pin_14
#define RCC_AHBPeriph_GPIO_DISCONNECT       RCC_AHBPeriph_GPIOE
                                     
/* Máscara global para ativação de todos os relógios dos portos GPIO no barramento AHB */
#define RCC_AHBPeriph_ALLGPIO               (RCC_AHBPeriph_GPIOA \
                                            | RCC_AHBPeriph_GPIOB \
                                            | RCC_AHBPeriph_GPIOC \
                                            | RCC_AHBPeriph_GPIOD \
                                            | RCC_AHBPeriph_GPIOE \
                                            | RCC_AHBPeriph_GPIOF)

#endif /* USE_STM32303C_EVAL */

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/