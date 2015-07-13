#include "camerawidget.h"
#include "ui_camerawidget.h"

CameraWidget::CameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraWidget)
{
    ui->setupUi(this);
}

CameraWidget::~CameraWidget()
{
    delete ui;
}
