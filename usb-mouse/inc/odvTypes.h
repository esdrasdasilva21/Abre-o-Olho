/**
 ******************************************************************************
 * @file    odvTypes.h
 * @author  Kailang@DotLab (Melhorado e Comentado)
 * @version V0.1.0
 * @date    2026
 * @brief   Definições de tipos, tamanhos de pacotes e estruturas dos relatórios HID
 *          para o Open DoVision.
 * @note    Licensed under GNU GPL V3.0 License.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ODVTYPES_H_
#define ODVTYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported constants (Tamanhos exatos dos buffers em bytes) -----------------*/
#define OutData_Size            ((uint8_t)40)   /* 1 (type) + 2 (count) + 1 (temp) + 36 (3x3x4 bytes de dados IMU) */
#define FeaCheck_Size           ((uint8_t)5)    /* 1 (type) + 4 (magicWord) */
#define FeaRenew_Size           ((uint8_t)5)    /* 1 (type) + 4 (magicWord) */
#define FeaConfig_Size          ((uint8_t)16)   /* 1 (type) + 4 (dueTime) + 2*4 (interval/scales) + 3 (units) */

/* Exported types (Estruturas internas dos pacotes) ---------------------------*/

/** @brief Estrutura dos dados de telemetria enviados no Endpoint 1 (OUT/INPUT para o PC). */
typedef struct OutDataFrame {
    uint8_t     messageType;    /* Tipo de mensagem (0x11 para Out_Data) */
    uint16_t    count;          /* Contador incremental de pacotes */
    int8_t      temp;           /* Temperatura interna do sensor */
    int32_t     gyrData[3];     /* Dados XYZ do Giroscópio (convertidos com escala) */
    int32_t     accData[3];     /* Dados XYZ do Acelerómetro */
    int32_t     magData[3];     /* Dados XYZ do Magnetómetro */
} OutDataFrame;

/** @brief Estrutura do Feature Report de Verificação (Report ID 0x01). */
typedef struct FeaCheckFrame {
    uint8_t     reportType;     /* 0x01 */
    uint32_t    magicWord;      /* Palavra mágica de validação (Magic_Word) */
} FeaCheckFrame;

/** @brief Estrutura do Feature Report de Renovação / Reset (Report ID 0x02). */
typedef struct FeaRenewFrame {
    uint8_t     reportType;     /* 0x02 */
    uint32_t    magicWord;      /* Palavra mágica */
} FeaRenewFrame;

/** @brief Estrutura do Feature Report de Configuração (Report ID 0x03). */
typedef struct FeaConfigFrame {
    uint8_t     reportType;     /* 0x03 */
    uint32_t    dueTime;        /* Tempo limite de sessão (ms) */
    uint16_t    mesInterval;    /* Intervalo entre medições de telemetria (ms) */
    uint16_t    gyrScale;       /* Escala do giroscópio */
    uint16_t    accScale;       /* Escala do acelerómetro */
    uint16_t    magScale;       /* Escala do magnetómetro */
    uint8_t     gyrUnit;        /* Unidade do giroscópio */
    uint8_t     accUnit;        /* Unidade do acelerómetro */
    uint8_t     magUnit;        /* Unidade do magnetómetro */
} FeaConfigFrame;

/* Exported Unions (Permitem acesso direto aos bytes crus ou à estrutura tipada) */

typedef union OutDataFrameSep {
    uint8_t         rawStream[OutData_Size];
    OutDataFrame    frame;
} OutDataFrameSep;

typedef union FeaCheckFrameSep {
    uint8_t         rawStream[FeaCheck_Size];
    FeaCheckFrame   frame;
} FeaCheckFrameSep;

typedef union FeaRenewFrameSep {
    uint8_t         rawStream[FeaRenew_Size];
    FeaRenewFrame   frame;
} FeaRenewFrameSep;

typedef union FeaConfigFrameSep {
    uint8_t         rawStream[FeaConfig_Size];
    FeaConfigFrame  frame;
} FeaConfigFrameSep;

/* Enumerations --------------------------------------------------------------*/

/** @brief Identificadores dos tipos de relatórios (Feature e Out). */
typedef enum RepType {
    Feature_None    = 0x00,
    Feature_Check   = 0x01,     /* EP0 Feature Report: Verificação */
    Feature_Renew   = 0x02,     /* EP0 Feature Report: Renovação */
    Feature_Config  = 0x03,     /* EP0 Feature Report: Configuração */

    Out_None        = 0x10,
    Out_Data        = 0x11,     /* EP1 Relatório de Telemetria IMU */
} RepType;

/** @brief Estados de funcionamento da máquina de estados principal. */
typedef enum State {
    State_Idle      = 0x00,     /* Estado de repouso / aguardar ativação */
    State_Track     = 0x01,     /* Estado de leitura e transmissão ativa de sensores */
} State;

#ifdef __cplusplus
}
#endif

#endif /* ODVTYPES_H_ */