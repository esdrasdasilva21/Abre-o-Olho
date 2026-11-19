/**
 ******************************************************************************
 * @file    globalVars.h
 * @author  Kailang@DotLab (Melhorado e Comentado)
 * @version V0.1.0
 * @date    2026
 * @brief   Variáveis globais, buffers USB HID e mapeamento de eixos dos sensores
 *          para o projeto Open DoVision.
 * @note    Licensed under GNU GPL V3.0 License.
 ******************************************************************************
 */

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f30x.h"  /* Garante a definição da macro __IO (volatile) */
#include "odvTypes.h"

/* Exported constants --------------------------------------------------------*/

/** @brief Tamanho padrão dos buffers de transferência USB (EP0 e EP1). */
#define USB_STREAM_BUFFER_SIZE      128U

/** @brief Palavra mágica para validação de pacotes Feature_Check e Feature_Renew. */
#define Magic_Word                  ((uint32_t)0x19980317U)

/** @brief Endereço base na memória Flash para armazenamento persistente de dados. */
#define Uint32_Flash_Data_Addr      ((uint32_t)0x08010000U)

/* Exported types (Mapeamento de Eixos dos Sensores) -------------------------*/

/** @brief Índices universais de coordenadas finais (alinhadas ao dispositivo). */
typedef enum {
    X = 0,
    Y = 1,
    Z = 2
} UniCoordinate;

/** @brief Índices do buffer bruto do Giroscópio (ordem padrão XYZ). */
typedef enum {
    gyrX = 0,
    gyrY = 1,
    gyrZ = 2
} GyrCoordinate;

/** @brief Índices do buffer bruto do Acelerómetro (ordem padrão XYZ). */
typedef enum {
    accX = 0,
    accY = 1,
    accZ = 2
} AccCoordinate;

/**
 * @brief Índices do buffer bruto do Magnetómetro (LSM303DLHC).
 * @note  O sensor entrega os registos na ordem física X, Z, Y (Y=2, Z=1).
 */
typedef enum {
    magX = 0,
    magY = 2,
    magZ = 1
} MagCoordinate;

/** @brief Índices de coordenadas para emulação de rato HID. */
typedef enum {
    mouX = 1,
    mouY = 2
} MouCoordinate;

/* Exported variables --------------------------------------------------------*/

/**
 * @brief Buffers de comunicação USB HID:
 *        - EP0 (0x00): Control Pipe (Feature Reports de configuração/estado)
 *        - EP1 (0x81 IN / 0x01 OUT): Interrupt Pipe (Streaming de sensores)
 *        Nota: A nomenclatura IN/OUT na norma HID segue o ponto de vista do PC (Host).
 */
extern uint8_t feaRawStream[USB_STREAM_BUFFER_SIZE];    /* Buffer bruto de Feature Reports (EP0) */
extern uint8_t EP0Buffer[USB_STREAM_BUFFER_SIZE];       /* Buffer auxiliar de controlo (EP0) */
extern uint8_t inRawStream[USB_STREAM_BUFFER_SIZE];     /* Buffer bruto de relatórios de entrada (EP1) */
extern uint8_t EP1Buffer[USB_STREAM_BUFFER_SIZE];       /* Buffer auxiliar de dados (EP1) */

/** @brief Estruturas empacotadas (#pragma pack(1)) dos frames HID. */
extern OutDataFrameSep   outData;       /* Pacote de telemetria enviado ao PC (EP1 IN) */
extern FeaCheckFrameSep  feaCheck;      /* Pacote de verificação de conexão (Report ID 0x01) */
extern FeaRenewFrameSep  feaRenew;      /* Pacote de renovação de sessão / reset counter (ID 0x02) */
extern FeaConfigFrameSep feaConfig;     /* Pacote de configuração de escalas e intervalos (ID 0x03) */

/** @brief Flags de estado das transferências USB (modificadas dentro de ISRs). */
extern __IO uint8_t EP1OutState;        /* 1 = Pacote enviado e pronto para novo envio; 0 = Ocupado */
extern __IO uint8_t EP1InState;         /* 1 = Novos dados recebidos do PC no EP1; 0 = Vazio */
extern __IO int8_t  EP0FeaState;        /* ID do Feature Report recebido no EP0 (-1 = Nenhum) */

/** @brief Estado da máquina de estados principal (State_Idle / State_Track). */
extern uint8_t odvState;

/** @brief Indicador de prontidão dos sensores (0 = Prontos; != 0 = Não prontos). */
extern __IO uint8_t senDatReady;

/** @brief Contadores de tempo incrementados a cada 1 ms pelo SysTick_Handler. */
extern __IO uint32_t timeSinceRenew;    /* Tempo decorrido desde o último Feature Report (ms) */
extern __IO uint32_t timeSinceSend;     /* Tempo decorrido desde o último envio de telemetria (ms) */
extern __IO uint32_t timeSinceRead;     /* Tempo decorrido desde a última leitura dos sensores (ms) */
extern __IO uint32_t timerDelay;        /* Contador regressivo usado pela função Delay() (ms) */

/** @brief Buffers de leitura em ponto flutuante dos sensores IMU. */
extern float   gyrBuffer[3];            /* Giroscópio XYZ (deg/s) */
extern float   accBuffer[3];            /* Acelerómetro XYZ (0.01 m/s^2) */
extern float   magBuffer[3];            /* Magnetómetro XZY (0.001 Gauss) */
extern int16_t tempBuffer;              /* Temperatura interna do sensor */

/** @brief Parâmetros ativos de amostragem e escala configuráveis pelo Host (TypeScript). */
extern __IO uint32_t curDueTime;        /* Timeout de inatividade para voltar a State_Idle (ms) */
extern __IO uint16_t curMesInterval;    /* Intervalo de envio de pacotes HID (ms) */
extern __IO uint16_t curGyrScale;       /* Escala ativa do giroscópio */
extern __IO uint16_t curAccScale;       /* Escala ativa do acelerómetro */
extern __IO uint16_t curMagScale;       /* Escala ativa do magnetómetro */
extern __IO uint8_t  curGyrUnit;        /* Unidade de medida do giroscópio */
extern __IO uint8_t  curAccUnit;        /* Unidade de medida do acelerómetro */
extern __IO uint8_t  curMagUnit;        /* Unidade de medida do magnetómetro */

#ifdef __cplusplus
}
#endif

#endif /* GLOBALVARS_H_ */