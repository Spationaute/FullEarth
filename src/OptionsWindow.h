#ifndef OPTWIND
#define OPTWIND
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Value_Slider.H>
#include "Parametres.h"
#include "EQList.h"
#include "FullEarth.h"

class OptionsWindow : public Fl_Window {
public:
    OptionsWindow(Parametres* param);
    ~OptionsWindow();

    std::string getWmFilePath();
    std::string getBgFilePath();
    Fl_File_Input* wmFilePath;
    Fl_File_Input* bgFilePath;
    Fl_File_Input* sourceFilePath;
    Fl_Value_Slider* maxTime;
    Fl_Value_Slider* minMag;
    Fl_Value_Slider* maxMag;
    Fl_Value_Slider* maxDepth;
    Fl_Value_Slider* ringSize;
    Fl_Value_Slider* diskSize;

    Parametres* param;
    EQList* eqList;
    FullEarth* feg;
    static void cb_setFile(Fl_Widget* p, void* d);
    static void cb_showHideButton(Fl_Widget* parent, void* d);
    static void cb_sliderChanged(Fl_Widget* parent,void* d);
    static void cb_acceptImage(Fl_Widget* parent,void* d);

private:
    void settingsPanel();
    void displayFileInput(Fl_File_Input* &to_dysp, const char* baseUrl);
    void displayPanel();
    void displayShowButton(const char* label, unsigned int toChange);
    void filterPanel();

};

#endif