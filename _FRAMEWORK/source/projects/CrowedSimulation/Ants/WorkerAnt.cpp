#include "stdafx.h"
#include "WorkerAnt.h"
using namespace Elite;
WorkerAnt::WorkerAnt():
	AntBase(1.f)
{
	SetBodyColor(Color{ 255.f / 255.f, 128.f / 255.f, 0.f });

	m_MaxEnergy = 50;
	m_CurrentEnergy = 50;
}

WorkerAnt::~WorkerAnt()
{
}

void WorkerAnt::Update(float deltaTime)
{
	AntBase::Update(deltaTime);
}


bool WorkerAnt::IsSocialStomachFull() const
{
	if (m_SocialStomach >= m_MaxSizeSocialStomach)
	{
		return true;
	}
	return false;
}

void WorkerAnt::StoreFoodInSocialStomach(int food)
{
	m_SocialStomach += food;
}

int WorkerAnt::GiveFoodFromSocialStomach()
{
	if(m_SocialStomach > 0)
	{
		m_SocialStomach -= 1;
		return 1;
	}
	return 0;
}
