#include "stdafx.h"
#include "WorkerAnt.h"
using namespace Elite;
WorkerAnt::WorkerAnt():
	AntBase(1.f)
{
	SetBodyColor(Color{ 255.f / 255.f, 128.f / 255.f, 0.f });
}

WorkerAnt::~WorkerAnt()
{
}

void WorkerAnt::Update(float deltaTime)
{
	AntBase::Update(deltaTime);
}
