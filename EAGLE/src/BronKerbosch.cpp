/***************************************************************************\
 *   Copyright (C) 2007 by Huawei Shen                                     *
 *   shenhuawei@software.ict.ac.cn                                         *
 *   last revision: 2009-06-17                                               *
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

#include "BronKerbosch.h"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

CBronKerbosch::CBronKerbosch()
{
}

CBronKerbosch::~CBronKerbosch()
{
}

const vector< std::vector<int> > & CBronKerbosch::getMaximalCliqueList()
{
	return m_maximalCliqueList;
}

int output( const vector< vector<int> > & maximalCliqueList, const char *maximalCliqueFileName )
{
	ofstream maximalCliqueFile;

	maximalCliqueFile.open( maximalCliqueFileName );
	if ( maximalCliqueFile.fail() )
	{
		cout << "open file " << maximalCliqueFileName << " failed.\n";
		return -1;
	}

//	maximalCliqueFile << maximalCliqueList.size() << endl;
	for ( unsigned int maximalCliqueIndex = 0; maximalCliqueIndex < maximalCliqueList.size(); ++maximalCliqueIndex )
	{
		maximalCliqueFile << maximalCliqueList[maximalCliqueIndex].size() << "\t";
		for ( unsigned int nodeIndex = 0; nodeIndex < maximalCliqueList[maximalCliqueIndex].size(); ++nodeIndex )
		{
			maximalCliqueFile << maximalCliqueList[maximalCliqueIndex][nodeIndex] << " ";
		}
		maximalCliqueFile << "\n";
	}

/*
	for ( unsigned int ii = 1; ii < m_status.size(); ++ii )
	{
		if ( m_status[ii] == 0 )
		{
			maximalCliquefile << "1\t";
			maximalCliquefile << ii << " \n";
		}
	}
	*/

	maximalCliqueFile.close();

	return 0;
}

void CBronKerbosch::extend( vector< map<int, int> > & adjacencyMatrix, vector<int> & old, int ne, const int ce )
{
	int nod; // number of disconnected
	int fixp = 0;
	int new_ne;
	int new_ce;
	int i;
	int j;
	int count;
	int pos = 0; // point of selection
	int p;
	int s = 0;
	int sel;
	int min_nod; // minimum number of disconnected
	vector<int> fresh;
	map<int,int>::iterator iter;
	int vertexNumber;

	vertexNumber = adjacencyMatrix.size() - 1;
	fresh.resize( vertexNumber + 1 );

	min_nod = ce;
	nod = 0;
	i = 1;
	while ( i <= ce && min_nod != 0 )
	{
		p = old[i];
		count = 0;

		j = ne + 1;
		while ( j <= ce && count < min_nod )
		{
			iter = adjacencyMatrix[p].find( old[j] );
			if ( iter == adjacencyMatrix[p].end() )
			{
				++count;
				pos = j;
			}
			++j;
		}

		if ( count < min_nod )
		{
			fixp = p;
			min_nod = count;
			if ( i <= ne )
				s = pos;
			else
            {
				s = i;
				nod = 1;
			}
		}
		++i;
	}// while i

	for ( nod = min_nod + nod; nod >= 1; --nod )
	{
		p = old[s];
		old[s] = old[ne + 1];
		old[ne + 1] = p;

		sel = p;

		new_ne = 0;
		i = 1;
		while ( i <= ne )
		{
			iter = adjacencyMatrix[sel].find( old[i] );
			if ( iter != adjacencyMatrix[sel].end() )
			{
				++new_ne;
				fresh[new_ne] = old[i];
			}
            ++i;
        }

		new_ce = new_ne;
		i = ne + 1;
		while ( i <= ce )
		{
			iter = adjacencyMatrix[sel].find( old[i] );
			if ( iter != adjacencyMatrix[sel].end() && sel != old[i] )
			{
				++new_ce;
				fresh[new_ce] = old[i];
			}
            ++i;
		}

		++m_c;
		m_compsub[m_c] = sel;
		if ( 0 == new_ce )
		{
			int loc;

			if ( m_c >= m_minSize )
			{
				for ( loc = 1; loc <= m_c; ++loc )
				{
					m_outBuffer[loc] = m_compsub[loc];
				}
				sort( m_outBuffer.begin() + 1, m_outBuffer.begin() + m_c + 1 );

				vector<int> maximalClique;
				for ( loc = 1; loc <= m_c; ++loc )
				{
					maximalClique.push_back( m_outBuffer[loc] );
					m_status[ m_outBuffer[loc] ] = 1;
				}

				m_maximalCliqueList.push_back( maximalClique );
			}
		}
		else
		{
			if ( new_ne < new_ce )
				extend( adjacencyMatrix, fresh, new_ne, new_ce );
		}

		--m_c;
		++ne;
		if ( nod > 1 )
		{
			s = ne;
LOOK:		s++;
			iter = adjacencyMatrix[fixp].find( old[s] );
			if ( iter != adjacencyMatrix[fixp].end() )
				goto LOOK;
		}
	}
}

void CBronKerbosch::findMaximalClique( vector< map<int, int> > & adjacencyMatrix, const int size )
{
	int vertexNumber = adjacencyMatrix.size() - 1;

	m_compsub.resize( vertexNumber + 1 );
	m_all.resize ( vertexNumber + 1 );
	m_status.resize ( vertexNumber + 1 );
	m_outBuffer.resize ( vertexNumber + 1 );
	m_minSize = size;

	for ( int c = 1; c <= vertexNumber; ++c )
		m_all[c] = c;
	m_c = 0;

	extend( adjacencyMatrix, m_all, 0, vertexNumber );

	for ( unsigned int ii = 1; ii < m_status.size(); ++ii )
	{
		if ( m_status[ii] == 0 )
		{
			vector<int> maximalClique;
			maximalClique.push_back( ii );
			m_status[ii] = 1;
			m_maximalCliqueList.push_back( maximalClique );
		}
	}
}

int loadMatrix( const char *matrixFileName, std::vector< std::map<int, int> > & adjacencyMatrix )
{
	ifstream matrixFile;
	int vertexNumber;
	int row;
	int col;

	matrixFile.open( matrixFileName );
	if ( matrixFile.fail() )
	{
		cout << "open file " << matrixFileName << " failed.\n";
		return -1;
	}

	matrixFile >> vertexNumber;

	adjacencyMatrix.resize( vertexNumber + 1 );

	for ( int nodeIndex = 1; nodeIndex <= vertexNumber; ++nodeIndex )
	{
		adjacencyMatrix[nodeIndex][nodeIndex] = 1;
	}

	int weight;
//	int count = 0;

//	while ( matrixFile >> row >> col )
	while ( matrixFile >> row >> col >> weight )
	{
		adjacencyMatrix[row][col] = weight;
		//adjacencyMatrix[col][row] = 1;
		//++count;
	}

//	cout << count  << endl;

	matrixFile.close();

	return 0;
}

int main( int argc, char **argv )
{
	if ( argc != 3 )
	{
		cout << "Usage: " << argv[0] << " adjacencyMatrixFileName minMaximalCliqueSize\n";
		exit(0);
	}

	CBronKerbosch	iBronKerbosch;
	vector< map<int, int> > adjacencyMatrix;
	int minMaximalCliqueSize;
	int ret;

	ret = loadMatrix( argv[1], adjacencyMatrix );
	if ( ret != 0 )
		exit( ret );

	minMaximalCliqueSize = atoi( argv[2] );

	iBronKerbosch.findMaximalClique( adjacencyMatrix, minMaximalCliqueSize );

	const vector< vector<int> > & maximalCliqueList = iBronKerbosch.getMaximalCliqueList();

	ret = output( maximalCliqueList, "AllClique.res" );
	if ( ret )
		exit( ret );

	return 0;
}
