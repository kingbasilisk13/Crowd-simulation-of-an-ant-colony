#include "stdafx.h"
#include "App_CrowdSimulation.h"


#include "projects/DecisionMaking/BehaviorTrees/Behaviors.h"



using namespace Elite;
App_CrowdSimulation::~App_CrowdSimulation()
{
	for (Food* pFood : m_pFoodVec)
	{
		if(pFood)
		{
			SAFE_DELETE(pFood);
		}
	}

	for(AntBase* pAnt: m_pAnts)
	{
		if (pAnt)
		{
			SAFE_DELETE(pAnt);
		}
	}
	SAFE_DELETE(m_pBehaviorTree);
	SAFE_DELETE(m_pInfluenceMapFood);
	SAFE_DELETE(m_pInfluenceMapHome);
	SAFE_DELETE(m_pInfluenceMapHunger);
	SAFE_DELETE(m_pInfluenceMapDeath);
	SAFE_DELETE(m_pInfluenceMapThreat);
}

void App_CrowdSimulation::Start()
{
	//----------- CAMERA ------------
	DEBUGRENDERER2D->GetActiveCamera()->SetZoom(200.f);
	DEBUGRENDERER2D->GetActiveCamera()->SetCenter(Elite::Vector2(200, 200));

	 m_CenterWorld = { m_WorldSize * 0.5f, m_WorldSize * 0.5f };


	 const float decay{ 0.15f };
	 const float momentum{ 1.f - decay };
	//influence maps
	m_pInfluenceMapFood = new InfluenceMap(50, 50, 8);
	m_pInfluenceMapFood->SetDecay(decay);
	m_pInfluenceMapFood->SetMomentum(momentum);

	m_pInfluenceMapHome = new InfluenceMap(50, 50, 8);
	m_pInfluenceMapHome->SetDecay(decay);
	m_pInfluenceMapHome->SetMomentum(momentum);
	
	m_pInfluenceMapHunger = new InfluenceMap(50, 50, 8);
	m_pInfluenceMapHunger->SetDecay(decay);
	m_pInfluenceMapHunger->SetMomentum(momentum);

	m_pInfluenceMapDeath = new InfluenceMap(50, 50, 8);
	m_pInfluenceMapDeath->SetDecay(decay);
	m_pInfluenceMapDeath->SetMomentum(momentum);

	m_pInfluenceMapThreat = new InfluenceMap(50, 50, 8);
	m_pInfluenceMapThreat->SetDecay(decay);
	m_pInfluenceMapThreat->SetMomentum(momentum);

	
		

	//Spawn food
	m_pFoodVec.reserve(m_AmountOfFoodItems);
	for (int idx = 0; idx < m_AmountOfFoodItems; ++idx)
	{
		float angle = Elite::randomFloat(float(M_PI) * 2);
		Vector2 pos = m_CenterWorld + Elite::OrientationToVector(angle) * m_FoodDistance;
		m_pFoodVec.emplace_back(new Food(pos, m_FoodAmount));
	}

	//create ants
	for(int i{0}; i < m_StartAmountOfWorkers; ++i)
	{
		WorkerAnt* pWorker = new WorkerAnt();
		pWorker->SetPosition(m_CenterWorld);
		pWorker->SetInfluencePerSecond(50.f);
		m_pAnts.push_back(pWorker);
	}

	for (int i{ 0 }; i < m_StartAmountOfSoldiers; ++i)
	{
		SoldierAnt* pSoldier = new SoldierAnt();
		pSoldier->SetPosition(m_CenterWorld);
		m_pAnts.push_back(pSoldier);
	}

	QueenAnt* pQueen = new QueenAnt();
	pQueen->SetPosition(m_CenterWorld);
	m_pAnts.push_back(pQueen);

	for (auto& ant : m_pAnts)
	{
		ant->TrimToWorld(m_WorldSize, false);
	}
	
	CreateBlackboard();
	CreateBehaviorTree();
}

void App_CrowdSimulation::Update(float deltaTime)
{
	//stop the simulation the moment the queen is dead.
	if(!m_QueenAntIsDead)
	{
		m_pInfluenceMapFood->Update(deltaTime);
		m_pInfluenceMapHome->Update(deltaTime);
		m_pInfluenceMapHunger->Update(deltaTime);
		m_pInfluenceMapDeath->Update(deltaTime);
		m_pInfluenceMapThreat->Update(deltaTime);

		m_pBlackboard->GetData("Ants", m_pAnts);
		//the first loop is used to update the behavior of the ants
		for (const auto& ant : m_pAnts)
		{
			m_pBlackboard->ChangeData("CurrentAnt", ant);

			m_pBehaviorTree->Update(deltaTime);
		}

		//the second loop makes the ants act upon the set behavior.
		for (int i{ static_cast<int>(m_pAnts.size()) - 1 }; i >= 0; --i)
		{
			m_pAnts[i]->Update(deltaTime);
			m_pAnts[i]->TrimToWorld(m_WorldSize, false);
		}
		

		m_pBlackboard->GetData("FoodSpots", m_pFoodVec);
		for(int i{ static_cast<int>(m_pFoodVec.size())-1}; i >= 0; --i)
		{
			if (m_pFoodVec[i]->GetAmount() <= 0)
			{
				SAFE_DELETE(m_pFoodVec[i]);
				
				float angle = Elite::randomFloat(float(M_PI) * 2);
				Vector2 pos = m_CenterWorld + Elite::OrientationToVector(angle) * m_FoodDistance;

				m_pFoodVec[i] = new Food(pos, m_FoodAmount);
			}
		}
		m_pBlackboard->ChangeData("FoodSpots", m_pFoodVec);

		QueenAnt* pQueen{};
		m_pBlackboard->GetData("Queen", pQueen);
		if (pQueen)
		{
			if (pQueen->IsAntDead())
			{
				m_QueenAntIsDead = true;
			}
		}
	}
	
	UpdateUI();
}

void App_CrowdSimulation::UpdateUI()
{
	if(m_QueenAntIsDead)
	{
		//Setup
		int menuWidth = 200;
		int menuHeight = 200;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2((width * 0.5f) - (menuWidth * 0.5f), (height * 0.5f) - (menuHeight * 0.5f)));
		ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)menuHeight));
		ImGui::Begin("The queen ant has died.", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		
		//End
		ImGui::End();
	}
	else
	{
		//Setup
		int menuWidth = 230;
		int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
		int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
		bool windowActive = true;
		ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 20, 10));
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

		ImGui::Checkbox("Show Food Influence map ", &m_RenderInfluenceMapFood);
		ImGui::Checkbox("Show Home Influence map ", &m_RenderInfluenceMapHome);
		ImGui::Checkbox("Show Hunger Influence map ", &m_RenderInfluenceMapHunger);
		ImGui::Checkbox("Show Death Influence map ", &m_RenderInfluenceMapDeath);
		ImGui::Checkbox("Show Threat Influence map ", &m_RenderInfluenceMapThreat);

		auto momentum = m_pInfluenceMapFood->GetMomentum();
		auto decay = m_pInfluenceMapFood->GetDecay();
		auto propagationInterval = m_pInfluenceMapFood->GetPropagationInterval();

		ImGui::SliderFloat("Momentum", &momentum, 0.0f, 1.f, "%.2");
		ImGui::SliderFloat("Decay", &decay, 0.f, 1.f, "%.2");
		ImGui::SliderFloat("Propagation Interval", &propagationInterval, 0.f, 2.f, "%.2");
		ImGui::Spacing();

		//Set data
		m_pInfluenceMapFood->SetMomentum(momentum);
		m_pInfluenceMapFood->SetDecay(decay);
		m_pInfluenceMapFood->SetPropagationInterval(propagationInterval);
		
		m_pInfluenceMapHome->SetMomentum(momentum);
		m_pInfluenceMapHome->SetDecay(decay);
		m_pInfluenceMapHome->SetPropagationInterval(propagationInterval);
		
		m_pInfluenceMapHunger->SetMomentum(momentum);
		m_pInfluenceMapHunger->SetDecay(decay);
		m_pInfluenceMapHunger->SetPropagationInterval(propagationInterval);

		m_pInfluenceMapDeath->SetMomentum(momentum);
		m_pInfluenceMapDeath->SetDecay(decay);
		m_pInfluenceMapDeath->SetPropagationInterval(propagationInterval);

		m_pInfluenceMapThreat->SetMomentum(momentum);
		m_pInfluenceMapThreat->SetDecay(decay);
		m_pInfluenceMapThreat->SetPropagationInterval(propagationInterval);

		//Ant Parameters
		ImGui::Text("Ants");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::SliderFloat("Wander Pct", &m_AntWanderPct, 0.0f, 1.f, "%.2");
		ImGui::SliderFloat("Sample Distance", &m_AntSampleDist, 1.f, 20.f, "%.2");
		ImGui::SliderFloat("Sample Angle", &m_AntSampleAngle, 0.f, 180.f, "%.2");
		ImGui::Checkbox("Render interaction range", &m_RenderAntInteractionRange);

		for (const auto& ant : m_pAnts)
		{
			ant->SetWanderAmount(m_AntWanderPct);
			ant->SetSampleDistance(m_AntSampleDist);
			ant->SetSampleAngle(m_AntSampleAngle);
		}

		//End
		ImGui::PopAllowKeyboardFocus();
		ImGui::End();
	}
}

void App_CrowdSimulation::Render(float deltaTime) const
{
	for (const auto& ant : m_pAnts)
	{
		ant->Render(deltaTime, m_RenderAntInteractionRange);
	}

	for (Food* pFood : m_pFoodVec)
	{
		DEBUGRENDERER2D->DrawCircle(pFood->GetPosition(), m_foodRadius, { 0.f, 1.f, 0.f }, DEBUGRENDERER2D->NextDepthSlice());

		pFood->Render(deltaTime);
	}

	DEBUGRENDERER2D->DrawCircle(m_GarbageSiteLocation, m_foodRadius, { 0.f, 0.f, 0.f }, DEBUGRENDERER2D->NextDepthSlice());


	if (m_RenderInfluenceMapFood)
	{
		m_pInfluenceMapFood->Render();
	}
	else if (m_RenderInfluenceMapHome)
	{
		m_pInfluenceMapHome->Render();
	}
	else if (m_RenderInfluenceMapHunger)
	{
		m_pInfluenceMapHunger->Render();
	}
	else if (m_RenderInfluenceMapDeath)
	{
		m_pInfluenceMapDeath->Render();
	}
	else if (m_RenderInfluenceMapThreat)
	{
		m_pInfluenceMapThreat->Render();
	}
		
}

void App_CrowdSimulation::CreateBlackboard()
{
	m_pBlackboard = new Blackboard();

	//ants data
	m_pBlackboard->AddData("Ants", m_pAnts);
	
	m_pBlackboard->AddData("CurrentAnt", m_pAnts[0]);

	WorkerAnt* worker{};
	m_pBlackboard->AddData("CurrentWorker", worker);
	SoldierAnt* soldier{};
	m_pBlackboard->AddData("CurrentSoldier", soldier);

	for (const auto& ant : m_pAnts)
	{
		if (typeid(QueenAnt) == typeid(*ant))
		{
			QueenAnt* queen = dynamic_cast<QueenAnt*>(ant);
			m_pBlackboard->AddData("Queen", queen);
			break;
		}
	}

	//food
	m_pBlackboard->AddData("FoodSpots", m_pFoodVec);

	Food* pFood{};
	m_pBlackboard->AddData("TargetFoodSpot", pFood);

	AntBase* pStarvingAnt{};
	m_pBlackboard->AddData("TargetStarvingAnt", pStarvingAnt);

	AntBase* pTargetDeadAnt{};
	m_pBlackboard->AddData("TargetDeadAnt", pTargetDeadAnt);
	
	m_pBlackboard->AddData("FoodMap", m_pInfluenceMapFood);
	m_pBlackboard->AddData("HomeMap", m_pInfluenceMapHome);
	m_pBlackboard->AddData("HungerMap", m_pInfluenceMapHunger);
	m_pBlackboard->AddData("DeathMap", m_pInfluenceMapDeath);
	m_pBlackboard->AddData("ThreatMap", m_pInfluenceMapThreat);


	m_pBlackboard->AddData("GarbageSiteLocation", m_GarbageSiteLocation);
}

void App_CrowdSimulation::CreateBehaviorTree()
{
	m_pBehaviorTree = new BehaviorTree(
		m_pBlackboard,
		new BehaviorSelector(
			{
				CreateDeadAntSequence(),
				CreateWorkerAntSequence(),
				CreateSoldierAntSequence(),
				CreateQueenAntSequence()
			}
		)
	);
}

Elite::BehaviorSequence* App_CrowdSimulation::CreateDeadAntSequence()
{
	return new BehaviorSequence(
		{
			new BehaviorConditional(BT_Conditions::IsAntDead),
			new BehaviorAction(BT_Actions::SetDeathMapAsWriteMap),
			new BehaviorAction(BT_Actions::SetReadMapToNull)
		}
	);
}

Elite::BehaviorSequence* App_CrowdSimulation::CreateWorkerAntSequence()
{
	return new BehaviorSequence(
		{
			new BehaviorConditional(BT_Conditions::IsThisAWorkerAnt),
			//job selector
			new BehaviorSelector(
				{
					//idle give random job
					new BehaviorSequence(
						{
							new BehaviorConditional(BT_Conditions::IsAntIdle),
							new BehaviorAction(BT_Actions::GiveAntRandomJob)
						}
					),
					//scavenging
					new BehaviorSequence(
						{
							new BehaviorConditional(BT_Conditions::IsAntScavenging),
							new BehaviorAction(BT_Actions::SetFoodMapAsReadMap),
							new BehaviorAction(BT_Actions::SetHomeMapAsWriteMap),
							new BehaviorSelector(
								{
									new BehaviorSequence(
										{
											new BehaviorConditional(BT_Conditions::IsSocialStomachFull),
											new BehaviorAction(BT_Actions::StartFeedingOtherAnts)
										}
									),
									new BehaviorSequence(
										{
											new BehaviorConditional(BT_Conditions::IsAntNearFood),
											new BehaviorAction(BT_Actions::SetFoodInRangeAsTarget),
											new BehaviorAction(BT_Actions::SetFoodMapAsWriteMap),
											new BehaviorAction(BT_Actions::SetReadMapToNull),
											new BehaviorAction(BT_Actions::CollectFood)
										}
									),
									new BehaviorAction(BT_Actions::ReturnSuccess)
								}
							)
						}
					),
					//feeding
					new BehaviorSequence(
						{
							new BehaviorConditional(BT_Conditions::IsAntFeeding),
							new BehaviorAction(BT_Actions::SetHungerMapAsReadMap),
							new BehaviorAction(BT_Actions::SetWriteMapToNull),
							new BehaviorSelector(
								{
									new BehaviorSequence(
										{
											new BehaviorConditional(BT_Conditions::IsSocialStomachEmpty),
											new BehaviorAction(BT_Actions::SetAntIdle)
										}
									),
									new BehaviorSequence(
										{
											new BehaviorConditional(BT_Conditions::IsStarvingAntInRange),
											new BehaviorAction(BT_Actions::SetStarvingAntInRangeAsTarget),
											new BehaviorAction(BT_Actions::FeedStarvingAnt)
										}
									),
									new BehaviorAction(BT_Actions::ReturnSuccess)
								}
							)
						}
					),
					//cleaning
					new BehaviorSequence(
						{
							new BehaviorConditional(BT_Conditions::IsAntCleaning),
							new BehaviorAction(BT_Actions::SetDeathMapAsReadMap),
							new BehaviorAction(BT_Actions::SetWriteMapToNull),
							new BehaviorSelector(
								{
									new BehaviorSequence(
										{
											new BehaviorConditional(BT_Conditions::IsAntHoldingDeadAnt),
											new BehaviorAction(BT_Actions::SetGarbageSiteAsTarget),
											new BehaviorAction(BT_Actions::SetReadMapToNull),
											new BehaviorAction(BT_Actions::SetWriteMapToNull),
											new BehaviorSelector(
												{
													new BehaviorSequence(
														{
															new BehaviorConditional(BT_Conditions::IsAntNearGarbageSite),
															new BehaviorAction(BT_Actions::DropDeadAnt),
															new BehaviorAction(BT_Actions::SetAntIdle)
														}
													),
													new BehaviorAction(BT_Actions::ReturnSuccess)
												}
											)
										}
									),
									new BehaviorSequence(
										{
											new BehaviorConditional(BT_Conditions::IsAntNearDeadAnt),
											new BehaviorAction(BT_Actions::SetDeadAntInRangeAsTargetAnt),
											new BehaviorAction(BT_Actions::CollectDeadAnt)
										}
									),
									new BehaviorAction(BT_Actions::ReturnSuccess)
								}
							)
						}
					)
				}
			),
			new BehaviorSelector(
				{
					new BehaviorSequence(
						{
							new BehaviorConditional(BT_Conditions::IsAntStarving),
							new BehaviorAction(BT_Actions::SetHungerMapAsWriteMap)
						}
					),
					new BehaviorAction(BT_Actions::ReturnSuccess)
				}
			)
		}
	);
}

Elite::BehaviorSequence* App_CrowdSimulation::CreateSoldierAntSequence()
{
	return new BehaviorSequence(
		{
			new BehaviorConditional(BT_Conditions::IsThisASoldierAnt),
			new BehaviorAction(BT_Actions::SetHomeMapAsReadMap),
			new BehaviorSelector(
				{
					new BehaviorSequence(
						{
							new BehaviorConditional(BT_Conditions::IsAntStarving),
							new BehaviorAction(BT_Actions::SetHungerMapAsWriteMap)
						}
					),
					new BehaviorConditional(BT_Conditions::ReturnTrue)
				}
			)
		}
	);
}

Elite::BehaviorSequence* App_CrowdSimulation::CreateQueenAntSequence()
{
	return new BehaviorSequence(
		{
			new BehaviorConditional(BT_Conditions::CanQueenSpawnBrood),
			new BehaviorAction(BT_Actions::SpawnBrood),
			new BehaviorAction(BT_Actions::SetHomeMapAsWriteMap),
			new BehaviorSelector(
				{
					new BehaviorSequence(
						{
							new BehaviorConditional(BT_Conditions::IsAntStarving),
							new BehaviorAction(BT_Actions::SetHungerMapAsWriteMap)
						}
					),
					new BehaviorConditional(BT_Conditions::ReturnTrue)
				}
			)
		}
	);
}