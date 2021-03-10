[Specification]
The original network is stored in the file named 'SubGraph0' and the file 'SubVertex0' accomodates the label for the nodes in the original network.

The result is stored in the directory named 'result' in the path same to the executable program.

The hierarchical community structure is stored by means of the hierarchy of the file system. In each level of the hierarchy of the file system, a file named 'result.txt' records the overlapped communities in the corresponding level in the hierarchy of community structure. The first line in the file records the value of the correponding EQ. And from the second line to the last line, each line corresponds to a community.

When the program proceeds, several temp files are generated to accomodate the subnetwork and labels for the nodes in the subnetwork. These temp files have the name formats: SubGraph[n] or SubVertex[n]. Here, [n] can be 0, 1, 2, ..., and the value of [n] correponds to the level of the current subnetwork (commnity). The maximum value of [n] depends of the specifc network inputed. These temp files are not deleted by the program. Users can delete them and the deletion does not affect the result of the program. 

Additionally, a file named 'AllLeaf.txt' is generated in the path same to the executable program. This file store all the leaf-node community, which can not be divided into sub-communities again.

The main program named 'OverlapCommunityShell' call other programs.

[Compile]
	make

[Usage]
./OverlapCommunityShell MinimumCliqueSize
For example: ./OverlapCommunityShell 4

