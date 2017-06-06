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
    QLabel *lengthStretchingPercentLabel;
    QLabel *widthStretchingPercentLabel;
    QLabel *templateLabel;

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
    QSlider *lengthStretchingPercentSlider;
    QSlider *widthStretchingPercentSlider;
    QPushButton *colorButton;
    QColorDialog *colorDialog;
    QCheckBox *autoRotateCheckBox;

    QPushButton *dropTemplateButton;
    QPushButton *triangleTemplateButton;
    QPushButton *brilliantTemplateButton;
    QPushButton *marquiseTemplateButton;
    QPushButton *princessTemplateButton;
    QPushButton *ovalTemplateButton;
    QPushButton *hexagonTemplateButton;

    QLayout *createLayout();
    void initSlider(QSlider *slider, int minimum, int maximum);
    void initTemplateButton(QPushButton *templateButton, QString name, QString imagePath);
    void initMenu();
    void updateToolTips();
    void updateControls();
    void updateSlider(QSlider *slider, int value);
    void setParametersLabelText();

public:

    MenuWidget(DisplayWidget *displayWidget);

protected slots:

    void updateGem(int);
    void changeColor();
    void applyDropTemplate();
    void applyTriangleTemplate();
    void applyBrilliantTemplate();
    void applyMarquiseTemplate();
    void applyPrincessTemplate();
    void applyOvalTemplate();
    void applyHexagonTemplate();

};


#endif //IN55PROJECT_MENUWIDGET_H
