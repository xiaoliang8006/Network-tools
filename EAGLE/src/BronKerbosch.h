#ifndef BRONKERBOSCH_H
#define BRONKERBOSCH_H

#include <vector>
#include <map>
#include <set>
#include <fstream>

class CBronKerbosch 
{
public:
	CBronKerbosch();
	~CBronKerbosch();

public:
	void findMaximalClique( std::vector< std::map<int, int> > & adjacencyMatrix, const int minCliqueSize );

	const std::vector< std::vector<int> > & getMaximalCliqueList();

	int output( const char *maximalCliqueFileName );

private:
	void extend( std::vector< std::map<int, int> > & adjacencyMatrix, std::vector<int> & old, int ne, const int ce );

private:
	std::vector<int> m_all;
	std::vector<int> m_compsub;
	int m_c;
	int m_minSize;
	std::vector<int> m_status;
	std::vector<int> m_outBuffer;
	std::vector< std::vector<int> > m_maximalCliqueList;
};

#endif /* BRONKERBOSCH_H */
