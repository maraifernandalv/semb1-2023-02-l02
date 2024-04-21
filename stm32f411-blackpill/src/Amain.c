/****************************************************************************
 * main.c
 *
 *   Copyright (C) 2021 Daniel P. Carvalho. All rights reserved.
 *   Authors: Daniel P. Carvalho <daniel.carvalho@ufu.br>
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <stdlib.h>

 /****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* AHB1 Base Addresses ******************************************************/

#define STM32_RCC_BASE       0x40023800     /* 0x40023800-0x40023bff: Reset and Clock control RCC */

/* GPIOC Base Addresses ******************************************************/

#define STM32_GPIOC_BASE     0x40020800     /* 0x48000800-0x48000bff: GPIO Port C */

/* Register Offsets *********************************************************/

#define STM32_RCC_AHB1ENR_OFFSET  0x0030   /* AHB1 Peripheral Clock enable register */

#define STM32_GPIO_MODER_OFFSET   0x0000  /* GPIO port mode register */
#define STM32_GPIO_OTYPER_OFFSET  0x0004  /* GPIO port output type register */
#define STM32_GPIO_PUPDR_OFFSET   0x000c  /* GPIO port pull-up/pull-down register */
#define STM32_GPIO_ODR_OFFSET     0x0014  /* GPIO port output data register */
#define STM32_GPIO_BSRR_OFFSET    0x0018  /* GPIO port bit set/reset register */


/* Register Addresses *******************************************************/

#define STM32_RCC_AHB1ENR        (STM32_RCC_BASE+STM32_RCC_AHB1ENR_OFFSET)

#define STM32_GPIOC_MODER        (STM32_GPIOC_BASE+STM32_GPIO_MODER_OFFSET)
#define STM32_GPIOC_OTYPER       (STM32_GPIOC_BASE+STM32_GPIO_OTYPER_OFFSET)
#define STM32_GPIOC_PUPDR        (STM32_GPIOC_BASE+STM32_GPIO_PUPDR_OFFSET)
#define STM32_GPIOC_ODR          (STM32_GPIOC_BASE+STM32_GPIO_ODR_OFFSET)
#define STM32_GPIOC_BSRR         (STM32_GPIOC_BASE+STM32_GPIO_BSRR_OFFSET)

/* AHB1 Peripheral Clock enable register */

#define RCC_AHB1ENR_GPIOCEN      (1 << 2)  /* Bit 2:  IO port C clock enable */

/* GPIO port mode register */

#define GPIO_MODER_INPUT           (0) /* Input */
#define GPIO_MODER_OUTPUT          (1) /* General purpose output mode */
#define GPIO_MODER_ALT             (2) /* Alternate mode */
#define GPIO_MODER_ANALOG          (3) /* Analog mode */

#define GPIO_MODER_SHIFT(n)        (n << 1)
#define GPIO_MODER_MASK(n)         (3 << GPIO_MODER_SHIFT(n))

/* GPIO port output type register */

#define GPIO_OTYPER_PP             (0) /* 0=Output push-pull */
#define GPIO_OTYPER_OD             (1) /* 1=Output open-drain */

#define GPIO_OT_SHIFT(n)           (n)
#define GPIO_OT_MASK(n)            (1 << GPIO_OT_SHIFT(n))

/* GPIO port pull-up/pull-down register */

#define GPIO_PUPDR_NONE            (0) /* No pull-up, pull-down */
#define GPIO_PUPDR_PULLUP          (1) /* Pull-up */
#define GPIO_PUPDR_PULLDOWN        (2) /* Pull-down */

#define GPIO_PUPDR_SHIFT(n)        (n << 1)
#define GPIO_PUPDR_MASK(n)         (3 << GPIO_PUPDR_SHIFT(n))

/* GPIO port bit set/reset register */

#define GPIO_BSRR_SET(n)           (1 << (n))
#define GPIO_BSRR_RESET(n)         (1 << ((n) + 16))

/* Configuration ************************************************************/

#define LED_DELAY  100000

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

 /****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(void)
{
    uint32_t reg;

    /* Ponteiros para registradores */
    volatile uint32_t *pRCC_AHB1ENR = (uint32_t *)STM32_RCC_AHB1ENR;
    volatile uint32_t *pGPIOA_MODER = (uint32_t *)STM32_GPIOA_MODER;
    volatile uint32_t *pGPIOA_IDR = (uint32_t *)STM32_GPIOA_IDR;
    volatile uint32_t *pGPIOC_MODER = (uint32_t *)STM32_GPIOC_MODER;
    volatile uint32_t *pGPIOC_BSRR = (uint32_t *)STM32_GPIOC_BSRR;

    /* Habilita clock para GPIOC e GPIOA */
    reg = *pRCC_AHB1ENR;
    reg |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN;
    *pRCC_AHB1ENR = reg;

    /* Configura PC13 como saída */
    reg = *pGPIOC_MODER;
    reg &= ~(3 << (13 * 2));  // Limpa os bits de modo para PC13
    reg |= (1 << (13 * 2));   // Configura PC13 como saída
    *pGPIOC_MODER = reg;

    /* Configura PA0 como entrada */
    reg = *pGPIOA_MODER;
    reg &= ~(3 << (0 * 2));  // Limpa os bits de modo para PA0
    *pGPIOA_MODER = reg;

    while (1)
    {
        /* Verifica o estado do botão em PA0 */
        if ((*pGPIOA_IDR & (1 << 0)) == 0)  // Se o botão está pressionado (assumindo ativo baixo)
        {
            *pGPIOC_BSRR = (1 << 13);  // Acende o LED (PC13)
        }
        else
        {
            *pGPIOC_BSRR = (1 << (13 + 16));  // Apaga o LED (PC13)
        }
    }

    return 0;  // Nunca deveria chegar aqui
}
