#pragma once
#include "projects/Movement/SteeringBehaviors/SteeringAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/CombinedSteering/CombinedSteeringBehaviors.h"
#include "framework/EliteAI/EliteGraphs/EliteInfluenceMap/EInfluenceMap.h"

//todo: should i keep this or is this basicly making a finite state machine out of a behavior tree?

enum class Status
{
	Idle,
	Scavenging,
	Feeding,
	Starving,
	Cleaning,
	Defending,
	Attacked
};

class AntBase : public SteeringAgent
{
	

public:
	AntBase(float radius = 1.0f);
	virtual ~AntBase();

	virtual void Update(float deltaTime);
	void Render(float dt, bool renderInteractionRange = false, bool renderName = false);

	//public functions
	void EatFood(int food);

	bool IsStomachFull() const;

	bool IsAntDead();

	void SetWanderAmount(float wanderPct);
	
	void SetSeekTarget(Elite::Vector2 target);

	void TakeDamage();

	bool IsAntStarving() const;
	//getters and setters
	Status GetCurrentStatus() const { return m_CurrentStatus; } ;
	void SetCurrentStatus(Status status) { m_CurrentStatus = status; };

	AntBase* GetDeadAnt() const { return m_pDeadAnt; };
	void SetDeadAnt(AntBase* pDeadAnt) { m_pDeadAnt = pDeadAnt; };

	bool GetIsBeingCaried() const { return m_IsBeingCaried; };
	void SetIsBeingCaried(bool caried) { m_IsBeingCaried = caried; };

	int GetCurrentEnergy() const { return m_CurrentEnergy; };
	int GetMaxEnergy() const { return m_MaxEnergy; };

	float GetInteractionRange() const { return m_InteractionRange; };

	float GetInfluencePerSecond() const { return m_InfluencePerSecond; }
	void SetInfluencePerSecond(float influence) { m_InfluencePerSecond = influence; }

	float GetSampleDistance() const { return m_sampleDistance; }
	void SetSampleDistance(float sampleDistance) { m_sampleDistance = sampleDistance; }

	float GetSampleAngle()const { return m_sampleAngle; }
	void SetSampleAngle(float sampleAngle) { m_sampleAngle = sampleAngle; }

	void SetReadInfluenceMap(Elite::InfluenceMap* pInfluenceMap) { m_pReadInfluenceMap = pInfluenceMap; };
	void SetWriteInfluenceMap(Elite::InfluenceMap* pInfluenceMap) { m_pWriteInfluenceMap = pInfluenceMap; };

protected:
	//base stats
	std::string m_Name{ "default" };
	
	Status m_CurrentStatus{ Status::Idle };

	bool m_IsAntDead{ false };

	AntBase* m_pDeadAnt{ nullptr };
	bool m_IsBeingCaried{ false };
	
	int m_MaxHealth{200};
	int m_CurrentHealth{200};

	const float m_TimeMaxBetweenHealthReduction{ 10.f };
	float m_TimeBetweenHealthReduction{};

	int m_MaxEnergy{200};
	int m_CurrentEnergy{200};

	const float m_TimeMaxBetweenEnergyReduction{ 10.f };
	float m_TimeBetweenEnergyReduction{};

	float m_MaxLifeTime{1000};
	float m_Age{0};

	const float m_InteractionRange{ 10.f };
	
	//movement
	Wander* m_pWander{ nullptr };
	Seek* m_pSeek{ nullptr };
	BlendedSteering* m_pBlendedSteering{ nullptr };

	//influence
	float m_InfluencePerSecond{ 10.f };
	float m_sampleDistance{ 10.0f };
	float m_sampleAngle{ 45.f };

	Elite::InfluenceMap* m_pReadInfluenceMap{ nullptr };
	Elite::InfluenceMap* m_pWriteInfluenceMap{ nullptr };

	void WriteToInfluenceMap(float deltaTime);
	void ReadFromInfluenceMap(float deltaTime);
};