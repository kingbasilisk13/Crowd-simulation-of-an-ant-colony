#include "stdafx.h"
#include "AntBase.h"

using namespace Elite;

AntBase::AntBase(float radius)
	:SteeringAgent(radius)
{
	m_pSeek = new Seek();
	m_pWander = new Wander();
	m_pBlendedSteering = new BlendedSteering({ { m_pWander , 0.1f}, { m_pSeek, 0.9f } });
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
	//lower food
	if (m_CurrentEnergy <= 0)
	{
		m_TimeBetweenHealthReduction += deltaTime;
		if (m_TimeMaxBetweenHealthReduction < m_TimeBetweenHealthReduction)
		{
			m_TimeBetweenHealthReduction = 0.f;
			m_CurrentHealth -= 1;
		}
	}
	else
	{
		m_TimeBetweenEnergyReduction += deltaTime;
		if (m_TimeMaxBetweenEnergyReduction < m_TimeBetweenEnergyReduction)
		{
			m_TimeBetweenEnergyReduction = 0.f;
			m_CurrentEnergy -= 1;
		}
	}

	WriteToInfluenceMap(deltaTime);
	ReadFromInfluenceMap(deltaTime);

	SteeringAgent::Update(deltaTime);
}

void AntBase::Render(float dt)
{
	SteeringAgent::Render(dt);
	DEBUGRENDERER2D->DrawCircle(SteeringAgent::GetPosition(), m_InteractionRange, { 1.f, 0.f, 0.f }, 0.5f);
}

void AntBase::EatFood(int food)
{
	m_CurrentEnergy += food;
}

bool AntBase::IsStomachFull() const
{
	if (m_CurrentEnergy >= m_MaxEnergy)
	{
		return true;
	}
	return false;
}

bool AntBase::IsAntDead() const
{
	if(m_CurrentHealth <= 0.f)
	{
		return true;
	}
	return false;
}

void AntBase::SetWanderAmount(float wanderPct)
{
	wanderPct = Clamp(wanderPct, 0.f, 1.f);
	auto& weightsRef = m_pBlendedSteering->GetWeightedBehaviorsRef();
	weightsRef[0].weight = wanderPct;
	weightsRef[1].weight = 1.f - wanderPct;
}

void AntBase::SetSeekTarget(Elite::Vector2 target)
{
	m_pSeek->SetTarget(target);
}

void AntBase::WriteToInfluenceMap(float deltaTime)
{
	if(!m_pWriteInfluenceMap)
	{
		return;
	}

	Vector2 pos = GetPosition();
	m_pWriteInfluenceMap->SetInfluenceAtPosition(pos, (m_CurrentInfluencePerSecond * deltaTime), true);
}

void AntBase::ReadFromInfluenceMap(float deltaTime)
{
	if (!m_pReadInfluenceMap)
	{
		return;
	}

	Vector2 pos = GetPosition();
	m_CurrentInfluencePerSecond = m_CurrentInfluencePerSecond - 0.1f;

	if (m_CurrentInfluencePerSecond < 0.1f) m_CurrentInfluencePerSecond = 0.1f;

	float sampleAngleRadiance = m_sampleAngle * (M_PI / 180.f);

	float rotation = GetRotation();

	float bestSample{ 0 };

	Vector2 bestSamplePos = pos;

	for (int i{ -1 }; i < 2; ++i)
	{
		Vector2 position = Elite::OrientationToVector(rotation + (sampleAngleRadiance * i));

		position = pos + (position * m_sampleDistance);

		float newSample = m_pReadInfluenceMap->GetInfluenceAtPosition(position);

		if (newSample > bestSample)
		{
			bestSample = newSample;
			bestSamplePos = position;
		}
	}

	m_pSeek->SetTarget(bestSamplePos);
}