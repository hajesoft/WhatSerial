#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <tchar.h>
//#include <ntddser.h>

#include ".\\..\\..\\common\\common_header\\serial.h"
#include ".\\..\\..\\common\\common_header\\parserdefinition.h"


#define INOUTDIR	(L"IN/OUT")
#define INDIR		(L"IN")
#define OUTDIR		(L"OUT")

#define INOUTDIR_CONST	(0)
#define INDIR_CONST		(1)
#define OUTDIR_CONST	(2)

#define COLUMNCOUNT 9 // 최소한 2개 이상의 컬럼을 가져야 합니다. 컬럼 1, 2 는 순서대로 레코드번호와 시간정보입니다

typedef struct _HJ_CUSTOM_PARSING_FIXED_HEADER
{
	SIZE_T					ItemCount;
	HJ_COLUMN				ColumnHeader[COLUMNCOUNT];
} HJ_CUSTOM_PARSING_FIXED_HEADER, *PHJ_CUSTOM_PARSING_FIXED_HEADER;


