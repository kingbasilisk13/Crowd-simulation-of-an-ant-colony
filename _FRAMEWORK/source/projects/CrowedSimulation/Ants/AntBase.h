#pragma once
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/CombinedSteering/CombinedSteeringBehaviors.h"
#include "framework/EliteAI/EliteGraphs/EliteInfluenceMap/EInfluenceMap.h"
class AntBase : public SteeringAgent
{

public:
	AntBase(float radius = 1.0f);
	virtual ~AntBase();

	virtual void Update(float deltaTime);

	

protected:
	float m_MaxHealth{1000};
	float m_CurrentHealth{1000};

	const float m_TimeMaxBetweenHealthReduction{ 10.f };
	float m_TimeBetweenHealthReduction{};

	float m_MaxEnergy{1000};
	float m_CurrentEnergy{1000};

	const float m_TimeMaxBetweenEnergyReduction{ 10.f };
	float m_TimeBetweenEnergyReduction{};


	float m_MaxLifeTime{1000};
	float m_TimeSpend{1000};


	Wander* m_pWander{ nullptr };
	Seek* m_pSeek{ nullptr };
	BlendedSteering* m_pBlendedSteering{ nullptr };

private:
	

};