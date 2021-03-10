OPTION=-O2 -DDEBUG
"OPTION = -g

all: OverlapCommunityShell BronKerbosch Extract

OverlapCommunityShell: HierarchicalOverlap.cpp OverlapCommunityShell.cpp fraction.cpp HierarchicalOverlap.h fraction.h
	g++ HierarchicalOverlap.cpp OverlapCommunityShell.cpp fraction.cpp -o OverlapCommunityShell $(OPTION) -Wall

BronKerbosch: BronKerbosch.cpp BronKerbosch.h
	g++ BronKerbosch.cpp -o BronKerbosch $(OPTION) -Wall

Extract: Extract.cpp
	g++ Extract.cpp -o Extract $(OPTION) -Wall

clean:
	rm BronKerbosch || true
	rm Extract || true
	rm OverlapCommunityShell || true
	rm -rf result || true
	rm -rf SubGraph* SubVertex* || true
	cp ../Graph.txt ./SubGraph0
	cp ../Vertex.txt ./SubVertex0

