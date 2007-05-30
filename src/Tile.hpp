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
    virtual void setOwner(char owner);
    
    virtual unsigned char getTerrain();    
    
    virtual unsigned char getZone();
    virtual void setZone(unsigned char zone);
    
    virtual unsigned char getHouse();
    
    virtual bool isRoad();
    virtual void setRoad(bool road);
};

#endif
