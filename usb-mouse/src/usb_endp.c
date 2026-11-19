/**
 ******************************************************************************
 * @file    usb_endp.c
 * @author  MCD Application Team (Comentado e Otimizado)
 * @version V1.2.0
 * @date    2026
 * @brief   Rotinas de callback para os Endpoints USB (EP1 IN e EP1 OUT).
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_istr.h"
#include "hw_config.h"
#include "globalVars.h"
#include "peripherals.h"

/**
 * @brief  Callback executado quando a transmissão do Endpoint 1 IN (Dispositivo -> PC)
 *         é concluída com sucesso no barramento USB.
 */
void EP1_IN_Callback(void) {
    /* Pisca o LED4 para indicar tráfego de envio ativo */
    STM_EVAL_LEDToggle(LED4);

    /* Liberta o canal de envio para a próxima amostra de sensores */
    EP1OutState = 1;
}

/**
 * @brief  Callback executado quando o Endpoint 1 OUT (PC -> Dispositivo)
 *         recebe um novo pacote de dados.
 */
void EP1_OUT_Callback(void) {
    /* Pisca o LED5 para indicar tráfego de receção ativo */
    STM_EVAL_LEDToggle(LED5);

    /* Sinaliza ao loop principal (main.c) que há dados prontos para leitura */
    EP1InState = 1;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/