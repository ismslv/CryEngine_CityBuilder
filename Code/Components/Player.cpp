#include "StdAfx.h"
#include "Player.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CryInput/IHardwareMouse.h>
#include <CryGame/IGameFramework.h>

void CPlayerComponent::Initialize()
{
	//Set components
	m_pCameraComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CCameraComponent>();
	m_pInputComponent = m_pEntity->GetOrCreateComponent<Cry::DefaultComponents::CInputComponent>();

	//Set input
	m_pInputComponent->RegisterAction("player", "moveleft", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveLeft, activationMode);  });
	m_pInputComponent->BindAction("player", "moveleft", eAID_KeyboardMouse,	EKeyId::eKI_A);

	m_pInputComponent->RegisterAction("player", "moveright", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveRight, activationMode);  });
	m_pInputComponent->BindAction("player", "moveright", eAID_KeyboardMouse, EKeyId::eKI_D);

	m_pInputComponent->RegisterAction("player", "moveforward", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveForward, activationMode);  });
	m_pInputComponent->BindAction("player", "moveforward", eAID_KeyboardMouse, EKeyId::eKI_W);

	m_pInputComponent->RegisterAction("player", "moveback", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveBack, activationMode);  });
	m_pInputComponent->BindAction("player", "moveback", eAID_KeyboardMouse, EKeyId::eKI_S);

	m_pInputComponent->RegisterAction("player", "moveup", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveUp, activationMode);  });
	m_pInputComponent->BindAction("player", "moveup", eAID_KeyboardMouse, EKeyId::eKI_Q);

	m_pInputComponent->RegisterAction("player", "movedown", [this](int activationMode, float value) { HandleInputFlagChange((TInputFlags)EInputFlag::MoveDown, activationMode);  });
	m_pInputComponent->BindAction("player", "movedown", eAID_KeyboardMouse, EKeyId::eKI_E);

	m_pInputComponent->RegisterAction("player", "mouse_rotateyaw", [this](int activationMode, float value) { m_mouseDeltaRotation.x -= value; });
	m_pInputComponent->BindAction("player", "mouse_rotateyaw", eAID_KeyboardMouse, EKeyId::eKI_MouseX);

	m_pInputComponent->RegisterAction("player", "mouse_rotatepitch", [this](int activationMode, float value) { m_mouseDeltaRotation.y -= value; });
	m_pInputComponent->BindAction("player", "mouse_rotatepitch", eAID_KeyboardMouse, EKeyId::eKI_MouseY);

	m_pInputComponent->RegisterAction("player", "floors_plus", [this](int activationMode, float value) { ChangeFloorsNumber(1); });
	m_pInputComponent->BindAction("player", "floors_plus", eAID_KeyboardMouse, EKeyId::eKI_Up, true, false, false);
	m_pInputComponent->BindAction("player", "floors_plus", eAID_KeyboardMouse, EKeyId::eKI_MouseWheelUp, true, false, false);

	m_pInputComponent->RegisterAction("player", "floors_minus", [this](int activationMode, float value) { ChangeFloorsNumber(-1); });
	m_pInputComponent->BindAction("player", "floors_minus", eAID_KeyboardMouse, EKeyId::eKI_Down, true, false, false);
	m_pInputComponent->BindAction("player", "floors_minus", eAID_KeyboardMouse, EKeyId::eKI_MouseWheelDown, true, false, false);

	m_pInputComponent->RegisterAction("player", "numkey1", [this](int activationMode, float value) { NumKeyPress(1); });
	m_pInputComponent->BindAction("player", "numkey1", eAID_KeyboardMouse, EKeyId::eKI_1, true, false, false);
	m_pInputComponent->BindAction("player", "numkey1", eAID_KeyboardMouse, EKeyId::eKI_NP_1, true, false, false);
	m_pInputComponent->RegisterAction("player", "numkey2", [this](int activationMode, float value) { NumKeyPress(2); });
	m_pInputComponent->BindAction("player", "numkey2", eAID_KeyboardMouse, EKeyId::eKI_2, true, false, false);
	m_pInputComponent->BindAction("player", "numkey2", eAID_KeyboardMouse, EKeyId::eKI_NP_2, true, false, false);
	m_pInputComponent->RegisterAction("player", "numkey3", [this](int activationMode, float value) { NumKeyPress(3); });
	m_pInputComponent->BindAction("player", "numkey3", eAID_KeyboardMouse, EKeyId::eKI_3, true, false, false);
	m_pInputComponent->BindAction("player", "numkey3", eAID_KeyboardMouse, EKeyId::eKI_NP_3, true, false, false);
	m_pInputComponent->RegisterAction("player", "numkey4", [this](int activationMode, float value) { NumKeyPress(4); });
	m_pInputComponent->BindAction("player", "numkey4", eAID_KeyboardMouse, EKeyId::eKI_4, true, false, false);
	m_pInputComponent->BindAction("player", "numkey4", eAID_KeyboardMouse, EKeyId::eKI_NP_4, true, false, false);
	m_pInputComponent->RegisterAction("player", "numkey5", [this](int activationMode, float value) { NumKeyPress(5); });
	m_pInputComponent->BindAction("player", "numkey5", eAID_KeyboardMouse, EKeyId::eKI_5, true, false, false);
	m_pInputComponent->BindAction("player", "numkey5", eAID_KeyboardMouse, EKeyId::eKI_NP_5, true, false, false);
	m_pInputComponent->RegisterAction("player", "numkey6", [this](int activationMode, float value) { NumKeyPress(6); });
	m_pInputComponent->BindAction("player", "numkey6", eAID_KeyboardMouse, EKeyId::eKI_6, true, false, false);
	m_pInputComponent->BindAction("player", "numkey6", eAID_KeyboardMouse, EKeyId::eKI_NP_6, true, false, false);
	m_pInputComponent->RegisterAction("player", "numkey7", [this](int activationMode, float value) { NumKeyPress(7); });
	m_pInputComponent->BindAction("player", "numkey7", eAID_KeyboardMouse, EKeyId::eKI_7, true, false, false);
	m_pInputComponent->BindAction("player", "numkey7", eAID_KeyboardMouse, EKeyId::eKI_NP_7, true, false, false);
	m_pInputComponent->RegisterAction("player", "numkey8", [this](int activationMode, float value) { NumKeyPress(8); });
	m_pInputComponent->BindAction("player", "numkey8", eAID_KeyboardMouse, EKeyId::eKI_8, true, false, false);
	m_pInputComponent->BindAction("player", "numkey8", eAID_KeyboardMouse, EKeyId::eKI_NP_8, true, false, false);
	m_pInputComponent->RegisterAction("player", "numkey9", [this](int activationMode, float value) { NumKeyPress(9); });
	m_pInputComponent->BindAction("player", "numkey9", eAID_KeyboardMouse, EKeyId::eKI_9, true, false, false);
	m_pInputComponent->BindAction("player", "numkey9", eAID_KeyboardMouse, EKeyId::eKI_NP_9, true, false, false);

	m_pInputComponent->RegisterAction("player", "modeswitch", [this](int activationMode, float value) { ModeSwitch(); });
	m_pInputComponent->BindAction("player", "modeswitch", eAID_KeyboardMouse, EKeyId::eKI_Space, true, false, false);

	m_pInputComponent->RegisterAction("player", "shift", [this](int activationMode, float value) { 
		if (activationMode == eIS_Pressed) {
			selectionMode = SelectionMode::Group;
		}
		else if (activationMode == eIS_Released) {
			selectionMode = SelectionMode::Single;
		}
		if (selectedBuilding != NULL) {
			auto b = selectedBuilding;
			DeselectBuilding();
			SelectBuilding(b);
		}
	});
	m_pInputComponent->BindAction("player", "shift", eAID_KeyboardMouse, EKeyId::eKI_LShift);
	m_pInputComponent->BindAction("player", "shift", eAID_KeyboardMouse, EKeyId::eKI_RShift);

	m_pInputComponent->RegisterAction("player", "ctrl", [this](int activationMode, float value) {
		if (activationMode == eIS_Pressed) {
			selectionMode = SelectionMode::Distance;
		}
		else if (activationMode == eIS_Released) {
			selectionMode = SelectionMode::Single;
		}
		if (selectedBuilding != NULL) {
			auto b = selectedBuilding;
			DeselectBuilding();
			SelectBuilding(b);
		}
	});
	m_pInputComponent->BindAction("player", "ctrl", eAID_KeyboardMouse, EKeyId::eKI_LCtrl);
	m_pInputComponent->BindAction("player", "ctrl", eAID_KeyboardMouse, EKeyId::eKI_RCtrl);

	m_pInputComponent->RegisterAction("player", "shoot", [this](int activationMode, float value) { OnClick(); });
	m_pInputComponent->BindAction("player", "shoot", eAID_KeyboardMouse, EKeyId::eKI_Mouse1, true, false, false);

	//Create the city
	city = new CityManager();
	city->GrowTrees(Vec3(64, 64, 32), 40, 30);

	//Timer for population update
	GetEntity()->SetTimer(1, 1000);

	//Set entity and camera position and rotation
	GetEntity()->SetPos(startPosition);
	Matrix34 localTransform = IDENTITY;
	localTransform.SetRotation33(Matrix33(m_pEntity->GetWorldRotation().GetInverted()) * Matrix33::CreateRotationX(DEG2RAD(cameraAngle)));
	localTransform.SetTranslation(Vec3(0, 0, viewDistanceFromPlayer));
	m_pCameraComponent->SetTransformMatrix(localTransform);

	//Cursor and mode
	gEnv->pHardwareMouse->IncrementCounter(); //because it is hidden by default;
	BuildCursorCreation();
	ModeSet(PlayerMode::Building);
}

void CPlayerComponent::BuildCursorCreation() {
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	IEntity* entityB = gEnv->pEntitySystem->SpawnEntity(spawnParams);
	cursorB = entityB->GetOrCreateComponentClass<Building>(Building::floorsDefault, m_cursorPositionInWorld, true);
}

void CPlayerComponent::NumKeyPress(int num) {
	if (mode == PlayerMode::Building) {
		currentGroup = num;
	}
}

void CPlayerComponent::ModeSwitch() {
	switch (mode)
	{
	case PlayerMode::Building:
		ModeSet(PlayerMode::Selecting);
		break;
	case PlayerMode::Selecting:
		ModeSet(PlayerMode::Building);
		break;
	}
}

void CPlayerComponent::ModeSet(PlayerMode _mode) {
	switch (_mode)
	{
	case PlayerMode::Selecting:
		mode = PlayerMode::Selecting;
		cursorB->GetEntity()->Hide(true);
		gEnv->pHardwareMouse->IncrementCounter();
		break;
	case PlayerMode::Building:
		mode = PlayerMode::Building;
		SetFocusBuilding(NULL);
		DeselectBuilding();
		m_currentFloors = Building::floorsDefault;
		cursorB->GetEntity()->Hide(false);
		gEnv->pHardwareMouse->DecrementCounter();
		break;
	}
}

uint64 CPlayerComponent::GetEventMask() const
{
	return ENTITY_EVENT_BIT(ENTITY_EVENT_RESET) | ENTITY_EVENT_BIT(ENTITY_EVENT_START_GAME) | ENTITY_EVENT_BIT(ENTITY_EVENT_UPDATE) | ENTITY_EVENT_BIT(ENTITY_EVENT_TIMER);
}

void CPlayerComponent::ProcessEvent(const SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_RESET:
	{
		// Check if we're entering game mode
		if (event.nParam[0] == 1)
		{
			//SpawnBuildingFloor();
		}
		else
		{
			// Removed by Sandbox
			//m_pFloorEntity = nullptr;
		}
	}
	break;
	case ENTITY_EVENT_START_GAME:
	{
		
	}
	break;
	case ENTITY_EVENT_UPDATE:
	{
		SEntityUpdateContext* pCtx = (SEntityUpdateContext*)event.nParam[0];

		// Update the in-world cursor position
		UpdateCursor(pCtx->fFrameTime);

		// Start by updating the movement request we want to send to the character controller
		// This results in the physical representation of the character moving
		UpdateMovementRequest(pCtx->fFrameTime);

		// Update the camera component offset
		//UpdateCamera(pCtx->fFrameTime);
		UpdateUI(pCtx->fFrameTime);
	}
	break;
	case ENTITY_EVENT_TIMER:
		if (event.nParam[0] == 1) {
			if (city != NULL)
				city->UpdatePopulation();
			GetEntity()->SetTimer(1, 1000);
		}
	break;
	}
}

void CPlayerComponent::OnClick() {
	switch (mode)
	{
	case PlayerMode::Building:
		if (canBuild) {
			BuildBuilding();
		}
		break;
	case PlayerMode::Selecting:
		if (focusBuildingNext != NULL) {
			SelectBuilding(focusBuildingNext);
		}
		else {
			DeselectBuilding();
		}
		break;
	default:
		break;
	}
}

void CPlayerComponent::ChangeFloorsNumber(int toAdd = 1) {
	m_currentFloors = CLAMP(m_currentFloors + toAdd, Building::floorsMinMax.x, Building::floorsMinMax.y);
}

void CPlayerComponent::BuildBuilding() {
	city->Build(m_currentFloors, m_cursorPositionInWorld, currentGroup);
	m_currentFloors = Building::floorsDefault;
}

void CPlayerComponent::UpdateMovementRequest(float frameTime)
{
	Vec3 velocity = ZERO;

	if (m_inputFlags & (TInputFlags)EInputFlag::MoveLeft)
	{
		velocity.x -= moveSpeed.x * frameTime;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveRight)
	{
		velocity.x += moveSpeed.x * frameTime;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveForward)
	{
		velocity.y += moveSpeed.x * frameTime;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveBack)
	{
		velocity.y -= moveSpeed.x * frameTime;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveUp) {
		velocity.z += moveSpeed.y * frameTime;
	}
	if (m_inputFlags & (TInputFlags)EInputFlag::MoveDown) {
		velocity.z -= moveSpeed.y * frameTime;
	}

	Vec3 NewPos = m_pEntity->GetPos() + velocity;
	NewPos.z = clamp_tpl(NewPos.z, verticalLimits.x, verticalLimits.y);

	m_pEntity->SetPos(NewPos);
}

void CPlayerComponent::UpdateCursor(float frameTime)
{
	float mouseX, mouseY;
	gEnv->pHardwareMouse->GetHardwareMouseClientPosition(&mouseX, &mouseY);

	// Invert mouse Y
	mouseY = gEnv->pRenderer->GetHeight() - mouseY;

	Vec3 vPos0(0, 0, 0);
	gEnv->pRenderer->UnProjectFromScreen(mouseX, mouseY, 0, &vPos0.x, &vPos0.y, &vPos0.z);

	Vec3 vPos1(0, 0, 0);
	gEnv->pRenderer->UnProjectFromScreen(mouseX, mouseY, 1, &vPos1.x, &vPos1.y, &vPos1.z);

	Vec3 vDir = vPos1 - vPos0;
	vDir.Normalize();

	const auto rayFlags = rwi_stop_at_pierceable | rwi_colltype_any;
	ray_hit hit;

	//In building mode, check only for terrain, in selecting for everything
	auto queryflags = mode == PlayerMode::Building ? ent_terrain : ent_all;

	if (gEnv->pPhysicalWorld->RayWorldIntersection(vPos0, vDir * gEnv->p3DEngine->GetMaxViewDistance(), queryflags, rayFlags, &hit, 1))
	{
		m_cursorPositionInWorld = hit.pt;
		if (mode == PlayerMode::Building) {
			if (cursorB != nullptr)
			{
				cursorB->Update(hit.pt, m_currentFloors);
				CheckBuildingZone(hit.pt);
			}
		}
		else if (mode == PlayerMode::Selecting) {
			if (IEntity* hitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(hit.pCollider)) {
				if (Building* hitBuilding = hitEntity->GetComponent<Building>()) {
					SetFocusBuilding(hitBuilding);
				}
				else {
					SetFocusBuilding(NULL);
				}
			}
			else {
				SetFocusBuilding(NULL);
			}
		}
	}
	else
	{
		m_cursorPositionInWorld = ZERO;
	}
}

void CPlayerComponent::UpdateUI(float frameTime) {
	auto debug = gEnv->pGameFramework->GetIPersistantDebug();
	debug->Begin("t1", true);
	debug->AddText(20.0f, 250.0f, 4.0f, ColorF(1, 1, 1), 1.0f, DebugLine);
	switch (mode)
	{
	case PlayerMode::Building:
		//INFO
		debug->AddText(20.0f, 15.0f, 4.0f, ColorF(1, 1, 1), 1.0f, ToString(m_currentFloors) + "-story building, group " + ToString(currentGroup));
		if (!canBuild)
			debug->AddText(20.0f, 50.0f, 4.0f, ColorF(1, 0, 0), 1.0f, "But not here!");
		debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "Buildings: " + ToString(city->buildingsQ()) + ". Groups: " + city->groupsListString() + ".");

		//CONTROLS
		debug->AddText(20.0f, gEnv->pRenderer->GetHeight() - 140.0f, 4.0f, ColorF(1, 1, 1), 1.0f, "[BUILD MODE]  <-space to switch->  selection mode");
		debug->AddText(20.0f, gEnv->pRenderer->GetHeight() - 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "Left click to build. Scrollwheel to change height. 1-9 to change group.");
		break;
	case PlayerMode::Selecting:
	{
		//INFO
			switch (selectionMode)
			{
			case CPlayerComponent::SelectionMode::Single:
				if (selectedBuilding != NULL) {
					debug->AddText(20.0f, 15.0f, 4.0f, ColorF(1, 1, 1), 1.0f, "Selected " + ToString(selectedBuilding->floorsQ) + "-store building");
					debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "Population: " + ToString(selectedBuilding->population));
				}
				else {
					debug->AddText(20.0f, 15.0f, 4.0f, ColorF(1, 1, 1), 1.0f, "No building selected");
					if (city->buildings.size() > 0) {
						debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "Buildings available: " + ToString(city->buildings.size()));
					}
					else {
						debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "You have to build something to select something.");
					}
				}
				break;
			case CPlayerComponent::SelectionMode::Group:
				if (selectedGroup.size() > 0) {
					debug->AddText(20.0f, 15.0f, 4.0f, ColorF(1, 1, 1), 1.0f, "Selected group " + ToString(selectedGroup.front()->group));
					debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, ToString(selectedGroup.size() + 1) + " houses, total population: " + ToString(city->GetPopulationOfGroup(selectedGroup) + selectedBuilding->population));
				}
				else {
					debug->AddText(20.0f, 15.0f, 4.0f, ColorF(1, 1, 1), 1.0f, "No group selected");
					if (city->buildings.size() > 0) {
						debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "Groups available: " + city->groupsListString());
					}
					else {
						debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "You have to build something to select something.");
					}
					
				}
				break;
			case CPlayerComponent::SelectionMode::Distance:
				if (selectedBuilding != NULL) {
					debug->AddText(20.0f, 15.0f, 4.0f, ColorF(1, 1, 1), 1.0f, "Selected " + ToString(selectedBuilding->floorsQ) + "-store building area");
					debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, ToString(selectedGroup.size() + 1) + " houses, " + ToString(city->GetTreesInArea(selectedBuilding->coords, distanceToCheck)) + " trees, total population: " + ToString(city->GetPopulationOfGroup(selectedGroup) + selectedBuilding->population));
					debug->AddPlanarDisc(selectedBuilding->coords + Vec3(0, 0, 2.f), 0.f, 10.f, ColorF(1, 1, 1, 0.5f), 1.0f);
				}
				else {
					debug->AddText(20.0f, 15.0f, 4.0f, ColorF(1, 1, 1), 1.0f, "No building selected");
					if (city->buildings.size() > 0) {
						debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "Buildings available: " + ToString(city->buildings.size()));
					}
					else {
						debug->AddText(20.0f, 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "You have to build something to select something.");
					}
				}
				break;
			default:
				break;
			}

		//CONTROLS
		string selectionModeString = "";
		switch (selectionMode)
		{
		case SelectionMode::Single:
			selectionModeString = "SINGLE";
			break;
		case SelectionMode::Group:
			selectionModeString = "GROUP";
			break;
		case SelectionMode::Distance:
			selectionModeString = "DISTANCE";
			break;
		default:
			break;
		}
		debug->AddText(20.0f, gEnv->pRenderer->GetHeight() - 140.0f, 4.0f, ColorF(1, 1, 1), 1.0f, "build mode  <-space to switch->  [SELECTION MODE: " + selectionModeString + "]");
		debug->AddText(20.0f, gEnv->pRenderer->GetHeight() - 100.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "Left click to select. Shift - group, ctrl - distance.");
		break;
	}
	default:
		break;
	}
	debug->AddText(20.0f, gEnv->pRenderer->GetHeight() - 60.0f, 3.0f, ColorF(1, 1, 1), 1.0f, "W-S-A-D to move camera. Q-E up and down. Mouse to move cursor.");
}

void CPlayerComponent::SetFocusBuilding(Building* next) {
	if (focusBuildingNext != next) {
		focusBuildingPrev = focusBuildingNext;
		focusBuildingNext = next;
		if (focusBuildingPrev != NULL) {
			if (focusBuildingPrev->state != Building::BuildingState::BuildingSelected
				&& focusBuildingPrev->state != Building::BuildingState::BuildingSelectedSecondary)
				focusBuildingPrev->Update(Building::BuildingState::BuildingDefault);
		}
		if (focusBuildingNext != NULL) {
			if (focusBuildingNext->state != Building::BuildingState::BuildingSelected
				&& focusBuildingNext->state != Building::BuildingState::BuildingSelectedSecondary)
				focusBuildingNext->Update(Building::BuildingState::BuildingHighlighted);
		}
	}
}

void CPlayerComponent::SelectBuilding(Building* b) {
	SetFocusBuilding(NULL);
	if (selectedBuilding != b) {
		if (selectedBuilding != NULL)
			DeselectBuilding();
		selectedBuilding = b;
		selectedBuilding->Update(Building::BuildingState::BuildingSelected);
		switch (selectionMode)
		{
		case SelectionMode::Group:
		{
			selectedGroup = city->GetGroup(selectedBuilding->group);
			selectedGroup.remove(selectedBuilding);
			for each (Building* b2 in selectedGroup)
				b2->Update(Building::BuildingState::BuildingSelected);
		}
		break;
		case SelectionMode::Distance:
		{
			auto group = city->GetByDistance(b, distanceToCheck);
			for each (std::tuple<Building*, float> b2 in group)
			{
				std::get<0>(b2)->Update(Building::BuildingState::BuildingSelectedSecondary);
				selectedGroup.push_front(std::get<0>(b2));
			}
		}
		break;
		}
	}
}

void CPlayerComponent::DeselectBuilding() {
	if (selectedBuilding != NULL) {
		selectedBuilding->Update(Building::BuildingState::BuildingDefault);
		selectedBuilding = NULL;
	}
	if (selectedGroup.size() > 0) {
		for each (Building* b in selectedGroup)
			b->Update(Building::BuildingState::BuildingDefault);
		selectedGroup.clear();
	}
}

Vec2 CPlayerComponent::GetScreenCoords(float x, float y) {
	Vec2 coords = Vec2(x, y);
	if (x < 0) {
		coords.x = gEnv->pRenderer->GetWidth() + x;
	}
	if (y < 0) {
		coords.x = gEnv->pRenderer->GetHeight() + y;
	}
	return coords;
}

void CPlayerComponent::CheckBuildingZone(Vec3 hitPoint) {
	canBuild = city->GetBuildPermission(hitPoint);
	cursorB->Update(canBuild ? Building::BuildingState::cursorEnabled : Building::BuildingState::cursorDisabled);
}

void CPlayerComponent::HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type)
{
	switch (type)
	{
	case EInputFlagType::Hold:
	{
		if (activationMode == eIS_Released)
		{
			m_inputFlags &= ~flags;
		}
		else
		{
			m_inputFlags |= flags;
		}
	}
	break;
	case EInputFlagType::Toggle:
	{
		if (activationMode == eIS_Released)
		{
			// Toggle the bit(s)
			m_inputFlags ^= flags;
		}
	}
	break;
	}
}