/**
 ******************************************************************************
 * @file    usb_init.h
 * @author  MCD Application Team (Comentado e Atualizado)
 * @version V4.1.0
 * @date    2026
 * @brief   Rotinas de inicialização e ponteiros globais do núcleo USB-FS.
 ******************************************************************************
 */

#ifndef USB_INIT_H
#define USB_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "usb_core.h"

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Inicializa as estruturas internas do stack USB, associa os ponteiros
 *         globais (pInformation, pProperty) e chama a rotina Init() da classe.
 */
void USB_Init(void);

/* External variables --------------------------------------------------------*/

/** @brief Índice do endpoint ativo no atendimento da interrupção atual (0..7). */
extern uint8_t EPindex;

/** @brief Ponteiro para o contexto da transação atual (Setup Packet e estado). */
extern DEVICE_INFO *pInformation;

/** @brief Ponteiro para a tabela de callbacks da classe (HID Init, Reset, Setup). */
extern DEVICE_PROP *pProperty;

/** @brief Ponteiro para os hooks opcionais de pedidos padrão USB 2.0. */
extern USER_STANDARD_REQUESTS *pUser_Standard_Requests;

/** @brief Guarda temporariamente o estado dos registos Rx/Tx durante uma ISR. */
extern uint16_t SaveState;

/** @brief Máscara de interrupções ativas no registo CNTR do periférico USB. */
extern uint16_t wInterrupt_Mask;

#ifdef __cplusplus
}
#endif

#endif /* USB_INIT_H *//**
 ******************************************************************************
 * @file    usb_init.h
 * @author  MCD Application Team (Comentado e Atualizado)
 * @version V4.1.0
 * @date    2026
 * @brief   Rotinas de inicialização e ponteiros globais do núcleo USB-FS.
 ******************************************************************************
 */

#ifndef USB_INIT_H
#define USB_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "usb_core.h"

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Inicializa as estruturas internas do stack USB, associa os ponteiros
 *         globais (pInformation, pProperty) e chama a rotina Init() da classe.
 */
void USB_Init(void);

/* External variables --------------------------------------------------------*/

/** @brief Índice do endpoint ativo no atendimento da interrupção atual (0..7). */
extern uint8_t EPindex;

/** @brief Ponteiro para o contexto da transação atual (Setup Packet e estado). */
extern DEVICE_INFO *pInformation;

/** @brief Ponteiro para a tabela de callbacks da classe (HID Init, Reset, Setup). */
extern DEVICE_PROP *pProperty;

/** @brief Ponteiro para os hooks opcionais de pedidos padrão USB 2.0. */
extern USER_STANDARD_REQUESTS *pUser_Standard_Requests;

/** @brief Guarda temporariamente o estado dos registos Rx/Tx durante uma ISR. */
extern uint16_t SaveState;

/** @brief Máscara de interrupções ativas no registo CNTR do periférico USB. */
extern uint16_t wInterrupt_Mask;

#ifdef __cplusplus
}
#endif

#endif /* USB_INIT_H */