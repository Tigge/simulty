
#include "BuildingFactory.hpp"

#include "BuildingPolice.hpp"
#include "BuildingFire.hpp"
#include "BuildingHospital.hpp"
#include "BuildingPowerplant.hpp"

#include "BuildingResidential.hpp"
#include "BuildingCommersial.hpp"
#include "BuildingIndustrial.hpp"

#include "SimultyException.hpp"
#include "Date.hpp"


bool BuildingFactory::inited = false;
std::map<int, BuildingInformation *> BuildingFactory::buildings;

void BuildingFactory::init() {

    const char *files[] = {"CrimePoliceStation.xml", "CrimePrison.xml", 
            "FireStation.xml", "HealthClinic.xml", "HealthHospital.xml",
            "PowerplantCoal.xml", "PowerplantNuclear.xml"};

    if(!inited) {
    
        for(int i = 0; i < 7; i++) {
            std::string file = std::string("resources/buildings/special/") 
                    + files[i];
            BuildingInformation *bi = BuildingLoader::loadBuildingInformation(
                    file);
            buildings[bi->id] = bi;
        }
        inited = true;
    }
    
}

void BuildingFactory::loadBuildingInformation(std::string file) {



}


Building *BuildingFactory::getBuilding(int type, Point position,
    unsigned char owner, Date built) {


  switch(type) {

    case Building::TYPE_POLICE: {
      return new BuildingPolice(position, owner, built);
    }

    case Building::TYPE_FIRE: {
      return new BuildingFire(position, owner, built);
    }

    case Building::TYPE_HOSPITAL: {
      return new BuildingHospital(position, owner, built);
    }
    
    case Building::TYPE_POWERPLANT: {
      return new BuildingPowerplant(position, owner, built);
    }

  }

  throw SIMULTYEXCEPTION("No such building type");
}

BuildingZone *BuildingFactory::getBuildingZone(int type, Point position,
    unsigned char owner, int w, int h, Date built, int level, int style) {
  
  switch(type) {

    case Building::TYPE_RESIDENTIAL: {
      return new BuildingResidential(position, owner, w, h, built, level, style);
    }
    case Building::TYPE_COMMERSIAL: {
      return new BuildingCommersial(position, owner, w, h, built, level, style);
    }
    case Building::TYPE_INDUSTRIAL: {
      return new BuildingIndustrial(position, owner, w, h, built, level, style);
    }
  }
  throw SIMULTYEXCEPTION("No such zone building type");
  
}


