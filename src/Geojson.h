//
// Created by gabriel on 14/07/19.
//

#ifndef FULLEARTH_GEOJSON_H
#define FULLEARTH_GEOJSON_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
//#ifdef __MINGW32__
//#include "json_mingw.hpp"
//#else
#include "json.hpp"
//#endif
#include <curl/curl.h>



typedef std::vector<float> Pts;
typedef std::vector<Pts>  Poly;
typedef std::vector<Pts>  Lines;

struct Owner{
    nlohmann::json* properties;
    std::vector<unsigned int> ptsIds;
    std::vector<unsigned int> lineIds;
    std::vector<unsigned int> polyIds;
};

class Geojson {
public:
    Geojson();
    Geojson(const char* fileTarget);
    ~Geojson();

    bool readFile(const char* fileTarget);
    bool gatter(const char* url);
    bool parse();
    void cleanData();

    void setBuffer(std::string toSet);
    void appendBuffer(std::string toAdd);
    static size_t getData(void* buffer, size_t size,size_t nmemb,void* user);
    Poly getPolygons();
    Lines getLines();
    std::vector<Pts> getPoints();
    std::vector<Owner*> getOwners();

private:
    const char* fileTarget;
    std::string buffer;
    nlohmann::json parser;
    Poly polygons;
    Lines lineList;
    std::vector<Pts> pointList;
    std::vector<Owner*> ownerList;
};


#endif //FULLEARTH_GEOJSON_H
