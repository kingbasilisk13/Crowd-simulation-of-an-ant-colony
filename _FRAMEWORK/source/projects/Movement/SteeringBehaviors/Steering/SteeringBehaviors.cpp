//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"
#include <limits>

using namespace Elite;

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	Vector2 position = pAgent->GetPosition();
	Vector2 direction = m_Target.Position - position;
	direction.Normalize();

	steering.LinearVelocity = direction * pAgent->GetMaxLinearSpeed();

	if(pAgent->GetDebugRenderingEnabled())
	{
		DEBUGRENDERER2D->DrawSegment(position, position + steering.LinearVelocity, {1.f,0.f,0.f});
	}

	return steering;
}

//FLEE
//****
SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	//SteeringOutput steering = Seek::CalculateSteering(deltaT, pAgent);
	//steering.LinearVelocity *= -1;

	SteeringOutput steering = {};
	Vector2 position = pAgent->GetPosition();
	Vector2 direction = m_Target.Position - position;
	direction.Normalize();

	steering.LinearVelocity = -direction * pAgent->GetMaxLinearSpeed();

	if (pAgent->GetDebugRenderingEnabled())
	{
		DEBUGRENDERER2D->DrawSegment(position, position + steering.LinearVelocity, { 1.f,0.f,0.f });
	}

	return steering;
}

//ARRIVE
//****
SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	Vector2 toTarget = m_Target.Position - pAgent->GetPosition();
	float distance = toTarget.Magnitude();

	float a = distance - m_TargetRadius;
	float b = m_SlowRadius - m_TargetRadius;

	float ratio = a / b;

	//clamp from zero to one range
	ratio = Elite::Clamp(ratio, 0.f, 1.f);

	/*if (ratio > 1) 
	{
		ratio = 1;
	}
	else if(ratio < 0)
	{
		ratio = 0;
	}*/

	steering.LinearVelocity = toTarget.GetNormalized() * pAgent->GetMaxLinearSpeed() * ratio;

	if(pAgent->GetDebugRenderingEnabled())
	{
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), m_TargetRadius, { 1.f,0.f,0.f }, DEBUGRENDERER2D->NextDepthSlice());
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), m_SlowRadius, { 0.f,0.f,1.f }, DEBUGRENDERER2D->NextDepthSlice());

	}

	return steering;
}

//PERSUIT
//****
SteeringOutput Persuit::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	Vector2 position = pAgent->GetPosition();
	Vector2 toTarget = m_Target.Position - position;
	float distance = toTarget.Magnitude();

	float time = distance / pAgent->GetMaxLinearSpeed();

	Vector2 newTarget = m_Target.Position + (m_Target.LinearVelocity * time);

	Vector2 direction = newTarget - position;
	direction.Normalize();

	steering.LinearVelocity = direction * pAgent->GetMaxLinearSpeed();

	if (pAgent->GetDebugRenderingEnabled())
	{
		DEBUGRENDERER2D->DrawSolidCircle(newTarget, 0.3f, { 0.f,0.f }, { 128 / 255.f,0.f,128 / 255.f }, -0.8f);
		DEBUGRENDERER2D->DrawSegment(position, position + steering.LinearVelocity, { 0.f,1.f,0.f });
	}

	return steering;
}

//EVADE
//****
SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	Vector2 position = pAgent->GetPosition();
	Vector2 toTarget = m_Target.Position - position;
	float distanceSq = toTarget.MagnitudeSquared();

	if(distanceSq > m_evadeRadius* m_evadeRadius)
	{
		if (pAgent->GetDebugRenderingEnabled())
		{
			DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), m_evadeRadius, { 1.f,0.f,0.f },
				DEBUGRENDERER2D->NextDepthSlice());
		}
		
		SteeringOutput result{};
		result.IsValid = false;

		return result;
	}

	float distance = sqrtf(distanceSq);
	float time = distance / pAgent->GetMaxLinearSpeed();

	Vector2 newTarget = m_Target.Position + (m_Target.LinearVelocity * time);

	Vector2 direction = newTarget - position;

	direction.Normalize();

	steering.LinearVelocity = -direction * pAgent->GetMaxLinearSpeed();

	if (pAgent->GetDebugRenderingEnabled())
	{
		DEBUGRENDERER2D->DrawSolidCircle(newTarget, 0.3f, { 0.f,0.f }, { 128 / 255.f,0.f,128 / 255.f }, -0.8f);
		DEBUGRENDERER2D->DrawSegment(position, position + steering.LinearVelocity, { 0.f,1.f,0.f });
	}

	return steering;
}

//WANDER
//****
SteeringOutput Wander::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	Vector2 position = pAgent->GetPosition();
	
	Vector2 center{};
	center.x = position.x + (pAgent->GetDirection().x * m_OffsetDistance);
	center.y = position.y + (pAgent->GetDirection().y * m_OffsetDistance);
	
	Vector2 target{};

	int maxAngle{ int(ToDegrees(m_MaxAngleChange)) };

	m_WanderAngle += ToRadians(rand() % (maxAngle*2) - maxAngle);

	target.x = center.x + (m_Radius * cos(m_WanderAngle));
	target.y = center.y + (m_Radius * sin(m_WanderAngle));

	m_Target = target;

	SteeringOutput steering = Seek::CalculateSteering(deltaT, pAgent);

	if (pAgent->GetDebugRenderingEnabled())
	{
		DEBUGRENDERER2D->DrawCircle(center, m_Radius, { 0.f,0.f,1.f }, DEBUGRENDERER2D->NextDepthSlice());

		DEBUGRENDERER2D->DrawSegment(position, target, { 0.f,1.f,0.f });
	}

	return steering;
}