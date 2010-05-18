
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

void BuildingFactory::init() {

    if(!inited) {
    
        loadBuildingInformation("resources/PowerplantCoal.xml");    
        inited = true;
    }
    
}

void BuildingFactory::loadBuildingInformation(std::string file) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    LIBXML_TEST_VERSION

    doc = xmlReadFile(file.c_str(), NULL, 0);
    if (doc == NULL) {
        std::cerr << "error: could not parse file " << file;
    }

    root_element = xmlDocGetRootElement(doc);
    xmlFreeDoc(doc);
    xmlCleanupParser();

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


