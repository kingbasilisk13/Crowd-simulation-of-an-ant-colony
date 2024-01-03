#include "stdafx.h"
#include "DeadAnt.h"
using namespace Elite;
DeadAnt::DeadAnt():
	AntBase(1.f)
{
	SetMaxLinearSpeed(0.f);
	SetBodyColor(Color{ 1.f, 1.f, 1.f });
	SetAutoOrient(false);
	m_pReadInfluenceMap = nullptr;
}

DeadAnt::~DeadAnt()
{
}

void DeadAnt::Update(float deltaTime)
{
	AntBase::Update(deltaTime);
}