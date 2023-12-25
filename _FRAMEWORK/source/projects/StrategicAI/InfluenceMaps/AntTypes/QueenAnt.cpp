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
}
