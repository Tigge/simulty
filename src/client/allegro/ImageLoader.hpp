
#ifndef _IMAGELOADER_HPP_
#define _IMAGELOADER_HPP_

#include "AllegroGUIShared.hpp"

#include <iostream>
#include <string>

#include "ImageLoaderException.hpp"

class ImageLoader {

  public:

  static BITMAP *getImage(const char *file) {  
    BITMAP *b = load_bitmap(file, NULL);    

    // Error:
    if(b == NULL) {
      throw ImageLoaderException(file);
    // Success:
    } else {
    
      int green = makecol(0, 255, 0);
      int pink  = makecol(255, 0, 255);

      for(int x = 0; x < b->w; x++) {
        for(int y = 0; y < b->h; y++) {
          if((int)getpixel(b, x, y) == green)putpixel(b, x, y, pink);
        }
      }
      return b;
    }
  }


  static BITMAP *getImage(std::string file) {
    return getImage(file.c_str());
  
  }


};


#endif
