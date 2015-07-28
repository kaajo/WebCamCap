#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_panel.show();

    connect(&m_panel, &ControlPanel::pointsReady, this, &MainWindow::handlePoints);
    connect(&m_panel, &ControlPanel::movement, this, &MainWindow::handleMovement);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handlePoints(QVector<Marker> pts)
{
    QString text;

    for(Marker &pnt: pts)
    {
        QString one(" " + QString::number(pnt.id()) + " " + QString::number(pnt.position().x()) + " " + QString::number(pnt.position().y()) + " " + QString::number(pnt.position().z()));
        text += one;
    }

    ui->positions->setText(text);
}

void MainWindow::handleMovement(Movement mvmnt, size_t index)
{
    QString text(QString::number(index) + " "
                 + QString::number(mvmnt.direction().x) + " "
                 + QString::number(mvmnt.direction().y) + " "
                 + QString::number(mvmnt.direction().z) + " "
                                   + mvmnt.size());

    ui->movements->setText(text);
}
