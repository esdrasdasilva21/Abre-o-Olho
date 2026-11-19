/**
 ******************************************************************************
 * @file    usb_def.h
 * @author  MCD Application Team (Comentado e Atualizado)
 * @version V4.1.0
 * @date    2026
 * @brief   Definições fundamentais do protocolo USB 2.0 (Capítulo 9).
 *          Contém enumerações e máscaras para decodificar Setup Packets.
 ******************************************************************************
 */

#ifndef USB_DEF_H
#define USB_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Destinatário do pedido USB (Bits 0..4 do campo bmRequestType).
 *        Indica a que componente do dispositivo o comando se destina.
 */
typedef enum _RECIPIENT_TYPE
{
  DEVICE_RECIPIENT = 0,     /* O pedido destina-se ao dispositivo USB como um todo */
  INTERFACE_RECIPIENT,      /* O pedido destina-se a uma interface específica (ex: Interface HID) */
  ENDPOINT_RECIPIENT,       /* O pedido destina-se a um endpoint específico (ex: EP1 IN/OUT) */
  OTHER_RECIPIENT           /* Outro destinatário definido pelo protocolo */
} RECIPIENT_TYPE;

/**
 * @brief Códigos dos Pedidos Padrão USB (campo bRequest).
 *        Usados principalmente pelo sistema operativo durante a enumeração do dispositivo.
 */
typedef enum _STANDARD_REQUESTS
{
  GET_STATUS = 0,           /* Lê o estado atual do dispositivo, interface ou endpoint */
  CLEAR_FEATURE,            /* Desativa uma funcionalidade (ex: limpa condição de STALL) */
  RESERVED1,                /* Reservado pela norma USB */
  SET_FEATURE,              /* Ativa uma funcionalidade (ex: Remote Wakeup ou Halt) */
  RESERVED2,                /* Reservado pela norma USB */
  SET_ADDRESS,              /* Atribui um endereço único (1..127) ao dispositivo no barramento */
  GET_DESCRIPTOR,           /* Solicita um descritor (Device, Config, String, HID Report, etc.) */
  SET_DESCRIPTOR,           /* Atualiza ou adiciona um descritor (raramente usado) */
  GET_CONFIGURATION,        /* Verifica qual a configuração ativa no momento */
  SET_CONFIGURATION,        /* Ativa a configuração do dispositivo (coloca-o no estado CONFIGURED) */
  GET_INTERFACE,            /* Lê a configuração alternativa ativa de uma interface */
  SET_INTERFACE,            /* Seleciona uma configuração alternativa para uma interface */
  TOTAL_sREQUEST,           /* Sentinel: número total de pedidos padrão suportados na tabela (12) */
  SYNCH_FRAME = 12          /* Usado apenas em endpoints isócronos para sincronização */
} STANDARD_REQUESTS;

/**
 * @brief Tipos de Descritores USB padrão (byte alto do campo wValue em GET_DESCRIPTOR).
 */
typedef enum _DESCRIPTOR_TYPE
{
  DEVICE_DESCRIPTOR = 1,    /* Descritor principal (VID, PID, versão USB, classe) */
  CONFIG_DESCRIPTOR,        /* Descritor de configuração (consumo de energia, nº de interfaces) */
  STRING_DESCRIPTOR,        /* Textos legíveis (Fabricante, Produto, Número de Série) */
  INTERFACE_DESCRIPTOR,     /* Define a classe da interface (ex: 0x03 para HID) */
  ENDPOINT_DESCRIPTOR       /* Define direção, tipo (Interrupt/Bulk) e tamanho máximo de pacote */
} DESCRIPTOR_TYPE;

/**
 * @brief Seletores de funcionalidade para os pedidos SET_FEATURE e CLEAR_FEATURE.
 */
typedef enum _FEATURE_SELECTOR
{
  ENDPOINT_STALL = 0,       /* Bloqueia/desbloqueia um endpoint por erro ou controle de fluxo */
  DEVICE_REMOTE_WAKEUP = 1  /* Permite que o dispositivo acorde o PC do modo de suspensão */
} FEATURE_SELECTOR;

/* Exported constants --------------------------------------------------------*/

/**
 * @brief Máscaras e valores para interpretar o byte bmRequestType de um Setup Packet:
 *        - Bit 7:    Direção (0 = Host-to-Device, 1 = Device-to-Host)
 *        - Bits 6..5: Tipo de Pedido (máscara REQUEST_TYPE = 0x60)
 *        - Bits 4..0: Destinatário (máscara RECIPIENT = 0x1F)
 */
#define REQUEST_TYPE      0x60  /* Máscara binária (0110 0000) para extrair o tipo de pedido */
#define STANDARD_REQUEST  0x00  /* Pedido padrão USB 2.0 (enumeração e controle básico) */
#define CLASS_REQUEST     0x20  /* Pedido específico de classe (ex: HID Get_Report / Set_Report) */
#define VENDOR_REQUEST    0x40  /* Pedido proprietário definido pelo fabricante */

#define RECIPIENT         0x1F  /* Máscara binária (0001 1111) para extrair o destinatário */

/* Exported macro ------------------------------------------------------------*/
/**
 * @brief Macros auxiliares para facilitar a leitura do bmRequestType no código C.
 */
#define USB_GET_REQUEST_TYPE(bmReqType)  ((bmReqType) & REQUEST_TYPE)
#define USB_GET_RECIPIENT(bmReqType)     ((bmReqType) & RECIPIENT)
#define USB_IS_DEVICE_TO_HOST(bmReqType) (((bmReqType) & 0x80U) != 0U)

#ifdef __cplusplus
}
#endif

#endif /* USB_DEF_H */