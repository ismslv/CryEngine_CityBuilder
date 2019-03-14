#include "CityManager.h"

#include <CryRenderer/IRenderAuxGeom.h>
#include <CryGame/IGameFramework.h>

#include <random>
#include <iostream>


CityManager::CityManager()
{
	//Error-proof building floors limits
	Building::floorsMinMax.y = CLAMP(Building::floorsMinMax.y, 1, Building::floorsAll);
	Building::floorsMinMax.x = CLAMP(Building::floorsMinMax.x, 1, Building::floorsMinMax.y);
	Building::floorsDefault = CLAMP(Building::floorsDefault, Building::floorsMinMax.x, Building::floorsMinMax.y);
	
	//Loading materials
	materials[Building::BuildingState::cursorEnabled] = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/BuildingNotPlaced", true, true);
	materials[Building::BuildingState::cursorDisabled] = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/BuildingDisabled", true, true);
	materials[Building::BuildingState::BuildingDefault] = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/Building", true, true);
	materials[Building::BuildingState::BuildingHighlighted] = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/BuildingHighlighted", true, true);
	materials[Building::BuildingState::BuildingSelected] = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/BuildingSelected", true, true);
	materials[Building::BuildingState::BuildingSelectedSecondary] = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/BuildingSelectedSecondary", true, true);
}

CityManager::Materials CityManager::materials = {};

void CityManager::Build(int floors, Vec3 coords, int group) {
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	IEntity* entityB = gEnv->pEntitySystem->SpawnEntity(spawnParams);
	Building* newB = entityB->GetOrCreateComponentClass<Building>(floors, coords);
	newB->group = group;
	buildings.push_back(newB);
}

void CityManager::GrowTrees(Vec3 center, int radius, int q) {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 gen(rd()); // seed the generator
	std::uniform_int_distribution<> distr(-radius, radius);
	std::uniform_int_distribution<> distr2(8, 30);

	//Vec3 pointStart = Vec3(64, 64, 32);
	for (int i = 0; i < q; i++) {
		Vec3 pointRand = Vec3(center.x + distr(gen), center.y + distr(gen), center.z);
		GrowTree(pointRand, distr2(gen)/10.f);
	}
}

void CityManager::GrowTree(Vec3 pos, float scale) {
	SEntitySpawnParams spawnParams;
	spawnParams.pClass = gEnv->pEntitySystem->GetClassRegistry()->GetDefaultClass();
	IEntity* entity = gEnv->pEntitySystem->SpawnEntity(spawnParams);
	entity->LoadGeometry(0, "Objects/tree.cgf");
	entity->SetScale(Vec3(scale));
	entity->SetName("Tree");
	entity->SetPos(pos);
	trees.push_front(pos);
	SEntityPhysicalizeParams physicParams;
	physicParams.mass = 0;
	physicParams.density = 1;
	physicParams.type = PE_STATIC;
	entity->Physicalize(physicParams);
	entity->SetMaterial(gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/Tree"));
}

void CityManager::UpdatePopulation()
{
	for each (Building* b in buildings)
	{
		b->UpdatePopulation();
	}
}

int CityManager::GetPopulationOfGroup(std::list<Building*> group) {
	int population = 0;
	for each (Building* b in group)
		population += b->population;
	return population;
}

bool CityManager::GetBuildPermission(Vec3 hitPoint) {
	// Simple method
	/*
	bool tooclose = false;

	for each (Building* b in buildings)
	{
		if (hitPoint.GetDistance(b->coords) < 5.0f) {
			tooclose = true;
		}
	}
	return !tooclose;
	*/

	// More precise and heavy method
	ray_hit hit;
	float size = 1.f;
	for (int x = -2; x <= 2; x++) {
		for (int y = -2; y <= 2; y++) {
			Vec3 to = hitPoint + Vec3(x*size, y*size, 0.f);
			Vec3 from = to;
			from.z += 40.f;
			Vec3 dir = (to - from).Normalize() * 40.f;
			if (gEnv->pPhysicalWorld->RayWorldIntersection(from, dir, ent_all, rwi_colltype_any, &hit, 1)) {
				if (IEntity* hitEntity = gEnv->pEntitySystem->GetEntityFromPhysics(hit.pCollider)) {
					return false;
				}
			}
		}
	}
	return true;
}

int CityManager::buildingsQ()
{
	return buildings.size();
}

std::vector<int> CityManager::groupsList() {
	std::vector<int> list = {};
	for each (Building* b in buildings)
	{
		if (std::find(list.begin(), list.end(), b->group) == list.end()) {
			list.push_back(b->group);
		}
	}
	return list;
}

string CityManager::groupsListString(string divider = ", ") {
	string line = "";
	if (buildingsQ() > 0) {
		std::vector<int> list = groupsList();
		for (int i = 0; i < list.size(); i++) {
			line += ToString(list[i]);
			if (i < list.size() - 1) {
				line += divider;
			}
		}
	}
	return line;
}

std::list<Building*> CityManager::GetGroup(int groupN) {
	std::list<Building*> group;
	for each (Building* b in buildings)
	{
		if (b->group == groupN)
			group.push_back(b);
	}
	return group;
}

std::list<std::tuple<Building*, float>> CityManager::GetByDistance(Building* start, float limit) {
	std::list<std::tuple<Building*, float>> group;
	for each (Building* b in buildings)
	{
		if (b != start) {
			float distance = start->coords.GetDistance(b->coords);
			if (distance < limit) {
				group.push_back(std::make_tuple(b, distance));
			}
		}
	}
	std::stable_sort(std::begin(group), std::end(group), sortListByDistance);
	return group;
}

int CityManager::GetTreesInArea(Vec3 start, float limit) {
	int q = 0;
	for each (Vec3 v in trees)
	{
		float distance = start.GetDistance(v);
		if (distance < limit)
			q++;
	}
	return q;
}

bool sortListByDistance(const std::tuple<Building*, float> &t1, const std::tuple<Building*, float> &t2) {
	return (std::get<1>(t1) < std::get<1>(t2));
}

//Random integer between bounds
int CityManager::rangeRandomAlg2(int min, int max) {
	int n = max - min + 1;
	int remainder = RAND_MAX % n;
	int x;
	do {
		x = rand();
	} while (x >= RAND_MAX - remainder);
	return min + x % n;
}