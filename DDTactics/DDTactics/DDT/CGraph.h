#pragma once

#include "MiscStructs.h"

class CGraph
{
public:
	std::vector<Battle_Node> m_graph;

public:
	CGraph(void);
	~CGraph(void);

	void init();
	void shutdown();

	//pathing for a.i
	void getPath(int start, int end, std::vector<int> &path);

};
