#include <stdio.h>
#include <stdlib.h>
#include "Constant.h"
#include "Macro.h"
#include "Structure.h"
#include "BMPtoASCII.h"
#include "FileIO.h"

int main(int argc, char *argv[])
{
	FILE *pBitmap;   // ����� ��Ʈ�� ���� ������
	BITMAPFILEHEADER BitmapFileHeader; // ��Ʈ�� ���� ���
	BITMAPINFOHEADER BitmapInfoHeader; // ��Ʈ�� ���� ���
	BYTE *pPixel = NULL; // �ȼ� ���� ������
	char *strFileName;
	int iPadding = 0; // ���� ũ�Ⱑ 4����� �������� ���� �� ���� ����Ʈ ��

	strFileName = GetPureFileName(argv[1]);
	LoadBitmap(&pBitmap, strFileName, &BitmapFileHeader, &BitmapInfoHeader);
	InitBitmap(pBitmap, &BitmapFileHeader, &BitmapInfoHeader, &pPixel, iPadding);
	ConvertBitmap(strFileName, &BitmapInfoHeader, pPixel, iPadding);
	SAFEDELETE(pPixel);

	puts("��Ʈ�� ������ ASCII ��Ʈ�� ��ȯ�߽��ϴ�.");
	system("pause");
	return 0;
}