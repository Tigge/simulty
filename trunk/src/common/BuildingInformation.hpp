#ifndef _BUILDINGINFORMATION_HPP_
#define _BUILDINGINFORMATION_HPP_

#include <string>

class BuildingInformation {

    private:

    unsigned char type;
    int id;

    std::string name;
    std::string description;

    int costBuy, costDemolish, costUpkeep;
    unsigned char sizeWidth, sizeHeight;
    
    std::string graphicLocation;

    static unsigned char typeStringToTypeNumber(std::string);
    
    BuildingInformation();
    BuildingInformation(const BuildingInformation&);
    BuildingInformation & operator=(const BuildingInformation &);
    
    public:
    
    int           getId();
    unsigned char getType();
    unsigned char getWidth();
    unsigned char getHeight();
    
    int           getBuyCost();
    int           getDemolishCost();
    int           getUpkeepCost();
    
    static BuildingInformation *load(std::string);
    
};

#endif

