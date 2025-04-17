#include "core.h"
#include "time.h"
#include "spi.h"
#include "SEGGER_RTT\SEGGER_RTT.h"
#include "hw_conf.h"

#ifdef CPU_SAME53
static u32 SPI_CS_MASK[] = { CS0, CS1 };
#elif defined(CPU_XMC48)
static byte SPI_CS_PIN[] = { PIN_CS0, PIN_CS1 };
#endif

#ifdef WIN32

#include <windows.h>
#include <Share.h>
#include <conio.h>
#include <stdarg.h>
#include <stdio.h>
#include <intrin.h>
//#include "CRC16_CCIT.h"
#include "list.h"

//static HANDLE handleNandFile;
//static const char nameNandFile[] = "NAND_FLASH_STORE.BIN";
//
//static HANDLE handleWriteThread;
//static HANDLE handleReadThread;
//
//static byte nandChipSelected = 0;
//
//static u64 curNandFilePos = 0;
////static u64 curNandFileBlockPos = 0;
//static u32 curBlock = 0;
//static u32 curRawBlock = 0;
//static u16 curPage = 0;
//static u16 curCol = 0;
//
//static OVERLAPPED	_overlapped;
//static u32			_ovlReadedBytes = 0;
//static u32			_ovlWritenBytes = 0;
//
//static void* nandEraseFillArray;
//static u32 nandEraseFillArraySize = 0;
//static byte nandReadStatus = 0x41;
//static u32 lastError = 0;
//
//
//static byte fram_I2c_Mem[0x10000];
static byte fram_SPI_Mem[0x40000];
//
static bool fram_spi_WREN = false;
//
//static u16 crc_ccit_result = 0;

static S_SPIM	spi;

#elif defined(CPU_SAME53)

static S_SPIM	spi(SPI_SERCOM_NUM, PIO_SPCK, PIO_MOSI, PIO_MISO, PIO_CS, SPCK, MOSI, MISO, SPI_PMUX_SPCK, SPI_PMUX_MOSI, SPI_PMUX_MISO, SPI_CS_MASK, ArraySize(SPI_CS_MASK), 
					SPI_DIPO_BITS, SPI_DOPO_BITS, SPI_GEN_SRC, SPI_GEN_CLK, &SPI_DMA_TX, &SPI_DMA_RX);

#elif defined(CPU_SAM4SA)

static u32 SPI_CS_MASK[] = { CS0, CS1 };

static S_SPIM	spi(SPI_SERCOM_NUM, PIO_CS, SPI_CS_MASK, ArraySize(SPI_CS_MASK), MCK);

#elif defined(CPU_XMC48)

static S_SPIM	spi(SPI_USIC_NUM, PIO_SPCK, PIO_MOSI, PIO_MISO, PIO_CS, PIN_SPCK, PIN_MOSI, PIN_MISO, MUX_SPCK, MUX_MOSI, SPI_CS_PIN, ArraySize(SPI_CS_PIN), &SPI_DMA, SPI_DRL, SPI_DX0CR, SPI_DX1CR, SYSCLK);

#endif 


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool SPI_AddRequest(DSCSPI *d)
{
	if (d == 0) { return false; };
	//if ((d->wdata == 0 || d->wlen == 0) && (d->rdata == 0 || d->rlen == 0)) { return false; }

	return spi.AddRequest(d);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool SPI_Update()
{
	return spi.Update();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SPI_Init()
{
	spi.Connect(SPI_BAUDRATE);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "SPIM_IMP.H"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
