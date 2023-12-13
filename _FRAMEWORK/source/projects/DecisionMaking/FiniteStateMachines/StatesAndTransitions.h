/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// StatesAndTransitions.h: Implementation of the state/transition classes
/*=============================================================================*/
#ifndef ELITE_APPLICATION_FSM_STATES_TRANSITIONS
#define ELITE_APPLICATION_FSM_STATES_TRANSITIONS

#include "projects/DecisionMaking/SmartAgent.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework/EliteAI/EliteData/EBlackboard.h"

using namespace Elite;
// FSM TODO:

class PathFollow;

//------------
//---STATES---
//------------
namespace FSMStates 
{
	class Patrol : public FSMState
	{

	public:
		Patrol() = default;
		~Patrol() = default;

		void OnEnter(Blackboard*) override;
		void OnExit(Blackboard*) override;
		void Update(Blackboard*, float) override;
	};

	class Chase : public FSMState
	{

	public:
		Chase() = default;
		~Chase() = default;

		void OnEnter(Blackboard*) override;
		void OnExit(Blackboard*) override;
		void Update(Blackboard*, float) override;
	};

	class Search : public FSMState
	{

	public:
		Search() = default;
		~Search() = default;

		void OnEnter(Blackboard*) override;
		void OnExit(Blackboard*) override;
		void Update(Blackboard*, float) override;
	};
}



//-----------------
//---CONDITIONS---
//-----------------
namespace FSMConditions
{
	class IsTargetVisible : public FSMCondition
	{

	public:
		IsTargetVisible() = default;
		~IsTargetVisible() = default;

		bool Evaluate(Blackboard* pBlackboard) const override;
	};

	class IsTargetNotVisible : public FSMCondition
	{

	public:
		IsTargetNotVisible() = default;
		~IsTargetNotVisible() = default;

		bool Evaluate(Blackboard* pBlackboard) const override;
	};

	class IsSearchingTooLong : public FSMCondition
	{

	public:
		IsSearchingTooLong() = default;
		~IsSearchingTooLong() = default;

		bool Evaluate(Blackboard* pBlackboard) const override;
	};
}
#endif