#pragma once

////////////////////////////////////////////////////////
// A structure that manages buildings
////////////////////////////////////////////////////////

class CityManager
{
public:
	CityManager();

	void Build(int floors, Vec3 coords, int group);

	void GrowTrees(Vec3 center, int radius, int q);

	void GrowTree(Vec3 pos, float scale);

	void UpdatePopulation();

	int GetPopulationOfGroup(std::list<Building*> group);

	bool GetBuildPermission(Vec3 hitPoint);

	std::list<Building*> buildings = {};
	typedef std::map<Building::BuildingState, IMaterial*> Materials;
	static Materials materials;
	std::list<Vec3> trees = {};

	int buildingsQ();
	std::vector<int> groupsList();
	string groupsListString(string divider);

	std::list<Building*> GetGroup(int groupN);
	std::list<std::tuple<Building*, float>> GetByDistance(Building* start, float limit);

	int GetTreesInArea(Vec3 start, float limit);

	static int rangeRandomAlg2(int min, int max);
};

bool sortListByDistance(const std::tuple<Building*, float>& t1, const std::tuple<Building*, float>& t2);
