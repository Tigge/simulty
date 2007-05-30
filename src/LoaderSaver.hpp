
#ifndef _LOADERSAVER_HPP_
#define _LOADERSAVER_HPP_

#include <string>

class LoaderSaver {

  public:

  static NLINT32 readInt32(std::string from, unsigned int pos) {


    if(from.length() >= pos + 4) {
      unsigned char b[4]; for(int i = 0; i < 4; i++)b[i] = from[pos + i];
      return ntohl( *((NLINT32 *)b) );
    } else {
      throw pos;
    }
  }

  static std::string writeInt32(std::string to, NLINT32 i) {

    std::string toNew(to);
    unsigned char b[4]; *((NLINT32 *)b) = htonl(i);
    for(int i = 0; i < 4; i++)toNew.push_back((char)b[i]);
    return toNew;
  }


  static std::string readString(std::string from, unsigned int pos) {

    std::string s;
    for(unsigned int i = 0; i < from.length() - pos && from[pos + i] != '\0'; i++) {
      s.push_back(from[pos + i]);
    }
    return s;
  }

  static std::string writeString(std::string to, std::string s) {
    std::string toNew(to);
    toNew.append(s);
    toNew.push_back('\0');
    return toNew;
  }




  static std::string saveGame( Map *m, PlayerManager *pm, BuildingManager *bm) {

    std::string s;

    s = writeInt32(s, m->getWidth());
    s = writeInt32(s, m->getHeight());
    for(int x = 0; x < m->getWidth(); x++) {
      for(int y = 0; y < m->getHeight(); y++) {
        s = writeInt32(s, m->getTile(x, y)->getOwner());
        s = writeInt32(s, m->getTile(x, y)->getTerrain());
        s = writeInt32(s, m->getTile(x, y)->getZone());
        s = writeInt32(s, m->getTile(x, y)->getHouse());
        s = writeInt32(s, m->getTile(x, y)->isRoad());
      }
    }

    return s;
  }


  static void loadGame(std::string s, Map *m, PlayerManager *pm, BuildingManager *bm) {

    //std::cout << "loading from string (length " << s.length() << "): " << s << std::endl;

    int i = 0;
    int w = readInt32(s, i); i += 4;
    int h = readInt32(s, i); i += 4;
    delete m;
    m = new Map(w, h);

    for(int x = 0; x < m->getWidth(); x++) {
      for(int y = 0; y < m->getHeight(); y++) {
        m->getTile(x, y)->setOwner(readInt32(s, i)); i += 4;
        readInt32(s, i); i += 4;
        m->getTile(x, y)->setZone(readInt32(s, i)); i += 4;
        readInt32(s, i); i += 4;
        m->getTile(x, y)->setRoad((bool)readInt32(s, i)); i += 4;
      }
    }

  }

};

#endif

