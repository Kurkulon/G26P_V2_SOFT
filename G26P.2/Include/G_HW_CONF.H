#ifndef G_HW_CONF_H__09_04_2025__17_28
#define G_HW_CONF_H__09_04_2025__17_28

#pragma once

#include <types.h>
#include <core.h>

#define CLKIN_MHz	25
#define MCK_MHz		100
#define MCK			(MCK_MHz*1000000)
#define NS2CLK(x)	(((x)*MCK_MHz+500)/1000)
#define US2CLK(x)	((x)*MCK_MHz)
#define MS2CLK(x)	((x)*MCK_MHz*1000)

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//#define FRAM_SPI_MAINVARS_ADR 0
//#define FRAM_SPI_SESSIONS_ADR 0x200

#define FRAM_I2C_MAINVARS_ADR 0
#define FRAM_I2C_SESSIONS_ADR 0x200

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef CPU_SAM4SA

	// Test Pins
	// 11 - PA21
	// 14 - PA22
	// 19 - PA16
	// 20 - PA15
	// 23 - PA24
	// 25 - PA25
	// 26 - PA26
	// 31 - PA8		- main loop
	// 32 - PA7
	// 38 - PA28
	// 41 - PA29
	// 42 - PA30
	// 56 - PB10	- ManRcvIRQ sync true
	// 57 - PB11	-
	// 61 - PB8		- ManRcvIRQ
	// 63 - PB14	- ManTrmIRQ
	
	// 	- SPI_Handler
	// 
	// 
	// 
	// 
	// 
	// 	- SPI DSP
	// 	
	// 	



	// ++++++++++++++	SERCOM	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define SPI_SERCOM_NUM		0
//	#define I2C0_SERCOM_NUM		1
	#define I2C_SERCOM_NUM		2
	#define UART3_RCV_NUM		3
//	#define UART4_SERCOM_NUM	4
	#define UART5_MEM_NUM		5
	#define UART6_TRM_NUM		6

	// ++++++++++++++	DMA	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//#define	NAND_DMA			DMA_CH0
	////#define	DSP_SPI_DMA_RX		DMA_CH1
	//#define	UART7_DMA			DMA_CH2
	//#define	UART4_DMA			DMA_CH3
	//#define	SPI_DMA_TX			DMA_CH4
	//#define	SPI_DMA_RX			DMA_CH5
	//#define	NAND_MEMCOPY_DMA	DMA_CH6
	//#define	I2C_DMA				DMA_CH7
	////#define	DSP_SPI_DMA_RX		DMA_CH8

	////#define	DSP_SPI_DMA_TX		DMA_CH30
	//#define	CRC_DMA				DMA_CH31

	// ++++++++++++++	EVENT	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//#define EVENT_NAND_1	0
	////#define EVENT_NAND_2	1
	////#define EVENT_NAND_3	2
	//#define EVENT_MANR_1	3
	//#define EVENT_MANR_2	4

	// ++++++++++++++	TC	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define MANT_TC			0
	//#define CRCDMA_TC		TC1
	//#define 				TC2
	//#define 				TC3
	//#define 				TC4
	//#define 				TC5
	//#define 				TC6
	//#define 				TC7

	// ++++++++++++++	TCC	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//#define MANT_TCC		TCC0
	////#define				TCC1
	//#define MANR_TCC		TCC2
	//#define SYNC_TCC		TCC3
	//#define NAND_TCC		TCC4
	////#define MltTmr		TCC4


	// ++++++++++++++	I2C	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define PIO_I2C				HW::PIOA 
	#define PIN_SDA				22 
	#define PIN_SCL				23 
	#define SDA					(1<<PIN_SDA) 
	#define SCL					(1<<PIN_SCL) 
	#define I2C_PMUX_SDA		PORT_PMUX_C 
	#define I2C_PMUX_SCL		PORT_PMUX_C 
	#define I2C_GEN_SRC			GEN_MCK
	#define I2C_GEN_CLK			GEN_MCK_CLK
	#define I2C_BAUDRATE		400000 

	// ++++++++++++++	SPI	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define PIO_CS				HW::PIOA

	#define PIN_CS0				23 
	//#define PIN_CS1				11

	#define CS0					(1<<PIN_CS0) 
	//#define CS1					(1<<PIN_CS1) 

	#define SPI_BAUDRATE		8000000

	//#define Pin_SPI_IRQ_Set() HW::PIOB->BSET(15)		
	//#define Pin_SPI_IRQ_Clr() HW::PIOB->BCLR(15)		

	// ++++++++++++++	MANCH	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	#define MAN_TRANSMIT_V1

	#define PIO_MANCH		HW::PIOB
	#define PIN_L1			1 
	#define PIN_L2			3 
	#define PIN_H1			0 
	#define PIN_H2			2

	#define L1				(1UL<<PIN_L1)
	#define L2				(1UL<<PIN_L2)
	#define H1				(1UL<<PIN_H1)
	#define H2				(1UL<<PIN_H2)

	#define PIO_RXD			PIOB
	#define PIN_RXD			0
	#define RXD				(1UL<<PIN_RXD)

	#define Pin_ManRcvIRQ_Set()		HW::PIOB->BSET(8)
	#define Pin_ManRcvIRQ_Clr()		HW::PIOB->BCLR(8)

	#define Pin_ManTrmIRQ_Set()		HW::PIOB->BSET(14)		
	#define Pin_ManTrmIRQ_Clr()		HW::PIOB->BCLR(14)		

	#define Pin_ManRcvSync_Set()	HW::PIOB->BSET(10)		
	#define Pin_ManRcvSync_Clr()	HW::PIOB->BCLR(10)		


	// ++++++++++++++	UART TRM	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define PIO_UTXD6			HW::PIOB 
	#define PIO_URXD6			HW::PIOB 
	#define PIO_RTS6			HW::PIOB 
	#define PMUX_UTXD6			0
	#define PMUX_URXD6			0 
	#define PIN_UTXD6			3 
	#define PIN_URXD6			2 
	#define PIN_RTS6			13
	#define UTXD6				(1<<PIN_UTXD6) 
	#define URXD6				(1<<PIN_URXD6) 
	#define RTS6				(1<<PIN_RTS6) 
	#define TRMUART				HW::UART1
	#define PIO_TRMRTS			PIO_RTS6 
	#define TRMRTS				RTS6
	#define RTS1				RTS6 

	// ++++++++++++++	UART RCV	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define PIO_UTXD3			HW::PIOA
	#define PIO_URXD3			HW::PIOA 
	#define PIO_RTS3			HW::PIOA 
	#define PMUX_UTXD3			0 
	#define PMUX_URXD3			0 
	#define PIN_UTXD3			6 
	#define PIN_URXD3			5
	#define PIN_RTS3			4
	#define UTXD3				(1<<PIN_UTXD3) 
	#define URXD3				(1<<PIN_URXD3) 
	#define RTS3				(1<<PIN_RTS3) 
	#define RCVUART				HW::USART0
	#define PIO_RCVRTS			PIO_RTS3 
	#define RCVRTS				RTS3
	#define RTS2				RTS3 

	// ++++++++++++++	UART MEM	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define PIO_UTXD5			HW::PIOA
	#define PIO_URXD5			HW::PIOA 
	#define PIO_RTS5			HW::PIOA 
	#define PMUX_UTXD5			0 
	#define PMUX_URXD5			0 
	#define PIN_UTXD5			10 
	#define PIN_URXD5			9
	#define PIN_RTS5			31
	#define UTXD5				(1<<PIN_UTXD5) 
	#define URXD5				(1<<PIN_URXD5) 
	#define RTS5				(1<<PIN_RTS5) 
	#define RTS0				RTS5 

	// ++++++++++++++	ADC		++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define PIO_ARID			HW::PIOB
	#define PIO_AVMAN 			HW::PIOB

	#define PIN_ARID			8
	#define PIN_AVMAN			9 
	
	#define ARID				(1UL<<PIN_ARID	) 
	#define AVMAN				(1UL<<PIN_AVMAN	) 

	// ++++++++++++++	PIO INIT	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define PIOA_INIT_DIR		(L1|L2|H1|H2|RTS0|RTS2|PA07|PA08|PA15|PA16|PA21|PA22|PA24|PA25|PA26|PA28|PA29|PA30)
	#define PIOA_INIT_SET		(H1|H2)
	#define PIOA_INIT_CLR		(L1|L2|RTS0|RTS2|PA07|PA08|PA15|PA16|PA21|PA22|PA24|PA25|PA26|PA28|PA29|PA30)
	#define PIOA_TEST_MASK		(0)

	#define PIOB_INIT_DIR		(RTS1|PB8|PB10|PB11|PB14)
	#define PIOB_INIT_SET		(0)
	#define PIOB_INIT_CLR		(RTS1|PB8|PB10|PB11|PB14)
	#define PIOB_TEST_MASK		(0)

	#define PIOC_INIT_DIR		(0)
	#define PIOC_INIT_SET		(0)
	#define PIOC_INIT_CLR		(0)
	#define PIOC_TEST_MASK		(0)

	//#define TEST_PIN_DELAY		(MCK_MHz*10)

	#define Pin_MainLoop_Set()	HW::PIOA->BSET(8)
	#define Pin_MainLoop_Clr()	HW::PIOA->BCLR(8)
	#define Pin_MainLoop_Tgl()	HW::PIOA->BTGL(8)
	

#elif defined(WIN32) //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	#define BAUD2CLK(x)				(x)
	#define MT(v)					(v)
	#define Pin_MainLoop_Set()	
	#define Pin_MainLoop_Clr()	

#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




#endif // G_HW_CONF_H__09_04_2025__17_28
