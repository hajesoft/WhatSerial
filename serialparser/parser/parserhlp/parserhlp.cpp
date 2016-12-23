#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include ".\..\..\common\common_header\parserhlp.h"

extern PPARSER_CONTEXT g_pParser_Context_for_Output;

// View로 문자열을 출력하는 함수
BOOL parserHlpCmn_OutputStringToView(WCHAR * String)
{
	BOOL bRet = FALSE;
	WCHAR * TempString = NULL;

	if (g_pParser_Context_for_Output == NULL)
		return bRet;

	TempString = (WCHAR *)malloc((wcslen(String) + 1) * sizeof(WCHAR));
	wcscpy(TempString, String);
//	wcscat(TempString, L"\0");
	if (g_pParser_Context_for_Output->hlp_function_list.pfn_ViewOutput)
		bRet = g_pParser_Context_for_Output->hlp_function_list.pfn_ViewOutput(g_pParser_Context_for_Output->MgrHandle, TempString);
	free(TempString);
	return bRet;
}

// MessageBox를 출력하는 함수
int parserHlpCmn_MessageBoxToView(WCHAR * pTitle, WCHAR * pMessage, int Type, U32 dwmsecTimeout)
{
	int wRet = IDABORT;

	if (g_pParser_Context_for_Output == NULL)
		return wRet;

	if (g_pParser_Context_for_Output->hlp_function_list.pfn_MessageBox)
		wRet = g_pParser_Context_for_Output->hlp_function_list.pfn_MessageBox(g_pParser_Context_for_Output->MgrHandle, pTitle, pMessage, Type, dwmsecTimeout);

	return wRet;
}

void freeFieldFromValue(PHJ_PARSING_VALUE pValue)
{
	SIZE_T i;
	SIZE_T count;
	PHJ_PARSING_FIELD_VALUE pFieldValue = NULL;

	count = pValue->nRowFieldNameCount;
	for (i = 0; i < count; i++)
	{
		pFieldValue = (PHJ_PARSING_FIELD_VALUE)RemoveHeadList(&pValue->__FieldHead__);
		if (pFieldValue->bRowFieldValueType & HJ_VALUE_EXTRA_BUFF)
		{
			free(pFieldValue->nRowFieldBuffer);
		}
		if (pFieldValue->bRowFieldValueType & HJ_VALUE_EXTRA_PRIVATEBUFF)
		{
			free(pFieldValue->nRowFieldPrivateBuffer);
		}
		free(pFieldValue);
	}
}

void freePartialData(PHJ_PARSING_DATA pData)
{
	PHJ_PARSING_DATA pSubData = NULL;
	PHJ_PARSING_VALUE pValue = NULL;
	PHJ_PARSING_FIELD_VALUE pFieldValue = NULL;
	SIZE_T count = 0;

	pValue = NULL;
	pFieldValue = NULL;

	while (IsListEmpty(&pData->__Child__) != TRUE)
	{
		pSubData = (PHJ_PARSING_DATA)RemoveHeadList(&pData->__Child__);
		for (count = 0; count < pSubData->nParsingValueCount; count++)
		{
			pValue = (PHJ_PARSING_VALUE)RemoveHeadList(&pSubData->__ValueHead__);
			freeFieldFromValue(pValue);
			free(pValue);
		}
		free(pSubData);
	}

	// 원래 구조체를 메모리에서 해재한다
	for (count = 0; count < pData->nParsingValueCount; count++)
	{
		pValue = (PHJ_PARSING_VALUE)RemoveHeadList(&pData->__ValueHead__);
		freeFieldFromValue(pValue);
		free(pValue);
	}
	free(pData);
}

WCHAR * translateIRPStatus(U32 u32NtStatus)
{
	WCHAR * bRet = NULL;

	switch (u32NtStatus)
	{
	case 0:
		bRet = L"STATUS_SUCCESS";
		break;
	case STATUS_TIMEOUT:
		bRet = L"STATUS_TIMEOUT";
		break;
	case STATUS_PENDING:
		bRet = L"STATUS_PENDING";
		break;
	case STATUS_BUFFER_OVERFLOW:
		bRet = L"STATUS_BUFFER_OVERFLOW";
		break;
	case STATUS_DEVICE_POWERED_OFF:
		bRet = L"STATUS_DEVICE_POWERED_OFF";
		break;
	case STATUS_DEVICE_BUSY:
		bRet = L"STATUS_DEVICE_BUSY";
		break;
	case STATUS_BUS_RESET:
		bRet = L"STATUS_BUS_RESET";
		break;
	case STATUS_UNSUCCESSFUL:
		bRet = L"STATUS_UNSUCCESSFUL";
		break;
	case STATUS_ACCESS_VIOLATION:
		bRet = L"STATUS_ACCESS_VIOLATION";
		break;
	case STATUS_INVALID_HANDLE:
		bRet = L"STATUS_INVALID_HANDLE";
		break;
	case STATUS_INVALID_PARAMETER:
		bRet = L"STATUS_INVALID_PARAMETER";
		break;
	case STATUS_NO_SUCH_DEVICE:
		bRet = L"STATUS_NO_SUCH_DEVICE";
		break;
	case STATUS_INVALID_DEVICE_REQUEST:
		bRet = L"STATUS_INVALID_DEVICE_REQUEST";
		break;
	case STATUS_MORE_PROCESSING_REQUIRED:
		bRet = L"STATUS_MORE_PROCESSING_REQUIRED";
		break;
	case STATUS_ACCESS_DENIED:
		bRet = L"STATUS_ACCESS_DENIED";
		break;
	case STATUS_BUFFER_TOO_SMALL:
		bRet = L"STATUS_BUFFER_TOO_SMALL";
		break;
	case STATUS_PORT_DISCONNECTED:
		bRet = L"STATUS_PORT_DISCONNECTED";
		break;
	case STATUS_DATA_OVERRUN:
		bRet = L"STATUS_DATA_OVERRUN";
		break;
	case STATUS_DATA_ERROR:
		bRet = L"STATUS_DATA_ERROR";
		break;
	case STATUS_CRC_ERROR:
		bRet = L"STATUS_CRC_ERROR";
		break;
	case STATUS_SHARING_VIOLATION:
		bRet = L"STATUS_SHARING_VIOLATION";
		break;
	case STATUS_DELETE_PENDING:
		bRet = L"STATUS_DELETE_PENDING";
		break;
	case STATUS_CANCELLED:
		bRet = L"STATUS_CANCELLED";
		break;
	default:
		bRet = L"STATUS_UNKNOWN";
		break;
	}

	return bRet;
}


TCHAR *GetMajorFunction(U8 u8Function)
{
	switch (u8Function)
	{
	case IRP_MJ_CREATE:
		return _T("IRP_MJ_CREATE");
	case IRP_MJ_CREATE_NAMED_PIPE:
		return _T("IRP_MJ_CREATE_NAMED_PIPE");
	case IRP_MJ_CLOSE:
		return _T("IRP_MJ_CLOSE");
	case IRP_MJ_READ:
		return _T("IRP_MJ_READ");
	case IRP_MJ_WRITE:
		return _T("IRP_MJ_WRITE");
	case IRP_MJ_QUERY_INFORMATION:
		return _T("IRP_MJ_QUERY_INFORMATION");
	case IRP_MJ_SET_INFORMATION:
		return _T("IRP_MJ_SET_INFORMATION");
	case IRP_MJ_QUERY_EA:
		return _T("IRP_MJ_QUERY_EA");
	case IRP_MJ_SET_EA:
		return _T("IRP_MJ_SET_EA");
	case IRP_MJ_FLUSH_BUFFERS:
		return _T("IRP_MJ_FLUSH_BUFFERS");
	case IRP_MJ_QUERY_VOLUME_INFORMATION:
		return _T("IRP_MJ_QUERY_VOLUME_INFORMATION");
	case IRP_MJ_SET_VOLUME_INFORMATION:
		return _T("IRP_MJ_SET_VOLUME_INFORMATION");
	case IRP_MJ_DIRECTORY_CONTROL:
		return _T("IRP_MJ_DIRECTORY_CONTROL");
	case IRP_MJ_FILE_SYSTEM_CONTROL:
		return _T("IRP_MJ_FILE_SYSTEM_CONTROL");
	case IRP_MJ_DEVICE_CONTROL:
		return _T("IRP_MJ_DEVICE_CONTROL");
	case IRP_MJ_INTERNAL_DEVICE_CONTROL:
		return _T("IRP_MJ_INTERNAL_DEVICE_CONTROL");
	case IRP_MJ_SHUTDOWN:
		return _T("IRP_MJ_SHUTDOWN");
	case IRP_MJ_LOCK_CONTROL:
		return _T("IRP_MJ_LOCK_CONTROL");
	case IRP_MJ_CLEANUP:
		return _T("IRP_MJ_CLEANUP");
	case IRP_MJ_CREATE_MAILSLOT:
		return _T("IRP_MJ_CREATE_MAILSLOT");
	case IRP_MJ_QUERY_SECURITY:
		return _T("IRP_MJ_QUERY_SECURITY");
	case IRP_MJ_SET_SECURITY:
		return _T("IRP_MJ_SET_SECURITY");
	case IRP_MJ_POWER:
		return _T("IRP_MJ_POWER");
	case IRP_MJ_SYSTEM_CONTROL:
		return _T("IRP_MJ_SYSTEM_CONTROL");
	case IRP_MJ_DEVICE_CHANGE:
		return _T("IRP_MJ_DEVICE_CHANGE");
	case IRP_MJ_QUERY_QUOTA:
		return _T("IRP_MJ_QUERY_QUOTA");
	case IRP_MJ_SET_QUOTA:
		return _T("IRP_MJ_SET_QUOTA");
	case IRP_MJ_PNP:
		return _T("IRP_MJ_PNP");
	default:
		return _T("UNKNOWN");
	}
	return NULL;
}

TCHAR *GetMinorFunction(U8 u8MajorFunction, U8 u8Function)
{
	switch (u8MajorFunction)
	{
	case IRP_MJ_INTERNAL_DEVICE_CONTROL:
		return _T("");
	case IRP_MJ_POWER:
		switch (u8Function)
		{
		case IRP_MN_WAIT_WAKE:
			return _T("IRP_MN_WAIT_WAKE");
		case IRP_MN_POWER_SEQUENCE:
			return _T("IRP_MN_POWER_SEQUENCE");
		case IRP_MN_SET_POWER:
			return _T("IRP_MN_SET_POWER");
		case IRP_MN_QUERY_POWER:
			return _T("IRP_MN_QUERY_POWER");
		default:
			return _T("UNKNOWN");
		}
	case IRP_MJ_SYSTEM_CONTROL:
		switch (u8Function)
		{
		case IRP_MN_QUERY_ALL_DATA:
			return _T("IRP_MN_QUERY_ALL_DATA");
		case IRP_MN_QUERY_SINGLE_INSTANCE:
			return _T("IRP_MN_QUERY_SINGLE_INSTANCE");
		case IRP_MN_CHANGE_SINGLE_INSTANCE:
			return _T("IRP_MN_CHANGE_SINGLE_INSTANCE");
		case IRP_MN_CHANGE_SINGLE_ITEM:
			return _T("IRP_MN_CHANGE_SINGLE_ITEM");
		case IRP_MN_ENABLE_EVENTS:
			return _T("IRP_MN_ENABLE_EVENTS");
		case IRP_MN_DISABLE_EVENTS:
			return _T("IRP_MN_DISABLE_EVENTS");
		case IRP_MN_ENABLE_COLLECTION:
			return _T("IRP_MN_ENABLE_COLLECTION");
		case IRP_MN_DISABLE_COLLECTION:
			return _T("IRP_MN_DISABLE_COLLECTION");
		case IRP_MN_REGINFO:
			return _T("IRP_MN_REGINFO");
		case IRP_MN_EXECUTE_METHOD:
			return _T("IRP_MN_EXECUTE_METHOD");
		case IRP_MN_REGINFO_EX:
			return _T("IRP_MN_REGINFO_EX");
		default:
			return _T("");
		}
	case IRP_MJ_PNP:
		switch (u8Function)
		{
		case IRP_MN_START_DEVICE:
			return _T("IRP_MN_START_DEVICE");
		case IRP_MN_QUERY_REMOVE_DEVICE:
			return _T("IRP_MN_QUERY_REMOVE_DEVICE");
		case IRP_MN_REMOVE_DEVICE:
			return _T("IRP_MN_REMOVE_DEVICE");
		case IRP_MN_CANCEL_REMOVE_DEVICE:
			return _T("IRP_MN_CANCEL_REMOVE_DEVICE");
		case IRP_MN_STOP_DEVICE:
			return _T("IRP_MN_STOP_DEVICE");
		case IRP_MN_QUERY_STOP_DEVICE:
			return _T("IRP_MN_QUERY_STOP_DEVICE");
		case IRP_MN_CANCEL_STOP_DEVICE:
			return _T("IRP_MN_CANCEL_STOP_DEVICE");
		case IRP_MN_QUERY_DEVICE_RELATIONS:
			return _T("IRP_MN_QUERY_DEVICE_RELATIONS");
		case IRP_MN_QUERY_INTERFACE:
			return _T("IRP_MN_QUERY_INTERFACE");
		case IRP_MN_QUERY_CAPABILITIES:
			return _T("IRP_MN_QUERY_CAPABILITIES");
		case IRP_MN_QUERY_RESOURCES:
			return _T("IRP_MN_QUERY_RESOURCES");
		case IRP_MN_QUERY_RESOURCE_REQUIREMENTS:
			return _T("IRP_MN_QUERY_RESOURCE_REQUIREMENTS");
		case IRP_MN_QUERY_DEVICE_TEXT:
			return _T("IRP_MN_QUERY_DEVICE_TEXT");
		case IRP_MN_FILTER_RESOURCE_REQUIREMENTS:
			return _T("IRP_MN_FILTER_RESOURCE_REQUIREMENTS");
		case IRP_MN_READ_CONFIG:
			return _T("IRP_MN_READ_CONFIG");
		case IRP_MN_WRITE_CONFIG:
			return _T("IRP_MN_WRITE_CONFIG");
		case IRP_MN_EJECT:
			return _T("IRP_MN_EJECT");
		case IRP_MN_SET_LOCK:
			return _T("IRP_MN_SET_LOCK");
		case IRP_MN_QUERY_ID:
			return _T("IRP_MN_QUERY_ID");
		case IRP_MN_QUERY_PNP_DEVICE_STATE:
			return _T("IRP_MN_QUERY_PNP_DEVICE_STATE");
		case IRP_MN_QUERY_BUS_INFORMATION:
			return _T("IRP_MN_QUERY_BUS_INFORMATION");
		case IRP_MN_DEVICE_USAGE_NOTIFICATION:
			return _T("IRP_MN_DEVICE_USAGE_NOTIFICATION");
		case IRP_MN_SURPRISE_REMOVAL:
			return _T("IRP_MN_SURPRISE_REMOVAL");
		default:
			return _T("");
		}
	default:
		return _T("");
	}
	return NULL;
}

BOOL	parserHlpCmn_free(PVOID handle, PVOID Buffer, SIZE_T Size)
{
	SIZE_T count = 0;
	PHJ_PARSING_FIXED_DATA_ENTRY pFixedData = NULL;
	PHJ_PARSING_FIXED_DATA pBaseFixedData = NULL;
	PHJ_PARSING_DATA	pDetailsPage = NULL;
	PHJ_PARSING_DATA_FOR_DATA	pDataPage = NULL;

	pBaseFixedData = (PHJ_PARSING_FIXED_DATA)Buffer;

	while (IsListEmpty(&pBaseFixedData->__AllDataEntryHeader__) == FALSE)
	{
		pFixedData = (PHJ_PARSING_FIXED_DATA_ENTRY)RemoveHeadList(&pBaseFixedData->__AllDataEntryHeader__);

		pDataPage = pFixedData->pDataPage;
		pDetailsPage = pFixedData->pDetailsPage;

		for (count = 0; count < pFixedData->ColCount; count++)
		{
			if (pFixedData->ColumnData[count].bIsConstant == FALSE)
			{
				if (pFixedData->ColumnData[count].pszData)
					free(pFixedData->ColumnData[count].pszData);
			}
		}

		free(pFixedData);

		if (pDataPage)
		{
			if (pDataPage->Size)
			{
				if (pDataPage->bIsConstant == FALSE)
				{
					free(pDataPage->pDataBuffer);
				}
			}
			free(pDataPage);
		}

		if (pDetailsPage)
			freePartialData(pDetailsPage);

	}

	free(pBaseFixedData);

	return TRUE;
}

void parserHlpCmn_FillField(PHJ_PARSING_FIELD_VALUE pField, WCHAR *FieldName, U32 Type, WCHAR *ValueName, SIZE_T ValueBitOff, SIZE_T ValueBitSize, SIZE_T ValueByteOff, SIZE_T ValueByteSize, void * pRealData)
{
	pField->pRowFieldName = FieldName;
	pField->bRowFieldValueType = Type;
	pField->pRowFieldValueName = ValueName;
	pField->nRowFieldValueBitsOffset = ValueBitOff;
	pField->nRowFieldValueBitsSize = ValueBitSize;
	pField->nRowFieldValueBytesOffset = ValueByteOff;
	pField->nRowFieldValueBytesSize = ValueByteSize;

	if (pRealData)
		memcpy(&pField->nRowFieldValue[0], pRealData, ValueByteSize);

}

PHJ_PARSING_DATA parserHlpCmn_CreateParsingData(PST_CF_IRP_URB pSource, WCHAR * DataTitleName, SIZE_T ExtensionSize)
{
	PHJ_PARSING_DATA pData = NULL;

	pData = (PHJ_PARSING_DATA)malloc(sizeof(HJ_PARSING_DATA) + ExtensionSize);

//	MemoryLeakCheck(_T("CreateParentData, PHJ_PARSING_DATA"), pData);

	pData->Type = HJ_STRUCT_TYPE__PARSING_DATA;
	InitializeListHead(&pData->__Sibling__);

	pData->Level = 1;
	InitializeListHead(&pData->__Child__);
	pData->ChildCount = 0;

	pData->pszTitle = DataTitleName;

	pData->nParsingValueCount = 0;
	InitializeListHead(&pData->__ValueHead__);

	return pData;
}

PHJ_PARSING_DATA parserHlpCmn_CreateAndLinkParsingDataToParsingData(PHJ_PARSING_DATA pMainData, WCHAR * DataTitleName)
{
	PHJ_PARSING_DATA pSubData = NULL;
	pMainData->ChildCount++;

	pSubData = parserHlpCmn_CreateParsingData(NULL, DataTitleName, 0);
	pSubData->Level = pMainData->Level + 1;

	InsertTailList(&pMainData->__Child__, &pSubData->__Sibling__);
	return pSubData;
}

PHJ_PARSING_VALUE parserHlpCmn_CreateAndLinkParsingDataToParsingValue(PHJ_PARSING_DATA pData, WCHAR * ValueTitleName)
{
	PHJ_PARSING_VALUE pValue = NULL;

	pValue = (PHJ_PARSING_VALUE)malloc(sizeof(HJ_PARSING_VALUE));

	pValue->nRowFieldNameCount = 0;
	InitializeListHead(&pValue->__FieldHead__);

//	MemoryLeakCheck(_T("parserHlpCmn_CreateAndLinkParsingDataToParsingValue, PHJ_PARSING_VALUE"), pValue);

	InsertTailList(&pData->__ValueHead__, &pValue->__Self__);
	//	pData->pParsingValue[pData->nParsingValueCount] = pValue;
	pData->nParsingValueCount++;

	pValue->Title = ValueTitleName;
	return pValue;
}

PHJ_PARSING_FIELD_VALUE parserHlpCmn_CreateAndLinkParsingValueToParsingField(PHJ_PARSING_VALUE pValue)
{
	PHJ_PARSING_FIELD_VALUE pField = NULL;
	pField = (PHJ_PARSING_FIELD_VALUE)malloc(sizeof(HJ_PARSING_FIELD_VALUE));

//	MemoryLeakCheck(_T("parserHlpCmn_CreateAndLinkParsingValueToParsingField, PHJ_PARSING_FIELD_VALUE"), pField);

	InsertTailList(&pValue->__FieldHead__, &pField->__Self__);
	//	pValue->pFieldInfo[pValue->nRowFieldNameCount] = pField;
	pValue->nRowFieldNameCount++;
	return pField;
}

void GetPurgeTranslate(PHJ_PARSING_DATA pData, U32 u32Purge)
{
	PSPECIFIC_INFO pSpecificInfo = NULL;
	WCHAR TempString[MAX_PATH] = { 0, };
	int last;

	pSpecificInfo = (PSPECIFIC_INFO)(pData + 1); // 나중을 위해서 예비한 메모리영역

	if (u32Purge & SERIAL_PURGE_TXABORT)
	{
		wcscat(TempString, L"TXABORT | ");
	}
	if (u32Purge & SERIAL_PURGE_RXABORT)
	{
		wcscat(TempString, L"RXABORT | ");
	}
	if (u32Purge & SERIAL_PURGE_TXCLEAR)
	{
		wcscat(TempString, L"TXCLEAR | ");
	}
	if (u32Purge & SERIAL_PURGE_RXCLEAR)
	{
		wcscat(TempString, L"RXCLEAR | ");
	}

	pSpecificInfo->PurgeString[0] = 0;

	last = wcslen(TempString);
	if (last)
	{
		TempString[last - 2] = 0;
		TempString[last - 1] = 0;
		wcscpy(pSpecificInfo->PurgeString, TempString);
	}
}

void GetMaskTranslate(PHJ_PARSING_DATA pData, U32 u32Mask)
{
	PSPECIFIC_INFO pSpecificInfo = NULL;
	WCHAR TempString[MAX_PATH] = { 0, };
	int last;

	pSpecificInfo = (PSPECIFIC_INFO)(pData + 1); // 나중을 위해서 예비한 메모리영역

	if (u32Mask & SERIAL_EV_RXCHAR)
	{
		wcscat(TempString, L"EV_RXCHAR | ");
	}
	if (u32Mask & SERIAL_EV_RXFLAG)
	{
		wcscat(TempString, L"EV_RXFLAG | ");
	}
	if (u32Mask & SERIAL_EV_TXEMPTY)
	{
		wcscat(TempString, L"EV_TXEMPTY | ");
	}
	if (u32Mask & SERIAL_EV_CTS)
	{
		wcscat(TempString, L"EV_CTS | ");
	}
	if (u32Mask & SERIAL_EV_DSR)
	{
		wcscat(TempString, L"EV_DSR,");
	}
	if (u32Mask & SERIAL_EV_RLSD)
	{
		wcscat(TempString, L"EV_RLSD | ");
	}
	if (u32Mask & SERIAL_EV_BREAK)
	{
		wcscat(TempString, L"EV_BREAK | ");
	}
	if (u32Mask & SERIAL_EV_ERR)
	{
		wcscat(TempString, L"EV_ERR | ");
	}
	if (u32Mask & SERIAL_EV_RING)
	{
		wcscat(TempString, L"EV_RING | ");
	}
	if (u32Mask & SERIAL_EV_PERR)
	{
		wcscat(TempString, L"EV_PERR | ");
	}
	if (u32Mask & SERIAL_EV_RX80FULL)
	{
		wcscat(TempString, L"EV_RX80FULL | ");
	}
	if (u32Mask & SERIAL_EV_EVENT1)
	{
		wcscat(TempString, L"EV_EVENT1 | ");
	}
	if (u32Mask & SERIAL_EV_EVENT2)
	{
		wcscat(TempString, L"EV_EVENT2 | ");
	}

	pSpecificInfo->WaitMaskString[0] = 0;

	last = wcslen(TempString);
	if (last)
	{
		TempString[last - 2] = 0;
		TempString[last - 1] = 0;
		wcscpy(pSpecificInfo->WaitMaskString, TempString);
	}
}

PHJ_PARSING_DATA_FOR_DATA FillDataItemForIoctl(U32 u32IoControlCode, void * pBuffer, U64 Length, BOOLEAN bCompletion)
{
	PHJ_PARSING_VALUE pValue = NULL;
	PHJ_PARSING_FIELD_VALUE pFieldValue = NULL;
	PHJ_PARSING_DATA_FOR_DATA pData = NULL;

	switch (u32IoControlCode)
	{
	case IOCTL_SERIAL_SET_BAUD_RATE:
	case IOCTL_SERIAL_SET_QUEUE_SIZE:
	case IOCTL_SERIAL_SET_LINE_CONTROL:
	case IOCTL_SERIAL_IMMEDIATE_CHAR:
	case IOCTL_SERIAL_SET_TIMEOUTS:
	case IOCTL_SERIAL_SET_WAIT_MASK:
	case IOCTL_SERIAL_WAIT_ON_MASK:
	case IOCTL_SERIAL_PURGE:
	case IOCTL_SERIAL_SET_CHARS:
	case IOCTL_SERIAL_SET_HANDFLOW:
	case IOCTL_SERIAL_XOFF_COUNTER:
	case IOCTL_SERIAL_LSRMST_INSERT:
	case IOCTL_SERIAL_SET_MODEM_CONTROL:
	case IOCTL_SERIAL_SET_FIFO_CONTROL:
		pData = (PHJ_PARSING_DATA_FOR_DATA)malloc(sizeof(HJ_PARSING_DATA_FOR_DATA));
		pData->bIsConstant = FALSE;
		pData->Size = (SIZE_T)Length;
		pData->pDataBuffer = malloc(pData->Size);
		memcpy(pData->pDataBuffer, pBuffer, pData->Size);
		break;
	case IOCTL_SERIAL_GET_TIMEOUTS:
	case IOCTL_SERIAL_GET_WAIT_MASK:
	case IOCTL_SERIAL_GET_BAUD_RATE:
	case IOCTL_SERIAL_GET_LINE_CONTROL:
	case IOCTL_SERIAL_GET_CHARS:
	case IOCTL_SERIAL_GET_HANDFLOW:
	case IOCTL_SERIAL_GET_MODEMSTATUS:
	case IOCTL_SERIAL_GET_COMMSTATUS:
	case IOCTL_SERIAL_GET_PROPERTIES:
	case IOCTL_SERIAL_GET_DTRRTS:
	case IOCTL_SERIAL_CONFIG_SIZE:
	case IOCTL_SERIAL_GET_STATS:
	case IOCTL_SERIAL_GET_MODEM_CONTROL:
		if (bCompletion == FALSE)
			break;
		pData = (PHJ_PARSING_DATA_FOR_DATA)malloc(sizeof(HJ_PARSING_DATA_FOR_DATA));
		pData->bIsConstant = FALSE;
		pData->Size = (SIZE_T)Length;
		pData->pDataBuffer = malloc(pData->Size);
		memcpy(pData->pDataBuffer, pBuffer, pData->Size);
		break;
	}
	return pData;
}

void FillItemForIoctl(U32 u32IoControlCode, PHJ_PARSING_DATA pData, void * pBuffer, BOOLEAN bCompletion)
{
	PHJ_PARSING_VALUE pValue = NULL;
	PHJ_PARSING_FIELD_VALUE pFieldValue = NULL;

	switch (u32IoControlCode)
	{
	case IOCTL_SERIAL_SET_BAUD_RATE:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_BAUD_RATE"));
		{
			PSERIAL_BAUD_RATE pSerialBaudRate;
			pSerialBaudRate = (PSERIAL_BAUD_RATE)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"BaudRate", HJ_VALUE_ENDIAN_LSB, L"보율",
				0, 32, 0, sizeof(U32), &pSerialBaudRate->BaudRate);
		}
		break;
	case IOCTL_SERIAL_SET_QUEUE_SIZE:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_QUEUE_SIZE"));
		{
			PSERIAL_QUEUE_SIZE pSerialQueueSize;
			pSerialQueueSize = (PSERIAL_QUEUE_SIZE)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"InSize", HJ_VALUE_ENDIAN_LSB, L"Input Queue 크기",
				0, 32, 0, sizeof(U32), &pSerialQueueSize->InSize);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"OutSize", HJ_VALUE_ENDIAN_LSB, L"Output Queue 크기",
				0, 32, 4, sizeof(U32), &pSerialQueueSize->OutSize);
		}
		break;
	case IOCTL_SERIAL_SET_LINE_CONTROL:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_LINE_CONTROL"));
		{
			PSERIAL_LINE_CONTROL pSerialLineControl;
			pSerialLineControl = (PSERIAL_LINE_CONTROL)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"StopBits", HJ_VALUE_ENDIAN_LSB, L"Stop Bit수",
				0, 8, 0, sizeof(U8), &pSerialLineControl->StopBits);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"Parity", HJ_VALUE_ENDIAN_LSB, L"패리티체크 종류",
				0, 8, 1, sizeof(U8), &pSerialLineControl->Parity);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WordLength", HJ_VALUE_ENDIAN_LSB, L"데이타 비트수",
				0, 8, 2, sizeof(U8), &pSerialLineControl->WordLength);
		}
		break;
	case IOCTL_SERIAL_IMMEDIATE_CHAR:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("IMMEDIATE_CHAR"));
		{
			U8 * pSerialImmediateChar;
			pSerialImmediateChar = (U8 *)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ImmediateChar", HJ_VALUE_ENDIAN_LSB, L"출력문자",
				0, 8, 0, sizeof(U8), pSerialImmediateChar);
		}
		break;
	case IOCTL_SERIAL_SET_TIMEOUTS:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_TIMEOUTS"));
		{
			PSERIAL_TIMEOUTS pSerialTimeOut;
			pSerialTimeOut = (PSERIAL_TIMEOUTS)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ReadIntervalTimeout", HJ_VALUE_ENDIAN_LSB, L"수신되는 문자들 사이의 타임아웃",
				0, 32, 0, sizeof(U32), &pSerialTimeOut->ReadIntervalTimeout);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ReadTotalTimeoutMultiplier", HJ_VALUE_ENDIAN_LSB, L"수신될 문자의 개수의 복수배 타임아웃",
				0, 32, 4, sizeof(U32), &pSerialTimeOut->ReadTotalTimeoutMultiplier);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ReadTotalTimeoutConstant", HJ_VALUE_ENDIAN_LSB, L"전체 타임아웃 추가시간",
				0, 32, 8, sizeof(U32), &pSerialTimeOut->ReadTotalTimeoutConstant);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WriteTotalTimeoutMultiplier", HJ_VALUE_ENDIAN_LSB, L"송신할 문자의 개수의 복수배 타임아웃",
				0, 32, 12, sizeof(U32), &pSerialTimeOut->WriteTotalTimeoutMultiplier);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WriteTotalTimeoutConstant", HJ_VALUE_ENDIAN_LSB, L"전체 타임아웃 추가시간",
				0, 32, 16, sizeof(U32), &pSerialTimeOut->WriteTotalTimeoutConstant);
		}
		break;
	case IOCTL_SERIAL_GET_TIMEOUTS:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_TIMEOUTS"));
		{
			PSERIAL_TIMEOUTS pSerialTimeOut;
			pSerialTimeOut = (PSERIAL_TIMEOUTS)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ReadIntervalTimeout", HJ_VALUE_ENDIAN_LSB, L"수신되는 문자들 사이의 타임아웃",
				0, 32, 0, sizeof(U32), &pSerialTimeOut->ReadIntervalTimeout);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ReadTotalTimeoutMultiplier", HJ_VALUE_ENDIAN_LSB, L"수신될 문자의 개수의 복수배 타임아웃",
				0, 32, 4, sizeof(U32), &pSerialTimeOut->ReadTotalTimeoutMultiplier);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ReadTotalTimeoutConstant", HJ_VALUE_ENDIAN_LSB, L"전체 타임아웃 추가시간",
				0, 32, 8, sizeof(U32), &pSerialTimeOut->ReadTotalTimeoutConstant);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WriteTotalTimeoutMultiplier", HJ_VALUE_ENDIAN_LSB, L"송신할 문자의 개수의 복수배 타임아웃",
				0, 32, 12, sizeof(U32), &pSerialTimeOut->WriteTotalTimeoutMultiplier);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WriteTotalTimeoutConstant", HJ_VALUE_ENDIAN_LSB, L"전체 타임아웃 추가시간",
				0, 32, 16, sizeof(U32), &pSerialTimeOut->WriteTotalTimeoutConstant);
		}
		break;
	case IOCTL_SERIAL_GET_WAIT_MASK:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("WAIT_MASK"));
		{
			U32 * pSerialWaitMask;
			PSPECIFIC_INFO pSpecific;

			pSerialWaitMask = (U32 *)pBuffer;

			GetMaskTranslate(pData, *pSerialWaitMask);
			pSpecific = (PSPECIFIC_INFO)(pData + 1);
			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WaitMask", HJ_VALUE_ENDIAN_LSB,
				pSpecific->WaitMaskString, 0, 32, 0, sizeof(U32), pSerialWaitMask);
		}
		break;
	case IOCTL_SERIAL_SET_WAIT_MASK:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("WAIT_MASK"));
		{
			U32 * pSerialWaitMask;
			PSPECIFIC_INFO pSpecific;

			pSerialWaitMask = (U32 *)pBuffer;

			GetMaskTranslate(pData, *pSerialWaitMask);
			pSpecific = (PSPECIFIC_INFO)(pData + 1);
			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WaitMask", HJ_VALUE_ENDIAN_LSB,
				pSpecific->WaitMaskString, 0, 32, 0, sizeof(U32), pSerialWaitMask);
		}
		break;
	case IOCTL_SERIAL_WAIT_ON_MASK:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("WAIT_MASK"));
		{
			U32 * pSerialWaitMask;
			PSPECIFIC_INFO pSpecific;

			pSerialWaitMask = (U32 *)pBuffer;

			GetMaskTranslate(pData, *pSerialWaitMask);
			pSpecific = (PSPECIFIC_INFO)(pData + 1);
			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WaitMask", HJ_VALUE_ENDIAN_LSB,
				pSpecific->WaitMaskString, 0, 32, 0, sizeof(U32), pSerialWaitMask);
		}
		break;
	case IOCTL_SERIAL_PURGE:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("PURGE"));
		{
			U32 * pSerialPurge;
			PSPECIFIC_INFO pSpecific;

			pSerialPurge = (U32 *)pBuffer;

			GetPurgeTranslate(pData, *pSerialPurge);
			pSpecific = (PSPECIFIC_INFO)(pData + 1);
			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"Purge", HJ_VALUE_ENDIAN_LSB,
				pSpecific->PurgeString, 0, 32, 0, sizeof(U32), pSerialPurge);
		}
		break;
	case IOCTL_SERIAL_GET_BAUD_RATE:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_BAUD_RATE"));
		{
			PSERIAL_BAUD_RATE pSerialBaudRate;
			pSerialBaudRate = (PSERIAL_BAUD_RATE)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"BaudRate", HJ_VALUE_ENDIAN_LSB, L"보율",
				0, 32, 0, sizeof(U32), &pSerialBaudRate->BaudRate);
		}
		break;
	case IOCTL_SERIAL_GET_LINE_CONTROL:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_LINE_CONTROL"));
		{
			PSERIAL_LINE_CONTROL pSerialLineControl;
			pSerialLineControl = (PSERIAL_LINE_CONTROL)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"StopBits", HJ_VALUE_ENDIAN_LSB, L"Stop Bit수",
				0, 8, 0, sizeof(U8), &pSerialLineControl->StopBits);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"Parity", HJ_VALUE_ENDIAN_LSB, L"패리티체크 종류",
				0, 8, 1, sizeof(U8), &pSerialLineControl->Parity);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WordLength", HJ_VALUE_ENDIAN_LSB, L"데이타 비트수",
				0, 8, 2, sizeof(U8), &pSerialLineControl->WordLength);
		}
		break;
	case IOCTL_SERIAL_GET_CHARS:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_CHARS"));
		{
			PSERIAL_CHARS pSerialChars;
			pSerialChars = (PSERIAL_CHARS)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"EofChar", HJ_VALUE_ENDIAN_LSB, L"마지막 문자",
				0, 8, 0, sizeof(U8), &pSerialChars->EofChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ErrorChar", HJ_VALUE_ENDIAN_LSB, L"에러 문자",
				0, 8, 1, sizeof(U8), &pSerialChars->ErrorChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"BreakChar", HJ_VALUE_ENDIAN_LSB, L"중단 문자",
				0, 8, 2, sizeof(U8), &pSerialChars->BreakChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"EventChar", HJ_VALUE_ENDIAN_LSB, L"이벤트 문자",
				0, 8, 3, sizeof(U8), &pSerialChars->EventChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"XonChar", HJ_VALUE_ENDIAN_LSB, L"XON 문자",
				0, 8, 4, sizeof(U8), &pSerialChars->XonChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"XoffChar", HJ_VALUE_ENDIAN_LSB, L"XOFF 문자",
				0, 8, 5, sizeof(U8), &pSerialChars->XoffChar);
		}
		break;
	case IOCTL_SERIAL_SET_CHARS:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_CHARS"));
		{
			PSERIAL_CHARS pSerialChars;
			pSerialChars = (PSERIAL_CHARS)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"EofChar", HJ_VALUE_ENDIAN_LSB, L"마지막 문자",
				0, 8, 0, sizeof(U8), &pSerialChars->EofChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ErrorChar", HJ_VALUE_ENDIAN_LSB, L"에러 문자",
				0, 8, 1, sizeof(U8), &pSerialChars->ErrorChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"BreakChar", HJ_VALUE_ENDIAN_LSB, L"중단 문자",
				0, 8, 2, sizeof(U8), &pSerialChars->BreakChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"EventChar", HJ_VALUE_ENDIAN_LSB, L"이벤트 문자",
				0, 8, 3, sizeof(U8), &pSerialChars->EventChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"XonChar", HJ_VALUE_ENDIAN_LSB, L"XON 문자",
				0, 8, 4, sizeof(U8), &pSerialChars->XonChar);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"XoffChar", HJ_VALUE_ENDIAN_LSB, L"XOFF 문자",
				0, 8, 5, sizeof(U8), &pSerialChars->XoffChar);
		}
		break;
	case IOCTL_SERIAL_GET_HANDFLOW:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_HANDFLOW"));
		{
			PSERIAL_HANDFLOW pSerialHandFlow;
			pSerialHandFlow = (PSERIAL_HANDFLOW)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ControlHandShake", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 0, sizeof(U32), &pSerialHandFlow->ControlHandShake);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"FlowReplace", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 4, sizeof(U32), &pSerialHandFlow->FlowReplace);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"XonLimit", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 8, sizeof(U32), &pSerialHandFlow->XonLimit);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"XoffLimit", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 12, sizeof(U32), &pSerialHandFlow->XoffLimit);
		}
		break;
	case IOCTL_SERIAL_SET_HANDFLOW:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_HANDFLOW"));
		{
			PSERIAL_HANDFLOW pSerialHandFlow;
			pSerialHandFlow = (PSERIAL_HANDFLOW)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ControlHandShake", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 0, sizeof(U32), &pSerialHandFlow->ControlHandShake);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"FlowReplace", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 4, sizeof(U32), &pSerialHandFlow->FlowReplace);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"XonLimit", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 8, sizeof(U32), &pSerialHandFlow->XonLimit);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"XoffLimit", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 12, sizeof(U32), &pSerialHandFlow->XoffLimit);
		}
		break;
	case IOCTL_SERIAL_GET_MODEMSTATUS:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("MODEMSTATUS"));
		{
			U32 * pSerialModemStatus;
			pSerialModemStatus = (U32 *)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ModemStatus", HJ_VALUE_ENDIAN_LSB, L"모뎀상태레지스터값",
				0, 32, 0, sizeof(U32), pSerialModemStatus);
		}
		break;
	case IOCTL_SERIAL_GET_COMMSTATUS:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_STATUS"));
		{
			PSERIAL_STATUS pSerialStatus;
			pSerialStatus = (PSERIAL_STATUS)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"Errors", HJ_VALUE_ENDIAN_LSB, L"에러처리된 데이타수",
				0, 32, 0, sizeof(U32), &pSerialStatus->Errors);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"HoldReasons", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 4, sizeof(U32), &pSerialStatus->HoldReasons);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"AmountInInQueue", HJ_VALUE_ENDIAN_LSB, L"수신 큐에 보관중인 데이타수",
				0, 32, 8, sizeof(U32), &pSerialStatus->AmountInInQueue);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"AmountInOutQueue", HJ_VALUE_ENDIAN_LSB, L"송신 큐에 보관중인 데이타수",
				0, 32, 12, sizeof(U32), &pSerialStatus->AmountInOutQueue);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"EofReceived", HJ_VALUE_ENDIAN_LSB, L"수신된 EOF 데이타수",
				0, 8, 16, sizeof(U8), &pSerialStatus->EofReceived);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"WaitForImmediate", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 8, 17, sizeof(U8), &pSerialStatus->WaitForImmediate);

		}
		break;
	case IOCTL_SERIAL_XOFF_COUNTER:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_XOFF_COUNTER"));
		{
			PSERIAL_XOFF_COUNTER pSerialXoffCounter;
			pSerialXoffCounter = (PSERIAL_XOFF_COUNTER)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"Timeout", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 0, sizeof(U32), &pSerialXoffCounter->Timeout);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"Counter", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 4, sizeof(U32), &pSerialXoffCounter->Counter);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"XoffChar", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 8, 8, sizeof(U8), &pSerialXoffCounter->XoffChar);
		}
		break;
	case IOCTL_SERIAL_GET_PROPERTIES:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIAL_COMMPROP"));
		{
			PSERIAL_COMMPROP pSerialCommProp;
			pSerialCommProp = (PSERIAL_COMMPROP)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"PacketLength", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 16, 0, sizeof(U16), &pSerialCommProp->PacketLength);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"PacketVersion", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 16, 2, sizeof(U16), &pSerialCommProp->PacketVersion);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ServiceMask", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 4, sizeof(U32), &pSerialCommProp->ServiceMask);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"Reserved1", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 8, sizeof(U32), &pSerialCommProp->Reserved1);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"MaxTxQueue", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 12, sizeof(U32), &pSerialCommProp->MaxTxQueue);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"MaxRxQueue", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 16, sizeof(U32), &pSerialCommProp->MaxRxQueue);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"MaxBaud", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 20, sizeof(U32), &pSerialCommProp->MaxBaud);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ProvSubType", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 24, sizeof(U32), &pSerialCommProp->ProvSubType);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ProvCapabilities", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 28, sizeof(U32), &pSerialCommProp->ProvCapabilities);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"SettableParams", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 32, sizeof(U32), &pSerialCommProp->SettableParams);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"SettableBaud", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 36, sizeof(U32), &pSerialCommProp->SettableBaud);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"SettableData", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 16, 40, sizeof(U16), &pSerialCommProp->SettableData);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"SettableStopParity", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 16, 42, sizeof(U16), &pSerialCommProp->SettableStopParity);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"CurrentTxQueue", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 44, sizeof(U32), &pSerialCommProp->CurrentTxQueue);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"CurrentRxQueue", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 46, sizeof(U32), &pSerialCommProp->CurrentRxQueue);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ProvSpec1", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 50, sizeof(U32), &pSerialCommProp->ProvSpec1);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ProvSpec2", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 54, sizeof(U32), &pSerialCommProp->ProvSpec2);
		}
		break;
	case IOCTL_SERIAL_GET_DTRRTS:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("DTRRTS"));
		{
			U32 * pSerialDtrRts;
			pSerialDtrRts = (U32 *)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"DtrRts", HJ_VALUE_ENDIAN_LSB, L"DTR과 RTS레지스터값",
				0, 32, 0, sizeof(U32), pSerialDtrRts);
		}
		break;
	case IOCTL_SERIAL_LSRMST_INSERT:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("LSRMST_INSERT"));
		{
			U8 * pSerialInsert;
			pSerialInsert = (U8 *)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"Insert", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 8, 0, sizeof(U8), pSerialInsert);
		}
		break;
	case IOCTL_SERIAL_CONFIG_SIZE:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("CONFIG_SIZE"));
		{
			U32 * pSerialConfigSize;
			pSerialConfigSize = (U32 *)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ConfigSize", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 0, sizeof(U32), pSerialConfigSize);
		}
		break;
	case IOCTL_SERIAL_GET_STATS:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("SERIALPERF_STATS"));
		{
			PSERIALPERF_STATS pSerialPerfStats;
			pSerialPerfStats = (PSERIALPERF_STATS)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ReceivedCount", HJ_VALUE_ENDIAN_LSB, L"통계정보(수신데이타수)",
				0, 32, 0, sizeof(U32), &pSerialPerfStats->ReceivedCount);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"TransmittedCount", HJ_VALUE_ENDIAN_LSB, L"통계정보(송신데이타수)",
				0, 32, 4, sizeof(U32), &pSerialPerfStats->TransmittedCount);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"FrameErrorCount", HJ_VALUE_ENDIAN_LSB, L"통계정보(프레임에러데이타수)",
				0, 32, 8, sizeof(U32), &pSerialPerfStats->FrameErrorCount);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"SerialOverrunErrorCount", HJ_VALUE_ENDIAN_LSB, L"통계정보(하드웨어 덮어쓰기오류데이타수)",
				0, 32, 12, sizeof(U32), &pSerialPerfStats->SerialOverrunErrorCount);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"BufferOverrunErrorCount", HJ_VALUE_ENDIAN_LSB, L"통계정보(내부 메모리 덮어쓰기오류데이타수)",
				0, 32, 16, sizeof(U32), &pSerialPerfStats->BufferOverrunErrorCount);

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ParityErrorCount", HJ_VALUE_ENDIAN_LSB, L"통계정보(페리티에러데이타수)",
				0, 32, 20, sizeof(U32), &pSerialPerfStats->ParityErrorCount);
		}

		break;
	case IOCTL_SERIAL_GET_MODEM_CONTROL:
		if (bCompletion == FALSE)
			break;
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("MODEM_CONTROL"));
		{
			U32 * pSerialModemControl;
			pSerialModemControl = (U32 *)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ModemControl", HJ_VALUE_ENDIAN_LSB, L"모뎀 Control 레지스터값",
				0, 32, 0, sizeof(U32), pSerialModemControl);
		}
		break;
	case IOCTL_SERIAL_SET_MODEM_CONTROL:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("MODEM_CONTROL"));
		{
			U32 * pSerialModemControl;
			pSerialModemControl = (U32 *)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"ModemControl", HJ_VALUE_ENDIAN_LSB, L"모뎀 Control 레지스터값",
				0, 32, 0, sizeof(U32), pSerialModemControl);
		}
		break;
	case IOCTL_SERIAL_SET_FIFO_CONTROL:
		pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("FIFO_CONTROL"));
		{
			U32 * pSerialFifoControl;
			pSerialFifoControl = (U32 *)pBuffer;

			pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
			parserHlpCmn_FillField(pFieldValue, L"FifoControl", HJ_VALUE_ENDIAN_LSB, NULL,
				0, 32, 0, sizeof(U32), pSerialFifoControl);
		}
		break;
	}
}

WCHAR * ReplaceItemForIoctl(U32 u32IoControlCode)
{
	WCHAR * pRet = NULL;
	switch (u32IoControlCode)
	{
	case IOCTL_SERIAL_SET_BAUD_RATE:
		pRet = _T("IOCTL_SERIAL_SET_BAUD_RATE");
		break;
	case IOCTL_SERIAL_SET_QUEUE_SIZE:
		pRet = _T("IOCTL_SERIAL_SET_QUEUE_SIZE");
		break;
	case IOCTL_SERIAL_SET_LINE_CONTROL:
		pRet = _T("IOCTL_SERIAL_SET_LINE_CONTROL");
		break;
	case IOCTL_SERIAL_SET_BREAK_ON:
		pRet = _T("IOCTL_SERIAL_SET_BREAK_ON");
		break;
	case IOCTL_SERIAL_SET_BREAK_OFF:
		pRet = _T("IOCTL_SERIAL_SET_BREAK_OFF");
		break;
	case IOCTL_SERIAL_IMMEDIATE_CHAR:
		pRet = _T("IOCTL_SERIAL_IMMEDIATE_CHAR");
		break;
	case IOCTL_SERIAL_SET_TIMEOUTS:
		pRet = _T("IOCTL_SERIAL_SET_TIMEOUTS");
		break;
	case IOCTL_SERIAL_GET_TIMEOUTS:
		pRet = _T("IOCTL_SERIAL_GET_TIMEOUTS");
		break;
	case IOCTL_SERIAL_SET_DTR:
		pRet = _T("IOCTL_SERIAL_SET_DTR");
		break;
	case IOCTL_SERIAL_CLR_DTR:
		pRet = _T("IOCTL_SERIAL_CLR_DTR");
		break;
	case IOCTL_SERIAL_RESET_DEVICE:
		pRet = _T("IOCTL_SERIAL_RESET_DEVICE");
		break;
	case IOCTL_SERIAL_SET_RTS:
		pRet = _T("IOCTL_SERIAL_SET_RTS");
		break;
	case IOCTL_SERIAL_CLR_RTS:
		pRet = _T("IOCTL_SERIAL_CLR_RTS");
		break;
	case IOCTL_SERIAL_SET_XOFF:
		pRet = _T("IOCTL_SERIAL_SET_XOFF");
		break;
	case IOCTL_SERIAL_SET_XON:
		pRet = _T("IOCTL_SERIAL_SET_XON");
		break;
	case IOCTL_SERIAL_GET_WAIT_MASK:
		pRet = _T("IOCTL_SERIAL_GET_WAIT_MASK");
		break;
	case IOCTL_SERIAL_SET_WAIT_MASK:
		pRet = _T("IOCTL_SERIAL_SET_WAIT_MASK");
		break;
	case IOCTL_SERIAL_WAIT_ON_MASK:
		pRet = _T("IOCTL_SERIAL_WAIT_ON_MASK");
		break;
	case IOCTL_SERIAL_PURGE:
		pRet = _T("IOCTL_SERIAL_PURGE");
		break;
	case IOCTL_SERIAL_GET_BAUD_RATE:
		pRet = _T("IOCTL_SERIAL_GET_BAUD_RATE");
		break;
	case IOCTL_SERIAL_GET_LINE_CONTROL:
		pRet = _T("IOCTL_SERIAL_GET_LINE_CONTROL");
		break;
	case IOCTL_SERIAL_GET_CHARS:
		pRet = _T("IOCTL_SERIAL_GET_CHARS");
		break;
	case IOCTL_SERIAL_SET_CHARS:
		pRet = _T("IOCTL_SERIAL_SET_CHARS");
		break;
	case IOCTL_SERIAL_GET_HANDFLOW:
		pRet = _T("IOCTL_SERIAL_GET_HANDFLOW");
		break;
	case IOCTL_SERIAL_SET_HANDFLOW:
		pRet = _T("IOCTL_SERIAL_SET_HANDFLOW");
		break;
	case IOCTL_SERIAL_GET_MODEMSTATUS:
		pRet = _T("IOCTL_SERIAL_GET_MODEMSTATUS");
		break;
	case IOCTL_SERIAL_GET_COMMSTATUS:
		pRet = _T("IOCTL_SERIAL_GET_COMMSTATUS");
		break;
	case IOCTL_SERIAL_XOFF_COUNTER:
		pRet = _T("IOCTL_SERIAL_XOFF_COUNTER");
		break;
	case IOCTL_SERIAL_GET_PROPERTIES:
		pRet = _T("IOCTL_SERIAL_GET_PROPERTIES");
		break;
	case IOCTL_SERIAL_GET_DTRRTS:
		pRet = _T("IOCTL_SERIAL_GET_DTRRTS");
		break;
	case IOCTL_SERIAL_LSRMST_INSERT:
		pRet = _T("IOCTL_SERIAL_LSRMST_INSERT");
		break;
	case IOCTL_SERENUM_EXPOSE_HARDWARE:
		pRet = _T("IOCTL_SERENUM_EXPOSE_HARDWARE");
		break;
	case IOCTL_SERENUM_REMOVE_HARDWARE:
		pRet = _T("IOCTL_SERENUM_REMOVE_HARDWARE");
		break;
	case IOCTL_SERENUM_PORT_DESC:
		pRet = _T("IOCTL_SERENUM_PORT_DESC");
		break;
	case IOCTL_SERENUM_GET_PORT_NAME:
		pRet = _T("IOCTL_SERENUM_GET_PORT_NAME");
		break;
	case IOCTL_SERIAL_CONFIG_SIZE:
		pRet = _T("IOCTL_SERIAL_CONFIG_SIZE");
		break;
	case IOCTL_SERIAL_GET_COMMCONFIG:
		pRet = _T("IOCTL_SERIAL_GET_COMMCONFIG");
		break;
	case IOCTL_SERIAL_SET_COMMCONFIG:
		pRet = _T("IOCTL_SERIAL_SET_COMMCONFIG");
		break;
	case IOCTL_SERIAL_GET_STATS:
		pRet = _T("IOCTL_SERIAL_GET_STATS");
		break;
	case IOCTL_SERIAL_CLEAR_STATS:
		pRet = _T("IOCTL_SERIAL_CLEAR_STATS");
		break;
	case IOCTL_SERIAL_GET_MODEM_CONTROL:
		pRet = _T("IOCTL_SERIAL_GET_MODEM_CONTROL");
		break;
	case IOCTL_SERIAL_SET_MODEM_CONTROL:
		pRet = _T("IOCTL_SERIAL_SET_MODEM_CONTROL");
		break;
	case IOCTL_SERIAL_SET_FIFO_CONTROL:
		pRet = _T("IOCTL_SERIAL_SET_FIFO_CONTROL");
		break;
	default:
		pRet = _T("IOCTL_SERIAL_UNKNOWN");
		break;
	}
	return pRet;
}

PHJ_PARSING_DATA parserHlpCmn_FillDetailsPage(PPARSER_CONTEXT pParset_Context, PST_CF_IRP_URB pSource, SIZE_T	Level)
{
	PSPECIFIC_INFO pSpecificInfo = NULL;
	int newIndex = 0;
	PHJ_PARSING_DATA pData = NULL;
	PHJ_PARSING_DATA pMainData = NULL;
	PHJ_PARSING_VALUE pValue = NULL;
	PHJ_PARSING_FIELD_VALUE pFieldValue = NULL;

	BOOL bCompletion = FALSE;
	int InterfaceCount = 0;
	int count = 0;

	int arraycontentcount = 0;
	int arraycontentcount1 = 0;
	int arraycontentcount2 = 0;

	pData = parserHlpCmn_CreateParsingData(pSource, L"TITLE", sizeof(SPECIFIC_INFO));

	pMainData = pData;

	pValue = parserHlpCmn_CreateAndLinkParsingDataToParsingValue(pData, _T("IRP Parameter"));

	WCHAR * pTempString;
	if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
	{
		bCompletion = TRUE;
		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, L"요청시작/완료", HJ_VALUE_ENDIAN_IGN, L"완료", 0, 0, -1, 0, NULL);
		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		pTempString = translateIRPStatus(pSource->stUrbHeader.stHeader.u32NtStatus);
		parserHlpCmn_FillField(pFieldValue, _T("NtStatus"), HJ_VALUE_ENDIAN_LSB, pTempString, 0, 32, -1, sizeof(U32), &pSource->stUrbHeader.stHeader.u32NtStatus);

		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, _T("Information"), HJ_VALUE_ENDIAN_LSB, NULL, 0, 64, -1, sizeof(U64), &pSource->stUrbHeader.stHeader.u64Information);
	}
	else
	{
		bCompletion = FALSE;
		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, L"요청시작/완료", HJ_VALUE_ENDIAN_IGN, L"시작", 0, 0, -1, 0, NULL);
	}

	pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
	if (pSource->stUrbHeader.stHeader.u32RequestorMode == 0)
	{
		parserHlpCmn_FillField(pFieldValue, _T("RequestorMode"), HJ_VALUE_ENDIAN_LSB, L"KernelMode", 0, 32, -1, sizeof(U32), &pSource->stUrbHeader.stHeader.u32RequestorMode);
	}
	else if (pSource->stUrbHeader.stHeader.u32RequestorMode == 1)
	{
		parserHlpCmn_FillField(pFieldValue, _T("RequestorMode"), HJ_VALUE_ENDIAN_LSB, L"UserMode", 0, 32, -1, sizeof(U32), &pSource->stUrbHeader.stHeader.u32RequestorMode);
	}
	else
	{
		parserHlpCmn_FillField(pFieldValue, _T("RequestorMode"), HJ_VALUE_ENDIAN_LSB, L"Unknown", 0, 32, -1, sizeof(U32), &pSource->stUrbHeader.stHeader.u32RequestorMode);
	}
	pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
	parserHlpCmn_FillField(pFieldValue, _T("Time"), HJ_VALUE_ENDIAN_LSB, NULL, 0, 64, -1, sizeof(U64), &pSource->stUrbHeader.stHeader.u64Time);
	pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
	parserHlpCmn_FillField(pFieldValue, _T("MajorFunction"), HJ_VALUE_ENDIAN_LSB, GetMajorFunction(pSource->stUrbHeader.stHeader.u8MajorFunction), 0, 8, -1, sizeof(U8), &pSource->stUrbHeader.stHeader.u8MajorFunction);

	switch (pSource->stUrbHeader.stHeader.u8MajorFunction)
	{
	case IRP_MJ_DEVICE_CONTROL:
		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, _T("IOControlCode"), HJ_VALUE_ENDIAN_LSB, 
			ReplaceItemForIoctl(pSource->stUrbHeader.stHeader.u32IoControlCode), 
			0, 32, -1, sizeof(U32), &pSource->stUrbHeader.stHeader.u32IoControlCode);

		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, _T("InputBufferLength"), HJ_VALUE_ENDIAN_LSB, NULL, 0, 32, -1, sizeof(U32), &pSource->stUrbHeader.stHeader.u32InputBufferLength);

		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, _T("OutputBufferLength"), HJ_VALUE_ENDIAN_LSB, NULL, 0, 32, -1, sizeof(U32), &pSource->stUrbHeader.stHeader.u32OutputBufferLength);

		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			FillItemForIoctl(pSource->stUrbHeader.stHeader.u32IoControlCode, pData, 
				(U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer, TRUE );
		else
			FillItemForIoctl(pSource->stUrbHeader.stHeader.u32IoControlCode, pData, 
				(U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer, FALSE);

		break;

	case IRP_MJ_READ:
		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, _T("Length"), HJ_VALUE_ENDIAN_LSB, NULL, 0, 32, -1, sizeof(U32), &pSource->stUrbHeader.stHeader.u32Length);

		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, _T("ByteOffset"), HJ_VALUE_ENDIAN_LSB, NULL, 0, 64, -1, sizeof(U64), &pSource->stUrbHeader.stHeader.u64ByteOffset);

		break;

	case IRP_MJ_WRITE:
		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, _T("Length"), HJ_VALUE_ENDIAN_LSB, NULL, 0, 32, -1, sizeof(U32), &pSource->stUrbHeader.stHeader.u32Length);

		pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
		parserHlpCmn_FillField(pFieldValue, _T("ByteOffset"), HJ_VALUE_ENDIAN_LSB, NULL, 0, 64, -1, sizeof(U64), &pSource->stUrbHeader.stHeader.u64ByteOffset);

		break;

	}

	pFieldValue = parserHlpCmn_CreateAndLinkParsingValueToParsingField(pValue);
	parserHlpCmn_FillField(pFieldValue, _T("OrignialIrp"), HJ_VALUE_ENDIAN_LSB, NULL, 0, 64, -1, sizeof(U64), &pSource->stUrbHeader.stHeader.u64OrignialIrp);

	return pData;
}

PHJ_PARSING_DATA_FOR_DATA parserHlpCmn_FillDataPages(PPARSER_CONTEXT pParset_Context, PST_CF_IRP_URB pSource, SIZE_T	Level)
{
	BOOLEAN bCompletion = FALSE;
	PHJ_PARSING_DATA_FOR_DATA pData = NULL;
	PHJ_PARSING_VALUE pValue = NULL;
	PHJ_PARSING_FIELD_VALUE pFieldValue = NULL;

	if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
	{
		bCompletion = TRUE;
	}

	if ((bCompletion == TRUE) && (pSource->stUrbHeader.stHeader.u32NtStatus != 0))
		return pData;

	switch (pSource->stUrbHeader.stHeader.u8MajorFunction)
	{
	case IRP_MJ_DEVICE_CONTROL:
		if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer)
		{
			pData = (PHJ_PARSING_DATA_FOR_DATA)FillDataItemForIoctl(pSource->stUrbHeader.stHeader.u32IoControlCode,
				(U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
				pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, bCompletion);
		}
		break;
	case IRP_MJ_READ:
		if ((bCompletion == TRUE) && (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer))
		{
			pData = (PHJ_PARSING_DATA_FOR_DATA)malloc(sizeof(HJ_PARSING_DATA_FOR_DATA));
			pData->bIsConstant = FALSE;
			pData->Size = (SIZE_T)pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer;
			pData->pDataBuffer = malloc(pData->Size);
			memcpy(pData->pDataBuffer, (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer, pData->Size);
		}
		break;

	case IRP_MJ_WRITE:
		if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer)
		{
			pData = (PHJ_PARSING_DATA_FOR_DATA)malloc(sizeof(HJ_PARSING_DATA_FOR_DATA));
			pData->bIsConstant = FALSE;
			pData->Size = (SIZE_T)pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer;
			pData->pDataBuffer = malloc(pData->Size);
			memcpy(pData->pDataBuffer, (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer, pData->Size);
		}
		break;
	}
	return pData;
}

WCHAR * parserHlpCmn_translateString(const WCHAR * Prefix, unsigned char * pSrc, U64 Bytes, U64 MaxBytes)
{
	WCHAR * pDes = NULL;
	WCHAR wData[3] = { 0, };
	int count = 0;
	WCHAR Upper, Lower;
	SIZE_T strsize = 0;
	SIZE_T allocsize = 0;
	BOOLEAN bOmitted = FALSE;

	allocsize = 4;

	if (Prefix)
	{
		allocsize += (wcslen(Prefix) * sizeof(WCHAR) + 2);
	}

	if (MaxBytes < Bytes)
	{
		Bytes = MaxBytes - 2;
		bOmitted = TRUE;
	}
	allocsize += (SIZE_T)(Bytes * 3 * sizeof(WCHAR));

	pDes = (WCHAR *)malloc(allocsize);
	memset(pDes, 0, allocsize);
	if (Prefix)
	{
		wcscpy(pDes, Prefix);
//		wcscat(pDes, L" ");
		wsprintf(pDes, L"%ws ", pDes);
		strsize = wcslen(pDes);
	}

	for (count = 0; count < (int)Bytes; count++)
	{
		Upper = (pSrc[count] & 0xF0) >> 4;
		Lower = pSrc[count] & 0x0F;
		if (Upper>9)
		{
			Upper = L'A' + (Upper - 10);
		}
		else
		{
			Upper = L'0' + Upper;
		}

		if (Lower>9)
		{
			Lower = L'A' + (Lower - 10);
		}
		else
		{
			Lower = L'0' + Lower;
		}
		pDes[strsize] = Upper;
		strsize++;
		pDes[strsize] = Lower;
		strsize++;
		if ((count + 1) >= (int)Bytes)
			break;
		pDes[strsize] = L' ';
		strsize++;
	}

	if (bOmitted == TRUE)
	{
		wsprintf(pDes, L"%ws..", pDes);
//		wcscat(pDes, L"..");
	}
	return pDes;
}

