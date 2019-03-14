#pragma once
#include <CryEntitySystem/IEntityComponent.h>
#include <CryEntitySystem/IEntitySystem.h>

////////////////////////////////////////////////////////
// Represents a building
////////////////////////////////////////////////////////

class Building : public IEntityComponent
{
public:
	Building(int _floorsQ, Vec3 _coords, bool _isCursor = false);
	virtual void Initialize() override;

	enum class BuildingState
	{
		cursorEnabled,
		cursorDisabled,
		BuildingDefault,
		BuildingHighlighted,
		BuildingSelected,
		BuildingSelectedSecondary
	};

	std::map<Building::BuildingState, IMaterial*> materialsBuilding;

	BuildingState state = BuildingState::BuildingDefault;

	bool isCursor;
	int floorsQ = 1;
	EntityId ID;
	IEntity* childEntrance;
	Vec3 coords;
	int group;
	int population = 0;

	static  Vec2i floorsMinMax;
	static  float floorHeight;
	static const int floorsAll;
	static  int floorsDefault;

	void Spawn(Vec3 pos);
	void Update(Vec3 pos, int _floorsQ);
	void Update(BuildingState _state);
	void Update();
	void UpdatePopulation();

	float GetPosHeight();

	void RenderMaterial();

	static void ReflectType(Schematyc::CTypeDesc<Building>& desc)
	{
		desc.SetGUID("{63F4C0C6-32AF-4ACB-8FB0-57D45DD14726}"_cry_guid);
	}
};