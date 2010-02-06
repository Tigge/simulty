
#ifndef _IMAGELOADER_HPP_
#define _IMAGELOADER_HPP_

#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdio>

#include "SDL.h"
#include "SDL_image.h"

#include "ImageLoaderException.hpp"
#include "ImageUtils.hpp"

class ImageLoader {

  public:

  static SDL_Surface *getImage(const char *file) {  
    
    SDL_Surface *surface = IMG_Load(file);
    // Error:
    if(!surface) 
    {
      throw ImageLoaderException(file, "IMG_Load: %s\n", IMG_GetError());
    // Success:
    } else {    
      surface = optimize(surface);
      surface = clean(surface);
      return surface;
    }
  }


  static SDL_Surface *getImage(std::string file) {
    return getImage(file.c_str());
  }

  private:

  /**
   * Clean a surface
   *
   * This function converts the graphic drawing helper colors used when 
   * developing the art. More specifically it converts the colors 0xff00ff 
   * (pure magenta) and 0x00ff00 (pure green) to 0x00000000 (fully transparent).
   *
   * @param   The surface to clean
   * @return  The cleaned surface
   */
  static SDL_Surface *clean(SDL_Surface *surface) {

    Uint32 magenta = SDL_MapRGB(surface->format, 0xff, 0x00, 0xff);
    Uint32 green   = SDL_MapRGB(surface->format, 0x00, 0xff, 0x00);
    Uint32 transp  = SDL_MapRGBA(surface->format, 0x00, 0x00, 0x00, 0x00);

    for(int x = 0; x < surface->w; x++) {
      for(int y = 0; y < surface->h; y++) {
        Uint32 color = ImageUtils::getpixel(surface, x, y);
        if(color == magenta || color == green) {
          ImageUtils::putpixel(surface, x, y, transp);
        }
      }
    }
    
    return surface;
  }
  
  /**
   * Optimize a surface for screen drawing
   *
   * This function converts the specified surface to a format that is better
   * for drawing to the screen. It also adds an alpha channel.
   *
   * @param   The surface to optimize
   * @return  The optimized surface
   */
  static SDL_Surface *optimize(SDL_Surface *surface) {
    
    SDL_Surface *optSurface = SDL_DisplayFormatAlpha(surface);
    SDL_FreeSurface(surface);
    return optSurface;
  }

};

#endif
