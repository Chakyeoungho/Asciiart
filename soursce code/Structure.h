#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

typedef unsigned char      BYTE;
typedef unsigned short int WORD;
typedef signed   long  int LONG;
typedef unsigned long  int DWORD;

#pragma pack(push, 1)

// 비트맵 파일 헤더
typedef struct _BitmapFileHeader
{
	WORD  bfType;      // 매직 넘버
	DWORD bfSize;      // 파일 크기
	WORD  bfReserved1; // 예약된 공간1
	WORD  bfReserved2; // 예약된 공간2
	DWORD bfOffBits;   // 픽셀 정보 시작 위치
}BITMAPFILEHEADER, *PBITMAPFILEHEADER;

// 비트맵 정보 헤더(DIB 헤더)
typedef struct _BitmapInfoHeader
{
	DWORD biSize;          // 비트맵 정보 헤더 크기
	LONG  biWidth;         // 비트맵 가로 크기
	LONG  biHeight;        // 비트맵 세로 크기
	WORD  biPlanes;        // 사용하는 색상판 수, 1
	WORD  biBitCount;      // 픽셀 하나를 표현하는 비트 수
	DWORD biCompression;   // 압축 방식, 압축 안함(BI_RGB)
	DWORD biSizeImage;     // 픽셀 정보 크기
	LONG  biXPelsPerMeter; // 미터당 가로 픽셀 수
	LONG  biYPelsPerMeter; // 미터당 세로 픽셀 수
	DWORD biClrUsed;       // 색상 테이블에서 실제로 사용되는 색상 수
	DWORD biClrImportant;  // 비트맵을 출력하기 위한 색상 인덱스 수
}BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct _RGBTriple
{
	// B, G, R 순으로 저장되어있음
	BYTE rgbtBlue;  // 파란색 강도
	BYTE rgbtGreen; // 초록색 강도
	BYTE rgbtRed;   // 빨간색 강도
}RGBTRIPLE, *PRGBTRIPLE;

#pragma pack(pop)

#endif