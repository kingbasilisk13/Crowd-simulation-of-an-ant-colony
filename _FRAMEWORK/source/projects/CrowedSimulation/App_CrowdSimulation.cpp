#include "stdafx.h"
#include "App_CrowdSimulation.h"


#include "projects/DecisionMaking/BehaviorTrees/Behaviors.h"



using namespace Elite;
App_CrowdSimulation::~App_CrowdSimulation()
{
	for (Food* pFood : m_pFoodVec)
	{
		SAFE_DELETE(pFood);
	}

	for(auto ant: m_Ants)
	{
		SAFE_DELETE(ant);
	}
	SAFE_DELETE(m_pBehaviorTree);
}

void App_CrowdSimulation::Start()
{
	//----------- CAMERA ------------
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(200.f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(200, 200));

	Vector2 centerWorld = { m_WorldSize * 0.5f, m_WorldSize * 0.5f };

	//Spawn food
	m_pFoodVec.reserve(m_AmountOfFoodItems);
	for (int idx = 0; idx < m_AmountOfFoodItems; ++idx)
	{
		float angle = Elite::randomFloat(float(M_PI) * 2);
		Vector2 pos = centerWorld + Elite::OrientationToVector(angle) * m_FoodDistance;

		m_pFoodVec.emplace_back(new Food(pos, m_FoodAmount));
	}

	//create ants
	for(int i{0}; i < m_StartAmountOfWorkers; ++i)
	{
		WorkerAnt* pWorker = new WorkerAnt();
		pWorker->SetPosition(centerWorld);
		m_Ants.push_back(pWorker);
	}

	for (int i{ 0 }; i < m_StartAmountOfSoldiers; ++i)
	{
		SoldierAnt* pSoldier = new SoldierAnt();
		pSoldier->SetPosition(centerWorld);
		m_Ants.push_back(pSoldier);
	}

	QueenAnt* pQueen = new QueenAnt();
	pQueen->SetPosition(centerWorld);
	m_Ants.push_back(pQueen);

	CreateBlackboard();
	CreateBehaviorTree();
}

void App_CrowdSimulation::Update(float deltaTime)
{
	m_pBlackboard->GetData("Ants", m_Ants);

	for (const auto& ant : m_Ants)
	{
		m_pBlackboard->ChangeData("CurrentAnt", ant);

		m_pBehaviorTree->Update(deltaTime);

		ant->Update(deltaTime);
	}

	UpdateUI();
}

void App_CrowdSimulation::UpdateUI()
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

	/*auto momentum = m_pInfluenceMap_Food->GetMomentum();
	auto decay = m_pInfluenceMap_Food->GetDecay();
	auto propagationInterval = m_pInfluenceMap_Food->GetPropagationInterval();*/

	/*ImGui::SliderFloat("Momentum", &momentum, 0.0f, 1.f, "%.2");
	ImGui::SliderFloat("Decay", &decay, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Propagation Interval", &propagationInterval, 0.f, 2.f, "%.2");*/
	ImGui::Spacing();

	//Set data
	/*m_pInfluenceMap_Food->SetMomentum(momentum);
	m_pInfluenceMap_Food->SetDecay(decay);
	m_pInfluenceMap_Food->SetPropagationInterval(propagationInterval);
	m_pInfluenceMap_Home->SetMomentum(momentum);
	m_pInfluenceMap_Home->SetDecay(decay);
	m_pInfluenceMap_Home->SetPropagationInterval(propagationInterval);*/

	//Ant Parameters
	ImGui::Text("Ants");
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::SliderFloat("Influence Per Second", &m_InfluencePerSecond, 0.0f, 100.f, "%.2");
	ImGui::SliderFloat("Wander Pct", &m_AntWanderPct, 0.0f, 1.f, "%.2");
	ImGui::SliderFloat("Sample Distance", &m_AntSampleDist, 1.f, 20.f, "%.2");
	ImGui::SliderFloat("Sample Angle", &m_AntSampleAngle, 0.f, 180.f, "%.2");
	ImGui::Checkbox("Render debug", &m_RenderAntDebug);

	/*for (AntAgent* const ant : m_pAnts)
	{
		ant->SetInfluencePerSecond(m_InfluencePerSecond);
		ant->SetWanderAmount(m_AntWanderPct);
		ant->SetSampleDistance(m_AntSampleDist);
		ant->SetSampleAngle(m_AntSampleAngle);
	}*/

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
}

void App_CrowdSimulation::Render(float deltaTime) const
{
	for (const auto& ant : m_Ants)
	{
		ant->Render(deltaTime);
	}

	for (Food* pFood : m_pFoodVec)
	{
		DEBUGRENDERER2D->DrawCircle(pFood->GetPosition(), m_foodRadius, { 0.f, 1.f, 0.f }, DEBUGRENDERER2D->NextDepthSlice());

		pFood->Render(deltaTime);
	}
}

void App_CrowdSimulation::CreateBlackboard()
{
	m_pBlackboard = new Blackboard();

	m_pBlackboard->AddData("Ants", m_Ants);

	m_pBlackboard->AddData("CurrentAnt", m_Ants[0]);


}

void App_CrowdSimulation::CreateBehaviorTree()
{
	m_pBehaviorTree = new BehaviorTree(
		m_pBlackboard,
		new BehaviorSelector(
			{
				CreateWorkerAntSequence(),
				CreateSoldierAntSequence(),
				CreateQueenAntSequence()
			}
		)
	);
}

Elite::BehaviorSequence* App_CrowdSimulation::CreateQueenAntSequence()
{
	return new BehaviorSequence(
		{
			new BehaviorAction(BT_Actions::ThisIsTheQueen),
			new BehaviorConditional(BT_Conditions::CanQueenSpawnBrood),
			new BehaviorAction(BT_Actions::SpawnBrood)
		}
	);
}

Elite::BehaviorSequence* App_CrowdSimulation::CreateSoldierAntSequence()
{
	return new BehaviorSequence(
		{
			new BehaviorConditional(BT_Conditions::IsThisASoldierAnt),
			new BehaviorAction(BT_Actions::ThisIsASoldier)
		}
	);
}

Elite::BehaviorSequence* App_CrowdSimulation::CreateWorkerAntSequence()
{
	return new BehaviorSequence(
		{
			new BehaviorConditional(BT_Conditions::IsThisAWorkerAnt),
			new BehaviorAction(BT_Actions::ThisIsAWorker)
		}
	);
}
