#include "G_HW_CONF.H"
#include "G_TRM.H"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define BOOT_COM
//#define BOOT_HANDSHAKE
#define BOOT_COM_SPEED				TRM_COM_BAUDRATE
#define BOOT_COM_PARITY				TRM_COM_PARITY
#define BOOT_COM_STOPBITS			TRM_COM_STOPBITS
#define BOOT_COM_PRETIMEOUT			(~0)
#define BOOT_COM_POSTTIMEOUT		(US2COM(100))
//#define BOOT_COM_WRITEDELAY			(US2CTM(100))

#define BOOT_HANDSHAKE_PRETIMEOUT	(MS2COM(100))
#define BOOT_HANDSHAKE_POSTTIMEOUT	(US2COM(400))
#define BOOT_HANDSHAKE_TIMEOUT		(2000)
#define BOOT_SGUID					TRM_BOOT_SGUID
#define BOOT_MGUID					TRM_BOOT_MGUID
//#define BOOT_START_SECTOR				8
//#define BOOT_START_BREAKPOINT
//#define BOOT_EXIT_BREAKPOINT

#define BOOT_MAN_REQ_WORD			TRM_BOOT_REQ_WORD
#define BOOT_MAN_REQ_MASK 			TRM_BOOT_REQ_MASK

#define BOOT_MAX_NETADR				TRM_BOOT_NET_ADR
#define BOOT_TIMEOUT				(2000)
#define BOOT_MAIN_TIMEOUT			(10000)
#define BOOT_COM_ERROR_TIMEOUT		(2000)

//#define BOOT_HW_INIT				InitHardware

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inline u16 GetNetAdr() { return TRM_BOOT_NET_ADR; }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <ComPort\ComPort_imp.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static ComPort com(USART0_USIC_NUM, ~0, PIN_UTX0, PIN_URX0, PIN_RTS0);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <BOOT\boot_LPC824_imp.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
