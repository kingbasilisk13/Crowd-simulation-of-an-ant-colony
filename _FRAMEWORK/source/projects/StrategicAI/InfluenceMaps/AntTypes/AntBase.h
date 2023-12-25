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

	float m_MaxHunger{1000};
	float m_CurrentHunger{1000};

	float m_MaxLifeTime{1000};
	float m_TimeSpend{1000};


	Wander* m_pWander{ nullptr };
	Seek* m_pSeek{ nullptr };
	BlendedSteering* m_pBlendedSteering{ nullptr };

private:
	

};