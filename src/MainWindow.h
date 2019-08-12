//
// Created by isatis on 16/07/19.
//

#ifndef FULLEARTH_MAINWINDOW_H
#define FULLEARTH_MAINWINDOW_H
#define GL_GLEXT_PROTOTYPES
#if defined(_WIN64)
#include <winsock2.h>
#include <windows.h>
#endif
#include <FL/Fl_Gl_Window.H>
#include <GL/glew.h>
#include <GL/glu.h>

#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include "FullEarth.h"
#include "EQList.h"
#include "OptionsWindow.h"
#include "Parametres.h"

#define ONEHOUR_RES "https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/1.0_hour.geojson"
#define DAY_RES "https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/1.0_day.geojson"
#define MONTH_RES "https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/2.5_month.geojson"
#define EUR_500 "https://www.seismicportal.eu/fdsnws/event/1/query?orderby=time&limit=500&format=json&minmag=1"
class MainWindow : public Fl_Window {
public:

    MainWindow(int x, int y, int w, int h);
    ~MainWindow();

    void resize(int x, int y, int w, int h) override;
    static void button_clicked(Fl_Widget* parent, void* data);
    static void set_source(Fl_Widget* parent, void* data);

    FullEarth* fullEarth;
    EQList* eqList;
    Fl_Scroll* list_scroll;
    Fl_Button* list_button;

    struct MenuOption{
        std::string source;
        FullEarth* target;
    };
    OptionsWindow* optionsWindow;

private:
    Parametres* param;
    Fl_Choice* source_choice;
    Fl_Box* status_bar;
    Fl_Group* top_controls;
    Fl_Group* bottom_label;
};


#endif //FULLEARTH_MAINWINDOW_H
