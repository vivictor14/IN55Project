#include "menuWidget.h"

MenuWidget::MenuWidget(DisplayWidget *displayWidget) {

    this->displayWidget = displayWidget;

    topHeightLabel = new QLabel("Top height", this);
    bottomHeightLabel = new QLabel("Bottom height", this);
    topRadiusLabel = new QLabel("Top radius", this);
    middleRadiusLabel = new QLabel("Middle radius", this);
    bottomRadiusLabel = new QLabel("Bottom radius", this);
    topNbPointsLabel = new QLabel("Top points", this);
    middleNbPointsLabel = new QLabel("Middle points", this);
    bottomNbPointsLabel = new QLabel("Bottom points", this);
    topComplexityLabel = new QLabel("Top complexity", this);
    bottomComplexityLabel = new QLabel("Bottom complexity", this);
    lengthStretchingPercentLabel = new QLabel("Length stretching percent", this);
    widthStretchingPercentLabel = new QLabel("Width stretching percent", this);
    templateLabel = new QLabel("Templates :", this);
    parametersLabel = new QLabel(this);

    topHeightSlider = new QSlider(Qt::Horizontal, this);
    bottomHeightSlider = new QSlider(Qt::Horizontal, this);
    topRadiusSlider = new QSlider(Qt::Horizontal, this);
    middleRadiusSlider = new QSlider(Qt::Horizontal, this);
    bottomRadiusSlider = new QSlider(Qt::Horizontal, this);
    topNbPointsSlider = new QSlider(Qt::Horizontal, this);
    middleNbPointsSlider = new QSlider(Qt::Horizontal, this);
    bottomNbPointsSlider = new QSlider(Qt::Horizontal, this);
    topComplexitySlider = new QSlider(Qt::Horizontal, this);
    bottomComplexitySlider = new QSlider(Qt::Horizontal, this);
    lengthStretchingPercentSlider = new QSlider(Qt::Horizontal, this);
    widthStretchingPercentSlider = new QSlider(Qt::Horizontal, this);
    autoRotateCheckBox = new QCheckBox("Rotation auto", this);

    colorButton = new QPushButton(this);
    colorDialog = new QColorDialog(this);

    dropTemplateButton = new QPushButton(this);
    triangleTemplateButton = new QPushButton(this);
    brilliantTemplateButton = new QPushButton(this);
    marquiseTemplateButton = new QPushButton(this);
    princessTemplateButton = new QPushButton(this);
    ovalTemplateButton = new QPushButton(this);
    hexagonTemplateButton = new QPushButton(this);

    this->setLayout(createLayout());

    initMenu();
}

QLayout *MenuWidget::createLayout() {

    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QGridLayout *gLayout = new QGridLayout();

    gLayout->addWidget(topRadiusLabel, 0, 0);
    gLayout->addWidget(topRadiusSlider, 0, 1);
    gLayout->addWidget(topNbPointsLabel, 0, 2);
    gLayout->addWidget(topNbPointsSlider, 0, 3);
    gLayout->addWidget(topHeightLabel, 0, 4);
    gLayout->addWidget(topHeightSlider, 0, 5);
    gLayout->addWidget(topComplexityLabel, 0, 6);
    gLayout->addWidget(topComplexitySlider, 0, 7);

    gLayout->addWidget(bottomRadiusLabel, 1, 0);
    gLayout->addWidget(bottomRadiusSlider, 1, 1);
    gLayout->addWidget(bottomNbPointsLabel, 1, 2);
    gLayout->addWidget(bottomNbPointsSlider, 1, 3);
    gLayout->addWidget(bottomHeightLabel, 1, 4);
    gLayout->addWidget(bottomHeightSlider, 1, 5);
    gLayout->addWidget(bottomComplexityLabel, 1, 6);
    gLayout->addWidget(bottomComplexitySlider, 1, 7);

    gLayout->addWidget(middleRadiusLabel, 2, 0);
    gLayout->addWidget(middleRadiusSlider, 2, 1);
    gLayout->addWidget(middleNbPointsLabel, 2, 2);
    gLayout->addWidget(middleNbPointsSlider, 2, 3);
    gLayout->addWidget(lengthStretchingPercentLabel, 2, 4);
    gLayout->addWidget(lengthStretchingPercentSlider, 2, 5);
    gLayout->addWidget(widthStretchingPercentLabel, 2, 6);
    gLayout->addWidget(widthStretchingPercentSlider, 2, 7);


    gLayout->addWidget(colorButton, 3, 7);
    gLayout->addWidget(autoRotateCheckBox, 3, 0);

    hLayout->addSpacing(2000);
    hLayout->addWidget(templateLabel);
    hLayout->addWidget(dropTemplateButton);
    hLayout->addWidget(triangleTemplateButton);
    hLayout->addWidget(brilliantTemplateButton);
    hLayout->addWidget(marquiseTemplateButton);
    hLayout->addWidget(princessTemplateButton);
    hLayout->addWidget(ovalTemplateButton);
    hLayout->addWidget(hexagonTemplateButton);

    vLayout->addLayout(gLayout);
    vLayout->addWidget(parametersLabel);
    vLayout->addLayout(hLayout);

    return vLayout;
}

void MenuWidget::initMenu() {

    initSlider(topHeightSlider, 1, 10);
    initSlider(bottomHeightSlider, 1, 10);
    initSlider(topRadiusSlider, 1, 10);
    initSlider(middleRadiusSlider, 1, 10);
    initSlider(bottomRadiusSlider, 1, 10);
    initSlider(topNbPointsSlider, 2, 20);
    initSlider(middleNbPointsSlider, 3, 20);
    initSlider(bottomNbPointsSlider, 2, 20);
    initSlider(topComplexitySlider, 1, 10);
    initSlider(bottomComplexitySlider, 1, 10);
    initSlider(lengthStretchingPercentSlider, 10, 300);
    initSlider(widthStretchingPercentSlider, 10, 300);

    updateControls();
    updateToolTips();

    colorDialog->setCurrentColor(displayWidget->getGem()->getColor());

    colorButton->setStyleSheet("background-color: " + colorDialog->currentColor().name() + ";");
    colorButton->setToolTip("Gem color");
    colorButton->setFixedSize(20, 20);
    colorButton->setLayoutDirection(Qt::RightToLeft);

    QWidget::connect(colorButton, SIGNAL(clicked()), colorDialog, SLOT(open()));
    QWidget::connect(colorDialog, SIGNAL(accepted()), this, SLOT(changeColor()));

    autoRotateCheckBox->setChecked(displayWidget->isAutoRotate());
    QWidget::connect(autoRotateCheckBox, SIGNAL(stateChanged(int)), displayWidget, SLOT(triggerAutoRotation(int)));

    initTemplateButton(dropTemplateButton, "Drop", "");
    QWidget::connect(dropTemplateButton, SIGNAL(clicked()), this, SLOT(applyDropTemplate()));
    initTemplateButton(triangleTemplateButton, "Triangle", "");
    QWidget::connect(triangleTemplateButton, SIGNAL(clicked()), this, SLOT(applyTriangleTemplate()));
    initTemplateButton(brilliantTemplateButton, "Brilliant", "");
    QWidget::connect(brilliantTemplateButton, SIGNAL(clicked()), this, SLOT(applyBrilliantTemplate()));
    initTemplateButton(marquiseTemplateButton, "Marquise", "");
    QWidget::connect(marquiseTemplateButton, SIGNAL(clicked()), this, SLOT(applyMarquiseTemplate()));
    initTemplateButton(princessTemplateButton, "Princess", "");
    QWidget::connect(princessTemplateButton, SIGNAL(clicked()), this, SLOT(applyPrincessTemplate()));
    initTemplateButton(ovalTemplateButton, "Oval", "");
    QWidget::connect(ovalTemplateButton, SIGNAL(clicked()), this, SLOT(applyOvalTemplate()));
    initTemplateButton(hexagonTemplateButton, "Hexagon", "");
    QWidget::connect(hexagonTemplateButton, SIGNAL(clicked()), this, SLOT(applyHexagonTemplate()));

    setParametersLabelText();

}

void MenuWidget::initSlider(QSlider *slider, int minimum, int maximum) {
    slider->setMinimum(minimum);
    slider->setMaximum(maximum);
    QWidget::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
}

void MenuWidget::initTemplateButton(QPushButton *templateButton, QString name, QString imagePath) {
    templateButton->setFixedSize(40, 40);
    templateButton->setToolTip(name);
    templateButton->setIcon(QIcon(imagePath));
    templateButton->setIconSize(QSize(40, 40));
}

void MenuWidget::updateGem(int) {

    int topNbPoints = topNbPointsSlider->value();
    int bottomNbPoints = bottomNbPointsSlider->value();

    if(topNbPoints == 2) {
        topNbPoints = 1;
    }
    if(bottomNbPoints == 2) {
        bottomNbPoints = 1;
    }

    displayWidget->updateGem(topHeightSlider->value(), bottomHeightSlider->value(), topRadiusSlider->value(),
                             middleRadiusSlider->value(), bottomRadiusSlider->value(), topNbPoints,
                             middleNbPointsSlider->value(), bottomNbPoints, topComplexitySlider->value(),
                             bottomComplexitySlider->value(), lengthStretchingPercentSlider->value(),
                             widthStretchingPercentSlider->value(), colorDialog->currentColor());
    updateToolTips();
    setParametersLabelText();

}

void MenuWidget::changeColor() {

    colorButton->setStyleSheet("background-color: " + colorDialog->currentColor().name() + ";");
    updateGem(0);

}

void MenuWidget::updateToolTips() {

    int topNbPoints = topNbPointsSlider->value();
    int bottomNbPoints = bottomNbPointsSlider->value();

    if(topNbPoints == 2) {
        topNbPoints = 1;
    }
    if(bottomNbPoints == 2) {
        bottomNbPoints = 1;
    }

    topHeightSlider->setToolTip(QString::number(topHeightSlider->value()));
    bottomHeightSlider->setToolTip(QString::number(bottomHeightSlider->value()));
    topRadiusSlider->setToolTip(QString::number(topRadiusSlider->value()));
    middleRadiusSlider->setToolTip(QString::number(middleRadiusSlider->value()));
    bottomRadiusSlider->setToolTip(QString::number(bottomRadiusSlider->value()));
    topNbPointsSlider->setToolTip(QString::number(topNbPoints));
    middleNbPointsSlider->setToolTip(QString::number(middleNbPointsSlider->value()));
    bottomNbPointsSlider->setToolTip(QString::number(bottomNbPoints));
    topComplexitySlider->setToolTip(QString::number(topComplexitySlider->value()));
    bottomComplexitySlider->setToolTip(QString::number(bottomComplexitySlider->value()));
    lengthStretchingPercentSlider->setToolTip(QString::number(lengthStretchingPercentSlider->value()) + "%");

}

void MenuWidget::applyDropTemplate() {
    displayWidget->updateGem(10, 2, 1, 3, 2, 8, 8, 1, 3, 4, 100, 100, QColor("Aqua"));
    updateControls();
    updateToolTips();
    setParametersLabelText();
}

void MenuWidget::applyTriangleTemplate() {
    displayWidget->updateGem(1, 1, 2, 9, 2, 5, 3, 5, 5, 5, 100, 100, QColor("Red"));
    updateControls();
    updateToolTips();
    setParametersLabelText();
}

void MenuWidget::applyBrilliantTemplate() {
    displayWidget->updateGem(2, 6, 4, 7, 5, 8, 16, 1, 3, 4, 100, 100, QColor("White"));
    updateControls();
    updateToolTips();
    setParametersLabelText();
}

void MenuWidget::applyMarquiseTemplate() {
    displayWidget->updateGem(3, 3, 4, 7, 5, 6, 12, 6, 2, 2, 180, 100, QColor("Pink"));
    updateControls();
    updateToolTips();
    setParametersLabelText();
}

void MenuWidget::applyPrincessTemplate() {
    displayWidget->updateGem(2, 1, 3, 7, 3, 4, 4, 4, 2, 2, 100, 100, QColor("Green"));
    updateControls();
    updateToolTips();
    setParametersLabelText();
}

void MenuWidget::applyOvalTemplate() {
    displayWidget->updateGem(2, 4, 4, 7, 5, 20, 20, 20, 1, 1, 150, 100, QColor("Purple"));
    updateControls();
    updateToolTips();
    setParametersLabelText();
}

void MenuWidget::applyHexagonTemplate() {
    displayWidget->updateGem(2, 6, 4, 7, 5, 6, 6, 1, 2, 4, 100, 100, QColor("Grey"));
    updateControls();
    updateToolTips();
    setParametersLabelText();
}

void MenuWidget::updateControls() {

    int topNbPoints = displayWidget->getGem()->getTopNbPoints();
    int bottomNbPoints = displayWidget->getGem()->getBottomNbPoints();

    if(topNbPoints == 1) {
        topNbPoints = 2;
    }
    if(bottomNbPoints == 1) {
        bottomNbPoints = 2;
    }

    updateSlider(topHeightSlider, (int)displayWidget->getGem()->getTopHeight());
    updateSlider(bottomHeightSlider, (int) displayWidget->getGem()->getBottomHeight());
    updateSlider(topRadiusSlider, (int) displayWidget->getGem()->getTopRadius());
    updateSlider(middleRadiusSlider, (int) displayWidget->getGem()->getMiddleRadius());
    updateSlider(bottomRadiusSlider, (int) displayWidget->getGem()->getBottomRadius());
    updateSlider(topNbPointsSlider, topNbPoints);;
    updateSlider(middleNbPointsSlider, (int) displayWidget->getGem()->getMiddleNbPoints());
    updateSlider(bottomNbPointsSlider, bottomNbPoints);
    updateSlider(topComplexitySlider, (int) displayWidget->getGem()->getTopComplexity());
    updateSlider(bottomComplexitySlider, (int) displayWidget->getGem()->getBottomComplexity());
    updateSlider(lengthStretchingPercentSlider, (int) displayWidget->getGem()->getLengthStretchingPercent());
    updateSlider(widthStretchingPercentSlider, (int) displayWidget->getGem()->getWidthStretchingPercent());
    colorDialog->setCurrentColor(displayWidget->getGem()->getColor());
    colorButton->setStyleSheet("background-color: " + colorDialog->currentColor().name() + ";");

}

void MenuWidget::updateSlider(QSlider *slider, int value) {

    slider->blockSignals(true);
    slider->setValue(value);
    slider->blockSignals(false);

}

void MenuWidget::setParametersLabelText() {
    parametersLabel->setWordWrap(true);
    parametersLabel->setText("Gem(Top height: " + QString::number(displayWidget->getGem()->getTopHeight()) +
                             ", Bottom height: " + QString::number(displayWidget->getGem()->getBottomHeight()) +
                             ", Top radius: " + QString::number(displayWidget->getGem()->getTopRadius()) +
                             ", Middle radius: " + QString::number(displayWidget->getGem()->getMiddleRadius()) +
                             ", Bottom radius: " + QString::number(displayWidget->getGem()->getBottomRadius()) +
                             ", Top nb points: " + QString::number(displayWidget->getGem()->getTopNbPoints()) +
                             ", Middle nb points: " + QString::number(displayWidget->getGem()->getMiddleNbPoints()) +
                             ", Bottom nb points: " + QString::number(displayWidget->getGem()->getBottomNbPoints()) +
                             ", Top complexity: " + QString::number(displayWidget->getGem()->getTopComplexity()) +
                             ", Bottom complexity: " + QString::number(displayWidget->getGem()->getBottomComplexity()) +
                             ", Length stretching percent: " + QString::number(displayWidget->getGem()->getLengthStretchingPercent()) +
                             ", Width stretching percent: " + QString::number(displayWidget->getGem()->getWidthStretchingPercent()) +
                             ", Color : " + displayWidget->getGem()->getColor().name() + ")");
}
