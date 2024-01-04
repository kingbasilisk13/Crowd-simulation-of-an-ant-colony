#include "stdafx.h"
#include "QueenAnt.h"

using namespace Elite;
using namespace std::chrono;

QueenAnt::QueenAnt()
	:AntBase(3.f)
{
	SetMaxLinearSpeed(0.f);
	SetMass(1.f);
	SetBodyColor(Color{ 153.f/255.f, 0.f, 0.f });

	m_LastTimePoinSpawnBrood = std::chrono::steady_clock::now();
	//m_InfluencePerSecond = 100.f;
	m_CurrentEnergy = 50;
	m_Name = "Queen";
}

QueenAnt::~QueenAnt()
{
}

void QueenAnt::Update(float deltaTime)
{
	m_Age = 0.f;
	AntBase::Update(deltaTime);

}

bool QueenAnt::SpawnBrood()
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

	if (std::chrono::duration_cast<std::chrono::seconds>(now - m_LastTimePoinSpawnBrood).count() > m_MaxTimeBetweenSpawnBrood)
	{
		m_LastTimePoinSpawnBrood = std::chrono::steady_clock::now();

		return true;
	}


	return false;
}
