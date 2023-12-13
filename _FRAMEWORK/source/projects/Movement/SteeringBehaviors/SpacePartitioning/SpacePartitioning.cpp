#include "stdafx.h"
#include "SpacePartitioning.h"
#include "projects\Movement\SteeringBehaviors\SteeringAgent.h"

using namespace Elite;

// --- Cell ---
// ------------
Cell::Cell(float left, float bottom, float width, float height)
{
	boundingBox.bottomLeft = { left, bottom };
	boundingBox.width = width;
	boundingBox.height = height;
}

std::vector<Elite::Vector2> Cell::GetRectPoints() const
{
	auto left = boundingBox.bottomLeft.x;
	auto bottom = boundingBox.bottomLeft.y;
	auto width = boundingBox.width;
	auto height = boundingBox.height;

	std::vector<Elite::Vector2> rectPoints =
	{
		{ left , bottom  },
		{ left , bottom + height  },
		{ left + width , bottom + height },
		{ left + width , bottom  },
	};

	return rectPoints;
}

// --- Partitioned Space ---
// -------------------------
CellSpace::CellSpace(float width, float height, int rows, int cols, int maxEntities)
	: m_SpaceWidth(width)
	, m_SpaceHeight(height)
	, m_NrOfRows(rows)
	, m_NrOfCols(cols)
	, m_Neighbors(maxEntities)
	, m_NrOfNeighbors(0)
{
	//Calculate bounds of a cell
	m_CellWidth = width / cols;
	m_CellHeight = height / rows;

	//TODO: create the cells
	
	for (int y = 0; y < rows; ++y)
	{
		for(int x = 0; x < cols; ++x)
		{
			int index = (m_NrOfCols * y) + x;
			m_Cells.push_back( Cell
			{ 
				x * m_CellWidth, 
				y * m_CellHeight,
				m_CellWidth,
				m_CellHeight 
			});
		}
	}
}

void CellSpace::AddAgent(SteeringAgent* agent)
{
	//Add the agent to the correct cell
	
	int index = PositionToIndex(agent->GetPosition());
	
	m_Cells[index].agents.push_back(agent);
	

}

void CellSpace::AgentPositionChanged(SteeringAgent* agent, Elite::Vector2 oldPos)
{
	// TODO: Check if the agent needs to be moved to another cell.
	// Use the calculated index for oldPos and currentPos for this
	int oldIndex = PositionToIndex(oldPos);
	int newIndex = PositionToIndex(agent->GetPosition());

	if(oldIndex != newIndex)
	{
		m_Cells[oldIndex].agents.remove(agent);
		m_Cells[newIndex].agents.push_back(agent);
	}
}

void CellSpace::RegisterNeighbors(SteeringAgent* pAgent, float neighborhoodRadius)
{
	//TODO: Register the neighbors for the provided agent
	//Only check the cells that are within the radius of the neighborhood

	Vector2 position = pAgent->GetPosition();
	Vector2 toTarget{};
	float distance{};

	m_NrOfNeighbors = 0;

	Rect neighborhoodRectangle = GetNeighborhoodRectangle(position, neighborhoodRadius);

	for (int index{ 0 }; index < m_Cells.size(); ++index)
	{
		if(IsOverlapping(m_Cells[index].boundingBox, neighborhoodRectangle))
		{
			for (auto const &agent : m_Cells[index].agents)
			{
				if (pAgent != agent)
				{
					toTarget = agent->GetPosition() - position;

					distance = toTarget.Magnitude();

					if (distance < neighborhoodRadius)
					{
						m_Neighbors[m_NrOfNeighbors] = agent;
						++m_NrOfNeighbors;
					}
				}
			}
		}
	}
}

void CellSpace::EmptyCells()
{
	for (Cell& c : m_Cells)
		c.agents.clear();
}

void CellSpace::RenderCells() const
{
	//TODO: Render the cells with the number of agents inside of it
	//TIP: use DEBUGRENDERER2D->DrawPolygon(...) and Cell::GetRectPoints())
	//TIP: use DEBUGRENDERER2D->DrawString(...) 
	for(int index{0}; index < m_Cells.size(); ++index)
	{
		DEBUGRENDERER2D->DrawPolygon(&m_Cells[index].GetRectPoints()[0], 4, {1.f,0.f,0.f}, 0.4f);

		int numAgents = m_Cells[index].agents.size();

		DEBUGRENDERER2D->DrawString(m_Cells[index].GetRectPoints()[1], std::to_string(numAgents).c_str());
	}
	
}

void CellSpace::DebugRender(const SteeringAgent* pAgent, float neighborhoodRadius) const
{
	Vector2 position = pAgent->GetPosition();
	Rect neighborhoodRectangle = GetNeighborhoodRectangle(position, neighborhoodRadius);
	Cell neighborhoodCell
	{
		neighborhoodRectangle.bottomLeft.x,
		neighborhoodRectangle.bottomLeft.y,
		neighborhoodRectangle.width,
		neighborhoodRectangle.height
	};
	
	DEBUGRENDERER2D->DrawPolygon(&neighborhoodCell.GetRectPoints()[0], 4, {1.f,0.f,0.f}, 0.4f);
	
	for (int index{ 0 }; index < m_Cells.size(); ++index)
	{
		if (IsOverlapping(m_Cells[index].boundingBox, neighborhoodRectangle))
		{
			DEBUGRENDERER2D->DrawPolygon(&m_Cells[index].GetRectPoints()[0], 4, { 1.f,1.f,0.f }, 0.1f);
		}
	}


}

int CellSpace::PositionToIndex(const Elite::Vector2 pos) const
{
	//TODO: Calculate the index of the cell based on the position
	int columnNr = int(pos.x / m_CellWidth);
	int rowNr = int(pos.y / m_CellHeight);

	if (columnNr == m_NrOfCols) --columnNr;
	if (rowNr == m_NrOfRows) --rowNr;

	//cel index gaat van links naar rechts en onder naar boven.
	int index = (m_NrOfCols * rowNr) + (columnNr);

	return index;
}

Rect CellSpace::GetNeighborhoodRectangle(const Elite::Vector2 positionCenter, float rediusNeighborhood) const
{
	Rect result{};

	result.height = rediusNeighborhood * 2;
	result.width = rediusNeighborhood * 2;

	result.bottomLeft =
	{
		positionCenter.x - rediusNeighborhood,
		positionCenter.y - rediusNeighborhood,
	};

	return result;
}
