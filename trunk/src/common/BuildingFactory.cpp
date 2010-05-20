
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
            BuildingInformation *bi = BuildingInformation::load(file);
            buildings[bi->getId()] = bi;
            std::cout << "Adding " << bi->getId() << ": " << bi << std::endl;
        }
        inited = true;
    }
    
}


Building *BuildingFactory::getBuilding(int id, Point position,
    unsigned char owner, Date built) {
    
    /*std::cerr << "getting building with id " << id << std::endl;
    std::cerr << "map is " << buildings.size() << " size" << std::endl;
    std::cerr << "pointer (40):  " << buildings[40] << std::endl;
    std::cerr << "pointer (200): " << buildings[200] << std::endl;
    std::cout << "buildings contains:\n";

    for (std::map<int, BuildingInformation *>::iterator it=buildings.begin() ; it != buildings.end(); it++ )
      std::cout << (*it).first << " => " << (*it).second << std::endl;*/
    
    std::map<int, BuildingInformation *>::iterator it = buildings.find(id);
    
    if(it == buildings.end()) {
        throw SIMULTYEXCEPTION("No such building id was loaded");
    }
    
    BuildingInformation *bi = it->second;
    
    switch(bi->getType()) {
        case Building::TYPE_POLICE: {
            return new BuildingPolice(bi, position, owner, built);
        }

        case Building::TYPE_FIRE: {
            return new BuildingFire(bi, position, owner, built);
        }

        case Building::TYPE_HOSPITAL: {
            return new BuildingHospital(bi, position, owner, built);
        }
        
        case Building::TYPE_POWERPLANT: {
            return new BuildingPowerplant(bi, position, owner, built);
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


