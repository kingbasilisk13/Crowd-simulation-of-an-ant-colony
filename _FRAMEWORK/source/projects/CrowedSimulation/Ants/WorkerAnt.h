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

	//public functions
	bool IsSocialStomachFull() const;

	void StoreFoodInSocialStomach(int food);

	int GiveFoodFromSocialStomach();

	//getters and setters
	int GetMaxSizeSocialStomach() const { return m_MaxSizeSocialStomach; };

	int GetSocialStomach() const { return m_SocialStomach; };

private:
	const int m_MaxSizeSocialStomach{ 50 };
	int m_SocialStomach{ 0 };

	bool m_HoldingGarbage{ false };
};

