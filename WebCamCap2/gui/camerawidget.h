#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include "camerasettings.h"
#include "qtopencvviewergl.h"

#include <QWidget>

#include <memory>

namespace Ui {
class CameraWidget;
}

class CameraWidget : public QWidget
{
    Q_OBJECT

    std::shared_ptr<CameraSettings> m_settings = nullptr;

public:
    explicit CameraWidget(std::shared_ptr<CameraSettings> settings, QWidget *parent = 0);
    ~CameraWidget();

    QtOpenCVViewerGl *cameraVideoPreview();

private:
    Ui::CameraWidget *m_ui;

private slots:
    void handleMinSize(bool show);
};

#endif // CAMERAWIDGET_H
