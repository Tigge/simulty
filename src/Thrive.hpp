
#ifndef _THRIVE_HPP_
#define _THRIVE_HPP_

#include "Point.hpp"

class Thrive {

  public:

  static const unsigned char TYPE_CRIME        = 0;
  static const unsigned char TYPE_FIRE         = 1;
  static const unsigned char TYPE_HEALTH       = 2;
  
  static const unsigned char TYPE_ELECTRICITY  = 3;
  static const unsigned char TYPE_WATER        = 4;
  static const unsigned char TYPE_ENVIRONMENT  = 5;
  static const unsigned char TYPE_CONNECTIVITY = 6;
  static const unsigned char TYPE_TAXES        = 7;
  
  static const unsigned char TYPE_JOBS         = 8;
  static const unsigned char TYPE_COMMERSE     = 9;
  static const unsigned char TYPE_WORKERS      = 10;

  static const unsigned char TYPE_COUNT        = 11;

  public:

  double thrive[TYPE_COUNT];
  
  public:
  
  Thrive() {
    for(int i = 0; i < TYPE_COUNT; i++) {
      thrive[i] = 0.0;
    }
  }
  
};

#endif

