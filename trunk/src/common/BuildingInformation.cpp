#include "BuildingInformation.hpp"

#include <iostream>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "SimultyException.hpp"
#include "Building.hpp"


BuildingInformation::BuildingInformation() {

}


int BuildingInformation::getId() {
    return id;
}

unsigned char BuildingInformation::getType() {
    return type;
}

unsigned char BuildingInformation::getWidth() {
    return sizeWidth;
}
unsigned char BuildingInformation::getHeight() {
    return sizeHeight;
}

int BuildingInformation::getBuyCost() {
    return costBuy;
}
int BuildingInformation::getDemolishCost() {
    return costDemolish;
}
int BuildingInformation::getUpkeepCost() {
    return costUpkeep;
}

unsigned char BuildingInformation::typeStringToTypeNumber(std::string type) {
    
    if(type == "powerplant") {
        return Building::TYPE_POWERPLANT;
    } else if(type == "crime") {
        return Building::TYPE_POLICE;
    } else if(type == "fire") {
        return Building::TYPE_FIRE;
    } else if(type == "health") {
        return Building::TYPE_HOSPITAL;
    } else {
        throw SIMULTYEXCEPTION("The given type string is not valid");
    }
    
}

BuildingInformation *BuildingInformation::load(std::string file) {

    LIBXML_TEST_VERSION

    xmlDoc *document = xmlReadFile(file.c_str(), NULL, 0);
    if (document == NULL) {
        std::cerr << "error: could not parse file " << file;
        return NULL;
    }

    BuildingInformation *bi = NULL;

    xmlNode *rootNode = xmlDocGetRootElement(document);
    if(xmlStrcmp(rootNode->name, (const xmlChar *)"building") == 0) {
        bi = new BuildingInformation();
        xmlChar *xtype = xmlGetProp(rootNode, (const xmlChar *)"type");
        xmlChar *xid   = xmlGetProp(rootNode, (const xmlChar *)"id");
        int id = atoi((const char *)xid);
        bi->type = typeStringToTypeNumber(std::string((const char *)xtype));
        bi->id   = id;
        std::cout << "Id: " << id << ", type: " << xtype << ", " << (int)bi->type << std::endl;
        for (xmlNode *node = rootNode->children; node; node = node->next) {
            if (node->type == XML_ELEMENT_NODE) {
                 if(xmlStrcmp(node->name, (const xmlChar *)"name") == 0) {
                    xmlChar *content = xmlNodeListGetString(document, node->xmlChildrenNode, 1);
                    std::cout << "Name: " << content << std::endl;
                    bi->name = std::string((const char *)content);
                    xmlFree(content);
                 } else if(xmlStrcmp(node->name, (const xmlChar *)"description") == 0) {
                    xmlChar *content = xmlNodeListGetString(document, node->xmlChildrenNode, 0);
                    //std::cout << "Description: " << content << std::endl;
                    bi->description = std::string((const char *)content);
                    xmlFree(content);
                 } else if(xmlStrcmp(node->name, (const xmlChar *)"cost") == 0) {
                    xmlChar *xbuy = xmlGetProp(node, (const xmlChar *)"buy");
                    xmlChar *xdem = xmlGetProp(node, (const xmlChar *)"demolish");
                    xmlChar *xupk = xmlGetProp(node, (const xmlChar *)"upkeep");
                    //std::cout << "Cost - Buy: " << xbuy << ", Demolish: " << xdem << ", Upkeep: " << xupk << std::endl;
                    bi->costBuy      = atoi((const char *)xbuy);
                    bi->costDemolish = atoi((const char *)xdem);
                    bi->costUpkeep   = atoi((const char *)xupk);
                    xmlFree(xbuy);
                    xmlFree(xdem);
                    xmlFree(xupk);
                 } else if(xmlStrcmp(node->name, (const xmlChar *)"size") == 0) {
                    xmlChar *xwid = xmlGetProp(node, (const xmlChar *)"width");
                    xmlChar *xhei = xmlGetProp(node, (const xmlChar *)"height");
                    //std::cout << "Size: " << xwid << "x" << xhei << std::endl;
                    bi->sizeWidth  = atoi((const char *)xwid);
                    bi->sizeHeight = atoi((const char *)xhei);
                    xmlFree(xwid);
                    xmlFree(xhei);
                 } else if(xmlStrcmp(node->name, (const xmlChar *)"graphic") == 0) {
                    xmlChar *xloc = xmlGetProp(node, (const xmlChar *)"location");
                    //std::cout << "Graphic: " << xloc << std::endl;
                    bi->graphicLocation = std::string((const char *)xloc);
                    xmlFree(xloc);
                 }
            }
        }
        
        xmlFree(xtype);
        xmlFree(xid);
    } 
    xmlFreeDoc(document);
    xmlCleanupParser();
    return bi;
}

