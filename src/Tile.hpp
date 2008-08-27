#ifndef _TILE_HPP_
#define _TILE_HPP_


const unsigned char SIMULTY_ZONE_NONE        = 0;
const unsigned char SIMULTY_ZONE_RES         = 10;
const unsigned char SIMULTY_ZONE_COM         = 11;
const unsigned char SIMULTY_ZONE_IND         = 12;

const unsigned char SIMULTY_TERRAIN_GRASS    = 0;
const unsigned char SIMULTY_TERRAIN_WATER    = 1;

const unsigned char SIMULTY_BUILDING_NONE    = 0;
const unsigned char SIMULTY_BUILDING_SPECIAL = 1;
const unsigned char SIMULTY_BUILDING_ZONE    = 2;

class Tile {
  // Associations
  // Attributes
  private:
    char owner;
    unsigned char terrain;
    unsigned char zone;
    unsigned char building;
    bool road;
  // Operations
  public:
    Tile (  );
    virtual ~Tile (  );

    virtual char          getOwner();
    virtual void          setOwner(char owner);

    virtual unsigned char getTerrain();

    virtual unsigned char getZone();
    virtual void          setZone(unsigned char zone);

    virtual unsigned char getBuilding();
    virtual void          setBuilding(unsigned char building);

    virtual bool          isRoad();
    virtual void          setRoad(bool road);
    
    virtual bool          isOccupied();
};

#endif

