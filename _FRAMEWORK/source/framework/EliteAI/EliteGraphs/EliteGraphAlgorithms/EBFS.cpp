#include "stdafx.h"
#include "EBFS.h"

#include "../EliteGraph/EGraph.h"
#include "../EliteGraph/EGraphNode.h"
#include "../EliteGraph/EGraphConnection.h"

using namespace Elite;

BFS::BFS(Graph* pGraph)
	: m_pGraph(pGraph)
{
}

//Breath First Search Algorithm searches for a path from the startNode to the destinationNode
std::vector<GraphNode*> BFS::FindPath(GraphNode* pStartNode, GraphNode* pDestinationNode)
{
	std::vector<GraphNode*> path{};

	std::queue<GraphNode*> openList{};
	std::map<GraphNode*, GraphNode*> closedList{};

	openList.push(pStartNode);

	while (!openList.empty())
	{
		GraphNode* pCurrentNode = openList.front();
		openList.pop();

		//stop the loop when the destination node is found.
		if(pCurrentNode == pDestinationNode)
		{
			break;
		}

		//find neighbors, add them to openlist
		for(GraphConnection* connection: m_pGraph->GetConnectionsFromNode(pCurrentNode))
		{
			GraphNode* nextNode = m_pGraph->GetNode(connection->GetToNodeId());

			//don't check the same node twice.
			if(closedList.contains(nextNode) == false)
			{
				openList.push(nextNode);
				closedList[nextNode] = pCurrentNode;
			}
		}
	}
	//check if we reached the destication node.
	//if(!closedList.contains(pDestinationNode))
	if(openList.empty())
	{
		return path;
	}


	GraphNode* pCurrentNode = pDestinationNode;

	while (pCurrentNode != pStartNode)
	{
		path.push_back(pCurrentNode);
		pCurrentNode = closedList[pCurrentNode];

	}
	path.push_back(pStartNode);

	std::reverse(path.begin(), path.end());
	return path;
}