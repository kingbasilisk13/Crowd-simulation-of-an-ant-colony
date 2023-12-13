#include "stdafx.h"
#include "StatesAndTransitions.h"

#include "projects/Shared/NavigationColliderElement.h"
#include "projects/Movement/SteeringBehaviors/PathFollow/PathFollowSteeringBehavior.h"
// FSM TODO:

using namespace FSMStates;
using namespace FSMConditions;

//------------
//---STATES---
//------------

//
//
//PATROL
//
//
void Patrol::OnEnter(Blackboard* pBlackboard)
{
	std::vector<Elite::Vector2> patrolPath;
	pBlackboard->GetData("PatrolPath", patrolPath);

	SmartAgent* pAgent = nullptr;
	pBlackboard->GetData("Agent", pAgent);

	PathFollow* pGuardPathFollow = nullptr;
	pBlackboard->GetData("GuardPathFollow", pGuardPathFollow);

	int pathIndex{ 0 };
	pBlackboard->GetData("PatrolPathIndex", pathIndex);

	pGuardPathFollow->SetCurrentPathIndex(pathIndex);
	
	pGuardPathFollow->SetPath(patrolPath);

	pAgent->SetSteeringBehavior(pGuardPathFollow);
}

void Patrol::OnExit(Blackboard* pBlackboard)
{
	SmartAgent* pAgent = nullptr;
	pBlackboard->GetData("Agent", pAgent);

	PathFollow* pGuardPathFollow = pAgent->GetSteeringBehavior()->As<PathFollow>();
	
	pBlackboard->ChangeData("PatrolPathIndex", pGuardPathFollow->GetCurrentPathIndex());
}

void Patrol::Update(Blackboard* pBlackboard, float deltaTime)
{
	SmartAgent* pAgent = nullptr;
	pBlackboard->GetData("Agent", pAgent);

	//As allows you to cast the returned steering behavior to a sub class of it.
	PathFollow* pGuardPathFollow = pAgent->GetSteeringBehavior()->As<PathFollow>();
	
	if(pGuardPathFollow->HasArrived())
	{
		std::vector<Elite::Vector2> patrolPath;
		pBlackboard->GetData("PatrolPath", patrolPath);

		pGuardPathFollow->SetPath(patrolPath);
		pAgent->SetSteeringBehavior(pGuardPathFollow);
	}
}

//
// 
//CHASE
//
//
void Chase::OnEnter(Blackboard* pBlackboard)
{
	SmartAgent* pAgent = nullptr;
	pBlackboard->GetData("Agent", pAgent);

	SteeringAgent* pPlayer = nullptr;
	pBlackboard->GetData("Player", pPlayer);

	Seek* pSeek = pAgent->GetSeekBehavior();

	pSeek->SetTarget(pPlayer->GetPosition());

	pAgent->SetSteeringBehavior(pSeek);
}

void Chase::OnExit(Blackboard* pBlackboard)
{
	SteeringAgent* pPlayer = nullptr;
	pBlackboard->GetData("Player", pPlayer);

	pBlackboard->ChangeData("PlayerLastPosition", pPlayer->GetPosition());
}

void Chase::Update(Blackboard* pBlackboard, float deltaTime)
{
	SmartAgent* pAgent = nullptr;
	pBlackboard->GetData("Agent", pAgent);

	SteeringAgent* pPlayer = nullptr;
	pBlackboard->GetData("Player", pPlayer);

	Seek* pSeek = pAgent->GetSteeringBehavior()->As<Seek>();

	pSeek->SetTarget(pPlayer->GetPosition());

	pAgent->SetSteeringBehavior(pSeek);
}

//
// 
//SEARCH
//
//
void Search::OnEnter(Blackboard* pBlackboard)
{
	pBlackboard->ChangeData("GuardStartWandering", false);

	SmartAgent* pAgent = nullptr;
	pBlackboard->GetData("Agent", pAgent);

	Vector2 playerLastPosition{};
	pBlackboard->GetData("PlayerLastPosition", playerLastPosition);

	Seek* pSeek = pAgent->GetSteeringBehavior()->As<Seek>();

	pSeek->SetTarget(playerLastPosition);

	pAgent->SetSteeringBehavior(pSeek);
}

void Search::OnExit(Blackboard* pBlackboard)
{
	pBlackboard->ChangeData("SearchTime", 0.f);
}

void Search::Update(Blackboard* pBlackboard, float deltaTime)
{
	SmartAgent* pAgent = nullptr;
	pBlackboard->GetData("Agent", pAgent);

	bool startWandering;
	pBlackboard->GetData("GuardStartWandering", startWandering);

	if(!startWandering)
	{
		Vector2 playerLastPosition{};
		pBlackboard->GetData("PlayerLastPosition", playerLastPosition);
		float distanceBetweenAgentAndLastPoint = playerLastPosition.Distance(pAgent->GetPosition());

		if (distanceBetweenAgentAndLastPoint <= 0.1f)
		{
			pBlackboard->ChangeData("GuardStartWandering", true);
			Wander* pWander = pAgent->GetWanderBehavior();

			pAgent->SetSteeringBehavior(pWander);
		}
	}
	else
	{
		float searchTime{ 0 };
		pBlackboard->GetData("SearchTime", searchTime);
		searchTime += deltaTime;
		pBlackboard->ChangeData("SearchTime", searchTime);
	}
}


//-----------------
//---CONDITIONS---
//-----------------

bool IsTargetVisible::Evaluate(Blackboard* pBlackboard) const
{
	SmartAgent* pAgent = nullptr;
	pBlackboard->GetData("Agent", pAgent);

	SteeringAgent* pPlayer = nullptr;
	pBlackboard->GetData("Player", pPlayer);
	
	float detectionRadius = 0.f;
	pBlackboard->GetData("DetectionRadius", detectionRadius);

	float distance = Vector2{ pPlayer->GetPosition() - pAgent->GetPosition() }.Magnitude();
	
	if(distance < detectionRadius)
	{
		return pAgent->HasLineOfSight(pPlayer->GetPosition());
	}

	return false;
}

bool IsTargetNotVisible::Evaluate(Blackboard* pBlackboard) const
{
	SmartAgent* pAgent = nullptr;
	pBlackboard->GetData("Agent", pAgent);

	SteeringAgent* pPlayer = nullptr;
	pBlackboard->GetData("Player", pPlayer);

	float detectionRadius = 0.f;
	pBlackboard->GetData("DetectionRadius", detectionRadius);

	float distance = Vector2{ pPlayer->GetPosition() - pAgent->GetPosition() }.Magnitude();

	if (distance < detectionRadius)
	{
		return !pAgent->HasLineOfSight(pPlayer->GetPosition());
	}

	return true;
}

bool IsSearchingTooLong::Evaluate(Blackboard* pBlackboard) const
{
	float maxSearchTime{ 0 };
	pBlackboard->GetData("MaxSearchTime", maxSearchTime);

	float searchTime{ 0 };
	pBlackboard->GetData("SearchTime", searchTime);

	if(searchTime >= maxSearchTime)
	{
		return true;
	}

	return false;
}
