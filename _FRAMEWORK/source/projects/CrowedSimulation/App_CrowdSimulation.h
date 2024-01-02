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
	const int m_AmountOfFoodItems{ 3 };
	const int m_FoodAmount{ 1000 };
	const float m_FoodDistance{ 150.f };

	float m_WorldSize = 400.f;

	//Influence maps
	Elite::InfluenceMap* m_pInfluenceMapFood{ nullptr };
	Elite::InfluenceMap* m_pInfluenceMapHome{ nullptr };

	bool m_RenderInfluenceMapFood{ false };
	bool m_RenderInfluenceMapHome{ false };

	//Food sources
	std::vector<Food*> m_pFoodVec{};
	float m_foodRadius{ 20.f };

	//Home parameters
	Elite::Vector2 m_homePosition;
	float m_homeRadius{ 20.f };

	
	//Ant parameters
	float m_AntSpawnInterval{ 0.5f };

	float m_AntSpawnTimer{ 0.f };

	float m_AntWanderPct{ 0.1f };
	float m_AntSampleDist{ 10.f };
	float m_AntSampleAngle{ 45.f };
	float m_InfluencePerSecond{ 10.f };
	bool m_RenderAntDebug{ true };

	/*void CheckTakeFoodSources(AntAgent* pAgent);
	void CheckDropFood(AntAgent* pAgent);*/

	//ants
	std::vector<AntBase*> m_pAnts{};


	Elite::Blackboard* m_pBlackboard{ nullptr };
	Elite::BehaviorTree* m_pBehaviorTree{ nullptr };

	void CreateBlackboard();

	//all functions needed to create the behavior tree
	void CreateBehaviorTree();

	Elite::BehaviorSequence* CreateWorkerAntSequence();
	Elite::BehaviorSequence* CreateSoldierAntSequence();
	Elite::BehaviorSequence* CreateQueenAntSequence();
	
	//C++ make the class non-copyable
	App_CrowdSimulation(const App_CrowdSimulation&) = delete;
	App_CrowdSimulation& operator=(const App_CrowdSimulation&) = delete;
};

