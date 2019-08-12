//
// Created by gabriel on 18/07/19.
//

#include "EQjson.h"
using json = nlohmann::json;

EQjson::~EQjson() {
    clearEq();
}

bool EQjson::parse() {
    bool res = Geojson::parse();
    std::vector<Owner*> ownerList = this->getOwners();
    std::vector<Pts> pointsList = this->getPoints();
    clearEq();
    float max_mag = -1;
    float min_mag = 100;
    time_t max_time = INT_MIN;
    time_t min_time = INT_MAX;

    if(res){
        long int n = ownerList.size();
        for(int ii=0;ii<n;++ii){
            Owner* temp = ownerList.at(ii);
            if(temp->ptsIds.size()>0){
                json* properties = temp->properties;
                auto eq = new EarthQuake();
                eq->owner = temp;
                eq->label = properties->find("title").value().get<std::string>();
                eq->lon = pointsList.at(temp->ptsIds.at(0)-1).at(0);
                eq->lat = pointsList.at(temp->ptsIds.at(0)-1).at(1);
                eq->magnitude   = properties->find("mag").value().get<float>();
                eq->event_time  = properties->find("time").value().get<long int>()/1000;
                eq->depth = pointsList.at(temp->ptsIds.at(0)-1).at(2);
                eq->place = properties->find("place").value().get<std::string>();
                eq->url = properties->find("url").value().get<std::string>();
                eq->selected = false;
                eq->shown = true;
                eq->index = equake.size();
                eq->vindex = equake.size();
                equake.push_back(eq);

                if(eq->magnitude>max_mag){
                    max_mag = eq->magnitude;
                    largest = eq;
                }

                if(eq->magnitude<min_mag){
                    min_mag = eq->magnitude;
                    smallest = eq;
                }

                if(eq->event_time>max_time){
                    max_time = eq->event_time;
                    latest = eq;
                }
                if(eq->event_time<min_time){
                    min_time = eq->event_time;
                    oldest = eq;
                }
            }
        }
        sortedLatest.clear();
        sortedLargest.clear();
        sortedLargest = equake;
        sortedLatest = equake;
        std::sort(sortedLargest.begin(),sortedLargest.end(),EQjson::isLarger);
        std::sort(sortedLatest.begin(),sortedLatest.end(),EQjson::isLatest);
        return true;
    }
    return false;
}

void EQjson::clearEq() {
    for(auto ii=equake.begin();ii!=equake.end();++ii){
        delete((*ii));
    }
    equake.clear();
}

std::vector<EarthQuake*> EQjson::getEarthQuakes() {
    return equake;
}

time_t EQjson::latestTime() {
    return latest->event_time;
}

time_t EQjson::oldestTime() {
    return oldest->event_time;
}

EarthQuake *EQjson::largestEq() {
    return largest;
}

EarthQuake *EQjson::smallestEq() {
    return smallest;
}

EarthQuake *EQjson::latestEq() {
    return latest;
}

void EQjson::unselect() {
    for(auto ii=equake.begin();ii!=equake.end();++ii){
        EarthQuake* eq = (*ii);
        eq->selected = false;
    }
}

long int EQjson::size() {
    return n;
}

EarthQuake *EQjson::at(long int pos) {
    return equake.at(pos);
}

void EQjson::sortMagnitude() {
    std::sort(equake.begin(),equake.end(),EQjson::isLarger);
}

bool EQjson::isLarger(EarthQuake* a,EarthQuake* b) {
    if(a->magnitude > b->magnitude){
        return true;
    }
    return false;
}

void EQjson::sortTime() {
    std::sort(equake.begin(),equake.end(),EQjson::isLatest);
}

bool EQjson::isLatest(EarthQuake *a, EarthQuake *b) {
    if(a->event_time > b->event_time){
        return true;
    }
    return false;
}

std::vector<EarthQuake *> EQjson::getSortedLargest() {
    return this->sortedLargest;
}

std::vector<EarthQuake *> EQjson::getSortedLatest() {
    return this->sortedLatest;
}

void EQjson::filter(Parametres *param) {
    n=0;
    for(auto ii:equake){
        ii->shown = testFilter(ii,param);
        if(ii->shown)
            n++;
    }
}

bool EQjson::testFilter(EarthQuake *eq, Parametres *param) {
    if( time(nullptr)-eq->event_time > param->minDeltaTime*3600){
        return false;
    }
    if(eq->magnitude <param->minMag || eq->magnitude > param->maxMag) {
        return false;
    }
    return true;
}

