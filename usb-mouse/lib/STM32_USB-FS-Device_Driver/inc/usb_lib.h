/**
 ******************************************************************************
 * @file    usb_lib.h
 * @author  MCD Application Team (Comentado e Atualizado)
 * @version V4.1.0
 * @date    2026
 * @brief   Cabeçalho mestre (Umbrella Header) da biblioteca STM32 USB-FS.
 *          Agrupa hardware, registos, protocolo core, SIL e gestão de memória.
 ******************************************************************************
 */

#ifndef USB_LIB_H
#define USB_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Standard C Includes (garante tipos padrão antes dos headers legados) ------*/
#include <stdint.h>
#include <stdbool.h>

/* Camada de Hardware e Tipos Base -------------------------------------------*/
#include "hw_config.h"  /* Configuração de relógio (48MHz), pinos PA11/PA12 e NVIC */
#include "usb_type.h"   /* Tipos específicos da biblioteca USB da ST */
#include "usb_regs.h"   /* Mapeamento de registos de hardware do periférico USB */

/* Camada de Protocolo USB 2.0 (Núcleo) --------------------------------------*/
#include "usb_def.h"    /* Constantes e máscaras do Capítulo 9 (Setup Packets) */
#include "usb_core.h"   /* Máquina de estados do Endpoint 0 e callbacks de classe */
#include "usb_init.h"   /* Rotina USB_Init() e ponteiros globais de contexto */

/* Camada de Interface Simplificada (SIL), Memória PMA e Interrupções --------*/
#include "usb_sil.h"    /* Funções USB_SIL_Read() e USB_SIL_Write() usadas no main */
#include "usb_mem.h"    /* Transferência entre RAM e Packet Memory Area (PMA) */
#include "usb_int.h"    /* Handlers de baixo nível para interrupções de endpoints */

#ifdef __cplusplus
}
#endif

#endif /* USB_LIB_H *//**
 ******************************************************************************
 * @file    usb_lib.h
 * @author  MCD Application Team (Comentado e Atualizado)
 * @version V4.1.0
 * @date    2026
 * @brief   Cabeçalho mestre (Umbrella Header) da biblioteca STM32 USB-FS.
 *          Agrupa hardware, registos, protocolo core, SIL e gestão de memória.
 ******************************************************************************
 */

#ifndef USB_LIB_H
#define USB_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Standard C Includes (garante tipos padrão antes dos headers legados) ------*/
#include <stdint.h>
#include <stdbool.h>

/* Camada de Hardware e Tipos Base -------------------------------------------*/
#include "hw_config.h"  /* Configuração de relógio (48MHz), pinos PA11/PA12 e NVIC */
#include "usb_type.h"   /* Tipos específicos da biblioteca USB da ST */
#include "usb_regs.h"   /* Mapeamento de registos de hardware do periférico USB */

/* Camada de Protocolo USB 2.0 (Núcleo) --------------------------------------*/
#include "usb_def.h"    /* Constantes e máscaras do Capítulo 9 (Setup Packets) */
#include "usb_core.h"   /* Máquina de estados do Endpoint 0 e callbacks de classe */
#include "usb_init.h"   /* Rotina USB_Init() e ponteiros globais de contexto */

/* Camada de Interface Simplificada (SIL), Memória PMA e Interrupções --------*/
#include "usb_sil.h"    /* Funções USB_SIL_Read() e USB_SIL_Write() usadas no main */
#include "usb_mem.h"    /* Transferência entre RAM e Packet Memory Area (PMA) */
#include "usb_int.h"    /* Handlers de baixo nível para interrupções de endpoints */

#ifdef __cplusplus
}
#endif

#endif /* USB_LIB_H */