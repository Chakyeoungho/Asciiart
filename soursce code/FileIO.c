#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "FileIO.h"

char *GetPureFileName(char *strFileName)
{
	char *pToken[10];
	int iIndex = 0;

	// argv는 내부적으로 버퍼를 이용
	// 일반 char *는 strtok()을 사용할 수 없음
	pToken[iIndex++] = strtok(strFileName, "\\");
	while (pToken[iIndex - 1] != NULL)
	{
		pToken[iIndex++] = strtok(NULL, "\\");
	}
	return pToken[iIndex - 2]; // iIndex가 증가하므로 인덱스를 빼줌
}

void OpenFile(FILE **ppFile, char *strFileName, char *pMode)
{
	*ppFile = fopen(strFileName, pMode);
	if (*ppFile == NULL)
	{
		perror("파일 스트림 생성 실패!");
		exit(EXIT_FAILURE);
	}
}

void CloseFile(FILE *pFile)
{
	if (fclose(pFile) == EOF)
	{
		perror("파일 스트림 생성 실패!");
		exit(EXIT_FAILURE);
	}
}