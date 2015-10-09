// packingforCpp.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>

byte* OpenFile(std::string pFileName, int* pSize )
{
	HANDLE hFile = CreateFile(pFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int fileSize = GetFileSize(hFile, NULL);
	*pSize = fileSize;
	
	byte* pBuffer = new byte[fileSize];
	memset(pBuffer, 0, fileSize);

	DWORD size;
	ReadFile(hFile, pBuffer, sizeof(char)*fileSize, &size, NULL);

	auto error = GetLastError();

	CloseHandle(hFile);

	return pBuffer;
}

void Pack( const std::string& packName ) 
{
	std::string fileNameA("first.hiy");
	std::string fileNameB("img.bmp");

	std::string path("..\\data\\");

	int fileSizeA = 0;
	byte* pFileA = OpenFile(path + fileNameA, &fileSizeA);

	int fileSizeB = 0;
	byte* pFileB = OpenFile(path + fileNameB, &fileSizeB);

	int totalFileCount = 2;

	// ���� ��ġ��
	{
		HANDLE hPackFile = CreateFile(packName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD size(0);

		// ���� ��ü ����
		WriteFile(hPackFile, &totalFileCount, sizeof(int), &size, NULL);

		auto packingFile = [](HANDLE hHandle, std::string fileName, byte* pFileBuffer, int bufferSize)
		{
			DWORD size(0);

			// ���� �̸� ����
			size_t nameLength = fileName.size();
			WriteFile(hHandle, &nameLength, sizeof(size_t), &size, NULL);
			// ���� �̸�
			WriteFile(hHandle, fileName.c_str(), sizeof(char)*fileName.size(), &size, NULL);

			// ���� ������
			WriteFile(hHandle, &bufferSize, sizeof(int), &size, NULL);

			// ���̳ʸ� ����
			WriteFile(hHandle, pFileBuffer, bufferSize, &size, NULL);
		};

		packingFile(hPackFile, fileNameA, pFileA, fileSizeA);
		packingFile(hPackFile, fileNameB, pFileB, fileSizeB);

		CloseHandle(hPackFile);
	}
}

void Unpack( std::string filename )
{
	HANDLE hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD size;

	// ��ü ���� ����
	int totalFileCount = 0;
	ReadFile(hFile, &totalFileCount, sizeof(int), &size, NULL);

	for (int i = 0; i < totalFileCount; ++i)
	{
		// ���� �̸��� ����
		int nameLength;
		ReadFile(hFile, &nameLength, sizeof(int), &size, NULL);

		// ���� ���� �̸�
		char* pName = new char[nameLength+1];
		memset(pName, 0, nameLength + 1);
		ReadFile(hFile, pName, sizeof(char)*nameLength, &size, NULL);

		std::string fileName(pName);
		
		// ���� ������
		int bufferSize;
		ReadFile(hFile, &bufferSize, sizeof(int), &size, NULL);

		// ���̳ʸ� ������
		byte* pBuffer = new byte[bufferSize];
		memset(pBuffer, 0, bufferSize);
		ReadFile(hFile, pBuffer, sizeof(byte)*bufferSize, &size, NULL);

		// ���� ���� ����
		HANDLE hHandle = CreateFile(fileName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(hHandle, pBuffer, bufferSize, &size, NULL);
		CloseHandle(hHandle);
	}

	CloseHandle(hFile);
}

int main()
{
	Unpack("test.bin");
	//Pack("test.bin");
    return 0;
}

