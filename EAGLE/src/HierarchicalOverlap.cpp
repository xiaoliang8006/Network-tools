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

#include <iostream>
#include <fstream>
#include <cmath>

#ifdef __DEBUG__
	#include "fraction.h"
#endif

#include "HierarchicalOverlap.h"

using namespace std;

// default constructor
CHierarchicalOverlap::CHierarchicalOverlap()
{
}

// default deconstructor
CHierarchicalOverlap::~CHierarchicalOverlap()
{
}

// load the adjaceny matrix and the label for each node
int CHierarchicalOverlap::LoadDataFile( const char *labelFileName, const char *adjacencyMatrixFileName, float &totalWeight )
{
	ifstream labelFile;
	char	*buff;
	long	size;
	int		count;
	char	*ptr;

	// open the file for node label
	labelFile.open( labelFileName, ios::binary );
	if ( labelFile.fail() )
	{
		cout << "open file " << labelFileName << " failed.\n";
		return -1;
	}

	// load in memory for once
	labelFile.seekg( 0, ios::end );
	size = labelFile.tellg();
	buff = new char[ size + 2 ];
	labelFile.seekg( 0, ios::beg );
	labelFile.read( buff, size );
	labelFile.close();

	// add the end of one line and the string-end symbol
	if ( buff[size - 1] != '\n' )
	{
		buff[size] = '\n';
		size++;
	}
	buff[size] = '\0';

	// obtain the number of nodes in the network
	count = 0;
	ptr = buff;
	while (1)
	{
		ptr = strchr( ptr, '\n' );
		if ( ptr == NULL )
			break;

		++ptr;
		++count;
	}
	
	vertexNumber = count;
	nodeInfoList.resize( vertexNumber + 1 );

	char *begin;
	char *end;
	int index = 1;
	begin = buff;
	while (1)
	{
		// find the end of the line
		end = strchr( begin, '\n' );
		if ( NULL == end )
			break;
		
		ptr = end + 1;
		if ( *(end - 1) == '\r' )
			end--;
		*end = '\0';

		nodeInfoList[index].label =  begin;

		begin = ptr;
		++index;
	}

	delete []buff;
	
	int col;
	int row;
	ifstream adjacencyMatrixFile;

	adjacencyMatrixFile.open( adjacencyMatrixFileName );
	if ( adjacencyMatrixFile.fail() )
	{
		cout << "open file " << adjacencyMatrixFileName << " failed.\n";
		return -1;
	}

	adjacencyMatrix.resize( vertexNumber + 1 );
	
	adjacencyMatrixFile >> row;
	if ( row != vertexNumber )
	{
		cout << "node number mismatch\n" ;
		cout << "row = " << row << endl;
		cout << "vertexNumber = " << vertexNumber << endl;
		exit( -1 );
	}

	while ( adjacencyMatrixFile >> row >> col )
	{
		AddAdjacencyMatrixElement( row, col );
		AddAdjacencyMatrixElement( col, row );
	}

	adjacencyMatrixFile.close();

	// calculate the degree
	totalDegree = 0;
	for ( row = 1; row <= vertexNumber; ++row )
	{
		totalDegree += nodeInfoList[row].degree;
	}

	totalWeight = totalDegree;

	return 0;
}

/*
 *	Fuction:    
 *				add an edge <row, col> to adjacency matrix
 *	Parameters:
 *				row		the index for the tail node of the edge
 *				col		the index for the head node of the edge
 *	Return:
 *				void
 */
void CHierarchicalOverlap::AddAdjacencyMatrixElement( const int row, const int col )
{
	set<int> &neighborNodeList = adjacencyMatrix[row].neighborNodeList;
	set<int>::iterator set_iter;

	set_iter = neighborNodeList.find( col );
	if ( set_iter != neighborNodeList.end() )
		return;
	else
		neighborNodeList.insert( col );

	++(nodeInfoList[row].degree);
}

/*
 *	Fuction:    
 *				check whether node with index 'row' is connected to node with index 'col' by an edge 
 *	Parameters:
 *				row		the index for the tail node of the edge
 *				col		the index for the head node of the edge
 *	Return:
 *				1		if one edge connect node 'row' and node 'col' 
 *				0		otherwise
 */
int CHierarchicalOverlap::AdjacencyMatrixElement( const int row, const int col )
{
	set<int> &neighborNodeList = adjacencyMatrix[row].neighborNodeList;
	set<int>::iterator set_iter;

	set_iter = neighborNodeList.find( col );
	if ( set_iter != neighborNodeList.end() )
		return 1;
	else
		return 0; 
}

/*
 *	Function:
 *				load all the maximal cliques in the network
 *	Parameters:
 *				maximalCliqueFileName	the name of the file, in which all the maximal cliques are stored
 *	Return:
 *				0	success
 *				-1	failed
 */
int CHierarchicalOverlap::LoadMaximalClique( const char *maximalCliqueFileName )
{
	ifstream maximalCliqueFile;
	char	*buff;
	long	size;
	int		count;
	char	*ptr;

	maximalCliqueFile.open( maximalCliqueFileName, ios::binary );
	if ( maximalCliqueFile.fail() )
	{
		cout << "open file " << maximalCliqueFileName << " failed.\n";
		return -1;
	}
	
	maximalCliqueFile.seekg( 0, ios::end );
	size = maximalCliqueFile.tellg();
	buff = new char[size + 2];
	maximalCliqueFile.seekg( 0, ios::beg );
	maximalCliqueFile.read( buff, size );
	maximalCliqueFile.close();

	if ( buff[size - 1] != '\n' )
	{
		buff[size] = '\n';
		size++;
	}
	buff[size] = '\0';

	// calculte the number of all the maximal cliques
	count = 0;
	ptr = buff;
	while (1)
	{
		ptr = strchr( ptr, '\n' );
		if ( NULL == ptr )
			break;

		++ptr;
		++count;
	}
	maximalCliqueNumber = count;

	communityList.resize( maximalCliqueNumber + 1 );
	
	char *lineBegin;
	char *lineEnd;
	int cliqueSize;
	int cliqueIndex = 1;

	lineBegin = buff;
	while ( (lineEnd = strchr(lineBegin, '\n')) )
	{
		if ( *(lineEnd - 1) == '\r' )
			*(lineEnd - 1) = '\0';
		else
			*lineEnd = '\0';

		sscanf( lineBegin, "%d\t", &cliqueSize );

		communityList[cliqueIndex].status = 0;
		communityList[cliqueIndex].size = cliqueSize;
		communityList[cliqueIndex].nodeList.resize( cliqueSize + 1 );

		ptr = strchr( lineBegin, '\t' );
		++ptr;

		int index = 1;
		int nodeIndex;
		while ( *ptr != '\0' )
		{
			sscanf( ptr, "%d ", &nodeIndex );
			communityList[cliqueIndex].nodeList[index] = nodeIndex;
			++(nodeInfoList[nodeIndex].occurrence);
			
			ptr = strchr( ptr, ' ' );
			++ptr;
			++index;
		}

		++cliqueIndex;

		*lineEnd = '\n';
		lineBegin = lineEnd + 1;
	}

	delete []buff;

	return 0;
}

/*
 *	Function:
 *				calculate the similarity between two communities
 *	Parameters:
 *				firstCommunityIndex		the index of one community
 *				secondCommunityIndex	the index of the other community
 *	Retrun:
 *				the similarity between these two communities
 */
float CHierarchicalOverlap::Similarity( const int firstCommunityIndex, const int secondCommunityIndex )
{
	float sim = 0.0;

	for ( int index1 = 1; index1 <= communityList[firstCommunityIndex].size; ++index1 )
	{
		int nodeIndex1 = communityList[firstCommunityIndex].nodeList[index1];
		for ( int index2 = 1; index2 <= communityList[secondCommunityIndex].size; ++index2 )
		{
			int nodeIndex2 = communityList[secondCommunityIndex].nodeList[index2];

			if ( nodeIndex1 != nodeIndex2 )
				sim += 2 * ( AdjacencyMatrixElement(nodeIndex1, nodeIndex2) - nodeInfoList[nodeIndex1].degree * nodeInfoList[nodeIndex2].degree / totalDegree );
		}
	}

	return sim;
}

/*
 *	Function:
 *				initialize
 *	Parameters:
 *				none
 *	Return:
 *				void
 */
void CHierarchicalOverlap::PreProcess()
{
	int index1;
	int index2;
	int nodeIndex1;
	int nodeIndex2;
	int communityIndex;

	unionSequence.resize( maximalCliqueNumber + 1 );
	EQList.resize( maximalCliqueNumber + 1 );

	numberOfCooccurrence.resize( vertexNumber + 1 );
	/*for ( int ii = 1; ii <= vertexNumber; ++ii )
	{
		numberOfCooccurrence[ii].resize( vertexNumber + 1);
	}
	*/

	map<int, int>::iterator iter;
	EQ = 0.0;
	for ( communityIndex = 1; communityIndex <= maximalCliqueNumber; ++communityIndex )
	{
		for ( index1 = 1; index1 <= communityList[communityIndex].size; index1++ )
		{
			nodeIndex1 = communityList[communityIndex].nodeList[index1];
			nodeInfoList[nodeIndex1].commList.insert( communityIndex );
			for ( index2 = 1; index2 <= communityList[communityIndex].size; ++index2 )
			{
				nodeIndex2 = communityList[communityIndex].nodeList[index2];
				EQ += (AdjacencyMatrixElement(nodeIndex1, nodeIndex2) - nodeInfoList[nodeIndex1].degree * nodeInfoList[nodeIndex2].degree / totalDegree) / nodeInfoList[nodeIndex1].occurrence / nodeInfoList[nodeIndex2].occurrence;

			#ifdef __DEBUG__
				CFraction frac1( AdjacencyMatrixElement(nodeIndex1, nodeIndex2), 1 );
				CFraction frac2( nodeInfoList[nodeIndex1].degree * nodeInfoList[nodeIndex2].degree, (int)totalDegree );
				CFraction frac3( 1, nodeInfoList[nodeIndex1].occurrence * nodeInfoList[nodeIndex2].occurrence );

				frac1 -= frac2;
				frac1 *= frac3;

				fracEQ += frac1;
			#endif

				if ( nodeIndex1 <= nodeIndex2 )
				{
					iter = numberOfCooccurrence[nodeIndex1].find( nodeIndex2 );
					if ( iter != numberOfCooccurrence[nodeIndex1].end() )
					{
						++(iter->second);
					}
					else
					{
						numberOfCooccurrence[nodeIndex1][nodeIndex2] = 1;
					}
				}
			}
		}
	}
	EQList[0] = EQ;
	
	deltaExtendModularity.resize( maximalCliqueNumber + 1 );
	/*
	for ( index1 = 1; index1 <= maximalCliqueNumber; ++index1 )
	{
		deltaExtendModularity[index1].resize( maximalCliqueNumber + 1 );
	}
	*/
	
	float sim;

	for ( index1 = 1; index1 <= maximalCliqueNumber; ++index1 )
	{
		for ( index2 = index1 + 1; index2 <= maximalCliqueNumber; ++index2 )
		{
			sim = Similarity( index1, index2 );
			if ( sim > 0 )
				deltaExtendModularity[index1][index2] = sim;
		}
	}
}

/*
 *	Function:
 *				incorporate one two communities into a new one
 *	Parameters:
 *				firstCommunityIndex		the index of one community to be merged
 *				secondCommunityIndex	the index of the other community to be merged
 *				deltaEQ					the increase of EQ due to the amalgamation
 *	Retrun:
 *				void
 */
void CHierarchicalOverlap::UnionCommunity( const int firstCommunityIndex, const int secondCommunityIndex, float &deltaEQ )
{
	vector<int> tempNodeList;
	vector<int> onlyFirst;
	vector<int> onlySecond;
	vector<int> both;
	int i;
	int j;

	i = 1;
	j = 1;
	tempNodeList.push_back( 0 );
	while ( i <= communityList[firstCommunityIndex].size && j <= communityList[secondCommunityIndex].size )
	{
		if ( communityList[firstCommunityIndex].nodeList[i] < communityList[secondCommunityIndex].nodeList[j] )
		{
			tempNodeList.push_back( communityList[firstCommunityIndex].nodeList[i] );
			onlyFirst.push_back( communityList[firstCommunityIndex].nodeList[i] );
			i++;
		}
		else if ( communityList[firstCommunityIndex].nodeList[i] > communityList[secondCommunityIndex].nodeList[j] )
		{
			tempNodeList.push_back( communityList[secondCommunityIndex].nodeList[j] );
			onlySecond.push_back( communityList[secondCommunityIndex].nodeList[j] );
			j++;
		}
		else
		{
			tempNodeList.push_back( communityList[firstCommunityIndex].nodeList[i] );
			both.push_back( communityList[firstCommunityIndex].nodeList[i] );
			i++;
			j++;
		}
	}
	
	while ( i <= communityList[firstCommunityIndex].size )
	{
		tempNodeList.push_back( communityList[firstCommunityIndex].nodeList[i] );
		onlyFirst.push_back( communityList[firstCommunityIndex].nodeList[i] );
		i++;
	}
	
	while ( j <= communityList[secondCommunityIndex].size )
	{
		tempNodeList.push_back( communityList[secondCommunityIndex].nodeList[j] );
		onlySecond.push_back( communityList[secondCommunityIndex].nodeList[j] );
		j++;
	}

	size_t index1;
	size_t index2;
	int nodeIndex1;
	int nodeIndex2;
	
	deltaEQ = 0.0;

	for ( index1 = 0; index1 < onlyFirst.size(); ++index1 )
	{
		nodeIndex1 = onlyFirst[index1];
		for ( index2 = 0; index2 < onlySecond.size(); ++index2 )
		{
			nodeIndex2 = onlySecond[index2];

			deltaEQ += (AdjacencyMatrixElement(nodeIndex1, nodeIndex2) - nodeInfoList[nodeIndex1].degree * nodeInfoList[nodeIndex2].degree / totalDegree) / nodeInfoList[nodeIndex1].occurrence / nodeInfoList[nodeIndex2].occurrence;
			deltaEQ += (AdjacencyMatrixElement(nodeIndex2, nodeIndex1) - nodeInfoList[nodeIndex2].degree * nodeInfoList[nodeIndex1].degree / totalDegree) / nodeInfoList[nodeIndex2].occurrence / nodeInfoList[nodeIndex1].occurrence;

		#ifdef __DEBUG__
			CFraction frac1( AdjacencyMatrixElement(nodeIndex1, nodeIndex2), 1 );
			CFraction frac2( nodeInfoList[nodeIndex1].degree * nodeInfoList[nodeIndex2].degree, (int)totalDegree );
			CFraction frac3( 1, nodeInfoList[nodeIndex1].occurrence * nodeInfoList[nodeIndex2].occurrence );

			frac1 -= frac2;
			frac1 *= frac3;

			fracEQ += frac1;
			fracEQ += frac1;
		#endif

			map<int, int>::iterator iter;

			if ( nodeIndex1 < nodeIndex2 )
			{
				iter = numberOfCooccurrence[nodeIndex1].find( nodeIndex2 );
				if ( iter != numberOfCooccurrence[nodeIndex1].end() )
				{
					++(iter->second);
				}
				else
				{
					numberOfCooccurrence[nodeIndex1][nodeIndex2] = 1;
				}
			}
			else
			{
				iter = numberOfCooccurrence[nodeIndex2].find( nodeIndex1 );
				if ( iter != numberOfCooccurrence[nodeIndex2].end() )
				{
					++(iter->second);
				}
				else
				{
					numberOfCooccurrence[nodeIndex2][nodeIndex1] = 1;
				}
			}
		}
	}

	float oldValue;
	float newValue;
	int occurCount;
	map<int, int>::iterator map_iter;
	
	for ( index1 = 0; index1 < both.size(); ++index1 )
	{
		nodeIndex1 = both[index1];

		set<int> affectedNodeList;
		set<int>::iterator iter;
		int commIndex;

		for ( iter = nodeInfoList[nodeIndex1].commList.begin(); iter != nodeInfoList[nodeIndex1].commList.end(); ++iter )
		{
			commIndex = *iter;
			for ( int index = 1; index <= communityList[commIndex].size; ++index )
			{
				affectedNodeList.insert( communityList[commIndex].nodeList[index] );
			}
		}

		for ( index2 = 0; index2 < both.size(); ++index2 )
		{
			nodeIndex2 = both[index2];
			iter = affectedNodeList.find( nodeIndex2 );
			if ( iter != affectedNodeList.end() )
			{
				if ( nodeIndex1 < nodeIndex2 )
				{
					map_iter = numberOfCooccurrence[nodeIndex1].find( nodeIndex2 );
					if ( map_iter != numberOfCooccurrence[nodeIndex1].end() )
					{
						occurCount = map_iter->second;
					}
					else
						occurCount = 0;
				}
				else
				{
					map_iter = numberOfCooccurrence[nodeIndex2].find( nodeIndex1 );
					if ( map_iter != numberOfCooccurrence[nodeIndex2].end() )
					{
						occurCount = map_iter->second;
					}
					else
						occurCount = 0;
				}

				oldValue = (float)occurCount / nodeInfoList[nodeIndex1].occurrence / nodeInfoList[nodeIndex2].occurrence;

				newValue = (float)(occurCount - 1) / (nodeInfoList[nodeIndex1].occurrence - 1) / (nodeInfoList[nodeIndex2].occurrence - 1);

				deltaEQ += (newValue - oldValue) * (AdjacencyMatrixElement(nodeIndex1, nodeIndex2) - nodeInfoList[nodeIndex2].degree * nodeInfoList[nodeIndex1].degree / totalDegree);

			#ifdef __DEBUG__
				CFraction frac1( occurCount, nodeInfoList[nodeIndex1].occurrence * nodeInfoList[nodeIndex2].occurrence );
				CFraction frac2( occurCount - 1, (nodeInfoList[nodeIndex1].occurrence - 1) * (nodeInfoList[nodeIndex2].occurrence - 1) );
				CFraction frac3( AdjacencyMatrixElement(nodeIndex1, nodeIndex2), 1 );
				CFraction frac4( nodeInfoList[nodeIndex2].degree * nodeInfoList[nodeIndex1].degree, (int)totalDegree );

				frac2 -= frac1;
				frac3 -= frac4;
				frac2 *= frac3;

				fracEQ += frac2;
			#endif

				affectedNodeList.erase( iter );
			}
			else
			{
				cout << "Error!!\n";
				exit(0);
			}
		}

		for ( iter = affectedNodeList.begin(); iter != affectedNodeList.end(); ++iter )
		{
			nodeIndex2 = *iter;

			if ( nodeIndex1 < nodeIndex2 )
			{
				map_iter = numberOfCooccurrence[nodeIndex1].find( nodeIndex2 );
				if ( map_iter != numberOfCooccurrence[nodeIndex1].end() )
				{
					occurCount = map_iter->second;
				}
				else
					occurCount = 0;
			}
			else
			{
				map_iter = numberOfCooccurrence[nodeIndex2].find( nodeIndex1 );
				if ( map_iter != numberOfCooccurrence[nodeIndex2].end() )
				{
					occurCount = map_iter->second;
				}
				else
					occurCount = 0;
			}

			oldValue = (float)occurCount / nodeInfoList[nodeIndex1].occurrence / nodeInfoList[nodeIndex2].occurrence;

			newValue = (float)(occurCount) / (nodeInfoList[nodeIndex1].occurrence - 1) / nodeInfoList[nodeIndex2].occurrence;

			deltaEQ += (newValue - oldValue) * (AdjacencyMatrixElement(nodeIndex1, nodeIndex2) - nodeInfoList[nodeIndex2].degree * nodeInfoList[nodeIndex1].degree / totalDegree);
			deltaEQ += (newValue - oldValue) * (AdjacencyMatrixElement(nodeIndex1, nodeIndex2) - nodeInfoList[nodeIndex2].degree * nodeInfoList[nodeIndex1].degree / totalDegree);

			#ifdef __DEBUG__
				CFraction frac1( occurCount, nodeInfoList[nodeIndex1].occurrence * nodeInfoList[nodeIndex2].occurrence );
				CFraction frac2( occurCount, (nodeInfoList[nodeIndex1].occurrence - 1) * nodeInfoList[nodeIndex2].occurrence );
				CFraction frac3( AdjacencyMatrixElement(nodeIndex1, nodeIndex2), 1 );
				CFraction frac4( nodeInfoList[nodeIndex2].degree * nodeInfoList[nodeIndex1].degree, (int)totalDegree );

				frac2 -= frac1;
				frac3 -= frac4;
				frac2 *= frac3;

				fracEQ += frac2;
				fracEQ += frac2;
			#endif
		}
	}

	for ( index1 = 0; index1 < onlySecond.size(); ++index1 )
	{
		nodeIndex1 = onlySecond[index1];
		nodeInfoList[nodeIndex1].commList.erase( secondCommunityIndex );
		nodeInfoList[nodeIndex1].commList.insert( firstCommunityIndex );
	}

	for ( index1 = 0; index1 < both.size(); ++index1 )
	{
		nodeIndex1 = both[index1];
		--(nodeInfoList[nodeIndex1].occurrence);
		if ( nodeInfoList[nodeIndex1].occurrence == 0 )
		{
			cout << "occurrence = 0" << endl ;
			exit(0);
		}

		nodeInfoList[nodeIndex1].commList.erase( secondCommunityIndex );
	}

	for ( index1 = 0; index1 < both.size(); ++index1 )
	{
		nodeIndex1 = both[index1];
		for ( index2 = index1; index2 < both.size(); ++index2 )
		{
			nodeIndex2 = both[index2];

			if ( nodeIndex1 < nodeIndex2 )
			{
				map_iter = numberOfCooccurrence[nodeIndex1].find( nodeIndex2 );
				if ( map_iter != numberOfCooccurrence[nodeIndex1].end() )
				{
					if ( map_iter->second == 1 )
					{
						numberOfCooccurrence[nodeIndex1].erase( map_iter );
					}
					else
					{
						--(map_iter->second);
					}
				}
			}
			else
			{
				map_iter = numberOfCooccurrence[nodeIndex2].find( nodeIndex1 );
				if ( map_iter != numberOfCooccurrence[nodeIndex2].end() )
				{
					if ( map_iter->second == 1 )
					{
						numberOfCooccurrence[nodeIndex2].erase( map_iter );
					}
					else
					{
						--(map_iter->second);
					}
				}
			}
		}
	}

	communityList[firstCommunityIndex].nodeList = tempNodeList;
	communityList[firstCommunityIndex].size = tempNodeList.size() - 1;
	communityList[secondCommunityIndex].status = 1;

	int communityIndex;
	float sim;

	for ( communityIndex = 1; communityIndex < firstCommunityIndex; ++communityIndex )
	{
		if ( 1 == communityList[communityIndex].status )
			continue;

		sim = Similarity( firstCommunityIndex, communityIndex );

		if ( sim > 0 )
			deltaExtendModularity[communityIndex][firstCommunityIndex] = sim;
	}

	communityIndex++; 

	for ( ; communityIndex <= maximalCliqueNumber; communityIndex++ )
	{
		if ( 1 == communityList[communityIndex].status )
			continue;

		sim = Similarity( firstCommunityIndex, communityIndex );

		if ( sim > 0 )
			deltaExtendModularity[firstCommunityIndex][communityIndex] = sim;
	}

	EQ += deltaEQ;
}

/*
 *	Function:
 *				uncover the overlapping community structure of the network
 *	Parameters:
 *				communityNumber		record the number of overlapped communities
 *	Return:
 *				the value of corresponding EQ
 */
float CHierarchicalOverlap::UncoverCommunityStructure( int & communityNumber )
{
	int row;
	int col;
	int selectRow = 0;
	int selectCol = 0;
	float max = -1.0;
	int	unionRound;
	float deltaEQ;

	if (LoadMaximalClique("AllClique.res") < -1)
		return -1;
	
	PreProcess();

	cout << "begin merge...";
	cout.flush();

	#ifdef __DEBUG__
	cout << "Current EQ = " <<  EQ / totalDegree << "\n";
	cout << "Current EQ = " <<  fracEQ  << " = " << fracEQ.get_numerator() / totalDegree / fracEQ.get_denominator() << "\n\n";
	#endif

	unionRound = 1;
	while (1)
	{
		selectRow = 0;
		selectCol = 0;
		max = -1.0;
		for ( row = 1; row <= maximalCliqueNumber; row++ )
		{
			if ( communityList[row].status == 1 )
				continue;
				
			for ( map<int, float>::iterator iter = deltaExtendModularity[row].begin(); iter != deltaExtendModularity[row].end();  ++iter )
			{
				col = iter->first;
				if ( communityList[col].status == 1 )
					continue;

				if ( selectRow == 0 )
				{
					selectRow = row;
					selectCol = col;
					max = iter->second;
				}
				else
				{
					if ( iter->second > max )
					{
						selectRow = row;
						selectCol = col;
						max = iter->second;
					}
				}
			}
		}
		
		if ( selectRow == 0 )
			break;

		deltaExtendModularity[selectRow].erase( selectCol );
		
		UnionCommunity( selectRow, selectCol, deltaEQ );

		unionSequence[unionRound].leftChild = selectRow;
		unionSequence[unionRound].rightChild = selectCol;
		
		EQList[unionRound] = EQList[unionRound - 1] + deltaEQ;

	#ifdef __DEBUG__
		cout << "Merge community " <<  selectRow << " & " <<  selectCol;
		cout << " --|-- delta EQ = " <<  deltaEQ / totalDegree;
		cout << " --|-- Current EQ =" <<  EQ / totalDegree << "\n";
		cout << "Current EQ = " <<  fracEQ  << " = " << fracEQ.get_numerator() / totalDegree / fracEQ.get_denominator() << "\n\n";
	#endif

	#ifdef __DEBUG__
		cout << "Merge community " <<  selectRow << " & " <<  selectCol;
		cout << " --|-- Current EQ =" <<  EQ / totalDegree << "\n";
	#endif

		unionRound++;
	}

	communityNumber = Recovery( "AllClique.res" );
	
	float ultimateEQ;

	ultimateEQ = EQList[maximalCliqueNumber - communityNumber] / totalDegree;

#ifdef _DEBUG_
	cout << "EQ =" << ultimateEQ << "\n";
	if ( isnan(ultimateEQ ) )
	{
		cout << "ultimateEQ is not a number.\n";
		exit(0);
	}
		
	cout << "=====================================\n";
#endif

	cout << "ok" << endl;
//	cout << "one pass" << endl;

	return ultimateEQ;
}

/*
 *	Function:
 *				output the overlapped communities
 *	Parameters:
 *				resultFileName		the file to store the result
 *				EQ					the value of the corresponding EQ
 *	Return:
 *				0	success
 *				-1	failed
 */
int CHierarchicalOverlap::OutputCommunityStructure( const char *resultFileName, const float EQ )
{
	ofstream resultFile;

	resultFile.open( resultFileName );
	if ( resultFile.fail() )
	{
		cout << "open file " << resultFileName << " failed.\n";
		return -1;
	}

	resultFile << EQ << "\n";

	for ( int i = 1; i <= maximalCliqueNumber; ++i )
	{
		if ( communityList[i].status != 0 )
			continue;

		for ( int j = 1; j <= communityList[i].size; ++j )
		{
			resultFile <<  nodeInfoList[communityList[i].nodeList[j]].label << "\t";
		}
		resultFile << "\n";
	}

	resultFile.close();

	return 0;
}

/*
 *	Function:
 *				Recover the cover which corresponds to the maximum EQ 
 *	Parameters:
 *				cliqueFileName		the file which records the maximal cliques
 *	Return:
 *				the number of overlapped communities
 */
int CHierarchicalOverlap::Recovery( const char *cliqueFileName )
{
	int counter;
	float maxEQ;

	maxEQ = EQList[0];

	int unionRound = 0;
	for ( counter = 1; counter < maximalCliqueNumber; ++counter )
	{
		if ( maxEQ <= EQList[counter] )
		{
			maxEQ = EQList[counter];
			unionRound = counter;
		}
	}

	LoadMaximalClique1( cliqueFileName );

	int firstCommunityIndex;
	int secondCommunityIndex;

	for ( counter = 1; counter <= unionRound; counter++ )
	{
		firstCommunityIndex = unionSequence[counter].leftChild;
		secondCommunityIndex = unionSequence[counter].rightChild;

		UnionCommunity1( firstCommunityIndex, secondCommunityIndex );
	}

	return maximalCliqueNumber - unionRound; 
}

/*
 *	Similar to UnionCommunity, except that this fuction does not recalculate the delta EQ
 */
void CHierarchicalOverlap::UnionCommunity1( const int firstCommunityIndex, const int secondCommunityIndex )
{
	vector<int> tempNodeList;
	int i;
	int j;
	
	i = 1;
	j = 1;
	tempNodeList.push_back(0);
	while ( i <= communityList[firstCommunityIndex].size && j <= communityList[secondCommunityIndex].size )
	{
		if ( communityList[firstCommunityIndex].nodeList[i] < communityList[secondCommunityIndex].nodeList[j] )
		{
			tempNodeList.push_back( communityList[firstCommunityIndex].nodeList[i] );
			i++;
		}
		else if ( communityList[firstCommunityIndex].nodeList[i] > communityList[secondCommunityIndex].nodeList[j] )
		{
			tempNodeList.push_back( communityList[secondCommunityIndex].nodeList[j] );
			j++;
		}
		else
		{
			tempNodeList.push_back( communityList[firstCommunityIndex].nodeList[i] );
			i++;
			j++;
		}
	}
	
	while ( i <= communityList[firstCommunityIndex].size )
	{
		tempNodeList.push_back( communityList[firstCommunityIndex].nodeList[i] );
		i++;
	}
	
	while ( j <= communityList[secondCommunityIndex].size )
	{
		tempNodeList.push_back( communityList[secondCommunityIndex].nodeList[j] );
		j++;
	}

	communityList[firstCommunityIndex].nodeList = tempNodeList;
	communityList[firstCommunityIndex].size = tempNodeList.size() - 1;
	communityList[secondCommunityIndex].status = 1;
}

/*
 *	similar the LoadMaximalClique
 */
int CHierarchicalOverlap::LoadMaximalClique1( const char *maximalCliqueFileName )
{
	ifstream maximalCliqueFile;
	char	*buff;
	long	size;
	int		count;
	char	*ptr;

	maximalCliqueFile.open( maximalCliqueFileName, ios::binary );
	if ( maximalCliqueFile.fail() )
	{
		cout << "open file " << maximalCliqueFileName << " failed.\n";
		return -1;
	}
	
	maximalCliqueFile.seekg( 0, ios::end );
	size = maximalCliqueFile.tellg();
	buff = new char[size + 2];
	maximalCliqueFile.seekg( 0L, ios::beg );
	maximalCliqueFile.read( buff, size );
	maximalCliqueFile.close();

	if ( buff[size - 1] != '\n' )
	{
		buff[size] = '\n';
		size++;
	}
	buff[size] = '\0';
	
	count = 0;
	ptr = buff;
	while (1)
	{
		ptr = strchr( ptr, '\n' );
		if ( ptr == NULL )
			break;

		++ptr;
		++count;
	}
	
	maximalCliqueNumber = count;

	char *lineBegin;
	char *lineEnd;
	int cliqueSize;
	int cliqueIndex = 1;

	lineBegin = buff;
	while ( (lineEnd = strchr(lineBegin, '\n')) )
	{
		if ( *(lineEnd - 1) == '\r' )
			*(lineEnd - 1) = '\0';
		else
			*lineEnd = '\0';

		sscanf( lineBegin, "%d\t", &cliqueSize );

		communityList[cliqueIndex].nodeList.resize( cliqueSize + 1 );
		communityList[cliqueIndex].size = cliqueSize;
		communityList[cliqueIndex].status = 0;

		ptr = strchr( lineBegin, '\t');
		ptr++;

		int index = 1;
		int nodeIndex;
		while ( *ptr != '\0' )
		{
			sscanf(ptr, "%d ", &nodeIndex);
			communityList[cliqueIndex].nodeList[index] = nodeIndex;
			
			ptr = strchr(ptr, ' ');
			ptr++;
			index++;
		}

		cliqueIndex++;

		*lineEnd = '\n';
		lineBegin = lineEnd + 1;
	}

	delete []buff;

	return 0;
}
