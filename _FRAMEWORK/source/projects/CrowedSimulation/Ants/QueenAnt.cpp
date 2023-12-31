#include "stdafx.h"
#include "QueenAnt.h"

using namespace Elite;

QueenAnt::QueenAnt()
	:AntBase(3.f)
{
	SetMaxLinearSpeed(0.f);
	SetMass(1.f);
	SetBodyColor(Color{ 153.f/255.f, 0.f, 0.f });
}

QueenAnt::~QueenAnt()
{
}

void QueenAnt::Update(float deltaTime)
{
	//lower food
	if(m_CurrentEnergy <= 0)
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
