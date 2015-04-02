#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controlpanel.h"

#include <QVector>
#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void handlePoints(QVector<Point> pts);
    void handleMovement(Movement mvmnt, size_t index);

private:
    Ui::MainWindow *ui;

    ControlPanel m_panel;


};

#endif // MAINWINDOW_H
