//
// Created by gabriel on 2019-07-30.
//

#ifndef FULLEARTH_PARAMETRES_H
#define FULLEARTH_PARAMETRES_H

#include <vector>
#include <ctime>
#define SHOW_NONE   0x00
#define SHOW_COAST  0x01
#define SHOW_RIVERS 0x02
#define SHOW_FAULTS 0x04
#define SHOW_VOLCANOES  0x08
#define SHOW_GRID 0x10
#define SHOW_RINGS 0x20
#define SHOW_TIMES 0x40
#define TURN_DISPLAY 0x80
#define SHOW_ALL  0xFF
struct lineSettings{
    unsigned int width;
    float r,g,b,a;
    bool visible;
};

class Parametres {
public:
    Parametres();

    bool load(const char* dest);
    bool save(const char* target);

    char* getWorldMapFile();
    char* getBackgroundFile();
    void setWorldMapFile(const char* inString);
    void setBackgroundFile(const char* inString);

    unsigned short getShowFlags();
    bool changed();
    void setChanged();
    ~Parametres();

    unsigned int showFlags;
    time_t minDeltaTime;
    float minMag;
    float maxMag;
    float minDepth;
    float maxDepth;
    float ringSize;
    float diskSize;

private:
    bool isChanged;
    char* worldMapFile;
    char* backgroundFile;
    std::vector<lineSettings> lineStyle;

};


#endif //FULLEARTH_PARAMETRES_H
