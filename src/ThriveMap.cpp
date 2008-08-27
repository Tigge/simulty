
#include <iostream>
#include "ThriveMap.hpp"

ThriveMap::ThriveMap(int width, int height) {

  // Set width and height of thrive map
  this->width  = width;
  this->height = height;
  
  // Set keep factor to 0.80
  for(int i = 0; i < Thrive::TYPE_COUNT; i++) {
    thriveFactors[i] = 0.80;
  }
  
  
  
  // Set combine factor to 1 / Thrive::TYPE_COUNT
  // TODO: make real. Electricity and roads should be most important
  for(int i = 0; i < Thrive::TYPE_COUNT; i++) {
    thriveCombine[i] = 1.0 / Thrive::TYPE_COUNT;
  }
  
  // Add all thrives
  for(int i = 0; i < width * height; i++) {
    thrive.push_back(Thrive());
  }
}

ThriveMap::~ThriveMap() {

}


double ThriveMap::getThrive(int x, int y) {
  double t = 0.0;  
  for(int i = 0; i < Thrive::TYPE_COUNT; i++) {
    t += thrive[x + y * width].thrive[i] * thriveCombine[i];
  }
  return t;
}

double ThriveMap::getThrive(int x, int y, int type) {
  return thrive[x + y * width].thrive[type];
}

void ThriveMap::setThrive(int x, int y, int type, double value) {

}

void ThriveMap::updateThrive(int x, int y, int type, double value) {
  thrive[x + y * width].thrive[type] = 
      thrive[x + y * width].thrive[type] * thriveFactors[type] 
      + value * (1 - thriveFactors[type]);
  //std::cout << " -- " << x << ", " << y << " = " << type << " = " << value << std::endl;
}
  
double ThriveMap::getThriveFactor(int type) {

}

void ThriveMap::setThriveFactor(int type, double value) {

}


