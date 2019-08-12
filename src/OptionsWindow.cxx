
#include <iostream>
#include "OptionsWindow.h"
#include "verticalBox.h"
#include "MainWindow.h"

OptionsWindow::OptionsWindow(Parametres* param) : Fl_Window(0,0,300,500){
    this->param=param;
    auto settingsTab = new Fl_Tabs(0,0,w(),h());
    filterPanel();
    displayPanel();
    settingsPanel();
    this->color(FL_GRAY0);
    this->resizable(settingsTab);
    this->size_range(300,500);
    this->set_non_modal();

}

OptionsWindow::~OptionsWindow() {

}

void OptionsWindow::cb_setFile(Fl_Widget *p, void *d) {
    auto fin = (Fl_File_Input*) d;
    Fl_Native_File_Chooser fc;
    fc.title("Choose an Image");
    fc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    fc.filter("Image File\t*.{png,jpg}\nPNG Image \t*.png\nJPG Image\t*.jpg");
    fc.preset_file(fin->value());
    fc.directory("./");
    switch (fc.show()){
        case -1:
            break;
        case 1:
            break;
        default:
            fin->value(fc.filename());
            break;
    }
}

void OptionsWindow::settingsPanel() {
    auto gridLayout = new Fl_Group(10,32,280,475,"Settings");
    gridLayout->color(FL_DARK3);
    auto pack = new Fl_Pack(10,32,280,475);
    pack->begin();
    {
        auto b = new Fl_Box(0, 0, 150, 32, " -- World Map --");
        b->labelcolor(FL_LIGHT1);
    }
    displayFileInput(wmFilePath,param->getWorldMapFile());
    {
        auto b = new Fl_Box(0, 0, 150, 32, " -- Background Image --");
        b->labelcolor(FL_LIGHT1);
    }
    displayFileInput(bgFilePath,param->getBackgroundFile());
    {
        auto b = new Fl_Box(0, 0, 150, 32, " -- Source List --");
        b->labelcolor(FL_LIGHT1);
    }
    displayFileInput(sourceFilePath,"./res/sources.json");
    {
        auto b = new Fl_Button(0,0,150,32,"Apply");
        b->callback(OptionsWindow::cb_acceptImage,this);
    }
    pack->end();
    gridLayout->resizable(pack);
    gridLayout->end();
}

void OptionsWindow::displayFileInput(Fl_File_Input* &to_dysp, const char* baseUrl) {
    auto p = new Fl_Pack(0, 0, w() - 20, 32);
    p->type(FL_HORIZONTAL);
    p->begin();
    to_dysp = new Fl_File_Input(0, 0, 200, 32);
    to_dysp->value(baseUrl);
    auto b = new Fl_Button(0,0, 80, 32,"Set");
    b->callback(OptionsWindow::cb_setFile,(void*)to_dysp);
    p->end();
    p->resizable(p);
}

void OptionsWindow::displayPanel() {
    auto tabs = new Fl_Group(10,32,280,475,"Display");
    tabs->color(FL_DARK3);
    auto pack = new Fl_Pack(10,42,280,475);
    pack->begin();
    {
        auto l = new Fl_Box(0,0,280,26,"Ring Size");
        l->labelcolor(FL_LIGHT1);
    }
    ringSize = new Fl_Value_Slider(0,0,280,32);
    ringSize->type(FL_HORIZONTAL);
    ringSize->minimum(0.1);
    ringSize->value(param->ringSize*1e4);
    ringSize->maximum(10);
    ringSize->callback(OptionsWindow::cb_sliderChanged);
    {
        auto l = new Fl_Box(0,0,280,26,"Disk Size");
        l->labelcolor(FL_LIGHT1);
    }
    diskSize = new Fl_Value_Slider(0,0,280,32);
    diskSize->type(FL_HORIZONTAL);
    diskSize->minimum(0.1);
    diskSize->value(param->diskSize*1e5);
    diskSize->maximum(10);
    diskSize->callback(OptionsWindow::cb_sliderChanged);

    {
        auto l = new Fl_Box(0,0,280,26,"------");
        l->labelcolor(FL_LIGHT1);
    }
    displayShowButton("Coast",SHOW_COAST);
    displayShowButton("Rivers",SHOW_RIVERS);
    displayShowButton("Volcanoes",SHOW_VOLCANOES);
    displayShowButton("Faults",SHOW_FAULTS);
    displayShowButton("Rings",SHOW_RINGS);
    displayShowButton("Grid",SHOW_GRID);
    {
        auto b =new Fl_Box(0,0,280,32,"------");
        b->labelcolor(FL_LIGHT1);
    }
    displayShowButton("Rotate",TURN_DISPLAY);
    displayShowButton("Dimmed With Time",SHOW_TIMES);

    pack->end();
    tabs->end();
}

void OptionsWindow::filterPanel() {
    auto tabs = new Fl_Group(10,32,280,475,"Filters");
    tabs->color(FL_DARK3);
    auto pack = new Fl_Pack(10,56,280,400);
    pack->type(FL_VERTICAL);
    pack->begin();
        auto slidersPack = new Fl_Pack(10,31,280,400);
        slidersPack->type(FL_HORIZONTAL);
        slidersPack->begin();
            {
                auto l = new verticalBox(0, 0, 24, 300, "Oldest (Hour)");
                l->labelcolor(FL_LIGHT1);
            }
            maxTime = new Fl_Value_Slider(10,31,32,300);
            maxTime->type(FL_VERTICAL);
            maxTime->minimum(1);
            maxTime->step(1);
            maxTime->maximum(744);
            maxTime->value(param->minDeltaTime);
            maxTime->callback(OptionsWindow::cb_sliderChanged);

            {
                auto l = new verticalBox(0, 0, 24, 300, "Magnitude");
                l->labelcolor(FL_LIGHT1);
            }
            minMag = new Fl_Value_Slider(10,31,32,300,"MIN");
            minMag->type(FL_VERTICAL);
            minMag->align(FL_ALIGN_TOP);
            minMag->labelcolor(FL_LIGHT1);
            minMag->minimum(0);
            minMag->maximum(10);
            minMag->step(0.5);
            minMag->value(0);
            minMag->callback(OptionsWindow::cb_sliderChanged);
            maxMag = new Fl_Value_Slider(10,31,32,300,"MAX");
            maxMag->type(FL_VERTICAL);
            maxMag->align(FL_ALIGN_TOP);
            maxMag->labelcolor(FL_LIGHT1);
            maxMag->minimum(0);
            maxMag->maximum(10);
            maxMag->value(10);
            maxMag->callback(OptionsWindow::cb_sliderChanged);
            {
                auto l = new verticalBox(0, 0, 24, 300, "Depth");
                l->labelcolor(FL_LIGHT1);
            }
    slidersPack->end();
    pack->resizable(slidersPack);
    pack->end();
    tabs->end();
    tabs->resizable(pack);
}

void OptionsWindow::cb_showHideButton(Fl_Widget* parent, void* d)
{
    auto toSwap = ((unsigned int*) d);
    auto optionWindow = (OptionsWindow*)parent->window();
    auto param = optionWindow->param;
    unsigned short showFlags = param->showFlags;
    showFlags ^= (*toSwap);
    optionWindow->param->showFlags = showFlags;
}

void OptionsWindow::displayShowButton(const char *label, unsigned int toChange) {
    auto b = new Fl_Light_Button(0,0,150,32,label);
    unsigned int tf = ((param->showFlags)&toChange);
    b->value(tf);
    b->callback(OptionsWindow::cb_showHideButton,new unsigned int(toChange));
}

void OptionsWindow::cb_sliderChanged(Fl_Widget *parent, void *d) {
    auto optionWindow = (OptionsWindow*) parent->window();
    optionWindow->param->minDeltaTime = optionWindow->maxTime->value();
    if(optionWindow->minMag->value()>optionWindow->maxMag->value()){
        double temp = optionWindow->minMag->value();
        optionWindow->minMag->value(optionWindow->maxMag->value());
        optionWindow->maxMag->value(temp);
    }
    optionWindow->param->minMag = optionWindow->minMag->value();
    optionWindow->param->maxMag = optionWindow->maxMag->value();
    optionWindow->param->diskSize = optionWindow->diskSize->value()/1e5;
    optionWindow->param->ringSize = optionWindow->ringSize->value()/1e4;

    optionWindow->param->setChanged();
}

void OptionsWindow::cb_acceptImage(Fl_Widget *parent, void *d) {
    auto optionsWindow = (OptionsWindow*) d;
    auto fp = optionsWindow->getWmFilePath();
    optionsWindow->param->setWorldMapFile(fp.c_str());
    auto bg = optionsWindow->getBgFilePath();
    optionsWindow->param->setBackgroundFile(bg.c_str());
    optionsWindow->feg->reloadGraph();
}

std::string OptionsWindow::getWmFilePath() {
    std::string toReturn = this->wmFilePath->value();
    return toReturn;
}


std::string OptionsWindow::getBgFilePath() {
    std::string toReturn = this->bgFilePath->value();
    return toReturn;
}


