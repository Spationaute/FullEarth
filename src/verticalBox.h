//
// Created by gabriel on 2019-08-01.
//

#ifndef FULLEARTH_VERTICALBOX_H
#define FULLEARTH_VERTICALBOX_H

#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

class verticalBox : public Fl_Box {
public:
    verticalBox(int x, int y, int w, int h, const char* label=0L);
    ~verticalBox();

    void draw() override ;
};


#endif //FULLEARTH_VERTICALBOX_H
