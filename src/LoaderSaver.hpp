
#ifndef _LOADERSAVER_HPP_
#define _LOADERSAVER_HPP_

#include <string>

#include "PlayerManager.hpp"


// PROOF OF CONCEPT SOLUTION, NEEDS TO BE FIXED
// PERHAPS A SERIALIZE / DESERIALIZE INTERFACE THAT CLASSES 
// CAN EXTEND

class LoaderSaver {

  public:

  static char readChar(std::string from, unsigned int &pos) {
    return (char)from.at(pos++);
  }

  static std::string writeChar(std::string to, char c) {
    std::string toNew = to;
    toNew.push_back(c);
    return toNew;
  }

  static bool readBool(std::string from, unsigned int &pos) {
    return (bool)readChar(from, pos);
  }
  
  static std::string writeBool(std::string to, bool b) {
    return writeChar(to, (char)b);
  }

  static NLINT16 readInt16(std::string from, unsigned int &pos) {
    unsigned char b[2]; for(int i = 0; i < 2; i++)b[i] = from.at(pos + i);
    pos += 2;
    return ntohs( *((NLINT16 *)b) );
  }

  static std::string writeInt16(std::string to, NLINT16 i) {
    
    std::string toNew(to);
    unsigned char b[2]; *((NLINT32 *)b) = htons(i);
    for(int i = 0; i < 2; i++)toNew.push_back((char)b[i]); 
    return toNew;
  }

  static NLINT32 readInt32(std::string from, unsigned int &pos) {
      unsigned char b[4]; for(int i = 0; i < 4; i++)b[i] = from.at(pos + i);
      pos += 4;
      return ntohl( *((NLINT32 *)b) );
  }

  static std::string writeInt32(std::string to, NLINT32 i) {
    
    std::string toNew(to);
    unsigned char b[4]; *((NLINT32 *)b) = htonl(i);
    for(int i = 0; i < 4; i++)toNew.push_back((char)b[i]); 
    return toNew;
  }


  static std::string readString(std::string from, unsigned int pos) {

    std::string s; unsigned int i;
    for(i = 0; i < from.length() - pos && from[pos + i] != '\0'; i++) {
      s.push_back(from[pos + i]);
    }
    pos += i;
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

    s = writeInt16(s, m->getWidth());
    s = writeInt16(s, m->getHeight());
    for(unsigned int x = 0; x < m->getWidth(); x++) {
      for(unsigned int y = 0; y < m->getHeight(); y++) {
        s = writeChar(s, m->getTile(x, y)->getOwner());
        s = writeChar(s, m->getTile(x, y)->getTerrain());
        s = writeChar(s, m->getTile(x, y)->getZone());
        s = writeChar(s, m->getTile(x, y)->getHouse());
        s = writeBool(s, m->getTile(x, y)->isRoad());
      }
    }

    return s;
  }


  static void loadGame(std::string s, Map *m, PlayerManager *pm, BuildingManager *bm) {
  
    //std::cout << "loading from string (length " << s.length() << "): " << s << std::endl;
    
    unsigned int i = 0; 
    unsigned int w = readInt16(s, i);
    unsigned int h = readInt16(s, i);
    delete m;
    m = new Map(w, h);

    for(unsigned int x = 0; x < m->getWidth(); x++) {
      for(unsigned int y = 0; y < m->getHeight(); y++) {
        m->getTile(x, y)->setOwner(readChar(s, i));
        readChar(s, i);
        m->getTile(x, y)->setZone(readChar(s, i));
        readChar(s, i);
        m->getTile(x, y)->setRoad(readBool(s, i));
      }
    }

  }

};

#endif

