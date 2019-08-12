//
// Created by isatis on 16/07/19.
//

#include "MainWindow.h"

MainWindow::MainWindow(int x, int y, int w, int h):
    Fl_Window(x,y,w,h,"FullEarth")
{
    // Load Settings
    param = new Parametres();
    param->load("./res/settings");

    // Prepare Options Windows
    this->end();
    optionsWindow=new OptionsWindow(param);
    color(FL_BLACK);

    // Build the Window
    this->begin();
    top_controls = new Fl_Group(0,0,w,24);
    top_controls->begin();
    auto fixed = new Fl_Group(0,0,300,24);
    fixed->begin();
    auto latest_button = new Fl_Button(100,0,100,24,"Latest @menu");
    latest_button->color(FL_DARK3);
    latest_button->labelcolor(FL_WHITE);

    auto largest_button = new Fl_Button(200,0,100,24,"Largest @menu");
    largest_button->color(FL_DARK3);
    largest_button->labelcolor(FL_WHITE);

    list_button = new Fl_Button(0,0,100,24,"List @<");
    list_button->color(fl_rgb_color(100,100,100));
    list_button->color(FL_DARK3);
    list_button->labelcolor(FL_WHITE);
    fixed->end();
    fixed->resizable(nullptr);
    source_choice = new Fl_Choice(w-140,0,140,24,"Source:");
    source_choice->labelcolor(FL_WHITE);
    source_choice->color(FL_DARK3);
    top_controls->resizable(top_controls);
    top_controls->end();

    bottom_label = new Fl_Group(0,h-24,w,24);
    bottom_label->begin();
    auto optionButton = new Fl_Button(w-100,h-24,100,24,"Options");
    optionButton->color(FL_DARK3);
    optionButton->labelcolor(FL_WHITE);
    status_bar = new Fl_Box(0,h-24,w-100,24,"- Loading... - ");
    status_bar->labelcolor(FL_WHITE);
    bottom_label->end();
    bottom_label->resizable(bottom_label);

    fullEarth = new FullEarth(120,24,640-300,480-64,param, 0L);
    list_scroll = new Fl_Scroll(0,24,300,480-64);
    eqList = new EQList(0,24,300,480-64,param);
    list_scroll->type(Fl_Scroll::VERTICAL_ALWAYS);
    list_scroll->scrollbar.color(FL_DARK3);
    list_scroll->color(FL_BLACK);
    eqList->setEq(fullEarth->getEqJson());
    this->end();

    source_choice->add("USGS - Hourly",  0,  set_source, (void*)(new MenuOption{ONEHOUR_RES,fullEarth}));
    source_choice->add("USGS - Daily",   0,   set_source, (void*)(new MenuOption{DAY_RES,fullEarth}));
    source_choice->add("USGS - Monthly", 0, set_source, (void*)(new MenuOption{MONTH_RES,fullEarth}));
    source_choice->add("EMS - Last 500 (BROKEN)", 0, set_source, (void*)(new MenuOption{EUR_500,fullEarth}));

    source_choice->value(0);
    latest_button->callback(MainWindow::button_clicked, new int(0x01));
    largest_button->callback(MainWindow::button_clicked, new int(0x02));
    list_button->callback(MainWindow::button_clicked, new int(0x03));
    eqList->callback(MainWindow::button_clicked,new int(0x04));
    optionButton->callback(MainWindow::button_clicked,new int(0x05));
    optionsWindow->eqList = eqList;
    optionsWindow->feg = fullEarth;
    fullEarth->setStatusBar(status_bar);
    fullEarth->setSource(ONEHOUR_RES);
}

MainWindow::~MainWindow() {

}

void MainWindow::set_source(Fl_Widget* parent, void* data){
    MenuOption* inlet = (MenuOption*) data;
    std::cout << inlet->source << std::endl;
    std::string url(inlet->source);
    inlet->target->setSource(url);
}

void MainWindow::resize(int x, int y, int w, int h){
    top_controls->resize(0,    0,   w,   24);
    top_controls->redraw();
    if(list_scroll->visible()) {
        list_scroll->resize(0, 24, 300, h - 48);
        fullEarth->resize(   300,   24,   w-300, h-48);
    }else{
        fullEarth->resize(0,24,w,h-48);
    }
    bottom_label->resize(0, h - 24, w, 24);
    bottom_label->redraw();
    Fl_Widget::resize(x,y,w,h);
}


void MainWindow::button_clicked(Fl_Widget *parent, void *data) {
    auto flag = (int*) data;
    auto window =  (MainWindow*) parent->window();
    auto fullEarth = window->fullEarth;
    auto eqWidget = window->eqList;
    auto eqList = fullEarth->getEqJson();
    bool showOption = false;
    switch(*flag){
        case 0x01:
            eqList->unselect();
            eqList->latestEq()->selected = true;
            fullEarth->lookAt(eqList->latestEq());
            window->list_scroll->scroll_to(0,0);
            eqList->sortTime();
            eqWidget->redraw();
            break;
        case 0x02:
            eqList->unselect();
            eqList->largestEq()->selected = true;
            fullEarth->lookAt(eqList->largestEq());
            eqList->sortMagnitude();
            eqWidget->redraw();
            break;
        case 0x03:
            if(window->list_scroll->visible()) {
                window->list_scroll->hide();
                window->list_button->label("List @>");
            }else{
                window->list_scroll->show();
                window->list_button->label("List @<");
            }
            window->resize(window->x(),window->y(),window->w(),window->h());
            break;
        case 0x04:
            fullEarth->lookAt(eqWidget->getLastClicked());
            break;
        case 0x05:
            showOption = true;
            break;
        default:
            std::cout << "Not implemented." <<std::endl;
            break;
    }
    if(showOption) {
        std::cout << "Open Option Window" << std::endl;
        window->optionsWindow->position(parent->x()-window->optionsWindow->w(),parent->y()-window->optionsWindow->h());
        window->optionsWindow->show();
    }
}
