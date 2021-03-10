/***************************************************************************\
 *   Copyright (C) 2007 by Huawei Shen                                     *
 *   shenhuawei@software.ict.ac.cn                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
\***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int nVertexNumber;
int **OriginMatrix;

int nSubVertexNumber;
int *VertexList;

int nWordNumber;
char **ppchWordlist;

int BinarySearch(char *pchKeyword)
{
	for (int i = 1; i <= nVertexNumber; i++)
	{
		if (strcmp(pchKeyword, ppchWordlist[i]) == 0)
			return i;
	}

	return -1;

/*
	int nLeft;
	int nRight;
	int nMid;
	int nFlag;

	nLeft = 1;
	nRight = nVertexNumber;
	while (nLeft <= nRight)
	{
		nMid = (nLeft + nRight) / 2;
		nFlag = strcmp(pchKeyword, ppchWordlist[nMid]);
		if (nFlag == 0)
			return nMid;
		else if (nFlag > 0)
			nLeft = nMid + 1;
		else
			nRight = nMid - 1;
	}

	return -1;
	*/
}

void LoadWordlist(char *pchWordlistFileName)
{
	FILE *fpWordlistFile;
	char *pchBuffer;
	long lSize;
	int nCount;
	char *pchPtr;

	if ((fpWordlistFile = fopen(pchWordlistFileName, "rb")) == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

	fseek(fpWordlistFile, 0L, SEEK_END);
	lSize = ftell(fpWordlistFile);
	pchBuffer = (char *)malloc((lSize + 2) * sizeof(char));
	if (pchBuffer == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}
	fseek(fpWordlistFile, 0L, SEEK_SET);
	fread(pchBuffer, lSize, 1, fpWordlistFile);
	fclose(fpWordlistFile);

	if (pchBuffer[lSize - 1] != '\n')
	{
		pchBuffer[lSize] = '\n';
		lSize++;
	}
	pchBuffer[lSize] = '\0';

	nCount = 0;
	pchPtr = pchBuffer;
	while (1)
	{
		pchPtr = strchr(pchPtr, '\n');
		if (pchPtr == NULL)
			break;

		pchPtr++;
		nCount++;
	}
	
	nWordNumber = nCount;

	ppchWordlist = (char **)malloc((nCount + 1) * sizeof(char *));
	if (ppchWordlist == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

	char *pchBegin;
	char *pchEnd;
	int nIndex = 1;
	pchBegin = pchBuffer;
	while (1)
	{
		pchEnd = strchr(pchBegin, '\n');
		if (pchEnd == NULL)
			break;
		*pchEnd = '\0';

		ppchWordlist[nIndex] = (char *)malloc(((int)(pchEnd - pchBegin) + 1) * sizeof(char));
		if (ppchWordlist[nIndex] == NULL)
		{
			printf("Error: %s\n", strerror(errno));
			exit(0);
		}

		strcpy(ppchWordlist[nIndex], pchBegin);

		pchBegin = pchEnd + 1;
		nIndex++;
	}

	free( pchBuffer );
}


void LoadOriginGraph(char *pchOriginGraph)
{
	FILE *fp;

	if ((fp = fopen(pchOriginGraph, "r")) == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

	fscanf(fp, "%d\n", &nVertexNumber);
	OriginMatrix = (int **)malloc((nVertexNumber + 1) * sizeof(int *));
	if (OriginMatrix == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}
	for (int i = 1; i <= nVertexNumber; i++)
	{
		OriginMatrix[i] = (int *)calloc(nVertexNumber + 1, sizeof(int));
		if (OriginMatrix[i] == NULL)
		{
			printf("Error: %s\n", strerror(errno));
			exit(0);
		}
	}

	int nRow;
	int nCol;
	while (fscanf(fp, "%d %d\n", &nRow, &nCol) == 2)
	{
		OriginMatrix[nRow][nCol] = 1;
		OriginMatrix[nCol][nRow] = 1;
	}

	fclose(fp);
}

void LoadSubVertexSet(char *pchSubVertexSet)
{
	FILE *fp;
	char *pchBuffer;
	long lSize;
	int nCount;
	char *pchPtr;

	if ((fp = fopen(pchSubVertexSet, "rb")) == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	pchBuffer = (char *)malloc((lSize + 2) * sizeof(char));
	if (pchBuffer == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}
	fseek(fp, 0L, SEEK_SET);
	fread(pchBuffer, lSize, 1, fp);
	fclose(fp);

	if (pchBuffer[lSize - 1] != '\n')
	{
		pchBuffer[lSize] = '\n';
		lSize++;
	}
	pchBuffer[lSize] = '\0';

	nCount = 0;
	pchPtr = pchBuffer;
	while (1)
	{
		pchPtr = strchr(pchPtr, '\t');
		if (pchPtr == NULL)
			break;

		pchPtr++;
		nCount++;
	}
	
	nSubVertexNumber = nCount;

	if (nWordNumber != nVertexNumber)
	{
		printf("mismatch\n");
		exit(0);
	}

	VertexList = (int *)malloc((nCount + 1) * sizeof(int));
	if (VertexList == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

	char *pchBegin;
	char *pchEnd;
	int nIndex = 1;
	pchBegin = pchBuffer;
	while (1)
	{
		pchEnd = strchr(pchBegin, '\t');
		if (pchEnd == NULL)
			break;
		*pchEnd = '\0';

		VertexList[nIndex] = BinarySearch(pchBegin);
		if (VertexList[nIndex] != -1)
		{
			nIndex++;
		}

		pchBegin = pchEnd + 1;
	}

	free( pchBuffer );
}

void Output(char *pchSubGraph, char *pchSubVertexList)
{
	FILE *fp;

	if ((fp = fopen(pchSubGraph, "w")) == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

	fprintf(fp, "%d\n", nSubVertexNumber);
	for (int i = 1; i <= nSubVertexNumber; i++)
	{
		for (int j = i + 1; j <= nSubVertexNumber; j++)
		{
			if (OriginMatrix[VertexList[i]][VertexList[j]] == 1)	
				fprintf(fp, "%d %d\n", i, j);
		}
	}

	fclose(fp);

	if ((fp = fopen(pchSubVertexList, "w")) == NULL)
	{
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

	for (int i = 1; i <= nSubVertexNumber; i++)
	{
		fprintf(fp, "%s\n", ppchWordlist[VertexList[i]]);
	}

	fclose(fp);
}

void Clear()
{
	for (int i = 1; i <= nWordNumber; i++)
		free(ppchWordlist[i]);
	free(ppchWordlist);

	free(VertexList);

	for (int i = 1 ; i <= nVertexNumber; i++)
		free(OriginMatrix[i]);
	free(OriginMatrix);
}

int main(int argc, char **argv)
{
	if (argc < 6)
	{
		printf("Usage: %s SubVertexSet OriginGraph Subgraph SubVertexList OriginWordlist\n", argv[0]);
		return -1;
	}

	LoadOriginGraph(argv[2]);

	LoadWordlist(argv[5]);

	LoadSubVertexSet(argv[1]);

	Output(argv[3], argv[4]);

	Clear();

	return 0;
}
