//
// Created by gabriel on 10/07/19.
//

#ifndef FULLEARTH_FULLEARTH_H
#define FULLEARTH_FULLEARTH_H

#include <fstream>

#define GL_GLEXT_PROTOTYPES
#ifdef __linux
#include <X11/Xlib.h>
#endif

#include <GL/glew.h>
#include <GL/glu.h>
#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Gl_Window.H>
#include <cmath>
#include <vector>
#include "Geojson.h"
#include "EQjson.h"
#include "MathToolkit.h"
#include "time.h"
#include "ShapeEngine.h"
#include "Parametres.h"



enum STATUS{
    ST_READY,
    ST_UPDATE,
    ST_UPDATE_ERROR
};

class FullEarth: public  Fl_Gl_Window{
public:
    FullEarth(int x, int y, int w, int h,Parametres* param, const char* label);
    ~FullEarth();

    int run();
    int handle(int event) override;

    void turn();
    void draw() override;
    void resize(int x,int y,int w, int h) override;
    EQjson* getEqJson();

    void reloadGraph();
    void setSource(std::string source);
    void setStatusBar(Fl_Box* statusBar);
    void setLonLat(float lon, float lat);
    void setParametres(Parametres* inParam);
    void lookAt(EarthQuake* target);
    void setShowFlag(unsigned short flags);

    void nextStep();
    static void autoTurn(void* data);

    static void updateData(void* data);
    static void updateGraphic(void* data);
    void updateStatus(STATUS statVal);

private:
    int mouseX, mouseY;
    float rot[3];
    float zoom;
    unsigned int step;
    std::string sourceUrl;
    Fl_Box* statusBar;
    GLuint coastBuffer;
    GLuint riversBuffer;
    GLuint faultsBuffer;

    Geojson* coast;
    Geojson* rivers;
    Geojson* faults;
    Geojson* volcano;
    EQjson* eq;
    ShapeEngine sengine;
    void genLinesBuffer(GLuint &target, Geojson* source);
    void drawLinesBuffer(GLuint target, Geojson* source);
    void drawVolcano();

    void drawEq(EQjson* source);
    Parametres* param;
};


#endif //FULLEARTH_FULLEARTH_H
