//
// Created by gabriel on 2019-08-01.
//

#include "verticalBox.h"

verticalBox::verticalBox(int x, int y, int w, int h, const char* label) :
Fl_Box(x,y,w,h,label)
{
}

void verticalBox::draw() {
    fl_font(FL_TIMES,12);
    fl_color(labelcolor());
    if(this->label()) {
        fl_draw(90,this->label(), x()+w()/2+fl_height()/2, y()+h()/2.0+fl_width(label())/2);
    }
}

verticalBox::~verticalBox() {

}
