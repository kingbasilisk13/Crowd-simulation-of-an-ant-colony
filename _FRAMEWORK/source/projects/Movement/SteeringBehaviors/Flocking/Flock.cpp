#include "stdafx.h"
#include "Flock.h"

#include "../SteeringAgent.h"
#include "../Steering/SteeringBehaviors.h"
#include "../CombinedSteering/CombinedSteeringBehaviors.h"
#include "../SpacePartitioning/SpacePartitioning.h"

using namespace Elite;
//Constructor & Destructor
Flock::Flock(
	int flockSize /*= 50*/, 
	float worldSize /*= 100.f*/, 
	SteeringAgent* pAgentToEvade /*= nullptr*/, 
	bool trimWorld /*= false*/)

	: m_WorldSize{ worldSize }
	, m_FlockSize{ flockSize }
	, m_TrimWorld { trimWorld }
	, m_pAgentToEvade{pAgentToEvade}
	, m_NeighborhoodRadius{ 15 }
	, m_NrOfNeighbors{0}
{
	m_pCellSpace = new CellSpace(worldSize, worldSize, 10, 10, flockSize);

	//TODO: initialize the flock and the memory pool
	m_Agents.resize(m_FlockSize);

	m_OldPositions.resize(m_FlockSize);

	m_Neighbors.resize(m_FlockSize - 1);

	m_pSeparationBehavior = new Separation(this);
	m_pCohesionBehavior = new Cohesion(this);
	m_pVelMatchBehavior = new VelocityMatch(this);
	m_pSeekBehavior = new Seek();
	m_pWanderBehavior = new Wander();


	m_pBlendedSteering = new BlendedSteering({
		BlendedSteering::WeightedBehavior{m_pSeparationBehavior, 0.2f},
		BlendedSteering::WeightedBehavior{m_pCohesionBehavior, 0.2f},
		BlendedSteering::WeightedBehavior{m_pVelMatchBehavior, 0.2f},
		BlendedSteering::WeightedBehavior{m_pSeekBehavior, 0.2f},
		BlendedSteering::WeightedBehavior{m_pWanderBehavior, 0.2f}
		});

	m_pEvadeBehavior = new Evade();

	m_pPrioritySteering = new PrioritySteering({
		m_pEvadeBehavior,
		m_pBlendedSteering
		});

	for(int i{0}; i < flockSize; ++i)
	{
		m_Agents[i] = new SteeringAgent();
		m_Agents[i]->SetPosition(randomVector2(worldSize));
		m_Agents[i]->TrimToWorld(m_WorldSize);
		m_Agents[i]->SetSteeringBehavior(m_pPrioritySteering);

		m_Agents[i]->SetMaxLinearSpeed(100.f);
		m_Agents[i]->SetAutoOrient(true);

		m_pCellSpace->AddAgent(m_Agents[i]);

		m_OldPositions[i] = m_Agents[i]->GetPosition();
	}
}

Flock::~Flock()
{
	//TODO: Cleanup any additional data

	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pPrioritySteering);

	SAFE_DELETE(m_pSeparationBehavior);
	SAFE_DELETE(m_pCohesionBehavior);
	SAFE_DELETE(m_pVelMatchBehavior);
	SAFE_DELETE(m_pSeekBehavior);
	SAFE_DELETE(m_pWanderBehavior);

	SAFE_DELETE(m_pEvadeBehavior);

	SAFE_DELETE(m_pCellSpace);

	for(auto pAgent: m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();
	m_Neighbors.clear();
}

void Flock::Update(float deltaT)
{
	// TODO: update the flock
	// for every agent:
		// register the neighbors for this agent (-> fill the memory pool with the neighbors for the currently evaluated agent)
		// update the agent (-> the steeringbehaviors use the neighbors in the memory pool)
		// trim the agent to the world

	TargetData evadeTarget{};
	evadeTarget.LinearVelocity = m_pAgentToEvade->GetLinearVelocity();
	evadeTarget.Position = m_pAgentToEvade->GetPosition();

	m_pEvadeBehavior->SetTarget(evadeTarget);

	for (int i{ 0 }; i < m_FlockSize; ++i)
	{
		RegisterNeighbors(m_Agents[i]);
		
		m_Agents[i]->Update(deltaT);
		m_Agents[i]->TrimToWorld(m_WorldSize);

		m_pCellSpace->AgentPositionChanged(m_Agents[i], m_OldPositions[i]);

		m_OldPositions[i] = m_Agents[i]->GetPosition();
	}
	
}

void Flock::Render(float deltaT)
{
	// TODO: Render all the agents in the flock
	
	for (int i{ 0 }; i < m_FlockSize; ++i)
	{
		m_Agents[i]->Render(deltaT);
	}

	if (m_DebugRenderNeighborhood) //TODO: switch with imGUI checkbox
		RenderNeighborhood();

	m_Agents[0]->SetDebugRenderingEnabled(m_DebugRenderSteering);

	if(m_DebugRenderPartitions)
	{
		m_pCellSpace->RenderCells();
	}

}

void Flock::UpdateAndRenderUI()
{
	//Setup
	int menuWidth = 235;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);

	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Text("LMB: place target");
	ImGui::Text("RMB: move cam.");
	ImGui::Text("Scrollwheel: zoom cam.");
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Flocking");
	ImGui::Spacing();

	//TODO: implement ImGUI checkboxes for debug rendering here
	ImGui::Checkbox("Rendering Steering", &m_DebugRenderSteering);
	ImGui::Checkbox("Rendering Neighborhood", &m_DebugRenderNeighborhood);
	ImGui::Checkbox("Rendering Partitions", &m_DebugRenderPartitions);
	ImGui::Checkbox("Use Space Partitions", &m_UseSpacePartitions);

	ImGui::Text("Behavior Weights");
	ImGui::Spacing();

	//TODO: implement ImGUI sliders for steering behavior weights here
	ImGui::SliderFloat("Separation", &m_pBlendedSteering->GetWeightedBehaviorsRef()[0].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Cohesion", &m_pBlendedSteering->GetWeightedBehaviorsRef()[1].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Velocity Match", &m_pBlendedSteering->GetWeightedBehaviorsRef()[2].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Seek", &m_pBlendedSteering->GetWeightedBehaviorsRef()[3].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Wander", &m_pBlendedSteering->GetWeightedBehaviorsRef()[4].weight, 0.f, 1.f, "%.2");

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
	
}

void Flock::RenderNeighborhood()
{
	// TODO: Implement
	// Register the neighbors for the first agent in the flock
	// DebugRender the neighbors in the memory pool

	RegisterNeighbors(m_Agents[0]);

	DEBUGRENDERER2D->DrawCircle(m_Agents[0]->GetPosition(), m_NeighborhoodRadius, {1.f,1.f,1.f}, DEBUGRENDERER2D->NextDepthSlice());

	for (int i{ 0 }; i < GetNrOfNeighbors(); ++i)
	{
		DEBUGRENDERER2D->DrawSolidCircle(GetNeighbors()[i]->GetPosition(), GetNeighbors()[i]->GetRadius(), { 0.f,0.f }, { 0.f,1.f,0.f }, -0.8f);
	}

	m_pCellSpace->DebugRender(m_Agents[0], m_NeighborhoodRadius);
	
}


void Flock::RegisterNeighbors(SteeringAgent* pAgent)
{
	// TODO: Implement

	if(m_UseSpacePartitions)
	{
		m_pCellSpace->RegisterNeighbors(pAgent, m_NeighborhoodRadius);
	}
	else
	{
		m_NrOfNeighbors = 0;

		Vector2 position = pAgent->GetPosition();
		Vector2 toTarget{};
		float distance{};

		for (int i{ 0 }; i < m_FlockSize; ++i)
		{
			if (pAgent != m_Agents[i])
			{
				toTarget = m_Agents[i]->GetPosition() - position;
				distance = toTarget.Magnitude();

				if (distance < m_NeighborhoodRadius)
				{
					m_Neighbors[m_NrOfNeighbors] = m_Agents[i];
					++m_NrOfNeighbors;
				}
			}
		}
	}
}

int Flock::GetNrOfNeighbors() const 
{ 
	if(m_UseSpacePartitions)
	{
		return m_pCellSpace->GetNrOfNeighbors();
	}

	return m_NrOfNeighbors;
}

const std::vector<SteeringAgent*>& Flock::GetNeighbors() const 
{ 
	if (m_UseSpacePartitions)
	{
		return m_pCellSpace->GetNeighbors();
	}

	return m_Neighbors;
}

Vector2 Flock::GetAverageNeighborPos() const
{
	Vector2 avgPosition = Elite::ZeroVector2;

	int nrOfNeighbors = GetNrOfNeighbors();

	if (nrOfNeighbors != 0)
	{
		// TODO: Implement
		for (int i{ 0 }; i < nrOfNeighbors; ++i)
		{
			avgPosition += GetNeighbors()[i]->GetPosition();
		}

		avgPosition /= float(nrOfNeighbors);
	}

	return avgPosition;
}

Vector2 Flock::GetAverageNeighborVelocity() const
{
	Vector2 avgVelocity = Elite::ZeroVector2;
	int nrOfNeighbors = GetNrOfNeighbors();

	if(nrOfNeighbors != 0)
	{
		// TODO: Implement
		for (int i{ 0 }; i < nrOfNeighbors; ++i)
		{
			avgVelocity += GetNeighbors()[i]->GetLinearVelocity();
		}

		avgVelocity /= float(nrOfNeighbors);
	}

	return avgVelocity;
}
void Flock::SetTarget_Seek(const TargetData& target)
{
	// TODO: Implement
	m_pSeekBehavior->SetTarget(target);
}

float* Flock::GetWeight(ISteeringBehavior* pBehavior) 
{
	if (m_pBlendedSteering)
	{
		auto& weightedBehaviors = m_pBlendedSteering->GetWeightedBehaviorsRef();
		auto it = find_if(weightedBehaviors.begin(),
			weightedBehaviors.end(),
			[pBehavior](BlendedSteering::WeightedBehavior el)
			{
				return el.pBehavior == pBehavior;
			}
		);

		if(it!= weightedBehaviors.end())
			return &it->weight;
	}

	return nullptr;
}
