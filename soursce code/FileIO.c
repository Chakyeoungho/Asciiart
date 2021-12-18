#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "FileIO.h"

char *GetPureFileName(char *strFileName)
{
	char *pToken[10];
	int iIndex = 0;

	// argv�� ���������� ���۸� �̿�
	// �Ϲ� char *�� strtok()�� ����� �� ����
	pToken[iIndex++] = strtok(strFileName, "\\");
	while (pToken[iIndex - 1] != NULL)
	{
		pToken[iIndex++] = strtok(NULL, "\\");
	}
	return pToken[iIndex - 2]; // iIndex�� �����ϹǷ� �ε����� ����
}

void OpenFile(FILE **ppFile, char *strFileName, char *pMode)
{
	*ppFile = fopen(strFileName, pMode);
	if (*ppFile == NULL)
	{
		perror("���� ��Ʈ�� ���� ����!");
		exit(EXIT_FAILURE);
	}
}

void CloseFile(FILE *pFile)
{
	if (fclose(pFile) == EOF)
	{
		perror("���� ��Ʈ�� ���� ����!");
		exit(EXIT_FAILURE);
	}
}