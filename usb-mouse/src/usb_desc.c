/**
 ******************************************************************************
 * @file    usb_desc.c
 * @author  MCD Application Team (Comentado e Otimizado)
 * @version V1.2.0
 * @date    2026
 * @brief   Descritores USB HID para o projeto Open DoVision (ODV).
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "odvTypes.h"

/* USB Standard Device Descriptor */
const uint8_t Hid_DeviceDescriptor[HID_SIZ_DEVICE_DESC] =
  {
    0x12,                       /* bLength: Tamanho do descritor (18 bytes) */
    USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType: DEVICE */
    0x00,                       /* bcdUSB: Versão USB 2.0 */
    0x02,
    0x00,                       /* bDeviceClass: Definido na interface */
    0x00,                       /* bDeviceSubClass */
    0x00,                       /* bDeviceProtocol */
    0x40,                       /* bMaxPacketSize: 64 bytes para o Endpoint 0 */
    0x21,                       /* idVendor: DotLab (0x6021) */
    0x60,
    0x01,                       /* idProduct: ODV (0x0001) */
    0x00,
    0x01,                       /* bcdDevice: Versão do firmware 0.01 */
    0x00,
    1,                          /* iManufacturer: Índice da string do fabricante */
    2,                          /* iProduct: Índice da string do produto */
    3,                          /* iSerialNumber: Índice da string do número de série */
    0x01                        /* bNumConfigurations: 1 configuração suportada */
  };

/* USB Configuration Descriptor (Configuração + Interface + HID + Endpoints) */
const uint8_t Hid_ConfigDescriptor[HID_SIZ_CONFIG_DESC] =
  {
    /* --- Descritor de Configuração --- */
    0x09,                       /* bLength: Tamanho (9 bytes) */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,
    HID_SIZ_CONFIG_DESC,        /* wTotalLength: Tamanho total combinado */
    0x00,
    0x01,                       /* bNumInterfaces: 1 interface */
    0x01,                       /* bConfigurationValue */
    0x00,                       /* iConfiguration: Sem string descritiva */
    0xE0,                       /* bmAttributes: Self-powered / Bus-powered com Remote Wakeup */
    0xFA,                       /* MaxPower: 500 mA (500 * 2mA = 1000mA máx / 500mA especificado) */

    /* --- Descritor de Interface HID --- */
    0x09,                       /* bLength: Tamanho (9 bytes) */
    USB_INTERFACE_DESCRIPTOR_TYPE,
    0x00,                       /* bInterfaceNumber: Interface 0 */
    0x00,                       /* bAlternateSetting: 0 */
    0x02,                       /* bNumEndpoints: 2 endpoints (1 IN, 1 OUT) */
    0x03,                       /* bInterfaceClass: 0x03 = HID (Human Interface Device) */
    0x00,                       /* bInterfaceSubClass: 0 = No Boot */
    0x00,                       /* nInterfaceProtocol: 0 = None */
    0x00,                       /* iInterface: Sem string */

    /* --- Descritor específico da classe HID --- */
    0x09,                       /* bLength: Tamanho (9 bytes) */
    HID_DESCRIPTOR_TYPE,        /* bDescriptorType: HID (0x21) */
    0x11,                       /* bcdHID: Versão da especificação HID 1.11 */
    0x01,
    0x00,                       /* bCountryCode: 0 = Não suportado / Indefinido */
    0x01,                       /* bNumDescriptors: 1 descritor de relatório a seguir */
    0x22,                       /* bDescriptorType: Report Descriptor (0x22) */
    HID_SIZ_REPORT_DESC,        /* wItemLength: Tamanho total do Report Descriptor */
    0x00,

    /* --- Descritor do Endpoint Interrupt IN (0x81) --- */
    0x07,                       /* bLength: Tamanho (7 bytes) */
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x81,                       /* bEndpointAddress: EP1 IN (Direção Dispositivo -> Host) */
    0x03,                       /* bmAttributes: Tipo Interrupt */
    64,                         /* wMaxPacketSize: 64 bytes por pacote */
    0x00,
    0x01,                       /* bInterval: Polling de 1 ms */

    /* --- Descritor do Endpoint Interrupt OUT (0x01) --- */
    0x07,                       /* bLength: Tamanho (7 bytes) */
    USB_ENDPOINT_DESCRIPTOR_TYPE,
    0x01,                       /* bEndpointAddress: EP1 OUT (Direção Host -> Dispositivo) */
    0x03,                       /* bmAttributes: Tipo Interrupt */
    USB_PACKET_SIZE,            /* wMaxPacketSize: Definido em odvTypes.h */
    0x00,
    0x80                        /* bInterval: Polling de 128 ms */
  };

/* HID Report Descriptor (Define a estrutura lógica dos dados trocados) */
const uint8_t Hid_ReportDescriptor[HID_SIZ_REPORT_DESC] =
  {
    0x06, 0x00, 0xff,             /* USAGE_PAGE (Vendor Defined Page 1) */
    0x09, 0x01,                   /* USAGE (Vendor Usage 1) */
    0xa1, 0x01,                   /* COLLECTION (Application) */

    /* --- Report ID 0x01: Feature_Check --- */
    0x15, 0x00,                   /* LOGICAL_MINIMUM (0) */
    0x26, 0xff, 0x00,             /* LOGICAL_MAXIMUM (255) */
    0x75, 0x08,                   /* REPORT_SIZE (8 bits) */
    0x85, 0x01,                   /* REPORT_ID (0x01) */
    0x95, (FeaCheck_Size - 1),    /* REPORT_COUNT */
    0x09, 0x01,                   /* USAGE (Feature_Check) */
    0xb2, 0x02, 0x01,             /* FEATURE (Data, Var, Abs, Buffer) */

    /* --- Report ID 0x02: Feature_Renew --- */
    0x85, 0x02,                   /* REPORT_ID (0x02) */
    0x95, (FeaRenew_Size - 1),    /* REPORT_COUNT */
    0x09, 0x02,                   /* USAGE (Feature_Renew) */
    0xb2, 0x02, 0x01,             /* FEATURE (Data, Var, Abs, Buffer) */

    /* --- Report ID 0x03: Feature_Config --- */
    0x85, 0x03,                   /* REPORT_ID (0x03) */
    0x95, (FeaConfig_Size - 1),   /* REPORT_COUNT */
    0x09, 0x03,                   /* USAGE (Feature_Config) */
    0xb2, 0x02, 0x01,             /* FEATURE (Data, Var, Abs, Buffer) */

    /* --- Report ID 0x11: Out_Data (Telemetria IMU) --- */
    0x85, 0x11,                   /* REPORT_ID (0x11) */
    0x95, (OutData_Size - 1),     /* REPORT_COUNT */
    0x09, 0x11,                   /* USAGE (Out_Data) */
    0x82, 0x02, 0x01,             /* INPUT (Data, Var, Abs, Buffer) */

    0xc0                          /* END_COLLECTION */
  };

/* USB String Descriptors */
const uint8_t Hid_StringLangID[HID_SIZ_STRING_LANGID] =
  {
    HID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09, 0x04                    /* LangID = 0x0409: English (United States) */
  };

const uint8_t Hid_StringVendor[HID_SIZ_STRING_VENDOR] =
  {
    HID_SIZ_STRING_VENDOR,
    USB_STRING_DESCRIPTOR_TYPE,
    'D', 0, 'o', 0, 't', 0, 'L', 0, 'a', 0, 'b', 0  /* "DotLab" */
  };

const uint8_t Hid_StringProduct[HID_SIZ_STRING_PRODUCT] =
  {
    HID_SIZ_STRING_PRODUCT,
    USB_STRING_DESCRIPTOR_TYPE,
    'O', 0, 'D', 0, 'V', 0                        /* "ODV" */
  };

uint8_t Hid_StringSerial[HID_SIZ_STRING_SERIAL] =
  {
    HID_SIZ_STRING_SERIAL,
    USB_STRING_DESCRIPTOR_TYPE,
    '0', 0, 'X', 0, 'O', 0, 'O', 0                /* "0XOO" (Atualizado dinamicamente por Get_SerialNum) */
  };