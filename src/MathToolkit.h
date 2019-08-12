//
// Created by gabriel on 15/07/19.
//

#ifndef FULLEARTH_MATHTOOLKIT_H
#define FULLEARTH_MATHTOOLKIT_H
#include <cmath>


static inline float toRad(float x) {
    return x/180.0*M_PI;
}

static inline float lolaX(float lo, float la){
    return cos(toRad(la))*cos(toRad(lo));
}

static inline float lolaY(float lo, float la) {
    return cos(toRad(la))*sin(toRad(lo));
}

static inline float lolaZ(float lo, float la) {
    return sin(toRad(la));
}

#endif //FULLEARTH_MATHTOOLKIT_H
