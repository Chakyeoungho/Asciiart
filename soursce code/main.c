#include <stdio.h>
#include <stdlib.h>
#include "Constant.h"
#include "Macro.h"
#include "Structure.h"
#include "BMPtoASCII.h"
#include "FileIO.h"

int main(int argc, char *argv[])
{
	FILE *pBitmap;   // 사용할 비트맵 파일 포인터
	BITMAPFILEHEADER BitmapFileHeader; // 비트맵 파일 헤더
	BITMAPINFOHEADER BitmapInfoHeader; // 비트맵 정보 헤더
	BYTE *pPixel = NULL; // 픽셀 정보 포인터
	char *strFileName;
	int iPadding = 0; // 가로 크기가 4배수로 떨어지지 않을 때 남는 바이트 수

	strFileName = GetPureFileName(argv[1]);
	LoadBitmap(&pBitmap, strFileName, &BitmapFileHeader, &BitmapInfoHeader);
	InitBitmap(pBitmap, &BitmapFileHeader, &BitmapInfoHeader, &pPixel, iPadding);
	ConvertBitmap(strFileName, &BitmapInfoHeader, pPixel, iPadding);
	SAFEDELETE(pPixel);

	puts("비트맵 파일을 ASCII 아트로 변환했습니다.");
	system("pause");
	return 0;
}