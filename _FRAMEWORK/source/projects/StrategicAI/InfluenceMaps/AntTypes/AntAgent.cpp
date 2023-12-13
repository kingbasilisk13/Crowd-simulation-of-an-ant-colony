#include "stdafx.h"
#include "AntAgent.h"

using namespace Elite;

AntAgent::AntAgent(InfluenceMap* pInfluenceMapFood, InfluenceMap* pInfluenceMapHome, float radius)
	:SteeringAgent(radius)
	, m_pInfluenceMap_Home{ pInfluenceMapHome }
	, m_pInfluenceMap_Food{ pInfluenceMapFood }
{
	m_pSeek = new Seek();
	m_pWander = new Wander();
	m_pBlendedSteering = new BlendedSteering({ { m_pWander , 0.f}, { m_pSeek, 1.f } });
	SteeringAgent::SetSteeringBehavior(m_pBlendedSteering);
	SetAutoOrient(true);
	SetMaxAngularSpeed(180.f);
	SetMaxLinearSpeed(20.f);
	SetMass(0.1f);
	SetBodyColor(Color{ 1.f, 0.5f, 0.f });
	SetRotation(Elite::randomFloat(float(M_PI)*2));

	//Don't let agents collide with each otherA
	BaseAgent::m_pRigidBody->RemoveAllShapes();
}

AntAgent::~AntAgent()
{
	SAFE_DELETE(m_pSeek);
	SAFE_DELETE(m_pWander);
	SAFE_DELETE(m_pBlendedSteering);
}

void AntAgent::SetWanderAmount(float wanderPct)
{
	wanderPct = Clamp(wanderPct, 0.f, 1.f);
	auto& weightsRef = m_pBlendedSteering->GetWeightedBehaviorsRef();
	weightsRef[0].weight = wanderPct;
	weightsRef[1].weight = 1.f - wanderPct;
}

void AntAgent::SetHasFood(bool hasFood)
{
	if (m_HasFood == hasFood)return;
	m_HasFood = hasFood;

	SetBodyColor(m_HasFood ? Color{0.5f, 1.f, 0.f} : Color{1.f, 0.5f, 0.f});
	m_CurrentInfluencePerSecond = m_MaxInfluencePerSecond;
}

void AntAgent::Update(float deltaTime)
{
	Vector2 pos = GetPosition();

	//1. Determine which influence map to write to (has food or not
	//   - Set influence at current position
	if(m_HasFood)
	{
		m_pInfluenceMap_Food->SetInfluenceAtPosition(pos, (m_CurrentInfluencePerSecond * deltaTime), true);
	}
	else
	{
		m_pInfluenceMap_Home->SetInfluenceAtPosition(pos, (m_CurrentInfluencePerSecond * deltaTime), true);
	}

	m_CurrentInfluencePerSecond = m_CurrentInfluencePerSecond - 0.1f;

	if (m_CurrentInfluencePerSecond < 0.1f) m_CurrentInfluencePerSecond = 0.1f;

	//2. Update steering based on influence:
	//   - Determine which influence map to read from	
	//   - Create 3 sample positions: one straight forward, two on either side
	//   - sample influence on all 3 positions
	//   - Steer towards the highest sampled position
	Elite::InfluenceMap* influenceMap{};
	
	if (m_HasFood)
	{
		influenceMap = m_pInfluenceMap_Home;
	}
	else
	{
		influenceMap = m_pInfluenceMap_Food;
	}

	float sampleAngleRadiance = m_sampleAngle * (M_PI/180.f);

	float rotation = GetRotation();

	float bestSample{ 0 };

	Vector2 bestSamplePos = pos;

	for(int i{-1}; i < 2; ++i)
	{
		Vector2 position = Elite::OrientationToVector(rotation + (sampleAngleRadiance*i));

		position = pos + (position * m_sampleDistance);

		float newSample = influenceMap->GetInfluenceAtPosition(position);

		if(newSample > bestSample)
		{
			bestSample = newSample;
			bestSamplePos = position;
		}
	}

	m_pSeek->SetTarget(bestSamplePos);

	//Update steering
	SteeringAgent::Update(deltaTime);

	if (GetDebugRenderingEnabled())
	{
		//TODO: DEBUG RENDERING: show the sampled positions

		DEBUGRENDERER2D->DrawCircle(bestSamplePos, 1.0f, { 1.f, 1.f, 0.0f }, DEBUGRENDERER2D->NextDepthSlice());

	}
}

