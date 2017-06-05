#include "menuWidget.h"

MenuWidget::MenuWidget(DisplayWidget *displayWidget) {

    this->displayWidget = displayWidget;

    topHeightLabel = new QLabel("Top height");
    bottomHeightLabel = new QLabel("Bottom height");
    topRadiusLabel = new QLabel("Top radius");
    middleRadiusLabel = new QLabel("Middle radius");
    bottomRadiusLabel = new QLabel("Bottom radius");
    topNbPointsLabel = new QLabel("Top points");
    middleNbPointsLabel = new QLabel("Middle points");
    bottomNbPointsLabel = new QLabel("Bottom points");
    topComplexityLabel = new QLabel("Top complexity");
    bottomComplexityLabel = new QLabel("Bottom complexity");
    parametersLabel = new QLabel();

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

    colorButton = new QPushButton();
    colorDialog = new QColorDialog();

    this->setLayout(createLayout());

    initMenu();
}

QLayout *MenuWidget::createLayout() {

    QVBoxLayout *vLayout = new QVBoxLayout();
    QGridLayout *gLayout = new QGridLayout();

    gLayout->addWidget(topHeightLabel, 0, 0);
    gLayout->addWidget(topHeightSlider, 0, 1);
    gLayout->addWidget(topRadiusLabel, 0, 2);
    gLayout->addWidget(topRadiusSlider, 0, 3);
    gLayout->addWidget(topNbPointsLabel, 0, 4);
    gLayout->addWidget(topNbPointsSlider, 0, 5);
    gLayout->addWidget(topComplexityLabel, 0, 6);
    gLayout->addWidget(topComplexitySlider, 0, 7);

    gLayout->addWidget(bottomHeightLabel, 1, 0);
    gLayout->addWidget(bottomHeightSlider, 1, 1);
    gLayout->addWidget(bottomRadiusLabel, 1, 2);
    gLayout->addWidget(bottomRadiusSlider, 1, 3);
    gLayout->addWidget(bottomNbPointsLabel, 1, 4);
    gLayout->addWidget(bottomNbPointsSlider, 1, 5);
    gLayout->addWidget(bottomComplexityLabel, 1, 6);
    gLayout->addWidget(bottomComplexitySlider, 1, 7);

    gLayout->addWidget(middleRadiusLabel, 2, 0);
    gLayout->addWidget(middleRadiusSlider, 2, 1);
    gLayout->addWidget(middleNbPointsLabel, 2, 2);
    gLayout->addWidget(middleNbPointsSlider, 2, 3);

    gLayout->addWidget(colorButton, 2, 7);

    vLayout->addLayout(gLayout);
    vLayout->addWidget(parametersLabel);

    return vLayout;
}

void MenuWidget::initMenu() {

    topHeightSlider->setMaximum(10);
    topHeightSlider->setMinimum(1);
    topHeightSlider->setSingleStep(1);
    topHeightSlider->setValue((int) displayWidget->getGem()->getTopHeight());
    QWidget::connect(topHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
    bottomHeightSlider->setMaximum(10);
    bottomHeightSlider->setMinimum(1);
    bottomHeightSlider->setSingleStep(1);
    bottomHeightSlider->setValue((int) displayWidget->getGem()->getBottomHeight());
    QWidget::connect(bottomHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
    topRadiusSlider->setMaximum(10);
    topRadiusSlider->setMinimum(1);
    topRadiusSlider->setSingleStep(1);
    topRadiusSlider->setValue((int) displayWidget->getGem()->getTopRadius());
    QWidget::connect(topRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
    middleRadiusSlider->setMaximum(10);
    middleRadiusSlider->setMinimum(1);
    middleRadiusSlider->setSingleStep(1);
    middleRadiusSlider->setValue((int) displayWidget->getGem()->getMiddleRadius());
    QWidget::connect(middleRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
    bottomRadiusSlider->setMaximum(10);
    bottomRadiusSlider->setMinimum(1);
    bottomRadiusSlider->setSingleStep(1);
    bottomRadiusSlider->setValue((int) displayWidget->getGem()->getBottomRadius());
    QWidget::connect(bottomRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
    topNbPointsSlider->setMaximum(10);
    topNbPointsSlider->setMinimum(1);
    topNbPointsSlider->setSingleStep(1);
    topNbPointsSlider->setValue((int) displayWidget->getGem()->getTopNbPoints());
    QWidget::connect(topNbPointsSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
    middleNbPointsSlider->setMaximum(10);
    middleNbPointsSlider->setMinimum(1);
    middleNbPointsSlider->setSingleStep(1);
    middleNbPointsSlider->setValue((int) displayWidget->getGem()->getMiddleNbPoints());
    QWidget::connect(middleNbPointsSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
    bottomNbPointsSlider->setMaximum(10);
    bottomNbPointsSlider->setMinimum(1);
    bottomNbPointsSlider->setSingleStep(1);
    bottomNbPointsSlider->setValue((int) displayWidget->getGem()->getBottomNbPoints());
    QWidget::connect(bottomNbPointsSlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
    topComplexitySlider->setMaximum(10);
    topComplexitySlider->setMinimum(1);
    topComplexitySlider->setSingleStep(1);
    topComplexitySlider->setValue((int) displayWidget->getGem()->getTopComplexity());
    QWidget::connect(topComplexitySlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));
    bottomComplexitySlider->setMaximum(10);
    bottomComplexitySlider->setMinimum(1);
    bottomComplexitySlider->setSingleStep(1);
    bottomComplexitySlider->setValue((int) displayWidget->getGem()->getBottomComplexity());
    QWidget::connect(bottomComplexitySlider, SIGNAL(valueChanged(int)), this, SLOT(updateGem(int)));

    colorDialog->setCurrentColor(displayWidget->getGem()->getColor());

    colorButton->setStyleSheet("background-color: " + colorDialog->currentColor().name() + ";");
    colorButton->setToolTip("Gem color");
    colorButton->setFixedSize(20, 20);
    colorButton->setLayoutDirection(Qt::RightToLeft);

    QWidget::connect(colorButton, SIGNAL(clicked()), colorDialog, SLOT(open()));
    QWidget::connect(colorDialog, SIGNAL(accepted()), this, SLOT(changeColor()));

    setParametersLabelText();
}

void MenuWidget::updateGem(int) {
    setParametersLabelText();
    displayWidget->updateGem(topHeightSlider->value(), bottomHeightSlider->value(), topRadiusSlider->value(),
                             middleRadiusSlider->value(), bottomRadiusSlider->value(), topNbPointsSlider->value(),
                             middleNbPointsSlider->value(), bottomNbPointsSlider->value(), topComplexitySlider->value(),
                             bottomComplexitySlider->value(), colorDialog->currentColor());
}

void MenuWidget::changeColor() {
    colorButton->setStyleSheet("background-color: " + colorDialog->currentColor().name() + ";");
    updateGem(0);
}

void MenuWidget::setParametersLabelText() {
    parametersLabel->setText("Gem(Top height : " + QString::number(topHeightSlider->value()) +
                                     " Bottom height : " + QString::number(bottomHeightSlider->value()) +
                                     " Top radius : " + QString::number(topRadiusSlider->value()) +
                                     " Middle radius : " + QString::number(middleRadiusSlider->value()) +
                                     " Bottom radius : " + QString::number(bottomRadiusSlider->value()) +
                                     " Top nb points : " + QString::number(topNbPointsSlider->value()) +
                                     " Middle nb points : " + QString::number(middleNbPointsSlider->value()) +
                                     " Bottom nb points : " + QString::number(bottomNbPointsSlider->value()) +
                                     " Top complexity : " + QString::number(topComplexitySlider->value()) +
                                     " Bottom complexity : " + QString::number(bottomComplexitySlider->value()) +
                                     " Color : " + colorDialog->currentColor().name() + ")");
}
