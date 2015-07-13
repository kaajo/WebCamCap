#include "camerawidget.h"
#include "ui_camerawidget.h"


CameraWidget::CameraWidget(CameraSettings *settings, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::CameraWidget)
{
    m_ui->setupUi(this);

    m_settings = settings;
    connect(m_ui->showVideoCheckBox, &QCheckBox::clicked, m_ui->widget, &QtOpenCVViewerGl::setVisible);
    connect(m_ui->showVideoCheckBox, &QCheckBox::clicked, this, &CameraWidget::handleMinSize);

    m_ui->widget->setVisible(false);
    handleMinSize(false);

    m_ui->thresholdSpinBox->setValue(m_settings->getThresholdValue());
    connect(m_ui->thresholdSpinBox, SIGNAL(valueChanged(int)), m_settings, SLOT(setThresholdValue(int)));
}

CameraWidget::~CameraWidget()
{
    delete m_ui;
}

QtOpenCVViewerGl *CameraWidget::cameraVideoPreview()
{
    return m_ui->widget;
}

void CameraWidget::handleMinSize(bool show)
{
    if(show)
    {
        this->setMinimumHeight(279);
    }
    else
    {
        this->setMinimumHeight(50);
    }

    m_settings->setShowWindow(show);
}
