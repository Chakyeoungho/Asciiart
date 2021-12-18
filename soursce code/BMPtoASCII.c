#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Constant.h"
#include "Structure.h"
#include "FileIO.h"

void LoadBitmap(FILE **ppBitmap, char *strFileName, PBITMAPFILEHEADER pBitmapFileHeader, PBITMAPINFOHEADER pBitmapInfoHeader)
{
	OpenFile(ppBitmap, strFileName, "rb");

	// 비트맵 파일 헤더 읽기
	// fread()는 실패하면 세 번째 인자보다 작은 값을 반환
	if (fread(pBitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, *ppBitmap) < 1)
	{
		perror("비트맵 파일 헤더 읽기 실패!");
		CloseFile(*ppBitmap); // 파일 스트림을 생성했으므로 닫아줌
		exit(EXIT_FAILURE);
	}

	// 매직 넘버가 MB인지 확인, 리틀 엔디안에서는 거꾸로 저장되므로 MB로 확인
	if (pBitmapFileHeader->bfType != 'MB') // 문자는 주소로 인식 안함
	{
		perror("비트맵 파일이 아닙니다!");
		CloseFile(*ppBitmap);
		exit(EXIT_FAILURE);
	}

	// 비트맵 정보 헤더 읽기
	if (fread(pBitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, *ppBitmap) < 1)
	{
		perror("비트맵 정보 헤더 읽기 실패!");
		CloseFile(*ppBitmap);
		exit(EXIT_FAILURE);
	}

	// 24비트 비트맵이 아니면 프로그램 종료
	if (pBitmapInfoHeader->biBitCount != 24)
	{
		perror("24비트 비트맵이 아닙니다!");
		CloseFile(*ppBitmap);
		exit(EXIT_FAILURE);
	}
}

void InitBitmap(FILE *pBitmap, PBITMAPFILEHEADER pBitmapFileHeader, PBITMAPINFOHEADER pBitmapInfoHeader, BYTE **ppPixel, int iPadding)
{
	int iSize;    // 픽셀 정보 크기
	int iWidth;   // 비트맵 가로
	int iHeight;  // 비트맵 세로

	iSize = pBitmapInfoHeader->biSizeImage;
	iWidth = pBitmapInfoHeader->biWidth;
	iHeight = pBitmapInfoHeader->biHeight;

	// iPadding은 0, 1, 2, 3만 가능
	// 패딩 바이트는 4 - ((가로 * 픽셀 크기) % 4)
	iPadding = PIXELALIGN - ((iWidth * PIXELSIZE) % PIXELALIGN);
	if (iPadding == 4)
	{
		iPadding = 0;
	}

	// ((가로 * 픽셀 크기) + 패딩 바이트) * 세로
	if (iSize == 0) // 픽셀 정보 크기가 0이라면
	{
		iSize = ((iWidth * PIXELSIZE) + iPadding) * iHeight;
	}

	// iSize에는 PIXELSIZE가 곱해져있으므로 BYTE로 다룰 수 있음
	*ppPixel = (BYTE *)malloc(sizeof(BYTE) * iSize);

	// 처음부터 파일 포인터 이동
	fseek(pBitmap, pBitmapFileHeader->bfOffBits, SEEK_SET);

	if (fread(*ppPixel, iSize, 1, pBitmap) < 1)
	{
		perror("비트맵 픽셀 정보 읽어오기 실패!");
		CloseFile(pBitmap);
		exit(EXIT_FAILURE);
	}
	CloseFile(pBitmap);
}

void ConvertBitmap(char *strFileName, PBITMAPINFOHEADER pBitmapInfoHeader, BYTE *pPixel, int iPadding)
{
	char ASCIITable[] = { 'B', 'M', '8', 'O', 'P', 'q', 'k', 'X', 'S', 'F', '5', '1', '2', 'U', 'u', 'J', 'j', 'Y', 'L', 'v', '7',
						  'r', 'i', ':', ',', '.' , ' ', ' ' , ' ' , ' ' , ' ' , ' ' , ' ' , ' ', ' ', ' ' , ' ' ,' ', ' ' };
	char cASCII;
	char ResultFileName[40];
	int iPixelIndex; // 픽셀 정보 위치를 구할 변수
	int iWidth;   // 비트맵 가로
	int iHeight;  // 비트맵 세로
	int iExtension;
	FILE *pASCIIArt;
	RGBTRIPLE *pRGBTriple;
	BYTE GrayScale;

	iWidth = pBitmapInfoHeader->biWidth;
	iHeight = pBitmapInfoHeader->biHeight;

	strcpy(ResultFileName, "ASCIIART_");
	iExtension = strchr(strFileName, '.') - strFileName;
	strncat(ResultFileName, strFileName, iExtension);
	strcat(ResultFileName, ".txt");
	OpenFile(&pASCIIArt, ResultFileName, "wt");

	// 픽셀 정보는 상하가 뒤집혀서 저장되므로 끝 행부터 처리
	for (int iRow = iHeight - 1; iRow >= 0; --iRow)
	{
		for (int iCol = 0; iCol < iWidth; ++iCol)
		{
			// 행과 열을 더할 때는 행을 열로 바라봐야 함
			// 3행 3열의 배열을 1차원 배열의 인덱스로 바꾸려면
			// (행 * 열의 크기 + 열) 이렇게 계산해야 함
			// 비트맵은 여기에 픽셀과 패딩 바이트가 추가로 붙음
			iPixelIndex = (iCol * PIXELSIZE) + ((iRow * PIXELSIZE) * iWidth) + (iRow * iPadding);

			// 배열은 연속된 메모리 공간이므로 시작 주소를 알면
			// 그 이후의 값들을 참조할 수 있음
			pRGBTriple = (RGBTRIPLE *)&pPixel[iPixelIndex];

			// Red * 0.3 + Green * 0.59 + Blue * 0.11 => 흑백
			GrayScale = (pRGBTriple->rgbtRed * 0.3) + (pRGBTriple->rgbtGreen * 0.59) + (pRGBTriple->rgbtBlue * 0.11);

			// 0 / 256 ~ 255 / 256은 0 ~ 0.99까지의 값
			// 여기에 배열의 길이를 곱하면 마지막 인덱스보다 작은 값이 나옴
			// 0 * 39 ~ 0.99 * 39 => 0 ~ 38.61 => 0 ~ 38
			cASCII = ASCIITable[GrayScale * sizeof ASCIITable / 256];

			// 문자 저장
			fprintf(pASCIIArt, "%c%c%c", cASCII, cASCII, cASCII);
		}
		fprintf(pASCIIArt, "\n");
	}
	CloseFile(pASCIIArt);
}