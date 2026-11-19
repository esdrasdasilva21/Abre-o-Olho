/**
 ******************************************************************************
 * @file    usb_istr.c
 * @author  MCD Application Team (Comentado e Otimizado)
 * @version V1.2.0
 * @date    2026
 * @brief   ISR (Interrupt Service Routines) para eventos do periférico USB-FS.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_pwr.h"
#include "usb_istr.h"

/* Private variables ---------------------------------------------------------*/
__IO uint16_t wIstr;         /* Último valor lido do registo ISTR */
__IO uint8_t  bIntPackSOF = 0; /* Contador de pacotes SOF (Start of Frame) recebidos */

/* Tabelas de ponteiros para as rotinas de serviço dos endpoints IN e OUT (1 a 7) */
void (*pEpInt_IN[7])(void) =
  {
    EP1_IN_Callback,
    EP2_IN_Callback,
    EP3_IN_Callback,
    EP4_IN_Callback,
    EP5_IN_Callback,
    EP6_IN_Callback,
    EP7_IN_Callback,
  };

void (*pEpInt_OUT[7])(void) =
  {
    EP1_OUT_Callback,
    EP2_OUT_Callback,
    EP3_OUT_Callback,
    EP4_OUT_Callback,
    EP5_OUT_Callback,
    EP6_OUT_Callback,
    EP7_OUT_Callback,
  };

/**
 * @brief  Rotina principal de tratamento de interrupções de eventos USB.
 *         Lê o registo ISTR e despacha para o handler adequado de acordo com as máscaras.
 */
void USB_Istr(void)
{
  wIstr = _GetISTR();

  /* --- 1. Transferência Correta num Endpoint (CTR) --- */
#if (IMR_MSK & ISTR_CTR)
  if (wIstr & ISTR_CTR & wInterrupt_Mask)
  {
    /* Atende a transferência correta do endpoint (Low Power Mode) */
    CTR_LP();
#ifdef CTR_CALLBACK
    CTR_Callback();
#endif
  }
#endif  

  /* --- 2. Reset do Barramento USB --- */
#if (IMR_MSK & ISTR_RESET)
  if (wIstr & ISTR_RESET & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_RESET);
    Device_Property.Reset(); /* Reinicializa a máquina de estados e configuração USB */
#ifdef RESET_CALLBACK
    RESET_Callback();
#endif
  }
#endif

  /* --- 3. Sobrecarga de Dados (Packet Overrun / DOVR) --- */
#if (IMR_MSK & ISTR_DOVR)
  if (wIstr & ISTR_DOVR & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_DOVR);
#ifdef DOVR_CALLBACK
    DOVR_Callback();
#endif
  }
#endif

  /* --- 4. Erro no Barramento (Bus Error / ERR) --- */
#if (IMR_MSK & ISTR_ERR)
  if (wIstr & ISTR_ERR & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_ERR);
#ifdef ERR_CALLBACK
    ERR_Callback();
#endif
  }
#endif

  /* --- 5. Acordar do Barramento (Resume / WKUP) --- */
#if (IMR_MSK & ISTR_WKUP)
  if (wIstr & ISTR_WKUP & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_WKUP);
    Resume(RESUME_EXTERNAL);
#ifdef WKUP_CALLBACK
    WKUP_Callback();
#endif
  }
#endif

  /* --- 6. Suspensão do Barramento (Suspend / SUSP) --- */
#if (IMR_MSK & ISTR_SUSP)
  if (wIstr & ISTR_SUSP & wInterrupt_Mask)
  {
    if (fSuspendEnabled)
    {
      Suspend(); /* Entra em modo de baixo consumo */
    }
    else
    {
      Resume(RESUME_LATER);
    }
    _SetISTR((uint16_t)CLR_SUSP);
#ifdef SUSP_CALLBACK
    SUSP_Callback();
#endif
  }
#endif

  /* --- 7. Início de Frame (SOF - Start of Frame a cada 1ms) --- */
#if (IMR_MSK & ISTR_SOF)
  if (wIstr & ISTR_SOF & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_SOF);
    bIntPackSOF++;
#ifdef SOF_CALLBACK
    SOF_Callback();
#endif
  }
#endif

  /* --- 8. Início de Frame Esperado / Erro de Sincronismo (ESOF) --- */
#if (IMR_MSK & ISTR_ESOF)
  if (wIstr & ISTR_ESOF & wInterrupt_Mask)
  {
    _SetISTR((uint16_t)CLR_ESOF);
    Resume(RESUME_ESOF);
#ifdef ESOF_CALLBACK
    ESOF_Callback();
#endif
  }
#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/