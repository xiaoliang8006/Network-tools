#ifndef _HIERARCHICAL_OVERLAP_H_
#define _HIERARCHICAL_OVERLAP_H_

#ifdef __DEBUG__
	#include "fraction.h"
#endif

#include <vector>
#include <set>
#include <map>
#include <string>

struct stCommunity
{
	int	status;
	int size;
	std::vector<int> nodeList;
};

struct stNodeInfo
{
	stNodeInfo()
	{
		degree = 0;
		occurrence = 0;
	}

	int	degree;
	std::string	label;
	std::set<int>	commList;
	int occurrence;
};

struct stAdjacencyMatrixHeader
{
	std::set<int> neighborNodeList;
};

struct stUnionNode
{
	stUnionNode()
	{
		leftChild = 0;
		rightChild = 0;
		status = 0;
	}

	int leftChild;
	int rightChild;
	int status;
};

class CHierarchicalOverlap
{
public:
	CHierarchicalOverlap();
	~CHierarchicalOverlap();
	
public:
	// load the adjaceny matrix and the label for each node
	int LoadDataFile( const char *labelFileName, const char *adjacencyMatrixFileName, float &totalWeight );

	// uncover the overlapping community structure of the network
	float UncoverCommunityStructure( int & communityNumber );

	// output the overlapped communities
	int OutputCommunityStructure( const char *resultFileName, const float EQ );

	// load all the maximal cliques in the network
	int LoadMaximalClique( const char *maximalCliqueFileName );
	
private:
 	// add an edge <row, col> to adjacency matrix
	void AddAdjacencyMatrixElement( const int row, const int col );
	
 	// check whether node with index 'row' is connected to node with index 'col' by an edge 
	int AdjacencyMatrixElement( const int row, const int col );
	
//	float DeltaExtendModularity( const int firstCommunityIndex, const int secondCommunityIndex );

	// calculate the similarity between two communities
	float Similarity( const int firstCommunityIndex, const int secondCommunityIndex );

	// initialize
	void PreProcess();

	// incorporate one two communities into a new one
	void UnionCommunity( const int firstCommunityIndex, const int secondCommunityIndex, float & deltaEQ );

	// similar to UnionCommunity, except that this fuction does not recalculate the delta EQ
	void UnionCommunity1( const int firstCommunityIndex, const int secondCommunityIndex );

	// Recover the cover which corresponds to the maximum EQ 
	int Recovery( const char *cliqueFileName );
	
	// similar the LoadMaximalClique
	int LoadMaximalClique1( const char *maximalCliqueFileName );
	
private:
	int vertexNumber;
	
	float totalDegree;

	std::vector<stNodeInfo>	nodeInfoList;

	std::vector<stAdjacencyMatrixHeader>	adjacencyMatrix;
	
	int maximalCliqueNumber;
	
	std::vector< stCommunity > communityList;
	
	std::vector< std::map<int, float> >	deltaExtendModularity;

	std::vector< std::map<int, int> >	numberOfCooccurrence;	

	std::vector<stUnionNode>	unionSequence;

	std::vector<float>	EQList;

	float EQ;

#ifdef __DEBUG__
	CFraction fracEQ;
#endif
};

#endif /* _HIERARCHICAL_OVERLAP_H_ */
