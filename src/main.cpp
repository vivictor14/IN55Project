#include <QApplication>
#include <QMainWindow>
#include "displayWidget.h"


int main(int argv, char **args) {
    QApplication app(argv, args);

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);

    DisplayWidget *widget = new DisplayWidget();
    widget->setFormat(format);

    QMainWindow window;
    window.setCentralWidget(widget);
    window.resize(QSize(800, 600));
    window.show();

    return app.exec();
}