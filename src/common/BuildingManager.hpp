
#ifndef _BUILDINGMANAGER_H_
#define _BUILDINGMANAGER_H_

#include <vector>
#include "Map.hpp"
#include "Building.hpp"
#include "BuildingZone.hpp"
#include "BuildingFactory.hpp"

#include "Date.hpp"
#include "SimultyException.hpp"

class BuildingManager {

  protected:
    std::vector<BuildingZone *> zoneBuildings;
    std::vector<Building *> specialBuildings;

  public:
    virtual ~BuildingManager();

    // Buildings:
    void          addSpecialBuilding(Map *, Building *);
    void          removeSpecialBuilding(Map *, unsigned int id);

    Building     *getSpecialBuilding(int i);
    unsigned int  getSpecialBuildingCount();
    int           getSpecialBuildingID(Point at);

    BuildingZone *getZoneBuilding(int i);
    unsigned int  getZoneBuildingCount();
    int           getZoneBuildingID(Point at);

    bool          canBuildSpecialBuilding(Building *b, unsigned char slot, Map *m);
    bool          canBuild(Point at, unsigned char slot, Map *m);

    void          updateZoneBuildings(unsigned char player_slot, Map *map);
    virtual void  addZoneBuilding(Map *, unsigned char owner, int buildingType,
        Point p, int w, int h, Date built, int level, int style);
    void          removeZoneBuilding(Map *, unsigned int id);
    
    int           bulldozeCost(unsigned char player_slot, Map *, Point from, Point to);
    void          bulldoze(unsigned char player_slot, Map *, Point from, Point to);
};

#endif
