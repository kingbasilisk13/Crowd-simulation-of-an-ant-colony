//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
using namespace Elite;

//Includes
#include "App_InfluenceMap.h"
#include "framework/EliteAI/EliteGraphs/EliteInfluenceMap/EInfluenceMap.h"
#include "framework/EliteAI/EliteGraphs/EliteGraphUtilities/EGraphRenderer.h"
#include "AntTypes/AntAgent.h"
#include "AntTypes/FoodSource.h"

//Destructor
App_InfluenceMap::~App_InfluenceMap()
{
	SAFE_DELETE(m_pInfluenceMap_Food);
	SAFE_DELETE(m_pInfluenceMap_Home);
//	SAFE_DELETE(m_pAntAgent);

	for (FoodSource* pFood : m_pFoodVec)
	{
		SAFE_DELETE(pFood);
	}

	for (AntAgent* ant : m_pAnts)
	{
		SAFE_DELETE(ant);
	}
}

//Functions
void App_InfluenceMap::Start()
{
	//Initialization of your application. If you want access to the physics world you will need to store it yourself.
	//----------- CAMERA ------------
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(200.f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(200, 200));

	Vector2 centerWorld = { m_WorldSize * 0.5f, m_WorldSize * 0.5f };
	//m_homePosition = { 20,20 };
	m_homePosition = centerWorld;

	m_pInfluenceMap_Food = new InfluenceMap(50, 50, 8);
	m_pInfluenceMap_Food->SetDecay(0.1f);
	m_pInfluenceMap_Food->SetMomentum(0.99f);

	m_pInfluenceMap_Home = new InfluenceMap(50, 50, 8);
	m_pInfluenceMap_Home->SetDecay(0.1f);
	m_pInfluenceMap_Home->SetMomentum(0.99f);

	//Spawn food
	m_pFoodVec.reserve(AMOUNT_FOOD_ITEMS);
	for (int idx = 0; idx < AMOUNT_FOOD_ITEMS; ++idx)
	{
		float angle = Elite::randomFloat(float(M_PI) * 2);
		Vector2 pos = centerWorld + Elite::OrientationToVector(angle) * FOOD_DISTANCE;

		m_pFoodVec.emplace_back(new FoodSource(pos, FOOD_AMOUNT));
	}

	for(int i{0}; i < MAX_AMOUNT_ANTS; ++i)
	{
		AntAgent* newAnt = new AntAgent(m_pInfluenceMap_Food, m_pInfluenceMap_Home);
		newAnt->SetPosition(m_homePosition);
		m_pAnts.push_back(newAnt);
	}
}

void App_InfluenceMap::Update(float deltaTime)
{
	//TESTING ONLY: Set influence at position
	bool testWithMouseInput = false;
	if (testWithMouseInput)
	{
		if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eRight))
		{
			auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eRight);
			Elite::Vector2 mousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld(Elite::Vector2((float)mouseData.X, (float)mouseData.Y));

			m_pInfluenceMap_Food->SetInfluenceAtPosition(mousePos, -100.f, false);
		}
		if (INPUTMANAGER->IsMouseButtonUp(InputMouseButton::eLeft))
		{
			auto mouseData = INPUTMANAGER->GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
			Elite::Vector2 mousePos = DEBUGRENDERER2D->GetActiveCamera()->ConvertScreenToWorld(Elite::Vector2((float)mouseData.X, (float)mouseData.Y));

			m_pInfluenceMap_Food->SetInfluenceAtPosition(mousePos, 100.f, false);
		}
	}

	//Update influence maps
	m_pInfluenceMap_Food->Update(deltaTime);
	m_pInfluenceMap_Home->Update(deltaTime);

	//Update ant agent
	/*m_pAntAgent->Update(deltaTime);
	m_pAntAgent->TrimToWorld(400.f, false);
	CheckTakeFoodSources(m_pAntAgent);
	CheckDropFood(m_pAntAgent);*/

	if(m_pAnts.size() < MAX_AMOUNT_ANTS)
	{
		m_AntSpawnTimer += deltaTime;

		if (m_AntSpawnTimer > m_AntSpawnInterval)
		{
			m_AntSpawnTimer = 0;
			AntAgent* newAnt = new AntAgent(m_pInfluenceMap_Food, m_pInfluenceMap_Home);
			newAnt->SetPosition(m_homePosition);
			m_pAnts.push_back(newAnt);
		}
	}

	for(AntAgent* const ant : m_pAnts)
	{
		ant->Update(deltaTime);
		ant->TrimToWorld(400.f, false);
		CheckTakeFoodSources(ant);
		CheckDropFood(ant);
	}


	UpdateUI();
}

void App_InfluenceMap::UpdateUI()
{
	//Setup
	int menuWidth = 200;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 90));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);
	ImGui::SetWindowFocus();
	ImGui::PushItemWidth(70);
	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Home");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::SliderFloat("Home Radius", &m_homeRadius, 0.0f, 40.f, "%.2");

	ImGui::Text("Food");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::SliderFloat("Food Radius", &m_foodRadius, 0.0f, 40.f, "%.2");

	ImGui::Text("Influence Maps");
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Checkbox("Show Food Influence map ", &m_RenderInfluenceMap_Food);
	ImGui::Checkbox("Show Home Influence map ", &m_RenderInfluenceMap_Home);

	auto momentum = m_pInfluenceMap_Food->GetMomentum();
	auto decay = m_pInfluenceMap_Food->GetDecay();
	auto propagationInterval = m_pInfluenceMap_Food->GetPropagationInterval();

	ImGui::SliderFloat("Momentum", &momentum, 0.0f, 1.f, "%.2");
	ImGui::SliderFloat("Decay", &decay, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Propagation Interval", &propagationInterval, 0.f, 2.f, "%.2");
	ImGui::Spacing();

	//Set data
	m_pInfluenceMap_Food->SetMomentum(momentum);
	m_pInfluenceMap_Food->SetDecay(decay);
	m_pInfluenceMap_Food->SetPropagationInterval(propagationInterval);
	m_pInfluenceMap_Home->SetMomentum(momentum);
	m_pInfluenceMap_Home->SetDecay(decay);
	m_pInfluenceMap_Home->SetPropagationInterval(propagationInterval);

	//Ant Parameters
	ImGui::Text("Ants");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::SliderFloat("Influence Per Second", &m_InfluencePerSecond, 0.0f, 100.f, "%.2");
	ImGui::SliderFloat("Wander Pct", &m_AntWanderPct, 0.0f, 1.f, "%.2");
	ImGui::SliderFloat("Sample Distance", &m_AntSampleDist, 1.f, 20.f, "%.2");
	ImGui::SliderFloat("Sample Angle", &m_AntSampleAngle, 0.f, 180.f, "%.2");
	ImGui::Checkbox("Render debug", &m_RenderAntDebug);

	//m_pAntAgent->SetDebugRenderingEnabled(m_RenderAntDebug); //Only the first ant!!!

	////Apply parameters to ant(s)
	//m_pAntAgent->SetInfluencePerSecond(m_InfluencePerSecond);
	//m_pAntAgent->SetWanderAmount(m_AntWanderPct);
	//m_pAntAgent->SetSampleDistance(m_AntSampleDist);
	//m_pAntAgent->SetSampleAngle(m_AntSampleAngle);

	for (AntAgent* const ant : m_pAnts)
	{
		ant->SetInfluencePerSecond(m_InfluencePerSecond);
		ant->SetWanderAmount(m_AntWanderPct);
		ant->SetSampleDistance(m_AntSampleDist);
		ant->SetSampleAngle(m_AntSampleAngle);
	}

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
}

void App_InfluenceMap::Render(float deltaTime) const
{
	DEBUGRENDERER2D->DrawCircle(m_homePosition, m_homeRadius, { 1.f, 1.f, 0.f }, DEBUGRENDERER2D->NextDepthSlice());

	/*m_pInfluenceMap_Home->Render();

	m_pInfluenceMap_Food->Render();*/

	if (m_RenderInfluenceMap_Food)
		m_pInfluenceMap_Food->Render();
	else if (m_RenderInfluenceMap_Home)
		m_pInfluenceMap_Home->Render();

	for (FoodSource* pFood : m_pFoodVec)
	{
		DEBUGRENDERER2D->DrawCircle(pFood->GetPosition(), m_foodRadius, { 0.f, 1.f, 0.f }, DEBUGRENDERER2D->NextDepthSlice());

		pFood->Render(deltaTime);
	}

	//m_pAntAgent->Render(deltaTime);

	for (AntAgent* const ant : m_pAnts)
	{
		ant->Render(deltaTime);
	}
}



void App_InfluenceMap::CheckTakeFoodSources(AntAgent* pAgent)
{
	//Can't take food if we already have food
	if (pAgent->GetHasFood())return;

	for (FoodSource* pFood : m_pFoodVec)
	{
		if (pFood->GetAmount() <= 0)continue;

		if (pAgent->GetPosition().DistanceSquared(pFood->GetPosition()) <= m_foodRadius * m_foodRadius)
		{
			pFood->TakeFood();
			pAgent->SetHasFood(true);
		}
	}

}

void App_InfluenceMap::CheckDropFood(AntAgent* pAgent)
{
	//Can't drop food if we don't have any
	if (!pAgent->GetHasFood())return;

	if (pAgent->GetPosition().DistanceSquared(m_homePosition) <= m_homeRadius * m_homeRadius)
	{
		pAgent->SetHasFood(false);
	}

}


