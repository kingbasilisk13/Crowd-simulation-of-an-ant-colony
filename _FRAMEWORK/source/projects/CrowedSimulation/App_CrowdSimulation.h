#pragma once

#include "framework/EliteInterfaces/EIApp.h"

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
	const int MAX_AMOUNT_ANTS{ 200 };
	const int AMOUNT_FOOD_ITEMS{ 1 };
	const int FOOD_AMOUNT{ 1000 };
	const float FOOD_DISTANCE{ 150.f };

	float m_WorldSize = 400.f;

	//Influence maps
	//Elite::InfluenceMap* m_pInfluenceMap_Food{ nullptr };
	//Elite::InfluenceMap* m_pInfluenceMap_Home{ nullptr };

	bool m_RenderInfluenceMap_Food{ false };
	bool m_RenderInfluenceMap_Home{ false };

	//Food sources
	//std::vector<FoodSource*> m_pFoodVec{};
	float m_foodRadius{ 20.f };

	//Home parameters
	Elite::Vector2 m_homePosition;
	float m_homeRadius{ 20.f };

	//Ant Agent(s)
	//std::vector<AntAgent*> m_pAnts{};



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

	//base ants
	/*QueenAnt* m_pQueen{ nullptr };
	SoldierAnt* m_pSoldier{ nullptr };
	WorkerAnt* m_pWorker{ nullptr };*/




	//C++ make the class non-copyable
	App_CrowdSimulation(const App_CrowdSimulation&) = delete;
	App_CrowdSimulation& operator=(const App_CrowdSimulation&) = delete;
};

