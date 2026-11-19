/**
 ******************************************************************************
 * @file    stm32f30x_crc.h
 * @author  MCD Application Team (Melhorado e Comentado)
 * @version V1.1.0
 * @date    2026
 * @brief   Protótipos e constantes de configuração do periférico CRC por hardware.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32F30X_CRC_H
#define STM32F30X_CRC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"

/** @addtogroup STM32F30x_StdPeriph_Driver
 * @{
 */

/** @addtogroup CRC
 * @{
 */

/* Exported constants --------------------------------------------------------*/

/** @defgroup CRC_ReverseInputData
 * @{
 */
#define CRC_ReverseInputData_No         ((uint32_t)0x00000000U) /* Sem inversão de bits de entrada */
#define CRC_ReverseInputData_8bits      CRC_CR_REV_IN_0         /* Inversão em blocos de 8 bits */
#define CRC_ReverseInputData_16bits     CRC_CR_REV_IN_1         /* Inversão em blocos de 16 bits */
#define CRC_ReverseInputData_32bits     CRC_CR_REV_IN           /* Inversão total de 32 bits */

#define IS_CRC_REVERSE_INPUT_DATA(DATA) (((DATA) == CRC_ReverseInputData_No)     || \
                                         ((DATA) == CRC_ReverseInputData_8bits)  || \
                                         ((DATA) == CRC_ReverseInputData_16bits) || \
                                         ((DATA) == CRC_ReverseInputData_32bits))
/**
 * @}
 */

/** @defgroup CRC_PolynomialSize
 * @{
 */
#define CRC_PolSize_7                   CRC_CR_POLSIZE          /* Polinómio de 7 bits */
#define CRC_PolSize_8                   CRC_CR_POLSIZE_1        /* Polinómio de 8 bits */
#define CRC_PolSize_16                  CRC_CR_POLSIZE_0        /* Polinómio de 16 bits */
#define CRC_PolSize_32                  ((uint32_t)0x00000000U) /* Polinómio padrão de 32 bits (Ethernet) */

#define IS_CRC_POL_SIZE(SIZE)           (((SIZE) == CRC_PolSize_7)  || \
                                         ((SIZE) == CRC_PolSize_8)  || \
                                         ((SIZE) == CRC_PolSize_16) || \
                                         ((SIZE) == CRC_PolSize_32))
/**
 * @}
 */

/* Exported functions ------------------------------------------------------- */

/** @defgroup CRC_Group1 Configuration functions
 * @{
 */
void CRC_DeInit(void);
void CRC_ResetDR(void);
void CRC_PolynomialSizeSelect(uint32_t CRC_PolSize);
void CRC_ReverseInputDataSelect(uint32_t CRC_ReverseInputData);
void CRC_ReverseOutputDataCmd(FunctionalState NewState);
void CRC_SetInitRegister(uint32_t CRC_InitValue);
void CRC_SetPolynomial(uint32_t CRC_Pol);
/**
 * @}
 */

/** @defgroup CRC_Group2 CRC Computation functions
 * @{
 */
uint32_t CRC_CalcCRC(uint32_t CRC_Data);
uint32_t CRC_CalcCRC16bits(uint16_t CRC_Data);
uint32_t CRC_CalcCRC8bits(uint8_t CRC_Data);
uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength);
uint32_t CRC_GetCRC(void);
/**
 * @}
 */

/** @defgroup CRC_Group3 Independent Data Register functions
 * @{
 */
void CRC_SetIDRegister(uint8_t CRC_IDValue);
uint8_t CRC_GetIDRegister(void);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* STM32F30X_CRC_H */

/**
 * @}
 */

/**
 * @}
 */