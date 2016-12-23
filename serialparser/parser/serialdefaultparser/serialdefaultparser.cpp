#include "stdafx.h"
#include "serialdefaultparser.h"

WCHAR * g_ConstFixedColumnHeader[COLUMNCOUNT] = { 
	_T("���ڵ�#"), _T("�ð�"), _T("����/�Ϸ�"), _T("Item"), 
	_T("����"), _T("����"), _T("����"), _T("��ûũ��"), _T("����ũ��") };
SIZE_T g_ConstFixedColumnHeaderSize[COLUMNCOUNT] = { 120, 150, 70, 250, 80, 150, 180, 70, 70 };
HJ_CUSTOM_PARSING_FIXED_HEADER	g_FixedHeader;
HJ_PARSER_INFO g_ParserInfo;
PPARSER_CONTEXT g_pParser_Context_for_Output = NULL;

// ------------------------------------------------------------------------------------------------------------

HJPARSERAPI PVOID fnHjInit(IN PVOID MgrHandle, IN PVOID phlp_function, WCHAR *pRegistryPath, WCHAR *FilePath)
{
	WCHAR TempString[MAX_PATH];
	PPARSER_CONTEXT pParser_Context = NULL;
	PHJ_EXPORT_HLP_FUNCTION_LIST phlp_function_list = (PHJ_EXPORT_HLP_FUNCTION_LIST)phlp_function;
	pParser_Context = (PPARSER_CONTEXT)malloc(sizeof(PARSER_CONTEXT));

	pParser_Context->MgrHandle = MgrHandle;
	memcpy(&pParser_Context->hlp_function_list, phlp_function_list, sizeof(HJ_EXPORT_HLP_FUNCTION_LIST));

	g_pParser_Context_for_Output = pParser_Context;

	parserHlpCmn_OutputStringToView(L"2016�� 10�� 19�� ��������Ʈ���� ���۵� �ļ��Դϴ�\n");
	wsprintf(TempString, L"[Title] %ws", (WCHAR *)g_ParserInfo.COMMON.Title);
	parserHlpCmn_OutputStringToView(TempString);
	wsprintf(TempString, L"[Desc] %ws", (WCHAR *)g_ParserInfo.COMMON.Desc);
	parserHlpCmn_OutputStringToView(TempString);
	wsprintf(TempString, L"[MajorVersion] %d", g_ParserInfo.COMMON.MajorVersion);
	parserHlpCmn_OutputStringToView(TempString);

	return (PVOID)pParser_Context;
}

HJPARSERAPI BOOL fnHjDeinit(PVOID handle)
{
	PPARSER_CONTEXT pParser_Context = NULL;

	pParser_Context = (PPARSER_CONTEXT)handle;

	free(pParser_Context);

	return TRUE;
}

HJPARSERAPI BOOL fnHjFree(PVOID handle, PVOID Buffer, SIZE_T Size)
{
	BOOL bRet = FALSE;

	// ���������� �����ؾ��ϴ� ������ �ٷ�ϴ�

	bRet = parserHlpCmn_free(handle, Buffer, Size);
	if (bRet == FALSE)
		goto exit;

exit:
	return bRet;
}

HJPARSERAPI BOOL fnHjInfo(PHJ_PARSER_INFO pInfo)
{
	U32		u32Size;

	if (pInfo == NULL)
		return FALSE;

	u32Size = (pInfo->COMMON.u32Size > g_ParserInfo.COMMON.u32Size) ? g_ParserInfo.COMMON.u32Size : pInfo->COMMON.u32Size;
	if (u32Size)
		memcpy(pInfo, &g_ParserInfo, u32Size);

	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID lpReserved)
{
	int count;

	switch (fdwReason)
	{
	case DLL_PROCESS_DETACH:
		break;

	case DLL_PROCESS_ATTACH:
		g_ParserInfo.COMMON.u32Size = sizeof(HJ_PARSER_INFO);
		g_ParserInfo.COMMON.Type = SERIAL_PARSER;
		wcscpy_s((WCHAR *)g_ParserInfo.COMMON.Title, 1024, PARSER_TITLE);
		g_ParserInfo.COMMON.MajorVersion = 1;
		g_ParserInfo.COMMON.MinorVersion = 0;
		wcscpy_s((WCHAR *)g_ParserInfo.COMMON.Desc, 1024, PARSER_DESC);
		g_ParserInfo.EXTRA.bIsSupported = 1;
		g_ParserInfo.EXTRA.ParserInterface_MajorVersion = 1;
		g_ParserInfo.EXTRA.ParserInterface_MinorVersion = 0;

		g_ParserInfo.EXTRA.pFixedHeaderInfo = (PHJ_PARSING_FIXED_HEADER)&g_FixedHeader;

		g_FixedHeader.ItemCount = COLUMNCOUNT;
		for (count = 0; count < COLUMNCOUNT; count++)
		{
			g_FixedHeader.ColumnHeader[count].bIsConstant = TRUE;
			g_FixedHeader.ColumnHeader[count].MaxListCtrlSize = g_ConstFixedColumnHeaderSize[count];
			g_FixedHeader.ColumnHeader[count].pszData = g_ConstFixedColumnHeader[count];
		}
		break;
	}
	return TRUE;

}

WCHAR * replaceItemForIoctl(U32 u32IoControlCode)
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

int IsDirectionIoctl_IN(U32 u32IoControlCode)
{
	int Direction = INOUTDIR_CONST;
	switch (u32IoControlCode)
	{
	case IOCTL_SERIAL_SET_BAUD_RATE:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_QUEUE_SIZE:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_LINE_CONTROL:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_BREAK_ON:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_BREAK_OFF:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_IMMEDIATE_CHAR:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_TIMEOUTS:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_TIMEOUTS:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_DTR:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_CLR_DTR:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_RESET_DEVICE:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_RTS:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_CLR_RTS:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_XOFF:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_XON:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_WAIT_MASK:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_WAIT_MASK:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_WAIT_ON_MASK:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_PURGE:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_BAUD_RATE:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_LINE_CONTROL:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_CHARS:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_CHARS:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_HANDFLOW:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_HANDFLOW:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_MODEMSTATUS:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_COMMSTATUS:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_XOFF_COUNTER:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_PROPERTIES:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_DTRRTS:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_LSRMST_INSERT:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERENUM_EXPOSE_HARDWARE:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERENUM_REMOVE_HARDWARE:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERENUM_PORT_DESC:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERENUM_GET_PORT_NAME:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_CONFIG_SIZE:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_COMMCONFIG:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_COMMCONFIG:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_STATS:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_CLEAR_STATS:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_GET_MODEM_CONTROL:
		Direction = INDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_MODEM_CONTROL:
		Direction = OUTDIR_CONST;
		break;
	case IOCTL_SERIAL_SET_FIFO_CONTROL:
		Direction = OUTDIR_CONST;
		break;
	default:
		Direction = INOUTDIR_CONST;
		break;
	}
	return Direction;
}

HJPARSERAPI BOOL fnHjDataParsing(PVOID handle, PVOID pSourceIrpUrb, SIZE_T SourceSize, PVOID *ppBuffer, SIZE_T *pResultSize)
{
	BOOL bRet = FALSE;
	PST_CF_IRP_URB pSource = NULL;
	PPARSER_CONTEXT pParser_Context = NULL;
	SIZE_T FixedDataSize = 0;
	PHJ_PARSING_FIXED_DATA	pBaseFixedData = NULL;
	PHJ_PARSING_FIXED_DATA_ENTRY pParentFixedData = NULL;

	// �ڵ��� �غ��մϴ�
	pParser_Context = (PPARSER_CONTEXT)handle;

	// ���÷��ڵ带 �غ��մϴ�
	pSource = (PST_CF_IRP_URB)pSourceIrpUrb;

	//////////////////////////////////////////////////////////////////////////////////////
	// ���� Fixed Header�� ���� ����Ÿ�� �غ��Ѵ�(���ڿ�����)

	// 1. ���� ȭ�鿡 ��Ÿ�� ���ڵ�(HJ_PARSING_FIXED_DATA)�� ���� �޸𸮸� �Ҵ��մϴ�
	FixedDataSize = sizeof(HJ_PARSING_FIXED_DATA);
	pBaseFixedData = (PHJ_PARSING_FIXED_DATA)malloc(FixedDataSize);
	memset(pBaseFixedData, 0, FixedDataSize);

	// 2. ��ũ�帮��Ʈ�� ���� �ʱ�ȭ
	InitializeListHead(&pBaseFixedData->__AllDataEntryHeader__);

	// 3. �ϳ��ϳ��� ���õ���Ÿ�� �ؼ��ϱ� �����մϴ�.
	/*
	������ ������ ���ƾ� �մϴ�.
	[ù��° ������ Main���ڵ�] -> [�ι�° ������ Main���ڵ�] -> .... �̿� ���� ���ŵǾ�� �մϴ�.

	������ ������ ���ڵ�� ������ �����ϴ�.
	[�ʵ�1�� ���극�ڵ�] -> [�ʵ�2�� ���극�ڵ�] .... -> [������ �ʵ��� ���극�ڵ�] -> �����������ڵ�(����) -> ���������������ڵ�(����)
	�ʵ�1, 2�� ���� ���ϴϿ��� ����� ������ ����ؾ� �մϴ�(1=���ڵ�, 2=�ð�)


	������ Main���ڵ�� ������ ���� �������ǰ� �ʿ��մϴ�.
	pFixedData->pDetailsPage = NULL; // ������ �Ӽ����������ڵ尡 �ִٸ� �̰��� �� �ּҸ� �Է��ؾ� �մϴ�
	pFixedData->pDataPage = NULL; // ������ ���������������ڵ尡 �ִٸ� �̰��� �� �ּҸ� �Է��ؾ� �մϴ�
	pFixedData->ColCount = COLUMNCOUNT; // ���� ȭ�鿡 ����� �÷����� �ǹ��մϴ�.
	pFixedData->bIsLast = FALSE; // ���� Main���ڵ尡 �Ľ��� ������������ �ǹ��մϴ�.
	pFixedData->Level = 0; // ������ �ǹ��մϴ�. �ݵ�� ������� ���ǵǾ����� �մϴ�.
	pFixedData->IndexForViewerImage ���ϴϿ��� ������ ���ڵ��� �̹����������ε����Դϴ�.

	pFixedData->ColumnData[X] �� ������ ä��� ���� ���극�ڵ带 ä��� ���Դϴ�. ���극�ڵ带 ä�ﶧ�� ���������� �����ؾ� �մϴ�
	��� �⺻ ���� 0 �Ǵ� FALSE���� ä���� �ֽ��ϴ�.

	����Ÿ�� 8����Ʈ�̳��� ���,
	pFixedData->ColumnData[X].Data �� ����Ÿ�� ����մϴ�.
	pFixedData->ColumnData[X].bIsString = FALSE (�⺻��)
	pFixedData->ColumnData[X].nRadix ������ ����մϴ�. 10(10����), 16(16����)

	����Ÿ�� 8����Ʈ�� �ʰ��ϴ� ���, �����÷��� ����ϴ� ������ ���ڿ��� �ٷ��, ������ ���������������ڵ��� ���ؼ��� ���� ����Ÿ�� �����մϴ�
	pFixedData->ColumnData[X].pszData �� ���ڿ��� ����մϴ�. ���� ���, (54 44 EE 23 11 ..)�̷����� ���ڿ��� �����ǵ����մϴ�. ���ڿ��� ����Ÿ �Ϻθ� ǥ���Ͻʽÿ�.
	pFixedData->ColumnData[X].bIsString = TRUE

	����Ÿ�� ���ڿ��� ���, �ش��ϴ� ���ڿ��� ���� �޸𸮿����� ��������� ���,
	pFixedData->ColumnData[X].pszData = L"HELLO MR MONKEY";
	pFixedData->ColumnData[X].bIsString = TRUE
	pFixedData->ColumnData[X].bIsConstant = TRUE


	##################### �Ӽ����������ڵ�� ���������������ڵ��ۼ��� #########################
	�ΰ��� ���ڵ�� ������ ������� ����ϴ�. �ٸ�, �Ӽ����������ڵ�� �������� ���̺��� ������ �� ������, ���������������ڵ�� �� �ϳ��� ���ڵ常 ����մϴ�.
	1) parserHlpCmn_CreateParsingData �Լ��� ����ؼ� �Ӽ����������ڵ� �Ǵ� ���������������ڵ� �����͸� ����ϴ�. ����, ������ ä��� ���ؼ� ���� �Լ��� ����մϴ�.
	�ݵ�� �ѹ��� ����մϴ�
	2) parserHlpCmn_CreateAndLinkParsingDataToParsingValue �Լ� : Parsing Value�� ����ϴ�(Parsing Data�� �ڽ� ���)
	������ ���̺��� ����ŭ ����ϰ� �˴ϴ�
	3) parserHlpCmn_CreateAndLinkParsingValueToParsingField �Լ� : Parsing Field�� ����ϴ�(Parsing Value�� �ڽ� ���)
	������ ���̺��� �׸����ŭ ����ϰ� �˴ϴ�

	ParsingData--------- ParsingValue (Table)
	|
	---- ParsingValue (Table) ------- Parsing Field (���̺��� �׸�)
	|
	----- Parsing Field (���̺��� �׸�)
	|
	----- Parsing Field (���̺��� �׸�)

	�Ӽ��������� �������������� ��� �⺻������ ����Ʈ �Ľ̶��̺귯���� �����ϴ� �����Դϴ�.
	����, �����Ϸ��� �ļ��� �� �κ��� ���� �������� �ʰ� ����Ʈ �Ľ̶��̺귯���� ����Ϸ���, ������ ���� �Լ��� ȣ���ϸ� �˴ϴ�.

	PHJ_PARSING_DATA parserHlpCmn_FillDetailsPage(PPARSER_CONTEXT pParset_Context, PST_CF_IRP_URB pSource, SIZE_T	Level); // �Ӽ����������ڵ�
	PHJ_PARSING_DATA_FOR_DATA parserHlpCmn_FillDataPages(PPARSER_CONTEXT pParset_Context, PST_CF_IRP_URB pSource, SIZE_T	Level); // ���������������ڵ�

	�̿� ���� �Լ��� ����ؼ� ���� �޸��ּҸ� pFixedData->pDetailsPage, pFixedData->pDataPage �� ������ �˴ϴ�.

	######################## Parsing Field �� ����ϴ� �ʵ� ����(Type) ########################
	Parsing Field���� ������ ���� ������ ���ǵǾ� �ֽ��ϴ�

	#define HJ_VALUE_ENDIAN_IGN (U32)(0x00000000)
	���ڿ������ �����մϴ�

	#define HJ_VALUE_ENDIAN_LSB (U32)(0x00000001)
	���ڸ� LSB���·� ����Ұ��� �����մϴ�

	#define HJ_VALUE_ENDIAN_MSB (U32)(0x00000002)
	���ڸ� MSB���·� ����Ұ��� �����մϴ�

	#define HJ_VALUE_EXTRA_BUFF (U32)(0x00000004) // Data Byte�� �����ϱ� ���ؼ� ����ϴ� ���
	pFieldValue->nRowFieldBuffer �ʵ带 ����ؼ� ����Ÿ�� ���޵˴ϴ�. ���Ӱ� �Ҵ�Ǵ� �޸𸮿��� �մϴ�

	#define HJ_VALUE_EXTRA_PRIVATEBUFF (U32)(0x00000008) // Private �뵵�� ����ϴ� �޸��Ҵ��� �����ϴ� ���
	pFieldValue->nRowFieldPrivateBuffer �ʵ带 ����ؼ� ���ڿ������� ���޵˴ϴ�. ���Ӱ� �Ҵ�Ǵ� �޸𸮿��� �մϴ�

	#define HJ_STRUCT_TYPE__SPLIT_DESCRIPTOR (U32)(0x00000010)
	����Ǿ� �ֽ��ϴ�. ���� ���������� ������ �ʽ��ϴ�

	#define HJ_STRUCT_TYPE__SPLIT_ARRAY_BEGIN (U32)(0x00000020)
	�迭�� �����մϴ�. ��� �迭�� ���ۿ� ���˴ϴ�. �Ķ���ͷ� ���� �迭�� �׸���� ���޵˴ϴ�. ������ �迭���� �Ź� �������־�� �մϴ�.

	#define HJ_STRUCT_TYPE__SPLIT_ARRAY_PART_BEGIN (U32)(0x00000040)
	�迭�� ù��° ����Ÿ�� ���۵��� �˷��ݴϴ�.

	#define HJ_STRUCT_TYPE__SPLIT_ARRAY_PART_END (U32)(0x00000080)
	�迭�� ������ ����Ÿ�� ��ϵǾ� ������ �˷��ݴϴ�.

	#define HJ_STRUCT_TYPE__SPLIT_ARRAY_END (U32)(0x00000100)
	�迭�� �����ϴ�. ������ �迭���� �Ź� �������־�� �մϴ�.

	#define HJ_STRUCT_TYPE__SPLIT_COMMON (U32)(0x00000200)
	����Ǿ� �ֽ��ϴ�. ���� ���������� ������ �ʽ��ϴ�

	������ ���� ���ø� ���ؼ� �迭�� ����ϴ� ����� �� �� �ֽ��ϴ�

	[����]

	NewArray[0].Data1
	NewArray[0].Data2
	NewArray[0].Data3
	NewArray[1].Data1
	NewArray[1].Data2
	NewArray[1].Data3

	HJ_STRUCT_TYPE__SPLIT_ARRAY_BEGIN (2) : �迭�� �׸���� 2���̹Ƿ�

	HJ_STRUCT_TYPE__SPLIT_ARRAY_PART_BEGIN (0) : �迭�� ù��° ÷�� ����
	..... Data1, 2, 3
	HJ_STRUCT_TYPE__SPLIT_ARRAY_PART_END (0) : �迭�� ù��° ÷�� ��

	HJ_STRUCT_TYPE__SPLIT_ARRAY_PART_BEGIN (1) : �迭�� �ι�° ÷�� ����
	..... Data1, 2, 3
	HJ_STRUCT_TYPE__SPLIT_ARRAY_PART_END (1) : �迭�� �ι�° ÷�� ��

	HJ_STRUCT_TYPE__SPLIT_ARRAY_END : �迭�� ��

	*/
	switch (pSource->stUrbHeader.stHeader.u8MajorFunction)
	{
	case IRP_MJ_CREATE:
	{
		PHJ_PARSING_FIXED_DATA_ENTRY pFixedData = NULL;

		////// level 1 //////
		////// level 1 //////
		////// level 1 //////
		FixedDataSize = sizeof(HJ_PARSING_FIXED_DATA_ENTRY) + (sizeof(HJ_COLUMNDATA)*(COLUMNCOUNT - 1));
		pFixedData = (PHJ_PARSING_FIXED_DATA_ENTRY)malloc(FixedDataSize);
		memset(pFixedData, 0, FixedDataSize);
		InitializeListHead(&pFixedData->__Self__);
		pFixedData->pDetailsPage = NULL;
		pFixedData->pDataPage = NULL;
		pFixedData->ColCount = COLUMNCOUNT;
		pFixedData->bIsLast = FALSE;
		pFixedData->Level = 0;

		InsertTailList(&pBaseFixedData->__AllDataEntryHeader__, &pFixedData->__Self__);
		pBaseFixedData->nCount++;

		// ���ڵ�#[VIEWCOL_RECORD]
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecordOrg;
		}
		else
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecord;
		}
		pFixedData->ColumnData[0].nRadix = 10;

		// �ð�[VIEWCOL_TIME]
		pFixedData->ColumnData[1].Data = pSource->stUrbHeader.stHeader.u64Time;
		pFixedData->ColumnData[1].nRadix = 10;

		// ����/�Ϸ�[2]
		pFixedData->ColumnData[2].bIsString = TRUE;
		pFixedData->ColumnData[2].bIsConstant = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			pFixedData->ColumnData[2].pszData = L"�Ϸ�";
		else
			pFixedData->ColumnData[2].pszData = L"����";

		// Item[3]
		pFixedData->ColumnData[3].bIsString = TRUE;
		pFixedData->ColumnData[3].bIsConstant = TRUE;
		pFixedData->ColumnData[3].pszData = _T("Port Open");

		// ����[4]
		pFixedData->ColumnData[4].bIsString = TRUE;
		pFixedData->ColumnData[4].bIsConstant = TRUE;
		pFixedData->ColumnData[4].pszData = INOUTDIR;
		pFixedData->IndexForViewerImage = VIEWIMAGEINDEX_DEFAULT;

		// ����[5]
		pFixedData->ColumnData[5].bIsString = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[5].bIsConstant = TRUE;
			if (pSource->stUrbHeader.stHeader.u32NtStatus == 0)
				pFixedData->ColumnData[5].pszData = L"����";
			else
				pFixedData->ColumnData[5].pszData = L"����";
		}
		// ����[6]
		pFixedData->ColumnData[6].bIsString = TRUE;

		// ��ûũ��[7]
		pFixedData->ColumnData[7].bIsString = TRUE;

		// ����ũ��[8]
		pFixedData->ColumnData[8].bIsString = TRUE;

		// appendix
		pFixedData->pDetailsPage = parserHlpCmn_FillDetailsPage(pParser_Context, pSource, pFixedData->Level);
		pFixedData->pDataPage = parserHlpCmn_FillDataPages(pParser_Context, pSource, pFixedData->Level);
	}
	break;

	case IRP_MJ_CLEANUP:
	{
		PHJ_PARSING_FIXED_DATA_ENTRY pFixedData = NULL;

		////// level 1 //////
		////// level 1 //////
		////// level 1 //////
		FixedDataSize = sizeof(HJ_PARSING_FIXED_DATA_ENTRY) + (sizeof(HJ_COLUMNDATA)*(COLUMNCOUNT - 1));
		pFixedData = (PHJ_PARSING_FIXED_DATA_ENTRY)malloc(FixedDataSize);
		memset(pFixedData, 0, FixedDataSize);
		InitializeListHead(&pFixedData->__Self__);
		pFixedData->pDetailsPage = NULL;
		pFixedData->pDataPage = NULL;
		pFixedData->ColCount = COLUMNCOUNT;
		pFixedData->bIsLast = FALSE;
		pFixedData->Level = 0;

		InsertTailList(&pBaseFixedData->__AllDataEntryHeader__, &pFixedData->__Self__);
		pBaseFixedData->nCount++;

		// ���ڵ�#[VIEWCOL_RECORD]
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecordOrg;
		}
		else
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecord;
		}
		pFixedData->ColumnData[0].nRadix = 10;

		// �ð�[VIEWCOL_TIME]
		pFixedData->ColumnData[1].Data = pSource->stUrbHeader.stHeader.u64Time;
		pFixedData->ColumnData[1].nRadix = 10;

		// ����/�Ϸ�[2]
		pFixedData->ColumnData[2].bIsString = TRUE;
		pFixedData->ColumnData[2].bIsConstant = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			pFixedData->ColumnData[2].pszData = L"�Ϸ�";
		else
			pFixedData->ColumnData[2].pszData = L"����";

		// Item[3]
		pFixedData->ColumnData[3].bIsString = TRUE;
		pFixedData->ColumnData[3].bIsConstant = TRUE;
		pFixedData->ColumnData[3].pszData = _T("Port Cleanup");

		// ����[4]
		pFixedData->ColumnData[4].bIsString = TRUE;
		pFixedData->ColumnData[4].bIsConstant = TRUE;
		pFixedData->ColumnData[4].pszData = INOUTDIR;
		pFixedData->IndexForViewerImage = VIEWIMAGEINDEX_DEFAULT;

		// ����[5]
		pFixedData->ColumnData[5].bIsString = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[5].bIsConstant = TRUE;
			if (pSource->stUrbHeader.stHeader.u32NtStatus == 0)
				pFixedData->ColumnData[5].pszData = L"����";
			else
				pFixedData->ColumnData[5].pszData = L"����";
		}
		// ����[6]
		pFixedData->ColumnData[6].bIsString = TRUE;

		// ��ûũ��[7]
		pFixedData->ColumnData[7].bIsString = TRUE;

		// ����ũ��[8]
		pFixedData->ColumnData[8].bIsString = TRUE;

		// appendix
		pFixedData->pDetailsPage = parserHlpCmn_FillDetailsPage(pParser_Context, pSource, pFixedData->Level);
		pFixedData->pDataPage = parserHlpCmn_FillDataPages(pParser_Context, pSource, pFixedData->Level);
	}
	break;

	case IRP_MJ_CLOSE:
	{
		PHJ_PARSING_FIXED_DATA_ENTRY pFixedData = NULL;

		////// level 1 //////
		////// level 1 //////
		////// level 1 //////
		FixedDataSize = sizeof(HJ_PARSING_FIXED_DATA_ENTRY) + (sizeof(HJ_COLUMNDATA)*(COLUMNCOUNT - 1));
		pFixedData = (PHJ_PARSING_FIXED_DATA_ENTRY)malloc(FixedDataSize);
		memset(pFixedData, 0, FixedDataSize);
		InitializeListHead(&pFixedData->__Self__);
		pFixedData->pDetailsPage = NULL;
		pFixedData->pDataPage = NULL;
		pFixedData->ColCount = COLUMNCOUNT;
		pFixedData->bIsLast = FALSE;
		pFixedData->Level = 0;

		InsertTailList(&pBaseFixedData->__AllDataEntryHeader__, &pFixedData->__Self__);
		pBaseFixedData->nCount++;

		// ���ڵ�#[VIEWCOL_RECORD]
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecordOrg;
		}
		else
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecord;
		}
		pFixedData->ColumnData[0].nRadix = 10;

		// �ð�[VIEWCOL_TIME]
		pFixedData->ColumnData[1].Data = pSource->stUrbHeader.stHeader.u64Time;
		pFixedData->ColumnData[1].nRadix = 10;

		// ����/�Ϸ�[2]
		pFixedData->ColumnData[2].bIsString = TRUE;
		pFixedData->ColumnData[2].bIsConstant = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			pFixedData->ColumnData[2].pszData = L"�Ϸ�";
		else
			pFixedData->ColumnData[2].pszData = L"����";

		// Item[3]
		pFixedData->ColumnData[3].bIsString = TRUE;
		pFixedData->ColumnData[3].bIsConstant = TRUE;
		pFixedData->ColumnData[3].pszData = _T("Port Close");

		// ����[4]
		pFixedData->ColumnData[4].bIsString = TRUE;
		pFixedData->ColumnData[4].bIsConstant = TRUE;
		pFixedData->ColumnData[4].pszData = INOUTDIR;
		pFixedData->IndexForViewerImage = VIEWIMAGEINDEX_DEFAULT;

		// ����[5]
		pFixedData->ColumnData[5].bIsString = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[5].bIsConstant = TRUE;
			if (pSource->stUrbHeader.stHeader.u32NtStatus == 0)
				pFixedData->ColumnData[5].pszData = L"����";
			else
				pFixedData->ColumnData[5].pszData = L"����";
		}
		// ����[6]
		pFixedData->ColumnData[6].bIsString = TRUE;

		// ��ûũ��[7]
		pFixedData->ColumnData[7].bIsString = TRUE;

		// ����ũ��[8]
		pFixedData->ColumnData[8].bIsString = TRUE;

		// appendix
		pFixedData->pDetailsPage = parserHlpCmn_FillDetailsPage(pParser_Context, pSource, pFixedData->Level);
		pFixedData->pDataPage = parserHlpCmn_FillDataPages(pParser_Context, pSource, pFixedData->Level);
	}
	break;

	case IRP_MJ_READ:
	{
		PHJ_PARSING_FIXED_DATA_ENTRY pFixedData = NULL;

		////// level 1 //////
		////// level 1 //////
		////// level 1 //////
		FixedDataSize = sizeof(HJ_PARSING_FIXED_DATA_ENTRY) + (sizeof(HJ_COLUMNDATA)*(COLUMNCOUNT - 1));
		pFixedData = (PHJ_PARSING_FIXED_DATA_ENTRY)malloc(FixedDataSize);
		memset(pFixedData, 0, FixedDataSize);
		InitializeListHead(&pFixedData->__Self__);
		pFixedData->pDetailsPage = NULL;
		pFixedData->pDataPage = NULL;
		pFixedData->ColCount = COLUMNCOUNT;
		pFixedData->bIsLast = FALSE;
		pFixedData->Level = 0;

		InsertTailList(&pBaseFixedData->__AllDataEntryHeader__, &pFixedData->__Self__);
		pBaseFixedData->nCount++;

		// ���ڵ�#[VIEWCOL_RECORD]
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecordOrg;
		}
		else
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecord;
		}
		pFixedData->ColumnData[0].nRadix = 10;

		// �ð�[VIEWCOL_TIME]
		pFixedData->ColumnData[1].Data = pSource->stUrbHeader.stHeader.u64Time;
		pFixedData->ColumnData[1].nRadix = 10;

		// ����/�Ϸ�[2]
		pFixedData->ColumnData[2].bIsString = TRUE;
		pFixedData->ColumnData[2].bIsConstant = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			pFixedData->ColumnData[2].pszData = L"�Ϸ�";
		else
			pFixedData->ColumnData[2].pszData = L"����";

		// Item[3]
		pFixedData->ColumnData[3].bIsString = TRUE;
		pFixedData->ColumnData[3].bIsConstant = TRUE;
		pFixedData->ColumnData[3].pszData = _T("Port Read");

		// ����[4]
		pFixedData->ColumnData[4].bIsString = TRUE;
		pFixedData->ColumnData[4].bIsConstant = TRUE;
		pFixedData->ColumnData[4].pszData = INDIR;
		pFixedData->IndexForViewerImage = VIEWIMAGEINDEX_IN;

		// ����[5]
		pFixedData->ColumnData[5].bIsString = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[5].bIsConstant = TRUE;
			if (pSource->stUrbHeader.stHeader.u32NtStatus == 0)
				pFixedData->ColumnData[5].pszData = L"����";
			else
				pFixedData->ColumnData[5].pszData = L"����";
		}
		// ����[6]
		pFixedData->ColumnData[6].bIsString = TRUE;
		if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer)
		{
			if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			{
				if (pSource->stUrbHeader.stHeader.u32NtStatus == 0)
				{
					if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer > 8)
						pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
							pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, 8);
					else
						pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
							pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer);
				}
				else
				{
					// No
				}
			}
		}

		// ��ûũ��[7]
		pFixedData->ColumnData[7].nRadix = 10;
		pFixedData->ColumnData[7].Data = pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer;

		// ����ũ��[8]
		pFixedData->ColumnData[8].nRadix = 10;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			pFixedData->ColumnData[8].Data = pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer;
		else
			pFixedData->ColumnData[8].bIsString = TRUE;

		// appendix
		pFixedData->pDetailsPage = parserHlpCmn_FillDetailsPage(pParser_Context, pSource, pFixedData->Level);
		pFixedData->pDataPage = parserHlpCmn_FillDataPages(pParser_Context, pSource, pFixedData->Level);
	}
	break;

	case IRP_MJ_WRITE:
	{
		PHJ_PARSING_FIXED_DATA_ENTRY pFixedData = NULL;

		////// level 1 //////
		////// level 1 //////
		////// level 1 //////
		FixedDataSize = sizeof(HJ_PARSING_FIXED_DATA_ENTRY) + (sizeof(HJ_COLUMNDATA)*(COLUMNCOUNT - 1));
		pFixedData = (PHJ_PARSING_FIXED_DATA_ENTRY)malloc(FixedDataSize);
		memset(pFixedData, 0, FixedDataSize);
		InitializeListHead(&pFixedData->__Self__);
		pFixedData->pDetailsPage = NULL;
		pFixedData->pDataPage = NULL;
		pFixedData->ColCount = COLUMNCOUNT;
		pFixedData->bIsLast = FALSE;
		pFixedData->Level = 0;

		InsertTailList(&pBaseFixedData->__AllDataEntryHeader__, &pFixedData->__Self__);
		pBaseFixedData->nCount++;

		// ���ڵ�#[VIEWCOL_RECORD]
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecordOrg;
		}
		else
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecord;
		}
		pFixedData->ColumnData[0].nRadix = 10;

		// �ð�[VIEWCOL_TIME]
		pFixedData->ColumnData[1].Data = pSource->stUrbHeader.stHeader.u64Time;
		pFixedData->ColumnData[1].nRadix = 10;

		// ����/�Ϸ�[2]
		pFixedData->ColumnData[2].bIsString = TRUE;
		pFixedData->ColumnData[2].bIsConstant = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			pFixedData->ColumnData[2].pszData = L"�Ϸ�";
		else
			pFixedData->ColumnData[2].pszData = L"����";

		// Item[3]
		pFixedData->ColumnData[3].bIsString = TRUE;
		pFixedData->ColumnData[3].bIsConstant = TRUE;
		pFixedData->ColumnData[3].pszData = _T("Port Write");

		// ����[4]
		pFixedData->ColumnData[4].bIsString = TRUE;
		pFixedData->ColumnData[4].bIsConstant = TRUE;
		pFixedData->ColumnData[4].pszData = OUTDIR;
		pFixedData->IndexForViewerImage = VIEWIMAGEINDEX_OUT;

		// ����[5]
		pFixedData->ColumnData[5].bIsString = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[5].bIsConstant = TRUE;
			if (pSource->stUrbHeader.stHeader.u32NtStatus == 0)
				pFixedData->ColumnData[5].pszData = L"����";
			else
				pFixedData->ColumnData[5].pszData = L"����";
		}
		// ����[6]
		pFixedData->ColumnData[6].bIsString = TRUE;
		if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer)
		{
			if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			{
				if (pSource->stUrbHeader.stHeader.u32NtStatus == 0)
				{
					if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer > 8)
						pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
							pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, 8);
					else
						pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
							pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer);
				}
			}
			else
			{
				if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer > 8)
					pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
						pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, 8);
				else
					pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
						pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer);
			}
		}

		// ��ûũ��[7]
		pFixedData->ColumnData[7].nRadix = 10;
		pFixedData->ColumnData[7].Data = pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer;

		// ����ũ��[8]
		pFixedData->ColumnData[8].nRadix = 10;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			pFixedData->ColumnData[8].Data = pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer;
		else
			pFixedData->ColumnData[8].bIsString = TRUE;

		// appendix
		pFixedData->pDetailsPage = parserHlpCmn_FillDetailsPage(pParser_Context, pSource, pFixedData->Level);
		pFixedData->pDataPage = parserHlpCmn_FillDataPages(pParser_Context, pSource, pFixedData->Level);
	}
	break;

	case IRP_MJ_DEVICE_CONTROL:
	{
		PHJ_PARSING_FIXED_DATA_ENTRY pFixedData = NULL;

		////// level 1 //////
		////// level 1 //////
		////// level 1 //////
		FixedDataSize = sizeof(HJ_PARSING_FIXED_DATA_ENTRY) + (sizeof(HJ_COLUMNDATA)*(COLUMNCOUNT - 1));
		pFixedData = (PHJ_PARSING_FIXED_DATA_ENTRY)malloc(FixedDataSize);
		memset(pFixedData, 0, FixedDataSize);
		InitializeListHead(&pFixedData->__Self__);
		pFixedData->pDetailsPage = NULL;
		pFixedData->pDataPage = NULL;
		pFixedData->ColCount = COLUMNCOUNT;
		pFixedData->bIsLast = FALSE;
		pFixedData->Level = 0;

		InsertTailList(&pBaseFixedData->__AllDataEntryHeader__, &pFixedData->__Self__);
		pBaseFixedData->nCount++;

		// ���ڵ�#[VIEWCOL_RECORD]
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecordOrg;
		}
		else
		{
			pFixedData->ColumnData[0].Data = pSource->stUrbHeader.stHeader.u64NumRecord;
		}
		pFixedData->ColumnData[0].nRadix = 10;

		// �ð�[VIEWCOL_TIME]
		pFixedData->ColumnData[1].Data = pSource->stUrbHeader.stHeader.u64Time;
		pFixedData->ColumnData[1].nRadix = 10;

		// ����/�Ϸ�[2]
		pFixedData->ColumnData[2].bIsString = TRUE;
		pFixedData->ColumnData[2].bIsConstant = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			pFixedData->ColumnData[2].pszData = L"�Ϸ�";
		else
			pFixedData->ColumnData[2].pszData = L"����";

		// Item[3]
		pFixedData->ColumnData[3].bIsString = TRUE;
		pFixedData->ColumnData[3].bIsConstant = TRUE;
		pFixedData->ColumnData[3].pszData = replaceItemForIoctl(pSource->stUrbHeader.stHeader.u32IoControlCode);

		// ����[4]
		int Direction;
		Direction = IsDirectionIoctl_IN(pSource->stUrbHeader.stHeader.u32IoControlCode);
		pFixedData->ColumnData[4].bIsString = TRUE;
		pFixedData->ColumnData[4].bIsConstant = TRUE;

		switch(Direction)
		{
		case INDIR_CONST:
			pFixedData->ColumnData[4].pszData = INDIR;
			pFixedData->IndexForViewerImage = VIEWIMAGEINDEX_IN;
			break;
		case OUTDIR_CONST:
			pFixedData->ColumnData[4].pszData = OUTDIR;
			pFixedData->IndexForViewerImage = VIEWIMAGEINDEX_OUT;
			break;
		case INOUTDIR_CONST:
			pFixedData->ColumnData[4].pszData = INOUTDIR;
			pFixedData->IndexForViewerImage = VIEWIMAGEINDEX_INOUT;
			break;
		}

		// ����[5]
		pFixedData->ColumnData[5].bIsString = TRUE;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
		{
			pFixedData->ColumnData[5].bIsConstant = TRUE;
			if (pSource->stUrbHeader.stHeader.u32NtStatus == 0)
				pFixedData->ColumnData[5].pszData = L"����";
			else
				pFixedData->ColumnData[5].pszData = L"����";
		}
		// ����[6]
		pFixedData->ColumnData[6].bIsString = TRUE;
		if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer)
		{
			if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			{
				if (pSource->stUrbHeader.stHeader.u32NtStatus == 0)
				{
					if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer > 8)
						pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
							pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, 8);
					else
						pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
							pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer);
				}
			}
			else
			{
				if (pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer > 8)
					pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
						pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, 8);
				else
					pFixedData->ColumnData[6].pszData = parserHlpCmn_translateString(L"Data:", (U8*)pSource + pSource->stUrbHeader.stTransferBuffer.u64OffsetBuffer,
						pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer, pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer);
			}
		}

		// ��ûũ��[7]
		pFixedData->ColumnData[7].nRadix = 10;
		pFixedData->ColumnData[7].Data = pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer;

		// ����ũ��[8]
		pFixedData->ColumnData[8].nRadix = 10;
		if (pSource->stUrbHeader.stHeader.u8bCompletion == 1)
			pFixedData->ColumnData[8].Data = pSource->stUrbHeader.stTransferBuffer.u64LengthBuffer;
		else
			pFixedData->ColumnData[8].bIsString = TRUE;

		// appendix
		pFixedData->pDetailsPage = parserHlpCmn_FillDetailsPage(pParser_Context, pSource, pFixedData->Level);
		pFixedData->pDataPage = parserHlpCmn_FillDataPages(pParser_Context, pSource, pFixedData->Level);
	}
	break;

	default:
		break;
	}

	*ppBuffer = (void *)pBaseFixedData;
	*pResultSize = sizeof(PHJ_PARSING_FIXED_DATA);

	bRet = TRUE;
	return bRet;
}
