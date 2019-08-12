//
// Created by gabriel on 18/07/19.
//

#ifndef FULLEARTH_EQJSON_H
#define FULLEARTH_EQJSON_H

#include "Geojson.h"
#include "time.h"
#include "Parametres.h"

struct EarthQuake{
    unsigned int index;
    unsigned int vindex;
    std::string label;
    std::string place;
    std::string url;
    time_t event_time;
    float lon; float lat;
    float magnitude;
    float depth;
    bool selected;
    Owner* owner;
    bool shown;
};
class EQjson : public Geojson{
public:
    EQjson():Geojson(){};
    ~EQjson();

    time_t latestTime();
    time_t oldestTime();
    EarthQuake* at(long int pos);
    EarthQuake* latestEq();
    EarthQuake* largestEq();
    EarthQuake* smallestEq();

    void sortMagnitude();
    void filter(Parametres* param);
    static bool isLarger(EarthQuake* a,EarthQuake* b);
    void sortTime();
    static bool isLatest(EarthQuake* a,EarthQuake* b);

    void unselect();
    long int size();

    bool parse();
    std::vector<EarthQuake*> getEarthQuakes();
    std::vector<EarthQuake*> getSortedLargest();
    std::vector<EarthQuake*> getSortedLatest();
    void clearEq();

    static bool testFilter(EarthQuake* eq, Parametres* param);
private:
    EarthQuake* largest;
    EarthQuake* smallest;

    EarthQuake* latest;
    EarthQuake* oldest;

    std::vector<EarthQuake*> equake;
    std::vector<EarthQuake*> sortedLargest;
    std::vector<EarthQuake*> sortedLatest;
    size_t n;

};


#endif //FULLEARTH_EQJSON_H
