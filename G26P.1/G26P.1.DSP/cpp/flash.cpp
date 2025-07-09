#include "hardware.h"
#include "spi.h"
#include "BOOT\boot_req.h"
#include "flash.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#define FLASH_REQ_NUM 	16

//#define AT25_SPI_BAUD_RATE 10000000


static u16 SPI0_CS_MASK[] = { PF8 };

static S_SPIM	spi(0, HW::PIOF, SPI0_CS_MASK, ArraySize(SPI0_CS_MASK), SCLK);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define ADSP_CHECKFLASH

#include "FLASH\FlashSPI_imp_V2.h"

FlashSPI bootFlash(spi);

#define PARAMS_START_ADR	(0x30000-FLASH_START_ADR)

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define SMALL_BUF_LEN	(FLASH_PAGE_SIZE+64)
#define MEDIUM_BUF_LEN	(FLASH_PAGE_SIZE+68)
#define HUGE_BUF_LEN	(FLASH_PAGE_SIZE+72)

#define	NUM_SMALL_BUF	4       
#define	NUM_MEDIUM_BUF	4
#define	NUM_HUGE_BUF	4

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "MEM\mem_imp.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

MB* AllocFlashBuffer(u32 minLen)
{
	u32 len = sizeof(FLWB)+sizeof(BootReqV1)+FLASH_PAGE_SIZE;

	if (len < minLen) len = minLen;

	MB *p = AllocMemBuffer(len);

	if (p != 0) p->dataOffset = sizeof(FLWB);

	return p;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct ReqMes
{
	u32 len;

	BootReqV1 mes;

	u32 exdata[PAGEDWORDS];
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct RspMes
{
	u32 len;

	BootRspV1 mes;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static bool RequestBoot_00_GetInfo(ReqMes &req, RspMes &rsp)
{
	rsp.len = 0;

	BootReqV1::SF0 &rq = req.mes.F0;
	BootRspV1::SF0 &rp = rsp.mes.F0;

	if (rq.adr == 0 || req.len < sizeof(rq)) return true;

	rp.adr		= rq.adr;
	rp.rw		= rq.rw;
	rp.ver		= req.mes.VERSION;
	rp.maxFunc	= req.mes.FUNC_MAX;
	rp.guid		= RCV_BOOT_SGUID;
	rp.startAdr = FLASH_START_ADR;
	rp.pageLen	= FLASH_PAGE_SIZE;
	rp.crc		= GetCRC16(&rp, sizeof(rp)-sizeof(rp.crc));

	rsp.len = sizeof(rp);

	return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static bool RequestBoot_01_GetCRC(ReqMes &req, RspMes &rsp)
{
	rsp.len = 0;

	BootReqV1::SF1 &rq = req.mes.F1;
	BootRspV1::SF1 &rp = rsp.mes.F1;

	if (rq.adr == 0) return true;

	bool c = true;

	if (req.len == sizeof(rq) && c)
	{
		if (rq.len != 0)
		{
			rp.flashCRC = bootFlash.CRC16(rq.len, &rp.flashLen); //GetCRC16((void*)(FLASH_START), rq.len);
																 //rp.flashLen = rq.len;
		};
	}
	else
	{
		rp.flashCRC = 0;
		rp.flashLen = 0;
	};

	rp.adr		= rq.adr;
	rp.rw		= rq.rw;
	rp.crc		= GetCRC16(&rp, sizeof(rp)-sizeof(rp.crc));

	rsp.len = sizeof(rp);

	return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static bool RequestBoot_02_WritePage(Ptr<MB> &mb, RspMes &rsp)
{
	FLWB &flwb = *((FLWB*)mb->GetDataPtr());
	ReqMes &req = *((ReqMes*)flwb.data);

	BootReqV1::SF2 &rq = req.mes.F2;
	BootRspV1::SF2 &rp = rsp.mes.F2;

	rsp.len = 0;

	u16 xl = rq.plen + sizeof(rq) - sizeof(rq.pdata);

	bool c = false;

	if (req.len >= xl /*&& flash_write_error == 0*/)
	{
		flwb.adr		= rq.padr;
		flwb.dataLen	= rq.plen;
		flwb.dataOffset = (byte*)rq.pdata - flwb.data;

		c = bootFlash.RequestWrite(mb);
	};

	if (rq.adr == 0) return true;

	rp.adr		= rq.adr;
	rp.rw		= rq.rw;
	rp.res		= c;
	rp.crc		= GetCRC16(&rp, sizeof(rp)-sizeof(rp.crc));
	rsp.len		= sizeof(rp);

	return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static bool RequestBoot_03_ExitBootLoader(ReqMes &req, RspMes &rsp)
{
	BootReqV1::SF3 &rq = req.mes.F3;
	BootRspV1::SF3 &rp = rsp.mes.F3;

	rsp.len = 0;

	if (rq.adr == 0) return false;

	rp.adr		= rq.adr;
	rp.rw		= rq.rw;
	rp.crc		= GetCRC16(&rp, sizeof(rp)-sizeof(rp.crc));
	rsp.len		= sizeof(rp);

	return false;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool RequestBoot(Ptr<MB> &mb, ComPort::WriteBuffer *wb)
{
	static RspMes rsp;

	FLWB &flwb = *((FLWB*)(mb->GetDataPtr()));
	ReqMes &req = *((ReqMes*)flwb.data);
	BootReqV1::SF0 &rq = req.mes.F0;

	u16 t = rq.rw;
	u16 adr = GetNetAdr();

	bool cm = (t & RCV_BOOT_REQ_MASK) == RCV_BOOT_REQ_WORD;
	bool ca = rq.adr == adr || rq.adr == 0;

	if (!ca || !cm || req.len < (sizeof(rq.adr)+sizeof(rq.rw)))
	{
		return false;
	};

	bool c = false;

	t &= 0xFF;

	switch (t)
	{
		//case 0: c = RequestBoot_00_GetInfo(req, rsp);			break;
		//case 1: c = RequestBoot_01_GetCRC(req, rsp);			break;
		//case 2: c = RequestBoot_02_WritePage(mb, rsp);			break;
		//case 3: c = RequestBoot_03_ExitBootLoader(req, rsp);	break;
		//case 4: c = Request_04_SetTimeOut(req, rsp);			break;
	};

	return c;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool FlashLoadParams(void* data, u16 len)
{
	bool res = false;

	u32 adr = PARAMS_START_ADR;

	while (bootFlash.Busy()) bootFlash.Update();

	for (byte i = 0; i < 4; i++)
	{
		bootFlash.Read(adr, data, len);

		if (GetCRC16(data, len) == 0)
		{
			res = true;
			break;
		};

		adr += FLASH_SECTOR_SIZE;
	};

	return res;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static Ptr<MB> mbSave;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool FlashSaveParams(Ptr<MB> mb)
{
	mbSave = mb; 
	return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void FlashUpdate()
{
	static byte i = 0;

	static Ptr<MB> dst;
	static Ptr<MB> src;

	static byte count = 0;
	static u32 adr = 0;

	switch (i)
	{
		case 0:

			if (mbSave.Valid())
			{
				src = mbSave;
				mbSave.Free();
				adr = PARAMS_START_ADR;
				count = 4;
				i++;
			};

			break;

		case 1:

			dst = AllocFlashBuffer(src->len+2);

			if (dst.Valid())
			{
				dst->dataOffset = 0;
				FLWB* flwb = (FLWB*)dst->GetDataPtr(); 
				flwb->adr = adr;
				flwb->dataOffset = 0;
				flwb->dataLen = src->len;
				
				COPY(src->GetDataPtr(), flwb->data, flwb->dataLen);

				u16 crc = GetCRC16(flwb->data, flwb->dataLen);

				byte *p = flwb->data + flwb->dataLen;

				p[0] = crc;
				p[1] = crc>>8;

				flwb->dataLen += 2;

				bootFlash.RequestWrite(dst);

				adr += FLASH_SECTOR_SIZE;

				if (count > 0)
				{
					count -= 1;
				}
				else
				{
					src.Free();
					i = 0;
				};
			};

			break;
	};

	bootFlash.Update();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void FlashInit()
{
	bootFlash.Init();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
