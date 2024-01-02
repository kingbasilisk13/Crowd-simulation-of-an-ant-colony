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
	Elite::BehaviorState ReturnSuccess(Elite::Blackboard* pBlackboard)
	{
		return Elite::BehaviorState::Success;
	}
#pragma region WorkerAntActions
	Elite::BehaviorState StartScavengingForFood(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		pWorker->SetCurrentStatus(Status::Scavenging);

		Elite::InfluenceMap* pFoodMap{};
		pBlackboard->GetData("FoodMap", pFoodMap);
		pWorker->SetReadInfluenceMap(pFoodMap);


		Elite::InfluenceMap* pHomeMap{};
		pBlackboard->GetData("HomeMap", pFoodMap);
		pWorker->SetWriteInfluenceMap(pHomeMap);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetFoodInRangeAsTarget(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		std::vector<Food*> pFoodVec{};
		pBlackboard->GetData("FoodSpots", pFoodVec);

		Elite::Vector2 workerPosition{ worker->GetPosition() };
		float interactionRange{ worker->GetInteractionRange() };

		for (Food* food : pFoodVec)
		{
			float distanceSquared = DistanceSquared(workerPosition, food->GetPosition());

			if (distanceSquared < (interactionRange * interactionRange))
			{
				pBlackboard->ChangeData("TargetFoodSpot", food);
				worker->SetSeekTarget(food->GetPosition());
				return Elite::BehaviorState::Success;
			}
		}

		return Elite::BehaviorState::Failure;
	}

	Elite::BehaviorState StartCollectingFood(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		pWorker->SetCurrentStatus(Status::Collecting);

		Elite::InfluenceMap* pFoodMap{};
		pBlackboard->GetData("FoodMap", pFoodMap);
		pWorker->SetWriteInfluenceMap(pFoodMap);

		pWorker->SetReadInfluenceMap(nullptr);

		return Elite::BehaviorState::Success;
	}


	Elite::BehaviorState CollectFood(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		Food* pFood{};
		pBlackboard->GetData("TargetFoodSpot", pFood);
		
		pWorker->StoreFoodInSocialStomach(pFood->TakeFood());

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState EatFood(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		Food* pFood{};
		pBlackboard->GetData("TargetFoodSpot", pFood);

		pWorker->EatFood(pFood->GetAmount());

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState StartGoingHome(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		pWorker->SetCurrentStatus(Status::Returning);

		Elite::InfluenceMap* pFoodMap{};
		pBlackboard->GetData("FoodMap", pFoodMap);
		pWorker->SetWriteInfluenceMap(pFoodMap);

		Elite::InfluenceMap* pHomeMap{};
		pBlackboard->GetData("HomeMap", pFoodMap);
		pWorker->SetReadInfluenceMap(pHomeMap);

		return Elite::BehaviorState::Success;
	}

#pragma endregion


#pragma region SoldierAntActions
	Elite::BehaviorState ThisIsASoldier(Elite::Blackboard* pBlackboard)
	{
		//std::cout << "This is a soldier\n";
		return Elite::BehaviorState::Success;
	}

#pragma endregion


#pragma region QueenAntActions
	Elite::BehaviorState SpawnBrood(Elite::Blackboard* pBlackboard)
	{
		AntBase* pNewAnt{};

		//1/4 chanse to spawn soldier. 3/4 chanse for worker.
		int randomNumber = rand() % 101;

		if (randomNumber < 26)
		{
			pNewAnt = new SoldierAnt();
		}
		else
		{
			pNewAnt = new WorkerAnt();
		}

		QueenAnt* pAntQueen{};
		pBlackboard->GetData("Queen", pAntQueen);

		pNewAnt->SetPosition(pAntQueen->GetPosition());

		std::vector<AntBase*> ants{};
		pBlackboard->GetData("Ants", ants);

		ants.push_back(pNewAnt);
		pBlackboard->ChangeData("Ants", ants);

		return Elite::BehaviorState::Success;
	}
#pragma endregion
}

namespace BT_Conditions
{
	bool ReturnTrue(Elite::Blackboard* pBlackboard)
	{
		return true;
	}

#pragma region WorkerAntConditions
	bool IsThisAWorkerAnt(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		if (typeid(WorkerAnt) == typeid(*pAnt))
		{
			WorkerAnt* pWorker = dynamic_cast<WorkerAnt*>(pAnt);
			pBlackboard->ChangeData("CurrentWorker", pWorker);
			return true;
		}
		return false;
	}

	bool IsAntIdle(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (worker->GetCurrentStatus() == Status::Idle)
		{
			return true;
		}
		return false;
	}

	bool IsAntScavenging(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (worker->GetCurrentStatus() == Status::Scavenging)
		{
			return true;
		}
		return false;
	}

	bool IsAntCollecting(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (worker->GetCurrentStatus() == Status::Collecting)
		{
			return true;
		}
		return false;
	}

	bool IsFoodSpotEmpty(Elite::Blackboard* pBlackboard)
	{
		Food* pFood{};
		pBlackboard->GetData("TargetFoodSpot", pFood);

		if (pFood->GetAmount() == 0)
		{
			return true;
		}

		return false;
	}

	bool AreBothStomachsFull(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (worker->IsSocialStomachFull() && worker->IsStomachFull())
		{
			return true;
		}

		return false;
	}

	bool IsSocialStomachEmpty(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if(worker->GetSocialStomach() == 0)
		{
			return true;
		}

		return false;
	}

	bool IsSocialStomachNotFull(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (!worker->IsSocialStomachFull())
		{
			return true;
		}

		return false;
	}

	bool IsStomachEmpty(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (worker->GetCurrentEnergy() == 0)
		{
			return true;
		}

		return false;
	}

	bool IsStomachNotFull(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (!worker->IsStomachFull())
		{
			return true;
		}

		return false;
	}

	bool IsAntNearFood(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		std::vector<Food*> pFoodVec{};
		pBlackboard->GetData("FoodSpots", pFoodVec);

		Elite::Vector2 workerPosition{ worker->GetPosition() };
		float interactionRange{ worker->GetInteractionRange()};

		for(const Food* food : pFoodVec)
		{
			float distanceSquared = DistanceSquared(workerPosition, food->GetPosition());

			if(distanceSquared < (interactionRange * interactionRange))
			{
				return true;
			}
		}
		return false;
	}

	

#pragma endregion


#pragma region SoldierAntConditions
	bool IsThisASoldierAnt(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		if (typeid(SoldierAnt) == typeid(*pAnt))
		{
			SoldierAnt* pSoldier = dynamic_cast<SoldierAnt*>(pAnt);
			pBlackboard->ChangeData("CurrentSoldier", pSoldier);
			return true;
		}
		return false;
	}
#pragma endregion


#pragma region QueenAntConditions
	bool CanQueenSpawnBrood(Elite::Blackboard* pBlackboard)
	{
		QueenAnt* pQueen{};
		pBlackboard->GetData("Queen", pQueen);

		if (pQueen->SpawnBrood())
		{
			return true;
		}
		return false;
	}
#pragma endregion
}