#include "stdafx.h"
#include "SandboxAgent.h"

using namespace Elite;

SandboxAgent::SandboxAgent(): BaseAgent()
{
	m_Target = GetPosition();
}

void SandboxAgent::Update(float dt)
{
	const float speed = 20.f;
	//TODO: set linear velocity towards m_Target

	Vector2 direction = m_Target - GetPosition();
	direction.Normalize();
	Vector2 velocity = direction * speed;
	SetLinearVelocity(direction * speed);

	//DEBUGRENDERER2D->DrawSegment(GetPosition(), m_Target, {1.f,0.f,0.f});
	DEBUGRENDERER2D->DrawDirection(GetPosition(), velocity, speed, { 0.f, 1.f, 0.f });

	//Orientation
	AutoOrient();
}

void SandboxAgent::Render(float dt)
{
	BaseAgent::Render(dt); //Default Agent Rendering
}

void SandboxAgent::AutoOrient()
{
	//Determine angle based on direction
	Vector2 velocity = GetLinearVelocity();
	if (velocity.MagnitudeSquared() > 0)
	{
		SetRotation(VectorToOrientation(velocity));
	}
}