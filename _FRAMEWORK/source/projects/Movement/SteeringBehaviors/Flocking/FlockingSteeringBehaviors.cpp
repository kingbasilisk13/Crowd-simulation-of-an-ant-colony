#include "stdafx.h"
#include "FlockingSteeringBehaviors.h"
#include "Flock.h"
#include "../SteeringAgent.h"
#include "../SteeringHelpers.h"

using namespace Elite;

//*******************
//COHESION (FLOCKING)
SteeringOutput Cohesion::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	m_Target = m_pFlock->GetAverageNeighborPos();

	SteeringOutput steering = Seek::CalculateSteering(deltaT, pAgent);

	return steering;
}

//*********************
//SEPARATION (FLOCKING)
SteeringOutput Separation::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering{};

	float distanceSqr{};

	Vector2 position = pAgent->GetPosition();
	Vector2 direction{};
	Vector2 totalSteering{};

	int nrOfNeighbors = m_pFlock->GetNrOfNeighbors();
	std::vector<SteeringAgent*> m_Neighbors = m_pFlock->GetNeighbors();


	for(int i{0}; i < nrOfNeighbors; ++i)
	{
		direction = position - m_Neighbors[i]->GetPosition();
		
		distanceSqr = direction.MagnitudeSquared();
		if(distanceSqr != 0)
		{
			totalSteering += direction / distanceSqr;
		}
	}
	totalSteering.Normalize();

	steering.LinearVelocity = totalSteering * pAgent->GetMaxLinearSpeed();

	return steering;
}

//*************************
//VELOCITY MATCH (FLOCKING)
SteeringOutput VelocityMatch::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering{};

	steering.LinearVelocity = m_pFlock->GetAverageNeighborVelocity().GetNormalized() * pAgent->GetMaxLinearSpeed();

	return steering;
}