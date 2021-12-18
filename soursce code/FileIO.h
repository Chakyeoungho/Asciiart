#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <stdio.h>

char *GetPureFileName(char *strFileName);
void OpenFile(FILE **ppFile, char *strFileName, char *pMode);
void CloseFile(FILE *pFile);

#endif