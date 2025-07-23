#ifndef G_HW_CONF_H__23_07_2025__14_25
#define G_HW_CONF_H__23_07_2025__14_25

#include <types.h>
#include <core.h>

#define CLKIN_MHz	25
#define PLL_MSEL	2		// 1...32
#define PLL_PSEL	1		// 0...3
#define PLL_MHz		(CLKIN_MHz*PLL_MSEL)		
#define FCCO_MHz	(PLL_MHz*(2UL<<PLL_PSEL))		// 156...320

#if defined(FCCO_MHz) && ((FCCO_MHz < 156) || (FCCO_MHz > 320))
#error  FCCO_MHz must be 156...320
#endif

#define MCK_DIV			1
#define UARTCLK_DIV		1

#ifdef PLL_MHz
#define MCK_MHz ((float)PLL_MHz/MCK_DIV)
#else
#define MCK_MHz ((float)CLKIN_MHz/MCK_DIV)
#endif

#define MCK			((u32)(MCK_MHz*1000000UL))
#define NS2CLK(x) 	((u32)(((x)*MCK_MHz+500)/1000))
#define US2CLK(x) 	((u32)((x)*MCK_MHz))
#define MS2CLK(x) 	((u32)((x)*MCK_MHz*1000))

// ++++++++++++++	USIC	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define USART0_USIC_NUM		0
//#define USART1_USIC_NUM	1
//#define USART2_USIC_NUM	2
//#define SPI0_USIC_NUM		3
//#define SPI1_USIC_NUM		4
//#define I2C0_USIC_NUM		5
//#define I2C1_USIC_NUM		6
//#define I2C2_USIC_NUM		7
//#define I2C3_USIC_NUM		8

// ++++++++++++++	USART	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define PIN_UTX0			6 
#define PIN_URX0			14 
#define PIN_RTS0			0

#define UTX0				(1UL<<PIN_UTX0)
#define URX0				(1UL<<PIN_URX0)
#define RTS0				(1UL<<PIN_RTS0)

// ++++++++++++++	PWM		++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define PIN_FX1				17 
#define PIN_FX2				13 
#define PIN_FY1				12
#define PIN_FY2				4
#define PIN_EN				9
#define PIN_CHARGE			8
#define PIN_TR1				7

#define FX1					(1UL<<PIN_FX1	)
#define FX2					(1UL<<PIN_FX2	)
#define FY1					(1UL<<PIN_FY1	)
#define FY2					(1UL<<PIN_FY2	)
#define ENABLE				(1UL<<PIN_EN	)
#define CHARGE				(1UL<<PIN_CHARGE)
#define TR1					(1UL<<PIN_TR1	)


// ++++++++++++++	PIO INIT	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
																
#define GPIO_INIT_DIR0		FX1|FX2|FY1|FY2|RTS0|ENABLE|CHARGE|TR1|(1<<26)
#define GPIO_INIT_PIN0		ENABLE								

#define Pin_MainLoop_Set()	HW::GPIO->BSET(26)
#define Pin_MainLoop_Clr()	HW::GPIO->BCLR(26)
#define Pin_MainLoop_Tgl()	HW::GPIO->BTGL(26)


#endif // G_HW_CONF_H__23_07_2025__14_25
