/**
 ******************************************************************************
 * @file    usb_sil.h
 * @author  MCD Application Team (Melhorado e Comentado)
 * @version V4.1.0
 * @date    2026
 * @brief   Simplified Interface Layer (SIL) para leitura e escrita em Endpoints USB.
 *          Abstrai o acesso aos registos de hardware e à memória PMA.
 ******************************************************************************
 */

#ifndef USB_SIL_H
#define USB_SIL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported constants --------------------------------------------------------*/

/** @brief Máscara de direção do endereço do endpoint (Bit 7: 1 = IN, 0 = OUT). */
#define USB_EP_DIR_MASK         0x80U
#define USB_EP_DIR_IN           0x80U
#define USB_EP_DIR_OUT          0x00U

/** @brief Máscara para extrair o número do endpoint (Bits 0..3). */
#define USB_EP_NUM_MASK         0x0FU

/* Exported macro ------------------------------------------------------------*/

/** @brief Verifica se o endereço corresponde a um endpoint IN (Dispositivo -> Host). */
#define USB_IS_EP_IN(epAddr)    (((epAddr) & USB_EP_DIR_MASK) == USB_EP_DIR_IN)

/** @brief Verifica se o endereço corresponde a um endpoint OUT (Host -> Dispositivo). */
#define USB_IS_EP_OUT(epAddr)   (((epAddr) & USB_EP_DIR_MASK) == USB_EP_DIR_OUT)

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  Inicializa o periférico USB e limpa os flags de interrupção pendentes.
 * @retval 0 em caso de sucesso.
 */
uint32_t USB_SIL_Init(void);

/**
 * @brief  Escreve dados da SRAM no buffer PMA de um endpoint IN e prepara a
 *         contagem de bytes para transmissão ao Host (ex: envio de sensores HID).
 *
 * @note   Após chamar esta função, é necessário validar o estado Tx do endpoint
 *         chamando SetEPTxValid(ENDPx).
 *
 * @param  bEpAddr        Endereço do endpoint IN (ex: EP1_IN = 0x81).
 * @param  pBufferPointer Ponteiro constante para os dados a enviar.
 * @param  wBufferSize    Número de bytes a escrever (não deve exceder MaxPacketSize).
 * @retval 0 em caso de sucesso.
 */
uint32_t USB_SIL_Write(uint8_t bEpAddr, const uint8_t *pBufferPointer, uint32_t wBufferSize);

/**
 * @brief  Lê os dados recebidos na PMA de um endpoint OUT para a SRAM da aplicação.
 *
 * @note   O buffer de destino deve ter capacidade igual ou superior ao MaxPacketSize
 *         configurado para o endpoint (64 bytes no USB Full-Speed HID).
 *         Após a leitura, valide a receção com SetEPRxValid(ENDPx).
 *
 * @param  bEpAddr        Endereço do endpoint OUT (ex: EP1_OUT = 0x01).
 * @param  pBufferPointer Ponteiro para o buffer de destino na SRAM.
 * @retval Número de bytes efetivamente lidos do endpoint.
 */
uint32_t USB_SIL_Read(uint8_t bEpAddr, uint8_t *pBufferPointer);

#ifdef __cplusplus
}
#endif

#endif /* USB_SIL_H */