/**
 ******************************************************************************
 * @file    main.c
 * @author  Kailang@DotLab (Atualizado)
 * @version V0.0.2
 * @date    2026
 * @brief   Open DoVision HID Main
 ******************************************************************************
 * @note
 *
 * This is the project that can turn the Open DoVision into a customized HID
 * which sends and receives data through the HID protocol.
 *
 * odvTypes.h       - all the data types we need to process ODV data;
 * globalVars.h     - all the global variables like data holder and status
 *                    indicator we need to make ODV work;
 * peripherals.h    - all the useful functions for controlling the sensors and
 *                    other things like button and LED.
 *
 * Licensed under GNU GPL V3.0 License.
 ******************************************************************************
 */

/* Pragmas -------------------------------------------------------------------*/
#pragma pack(1)                         /* Align 1 byte */

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include "main.h"

/* Global variables ----------------------------------------------------------*/
uint8_t feaRawStream[128];              /* EP0 Feature Report Raw Stream */
uint8_t EP0Buffer[128];                 /* EP0 Unknown Data Buffer */
uint8_t inRawStream[128];               /* EP1 In Report Buffer */
uint8_t EP1Buffer[128];                 /* EP1 Unknown Data Buffer */

OutDataFrameSep     outData;            /* EP1 Out Report Holders */
FeaCheckFrameSep    feaCheck;           /* EP0 Feature Report Holders */
FeaRenewFrameSep    feaRenew;
FeaConfigFrameSep   feaConfig;

__IO uint8_t EP1OutState = 1;           /* EP1 Out Report Send Indicator: 1 = Sent */
__IO uint8_t EP1InState  = 0;           /* EP1 In Report Read Indicator: 0 = Not Read */
__IO int8_t  EP0FeaState = -1;          /* EP0 Feature Report Indicator: -1 = None */

__IO uint8_t senDatReady;               /* Sensor Data Ready Indicator: 0 = Ready */

__IO uint32_t timeSinceRenew;           /* Timer Count Since Renew Report */
__IO uint32_t timeSinceSend;            /* Timer Count Since Data Sent */
__IO uint32_t timeSinceRead;            /* Timer Count Since Read From Sensors */
__IO uint32_t timerDelay;               /* Timer Count Down To 0 */

float gyrBuffer[3];                     /* XYZ Gyroscope Data Buffer in deg/s */
float accBuffer[3];                     /* XYZ Accelerometer Data Buffer in 0.01 m/s2 */
float magBuffer[3];                     /* XZY Magnet Data Buffer in 0.001 Gauss */
int16_t tempBuffer;                     /* Temperature Data Buffer */

__IO uint32_t curDueTime;               /* Current Settings */
__IO uint16_t curMesInterval;
__IO uint16_t curGyrScale;
__IO uint16_t curAccScale;
__IO uint16_t curMagScale;
__IO uint8_t  curGyrUnit;
__IO uint8_t  curAccUnit;
__IO uint8_t  curMagUnit;

/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
uint8_t odvState = State_Idle;

/* Private function prototypes -----------------------------------------------*/
void AtoMemCopy(uint8_t *to, __IO uint8_t *from, size_t size);
int32_t FloToInt32(float flo, uint8_t movPoiLeft);

void USB_Config(void);
void ODV_Config(void);
uint8_t ButHandler(void);
uint8_t FeaHandler(void);
uint8_t OutHandler(uint8_t repType);
void OutPacker(uint8_t repType);

/* Private functions ---------------------------------------------------------*/

int main(void) {
    /* Setup SysTick event each 1 ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    if (SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000)) {
        return 1;
    }

    /* Configure the USB (waits until USB is plugged in and configured) */
    LED_Config();
    USB_Config();
    LED_Off();

    /* Initiate peripherals */
    ODV_Config();                       /* Restore Default Open DoVision Settings */
    Gyr_Init();
    Acc_Init();
    Mag_Init();

    /* Infinite loop */
    while (1) {
        /* Read from EP1 */
        if (EP1InState) {
            STM_EVAL_LEDToggle(LED5);

            USB_SIL_Read(EP1_OUT, EP1Buffer);
            SetEPRxValid(ENDP1);
            EP1InState = 0;
        }

        /* Handle Feature Report (if any) */
        FeaHandler();

        /* User Button Utility */
        ButHandler();

        /* Data Routine */
        switch (odvState) {
        case State_Idle:
            STM_EVAL_LEDOff(LED6);
            STM_EVAL_LEDToggle(LED7);
            Delay(100);
            break;

        case State_Track:
            STM_EVAL_LEDOff(LED7);
            STM_EVAL_LEDToggle(LED6);

            /* Read Sensor Data */
            senDatReady = 0;
            Gyr_Read(gyrBuffer);
            Acc_Read(accBuffer);
            Mag_Read(magBuffer);

            /* Send Data */
            if (timeSinceSend > curMesInterval) {
                OutHandler(Out_Data);
            }

            /* Check Due Timeout */
            if (timeSinceRenew > curDueTime) {
                LED_Dim(LED7);
                Delay(100);
                LED_Off();
                odvState = State_Idle;
            }
            break;

        default:
            odvState = State_Idle;
            break;
        }
    }
}

void USB_Config(void) {
    uint8_t i = 0;

    Set_System();
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

    while (bDeviceState != CONFIGURED) {
        i = (i < 0xFF) ? (i + 1) : 1;
        LED_Out(i);
        Delay(10);
    }
    LED_Off();
}

void ODV_Config(void) {
    curDueTime     = 3000;
    curMesInterval = 100;
}

uint8_t FeaHandler(void) {
    if (EP0FeaState < 0) {
        return 1;                                   /* No Data */
    } else if (EP0FeaState != feaRawStream[0]) {
        STM_EVAL_LEDToggle(LED10);
        EP0FeaState = -1;                           /* Glitch: Reset Report State */
        return 2;
    }

    STM_EVAL_LEDToggle(LED8);
    uint8_t repType = (uint8_t)EP0FeaState;
    EP0FeaState = -1;                               /* Reset Report State */

    switch (repType) {
    case Feature_None:
        /* Report ID 0x00 is reserved by HID Specification */
        break;

    case Feature_Check:
        AtoMemCopy(feaCheck.rawStream, feaRawStream, FeaCheck_Size);
        break;

    case Feature_Renew:
        AtoMemCopy(feaRenew.rawStream, feaRawStream, FeaRenew_Size);
        outData.frame.count = 0;
        break;

    case Feature_Config:
        AtoMemCopy(feaConfig.rawStream, feaRawStream, FeaConfig_Size);
        curDueTime     = feaConfig.frame.dueTime;
        curMesInterval = feaConfig.frame.mesInterval;
        curGyrScale    = feaConfig.frame.gyrScale;
        curAccScale    = feaConfig.frame.accScale;
        curMagScale    = feaConfig.frame.magScale;
        curGyrUnit     = feaConfig.frame.gyrUnit;
        curAccUnit     = feaConfig.frame.accUnit;
        curMagUnit     = feaConfig.frame.magUnit;
        break;
    }

    timeSinceRenew = 0;
    odvState = State_Track;

    return 0;
}

void FeaPacker(uint8_t repType) {
    STM_EVAL_LEDOn(LED9);

    switch (repType) {
    case Feature_None:
        break;

    case Feature_Check:
        feaCheck.frame.reportType  = 0x01;
        feaCheck.frame.magicWord   = Magic_Word;
        break;

    case Feature_Renew:
        feaRenew.frame.reportType  = 0x02;
        feaRenew.frame.magicWord   = Magic_Word;
        break;

    case Feature_Config:
        feaConfig.frame.reportType  = 0x03;
        feaConfig.frame.dueTime     = curDueTime;
        feaConfig.frame.mesInterval = curMesInterval;
        feaConfig.frame.gyrScale    = curGyrScale;
        feaConfig.frame.accScale    = curAccScale;
        feaConfig.frame.magScale    = curMagScale;
        feaConfig.frame.gyrUnit     = curGyrUnit;
        feaConfig.frame.accUnit     = curAccUnit;
        feaConfig.frame.magUnit     = curMagUnit;
        break;
    }
}

uint8_t OutHandler(uint8_t repType) {
    if (EP1OutState == 0) {
        STM_EVAL_LEDToggle(LED10);
        return 1;
    } else if (senDatReady != 0) {
        STM_EVAL_LEDToggle(LED10);
        return 2;
    }

    EP1OutState = 0;                                /* New Transmission */
    switch (repType) {
    case Out_None:
        break;

    case Out_Data:
        OutPacker(Out_Data);
        USB_SIL_Write(EP1_IN, outData.rawStream, OutData_Size);
        break;
    }

    SetEPTxValid(ENDP1);                            /* Validate EP1 Tx Status */
    timeSinceSend = 0;

    return 0;
}

void OutPacker(uint8_t repType) {
    switch (repType) {
    case Out_None:
        break;

    case Out_Data:
        outData.frame.messageType = 0x11;
        outData.frame.count++;                      /* Wrap-around automático de 16 bits */
        outData.frame.temp = tempBuffer;            /* Utiliza o buffer global de temperatura */

        /* Alinhamento de eixos dos sensores */
        outData.frame.gyrData[X] = FloToInt32( gyrBuffer[gyrX], 4);
        outData.frame.gyrData[Y] = FloToInt32( gyrBuffer[gyrY], 4);
        outData.frame.gyrData[Z] = FloToInt32( gyrBuffer[gyrZ], 4);

        outData.frame.accData[X] = FloToInt32( accBuffer[accY], 4);
        outData.frame.accData[Y] = FloToInt32(-accBuffer[accX], 4);
        outData.frame.accData[Z] = FloToInt32( accBuffer[accZ], 4);

        outData.frame.magData[X] = FloToInt32( magBuffer[magY], 4);
        outData.frame.magData[Y] = FloToInt32(-magBuffer[magX], 4);
        outData.frame.magData[Z] = FloToInt32( magBuffer[magZ], 4);
        break;
    }
}

uint8_t ButHandler(void) {
    return (But_Pres() == 0) ? 1 : 0;
}

void AtoMemCopy(uint8_t *to, __IO uint8_t *from, size_t size) {
    for (size_t i = 0; i < size; i++) {
        to[i] = from[i];
    }
}

int32_t FloToInt32(float flo, uint8_t movPoiLeft) {
    for (uint8_t i = 0; i < movPoiLeft; i++) {
        flo *= 10.0f;
    }
    return (int32_t)(flo >= 0.0f ? (flo + 0.5f) : (flo - 0.5f));
}