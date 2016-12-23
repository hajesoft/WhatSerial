#include "cf_base_types.h"
#include "cf.h"
#include "cf_irp_common.h"

#ifndef _CF_IRP_URB_H_
#define	_CF_IRP_URB_H_

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)

#pragma pack(1)

typedef struct _ST_CF_IRP_URB_HEADER
{
	ST_CF_IRP_HEADER										stHeader;
	U32														u32MaximumEntries;
	U64														u32UsedEntries;
	U64														u32Status;
	ST_CF_BUFFER											stTransferBuffer;
} ST_CF_IRP_URB_HEADER, *PST_CF_IRP_URB_HEADER;

typedef struct _ST_CF_IRP_URB
{
	union
	{
		ST_CF_IRP_URB_HEADER								stUrbHeader; // URB를 사용하지 않는 IOCTL_INTERNAL_USB_GET_PORT_STATUS... 등과 같은 요청도 이 구조체를 사용한다
	};
} ST_CF_IRP_URB, *PST_CF_IRP_URB;

#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application Interface

//#define IOCTL_BASE											(0xA00)

//#define IOCTL_IOMAN7_GET_VERSION							CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE + 0x0000, METHOD_BUFFERED, FILE_ANY_ACCESS)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define IOCTL_IOMAN7_GET_LAST_ERROR							CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE + 0x0002, METHOD_BUFFERED, FILE_ANY_ACCESS)
//#define IOCTL_IOMAN7_SET_LAST_ERROR							CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE + 0x0003, METHOD_BUFFERED, FILE_ANY_ACCESS)
//#define IOCTL_IOMAN7_GET_TRACE								CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE + 0x0004, METHOD_BUFFERED, FILE_ANY_ACCESS)
//#define IOCTL_IOMAN7_SET_TRACE								CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE + 0x0005, METHOD_BUFFERED, FILE_ANY_ACCESS)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IOCTL_IOMAN7_GET_CF_IRP_URB_PEEP					CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE + 0x0030, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IOMAN7_GET_CF_IRP_URB							CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE + 0x0032, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define	IOCTL_IOMAN7_SET_CF_IRP_URB_EVENT_INPUT				CTL_CODE(FILE_DEVICE_UNKNOWN, IOCTL_BASE + 0x0035, METHOD_BUFFERED, FILE_ANY_ACCESS)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _CF_IRP_URB_H_
