/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// Behaviors.h: Implementation of certain reusable behaviors for the BT version of the Agario Game
/*=============================================================================*/
#pragma once

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "framework/EliteMath/EMath.h"
#include "framework/EliteAI/EliteDecisionMaking/EliteBehaviorTree/EBehaviorTree.h"
#include "projects/DecisionMaking/SmartAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "projects/Movement/SteeringBehaviors/PathFollow/PathFollowSteeringBehavior.h"

//-----------------------------------------------------------------
// Behaviors
//-----------------------------------------------------------------
// BT TODO:

namespace BT_Actions 
{
	Elite::BehaviorState ReturnTrue(Elite::Blackboard* pBlackboard)
	{
		return Elite::BehaviorState::Success;
	}

	

	Elite::BehaviorState ThisIsASoldier(Elite::Blackboard* pBlackboard)
	{
		std::cout << "This is a soldier\n";
		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState ThisIsAWorker(Elite::Blackboard* pBlackboard)
	{
		std::cout << "This is a worker\n";
		return Elite::BehaviorState::Success;
	}

#pragma region WorkerAntActions

#pragma endregion


#pragma region SoldierAntActions

#pragma endregion


#pragma region QueenAntActions
	Elite::BehaviorState ThisIsTheQueen(Elite::Blackboard* pBlackboard)
	{
		std::cout << "This is the queen\n";
		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SpawnBrood(Elite::Blackboard* pBlackboard)
	{
		AntBase* newAnt{};

		//1/4 chanse to spawn soldier. 3/4 chanse for worker.
		int randomNumber = rand() % 101;

		if (randomNumber < 26)
		{
			newAnt = new SoldierAnt();
		}
		else
		{
			newAnt = new WorkerAnt();
		}

		AntBase* antQueen{};
		pBlackboard->GetData("CurrentAnt", antQueen);

		newAnt->SetPosition(antQueen->GetPosition());

		std::vector<AntBase*> ants{};
		pBlackboard->GetData("Ants", ants);

		ants.push_back(newAnt);
		pBlackboard->ChangeData("Ants", ants);

		return Elite::BehaviorState::Success;
	}
#pragma endregion

	Elite::BehaviorState Chase(Elite::Blackboard* pBlackboard)
	{
		SmartAgent* pAgent;
		pBlackboard->GetData("Agent", pAgent);

		SteeringAgent* pPlayer;
		pBlackboard->GetData("TargetAgent", pPlayer);

		assert(pAgent && "Agent was not set in the blackboard.");
		assert(pPlayer && "Player was not set in the blackboard.");

		Seek* pSeek = pAgent->GetSeekBehavior();

		pSeek->SetTarget(pPlayer->GetPosition());

		pAgent->SetSteeringBehavior(pSeek);

		pBlackboard->ChangeData("ReachedLastKnownLocation", false);

		pBlackboard->ChangeData("PlayersLastKnownLocation", pPlayer->GetPosition());

		pBlackboard->ChangeData("ShouldSearch", true);

		return Elite::BehaviorState::Success;
	}


	Elite::BehaviorState Patrol(Elite::Blackboard* pBlackboard)
	{
		SmartAgent* pAgent;
		std::vector<Elite::Vector2> patrolPath;
		PathFollow* pPathFollow;

		pBlackboard->GetData("Agent", pAgent);
		pBlackboard->GetData("PatrolPath", patrolPath);
		pBlackboard->GetData("PathFollow", pPathFollow);

		assert(pAgent && "Agent was not set in the blackboard.");
		assert(pPathFollow && "PathFollow was not set in the blackboard.");
		
		pAgent->SetSteeringBehavior(pPathFollow);
		if(pPathFollow->HasArrived())
		{
			pPathFollow->SetPath(patrolPath);
		}

		return Elite::BehaviorState::Success;
	}

	

	Elite::BehaviorState Search(Elite::Blackboard* pBlackboard)
	{
		SmartAgent* pAgent{};
		pBlackboard->GetData("Agent", pAgent);

		assert(pAgent && "Agent was not set in the blackboard.");

		bool reachedLastLocation{};
		pBlackboard->GetData("ReachedLastKnownLocation", reachedLastLocation);

		if(!reachedLastLocation)
		{
			Elite::Vector2 playerLastPosition{};
			pBlackboard->GetData("PlayersLastKnownLocation", playerLastPosition);

			float distanceBetweenAgentAndLastPoint = playerLastPosition.Distance(pAgent->GetPosition());

			if (distanceBetweenAgentAndLastPoint <= 0.1f)
			{
				pBlackboard->ChangeData("ReachedLastKnownLocation", true);

				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
				pBlackboard->ChangeData("TimeStartSearching", begin);

				return Elite::BehaviorState::Success;
			}

			Seek* pSeek = pAgent->GetSeekBehavior();

			pSeek->SetTarget(playerLastPosition);

			pAgent->SetSteeringBehavior(pSeek);
		}
		else
		{
			Wander* pWander = pAgent->GetWanderBehavior();

			pAgent->SetSteeringBehavior(pWander);
		}

		return Elite::BehaviorState::Success;
	}
}

namespace BT_Conditions
{
	

	

#pragma region WorkerAntConditions
	bool IsThisAWorkerAnt(Elite::Blackboard* pBlackboard)
	{
		AntBase* ant{};
		pBlackboard->GetData("CurrentAnt", ant);

		if (typeid(WorkerAnt) == typeid(*ant))
		{
			return true;
		}
		return false;
	}

#pragma endregion


#pragma region SoldierAntConditions
	bool IsThisASoldierAnt(Elite::Blackboard* pBlackboard)
	{
		AntBase* ant{};
		pBlackboard->GetData("CurrentAnt", ant);

		if (typeid(SoldierAnt) == typeid(*ant))
		{
			return true;
		}
		return false;
	}
#pragma endregion


#pragma region QueenAntConditions
	bool CanQueenSpawnBrood(Elite::Blackboard* pBlackboard)
	{
		AntBase* ant{};
		pBlackboard->GetData("CurrentAnt", ant);

		QueenAnt* queen = dynamic_cast<QueenAnt*>(ant);

		if (queen->SpawnBrood())
		{
			return true;
		}
		return false;
	}
#pragma endregion


	bool IsTargetVisible(Elite::Blackboard* pBlackboard)
	{
		SmartAgent* pAgent;
		pBlackboard->GetData("Agent", pAgent);

		SteeringAgent* pPlayer;
		pBlackboard->GetData("TargetAgent", pPlayer);

		float detectionRadius;
		pBlackboard->GetData("DetectRadius", detectionRadius);
		
		assert(pAgent && "Agent was not set in blackboard");
		assert(pPlayer && "Player was not set in blackboard");
		assert(detectionRadius && "DetectionRadius was not set in blackboard");

		bool isInDetectionRadius = Elite::DistanceSquared(pAgent->GetPosition(), pPlayer->GetPosition()) < (detectionRadius * detectionRadius);
		bool isInLightOfSight = pAgent->HasLineOfSight(pPlayer->GetPosition());

		return isInDetectionRadius && isInLightOfSight;
	}



	bool ShouldSearch(Elite::Blackboard* pBlackboard)
	{
		bool didAgentSpotTargetRecently{};
		pBlackboard->GetData("ShouldSearch", didAgentSpotTargetRecently);
		return didAgentSpotTargetRecently;
	}



	bool IsSearchingToLong(Elite::Blackboard* pBlackboard)
	{
		bool reachedLastLocation{};
		pBlackboard->GetData("ReachedLastKnownLocation", reachedLastLocation);

		float maxSearchTime{ 0 };
		pBlackboard->GetData("MaxSearchTime", maxSearchTime);

		if(reachedLastLocation)
		{
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point begin{};
			pBlackboard->GetData("TimeStartSearching", begin);

			if(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() > maxSearchTime)
			{
				pBlackboard->ChangeData("ReachedLastKnownLocation", false);
				pBlackboard->ChangeData("ShouldSearch", false);

				std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
				pBlackboard->ChangeData("TimeStartSearching", begin);

				return false;
			}
		}
		return true;
	}
}