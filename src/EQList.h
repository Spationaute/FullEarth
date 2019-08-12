//
// Created by gabriel on 21/07/19.
//

#ifndef FULLEARTH_EQLIST_H
#define FULLEARTH_EQLIST_H

#include <FL/Fl_Widget.H>
#include <FL/Fl_Help_Dialog.H>
#include <FL/fl_draw.H>
#include <time.h>
#include <boost/range/adaptor/filtered.hpp>
#include "EQjson.h"
#include "Parametres.h"

class EQList : public Fl_Widget {
public:
    EQList(int x,int y, int w, int h, Parametres* param, const char* label = 0L);
    ~EQList();

    EarthQuake* getLastClicked();
    void draw() override;
    void* setEq(EQjson* toSet);
    int handle(int event) override ;
    static void checkChange(void*data);
    Parametres* param;

private:
    EQjson* eq;
    EarthQuake* lastClicked;

};


#endif //FULLEARTH_EQLIST_H
