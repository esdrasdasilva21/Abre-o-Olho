/**
 ******************************************************************************
 * @file    peripherals.h
 * @author  Kailang@DotLab (Melhorado e Comentado)
 * @version V0.2.0
 * @date    2026
 * @brief   Cabeçalho de controlo dos periféricos (IMU, LEDs, Botão, Atrasos e Flash).
 * @note    Licensed under GNU GPL V3.0 License.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdint.h>
#include "main.h"
#include "globalVars.h"
#include "stm32f3_discovery_lsm303dlhc.h"
#include "stm32f3_discovery_l3gd20.h"

/* Exported constants (Sensibilidades e Físico-matemáticas) -------------------*/

/* Sensibilidade do Giroscópio L3GD20 [LSB/dps] */
#define L3G_Sensitivity_250dps      ((float)114.285f)
#define L3G_Sensitivity_500dps      ((float)57.1429f)
#define L3G_Sensitivity_2000dps     ((float)14.285f)

/* Sensibilidade do Acelerómetro LSM303DLHC [LSB/mg] */
#define LSM_Acc_Sensitivity_2g      ((float)1.0f)
#define LSM_Acc_Sensitivity_4g      ((float)2.0f)
#define LSM_Acc_Sensitivity_8g      ((float)4.0f)
#define LSM_Acc_Sensitivity_16g     ((float)12.0f)

#define PI                          ((float)3.14159265f)
#define GRAVITY_ACCEL               ((float)9.80665f)   /* Aceleração da gravidade standard [m/s^2] */

/* Exported macros (Com proteção estrita de parênteses) ----------------------*/
#define ABS(x)                      (((x) < 0) ? -(x) : (x))
#define MAX(x, y)                   (((x) < (y)) ? (y) : (x))

/* Exported functions ------------------------------------------------------- */

/** @defgroup System_Delays_Group Gestão de Tempo e Delays
 * @{
 */
void Delay(uint32_t time);
/** @} */

/** @defgroup IMU_Sensors_Group Inicialização e Leitura de Sensores IMU
 * @{
 */
void Gyr_Init(void);
void Acc_Init(void);
void Mag_Init(void);
void Gyr_Read(float *pfData);
void Acc_Read(float *pfData);
void Mag_Read(float *pfData);
void Gyr_Cali(uint8_t sample);
void Gyr_Corr(float *pfData);
/** @} */

/** @defgroup LED_Utility_Group Controlo Visual por LEDs
 * @{
 */
void LED_Config(void);
void LED_Off(void);
void LED_On(void);
void LED_Ign(Led_TypeDef LED);
void LED_Dim(Led_TypeDef LED);
void LED_Out(uint8_t byte);
void LED_Read(uint64_t data, uint8_t count);
/** @} */

/** @defgroup User_Button_Group Botão de Utilizador
 * @{
 */
uint8_t But_Pres(void);
/** @} */

/** @defgroup Flash_Storage_Group Leitura e Escrita em Memória Flash
 * @{
 */
void Fla_Read32(uint8_t id, uint32_t *pdata, uint8_t length);
uint8_t Fla_Writ32(uint8_t id, uint32_t *pdata, uint8_t length);
/** @} assaseaseasasasas*/

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERALS_H_ */


https://github.com/nirmalnishant645/Augmented-Reality-Unity-Projects