//
// Created by gabriel on 21/07/19.
//

#include <FL/Fl.H>
#include "EQList.h"

EQList::EQList(int x, int y, int w, int h, Parametres* param, const char *label):
    Fl_Widget(x,y,w,h,label)
{
    this->param = param;
    Fl::add_timeout(0.1,EQList::checkChange,(void*)this);
}

void *EQList::setEq(EQjson *toSet) {
    this->eq = toSet;
}

void EQList::draw() {
    int px = this->x();
    int py = this->y();
    int pw = this->w();

    long int n =0;
    fl_font(FL_HELVETICA,12);
    int delta = 2*(fl_height()+fl_descent());
    fl_color(FL_BLACK);
    int ph = (eq->size()+1>1500?1500:eq->size()+1)*delta;
    fl_rectf(0,0,w(),h());
    this->h(ph);
    for(auto ii : eq->getEarthQuakes() ){
        if(ii->shown && n<1500) {
            ii->vindex=n;
            fl_color(n % 2 == 0 ? FL_BLACK : FL_DARK3);
            if (ii->selected) {
                fl_color(FL_DARK_BLUE);
            }
            fl_rectf(px, py + n * delta, pw, delta);
            fl_color(FL_WHITE);
            fl_line(px, py + n * delta, px + pw, py + n * delta);
            fl_draw(ii->label.c_str(), px + 10, py + n * delta + delta - (delta / 2 + fl_descent()));
            fl_color(FL_DARK1);
            char timeToPrint[255];
            memset(timeToPrint, 0x00, sizeof(timeToPrint));
            tm *event_time = gmtime(&(ii->event_time));
            snprintf(timeToPrint, 254 * sizeof(char), "(#%i)- %.2f km - %02i:%02i:%02i GMT - %02i/%02i/%i",
                     n + 1,
                     ii->depth,
                     event_time->tm_hour,
                     event_time->tm_min,
                     event_time->tm_sec,
                     event_time->tm_mday,
                     event_time->tm_mon,
                     event_time->tm_year + 1900);
            fl_draw(timeToPrint, 10 + px, py + n * delta + delta - fl_descent());
            n++;
        }
    }

    fl_color(FL_WHITE);
    fl_line(px+pw-1,py,px+pw-1,py+ph);
}

EQList::~EQList() {

}

int EQList::handle(int event) {
    if(event == FL_UNFOCUS || event == FL_FOCUS ){
        return 1;
    }
    if(event == FL_PUSH){
        this->take_focus();
        int x, y =0;
        x=Fl::event_x();
        y=Fl::event_y();
        x -= this->x();
        y -= this->y() - this->parent()->y();
        int n = 0;
        for(auto ii : eq->getEarthQuakes() ){
            n += ii->shown;
        }
        fl_font(FL_HELVETICA,12);
        int delta = 2*(fl_height()+fl_descent());
        int ph = (n+1)*delta;
        int sel = (y+10)/delta;
        if(y<ph && sel>0 && sel<n+1){

            for(auto ii : eq->getEarthQuakes()) {
                if(ii->vindex==sel-1 && ii->shown) {
                    if(Fl::event_button() == FL_LEFT_MOUSE) {
                        if(!Fl::event_shift()){
                            eq->unselect();
                        }
                        ii->selected ^= true;
                        lastClicked = ii;
                        std::cout << x << ":" << y << " -> Select :" << sel << " " << ii->vindex << std::endl;
                    }
                    if(Fl::event_button()==FL_RIGHT_MOUSE){
                        auto browser = new Fl_Help_Dialog();
                        browser->load(ii->url.c_str());
                        lastClicked = ii;
                    }
                }
            }
            redraw();
            if(callback()!=nullptr){
                do_callback();
            }
            return 1;
        }
        return 0;
    }
    return 0;
}

EarthQuake *EQList::getLastClicked() {
    return lastClicked;
}

void EQList::checkChange(void *data) {
    auto eql = (EQList*) data;
    if(eql->param->changed()){
        eql->redraw();
    }
    Fl::repeat_timeout(0.1,EQList::checkChange,data);
}

