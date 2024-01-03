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

		AntBase* result{};

		//1/4 chanse to spawn soldier. 3/4 chanse for worker.
		int randomNumber = rand() % 101;

		//todo: find way to spawn both workers and soldiers.
		return true;
	}


	return false;
}
