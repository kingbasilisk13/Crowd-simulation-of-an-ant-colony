#pragma once
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteInterfaces/EIApp.h"
#include "projects/DecisionMaking/SmartAgent.h"
#include "projects/Movement/SteeringBehaviors/SteeringHelpers.h"
#include "framework/EliteAI/EliteGraphs/EliteGraphAlgorithms/EPathSmoothing.h"
#include "projects/DecisionMaking/FiniteStateMachines/StatesAndTransitions.h"

//Forward declarations
class NavigationColliderElement;
class SteeringAgent;
class PathFollow;

namespace Elite
{
	class NavGraph;
}


//-----------------------------------------------------------------
// Application
//-----------------------------------------------------------------
class App_FSM final : public IApp
{
public:
	//Constructor & Destructor
	App_FSM() = default;
	virtual ~App_FSM();

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) const override;

private:
	Elite::Blackboard* CreateBlackboard(SmartAgent* smartAgent);

	//Datamembers
	// --AI--
	// FSM TODO:
	FSMStates::Patrol* m_pPatrolState;
	FSMStates::Chase* m_pChaseState;
	FSMStates::Search* m_pSearchState;

	FSMConditions::IsSearchingTooLong* m_pIsSearchingTooLongTransition;
	FSMConditions::IsTargetNotVisible* m_pIsTargetNotVisibleTransition;
	FSMConditions::IsTargetVisible* m_pIsTargetVisibleTransition;

	// --Agents--
	SteeringAgent* m_pPlayer = nullptr;
	SmartAgent* m_pGuard = nullptr;
	PathFollow* m_pPlayerPathFollow = nullptr;
	PathFollow* m_pGuardPathFollow = nullptr;
	TargetData m_Target = {};
	float m_AgentRadius = 1.0f;
	float m_PlayerSpeed = 16.0f;
	float m_GuardSpeed = 8.0f;
	float m_DetectionRadius = 10.0f;
	float m_MaxSearchTime = 5.0f;

	// --Level--
	std::vector<NavigationColliderElement*> m_vNavigationColliders = {};

	// --Pathfinder--
	std::vector<Elite::Vector2> m_vPath;
	std::vector<Elite::Vector2> m_PatrolPath;

	// --Graph--
	Elite::NavGraph* m_pNavGraph = nullptr;

	// --Debug drawing information--
	static bool sShowPolygon;
	static bool sDrawPlayerPath;
	static bool sDrawPatrolPath;
	static bool sDrawDetectionRadius;

	void UpdateImGui();
private:
	//C++ make the class non-copyable
	App_FSM(const App_FSM&) = delete;
	App_FSM& operator=(const App_FSM&) = delete;
};
