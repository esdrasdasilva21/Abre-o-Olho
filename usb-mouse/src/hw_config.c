/**
 ******************************************************************************
 * @file    hw_config.c
 * @author  MCD Application Team (Atualizado)
 * @version V1.2.0
 * @date    2026
 * @brief   Hardware Configuration & Setup for STM32F3 USB HID.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>

/* Private define ------------------------------------------------------------*/
#define CLOCK_STARTUP_TIMEOUT   ((uint32_t)0x00005000)
#define UID_BASE_ADDRESS        ((uint32_t)0x1FFFF7E8)

/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;

/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Configures Main system clocks & power.
 */
void Set_System(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable PWR and SYSCFG clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Configure PA11 (USB_DM) and PA12 (USB_DP) */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_14);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_14);

#if defined(USB_USE_EXTERNAL_PULLUP)
    /* Enable the USB disconnect GPIO clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_DISCONNECT, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = USB_DISCONNECT_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
#endif /* USB_USE_EXTERNAL_PULLUP */

    /* Configure EXTI line 18 connected internally to USB Wakeup */
    EXTI_ClearITPendingBit(EXTI_Line18);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line    = EXTI_Line18;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
}

/**
 * @brief  Configures USB Clock input (48MHz).
 */
void Set_USBClock(void) {
    /* USBCLK = PLLCLK / 1.5 = 48 MHz (assuming 72 MHz PLL) */
    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/**
 * @brief  Configures unused IOs as Analog Input to reduce power consumption.
 */
void GPIO_AINConfig(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ALLGPIO, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ALLGPIO, DISABLE);
}

/**
 * @brief  Prepares device state when entering USB suspend mode.
 */
void Enter_LowPowerMode(void) {
    bDeviceState = SUSPENDED;

    /* Clear both USB Wakeup (Line 18) and User Button pending bits */
    EXTI_ClearITPendingBit(EXTI_Line18);
    EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
}

/**
 * @brief  Restores system clocks and power when exiting suspend mode.
 */
void Leave_LowPowerMode(void) {
    DEVICE_INFO *pInfo = &Device_Info;
    volatile uint32_t timeout = 0;

    /* Enable HSE and wait with timeout */
    RCC_HSEConfig(RCC_HSE_ON);
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS) {
        /* Enable PLL1 */
        RCC_PLLCmd(ENABLE);
        timeout = 0;
        while ((RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) && (timeout < CLOCK_STARTUP_TIMEOUT)) {
            timeout++;
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        timeout = 0;
        while ((RCC_GetSYSCLKSource() != 0x08) && (timeout < CLOCK_STARTUP_TIMEOUT)) {
            timeout++;
        }
    }

    /* Restore USB device state */
    bDeviceState = (pInfo->Current_Configuration != 0) ? CONFIGURED : ATTACHED;
}

/**
 * @brief  Configures the USB interrupts.
 */
void USB_Interrupts_Config(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* USB Low Priority Interrupt */
#if defined(USB_INT_DEFAULT)
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
#elif defined(USB_INT_REMAP)
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_IRQn;
#endif
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* USB Wake-up Interrupt */
#if defined(USB_INT_DEFAULT)
    NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
#elif defined(USB_INT_REMAP)
    NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_RMP_IRQn;
#endif
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* User Button EXTI Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = USER_BUTTON_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  Software Connection/Disconnection of USB Cable.
 */
void USB_Cable_Config(FunctionalState NewState) {
#if defined(USB_USE_EXTERNAL_PULLUP)
    if (NewState != DISABLE) {
        GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
    } else {
        GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
    }
#else
    (void)NewState; /* Avoid unused parameter warning if internal pull-up is used */
#endif
}

/**
 * @brief  Creates the serial number string descriptor from STM32 Unique ID.
 */
void Get_SerialNum(void) {
    uint32_t Device_Serial0 = *(__IO uint32_t *)(UID_BASE_ADDRESS);
    uint32_t Device_Serial1 = *(__IO uint32_t *)(UID_BASE_ADDRESS + 0x04);
    uint32_t Device_Serial2 = *(__IO uint32_t *)(UID_BASE_ADDRESS + 0x08);

    Device_Serial0 += Device_Serial2;

    if (Device_Serial0 != 0) {
        IntToUnicode(Device_Serial0, &Hid_StringSerial[2], 8);
        IntToUnicode(Device_Serial1, &Hid_StringSerial[18], 4);
    }
}

/**
 * @brief  Converts a 32-bit hex value into UTF-16LE Unicode string descriptor.
 */
static void IntToUnicode(uint32_t value, uint8_t *pbuf, uint8_t len) {
    for (uint8_t idx = 0; idx < len; idx++) {
        uint8_t nibble = (uint8_t)(value >> 28);
        pbuf[2 * idx]     = (nibble < 0xA) ? (nibble + '0') : (nibble + 'A' - 10);
        pbuf[2 * idx + 1] = 0;
        value <<= 4;
    }
}
