#ifndef G_RCV_H__27_12_2023__11_22
#define G_RCV_H__27_12_2023__11_22

#pragma once 

#include "types.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef _ADI_COMPILER
	#pragma pack(1)
	//#ifndef __packed
	//	#define __packed /**/
	//#endif
#else

	//#define RCV_8AD

#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define RCV_AUTO_GAIN_LO_AMP	8192
#define RCV_AUTO_GAIN_HI_AMP	24576

#define RCV_MAN_VEC_TIMOUT		10000  //ms

#define RCV_MAX_NUM_STATIONS	13
#define RCV_COM_BAUDRATE		6250000
#define RCV_COM_PARITY			0
#define RCV_COM_STOPBITS		2

#define RCV_BOOT_SGUID			0X99FBD4844CCB442C
#define RCV_BOOT_REQ_WORD		((~(RCV_MAN_REQ_WORD)) & RCV_MAN_REQ_MASK)
#define RCV_BOOT_REQ_MASK		RCV_MAN_REQ_MASK
#define RCV_BOOT_COM_BAUDRATE	RCV_COM_BAUDRATE
#define RCV_BOOT_COM_PARITY		RCV_COM_PARITY
#define RCV_BOOT_COM_STOPBITS	RCV_COM_STOPBITS

#define RCV_MAN_REQ_WORD 		0xAB00
#define RCV_MAN_REQ_MASK 		0xFF00

#define RCV_FltResist(v)	(((v) * 941 + 2048) / 4096)
#define RCV_NetResist(v)	(((v) * 941 + 128) / 256)
#define RCV_NetAdr(v)		(1 + (v)/1024)

//#define RCV_TEST_WAVEPACK 16

#define RCV_WAVEPACK

#define RCV_SAMPLE_LEN 1024

//#define RCV_RSP02_CRC16_CCIT	
//#define RCV_RSP02_CRC_TYPE		REQ::CRC16_CCIT
#define RCV_RSP02_CRC16
#define RCV_RSP02_CRC_TYPE		REQ::CRC16

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define TRANSMITER_NUM	3	// 0 - Monopole1, 1 - Monopole1, 2 - DipoleXY
#define RCV_FIRE_NUM	4	// 0 - Mnpl_1, 1 - Mnpl_2, 2 - DplX, 3 - DplY

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef BOOTLOADER

__packed struct ReqRcv01	// старт оцифровки
{
	__packed struct Req
	{
		byte 	len;
		byte 	adr;
		byte 	func;
		byte 	n; 
		byte 	next_n; 
		byte	next_gain;
		u16		fc;		// fire count
		u16		sl;
		u16		st;
		u16		sd;
		u16		packType;
		u16		math;

		word 	crc;  
	}
	r[3];
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct RspRcv01	// старт оцифровки
{
	byte adr;
	byte func;
	word crc;  
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct ReqRcv02	// чтение вектора
{
	__packed struct Req
	{
		byte 	len;
		byte 	adr;
		byte 	func;
		byte 	n; 
		word 	crc; 
	}
	r[2];
};  

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct RspRcvHdr02	// чтение вектора
{
	u16 rw;
	u32 cnt;
	u16 preAmp;
	u16 gain;
	u16 st;
	u16 sl;
	u16 sd;
	u16 packType;
	u16 math;
	u16 packLen1;
	u16 packLen2;
	u16 packLen3;
	u16 packLen4;
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct RspRcv02	// чтение вектора
{
	RspRcvHdr02 hdr;

	u16 data[(RCV_SAMPLE_LEN+64)*4]; 
	u16 crc;
};  

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct  ReqRcv03	// установка периода дискретизации вектора и коэффициента усиления
{ 
	__packed struct Req
	{
		byte 	len;
		byte 	adr;
		byte 	func;
		byte	numDevValid;		// если не ноль, numDev пральный и его нада записать в RAM
		u16		numDev;				// номер модуля приёмников
		u16 	gain[RCV_FIRE_NUM]; 
		word	crc; 
	}
	r[2];
};  

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct  RspRcv03	// установка периода дискретизации вектора и коэффициента усиления
{ 
	byte	adr;
	byte	func;
	u16		temp;			// температура
	u16		numdev;			// номер модуля приёмников
	u16		verdev; 		// версия ПО модуля приёмников
	byte	numDevValid;	// если не ноль, numDev считан из flash правильно или установлен запросом
	byte	flashStatus; 	// бит 0 - запись в процессе, бит 1 - запись ОК, бит 2 - ошибка записи
	word	crc; 
};  

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct  ReqRcv04	// установка коэффициента усиления
{ 
	__packed struct Req
	{
		byte 	len;
		byte 	adr;
		byte 	func;
		byte 	saveParams; // если не ноль, то записать параметры во flash
		word 	crc; 
	}
	r[2];
};  

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct  RspRcv04	// установка периода дискретизации вектора и коэффициента усиления
{ 
	byte	adr;
	byte	func;
	byte	numDevValid;	// если не ноль, numDev считан из flash правильно или установлен запросом
	byte	flashStatus; 	// бит 0 - запись в процессе, бит 1 - запись ОК, бит 2 - ошибка записи
	u16		temp;			// температура
	u16		numdev;			// номер модуля приёмников
	u16		verdev; 		// версия ПО модуля приёмников
	u16 	maxAmp[4];
	u16		power[4];
	word	crc; 
};  

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct ReqRcv05	// чтение упаковнного вектора или с математикой для передачи по кабелю
{
	__packed struct Req
	{
		byte 	len;
		byte 	adr;
		byte 	func;
		byte 	n; 
		word 	crc; 
	}
	r[2];
};  

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed union ReqRcv
{
	ReqRcv01 req01;
	ReqRcv02 req02;
	ReqRcv03 req03;
	ReqRcv04 req04;
	ReqRcv05 req05;
};

#endif // #ifndef BOOTLOADER

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

__packed struct Transmiter
{
	u16 gain;
	u16 st;		//sampleTime;
	u16 sl;		//sampleLen;
	u16 sd;		//sampleDelay;
	u16 freq;
	u16 duty;
	u16 amp;
	u16 pulseCount;
	u16 packType;
	u16 math;

	u16 GetGain() { return gain&7; }
	void SetGain(u16 v) { gain = (gain & ~7)|(v&7); }
	u16 GetPreAmp() { return gain>>7; }
	void SetPreAmp(u16 v) { gain = (gain & ~(1<<7))|(v<<7); }
};
	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct AutoGain
{
	u16 gain;
	u16 maxAmp;

	u16 GetGain()			{ return gain&7; }
	void SetGain(u16 v)		{ gain = (gain & ~7)|(v&7); }
	u16 GetPreAmp()			{ return gain>>7; }
	void SetPreAmp(u16 v)	{ gain = (gain & ~(1<<7))|(v<<7); }

	void Inc()
	{ 
		u16 g = GetGain();

		if (GetPreAmp())
		{
			if (g != 7) SetGain(g+1);
		}
		else
		{
			if (g == 3) gain = 1<<7; else if (g != 7) SetGain(g+1);
		};
	}

	void Dec()
	{
		u16 g = GetGain();

		if (GetPreAmp())
		{
			if (g == 0) gain = 3; else SetGain(g-1);
		}
		else
		{
			if (g != 0) SetGain(g-1);
		};
	}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef _ADI_COMPILER
#pragma pack()
//#undef __packed
#endif



#endif //G_RCV_H__27_12_2023__11_22
