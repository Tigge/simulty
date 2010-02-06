#ifndef _THRIVEMAP_HPP_
#define _THRIVEMAP_HPP_

#include <vector>
#include "Thrive.hpp"

class ThriveMap {

  private:

  int width, height;
  
  protected:
  // Thrive combine factors
  double thriveCombine[Thrive::TYPE_COUNT];

  // Thrive factors (increase / decrase rate)
  double thriveFactors[Thrive::TYPE_COUNT];

  // Thrive values (per tile):
  std::vector<Thrive> thrive;

  
  public:
  
  ThriveMap(int width, int height);
  ~ThriveMap();
  
  double getThrive(Point p);
  
  double getThrive(Point p, int type);
  void   setThrive(Point p, int type, double value);
  void   updateThrive(Point p, int type, double value);
  
  double getThriveFactor(int type);
  void   setThriveFactor(int type, double value);

};

#endif


