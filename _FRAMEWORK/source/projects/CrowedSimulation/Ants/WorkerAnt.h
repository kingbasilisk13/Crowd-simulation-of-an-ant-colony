#pragma once
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/CombinedSteering/CombinedSteeringBehaviors.h"
#include "framework/EliteAI/EliteGraphs/EliteInfluenceMap/EInfluenceMap.h"
#include "AntBase.h"

class WorkerAnt final : public AntBase
{
public:
	WorkerAnt();
	~WorkerAnt();

	virtual void Update(float deltaTime);

	void StoreFoodInSocialStomach(int food);
	int GiveFoodFromSocialStomach();

private:
	const int m_MaxSizeSocialStomach{ 200 };
	int m_SocialStomach{ 0 };

	bool m_HoldingGarbage{ false };
};

