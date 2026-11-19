/**
 ******************************************************************************
 * @file    system_stm32f30x.c
 * @author  MCD Application Team (Atualizado)
 * @version V1.2.0
 * @date    2026
 * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File.
 *          Configures STM32F30x system clock at 72 MHz (48 MHz USB clock).
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include <stdbool.h>

/* Private defines -----------------------------------------------------------*/
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET         0x0U
#define PLL_STARTUP_TIMEOUT     ((uint32_t)0x00005000)

/* Selecione a fonte de relógio:
 * - PLL_SOURCE_HSE_BYPASS: Placa STM32F3 Discovery (clock vindo do ST-Link)
 * - PLL_SOURCE_HSE:        Cristal externo de 8 MHz dedicado na placa
 * - PLL_SOURCE_HSI:        Oscilador interno (Nota: USB FS exige HSE para estabilidade)
 */
#define PLL_SOURCE_HSE_BYPASS

/* Global variables ----------------------------------------------------------*/
uint32_t SystemCoreClock = 72000000U;
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

/* Indicador de estado do relógio (útil para diagnóstico via USB/LED) */
static bool clockFallbackActive = false;

/* Private function prototypes -----------------------------------------------*/
static void SetSysClock(void);
static bool ConfigurePLL_HSI_Fallback(void);

/**
 * @brief  Indica se o sistema teve de recorrer ao oscilador interno (HSI)
 *         por falha no cristal externo (HSE).
 */
bool System_IsClockFallbackActive(void) {
    return clockFallbackActive;
}

/**
 * @brief  Setup the microcontroller system (FPU, RCC reset, Clock, Vector Table).
 */
void SystemInit(void) {
    /* FPU settings (Cortex-M4F) */
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* Full access CP10 & CP11 */
#endif

    /* Reset RCC clock configuration to default state */
    RCC->CR   |= (uint32_t)0x00000001;  /* Set HSION bit */
    RCC->CFGR &= 0xF87FC00C;            /* Reset CFGR register */
    RCC->CR   &= (uint32_t)0xFEF6FFFF;  /* Reset HSEON, CSSON and PLLON */
    RCC->CR   &= (uint32_t)0xFFFBFFFF;  /* Reset HSEBYP */
    RCC->CFGR &= (uint32_t)0xFF80FFFF;  /* Reset PLLSRC, PLLXTPRE, PLLMUL, USBPRE */
    RCC->CFGR2 &= (uint32_t)0xFFFFFFF0; /* Reset PREDIV1[3:0] */
    RCC->CFGR3 &= (uint32_t)0xFF00FCCC; /* Reset USARTSW, I2CSW, TIMs */
    RCC->CIR   = 0x00000000;            /* Disable all interrupts */

    /* Configure System Clock (72 MHz) */
    SetSysClock();

    /* Update SystemCoreClock variable to reflect actual configured frequency */
    SystemCoreClockUpdate();

    /* Vector Table Relocation */
#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET;
#else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
#endif
}

/**
 * @brief  Update SystemCoreClock variable according to Clock Register Values.
 */
void SystemCoreClockUpdate(void) {
    uint32_t tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp) {
    case 0x00: /* HSI used as system clock */
        SystemCoreClock = HSI_VALUE;
        break;

    case 0x04: /* HSE used as system clock */
        SystemCoreClock = HSE_VALUE;
        break;

    case 0x08: { /* PLL used as system clock */
        uint32_t pllmull   = ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18) + 2;
        uint32_t pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

        if (pllsource == 0x00) {
            /* HSI / 2 selected as PLL clock entry */
            SystemCoreClock = (HSI_VALUE >> 1) * pllmull;
        } else {
            uint32_t prediv1factor = (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;
            SystemCoreClock = (HSE_VALUE / prediv1factor) * pllmull;
        }
        break;
    }

    default:
        SystemCoreClock = HSI_VALUE;
        break;
    }

    /* Compute HCLK clock frequency */
    tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
    SystemCoreClock >>= tmp;
}

/**
 * @brief  Configura PLL usando HSI (64 MHz) como plano de contingência
 *         caso o HSE falhe no arranque.
 */
static bool ConfigurePLL_HSI_Fallback(void) {
    __IO uint32_t timeout = 0;

    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1);

    /* PLLCLK = (HSI / 2) * 16 = 64 MHz (máximo permitido via HSI) */
    RCC->CFGR &= (uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLXTPRE_PREDIV1 | RCC_CFGR_PLLMULL16);

    RCC->CR |= RCC_CR_PLLON;
    while (((RCC->CR & RCC_CR_PLLRDY) == 0) && (timeout < PLL_STARTUP_TIMEOUT)) {
        timeout++;
    }
    if ((RCC->CR & RCC_CR_PLLRDY) == 0) {
        return false;
    }

    RCC->CFGR &= (uint32_t)~(RCC_CFGR_SW);
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    timeout = 0;
    while (((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) && (timeout < PLL_STARTUP_TIMEOUT)) {
        timeout++;
    }

    return ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL);
}

/**
 * @brief  Configures System clock source, PLL, prescalers and Flash latency.
 */
static void SetSysClock(void) {
    __IO uint32_t StartUpCounter = 0;
    __IO uint32_t HSEStatus      = 0;
    __IO uint32_t timeout        = 0;

#if defined (PLL_SOURCE_HSI)
    clockFallbackActive = !ConfigurePLL_HSI_Fallback();
#else
    #if defined (PLL_SOURCE_HSE)
        RCC->CR |= ((uint32_t)RCC_CR_HSEON);
    #elif defined (PLL_SOURCE_HSE_BYPASS)
        RCC->CR |= (uint32_t)(RCC_CR_HSEON | RCC_CR_HSEBYP);
    #endif

    /* Wait till HSE is ready or timeout */
    do {
        HSEStatus = RCC->CR & RCC_CR_HSERDY;
        StartUpCounter++;
    } while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

    if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
        /* Enable Prefetch Buffer and set Flash Latency (2 wait states for 72 MHz) */
        FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;

        /* HCLK = SYSCLK (72 MHz), PCLK2 = 72 MHz, PCLK1 = 36 MHz (max allowed) */
        RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
        RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

        /* PLL configuration: PLLCLK = HSE (8 MHz) * 9 = 72 MHz */
        RCC->CFGR &= (uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
        RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_PREDIV1 | RCC_CFGR_PLLMULL9);

        /* Enable PLL with timeout */
        RCC->CR |= RCC_CR_PLLON;
        timeout = 0;
        while (((RCC->CR & RCC_CR_PLLRDY) == 0) && (timeout < PLL_STARTUP_TIMEOUT)) {
            timeout++;
        }

        if ((RCC->CR & RCC_CR_PLLRDY) != 0) {
            /* Select PLL as system clock source */
            RCC->CFGR &= (uint32_t)~(RCC_CFGR_SW);
            RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

            timeout = 0;
            while (((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) && (timeout < PLL_STARTUP_TIMEOUT)) {
                timeout++;
            }
            clockFallbackActive = false;
        } else {
            /* PLL falhou com HSE -> ativa fallback HSI */
            clockFallbackActive = ConfigurePLL_HSI_Fallback();
        }
    } else {
        /* HSE falhou -> ativa fallback HSI (64 MHz) para manter sensores/CPU operacionais */
        clockFallbackActive = ConfigurePLL_HSI_Fallback();
    }
#endif
}