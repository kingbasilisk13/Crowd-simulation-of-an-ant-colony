#include "stdafx.h"
#include "SoldierAnt.h"
using namespace Elite;
SoldierAnt::SoldierAnt():
	AntBase(2.f)
{
	SetMaxLinearSpeed(10.f);
	SetMass(0.3f);
	SetBodyColor(Color{ 102.f/255.f, 51.f/255.f, 0.f });
}

SoldierAnt::~SoldierAnt()
{
}

void SoldierAnt::Update(float deltaTime)
{
	AntBase::Update(deltaTime);
}
