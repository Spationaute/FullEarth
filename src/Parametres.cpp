//
// Created by gabriel on 2019-07-30.
//

#include <cstdio>
#include <cstring>
#include <iostream>
#include "Parametres.h"

Parametres::Parametres() {
    worldMapFile = new char[255];
    memset(worldMapFile,0x00,255);
    backgroundFile = new char[255];
    memset(backgroundFile,0x00,255);
    showFlags = SHOW_ALL;
    minDeltaTime = 0;
    minMag = 0;
    maxMag = 10;
    minDepth = 0;
    maxDepth = 1e6;
    ringSize = 0.0002;
    diskSize = 0.00005;
}

Parametres::~Parametres() {

}

char *Parametres::getWorldMapFile() {
    printf("%s\n",worldMapFile);
    return worldMapFile;
}

char *Parametres::getBackgroundFile() {
    return backgroundFile;
}

unsigned short Parametres::getShowFlags() {
    return showFlags;
}

bool Parametres::load(const char *dest) {
    printf("Loading Option File\n");
    FILE* file = fopen(dest,"r");
    unsigned int inFlags;
    char inString[255];
    fscanf(file,"%lx",&minDeltaTime);
    fscanf(file,"%xh",&showFlags);
    memset(inString, 0x00, 255);
    int ncpy = fscanf(file, "%s\n", inString);
    if(ncpy>0) {
        memcpy(worldMapFile, inString, sizeof(inString));
    }
    memset(inString,0x00,255);
    ncpy = fscanf(file, "%s\n", inString);
    if(ncpy>0) {
        memcpy(backgroundFile, inString, sizeof(inString));
    }
    printf("\tDelta T: %i\n",minDeltaTime);
    printf("\tFlag: %X\n",showFlags);
    printf("\tWorld Map File: %s  \n", worldMapFile);
    printf("\tBackground File: %s \n", backgroundFile);

    return false;
}

bool Parametres::save(const char *target) {
    return false;
}

bool Parametres::changed() {
    bool t = isChanged;
    isChanged= false;
    return t;
}

void Parametres::setChanged() {
    isChanged=true;
}

void Parametres::setWorldMapFile(const char* inString) {
    std::cout << "World Map: " << inString <<std::endl;
    memset(worldMapFile,0x00,255);
    snprintf(worldMapFile,255,"%s",inString);
}

void Parametres::setBackgroundFile(const char* inString) {
    std::cout << "Background Map: " << inString <<std::endl;
    memset(backgroundFile,0x00,255);
    snprintf(backgroundFile,255,"%s",inString);
}