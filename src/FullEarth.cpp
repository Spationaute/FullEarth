//
// Created by gabriel on 10/07/19.
//


#include "FullEarth.h"

using json = nlohmann::json;

FullEarth::FullEarth(int x, int y, int w, int h, Parametres* param, const char* label):
Fl_Gl_Window(x,y,w,h,label)
{
    param->showFlags = SHOW_ALL;
    coastBuffer =0;
    faultsBuffer=0;
    riversBuffer=0;
    rot[0]=0;
    rot[1]=0;
    rot[2]=0;
    step = 0;
    zoom = 1;
    this->param = param;


    mode(FL_RGB | FL_DOUBLE  | FL_DEPTH | FL_OPENGL3);


    coast = new Geojson("res/coastline50.geojson");
    rivers= new Geojson("res/ne_50m_rivers_lake_centerlines.geojson");
    faults= new Geojson("res/gem_active_faults.geojson");
    volcano= new Geojson("res/volcano.json");
    eq = new EQjson();

    coast->parse();
    rivers->parse();
    faults->parse();
    volcano->parse();

    Fl::add_timeout(0.02, FullEarth::autoTurn, this);
    Fl::add_timeout(0.025, FullEarth::updateGraphic, this);
    Fl::add_timeout(300, FullEarth::updateData, this);
}

FullEarth::~FullEarth(){
    std::cout << "Cleaning ..."<<std::endl;
    GLsizei n = 1;
    glDeleteBuffers(n,&riversBuffer);
    glDeleteBuffers(n,&coastBuffer);
    glDeleteBuffers(n,&faultsBuffer);

    delete coast;
    delete rivers;
    delete eq;
}

int FullEarth::run(){
    return Fl::run();
}

void FullEarth::draw() {
        if (!valid()) {
            make_current();
            std::cout << "Updating context" << std::endl;
            if (!can_do()) {
                std::cout << "Missing OpenGL suppots. Please Update your driver!" << std::endl;
            }
            glViewport(0, 0, pixel_w(), pixel_h());

            glEnable(GL_TEXTURE_2D);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);

            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

            if(param->showFlags & SHOW_RIVERS) {
                genLinesBuffer(riversBuffer, rivers);
            }

            if(param->showFlags & SHOW_COAST) {
                genLinesBuffer(coastBuffer, coast);
            }

            if(param->showFlags & SHOW_FAULTS) {
                genLinesBuffer(faultsBuffer, faults);
            }
            sengine.initShapes();

            std::cout << "Context Ok!" << std::endl;
            valid(1);
        }
        make_current();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = ((float) pixel_h()) / pixel_w();
        glOrtho(1 / zoom * 1 / ratio * -1.2, 1 / zoom * 1 / ratio * 1.2, 1 / zoom * -1.2, 1 / zoom * 1.2, -3, 5);

        //glRotated(rot[0], 0, 1, 0);
        //glRotated(rot[1], 1, 0, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);

        glViewport(0, 0, pixel_w(), pixel_h());
        glLineWidth(1.0);
        glPushMatrix();
        glLoadIdentity();
        glTranslated(0,0,-4.9);
        glColor3f(0.5,0.2,0.1);
        sengine.texture(1);
        glBegin(GL_POLYGON);
            glTexCoord2f(0,0);
            glVertex3f(-5.34,-3,0);
            glTexCoord2f(1,0);
            glVertex3f(5.34,-3,0);
            glTexCoord2f(1,1);
            glVertex3f(5.34,3,0);
            glTexCoord2f(0,1);
            glVertex3f(-5.34,3,0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D,0x00);
        glPopMatrix();

        glPushMatrix();
        glLoadIdentity();
        gluLookAt(lolaX(rot[0], rot[1]), lolaY(rot[0], rot[1]), lolaZ(rot[0], rot[1]), 0, 0, 0, 0, 0, 1);
        glColor4f(0.1f, 0.1f, .1f, 1.);

        GLUquadricObj *quad = gluNewQuadric();
        gluQuadricTexture(quad,GL_TRUE);
        sengine.texture(0);

        gluQuadricDrawStyle(quad, GLU_FILL);
        gluSphere(quad, 0.996, 100, 100);
        glBindTexture(GL_TEXTURE_2D,0x00);
        glRotated(-90,0,0,1);
        gluQuadricTexture(quad,GL_FALSE);

        if(param->showFlags & SHOW_GRID){
            glColor4f(0.f, 0.f, 0.60f, 0.3f);
            gluQuadricDrawStyle(quad, GLU_LINE);
            gluSphere(quad, 1.02, 36, 18);
        }

        if(param->showFlags & SHOW_VOLCANOES) {
            drawVolcano();
        }

        if(param->showFlags & SHOW_COAST) {
            glColor3f(0.f, 0.5f, 0.f);
            glLineWidth(3.0);
            drawLinesBuffer(coastBuffer, coast);
        }

        if(param->showFlags & SHOW_RIVERS) {
            glColor3f(0.0f, 0.0f, 1.0f);
            glLineWidth(1.0);
            drawLinesBuffer(riversBuffer, rivers);
        }

        if(param->showFlags & SHOW_FAULTS) {
            glColor3f(1.0f, 0.0f, 1.0f);
            glLineWidth(1.0);
            drawLinesBuffer(faultsBuffer, faults);
        }
    drawEq(eq);


    glPopMatrix();
        gluDeleteQuadric(quad);
        glFlush();
}
int FullEarth::handle(int event) {
    static int first = 1;
    if (first && event == FL_SHOW && shown()) {
        first = 0;
        make_current();
        std::cout << "Init glew." << std::endl;
        glewExperimental = GL_TRUE;

        if(glewInit()){
            std::cout << "Error loading GLEW" << std::endl;
        } // defines pters to functions of OpenGL V 1.2 and above
        sengine.setWMandBG(param->getWorldMapFile(),
                           param->getBackgroundFile());
    }
    if(event == FL_UNFOCUS || event == FL_FOCUS ){
        return 1;
    }
    if(event == FL_KEYDOWN){
        switch (Fl::event_key()){
            case 'a':
                this->rot[0] -= 2.0/zoom;
                this->rot[0] = fmod(this->rot[0], 360);
                return 1;
            case 's':
                this->rot[1] -= 2.0/zoom;
                this->rot[1] = fmod(this->rot[1], 360);
                return 1;
            case 'w':
                this->rot[1] += 2.0/zoom;
                this->rot[1] = fmod(this->rot[1], 360);
                return 1;
            case 'd':
                this->rot[0] += 2.0/zoom;
                this->rot[0] = fmod(this->rot[0], 360.0);
                return 1;
            case 'o':
                param->showFlags ^= TURN_DISPLAY;
                return 1;
            case '=':
                zoom += 0.1*zoom>0.1?0.1*zoom:0.1;
                zoom = zoom>64?64:zoom;
                return 1;
            case '-':
                zoom -= 0.1*zoom;
                zoom = zoom<0.5?0.5:zoom;
                return 1;
            case 'i':
                param->showFlags ^= SHOW_RINGS;
                return 1;
            case 'c':
                param->showFlags ^= SHOW_COAST;
                return 1;
            case 'g':
                param->showFlags ^= SHOW_GRID;
                return 1;
            case 'f':
                param->showFlags ^= SHOW_FAULTS;
                return 1;
            case 'v':
                if(!Fl::event_shift()){
                    param->showFlags ^= SHOW_VOLCANOES;
                }else {
                    param->showFlags ^= SHOW_RIVERS;
                }
                return 1;
            case 'n':
                param->showFlags = SHOW_NONE;
                return 1;
            case FL_F+11:
                param->showFlags = param->showFlags+1<0x2F?SHOW_NONE:param->showFlags+1;
                return 1;
            default:
                return 0;
        }
    }
    if(event == FL_MOVE){
        int x, y =0;
        Fl::get_mouse(x, y);
        return 1;
    }
    if(event == FL_PUSH){
        this->take_focus();
        int x, y =0;
        Fl::get_mouse(x, y);
        mouseX = x;
        mouseY = y;
        return 1;
    }
    if(event == FL_MOUSEWHEEL){
        if(Fl::event_dy()<0){
            zoom += 0.05*zoom>0.05?0.05*zoom:0.05;
            zoom = zoom>64?64:zoom;
        }else if(Fl::event_dy()>0){
            zoom -= 0.05*zoom;
            zoom = zoom<0.5?0.5:zoom;
        }
        return 1;
    }
    if(event == FL_DRAG){
        int x, y =0;
        Fl::get_mouse(x, y);
        rot[0] += 1.0/zoom*0.1*(mouseX-x);
        rot[1] -= 1.0/zoom*0.1*(mouseY-y);
        mouseX = x;
        mouseY = y;
        return 1;
    }
    return Fl_Widget::handle(event);
}
void FullEarth::resize(int x, int y, int w, int h) {
    valid(0);
    Fl_Gl_Window::resize(x, y, w, h);
    glViewport(0, 0, pixel_w(), pixel_h());
    redraw();
}

void FullEarth::autoTurn(void *data) {
    FullEarth* fe = (FullEarth*) data;
    fe->turn();
    Fl::repeat_timeout(0.01, FullEarth::autoTurn, (void*)fe);
}

void FullEarth::turn() {
    if(param->showFlags&TURN_DISPLAY){
        rot[0]-=0.05;
    }
}

void FullEarth::drawLinesBuffer(GLuint target, Geojson* source) {
    glBindBuffer(GL_ARRAY_BUFFER,target);
    glEnableVertexAttribArray(0); //?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES,0,source->getLines().size());
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void FullEarth::genLinesBuffer(GLuint &target, Geojson* source) {
    glBindBuffer(GL_ARRAY_BUFFER,target);
    GLsizei n = 1;
    glDeleteBuffers(n, &target);

    Lines lines = source->getLines();
    long int size = lines.size();
    GLfloat faultData[3*size];
    for(int ii=0; ii<size;++ii){
        float lo = lines.at(ii).at(0);
        float la = lines.at(ii).at(1);
        faultData[3*ii] = lolaX(lo,la);
        faultData[3*ii+1] = lolaY(lo,la);
        faultData[3*ii+2] = lolaZ(lo,la);
    }
    glGenBuffers(1,&target);
    glBindBuffer(GL_ARRAY_BUFFER,target);
    glBufferData(GL_ARRAY_BUFFER,sizeof(faultData),faultData,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void FullEarth::drawEq(EQjson *source) {
    std::vector<EarthQuake*> eqList = source->getSortedLatest();
    long int nEq = eqList.size();
    float maxtime = source->latestTime();
    float mintime = source->oldestTime();

    for(int ii=nEq-1;ii>=0;--ii){
        auto temp = eqList.at(ii);
        if(temp->shown) {
            float lon = temp->lon;
            float lat = temp->lat;
            auto mag = temp->magnitude;
            auto qtime = temp->event_time;
            float posx = lolaX(lon, lat);
            float posy = lolaY(lon, lat);
            float posz = lolaZ(lon, lat);
            float timefraqh = (qtime - mintime) / (maxtime - mintime);
            float timefraq = (SHOW_TIMES&(param->showFlags))?timefraqh:1.0;
            int subd = 20;
            float pFact = param->diskSize * ((pow(2.7, mag) + (1 - step % 200 / 100.0)));
            float lFact = param->ringSize * (pow(2.7, mag) * pow(step % 200 / 100.0, 0.5));

            glPushMatrix();
            glLoadIdentity();
            gluLookAt(lolaX(rot[0], rot[1]), lolaY(rot[0], rot[1]), lolaZ(rot[0], rot[1]), 0, 0, 0, 0, 0, 1);
            glRotated(-90, 0, 0, 1);
            glTranslated(posx, posy, posz);
            glRotated(lon, 0, 0, 1);
            glRotated(90 - lat, 0, 1, 0);

            glPushMatrix();
            glTranslated(0, 0, 0.005 * timefraqh);
            glLineWidth(1.0);


            glColor4f(1.f, temp->depth / 300, 0.f, 0.6 * timefraq);
            if (temp->selected) {
                glColor4f(0.f, 0.f, 1.f, 0.6 * timefraqh);
            }
            sengine.drawFCircle(pFact);
            sengine.drawCircle(pFact);

            glColor4f(1.f, temp->depth / 300, 0.f, timefraq);
            if (temp->selected) {
                glColor4f(0.f, 0.f, 1.f, timefraq);
                glDisable(GL_DEPTH_TEST);
            }

            if (param->showFlags & SHOW_RINGS) {
                glLineWidth(3.0);
                float ringAlpha =
                        0.01 + 1.0 - pow(step % 200 / 200.0, 0.5) > 0.1 ? (0.01 + 1.0 - pow(step % 200 / 200.0, 0.5))
                                                                        : 0.1;
                glColor4f(1.f, temp->depth / 70, 0.f, ringAlpha);
                if (temp->selected) {
                    glColor4f(0.f, 0.f, 1.f, ringAlpha);
                    glDisable(GL_DEPTH_TEST);
                }
                sengine.drawCircle(lFact);
            }
            glColor4f(1.f, 1.f, 0.f, 1.f);
            glLineWidth(2.0);
            glBegin(GL_LINES);
            glVertex3d(0, 0, 0);
            glVertex3d(0, 0, 0.01 + 0.05 * timefraqh + 0.0005 * pow(2.70, mag));
            glEnd();


            glPopMatrix();
            glColor4f(1.f, 1.f, 1.f, 1.f);
            gl_font(FL_TIMES_BOLD, 8 + mag);

            if (temp->selected) {
                glColor4f(1.f, 1.f, 0.f, 1.f);
                gl_font(FL_TIMES_BOLD, 24);
                glEnable(GL_DEPTH_TEST);
            }
            char toPrint[12];
            sprintf(toPrint, "%.1f", mag);
            glRasterPos3f(0, 0, 0.05 * timefraqh + 0.0005 * pow(2.70, mag));
            gl_draw(toPrint);
            glPopMatrix();
        }
    }
}

void FullEarth::setSource(std::string source) {
    this->sourceUrl = source;
    Fl::remove_timeout(FullEarth::updateData,this);
    updateData(this);
}

void FullEarth::setStatusBar(Fl_Box *statusBar) {
    this->statusBar = statusBar;
}

void FullEarth::updateStatus(STATUS statVal) {
    switch(statVal){
        case ST_UPDATE:
            statusBar->label("- Updating Data -");
            break;
        case ST_UPDATE_ERROR:
            statusBar->label("- Error: Unable to Update Data --");
            break;
        case ST_READY:
        default:
            statusBar->label("- Ready -");
    }
    statusBar->redraw();
    Fl::flush();
}

void FullEarth::updateData(void* data) {
    auto target = (FullEarth*) data;
    target->updateStatus(ST_UPDATE);
    EQjson* eq = target->getEqJson();
    bool gatres = eq->gatter(target->sourceUrl.c_str());
    bool parres = eq->parse();
    if(! (gatres&&parres)){
        target->updateStatus(ST_UPDATE_ERROR);
    }else {
        target->updateStatus(ST_READY);
    }
    target->parent()->redraw();
    Fl::remove_timeout(FullEarth::updateData,target);
    Fl::add_timeout(300,FullEarth::updateData,target);
}

EQjson* FullEarth::getEqJson() {
    return eq;
}

void FullEarth::setLonLat(float lon, float lat) {
    rot[0] = lon;
    rot[1] = lat;
    param->showFlags &= (0xFF^TURN_DISPLAY) ;
    redraw();
}

void FullEarth::lookAt(EarthQuake* target) {
    setLonLat(target->lon-90,target->lat);
}

void FullEarth::updateGraphic(void *data) {
    FullEarth* fe = (FullEarth*) data;
    fe->redraw();
    fe->nextStep();
    Fl::repeat_timeout(0.025, FullEarth::updateGraphic, (void*)fe);
}

void FullEarth::nextStep() {
    eq->filter(param);
    step +=1;
    step = step>5000?0:step;
}

void FullEarth::drawVolcano() {
    auto points = volcano->getPoints();
    for(auto ii = points.begin();ii!=points.end();++ii ) {
        float lon = ii->at(0);
        float lat = ii->at(1);
        float posx = lolaX(lon,lat);
        float posy = lolaY(lon,lat);
        float posz = lolaZ(lon,lat);

        glPushMatrix();
        glLoadIdentity();
        gluLookAt(lolaX(rot[0], rot[1]), lolaY(rot[0], rot[1]), lolaZ(rot[0], rot[1]), 0, 0, 0, 0, 0, 1);
        glRotated(-90,0,0,1);

        glTranslated(  posx, posy, posz);
        glRotated(lon, 0, 0, 1);
        glRotated(90-lat, 0, 1, 0);

        glRotated(90,0,0,1);
        glLineWidth(3.0);
        glColor4f(.5f,0.25f,0.f,0.5);
        sengine.drawFTriangle(0.0015);

        glColor3f(1.f,1.f,0.f);
        sengine.drawTriangle(0.0015);
        glPopMatrix();
    }
}

void FullEarth::setShowFlag(unsigned short flag) {
    param->showFlags = flag;
}

void FullEarth::setParametres(Parametres* inParam) {
    this->param = inParam;
}

void FullEarth::reloadGraph() {
    sengine.setWMandBG(param->getWorldMapFile(),
                       param->getBackgroundFile());
    valid(0x00);
}


