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

	Elite::BehaviorState SetWriteMapToNull(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		pAnt->SetWriteInfluenceMap(nullptr);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetReadMapToNull(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		pAnt->SetReadInfluenceMap(nullptr);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetFoodMapAsWriteMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pFoodMap{};
		pBlackboard->GetData("FoodMap", pFoodMap);
		pAnt->SetWriteInfluenceMap(pFoodMap);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetFoodMapAsReadMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pFoodMap{};
		pBlackboard->GetData("FoodMap", pFoodMap);
		pAnt->SetReadInfluenceMap(pFoodMap);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetHungerMapAsWriteMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pHungerMap{};
		pBlackboard->GetData("HungerMap", pHungerMap);
		pAnt->SetWriteInfluenceMap(pHungerMap);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetHungerMapAsReadMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pHungerMap{};
		pBlackboard->GetData("HungerMap", pHungerMap);
		pAnt->SetReadInfluenceMap(pHungerMap);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetHomeMapAsWriteMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pHomeMap{};
		pBlackboard->GetData("HomeMap", pHomeMap);
		pAnt->SetWriteInfluenceMap(pHomeMap);


		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetHomeMapAsReadMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pHomeMap{};
		pBlackboard->GetData("HomeMap", pHomeMap);
		pAnt->SetReadInfluenceMap(pHomeMap);


		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetDeathMapAsWriteMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pDeathMap{};
		pBlackboard->GetData("DeathMap", pDeathMap);
		pAnt->SetWriteInfluenceMap(pDeathMap);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetDeathMapAsReadMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pDeathMap{};
		pBlackboard->GetData("DeathMap", pDeathMap);
		pAnt->SetReadInfluenceMap(pDeathMap);

		return Elite::BehaviorState::Success;
	}

	//todo: add threats
	Elite::BehaviorState SetThreatMapAsWriteMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pThreatMap{};
		pBlackboard->GetData("ThreatMap", pThreatMap);
		pAnt->SetWriteInfluenceMap(pThreatMap);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetThreatMapAsReadMap(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		Elite::InfluenceMap* pThreatMap{};
		pBlackboard->GetData("ThreatMap", pThreatMap);
		pAnt->SetReadInfluenceMap(pThreatMap);

		return Elite::BehaviorState::Success;
	}


#pragma region WorkerAntActions
	Elite::BehaviorState GiveAntRandomJob(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		int randomNumber = rand() % 201;

		if(randomNumber <= 50)
		{
			pWorker->SetCurrentStatus(Status::Cleaning);
		}
		else
		{
			pWorker->SetCurrentStatus(Status::Scavenging);
		}
		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetAntIdle(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		pWorker->SetCurrentStatus(Status::Idle);

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

	Elite::BehaviorState SetDeadAntInRangeAsTargetAnt(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		std::vector<AntBase*> m_pAnts{};
		pBlackboard->GetData("Ants", m_pAnts);

		Elite::Vector2 workerPosition{ worker->GetPosition() };
		float interactionRange{ worker->GetInteractionRange() };

		for (AntBase* ant : m_pAnts)
		{
			if (ant->IsAntDead() && !ant->GetIsBeingCaried())
			{
				float distanceSquared = DistanceSquared(workerPosition, ant->GetPosition());

				if (distanceSquared < (interactionRange * interactionRange))
				{
					pBlackboard->ChangeData("TargetDeadAnt", ant);
					return Elite::BehaviorState::Success;
				}
			}
		}
		return Elite::BehaviorState::Failure;
	}

	Elite::BehaviorState SetGarbageSiteAsTarget(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		Elite::Vector2 garbageSiteLocation{};
		pBlackboard->GetData("GarbageSiteLocation", garbageSiteLocation);

		worker->SetSeekTarget(garbageSiteLocation);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState SetStarvingAntInRangeAsTarget(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		std::vector<AntBase*> m_pAnts{};
		pBlackboard->GetData("Ants", m_pAnts);

		Elite::Vector2 workerPosition{ worker->GetPosition() };
		float interactionRange{ worker->GetInteractionRange() };

		for (AntBase* ant : m_pAnts)
		{
			float distanceSquared = DistanceSquared(workerPosition, ant->GetPosition());

			if (distanceSquared < (interactionRange * interactionRange))
			{
				if (ant->GetCurrentStatus() == Status::Starving)
				{
					pBlackboard->ChangeData("TargetStarvingAnt", ant);
					return Elite::BehaviorState::Success;
				}
			}
		}
		return Elite::BehaviorState::Failure;
	}

	Elite::BehaviorState CollectFood(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		Food* pFood{};
		pBlackboard->GetData("TargetFoodSpot", pFood);
		
		pWorker->SetSeekTarget(pFood->GetPosition());

		pWorker->StoreFoodInSocialStomach(pFood->TakeFood());

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState CollectDeadAnt(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		AntBase* pTargetDeadAnt{};
		pBlackboard->GetData("TargetDeadAnt", pTargetDeadAnt);

		pWorker->SetDeadAnt(pTargetDeadAnt);

		pTargetDeadAnt->SetIsBeingCaried(true);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState DropDeadAnt(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		pWorker->GetDeadAnt()->SetIsBeingCaried(false);

		pWorker->SetDeadAnt(nullptr);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState StartFeedingOtherAnts(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		pWorker->SetCurrentStatus(Status::Feeding);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState FeedStarvingAnt(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* pWorker{};
		pBlackboard->GetData("CurrentWorker", pWorker);

		AntBase* pStarvingAnt{};
		pBlackboard->GetData("TargetStarvingAnt", pStarvingAnt);

		pStarvingAnt->EatFood(pWorker->GiveFoodFromSocialStomach());

		return Elite::BehaviorState::Success;
	}
#pragma endregion


#pragma region SoldierAntActions
	

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

	bool IsAntStarving(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		return pAnt->IsAntStarving();
	}

	bool IsAntDead(Elite::Blackboard* pBlackboard)
	{
		AntBase* pAnt{};
		pBlackboard->GetData("CurrentAnt", pAnt);

		return pAnt->IsAntDead();
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

	bool IsAntFeeding(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (worker->GetCurrentStatus() == Status::Feeding)
		{
			return true;
		}
		return false;
	}

	bool IsAntCleaning(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (worker->GetCurrentStatus() == Status::Cleaning)
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

	bool IsSocialStomachFull(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if (worker->IsSocialStomachFull())
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

	bool IsAntNearDeadAnt(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		std::vector<AntBase*> m_pAnts{};
		pBlackboard->GetData("Ants", m_pAnts);

		Elite::Vector2 workerPosition{ worker->GetPosition() };
		float interactionRange{ worker->GetInteractionRange() };

		for (AntBase* const ant : m_pAnts)
		{
			if(ant->IsAntDead() && !ant->GetIsBeingCaried())
			{
				float distanceSquared = DistanceSquared(workerPosition, ant->GetPosition());

				if (distanceSquared < (interactionRange * interactionRange))
				{
					return true;
				}
			}
		}
		return false;
	}

	bool IsAntNearGarbageSite(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		Elite::Vector2 garbageSiteLocation{};
		pBlackboard->GetData("GarbageSiteLocation", garbageSiteLocation);

		float interactionRange{ worker->GetInteractionRange() };

		float distanceSquared = DistanceSquared(worker->GetPosition(), garbageSiteLocation);

		if (distanceSquared < (interactionRange * interactionRange))
		{
			return true;
		}

		return false;
	}

	bool IsAntHoldingDeadAnt(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		if(worker->GetDeadAnt())
		{
			return true;
		}
		return false;
	}

	bool IsStarvingAntInRange(Elite::Blackboard* pBlackboard)
	{
		WorkerAnt* worker{};
		pBlackboard->GetData("CurrentWorker", worker);

		std::vector<AntBase*> m_pAnts{};
		pBlackboard->GetData("Ants", m_pAnts);

		Elite::Vector2 workerPosition{ worker->GetPosition() };
		float interactionRange{ worker->GetInteractionRange() };

		for (AntBase* ant : m_pAnts)
		{
			float distanceSquared = DistanceSquared(workerPosition, ant->GetPosition());

			if (distanceSquared < (interactionRange * interactionRange))
			{
				if(ant->IsAntStarving())
				{
					return true;
				}
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