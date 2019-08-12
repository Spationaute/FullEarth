//
// Created by gabriel on 2019-07-25.
//

#include "ShapeEngine.h"

ShapeEngine::ShapeEngine() {
    subd = 50;
    fl_register_images();
    wasInit=false;
}

ShapeEngine::~ShapeEngine() {

}

void ShapeEngine::drawCircle(float radius) {
    glPushMatrix();
    glScalef(radius,radius,radius);
    Shape* circle = pointerList.at(0);
    glBindBuffer(GL_ARRAY_BUFFER,circle->pointer);
    glEnableVertexAttribArray(0); //?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES,0, circle->size);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glPopMatrix();
}

void ShapeEngine::drawFCircle(float radius) {
    glPushMatrix();
    glScalef(radius,radius,radius);
    Shape* circle = pointerList.at(0);
    glBindBuffer(GL_ARRAY_BUFFER,circle->pointer);
    glEnableVertexAttribArray(0); //?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_POLYGON,0, circle->size);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glPopMatrix();
}

void ShapeEngine::initShapes() {
    if(!wasInit) {
        GLfloat circleShape[6 * subd];
        std::cout << "Setting up Shapes" << std::endl;
        for (int jj = 0; jj < subd; ++jj) {
            circleShape[6 * jj] = cos(jj * 2 * M_PI / subd);
            circleShape[6 * jj + 1] = sin(jj * 2 * M_PI / subd);
            circleShape[6 * jj + 2] = 0;
            circleShape[6 * jj + 3] = cos((jj + 1) * 2 * M_PI / subd);
            circleShape[6 * jj + 4] = sin((jj + 1) * 2 * M_PI / subd);
            circleShape[6 * jj + 5] = 0;
        }

        GLuint shapeNumber = 0;
        glGenBuffers(1, &shapeNumber);
        glBindBuffer(GL_ARRAY_BUFFER, shapeNumber);
        glBufferData(GL_ARRAY_BUFFER, sizeof(circleShape), circleShape, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0x00);
        auto temp = new Shape;
        temp->pointer = shapeNumber;
        temp->size = (sizeof(circleShape) / sizeof(float)) / 3;
        pointerList.push_back(temp);

        GLfloat triangleLine[] = {-1, 0, 0,
                                  1, 0, 0,
                                  1, 0, 0,
                                  0, 1, 0,
                                  0, 1, 0,
                                  -1, 0, 0};
        glGenBuffers(1, &shapeNumber);
        glBindBuffer(GL_ARRAY_BUFFER, shapeNumber);
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangleLine), triangleLine, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0x00);

        temp = new Shape;
        temp->pointer = shapeNumber;
        temp->size = (sizeof(triangleLine) / sizeof(float)) / 3;
        pointerList.push_back(temp);

        GLfloat square[] = {-1, -1, 0,
                            1, -1, 0,
                            1, 1, 0,
                            -1, 1, 0};
        shapeNumber = 0;
        glGenBuffers(1, &shapeNumber);
        glBindBuffer(GL_ARRAY_BUFFER, shapeNumber);
        glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0x00);

        temp = new Shape;
        temp->pointer = shapeNumber;
        temp->size = (sizeof(square) / sizeof(float)) / 3;
        pointerList.push_back(temp);
        wasInit=true;
    }
}

void ShapeEngine::drawTriangle(float radius) {
    glPushMatrix();
    glScalef(radius,radius,radius);
    Shape* triangle = pointerList.at(1);
    glBindBuffer(GL_ARRAY_BUFFER,triangle->pointer);
    glEnableVertexAttribArray(0); //?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES,0, triangle->size);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glPopMatrix();
}


void ShapeEngine::drawFTriangle(float radius) {
    glPushMatrix();
    glScalef(radius,radius,radius);
    Shape* triangle = pointerList.at(1);
    glBindBuffer(GL_ARRAY_BUFFER,triangle->pointer);
    glEnableVertexAttribArray(0); //?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_POLYGON,0, triangle->size);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glPopMatrix();
}

void ShapeEngine::texture(unsigned int index) {
    GLuint target = textures.at(index);
    glBindTexture(GL_TEXTURE_2D,target);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ShapeEngine::drawFSquare(float scale) {
    glPushMatrix();
    glScalef(scale,scale,scale);
    Shape* circle = pointerList.at(2);
    glBindBuffer(GL_ARRAY_BUFFER,circle->pointer);
    glEnableVertexAttribArray(0); //?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_POLYGON,0, circle->size);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glPopMatrix();
}

void ShapeEngine::LoadTexture(int target, const char *file) {
    std::cout << "Loading Texture: "<<file<<std::endl;
    auto skyTexture = Fl_Shared_Image::get(file);
    if (!skyTexture) {
        std::cout << "Texture not found" << std::endl;
        exit(1);
    }
    GLuint textP=0;
    if(target<textures.size()) {
        GLuint textP = textures.at(target-1);
        glDeleteTextures(1, &textP);
    }
    glGenTextures(1, &textP);
    glBindTexture(GL_TEXTURE_2D, textP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, skyTexture->w(), skyTexture->h(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 skyTexture->data()[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0x00);
    if(target<textures.size()) {
        textures.at(target-1)= textP;
    }else{
        textures.push_back(textP);
    }
    skyTexture->release();
}

void ShapeEngine::setWMandBG(char *wmFile, char *bgFile) {
    LoadTexture(1,wmFile);
    LoadTexture(2,bgFile);
}

void ShapeEngine::desinit() {
    wasInit = false;
}


