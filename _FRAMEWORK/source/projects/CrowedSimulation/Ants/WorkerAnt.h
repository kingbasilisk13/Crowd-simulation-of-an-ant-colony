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

	bool GetIsHoldingGarbage() const { return m_IsHoldingGarbage; };
	void SetIsHoldingGarbage(bool value) { m_IsHoldingGarbage = value; };

	AntBase* GetDeadAnt() const { return m_pDeadAnt; };
	void SetDeadAnt(AntBase* pDeadAnt) { m_pDeadAnt = pDeadAnt; };

private:
	const int m_MaxSizeSocialStomach{ 50 };
	int m_SocialStomach{ 0 };

	AntBase* m_pDeadAnt{ nullptr };

	bool m_IsHoldingGarbage{ false };
};

