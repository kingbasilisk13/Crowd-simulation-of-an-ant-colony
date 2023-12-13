//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
using namespace Elite;

//Includes
#include "App_FSM.h"
#include "projects/Shared/NavigationColliderElement.h"
#include "framework/EliteAI/EliteGraphs/EliteGraphUtilities/EGraphRenderer.h"

#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "projects/Movement/SteeringBehaviors/PathFollow/PathFollowSteeringBehavior.h"

#include "framework/EliteAI/EliteGraphs/EliteGraphAlgorithms/ENavGraphPathfinding.h"
#include "framework/EliteAI/EliteGraphs/EliteNavGraph/ENavGraph.h"

//Statics
bool App_FSM::sShowPolygon = false;
bool App_FSM::sDrawPlayerPath = false;
bool App_FSM::sDrawPatrolPath = true;
bool App_FSM::sDrawDetectionRadius = true;

//Destructor
App_FSM::~App_FSM()
{
	for (auto pNC : m_vNavigationColliders)
		SAFE_DELETE(pNC);
	m_vNavigationColliders.clear();

	SAFE_DELETE(m_pNavGraph);
	SAFE_DELETE(m_pPlayerPathFollow);
	SAFE_DELETE(m_pGuardPathFollow);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pGuard);

	//clean up self made members
	SAFE_DELETE(m_pPatrolState);
	SAFE_DELETE(m_pChaseState);
	SAFE_DELETE(m_pSearchState);

	SAFE_DELETE(m_pIsSearchingTooLongTransition);
	SAFE_DELETE(m_pIsTargetNotVisibleTransition);
	SAFE_DELETE(m_pIsTargetVisibleTransition);
}

//Functions
void App_FSM::Start()
{
	//Initialization of your application. 
	//----------- CAMERA ------------
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(36.782f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(12.9361f, 0.2661f));

	//----------- WORLD ------------
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(15.f, 0.f), 3.0f, 15.0f));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(-15.f, 0.f), 3.0f, 15.0f));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(0.f, 15.f), 15.0f, 3.0f));
	m_vNavigationColliders.push_back(new NavigationColliderElement(Elite::Vector2(0.f, -15.f), 15.0f, 3.0f));

	//----------- NAVMESH  ------------

	const auto& shapes = PHYSICSWORLD->GetAllStaticShapesInWorld(PhysicsFlags::NavigationCollider);
	m_pNavGraph = new Elite::NavGraph(shapes, 120,60, m_AgentRadius);

	//----------- PLAYER AGENT ------------
	m_pPlayerPathFollow = new PathFollow();
	m_Target = TargetData(Elite::ZeroVector2);
	m_pPlayer = new SteeringAgent(m_AgentRadius);
	m_pPlayer->SetPosition(Elite::Vector2{ -20.f, -20.f });
	m_pPlayer->SetSteeringBehavior(m_pPlayerPathFollow);
	m_pPlayer->SetMaxLinearSpeed(m_PlayerSpeed);
	m_pPlayer->SetAutoOrient(true);
	m_pPlayer->SetMass(0.1f);

	//----------- PATROL PATH  ------------
	m_PatrolPath = { 
		Elite::Vector2(20.f, 20.f), 
		Elite::Vector2(-20.f, 20.f), 
		Elite::Vector2(-20.f, -20.f),
		Elite::Vector2(20.f, -20.f) 
	};

	//----------- GUARD (AI) AGENT ------------
	m_pGuardPathFollow = new PathFollow();
	m_pGuard = new SmartAgent(m_AgentRadius);
	m_pGuard->SetPosition(Elite::Vector2{ 20.f, 0.f });
	m_pGuard->SetSteeringBehavior(m_pGuardPathFollow);
	m_pGuard->SetMaxLinearSpeed(m_GuardSpeed);
	m_pGuard->SetAutoOrient(true);
	m_pGuard->SetMass(0.1f);

	

	//----------- AI SETUP ------------
	// FSM TODO:
	//1. Create and add the necessary blackboard data
	Blackboard* pBlackboard = CreateBlackboard(m_pGuard);

	//2. Create the different agent states -> store in members, clean up!!
	m_pPatrolState = new FSMStates::Patrol();
	m_pChaseState = new FSMStates::Chase();
	m_pSearchState = new FSMStates::Search();

	//3. Create the transitions beetween those states -> store in members, clean up!!
	m_pIsTargetVisibleTransition = new FSMConditions::IsTargetVisible();
	m_pIsTargetNotVisibleTransition = new FSMConditions::IsTargetNotVisible();
	m_pIsSearchingTooLongTransition = new FSMConditions::IsSearchingTooLong();


	//4. Create the finite state machine with a starting state and the blackboard
	FiniteStateMachine* stateMachine = new FiniteStateMachine(m_pPatrolState, pBlackboard);

	//5. Add the transitions for the states to the state machine
	// stateMachine->AddTransition(startState, toState, condition)
	// startState: active state for which the transition will be checked
	// condition: if the Evaluate function returns true => transition will fire and move to the toState
	// toState: end state where the agent will move to if the transition fires
	
	stateMachine->AddTransition(m_pPatrolState, m_pChaseState, m_pIsTargetVisibleTransition);
	stateMachine->AddTransition(m_pChaseState, m_pSearchState, m_pIsTargetNotVisibleTransition);
	stateMachine->AddTransition(m_pSearchState, m_pChaseState, m_pIsTargetVisibleTransition);
	stateMachine->AddTransition(m_pSearchState, m_pPatrolState, m_pIsSearchingTooLongTransition);

	//6. Activate the decision making stucture on the custom agent by calling the SetDecisionMaking function
	m_pGuard->SetDecisionMaking(stateMachine);
}

void App_FSM::Update(float deltaTime)
{
	//Update target/path based on input
	if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eLeft))
	{
		auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		Elite::Vector2 mouseTarget = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld(
			Elite::Vector2((float)mouseData.X, (float)mouseData.Y));
		m_vPath = NavMeshPathfinding::FindPath(m_pPlayer->GetPosition(), mouseTarget, m_pNavGraph);

		//Check if a path exist and move to the following point
		if (m_vPath.size() > 0)
		{
			m_pPlayerPathFollow->SetPath(m_vPath);
		}
	}
	
	UpdateImGui();
	m_pPlayer->Update(deltaTime);
	m_pGuard->Update(deltaTime);
}

void App_FSM::Render(float deltaTime) const
{
	if (sShowPolygon)
	{
		DEBUGRENDERER2D->DrawPolygon(m_pNavGraph->GetNavMeshPolygon(),
			Color(0.1f, 0.1f, 0.1f));
		DEBUGRENDERER2D->DrawSolidPolygon(m_pNavGraph->GetNavMeshPolygon(),
			Color(0.0f, 0.5f, 0.1f, 0.05f), DEBUGRENDERER2D->NextDepthSlice());
	}

	if (sDrawPlayerPath && m_vPath.size() > 0)
	{
		for (auto pathPoint : m_vPath)
			DEBUGRENDERER2D->DrawCircle(pathPoint, 2.0f, Color(1.f, 0.f, 0.f), -0.2f);
	
		//DEBUGRENDERER2D->DrawSegment(m_pAgent->GetPosition(), m_vPath[0], Color(1.f, 0.0f, 0.0f), -0.2f);
		for (size_t i = 0; i < m_vPath.size() - 1; i++)
		{
			float g = float(i) / m_vPath.size();
			DEBUGRENDERER2D->DrawSegment(m_vPath[i], m_vPath[i+1], Color(1.f, g, g), -0.2f);
		}	
	}

	if (sDrawPatrolPath && !m_PatrolPath.empty())
	{
		for (auto pathPoint : m_PatrolPath)
			DEBUGRENDERER2D->DrawCircle(pathPoint, 2.0f, Color(0.f, 0.f, 1.f), -0.2f);

		//DEBUGRENDERER2D->DrawSegment(m_pAgent->GetPosition(), m_vPath[0], Color(1.f, 0.0f, 0.0f), -0.2f);
		for (size_t i = 0; i < m_PatrolPath.size(); i++)
		{
			DEBUGRENDERER2D->DrawSegment(m_PatrolPath[i], m_PatrolPath[(i + 1) % m_PatrolPath.size()], Color(0.f, 0.f, 1.f), -0.2f);
		}
	}

	if (sDrawDetectionRadius)
	{
		DEBUGRENDERER2D->DrawCircle(m_pGuard->GetPosition(), m_DetectionRadius, Color(0.f, 0.f, 1.f), -0.2f);
	}

	m_pPlayer->Render(deltaTime);
	m_pGuard->Render(deltaTime);
}

Elite::Blackboard* App_FSM::CreateBlackboard(SmartAgent* smartAgent)
{
	Blackboard* pBlackboard = new Blackboard();
	pBlackboard->AddData("Agent", smartAgent);
	// FSM TODO:
	//...
	pBlackboard->AddData("PatrolPath", m_PatrolPath);

	pBlackboard->AddData("Player", m_pPlayer);

	pBlackboard->AddData("DetectionRadius", m_DetectionRadius);

	pBlackboard->AddData("MaxSearchTime", m_MaxSearchTime);

	pBlackboard->AddData("PatrolPathIndex", 0);

	pBlackboard->AddData("SearchTime", 0.f);

	pBlackboard->AddData("GuardPathFollow", m_pGuardPathFollow);

	pBlackboard->AddData("GuardStartWandering", false);

	Vector2 playerPosition{};
	pBlackboard->AddData("PlayerLastPosition", playerPosition);
	
	return pBlackboard;
}

void App_FSM::UpdateImGui()
{
	//------- UI --------
#ifdef PLATFORM_WINDOWS
#pragma region UI
	{
		//Setup
		int menuWidth = 150;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 90));
		ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		ImGui::PushAllowKeyboardFocus(false);
		ImGui::SetWindowFocus();
		ImGui::PushItemWidth(70);
		//Elements
		ImGui::Text("CONTROLS");
		ImGui::Indent();
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
		ImGui::Spacing();

		ImGui::Checkbox("Show Polygon", &sShowPolygon);
		ImGui::Checkbox("Show Player Path", &sDrawPlayerPath);
		ImGui::Checkbox("Show Patrol Path", &sDrawPatrolPath);
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::SliderFloat("PlayerSpeed", &m_PlayerSpeed, 0.0f, 22.0f))
		{
			m_pPlayer->SetMaxLinearSpeed(m_PlayerSpeed);
		}

		if (ImGui::SliderFloat("GuardSpeed", &m_GuardSpeed, 0.0f, 22.0f))
		{
			m_pGuard->SetMaxLinearSpeed(m_GuardSpeed);
		}
		
		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
#pragma endregion
#endif
}