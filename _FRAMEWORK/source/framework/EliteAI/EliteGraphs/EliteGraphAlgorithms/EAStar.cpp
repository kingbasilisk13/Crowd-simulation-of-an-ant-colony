#include "stdafx.h"
#include "EAStar.h"

using namespace Elite;
AStar::AStar(Graph* pGraph, Heuristic hFunction)
	: m_pGraph(pGraph)
	, m_HeuristicFunction(hFunction)
{
}

std::vector<GraphNode*>AStar::FindPath(GraphNode* pStartNode, GraphNode* pGoalNode)
{
	std::vector<GraphNode*> path{};
	std::list<NodeRecord> openList;
	std::list<NodeRecord> closedList;
	NodeRecord currentNodeRecord{};

	NodeRecord startRecord{};
	startRecord.pNode = pStartNode;
	startRecord.estimatedTotalCost = GetHeuristicCost(pStartNode, pGoalNode);

	openList.push_back(startRecord);

	while(!openList.empty())
	{
		//std::min_element geeft niet het object maar een iterator terug. voor iterator gebruik je best auto.
		// je krijgt het object terug door het te de referencen aka *currentNodeRecordIt
		auto currentNodeRecordIt = std::min_element(openList.begin(), openList.end());
		currentNodeRecord = *currentNodeRecordIt;

		if (currentNodeRecord.pNode == pGoalNode)
		{
			break;
		}
		
		for (GraphConnection* connection : m_pGraph->GetConnectionsFromNode(currentNodeRecord.pNode))
		{
			bool skipToNextConection = false;

			GraphNode* pNextNode = m_pGraph->GetNode(connection->GetToNodeId());
			float gCost = currentNodeRecord.costSoFar + connection->GetCost();

			for (const NodeRecord& nodeRecord : closedList)
			{
				if(nodeRecord.pNode == pNextNode)
				{
					if(nodeRecord.costSoFar <= gCost)
					{
						skipToNextConection = true;
						break;
					}
					else
					{
						closedList.remove(nodeRecord);
						break;
					}
				}
			}

			for (const NodeRecord& nodeRecord : openList)
			{
				if (nodeRecord.pNode == pNextNode)
				{
					if (nodeRecord.costSoFar <= gCost)
					{
						skipToNextConection = true;
						break;
					}
					else
					{
						openList.remove(nodeRecord);
						break;
					}
				}
			}

			if (skipToNextConection)
			{
				continue;
			}

			NodeRecord newNodeRecord{};
			newNodeRecord.pNode = pNextNode;
			newNodeRecord.costSoFar = gCost;
			newNodeRecord.pConnection = connection;
			newNodeRecord.estimatedTotalCost = gCost + GetHeuristicCost(pNextNode, pGoalNode);

			openList.push_back(newNodeRecord);
		}

		openList.remove(currentNodeRecord);
		closedList.push_back(currentNodeRecord);
	}

	if (openList.empty())
	{
		return path;
	}

	while (currentNodeRecord.pNode != pStartNode)
	{
		path.push_back(currentNodeRecord.pNode);
		
		for (const NodeRecord& nodeRecord : closedList)
		{
			if(currentNodeRecord.pConnection)
			{
				if (nodeRecord.pNode == m_pGraph->GetNode(currentNodeRecord.pConnection->GetFromNodeId()))
				{
					currentNodeRecord = nodeRecord;
					break;
				}
			}
		}
	}
	path.push_back(pStartNode);

	std::reverse(path.begin(), path.end());

	return path;
}


float AStar::GetHeuristicCost(GraphNode* pStartNode, GraphNode* pEndNode) const
{
	Vector2 toDestination = m_pGraph->GetNodePos(pEndNode->GetId()) - m_pGraph->GetNodePos(pStartNode->GetId());
	return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
}