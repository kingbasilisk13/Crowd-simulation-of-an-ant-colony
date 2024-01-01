#include "stdafx.h"
#include "AntBase.h"
using namespace Elite;

AntBase::AntBase(float radius)
	:SteeringAgent(radius)
{
	m_pSeek = new Seek();
	m_pWander = new Wander();
	m_pBlendedSteering = new BlendedSteering({ { m_pWander , 0.5f}, { m_pSeek, 0.5f } });
	SteeringAgent::SetSteeringBehavior(m_pBlendedSteering);

	SetAutoOrient(true);
	SetMaxAngularSpeed(180.f);
	SetMaxLinearSpeed(20.f);
	SetMass(0.1f);
	SetBodyColor(Color{ 0.f, 0.f, 1.f });
	SetRotation(Elite::randomFloat(float(M_PI) * 2));

	BaseAgent::m_pRigidBody->RemoveAllShapes();
}

AntBase::~AntBase()
{
	SAFE_DELETE(m_pSeek);
	SAFE_DELETE(m_pWander);
	SAFE_DELETE(m_pBlendedSteering);
}

void AntBase::Update(float deltaTime)
{
	SteeringAgent::Update(deltaTime);

	//lower food
	if (m_CurrentEnergy <= 0)
	{
		m_TimeBetweenHealthReduction += deltaTime;
		if (m_TimeMaxBetweenHealthReduction < m_TimeBetweenHealthReduction)
		{
			m_TimeBetweenHealthReduction = 0.f;
			m_CurrentHealth -= 1.f;
		}
	}
	else
	{
		m_TimeBetweenEnergyReduction += deltaTime;
		if (m_TimeMaxBetweenEnergyReduction < m_TimeBetweenEnergyReduction)
		{
			m_TimeBetweenEnergyReduction = 0.f;
			m_CurrentEnergy -= 1.f;
		}
	}
}

void AntBase::EatFood(float food)
{
	m_CurrentEnergy += food;

	if(m_CurrentEnergy > m_MaxEnergy)
	{
		m_CurrentEnergy = m_MaxEnergy;
	}
}

bool AntBase::IsAntDead() const
{
	if(m_CurrentHealth <= 0.f)
	{
		return true;
	}
	return false;
}

