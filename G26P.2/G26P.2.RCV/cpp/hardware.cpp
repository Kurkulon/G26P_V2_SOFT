#include "types.h"
#include "core.h"
#include "time.h"
#include "CRC\CRC16_8005.h"

#include "hardware.h"

#include "SEGGER_RTT\SEGGER_RTT.h"
#include "hw_conf.h"
#include "hw_rtm.h"
#include "MANCH\manch.h"
#include "DMA\DMA.h"
#include "G_RCV.h"

#define CRCDMA_PER		(NS2CLK(0)-1)

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define SMALL_BUF_LEN	512
#define MEDIUM_BUF_LEN	1536

#ifndef WIN32
#define HUGE_BUF_LEN	(sizeof(RspRcv02)+32) //2034
#else
#define HUGE_BUF_LEN	0x4100    
#endif
#define	NUM_SMALL_BUF	32       
#define	NUM_MEDIUM_BUF	16
#define	NUM_HUGE_BUF	8

#include <MEM\mem_imp.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef WIN32

//#include <windows.h>
//#include <Share.h>
#include <conio.h>
//#include <stdarg.h>
#include <stdio.h>
#include <intrin.h>
//#include "CRC16_CCIT.h"
//#include "list.h"


static u16 crc_ccit_result = 0;

#else

//#pragma O3
//#pragma Otime

#endif 

#define GEAR_RATIO 12.25

//const u16 pulsesPerHeadRoundFix4 = GEAR_RATIO * 6 * 16;

//const u16 testNandChipMask = 0xFFFF;

static volatile u32 shaftCounter = 0;
static volatile u32 shaftPrevTime = 0;
static volatile u32 shaftCount = 0;
static volatile u32 shaftTime = 0;
u16 shaftRPS = 0;
volatile u16 curShaftCounter = 0;


//static void I2C_Init();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//static void InitVectorTable();

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#include <ARM\system_imp.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//#include <Clock_imp.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <CRC\CRC_CCITT_DMA_IMP.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void WDT_Init()
{
	SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_YELLOW "WDT Init ... ");

	#ifdef CPU_SAME53	

		//HW::MCLK->APBAMASK |= APBA_WDT;

		//HW::WDT->CONFIG = WDT_WINDOW_CYC512|WDT_PER_CYC1024;
	
		#ifndef _DEBUG
		HW::WDT->CTRLA = WDT_ENABLE|WDT_WEN|WDT_ALWAYSON;
		#else
		HW::WDT->CTRLA = 0;
		#endif

		//while(HW::WDT->SYNCBUSY);

	#elif defined(CPU_XMC48)

		#ifndef _DEBUG
	
		//HW::WDT_Enable();

		//HW::WDT->WLB = OFI_FREQUENCY/2;
		//HW::WDT->WUB = (3 * OFI_FREQUENCY)/2;
		//HW::SCU_CLK->WDTCLKCR = 0|SCU_CLK_WDTCLKCR_WDTSEL_OFI;

		//HW::WDT->CTR = WDT_CTR_ENB_Msk|WDT_CTR_DSP_Msk;

		#else

		HW::WDT_Disable();

		#endif

	#endif

	SEGGER_RTT_WriteString(0, "OK\n");
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef WIN32

//extern dword DI;
//extern dword DO;
char pressedKey;
//extern  dword maskLED[16];

//byte _array1024[0x60000]; 

HWND  hMainWnd;

HCURSOR arrowCursor;
HCURSOR handCursor;

HBRUSH	redBrush;
HBRUSH	yelBrush;
HBRUSH	grnBrush;
HBRUSH	gryBrush;

RECT rectLed[10] = { {20, 41, 33, 54}, {20, 66, 33, 79}, {20, 91, 33, 104}, {21, 117, 22, 118}, {20, 141, 33, 154}, {218, 145, 219, 146}, {217, 116, 230, 129}, {217, 91, 230, 104}, {217, 66, 230, 79}, {217, 41, 230, 54}  }; 
HBRUSH* brushLed[10] = { &yelBrush, &yelBrush, &yelBrush, &gryBrush, &grnBrush, &gryBrush, &redBrush, &redBrush, &redBrush, &redBrush };

//int x,y,Depth;

HFONT font1;
HFONT font2;

HDC memdc;
HBITMAP membm;

//HANDLE facebitmap;

static const u32 secBufferWidth = 80;
static const u32 secBufferHeight = 12;
static const u32 fontWidth = 12;
static const u32 fontHeight = 16;


static char secBuffer[secBufferWidth*secBufferHeight*2];

static u32 pallete[16] = {	0x000000,	0x800000,	0x008000,	0x808000,	0x000080,	0x800080,	0x008080,	0xC0C0C0,
							0x808080,	0xFF0000,	0x00FF00,	0xFFFF00,	0x0000FF,	0xFF00FF,	0x00FFFF,	0xFFFFFF };

const char lpAPPNAME[] = "¿—76÷_”œ–";

int screenWidth = 0, screenHeight = 0;

LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static __int64		tickCounter = 0;

#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef WIN32

static void InitDisplay()
{
	WNDCLASS		    wcl;

	wcl.hInstance		= NULL;
	wcl.lpszClassName	= lpAPPNAME;
	wcl.lpfnWndProc		= WindowProc;
	wcl.style	    	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	wcl.hIcon	    	= NULL;
	wcl.hCursor	    	= NULL;
	wcl.lpszMenuName	= NULL;

	wcl.cbClsExtra		= 0;
	wcl.cbWndExtra		= 0;
	wcl.hbrBackground	= NULL;

	RegisterClass (&wcl);

	int sx = screenWidth = GetSystemMetrics (SM_CXSCREEN);
	int sy = screenHeight = GetSystemMetrics (SM_CYSCREEN);

	hMainWnd = CreateWindowEx (0, lpAPPNAME, lpAPPNAME,	WS_DLGFRAME|WS_POPUP, 0, 0,	640, 480, NULL,	NULL, NULL, NULL);

	if(!hMainWnd) 
	{
		cputs("Error creating window\r\n");
		exit(0);
	};

	RECT rect;

	if (GetClientRect(hMainWnd, &rect))
	{
		MoveWindow(hMainWnd, 0, 0, 641 + 768 - rect.right - 2, 481 - rect.bottom - 1 + 287, true);
	};

	ShowWindow (hMainWnd, SW_SHOWNORMAL);

	font1 = CreateFont(30, 14, 0, 0, 100, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH|FF_DONTCARE, "Lucida Console");
	font2 = CreateFont(fontHeight, fontWidth-2, 0, 0, 100, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH|FF_DONTCARE, "Lucida Console");

	if (font1 == 0 || font2 == 0)
	{
		cputs("Error creating font\r\n");
		exit(0);
	};

	GetClientRect(hMainWnd, &rect);
	HDC hdc = GetDC(hMainWnd);
    memdc = CreateCompatibleDC(hdc);
	membm = CreateCompatibleBitmap(hdc, rect.right - rect.left + 1, rect.bottom - rect.top + 1 + secBufferHeight*fontHeight);
    SelectObject(memdc, membm);
	ReleaseDC(hMainWnd, hdc);


	arrowCursor = LoadCursor(0, IDC_ARROW);
	handCursor = LoadCursor(0, IDC_HAND);

	if (arrowCursor == 0 || handCursor == 0)
	{
		cputs("Error loading cursors\r\n");
		exit(0);
	};

	LOGBRUSH lb;

	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(0xFF, 0, 0);

	redBrush = CreateBrushIndirect(&lb);

	lb.lbColor = RGB(0xFF, 0xFF, 0);

	yelBrush = CreateBrushIndirect(&lb);

	lb.lbColor = RGB(0x7F, 0x7F, 0x7F);

	gryBrush = CreateBrushIndirect(&lb);

	lb.lbColor = RGB(0, 0xFF, 0);

	grnBrush = CreateBrushIndirect(&lb);

	for (u32 i = 0; i < sizeof(secBuffer); i+=2)
	{
		secBuffer[i] = 0x20;
		secBuffer[i+1] = 0xF0;
	};
}

#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef WIN32

void UpdateDisplay()
{
	static byte curChar = 0;
	//static byte c = 0, i = 0;
	//static byte flashMask = 0;
	static const byte a[4] = { 0x80, 0x80+0x40, 0x80+20, 0x80+0x40+20 };

	MSG msg;

	static dword pt = 0;
	static TM32 tm;

	u32 t = GetTickCount();

	if ((t-pt) >= 2)
	{
		pt = t;

		while(PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			GetMessage (&msg, NULL, 0, 0);

			TranslateMessage (&msg);

			DispatchMessage (&msg);
		};
	};

	static char buf[80];
	static char buf1[sizeof(secBuffer)];

	if (tm.Check(20))
	{
		bool rd = true;

		if (rd)
		{
			//SelectObject(memdc, font1);
			//SetBkColor(memdc, 0x074C00);
			//SetTextColor(memdc, 0x00FF00);
			//TextOut(memdc, 443, 53, buf, 20);
			//TextOut(memdc, 443, 30*1+53, buf+20, 20);
			//TextOut(memdc, 443, 30*2+53, buf+40, 20);
			//TextOut(memdc, 443, 30*3+53, buf+60, 20);

			SelectObject(memdc, font2);

			for (u32 j = 0; j < secBufferHeight; j++)
			{
				for (u32 i = 0; i < secBufferWidth; i++)
				{
					u32 n = (j*secBufferWidth+i)*2;

					u8 t = secBuffer[n+1];

					SetBkColor(memdc, pallete[(t>>4)&0xF]);
					SetTextColor(memdc, pallete[t&0xF]);
					TextOut(memdc, i*fontWidth, j*fontHeight+287, secBuffer+n, 1);
				};
			};

			RedrawWindow(hMainWnd, 0, 0, RDW_INVALIDATE);

		}; // if (rd)

	}; // if ((t-pt) > 10)

}

#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef WIN32

LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	HDC hdc;
	PAINTSTRUCT ps;
	bool c;
	static int x, y;
	int x0, y0, r0;
	static char key = 0;
	static char pkey = 0;

	RECT rect;

	static bool move = false;
	static int movex, movey = 0;

//	char *buf = (char*)screenBuffer;

    switch (message)
	{
        case WM_CREATE:

            break;

	    case WM_DESTROY:

		    break;

        case WM_PAINT:

			if (GetUpdateRect(hWnd, &rect, false)) 
			{
				hdc = BeginPaint(hWnd, &ps);

				//printf("Update RECT: %li, %li, %li, %li\r\n", ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);

				c = BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left + 1, ps.rcPaint.bottom - ps.rcPaint.top + 1, memdc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
	 
				EndPaint(hWnd, &ps);
			};

            break;

        case WM_CHAR:

			pressedKey = wParam;

			if (pressedKey == '`')
			{
				GetWindowRect(hWnd, &rect);

				if ((rect.bottom-rect.top) > 340)
				{
					MoveWindow(hWnd, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top-fontHeight*secBufferHeight, true); 
				}
				else
				{
					MoveWindow(hWnd, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top+fontHeight*secBufferHeight, true); 
				};
			};

            break;

		case WM_MOUSEMOVE:

			x = LOWORD(lParam); y = HIWORD(lParam);

			if (move)
			{
				GetWindowRect(hWnd, &rect);
				SetWindowPos(hWnd, HWND_TOP, rect.left+x-movex, rect.top+y-movey, 0, 0, SWP_NOSIZE); 
			};

			return 0;

			break;

		case WM_MOVING:

			return TRUE;

			break;

		case WM_SYSCOMMAND:

			return 0;

			break;

		case WM_LBUTTONDOWN:

			move = true;
			movex = x; movey = y;

			SetCapture(hWnd);

			return 0;

			break;

		case WM_LBUTTONUP:

			move = false;

			ReleaseCapture();

			return 0;

			break;

		case WM_MBUTTONDOWN:

			ShowWindow(hWnd, SW_MINIMIZE);

			return 0;

			break;

		case WM_ACTIVATE:

			if (HIWORD(wParam) != 0 && LOWORD(wParam) != 0)
			{
				ShowWindow(hWnd, SW_NORMAL);
			};

			break;

		case WM_CLOSE:

			//run = false;

			break;
	};
    
	return DefWindowProc(hWnd, message, wParam, lParam);
}

#endif		
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef WIN32

int PutString(u32 x, u32 y, byte c, const char *str)
{
	char *dst = secBuffer+(y*secBufferWidth+x)*2;
	dword i = secBufferWidth-x;

	while (*str != 0 && i > 0)
	{
		*(dst++) = *(str++);
		*(dst++) = c;
		i -= 1;
	};

	return secBufferWidth-x-i;
}

#endif
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef	WIN32

int Printf(u32 xx, u32 yy, byte c, const char *format, ... )
{
	char buf[1024];

	va_list arglist;

    va_start(arglist, format);
    vsprintf(buf, format, arglist);
    va_end(arglist);

	return PutString(xx, yy, c, buf);
}

#endif
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//i32	Get_FRAM_SPI_SessionsAdr() { return FRAM_SPI_SESSIONS_ADR; }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

i32	Get_FRAM_I2C_SessionsAdr() { return FRAM_I2C_SESSIONS_ADR; }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void Init_ADC()
{
	using namespace HW;

	PMC->PCER0 = PID::ADC_M;

	ADC->MR		= ADC_TRANSFER(2)|ADC_FREERUN|ADC_PRESCAL(~0)|ADC_STARTUP_8;	//0x0001FF80;
	ADC->ACR	= ADC_TSON;
	ADC->CHER	= ADC_CH3|ADC_CH15;
	ADC->CR		= ADC_START|ADC_AUTOCAL;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

u16 Get_NetResist()
{
	return HW::ADC->CDR[3]; //return (HW::ADC->CDR[3] * 10919) >> 16;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

u16 Get_AVMAN()
{
	return (HW::ADC->CDR[15] * 1617) >> 16;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef MAN_TRANSMIT_SSC

#pragma push
//#pragma O3
//#pragma Otime

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define BAUD2CLK(x) ((u32)(MCK/2/(x)+0.5))

static const u16 manbaud[5] = { BAUD2CLK(20833), BAUD2CLK(41666), BAUD2CLK(62500), BAUD2CLK(83333), BAUD2CLK(104166) };//0:20833Hz, 1:41666Hz,2:62500Hz,3:83333Hz

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static u16 GetTrmBaudRate(byte i)
{
	if (i >= ArraySize(manbaud)) { i = ArraySize(manbaud) - 1; };

	return (manbaud[i]+1)/2;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define _MAN_SSC_TCMR	(SSC_TCKS_MCK|SSC_TCKO_TRANSFER|SSC_TCKI)
#define _MAN_SSC_TFMR	(SSC_TDATLEN(20)|SSC_TMSBF|SSC_TFSOS_HIGH)

#define ManDisableTransmit()		{ HW::SSC->CR = SSC_TXDIS; HW::SSC->IDR = ~0; }
#define ManTransmit_ClockEnable()	{ HW::PMC->ClockEnable(HW::PID::SSC_I); }

inline void ManDisable()	{ } 
inline void ManZero()		{ } 
inline void ManOne()		{ } 
inline void ManDischarge()	{ } 

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static byte stateManTrans = 0;
static MTB *manTB = 0;
static bool trmBusy = false;
static bool trmTurbo = false;
static bool rcvBusy = false;
static const u16 *mantPtr = 0;
static u16 mantLen = 0;
static u32 manBuf[8];
static byte manBufStart = 0;
static byte manBufEnd = 0;
static byte manBufLen = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inline void ManBufAdd(u32 v)
{
	manBuf[manBufEnd] = v;
	manBufEnd = (manBufEnd+1) & (ArraySize(manBuf)-1);
	manBufLen += 1;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inline u32 ManBufGet()
{
	u32	v = manBuf[manBufStart];

	manBufStart = (manBufStart+1) & (ArraySize(manBuf)-1);
	manBufLen -= 1;

	return v;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inline bool ManBufEmpty()		{ return manBufLen == 0; }
inline bool ManBufFull()		{ return manBufLen == ArraySize(manBuf); }
inline byte ManBufFreeLen()		{ return ArraySize(manBuf)-manBufLen; }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

union ManBits
{
	struct
	{
		u64 pr : 2;
		u64 b1 : 8;
		u64 b2 : 8;
		u64 b3 : 8;
		u64 b4 : 8;
		u64 st : 6;
	};

	struct
	{
		u64 dw1 : 20;
		u64 dw2 : 20;
	};
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static u16 word2man[16] = {
	0x00AA, // 1010	1010    	
	0x80A9, // 1010	1001
	0x80A6, // 1010	0110
	0x00A5, // 1010	0101
	0x809A, // 1001	1010
	0x0099, // 1001	1001
	0x0096, // 1001	0110
	0x8095, // 1001	0101
	0x806A, // 0110	1010
	0x0069, // 0110	1001
	0x8066, // 0110	0110
	0x8065, // 0110	0101
	0x005A, // 0101	1010
	0x8859, // 0101	1001
	0x8856, // 0101	0110
	0x0055, // 0101	0101
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inline void GetManBits(ManBits &man, u16 t)
{
	u16 r1 = word2man[t&15];
	man.b1 = r1;

	u16 parity = r1;

	r1 = word2man[(t>>4)&15];
	man.b2 = r1;
	parity ^= r1;

	r1 = word2man[(t>>8)&15];
	man.b3 = r1;
	parity ^= r1;

	r1 = word2man[(t>>12)&15];
	man.b4 = r1;
	parity ^= r1;

	man.pr = 2 - (parity>>15);
	man.st = 0x38;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static __irq void ManTrm_ISR()
{
	HW::PIOA->BSET(21);

	u32 stat = HW::SSC->SR & HW::SSC->IMR;

	if (stat & SSC_TXRDY)
	{
		if (mantLen > 0)
		{
			if (ManBufFreeLen() >= 2)
			{
				ManBits man;

				GetManBits(man, *(mantPtr++));

				ManBufAdd(man.dw2); //;man.dw2;
				ManBufAdd(man.dw1); //man.dw1;

				mantLen -= 1;

				if (mantLen == 0 && manTB->data2 != 0 && manTB->len2 != 0)
				{
					mantPtr = manTB->data2;
					mantLen = manTB->len2;
					manTB->len2 = 0;
				};
			};
		};

		if (!ManBufEmpty())
		{
			HW::SSC->THR = ManBufGet(); //man.dw1;
		}
		else
		{
			//HW::SSC->THR = 0;
			HW::SSC->IDR = SSC_TXRDY;
			HW::SSC->IER = SSC_TXEMPTY;
		};
	}
	else if (stat & SSC_TXEMPTY)
	{
		ManDisableTransmit();

		manTB->ready = true;
		trmBusy = false;

		HW::PIOA->BCLR(22);
	};

	HW::PIOA->BCLR(21);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool SendManDataSSC(MTB *mtb)
{
	if (trmBusy || rcvBusy || mtb == 0 || mtb->data1 == 0 || mtb->len1 == 0)
	{
		return false;
	};

	mtb->ready = false;

	manTB = mtb;

	mantPtr = mtb->data1;
	mantLen = mtb->len1;

	stateManTrans = 0;

	ManBits man;

	GetManBits(man, *(mantPtr++)); mantLen--;

	//ManBufAdd(man.dw2); 
	//ManBufAdd(man.dw1); 

	HW::SSC->CMR	= SSC_DIV(GetTrmBaudRate(mtb->baud)); 
	HW::SSC->TCMR	= _MAN_SSC_TCMR;
	HW::SSC->TFMR	= _MAN_SSC_TFMR;
	HW::SSC->IDR	= ~SSC_TXRDY;
	HW::SSC->CR		= SSC_TXEN;

	HW::SSC->THR = man.dw2;

	while ((HW::SSC->SR & SSC_TXRDY) == 0);

	HW::SSC->THR = man.dw1;

	HW::SSC->IER = SSC_TXRDY;

	HW::PIOA->BSET(22);

	return trmBusy = true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static void InitManTransmitSSC()
{
	HW::PIOA->PDR = PA15|PA16|PA17;
	HW::PIOA->ABCDSR1 &= ~(PA15|PA16|PA17);
	HW::PIOA->ABCDSR2 &= ~(PA15|PA16|PA17);

	HW::PIOA->PUDR = PA15|PA16|PA17;
	HW::PIOA->PPDER = PA15|PA16|PA17;

	ManTransmit_ClockEnable();

	HW::SSC->CR	= SSC_SWRST;

	VectorTableExt[SSC_IRQ] = ManTrm_ISR;
	CM4::NVIC->CLR_PR(SSC_IRQ);
	CM4::NVIC->SET_ER(SSC_IRQ);
}

#pragma pop

#endif // #if defined(CPU_BF706) && defined(MAN_TRANSMIT)

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void InitHardware()
{
	SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_YELLOW "Hardware Init ... ");

#ifdef CPU_SAME53	
	
	using namespace HW;

//	HW::PIOA->BSET(13);

	HW::GCLK->GENCTRL[GEN_32K]		= GCLK_DIV(1)	|GCLK_SRC_OSCULP32K	|GCLK_GENEN;

	HW::GCLK->GENCTRL[GEN_1M]		= GCLK_DIV(25)	|GCLK_SRC_XOSC1		|GCLK_GENEN		|GCLK_OE;

	HW::GCLK->GENCTRL[GEN_25M]		= GCLK_DIV(1)	|GCLK_SRC_XOSC1		|GCLK_GENEN;

//	HW::GCLK->GENCTRL[GEN_500K] 	= GCLK_DIV(50)	|GCLK_SRC_XOSC1		|GCLK_GENEN;

	//PIO_32kHz->SetWRCONFIG(1UL<<PIN_32kHz, PORT_PMUX_M|PORT_WRPINCFG|PORT_PMUXEN|PORT_WRPMUX|PORT_PULLEN);

	//HW::GCLK->GENCTRL[GEN_EXT32K]	= GCLK_DIV(1)	|GCLK_SRC_GCLKIN	|GCLK_GENEN		;


	HW::MCLK->APBAMASK |= APBA_EIC;
	HW::GCLK->PCHCTRL[GCLK_EIC] = GCLK_GEN(GEN_MCK)|GCLK_CHEN;

	HW::MCLK->APBBMASK |= APBB_EVSYS;

	HW::GCLK->PCHCTRL[GCLK_SERCOM_SLOW]		= GCLK_GEN(GEN_32K)|GCLK_CHEN;	// 32 kHz
	//HW::GCLK->PCHCTRL[GCLK_SERCOM5_CORE]	= GCLK_GEN(GEN_MCK)|GCLK_CHEN;	
	//HW::GCLK->PCHCTRL[GCLK_SERCOM6_CORE]	= GCLK_GEN(GEN_MCK)|GCLK_CHEN;	
	//HW::GCLK->PCHCTRL[GCLK_SERCOM7_CORE]	= GCLK_GEN(GEN_MCK)|GCLK_CHEN;	

	//HW::MCLK->APBDMASK |= APBD_SERCOM5|APBD_SERCOM6|APBD_SERCOM7;


#endif

	SEGGER_RTT_WriteString(0, RTT_CTRL_TEXT_BRIGHT_GREEN "OK\n");

	Init_time(MCK);
	//RTT_Init();
	I2C_Init();
	SPI_Init();

#ifndef WIN32

	InitManRecieve();
	InitManTransmit();

#ifdef MAN_TRANSMIT_SSC
	InitManTransmitSSC();
#endif
	Init_ADC();

	//Init_CRC_CCITT_DMA();

	WDT_Init();

#else

	InitDisplay();

#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void UpdateHardware()
{
#ifndef WIN32

	static byte i = 0;

	static Deb db(false, 20);

	#define CALL(p) case (__LINE__-S): p; break;

	enum C { S = (__LINE__+3) };
	switch(i++)
	{
		CALL( SPI_Update();		);
	};

	i = (i > (__LINE__-S-3)) ? 0 : i;

#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
