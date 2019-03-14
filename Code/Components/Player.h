#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <CryMath/Cry_Camera.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Input/InputComponent.h>

#include "Building.h"
#include "CityManager.h"

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CPlayerComponent final : public IEntityComponent
{
	enum class EInputFlagType
	{
		Hold = 0,
		Toggle
	};

	typedef uint8 TInputFlags;

	enum class EInputFlag
		: TInputFlags
	{
		MoveLeft = 1 << 0,
		MoveRight = 1 << 1,
		MoveForward = 1 << 2,
		MoveBack = 1 << 3,
		MoveUp = 1 << 4,
		MoveDown = 1 << 5
	};

public:

	enum class PlayerMode {
		Building,
		Selecting
	};

	enum class SelectionMode {
		Single,
		Group,
		Distance
	};

	PlayerMode mode;
	SelectionMode selectionMode = SelectionMode::Single;
	bool canBuild = true;

	CPlayerComponent() = default;
	virtual ~CPlayerComponent() {}

	// IEntityComponent
	virtual void Initialize() override;
	void BuildCursorCreation();
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	void NumKeyPress(int num);

	void ModeSwitch();
	void ModeSet(PlayerMode _mode);
	
	void OnClick();
	void ChangeFloorsNumber(int toAdd);
	
	// ~IEntityComponent

	Building* cursorB;
	CityManager* city;

	string DebugLine = "";

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc)
	{
		desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14725}"_cry_guid);
	}

protected:

	int m_currentFloors = 1;
	int currentGroup = 1;
	Building* selectedBuilding;
	Building* focusBuildingPrev;
	Building* focusBuildingNext;
	std::list<Building*> selectedGroup;

	void SetFocusBuilding(Building* next);
	void SelectBuilding(Building * b);
	void DeselectBuilding();

	Vec2 moveSpeed = Vec2(20.f, 10.f);
	Vec2 verticalLimits = Vec2(28.f, 50.f);

	Vec3 startPosition = Vec3(64, 64, 38);
	float cameraAngle = -70.f;
	float viewDistanceFromPlayer = 5.0f;

	float distanceToCheck = 10.0f;

	void BuildBuilding();

	void UpdateMovementRequest(float frameTime);
	void UpdateCursor(float frameTime);
	void UpdateUI(float frameTime);

	Vec2 GetScreenCoords(float x, float y);

	void CheckBuildingZone(Vec3 hitPoint);

	void HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type = EInputFlagType::Hold);

protected:
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;

	TagID m_walkTagId;

	TInputFlags m_inputFlags = 0;

	Vec2 m_mouseDeltaRotation = ZERO;
	Vec3 m_cursorPositionInWorld = ZERO;

	IEntity* m_pFloorEntity = nullptr;
	IEntity* m_pEntranceEntity = nullptr;
};
