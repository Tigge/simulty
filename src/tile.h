#ifndef _TILE_H_
#define _TILE_H_


class tile {
  // Associations
  // Attributes
  public:
    char owner;
    unsigned char terrain;
    unsigned char zone;
    unsigned char house;
    bool road;
  // Operations
  public:
    tile (  );
    ~tile (  );
};

#endif
