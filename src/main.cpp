#include <iostream>
#if defined(_WIN64)
    #include <winsock2.h>
#endif
#include "MainWindow.h"
#include "Parametres.h"

int main(int argn, char* argv[]) {

    MainWindow mainWindow(0,0,1280,960);
    mainWindow.resizable(mainWindow);
    mainWindow.show();
    mainWindow.fullEarth->show();

    return Fl::run();
}