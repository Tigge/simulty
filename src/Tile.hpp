#ifndef _TILE_HPP_
#define _TILE_HPP_


class Tile {
  // Associations
  // Attributes
  private:
    char owner;
    unsigned char terrain;
    unsigned char zone;
    unsigned char house;
    bool road;
  // Operations  
  public:
    Tile (  );
    virtual ~Tile (  );
        
    virtual char getOwner();
    virtual char getTerrain();
    virtual char getZone();
    virtual char getHouse();
    
    virtual bool isRoad();
};

#endif
