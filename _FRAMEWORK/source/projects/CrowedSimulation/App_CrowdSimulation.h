#pragma once

#include "framework/EliteInterfaces/EIApp.h"
#include "Ants/AntBase.h"
#include "Ants/QueenAnt.h"
#include "Ants/SoldierAnt.h"
#include "Ants/WorkerAnt.h"
#include "Food/Food.h"

class App_CrowdSimulation final : public IApp
{
public:
	//Constructor & Destructor
	App_CrowdSimulation() = default;
	virtual ~App_CrowdSimulation() final;

	//App Functions
	void Start() override;
	void Update(float deltaTime) override;
	void UpdateUI();
	void Render(float deltaTime) const override;

private:
	bool m_QueenAntIsDead{ false };
	
	const int m_StartAmountOfWorkers{ 50 };
	const int m_StartAmountOfSoldiers{ 20 };
	const int m_AmountOfFoodItems{ 10 };
	const int m_FoodAmount{ 1000 };
	const float m_FoodDistance{ 100.f };

	float m_WorldSize = 400.f;

	Elite::Vector2 m_CenterWorld{};

	//Influence maps
	Elite::InfluenceMap* m_pInfluenceMapFood{ nullptr };
	Elite::InfluenceMap* m_pInfluenceMapHome{ nullptr };
	Elite::InfluenceMap* m_pInfluenceMapHunger{ nullptr };
	Elite::InfluenceMap* m_pInfluenceMapDeath{ nullptr };
	Elite::InfluenceMap* m_pInfluenceMapThreat{ nullptr };

	bool m_RenderInfluenceMapFood{ false };
	bool m_RenderInfluenceMapHome{ false };
	bool m_RenderInfluenceMapHunger{ false };
	bool m_RenderInfluenceMapDeath{ false };
	bool m_RenderInfluenceMapThreat{ false };
	

	//Food sources
	std::vector<Food*> m_pFoodVec{};
	float m_foodRadius{ 20.f };

	//garbage site
	Elite::Vector2 m_GarbageSiteLocation{ 20,20 };
	
	//Ant parameters
	float m_AntWanderPct{ 0.1f };
	float m_AntSampleDist{ 10.f };
	float m_AntSampleAngle{ 45.f };
	float m_InfluencePerSecond{ 10.f };
	
	bool m_RenderAntInteractionRange{ false };
	bool m_RenderAntName{ false };

	//ants
	std::vector<AntBase*> m_pAnts{};

	//behavior tree
	Elite::Blackboard* m_pBlackboard{ nullptr };
	Elite::BehaviorTree* m_pBehaviorTree{ nullptr };

	//all functions needed to create the behavior tree
	void CreateBlackboard();
	void CreateBehaviorTree();

	Elite::BehaviorSequence* CreateDeadAntSequence();
	Elite::BehaviorSequence* CreateWorkerAntSequence();
	Elite::BehaviorSequence* CreateSoldierAntSequence();
	Elite::BehaviorSequence* CreateQueenAntSequence();
	
	//C++ make the class non-copyable
	App_CrowdSimulation(const App_CrowdSimulation&) = delete;
	App_CrowdSimulation& operator=(const App_CrowdSimulation&) = delete;
};

