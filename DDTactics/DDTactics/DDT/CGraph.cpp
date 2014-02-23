#include "CGraph.h"
CGraph::CGraph(void)
{
}


CGraph::~CGraph(void)
{
}

void CGraph::init()
{
	float x,z;x=-11.7f;z=11.7f;
	Battle_Node temp;
	temp.position = D3DXVECTOR3(x,0.0f,z);
	m_graph.push_back(temp);
	int count = 1;

	for( int i = 0; i<99; i++){
		x+=2.6f;
		if(count == 10){
			count = 0;
			z -= 2.6f; x = (-11.7f);
		}
		temp.position = D3DXVECTOR3(x,0.0f,z);
		m_graph.push_back(temp);
		count++;
	}

	count = 0;
	for( int i = 0; i < 100; i++){
		// 4 possible connections
		// -10
		if((i-10) >= 0)
			m_graph[i].edges.push_back(&m_graph[i-10]);
		// -1
		if(count!=0)
			m_graph[i].edges.push_back(&m_graph[i-1]);
		// +10
		if((i+10) < 100)
			m_graph[i].edges.push_back(&m_graph[i+10]);
		// +1
		if(count != 9)
			m_graph[i].edges.push_back(&m_graph[i+1]);
		count++;
		if(count == 10)	count = 0;
	}
	for(auto &node:m_graph)
		node.visited = false;
}

void CGraph::shutdown()
{
}


void CGraph::getPath(int start,int end,std::vector<int> &path)
{
}

