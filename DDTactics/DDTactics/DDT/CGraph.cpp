#include "CGraph.h"

CGraph::CGraph(void)
{}

CGraph::~CGraph(void)
{}

void CGraph::init()
{
	float x,z;
	x = -11.7f;
	z= 11.7f;
	Battle_Node temp;
	temp.position = D3DXVECTOR3(x, 0.0f, z);
	m_graph.push_back(temp);
	for(int i = 0; i < 99; i++)
	{
	
		x += 2.6f;
		if(i>0)
		{
			if(i % 9 == 0)
			{
				z -= 2.6f;
				x = -11.7f;
			}
		}

		temp.position = D3DXVECTOR3(x, 0.0f, z);
		m_graph.push_back(temp);
	
	
	}

	for( int i = 0; i < 99; i++){
		// 4 possible connections
		// -10
		if((i-10) > 0)
			m_graph[i].edges.push_back(&m_graph[i-10]);

		/////// finish

		//// -1
		//if((i-10) > 0)
		//	m_graph[i].edges.push_back(&m_graph[i-10]);
		//// +10
		//if((i-10) > 0)
		//	m_graph[i].edges.push_back(&m_graph[i-10]);
		//// +1
		//if((i-10) > 0)
		//	m_graph[i].edges.push_back(&m_graph[i-10]);
	}


}

void CGraph::shutdown()
{
}

void CGraph::getPath(int start, int end, std::vector<int> &path)
{
}