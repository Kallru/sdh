// sdfsdfsdf.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	static const char LOG_ENCRYPTION_KEY[4] = { 0xdd , 0xab , 0xfd , 0xeb }; //FArchiveFileWriterWindows::Flush() 안의 값과 맞추어 주어야 한다.

	// 암호화 된 UNICODE_BOM과 비교하여 MCLog여부를 판단한다.
	static const char UNICODE_BOM[2] = { 0xfe, 0xff };
	static const char MARKER[2]	= { LOG_ENCRYPTION_KEY[0]^UNICODE_BOM[1] ,  LOG_ENCRYPTION_KEY[1]^UNICODE_BOM[0] }; // little endian을 맞춰줌

	return 0;
}

