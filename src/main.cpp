#include <QApplication>
#include <QMainWindow>
#include "displayWidget.h"
#include "menuWidget.h"


int main(int argv, char **args) {
    QApplication app(argv, args);

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3, 3);


    DisplayWidget *displayWidget = new DisplayWidget();
    displayWidget->setFormat(format);

    MenuWidget *menuWidget = new MenuWidget(displayWidget);


    QMainWindow window;
    window.setCentralWidget(displayWidget);
    window.setMenuWidget(menuWidget);

    window.resize(QSize(800, 600));
    window.show();

    return app.exec();
}