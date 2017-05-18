#ifndef IN55PROJECT_DISPLAYWINDOW_H
#define IN55PROJECT_DISPLAYWINDOW_H

#include <QGLWidget>
#include <QKeyEvent>

class DisplayWindow : public QGLWidget {
public:
    DisplayWindow();
    ~DisplayWindow();

private:
    void render();

    void keyPressEvent(QKeyEvent *event);

};


#endif //IN55PROJECT_DISPLAYWINDOW_H
