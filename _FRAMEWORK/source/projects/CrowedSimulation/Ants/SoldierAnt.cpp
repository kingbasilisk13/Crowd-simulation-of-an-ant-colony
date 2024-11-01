#include "stdafx.h"
#include "SoldierAnt.h"
using namespace Elite;
SoldierAnt::SoldierAnt():
	AntBase(2.f)
{
	SetMaxLinearSpeed(10.f);
	SetMass(0.3f);
	SetBodyColor(Color{ 102.f/255.f, 51.f/255.f, 0.f });

	m_MaxEnergy = 100;
	m_CurrentEnergy = 50;

	m_MaxHealth = 100;
	m_CurrentHealth = 100;

	m_MaxLifeTime = float(rand() % 500 + 500);

	m_Name = "Soldier";
}

SoldierAnt::~SoldierAnt()
{
}

void SoldierAnt::Update(float deltaTime)
{
	AntBase::Update(deltaTime);
}
