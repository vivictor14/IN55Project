//
// Created by Victor on 04/06/2017.
//

#ifndef IN55PROJECT_MENUWIDGET_H
#define IN55PROJECT_MENUWIDGET_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QColorDialog>
#include <QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QCheckBox>
#include "displayWidget.h"

class MenuWidget : public QWidget {

Q_OBJECT

private:

    DisplayWidget *displayWidget;

    QLabel *parametersLabel;

    QLabel *topHeightLabel;
    QLabel *bottomHeightLabel;
    QLabel *topRadiusLabel;
    QLabel *middleRadiusLabel;
    QLabel *bottomRadiusLabel;
    QLabel *topNbPointsLabel;
    QLabel *middleNbPointsLabel;
    QLabel *bottomNbPointsLabel;
    QLabel *topComplexityLabel;
    QLabel *bottomComplexityLabel;

    QSlider *topHeightSlider;
    QSlider *bottomHeightSlider;
    QSlider *topRadiusSlider;
    QSlider *middleRadiusSlider;
    QSlider *bottomRadiusSlider;
    QSlider *topNbPointsSlider;
    QSlider *middleNbPointsSlider;
    QSlider *bottomNbPointsSlider;
    QSlider *topComplexitySlider;
    QSlider *bottomComplexitySlider;
    QPushButton *colorButton;
    QColorDialog *colorDialog;
    QCheckBox *autoRotateCheckBox;

    QLayout *createLayout();
    void initMenu();
    void setParametersLabelText();

public:

    MenuWidget(DisplayWidget *displayWidget);

protected slots:

    void updateGem(int);
    void changeColor();

};


#endif //IN55PROJECT_MENUWIDGET_H
