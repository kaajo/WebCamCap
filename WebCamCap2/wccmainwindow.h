#ifndef WCCMAINWINDOW_H
#define WCCMAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include "src/markercamera.h"

namespace Ui {
class WccMainWindow;
}

class WccMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WccMainWindow(QWidget *parent = 0);
    ~WccMainWindow();

private:
    Ui::WccMainWindow *ui;
};

#endif // WCCMAINWINDOW_H
