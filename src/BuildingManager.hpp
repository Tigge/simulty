
#ifndef _BUILDINGMANAGER_H_
#define _BUILDINGMANAGER_H_

#include <vector>
#include "Map.hpp"
#include "Building.hpp"
#include "BuildingZone.hpp"

#include "Date.hpp"


class BuildingManager {

  protected:
    std::vector<BuildingZone *> zoneBuildings;
    std::vector<Building *> specialBuildings;

  public:
    virtual ~BuildingManager();

    // Thrive values:
    /*int getCrimeThrive(Map *m, char slot, Point where);
    int getConnectionThrive(Map *m, char slot, Point where);
    int getElectricityThrive(Map *m, char slot, Point where);
    int getTaxesThrive(char slot);

    int getCommersialThrive(Map *m, char slot, Point where);
    int getIndustrialThrive(Map *m, char slot, Point where);
    int getResidentialThrive(Map *m, char slot, Point where);

    int getEnvoirmentThrive(Map *m, char slot, Point where); 

    int getThrive(Map *m, char slot, Point where);

    unsigned int getThriveLevel(Map *m, char owner, Point where);
    unsigned int getThriveLevel(int thrive); */

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
};

#endif
