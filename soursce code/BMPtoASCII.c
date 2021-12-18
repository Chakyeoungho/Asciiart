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

	// ��Ʈ�� ���� ��� �б�
	// fread()�� �����ϸ� �� ��° ���ں��� ���� ���� ��ȯ
	if (fread(pBitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, *ppBitmap) < 1)
	{
		perror("��Ʈ�� ���� ��� �б� ����!");
		CloseFile(*ppBitmap); // ���� ��Ʈ���� ���������Ƿ� �ݾ���
		exit(EXIT_FAILURE);
	}

	// ���� �ѹ��� MB���� Ȯ��, ��Ʋ ����ȿ����� �Ųٷ� ����ǹǷ� MB�� Ȯ��
	if (pBitmapFileHeader->bfType != 'MB') // ���ڴ� �ּҷ� �ν� ����
	{
		perror("��Ʈ�� ������ �ƴմϴ�!");
		CloseFile(*ppBitmap);
		exit(EXIT_FAILURE);
	}

	// ��Ʈ�� ���� ��� �б�
	if (fread(pBitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, *ppBitmap) < 1)
	{
		perror("��Ʈ�� ���� ��� �б� ����!");
		CloseFile(*ppBitmap);
		exit(EXIT_FAILURE);
	}

	// 24��Ʈ ��Ʈ���� �ƴϸ� ���α׷� ����
	if (pBitmapInfoHeader->biBitCount != 24)
	{
		perror("24��Ʈ ��Ʈ���� �ƴմϴ�!");
		CloseFile(*ppBitmap);
		exit(EXIT_FAILURE);
	}
}

void InitBitmap(FILE *pBitmap, PBITMAPFILEHEADER pBitmapFileHeader, PBITMAPINFOHEADER pBitmapInfoHeader, BYTE **ppPixel, int iPadding)
{
	int iSize;    // �ȼ� ���� ũ��
	int iWidth;   // ��Ʈ�� ����
	int iHeight;  // ��Ʈ�� ����

	iSize = pBitmapInfoHeader->biSizeImage;
	iWidth = pBitmapInfoHeader->biWidth;
	iHeight = pBitmapInfoHeader->biHeight;

	// iPadding�� 0, 1, 2, 3�� ����
	// �е� ����Ʈ�� 4 - ((���� * �ȼ� ũ��) % 4)
	iPadding = PIXELALIGN - ((iWidth * PIXELSIZE) % PIXELALIGN);
	if (iPadding == 4)
	{
		iPadding = 0;
	}

	// ((���� * �ȼ� ũ��) + �е� ����Ʈ) * ����
	if (iSize == 0) // �ȼ� ���� ũ�Ⱑ 0�̶��
	{
		iSize = ((iWidth * PIXELSIZE) + iPadding) * iHeight;
	}

	// iSize���� PIXELSIZE�� �����������Ƿ� BYTE�� �ٷ� �� ����
	*ppPixel = (BYTE *)malloc(sizeof(BYTE) * iSize);

	// ó������ ���� ������ �̵�
	fseek(pBitmap, pBitmapFileHeader->bfOffBits, SEEK_SET);

	if (fread(*ppPixel, iSize, 1, pBitmap) < 1)
	{
		perror("��Ʈ�� �ȼ� ���� �о���� ����!");
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
	int iPixelIndex; // �ȼ� ���� ��ġ�� ���� ����
	int iWidth;   // ��Ʈ�� ����
	int iHeight;  // ��Ʈ�� ����
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

	// �ȼ� ������ ���ϰ� �������� ����ǹǷ� �� ����� ó��
	for (int iRow = iHeight - 1; iRow >= 0; --iRow)
	{
		for (int iCol = 0; iCol < iWidth; ++iCol)
		{
			// ��� ���� ���� ���� ���� ���� �ٶ���� ��
			// 3�� 3���� �迭�� 1���� �迭�� �ε����� �ٲٷ���
			// (�� * ���� ũ�� + ��) �̷��� ����ؾ� ��
			// ��Ʈ���� ���⿡ �ȼ��� �е� ����Ʈ�� �߰��� ����
			iPixelIndex = (iCol * PIXELSIZE) + ((iRow * PIXELSIZE) * iWidth) + (iRow * iPadding);

			// �迭�� ���ӵ� �޸� �����̹Ƿ� ���� �ּҸ� �˸�
			// �� ������ ������ ������ �� ����
			pRGBTriple = (RGBTRIPLE *)&pPixel[iPixelIndex];

			// Red * 0.3 + Green * 0.59 + Blue * 0.11 => ���
			GrayScale = (pRGBTriple->rgbtRed * 0.3) + (pRGBTriple->rgbtGreen * 0.59) + (pRGBTriple->rgbtBlue * 0.11);

			// 0 / 256 ~ 255 / 256�� 0 ~ 0.99������ ��
			// ���⿡ �迭�� ���̸� ���ϸ� ������ �ε������� ���� ���� ����
			// 0 * 39 ~ 0.99 * 39 => 0 ~ 38.61 => 0 ~ 38
			cASCII = ASCIITable[GrayScale * sizeof ASCIITable / 256];

			// ���� ����
			fprintf(pASCIIArt, "%c%c%c", cASCII, cASCII, cASCII);
		}
		fprintf(pASCIIArt, "\n");
	}
	CloseFile(pASCIIArt);
}