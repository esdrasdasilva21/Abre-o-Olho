/**
 ******************************************************************************
 * @file    usb_mem.h
 * @author  MCD Application Team (Melhorado e Comentado)
 * @version V4.1.0
 * @date    2026
 * @brief   Protótipos e utilitários para transferência de dados entre a SRAM
 *          da aplicação e a Packet Memory Area (PMA) dedicada do periférico USB.
 ******************************************************************************
 */

#ifndef USB_MEM_H
#define USB_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/

/** @brief Tamanho total da Packet Memory Area (PMA) no STM32F30x (512 bytes). */
#define USB_PMA_TOTAL_SIZE      512U

/** @brief Tamanho máximo de pacote suportado por endpoint Full-Speed HID (64 bytes). */
#define USB_HID_MAX_PACKET_SIZE 64U

/* Exported macro ------------------------------------------------------------*/

/**
 * @brief Verifica se um endereço de buffer na PMA está alinhado a 16 bits
 *        (requisito obrigatório do controlador USB-FS do STM32F3).
 */
#define USB_IS_PMA_ADDR_ALIGNED(addr) (((addr) & 0x01U) == 0U)

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Copia um bloco de bytes da SRAM do utilizador para a PMA do USB
 *         (usado nas transmissões IN: Dispositivo -> Computador/TypeScript).
 *
 * @param  pbUsrBuf    Ponteiro constante para o buffer de origem na SRAM/Flash.
 * @param  wPMABufAddr Endereço de destino dentro da PMA (deve ser par/alinhado).
 * @param  wNBytes     Número de bytes a transferir.
 */
void UserToPMABufferCopy(const uint8_t *pbUsrBuf, uint16_t wPMABufAddr, uint16_t wNBytes);

/**
 * @brief  Copia um bloco de bytes da PMA do USB para a SRAM do utilizador
 *         (usado nas receções OUT / Feature Reports: Computador -> Dispositivo).
 *
 * @param  pbUsrBuf    Ponteiro para o buffer de destino na SRAM.
 * @param  wPMABufAddr Endereço de origem dentro da PMA (deve ser par/alinhado).
 * @param  wNBytes     Número de bytes a transferir.
 */
void PMAToUserBufferCopy(uint8_t *pbUsrBuf, uint16_t wPMABufAddr, uint16_t wNBytes);

#ifdef __cplusplus
}
#endif

#endif /* USB_MEM_H */