#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

typedef unsigned char      BYTE;
typedef unsigned short int WORD;
typedef signed   long  int LONG;
typedef unsigned long  int DWORD;

#pragma pack(push, 1)

// ��Ʈ�� ���� ���
typedef struct _BitmapFileHeader
{
	WORD  bfType;      // ���� �ѹ�
	DWORD bfSize;      // ���� ũ��
	WORD  bfReserved1; // ����� ����1
	WORD  bfReserved2; // ����� ����2
	DWORD bfOffBits;   // �ȼ� ���� ���� ��ġ
}BITMAPFILEHEADER, *PBITMAPFILEHEADER;

// ��Ʈ�� ���� ���(DIB ���)
typedef struct _BitmapInfoHeader
{
	DWORD biSize;          // ��Ʈ�� ���� ��� ũ��
	LONG  biWidth;         // ��Ʈ�� ���� ũ��
	LONG  biHeight;        // ��Ʈ�� ���� ũ��
	WORD  biPlanes;        // ����ϴ� ������ ��, 1
	WORD  biBitCount;      // �ȼ� �ϳ��� ǥ���ϴ� ��Ʈ ��
	DWORD biCompression;   // ���� ���, ���� ����(BI_RGB)
	DWORD biSizeImage;     // �ȼ� ���� ũ��
	LONG  biXPelsPerMeter; // ���ʹ� ���� �ȼ� ��
	LONG  biYPelsPerMeter; // ���ʹ� ���� �ȼ� ��
	DWORD biClrUsed;       // ���� ���̺��� ������ ���Ǵ� ���� ��
	DWORD biClrImportant;  // ��Ʈ���� ����ϱ� ���� ���� �ε��� ��
}BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct _RGBTriple
{
	// B, G, R ������ ����Ǿ�����
	BYTE rgbtBlue;  // �Ķ��� ����
	BYTE rgbtGreen; // �ʷϻ� ����
	BYTE rgbtRed;   // ������ ����
}RGBTRIPLE, *PRGBTRIPLE;

#pragma pack(pop)

#endif