#ifndef _THRIVEMAP_HPP_
#define _THRIVEMAP_HPP_

#include <vector>
#include "Thrive.hpp"

class ThriveMap {

  private:

  int width, height;
  
  protected:

  // Thrive factors (increase / decrase rate)
  double thriveFactors[Thrive::TYPE_COUNT];

  // Thrive values (per tile):
  std::vector<Thrive> thrive;

  
  public:
  
  ThriveMap(int width, int height);
  ~ThriveMap();
  
  double getThrive(int x, int y, int type);
  void   setThrive(int x, int y, int type, double value);
  void   updateThrive(int x, int y, int type, double value);
  
  double getThriveFactor(int type);
  void   setThriveFactor(int type, double value);

};

#endif


