
#ifndef _IMAGELOADER_HPP_
#define _IMAGELOADER_HPP_

#include "allegro.h"
#ifdef WIN32
  #include "winalleg.h"
#endif

#include "ImageLoaderException.hpp"

class ImageLoader {

  public:

  static BITMAP *getImage(char *file) {  
    BITMAP *b = load_bitmap(file, NULL);    
    
    // Error:
    if(b == NULL) {
      throw ImageLoaderException(file);
      
    // Success:
    } else {
    
      int green = makecol(0, 255, 0);
      int pink  = makecol(255, 0, 255);

      for(int x = 0; x < b->w; x++)
        for(int y = 0; y < b->h; y++)
        {
          if(((short *)b->line[y])[x] == green)((short *)b->line[y])[x] = pink;
        }
    
      return b;
    }
    
  }


  static BITMAP *getImage(std::string file) {
    return getImage(file.c_str());
  
  }


};


#endif
