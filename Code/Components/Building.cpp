#include "Building.h"
#include <CryRenderer/IRenderAuxGeom.h>
#include "CityManager.h"

Building::Building(int _floorsQ, Vec3 _coords, bool _isCursor) : floorsQ(_floorsQ), coords(_coords), isCursor(_isCursor)
{
	state = _isCursor ? BuildingState::cursorEnabled : BuildingState::BuildingDefault;
}

void Building::Initialize() {
	Spawn(coords);
}

void Building::Spawn(Vec3 pos) {

	GetEntity()->LoadGeometry(0, "Objects/building.cgf");
	GetEntity()->SetScale(Vec3(1.0f));
	GetEntity()->SetName("Building");

	SEntitySpawnParams spawnParamsEntrance;
	spawnParamsEntrance.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	spawnParamsEntrance.pParent = GetEntity();
	childEntrance = gEnv->pEntitySystem->SpawnEntity(spawnParamsEntrance);
	childEntrance->LoadGeometry(0, "Objects/BuildingEntrance.cgf");
	childEntrance->SetScale(Vec3(1.0f));

	if (!isCursor) {
		SEntityPhysicalizeParams physicParams;
		physicParams.mass = 0;
		physicParams.density = 1;
		physicParams.type = PE_STATIC;
		GetEntity()->Physicalize(physicParams);

		pe_action_awake wake;
		wake.bAwake = 1;
		GetEntity()->GetPhysics()->Action(&wake);
	}

	coords = pos;

	ID = GetEntity()->GetId();

	Update();
}

void Building::Update(Vec3 pos, int _floorsQ)
{
	coords = pos;
	floorsQ = _floorsQ;
	Update();
}

void Building::Update(BuildingState _state)
{
	state = _state;
	RenderMaterial();
}

void Building::Update() {
	float heightD = GetPosHeight();
	GetEntity()->SetPos(Vec3(coords.x, coords.y, coords.z + heightD));
	childEntrance->SetPos(Vec3(0, 0, -heightD));
	RenderMaterial();
}

void Building::UpdatePopulation() {
	//for each floor
	for (int i = 0; i < floorsQ; i++)
	//there is one chance from 5
		if (CityManager::rangeRandomAlg2(0, 5) == 0)
	//that population will add 1 to 3 people
			population += CityManager::rangeRandomAlg2(1, 3);
}

float Building::GetPosHeight() {
	return (Building::floorsAll + 1 - floorsQ) * -Building::floorHeight;
}

void Building::RenderMaterial() {
	GetEntity()->SetMaterial(CityManager::materials[state]);
	childEntrance->SetMaterial(CityManager::materials[state]);
}

Vec2i Building::floorsMinMax = Vec2i(1, 11);
float Building::floorHeight = 1.065f;
const int Building::floorsAll = 11;
int Building::floorsDefault = 1;