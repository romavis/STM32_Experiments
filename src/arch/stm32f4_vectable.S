/*
 * Exception vector table for STM32F4
 *
 * Copyright (C) 2013 Roman Dobrodiy <ztcoils _d@g_ gmail.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */
	.thumb
	.syntax unified
	
	@Flash Vector table section
	.section .stm_vectable, "a", %progbits

	@Vector table consists of SP, PC initial values and various interrupt
	@vectors, which are initially set to "dumb" exception handler, just
	@to have some meaningful value

	@NOTE: reset vector address must end with 0x1 to ensure setting
	@	of T-bit in EPSR (otherwise Cortex-M faults!)

	.equ	vectable_entries, 256

	.global _init_vectable
_init_vectable:

	@Init
	.word	_mstack_top			@Initial MSP value
	.word	_init_entry			@Reset vector(entry point)
	@NMI
	.word	_vt_vect_NMI			@NMI
	@Faults
	.word	_vt_vect_HardFault		@HardFault
	.word	_vt_vect_MemManage		@MemManage
	.word	_vt_vect_BusFault		@BusFault
	.word	_vt_vect_UsageFault		@UsageFault
	.word	0x0				@-
	.word	0x0				@-
	.word	0x0				@-
	.word	0x0				@-
	@Core
	.word	_vt_vect_SVCall			@SVCall
	.word	_vt_vect_DebugMon		@Debug monitor (reserved
	.word	0x0				@-
	.word	_vt_vect_PendSV			@PendSV
	.word	_vt_vect_SysTick		@SysTick
	@IRQs
	.word	_vt_vect_WWDG			@Window Watchdog
	.word	_vt_vect_PVD			@PVD through EXTI
	.word	_vt_vect_TAMP_STAMP		@Tamper through EXTI
	.word	_vt_vect_RTC_WKUP		@RTC Wakeup through EXTI
	.word	_vt_vect_FLASH			@Flash global int
	.word	_vt_vect_RCC			@RCC global int
	.word	_vt_vect_EXTI0			@EXTI0
	.word	_vt_vect_EXTI1			@EXTI1
	.word	_vt_vect_EXTI2			@EXTI2
	.word	_vt_vect_EXTI3			@EXTI3
	.word	_vt_vect_EXTI4			@EXTI4
	.word	_vt_vect_DMA1_Stream0
	.word	_vt_vect_DMA1_Stream1
	.word	_vt_vect_DMA1_Stream2
	.word	_vt_vect_DMA1_Stream3
	.word	_vt_vect_DMA1_Stream4
	.word	_vt_vect_DMA1_Stream5
	.word	_vt_vect_DMA1_Stream6
	.word	_vt_vect_ADC			@ADC1, ADC2, ADC3
	.word	_vt_vect_CAN1_TX		@CAN1 Tx
	.word	_vt_vect_CAN1_RX0		@CAN1 Rx0
	.word	_vt_vect_CAN1_RX1		@CAN1 Rx1
	.word	_vt_vect_CAN1_SCE		@CAN1 SCE
	.word	_vt_vect_EXTI9_5		@EXTI[9:5]
	.word	_vt_vect_TIM1_BRK_TIM9
	.word	_vt_vect_TIM1_UP_TIM10
	.word	_vt_vect_TIM1_TRG_COM_TIM11
	.word	_vt_vect_TIM1_CC
	.word	_vt_vect_TIM2			@TIM2 global
	.word	_vt_vect_TIM3			@TIM3 global
	.word	_vt_vect_TIM4			@TIM4 global
	.word	_vt_vect_I2C1_EV		@I2C1 event
	.word	_vt_vect_I2C1_ER		@I2C1 error
	.word	_vt_vect_I2C2_EV		@I2C2 event
	.word	_vt_vect_I2C2_ER		@I2C2 error
	.word	_vt_vect_SPI1			@SPI1 global
	.word	_vt_vect_SPI2			@SPI2 global
	.word	_vt_vect_USART1			@USART1 global
	.word	_vt_vect_USART2			@USART2 global
	.word	_vt_vect_USART3			@USART3 global
	.word	_vt_vect_EXTI15_10		@EXTI[15:10]
	.word	_vt_vect_RTC_Alarm		@RTC Alarms through EXTI
	.word	_vt_vect_OTG_FS_WKUP		@OTG FS wakeup through EXTI
	.word	_vt_vect_TIM8_BRK_TIM12
	.word	_vt_vect_TIM8_UP_TIM13
	.word	_vt_vect_TIM8_TRG_COM_TIM14
	.word	_vt_vect_TIM8_CC
	.word	_vt_vect_DMA1_Stream7
	.word	_vt_vect_FSMC			@FSMC global
	.word	_vt_vect_SDIO			@SDIO global
	.word	_vt_vect_TIM5			@TIM5 global
	.word	_vt_vect_SPI3			@SPI3 global
	.word	_vt_vect_UART4			@UART4 global
	.word	_vt_vect_UART5			@UART5 global
	.word	_vt_vect_TIM6_DAC		@TIM6 global, DAC underrun
	.word	_vt_vect_TIM7			@TIM7 global
	.word	_vt_vect_DMA2_Stream0
	.word	_vt_vect_DMA2_Stream1
	.word	_vt_vect_DMA2_Stream2
	.word	_vt_vect_DMA2_Stream3
	.word	_vt_vect_DMA2_Stream4
	.word	_vt_vect_ETH			@Ethernet global
	.word	_vt_vect_ETH_WKUP		@Eth. wakeup through EXTI
	.word	_vt_vect_CAN2_TX		@CAN2 Tx
	.word	_vt_vect_CAN2_RX0		@CAN2 Rx0
	.word	_vt_vect_CAN2_RX1		@CAN2 Rx1
	.word	_vt_vect_CAN2_SCE		@CAN2 SCE
	.word	_vt_vect_OTG_FS			@OTG FS global
	.word	_vt_vect_DMA2_Stream5
	.word	_vt_vect_DMA2_Stream6
	.word	_vt_vect_DMA2_Stream7
	.word	_vt_vect_USART6			@USART6 global
	.word	_vt_vect_I2C3_EV		@I2C3 Event
	.word	_vt_vect_I2C3_ER		@I2C3 Error
	.word	_vt_vect_OTG_HS_EP1_OUT		@OTG-HS Endpoint 1 out
	.word	_vt_vect_OTG_HS_EP1_IN		@OTG-HS Endpoint 1 in
	.word	_vt_vect_OTG_HS_WKUP		@OTG-HS Wakeup through EXTI
	.word	_vt_vect_OTG_HS			@OTG-HS global
	.word	_vt_vect_DCMI			@DCMI global
	.word	_vt_vect_CRYP			@Crypto global
	.word	_vt_vect_HASH_RNG		@Hash/RNG global
	.word	_vt_vect_FPU			@FPU global

#ifdef	CONFIG_STM32F42_F43
	@STM32F42*, STM32F43* only
	.word	_vt_vect_UART7			@UART7 global
	.word	_vt_vect_UART8			@UART8 global
	.word	_vt_vect_SPI4			@SPI4 global
	.word	_vt_vect_SPI5			@SPI5 global
	.word	_vt_vect_SPI6			@SPI6 global
	.word	_vt_vect_SAI1			@SAI1 global
	.word	_vt_vect_LCD_TFT		@LTDC global
	.word	_vt_vect_LCD_TFT_ER		@LTDC global error
	.word	_vt_vect_DMA2D			@DMA2D global
#endif	/* CONFIG_STM32F42_F43 */
	@Fill with default handlers up to 256 entries
	.rept	(vectable_entries - (. - _init_vectable) / 4)
	.word	_irq_default_handler
	.endr

/*****************************************************************************/

	.weak	_vt_vect_NMI
	.weak	_vt_vect_HardFault
	.weak	_vt_vect_MemManage
	.weak	_vt_vect_BusFault
	.weak	_vt_vect_UsageFault
	.weak	_vt_vect_SVCall
	.weak	_vt_vect_DebugMon
	.weak	_vt_vect_PendSV
	.weak	_vt_vect_SysTick
	.weak	_vt_vect_WWDG
	.weak	_vt_vect_PVD
	.weak	_vt_vect_TAMP_STAMP
	.weak	_vt_vect_RTC_WKUP
	.weak	_vt_vect_FLASH
	.weak	_vt_vect_RCC
	.weak	_vt_vect_EXTI0
	.weak	_vt_vect_EXTI1
	.weak	_vt_vect_EXTI2
	.weak	_vt_vect_EXTI3
	.weak	_vt_vect_EXTI4
	.weak	_vt_vect_DMA1_Stream0
	.weak	_vt_vect_DMA1_Stream1
	.weak	_vt_vect_DMA1_Stream2
	.weak	_vt_vect_DMA1_Stream3
	.weak	_vt_vect_DMA1_Stream4
	.weak	_vt_vect_DMA1_Stream5
	.weak	_vt_vect_DMA1_Stream6
	.weak	_vt_vect_ADC
	.weak	_vt_vect_CAN1_TX
	.weak	_vt_vect_CAN1_RX0
	.weak	_vt_vect_CAN1_RX1
	.weak	_vt_vect_CAN1_SCE
	.weak	_vt_vect_EXTI9_5
	.weak	_vt_vect_TIM1_BRK_TIM9
	.weak	_vt_vect_TIM1_UP_TIM10
	.weak	_vt_vect_TIM1_TRG_COM_TIM11
	.weak	_vt_vect_TIM1_CC
	.weak	_vt_vect_TIM2
	.weak	_vt_vect_TIM3
	.weak	_vt_vect_TIM4
	.weak	_vt_vect_I2C1_EV
	.weak	_vt_vect_I2C1_ER
	.weak	_vt_vect_I2C2_EV
	.weak	_vt_vect_I2C2_ER
	.weak	_vt_vect_SPI1
	.weak	_vt_vect_SPI2
	.weak	_vt_vect_USART1
	.weak	_vt_vect_USART2
	.weak	_vt_vect_USART3
	.weak	_vt_vect_EXTI15_10
	.weak	_vt_vect_RTC_Alarm
	.weak	_vt_vect_OTG_FS_WKUP	
	.weak	_vt_vect_TIM8_BRK_TIM12
	.weak	_vt_vect_TIM8_UP_TIM13
	.weak	_vt_vect_TIM8_TRG_COM_TIM14
	.weak	_vt_vect_TIM8_CC
	.weak	_vt_vect_DMA1_Stream7
	.weak	_vt_vect_FSMC
	.weak	_vt_vect_SDIO
	.weak	_vt_vect_TIM5
	.weak	_vt_vect_SPI3
	.weak	_vt_vect_UART4
	.weak	_vt_vect_UART5
	.weak	_vt_vect_TIM6_DAC
	.weak	_vt_vect_TIM7
	.weak	_vt_vect_DMA2_Stream0
	.weak	_vt_vect_DMA2_Stream1
	.weak	_vt_vect_DMA2_Stream2
	.weak	_vt_vect_DMA2_Stream3
	.weak	_vt_vect_DMA2_Stream4
	.weak	_vt_vect_ETH
	.weak	_vt_vect_ETH_WKUP
	.weak	_vt_vect_CAN2_TX
	.weak	_vt_vect_CAN2_RX0
	.weak	_vt_vect_CAN2_RX1
	.weak	_vt_vect_CAN2_SCE
	.weak	_vt_vect_OTG_FS
	.weak	_vt_vect_DMA2_Stream5
	.weak	_vt_vect_DMA2_Stream6
	.weak	_vt_vect_DMA2_Stream7
	.weak	_vt_vect_USART6
	.weak	_vt_vect_I2C3_EV
	.weak	_vt_vect_I2C3_ER
	.weak	_vt_vect_OTG_HS_EP1_OUT
	.weak	_vt_vect_OTG_HS_EP1_IN
	.weak	_vt_vect_OTG_HS_WKUP
	.weak	_vt_vect_OTG_HS
	.weak	_vt_vect_DCMI
	.weak	_vt_vect_CRYP
	.weak	_vt_vect_HASH_RNG
	.weak	_vt_vect_FPU
#ifdef	CONFIG_STM32F42_F43
	.weak	_vt_vect_UART7
	.weak	_vt_vect_UART8
	.weak	_vt_vect_SPI4
	.weak	_vt_vect_SPI5
	.weak	_vt_vect_SPI6
	.weak	_vt_vect_SAI1
	.weak	_vt_vect_LCD_TFT
	.weak	_vt_vect_LCD_TFT_ER
	.weak	_vt_vect_DMA2D
#endif	/* CONFIG_STM32F42_F43 */

/*****************************************************************************/
	@NMI
	.thumb_set	_vt_vect_NMI,		_nmi_default_handler
	@Faults
	.thumb_set	_vt_vect_HardFault,	_hfault_default_handler
	.thumb_set	_vt_vect_MemManage,	_mmfault_default_handler
	.thumb_set	_vt_vect_BusFault,	_bfault_default_handler
	.thumb_set	_vt_vect_UsageFault,	_ufault_default_handler
	@Core
	.thumb_set	_vt_vect_SVCall,	_irq_default_handler
	.thumb_set	_vt_vect_DebugMon,	_nmi_default_handler
	.thumb_set	_vt_vect_PendSV,	_irq_default_handler
	.thumb_set	_vt_vect_SysTick,	_irq_default_handler
	@IRQs
	.thumb_set	_vt_vect_WWDG,		_irq_default_handler
	.thumb_set	_vt_vect_PVD,		_irq_default_handler
	.thumb_set	_vt_vect_TAMP_STAMP,	_irq_default_handler
	.thumb_set	_vt_vect_RTC_WKUP,	_irq_default_handler
	.thumb_set	_vt_vect_FLASH,		_irq_default_handler
	.thumb_set	_vt_vect_RCC,		_irq_default_handler
	.thumb_set	_vt_vect_EXTI0,		_irq_default_handler
	.thumb_set	_vt_vect_EXTI1,		_irq_default_handler
	.thumb_set	_vt_vect_EXTI2,		_irq_default_handler
	.thumb_set	_vt_vect_EXTI3,		_irq_default_handler
	.thumb_set	_vt_vect_EXTI4,		_irq_default_handler
	.thumb_set	_vt_vect_DMA1_Stream0,	_irq_default_handler
	.thumb_set	_vt_vect_DMA1_Stream1,	_irq_default_handler
	.thumb_set	_vt_vect_DMA1_Stream2,	_irq_default_handler
	.thumb_set	_vt_vect_DMA1_Stream3,	_irq_default_handler
	.thumb_set	_vt_vect_DMA1_Stream4,	_irq_default_handler
	.thumb_set	_vt_vect_DMA1_Stream5,	_irq_default_handler
	.thumb_set	_vt_vect_DMA1_Stream6,	_irq_default_handler
	.thumb_set	_vt_vect_ADC,		_irq_default_handler
	.thumb_set	_vt_vect_CAN1_TX,	_irq_default_handler
	.thumb_set	_vt_vect_CAN1_RX0,	_irq_default_handler
	.thumb_set	_vt_vect_CAN1_RX1,	_irq_default_handler
	.thumb_set	_vt_vect_CAN1_SCE,	_irq_default_handler
	.thumb_set	_vt_vect_EXTI9_5,	_irq_default_handler
	.thumb_set	_vt_vect_TIM1_BRK_TIM9,	_irq_default_handler
	.thumb_set	_vt_vect_TIM1_UP_TIM10,	_irq_default_handler
	.thumb_set	_vt_vect_TIM1_TRG_COM_TIM11,	_irq_default_handler
	.thumb_set	_vt_vect_TIM1_CC,	_irq_default_handler
	.thumb_set	_vt_vect_TIM2,		_irq_default_handler
	.thumb_set	_vt_vect_TIM3,		_irq_default_handler
	.thumb_set	_vt_vect_TIM4,		_irq_default_handler
	.thumb_set	_vt_vect_I2C1_EV,	_irq_default_handler
	.thumb_set	_vt_vect_I2C1_ER,	_irq_default_handler
	.thumb_set	_vt_vect_I2C2_EV,	_irq_default_handler
	.thumb_set	_vt_vect_I2C2_ER,	_irq_default_handler
	.thumb_set	_vt_vect_SPI1,		_irq_default_handler
	.thumb_set	_vt_vect_SPI2,		_irq_default_handler
	.thumb_set	_vt_vect_USART1,	_irq_default_handler
	.thumb_set	_vt_vect_USART2,	_irq_default_handler
	.thumb_set	_vt_vect_USART3,	_irq_default_handler
	.thumb_set	_vt_vect_EXTI15_10,	_irq_default_handler
	.thumb_set	_vt_vect_RTC_Alarm,	_irq_default_handler
	.thumb_set	_vt_vect_OTG_FS_WKUP,	_irq_default_handler
	.thumb_set	_vt_vect_TIM8_BRK_TIM12,_irq_default_handler
	.thumb_set	_vt_vect_TIM8_UP_TIM13,	_irq_default_handler
	.thumb_set	_vt_vect_TIM8_TRG_COM_TIM14,	_irq_default_handler
	.thumb_set	_vt_vect_TIM8_CC,	_irq_default_handler
	.thumb_set	_vt_vect_DMA1_Stream7,	_irq_default_handler
	.thumb_set	_vt_vect_FSMC,		_irq_default_handler
	.thumb_set	_vt_vect_SDIO,		_irq_default_handler
	.thumb_set	_vt_vect_TIM5,		_irq_default_handler
	.thumb_set	_vt_vect_SPI3,		_irq_default_handler
	.thumb_set	_vt_vect_UART4,		_irq_default_handler
	.thumb_set	_vt_vect_UART5,		_irq_default_handler
	.thumb_set	_vt_vect_TIM6_DAC,	_irq_default_handler
	.thumb_set	_vt_vect_TIM7,		_irq_default_handler
	.thumb_set	_vt_vect_DMA2_Stream0,	_irq_default_handler
	.thumb_set	_vt_vect_DMA2_Stream1,	_irq_default_handler
	.thumb_set	_vt_vect_DMA2_Stream2,	_irq_default_handler
	.thumb_set	_vt_vect_DMA2_Stream3,	_irq_default_handler
	.thumb_set	_vt_vect_DMA2_Stream4,	_irq_default_handler
	.thumb_set	_vt_vect_ETH,		_irq_default_handler
	.thumb_set	_vt_vect_ETH_WKUP,	_irq_default_handler
	.thumb_set	_vt_vect_CAN2_TX,	_irq_default_handler
	.thumb_set	_vt_vect_CAN2_RX0,	_irq_default_handler
	.thumb_set	_vt_vect_CAN2_RX1,	_irq_default_handler
	.thumb_set	_vt_vect_CAN2_SCE,	_irq_default_handler
	.thumb_set	_vt_vect_OTG_FS,	_irq_default_handler
	.thumb_set	_vt_vect_DMA2_Stream5,	_irq_default_handler
	.thumb_set	_vt_vect_DMA2_Stream6,	_irq_default_handler
	.thumb_set	_vt_vect_DMA2_Stream7,	_irq_default_handler
	.thumb_set	_vt_vect_USART6,	_irq_default_handler
	.thumb_set	_vt_vect_I2C3_EV,	_irq_default_handler
	.thumb_set	_vt_vect_I2C3_ER,	_irq_default_handler
	.thumb_set	_vt_vect_OTG_HS_EP1_OUT,_irq_default_handler
	.thumb_set	_vt_vect_OTG_HS_EP1_IN,	_irq_default_handler
	.thumb_set	_vt_vect_OTG_HS_WKUP,	_irq_default_handler
	.thumb_set	_vt_vect_OTG_HS,	_irq_default_handler
	.thumb_set	_vt_vect_DCMI,		_irq_default_handler
	.thumb_set	_vt_vect_CRYP,		_irq_default_handler
	.thumb_set	_vt_vect_HASH_RNG,	_irq_default_handler
	.thumb_set	_vt_vect_FPU,		_irq_default_handler
#ifdef	CONFIG_STM32F42_F43
	.thumb_set	_vt_vect_UART7,		_irq_default_handler
	.thumb_set	_vt_vect_UART8,		_irq_default_handler
	.thumb_set	_vt_vect_SPI4,		_irq_default_handler
	.thumb_set	_vt_vect_SPI5,		_irq_default_handler
	.thumb_set	_vt_vect_SPI6,		_irq_default_handler
	.thumb_set	_vt_vect_SAI1,		_irq_default_handler
	.thumb_set	_vt_vect_LCD_TFT,	_irq_default_handler
	.thumb_set	_vt_vect_LCD_TFT_ER,	_irq_default_handler
	.thumb_set	_vt_vect_DMA2D,		_irq_default_handler
#endif	/* CONFIG_STM32F42_F43 */
	
