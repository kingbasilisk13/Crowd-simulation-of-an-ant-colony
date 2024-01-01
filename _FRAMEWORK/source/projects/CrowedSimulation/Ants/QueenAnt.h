#pragma once
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/CombinedSteering/CombinedSteeringBehaviors.h"
#include "framework/EliteAI/EliteGraphs/EliteInfluenceMap/EInfluenceMap.h"
#include "AntBase.h"

class QueenAnt final : public AntBase 
{
public:
	QueenAnt();
	~QueenAnt();

	virtual void Update(float deltaTime);

	bool SpawnBrood();

private:
	const float m_MaxTimeBetweenSpawnBrood{ 30.f };
	
	std::chrono::steady_clock::time_point m_LastTimePoinSpawnBrood{};

};

