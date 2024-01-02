#pragma once
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/CombinedSteering/CombinedSteeringBehaviors.h"
#include "framework/EliteAI/EliteGraphs/EliteInfluenceMap/EInfluenceMap.h"

enum class Status
{
	Idle,
	Scavenging,
	Collecting,
	Eating,
	Feeding,
	Starving,
	Cleaning,
	Defending,
	Returning
};

class AntBase : public SteeringAgent
{
	

public:
	AntBase(float radius = 1.0f);
	virtual ~AntBase();

	virtual void Update(float deltaTime);

	//public functions
	void EatFood(int food);

	bool IsStomachFull() const;

	bool IsAntDead() const;

	void SetWanderAmount(float wanderPct);
	
	void SetSeekTarget(Elite::Vector2 target);

	//getters and setters
	Status GetCurrentStatus() const { return m_CurrentStatus; } ;
	void SetCurrentStatus(Status status) { m_CurrentStatus = status; };

	int GetCurrentEnergy() const { return m_CurrentEnergy; };

	float GetInteractionRange() const { return m_InteractionRange; };

	float GetInfluencePerSecond() const { return m_CurrentInfluencePerSecond; }
	void SetInfluencePerSecond(float influence) { m_MaxInfluencePerSecond = influence; }

	float GetSampleDistance() const { return m_sampleDistance; }
	void SetSampleDistance(float sampleDistance) { m_sampleDistance = sampleDistance; }

	float GetSampleAngle()const { return m_sampleAngle; }
	void SetSampleAngle(float sampleAngle) { m_sampleAngle = sampleAngle; }

	void SetReadInfluenceMap(Elite::InfluenceMap* pInfluenceMap) { m_pReadInfluenceMap = pInfluenceMap; };
	void SetWriteInfluenceMap(Elite::InfluenceMap* pInfluenceMap) { m_pWriteInfluenceMap = pInfluenceMap; };

protected:
	//base stats
	Status m_CurrentStatus{ Status::Idle };

	const int m_MaxHealth{1000};
	int m_CurrentHealth{1000};

	const float m_TimeMaxBetweenHealthReduction{ 10.f };
	float m_TimeBetweenHealthReduction{};

	const int m_MaxEnergy{1000};
	int m_CurrentEnergy{1000};

	const float m_TimeMaxBetweenEnergyReduction{ 10.f };
	float m_TimeBetweenEnergyReduction{};

	float m_MaxLifeTime{1000};
	float m_TimeSpend{1000};

	const float m_InteractionRange{ 100.f };
	
	//movement
	Wander* m_pWander{ nullptr };
	Seek* m_pSeek{ nullptr };
	BlendedSteering* m_pBlendedSteering{ nullptr };

	//influence
	float m_MaxInfluencePerSecond{ 10.f };
	float m_CurrentInfluencePerSecond{ 10.f };
	float m_sampleDistance{ 10.0f };
	float m_sampleAngle{ 45.f };

	Elite::InfluenceMap* m_pReadInfluenceMap{ nullptr };
	Elite::InfluenceMap* m_pWriteInfluenceMap{ nullptr };

	void WriteToInfluenceMap(float deltaTime);
	void ReadFromInfluenceMap(float deltaTime);
};