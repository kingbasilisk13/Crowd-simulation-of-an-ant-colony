#pragma once
#include <stack>
#include "../EliteGraph/EGraph.h"
#include "../EliteGraph/EGraphConnection.h"
#include "../EliteGraph/EGraphNode.h"
namespace Elite
{
	enum class Eulerianity
	{
		notEulerian,
		semiEulerian,
		eulerian,
	};

	class EulerianPath
	{
	public:

		EulerianPath(Graph* pGraph);

		Eulerianity IsEulerian() const;
		std::vector<GraphNode*> FindPath(Eulerianity& eulerianity) const;

	private:
		void VisitAllNodesDFS(const std::vector<GraphNode*>& pNodes, std::vector<bool>& visited, int startIndex) const;
		bool IsConnected() const;

		Graph* m_pGraph;
	};

	inline EulerianPath::EulerianPath(Graph* pGraph)
		: m_pGraph(pGraph)
	{
	}

	inline Eulerianity EulerianPath::IsEulerian() const
	{

		// If the graph is not connected, there can be no Eulerian Trail
		if(IsConnected() == false)
		{
			return Eulerianity::notEulerian;
		}

		// Count nodes with odd degree 
		int amtOddNodes{ 0 };
		auto& graphNodes = m_pGraph->GetAllNodes();
		for(auto node : graphNodes)
		{
			auto& connections = m_pGraph->GetConnectionsFromNode(node);
			if(connections.size() % 2 == 1)
			{
				++amtOddNodes;
			}
		}


		// A connected graph with more than 2 nodes with an odd degree (an odd amount of connections) is not Eulerian
		if(amtOddNodes > 2)
		{
			return Eulerianity::notEulerian;
		}

		// A connected graph with exactly 2 nodes with an odd degree is Semi-Eulerian
		// An Euler trail can be made, but only starting and ending in these 2 nodes
		if (amtOddNodes == 2)
		{
			return Eulerianity::semiEulerian;
		}

		// A connected graph with no odd nodes is Eulerian
		return Eulerianity::eulerian;
	}

	inline std::vector<GraphNode*> EulerianPath::FindPath(Eulerianity& eulerianity) const
	{
		// Get a copy of the graph because this algorithm involves removing edges
		auto graphCopy = m_pGraph->Clone();
		auto path = std::vector<GraphNode*>();
		int nrOfNodes = graphCopy->GetAmountOfNodes();

		// Check if there can be an Euler path
		// If this graph is not eulerian, return the empty path

		auto nodes = m_pGraph->GetAllNodes();

		GraphNode* currentNode{nullptr};
		
		switch (eulerianity)
		{
		case Eulerianity::eulerian:
			int startingIndex;

			startingIndex = rand() % nodes.size();

			currentNode = nodes[startingIndex];
			break;
		case Eulerianity::notEulerian:
			return path;
			break;
		case Eulerianity::semiEulerian:
			for(int i{0}; i < nodes.size(); ++i)
			{
				if(m_pGraph->GetConnectionsFromNode(nodes[i]->GetId()).size() % 2 != 0)
				{
					currentNode = nodes[i];
					break;
				}
			}
			break;
		default:
			break;
		}

		// Find a valid starting id for the algorithm

		// Start algorithm loop
		std::stack<int> nodeStack;

		do
		{
			if(graphCopy->GetConnectionsFromNode(currentNode).size() > 0)
			{
				nodeStack.push(currentNode->GetId());
				auto connection = graphCopy->GetConnectionsFromNode(currentNode)[0];
				currentNode = m_pGraph->GetNode(connection->GetToNodeId());
				graphCopy->RemoveConnection(connection);
			}
			else
			{
				path.push_back(m_pGraph->GetNode(currentNode->GetId()));
				currentNode = m_pGraph->GetNode(nodeStack.top());
				nodeStack.pop();
			}
		} while ( !(graphCopy->GetConnectionsFromNode(currentNode).size() == 0 && nodeStack.size() == 0));

		path.push_back(m_pGraph->GetNode(currentNode->GetId()));

		std::reverse(path.begin(), path.end()); // reverses order of the path
		
		return path;
	}


	inline void EulerianPath::VisitAllNodesDFS(const std::vector<GraphNode*>& pNodes, std::vector<bool>& visited, int startIndex ) const
	{
		// mark the visited node
		visited[startIndex] = true;
		// Ask the graph for the connections from that node
		auto connections = m_pGraph->GetConnectionsFromNode(pNodes[startIndex]);

		// recursively visit any connected nodes that were not visited before
		
		for(int i{0}; i< connections.size(); ++i)
		{
			int toNodeId = connections[i]->GetToNodeId();

			// Tip: use an index-based for-loop to find the correct index
			for(int index{0}; index < visited.size(); ++index)
			{
				if(pNodes[index]->GetId() == toNodeId)
				{
					if (visited[index] == false)
					{
						VisitAllNodesDFS(pNodes, visited, index);
					}
					break;
				}
			}
		}
	}

	inline bool EulerianPath::IsConnected() const
	{
		auto nodes = m_pGraph->GetAllNodes();
		if (nodes.size() == 0)
			return false;

		// pick a starting node index
		// start a depth-first-search traversal from that index
		int startingIndex;

		startingIndex = rand() % nodes.size();

		std::vector<bool> visited{};

		visited.resize(nodes.size());

		VisitAllNodesDFS(nodes, visited, startingIndex);

		// if a node was never visited, this graph is not connected

		for(int index{0}; index < visited.size(); ++index)
		{
			if (visited[index] == false) 
			{
				return false; 
			}
		}
		return true;
	}

}