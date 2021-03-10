/***************************************************************************\
 *   Copyright (C) 2007 by Huawei Shen                                     *
 *   shenhuawei@software.ict.ac.cn                                         *
 *	 last revision: 2009-06-17                                             *
 *																		   *
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
#include <string>
#include <sstream>
#include <cmath>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "HierarchicalOverlap.h"

using namespace std;

#define	RESOLUTION	0.000001

char workRootPath[PATH_MAX];

string subGraphFileName;
string subVertexFileName;
string commandString;
string argv1;
string argv2;
string folderName;
int minimumCliqueSize;

ofstream allLeafFile;

int level;

void process()
{
	ifstream resultFile;
	ofstream patternFile;
	float maxExtendedModularity;
	int	communityNumber;
	int retCode;
	float totalDegree;

	resultFile.open( "result.txt" );
	if ( resultFile.fail() )
	{
		cout << "open file \"result.txt\" failed.\n";
		exit(0);
	}

	int index = 1;
	string oneLine;
	char currentPath[PATH_MAX];

	std::getline( resultFile, oneLine );

	while ( std::getline( resultFile, oneLine ) )
	{
		getcwd( currentPath, PATH_MAX );

		if ( chdir(workRootPath) < 0 )
		{
			cout << "change directory failed.\n";
			exit(0);
		}

		patternFile.open( "pattern.txt" );
		if ( patternFile.fail() )
		{
			cout << "open file \"pattern.txt\" failed.\n";
			exit(0);
		}
		patternFile << oneLine << "\n";
		patternFile.close();

		// construct subgraph and its label list
		{
			stringstream ss;

			ss << "./Extract pattern.txt SubGraph" << level-1 << " SubGraph" << level << " SubVertex" << level << " SubVertex" << level-1;
			commandString = ss.str();

			system( commandString.c_str() );
		}

		// obtain all the maximal cliques
		{
			stringstream ss;

			ss << "./BronKerbosch SubGraph" << level << " " << minimumCliqueSize;
			commandString = ss.str();

			system( commandString.c_str() );
		}

//		sprintf(CommandString, "./ExtractMaximumClique");
//		system(CommandString);

		CHierarchicalOverlap *iHierarchicalOverlap = new CHierarchicalOverlap;

		{
			stringstream ss;

			ss << "SubGraph" << level;
			argv1 = ss.str();
		}

		{
			stringstream ss;

			ss << "SubVertex" << level;
			argv2 = ss.str();
		}
		iHierarchicalOverlap->LoadDataFile( argv2.c_str(), argv1.c_str(), totalDegree );

		if ( fabs(totalDegree) < RESOLUTION )
		{
			if ( chdir(currentPath) < 0)
			{
				cout << "change directory failed.\n";
				exit(0);
			}

			delete iHierarchicalOverlap;
			allLeafFile << oneLine << "\n";
		}
		else
		{
			maxExtendedModularity = iHierarchicalOverlap->UncoverCommunityStructure( communityNumber );

			if (chdir(currentPath) < 0)
			{
				cout << "change directory failed.\n";
				exit(0);
			}

			if ( communityNumber > 1 && fabs(maxExtendedModularity) > RESOLUTION	)
			{

				{
					stringstream ss;
					ss << index;
					folderName = ss.str();
				}

				if ( (retCode = mkdir( folderName.c_str(), 0755)) < 0 )
				{
					cout << "can not create directory\n";
					exit(0);
				}

				if ( chdir(folderName.c_str()) < 0 )
				{
					cout << "change directory failed.\n";
					exit(0);
				}

				iHierarchicalOverlap->OutputCommunityStructure( "result.txt", maxExtendedModularity );
				delete iHierarchicalOverlap;

				++level;
				process();
			}
			else
			{
				delete iHierarchicalOverlap;
				allLeafFile << oneLine << "\n";
			}
		}

		++index;
	}

	resultFile.close();

	if ( chdir("..") < 0 )
	{
		cout << "change directory failed.\n";
		exit(0);
	}
	--level;
}

int main(int argc, char **argv)
{
	int retCode;
	int communityNumber;
	float maxExtendedModularity;
	float totalDegree;

	if (argc != 2)
	{
		printf("usage: %s MinimumCliqueSize\n", argv[0]);
		exit(0);
	}

	minimumCliqueSize = atoi(argv[1]);

	allLeafFile.open( "AllLeaf.txt" );
	if ( allLeafFile.fail() )
	{
		cout << "open file \"AllLeaf.txt\" failed.\n";
		exit(0);
	}

	getcwd( workRootPath, PATH_MAX );

	{
		stringstream ss;

		ss << "./BronKerbosch SubGraph0 " << minimumCliqueSize;
		commandString = ss.str();

		system( commandString.c_str() );
	}

//	sprintf(CommandString, "./ExtractMaximumClique");
//	system(CommandString);

	{
		CHierarchicalOverlap iHierarchicalOverlap;

		iHierarchicalOverlap.LoadDataFile( "SubVertex0", "SubGraph0", totalDegree );

		if ( fabs(totalDegree) > RESOLUTION )
		{
			maxExtendedModularity = iHierarchicalOverlap.UncoverCommunityStructure( communityNumber );

			if ( communityNumber > 1 )
			{
				if ( (retCode = mkdir("result", 0755)) < 0 )
				{
					cout << "can not create directory\n";
					exit(0);
				}

				if (chdir("result") < 0)
				{
					cout << "change directory failed.\n";
					exit(0);
				}

				iHierarchicalOverlap.OutputCommunityStructure( "result.txt", maxExtendedModularity );

				level = 1;
				process();
			}
			else
			{
				iHierarchicalOverlap.OutputCommunityStructure( "result.txt", maxExtendedModularity );

			}
		}
		else
		{
			printf( "no edges exist in the given network.\n");
		}
	}

	allLeafFile.close();

	return 0;
}
