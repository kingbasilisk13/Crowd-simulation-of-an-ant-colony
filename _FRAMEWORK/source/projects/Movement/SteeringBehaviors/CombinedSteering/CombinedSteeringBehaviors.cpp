#include "stdafx.h"
#include "CombinedSteeringBehaviors.h"
#include <algorithm>
#include "../SteeringAgent.h"

BlendedSteering::BlendedSteering(std::vector<WeightedBehavior> weightedBehaviors)
	:m_WeightedBehaviors(weightedBehaviors)
{
};

//****************
//BLENDED STEERING
SteeringOutput BlendedSteering::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput blendedSteering = {};

	float totalWeight{0.f};

	//Calculate the weighted average steeringbehavior
	for(auto& WeightedBehavior : m_WeightedBehaviors)
	{
		//if(WeightedBehavior.weight)

		auto steering = WeightedBehavior.pBehavior->CalculateSteering(deltaT, pAgent);

		blendedSteering.LinearVelocity += steering.LinearVelocity * WeightedBehavior.weight;
		blendedSteering.AngularVelocity += steering.AngularVelocity * WeightedBehavior.weight;

		totalWeight += WeightedBehavior.weight;
	}

	if(totalWeight > 0.f)
	{
		blendedSteering.LinearVelocity /= totalWeight;
		blendedSteering.AngularVelocity /= totalWeight;
	}
	
	if (pAgent->GetDebugRenderingEnabled())
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), blendedSteering.LinearVelocity, 7, { 0, 1, 1 }, 0.40f);

	return blendedSteering;
}

//*****************
//PRIORITY STEERING
SteeringOutput PrioritySteering::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	for (auto pBehavior : m_PriorityBehaviors)
	{
		steering = pBehavior->CalculateSteering(deltaT, pAgent);

		if (steering.IsValid)
			break;
	}

	//If non of the behavior return a valid output, last behavior is returned
	return steering;
}