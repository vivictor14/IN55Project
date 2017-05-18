#include <QApplication>
#include "Windows/displayWindow.h"


int main(int argv, char **args) {
    QApplication app(argv, args);

    DisplayWindow win;
    win.show();

    return app.exec();
}