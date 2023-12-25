#pragma once
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/CombinedSteering/CombinedSteeringBehaviors.h"
#include "framework/EliteAI/EliteGraphs/EliteInfluenceMap/EInfluenceMap.h"
#include "projects/StrategicAI/InfluenceMaps/AntTypes/AntBase.h"

class QueenAnt final : public AntBase 
{
public:
	QueenAnt();
	~QueenAnt();

	virtual void Update(float deltaTime);

private:

};

