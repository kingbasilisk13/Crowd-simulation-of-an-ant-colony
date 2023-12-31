#pragma once
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteInterfaces/EIApp.h"
//#include "AntTypes/AntBase.h"
//#include "AntTypes/QueenAnt.h"
//#include "AntTypes/SoldierAnt.h"
//#include "AntTypes/WorkerAnt.h"

//--- Includes and Forward Declarations ---

namespace Elite
{
	class InfluenceMap;	
}

class AntAgent;
class FoodSource;
//-----------------------------------------------------------------
// Application
//-----------------------------------------------------------------
class App_InfluenceMap final : public IApp
{
public:
	//Constructor & Destructor
	App_InfluenceMap() = default;
	virtual ~App_InfluenceMap() final;

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
	Elite::InfluenceMap* m_pInfluenceMap_Food{ nullptr };
	Elite::InfluenceMap* m_pInfluenceMap_Home{ nullptr };

	bool m_RenderInfluenceMap_Food{ false };
	bool m_RenderInfluenceMap_Home{ false };

	//Food sources
	std::vector<FoodSource*> m_pFoodVec{};
	float m_foodRadius{ 20.f };

	//Home parameters
	Elite::Vector2 m_homePosition;
	float m_homeRadius{ 20.f };
	
	//Ant Agent(s)
	std::vector<AntAgent*> m_pAnts{};

	

	//Ant parameters
	float m_AntSpawnInterval{ 0.5f };

	float m_AntSpawnTimer{ 0.f };

	float m_AntWanderPct{ 0.1f };
	float m_AntSampleDist{ 10.f };
	float m_AntSampleAngle{ 45.f };
	float m_InfluencePerSecond{ 10.f };
	bool m_RenderAntDebug{ true };

	void CheckTakeFoodSources(AntAgent* pAgent);
	void CheckDropFood(AntAgent* pAgent);

	//base ants
	/*QueenAnt* m_pQueen{ nullptr };
	SoldierAnt* m_pSoldier{ nullptr };
	WorkerAnt* m_pWorker{ nullptr };*/




	//C++ make the class non-copyable
	App_InfluenceMap(const App_InfluenceMap&) = delete;
	App_InfluenceMap& operator=(const App_InfluenceMap&) = delete;
};
