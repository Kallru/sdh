// sdfsdfsdf.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	static const char LOG_ENCRYPTION_KEY[4] = { 0xdd , 0xab , 0xfd , 0xeb }; //FArchiveFileWriterWindows::Flush() ���� ���� ���߾� �־�� �Ѵ�.

	// ��ȣȭ �� UNICODE_BOM�� ���Ͽ� MCLog���θ� �Ǵ��Ѵ�.
	static const char UNICODE_BOM[2] = { 0xfe, 0xff };
	static const char MARKER[2]	= { LOG_ENCRYPTION_KEY[0]^UNICODE_BOM[1] ,  LOG_ENCRYPTION_KEY[1]^UNICODE_BOM[0] }; // little endian�� ������

	return 0;
}

