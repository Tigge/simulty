#ifndef _ALLEGROGUISHARED_HPP_
#define _ALLEGROGUISHARED_HPP_

#define GLYPH_TARGET GLYPH_TARGET_ALLEGRO

// Allegro
#include "allegro.h"
#ifdef WIN32
  #include "winalleg.h"
#endif

// Guichan
#include <guichan.hpp>
#include <guichan/allegro.hpp>

// Network library
#include "NL.hpp"

// Other shared stuff (from server / client)
#include "../../shared.h"


#endif
