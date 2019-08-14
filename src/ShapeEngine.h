//
// Created by gabriel on 2019-07-25.
//

#ifndef FULLEARTH_SHAPEENGINE_H
#define FULLEARTH_SHAPEENGINE_H


#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GL/glu.h>
#include <vector>
#include <FL/Fl_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <math.h>
#include <iostream>


struct Shape{
    GLuint pointer;
    size_t size;
};

class ShapeEngine {
public:
    ShapeEngine();
    ~ShapeEngine();

    void initShapes();
    void LoadTexture(int target, const char* file);
    void drawCircle(float radius);
    void drawTriangle(float radius);
    void drawFCircle(float radius);
    void drawFSquare(float scale);
    void drawFTriangle(float radius);

    void texture(unsigned int index);
    void setWMandBG(char* wmFile, char* bgFile);

    void desinit();
private:
    int subd;
    bool wasInit;
    std::vector<Shape*> pointerList;
    std::vector<GLuint> textures;
    char* wmFile;
    char* bgFile;
};


#endif //FULLEARTH_SHAPEENGINE_H
