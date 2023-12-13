#include "stdafx.h"

#include "ENavGraphPathfinding.h"
#include "framework/EliteAI/EliteGraphs/EliteGraphAlgorithms/EAStar.h"
#include "framework/EliteAI/EliteGraphs/EliteNavGraph/ENavGraph.h"

using namespace Elite;

std::vector<Vector2> NavMeshPathfinding::FindPath(Vector2 startPos, Vector2 endPos, NavGraph* pNavGraph, std::vector<Vector2>& debugNodePositions, std::vector<Portal>& debugPortals)
{
	//Create the path to return
	std::vector<Vector2> finalPath{};

	//Get the startTriangle and endTriangle
	const Triangle* startTriangle{};
	const Triangle* endTriangle{};
	
	startTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(startPos);
	endTriangle = pNavGraph->GetNavMeshPolygon()->GetTriangleFromPosition(endPos);

	//If we don't have a valid startTriangle or endTriangle -> return empty path
	if (!startTriangle || !endTriangle) return finalPath;

	//If the startTriangle and endTriangle are the same -> return straight line path
	if (startTriangle == endTriangle)
	{
		finalPath.push_back(startPos);
		finalPath.push_back(endPos);
		return finalPath;
	}

	//=> Start looking for a path
	//Clone the graph (returns shared_ptr!)
	std::shared_ptr<NavGraph> clone = pNavGraph->Clone();
	
	//Create extra node for the Start Node (Agent's position) and add it to the graph. 
	//Make connections between the Start Node and the startTriangle nodes.
	NavGraphNode* startNavGraphNode = new NavGraphNode(-1, startPos);
	
	clone->AddNode(startNavGraphNode);
	for (int lineIdx : startTriangle->metaData.IndexLines)
	{
		int nodeId = clone->GetNodeIdFromLineIndex(lineIdx);
		if (nodeId != invalid_node_id)
		{
			GraphConnection* connection = new GraphConnection();
			connection->SetFromNodeId(startNavGraphNode->GetId());
			connection->SetToNodeId(nodeId);
			float cost = Elite::Distance(startNavGraphNode->GetPosition(), clone->GetNodePos(nodeId));
			connection->SetCost(cost);
			clone->AddConnection(connection);
		}
	}
	//Create extra node for the End Node (endpos) and add it to the graph. 
	//Make connections between the End Node and the endTriangle nodes.
	NavGraphNode* endNavGraphNode = new NavGraphNode(-1, endPos);

	clone->AddNode(endNavGraphNode);
	for (int lineIdx : endTriangle->metaData.IndexLines)
	{
		int nodeId = clone->GetNodeIdFromLineIndex(lineIdx);
		if (nodeId != invalid_node_id)
		{
			GraphConnection* connection = new GraphConnection();
			connection->SetFromNodeId(nodeId);
			connection->SetToNodeId(endNavGraphNode->GetId());
			float cost = Elite::Distance(endNavGraphNode->GetPosition(), clone->GetNodePos(nodeId));
			connection->SetCost(cost);
			clone->AddConnection(connection);
		}
	}

	//Run AStar on the new graph
	//astar heeft raw pointer nodig, die krijg je via de shared_ptr
	auto pathfinder = AStar(clone.get(), Elite::HeuristicFunctions::Chebyshev);

	std::vector<GraphNode*> resultAStar = pathfinder.FindPath(startNavGraphNode, endNavGraphNode);

	debugNodePositions.clear();
	for(const GraphNode* node: resultAStar)
	{
		debugNodePositions.push_back(node->GetPosition());
		//finalPath.push_back(node->GetPosition());
	}

	//Run optimiser on new graph, MAKE SURE the AStar path is working properly before starting the following section:
	auto m_Portals = SSFA::FindPortals(resultAStar, pNavGraph->GetNavMeshPolygon());
	debugPortals = m_Portals;
	finalPath = SSFA::OptimizePortals(m_Portals);

	return finalPath;
}