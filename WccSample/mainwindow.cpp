#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_panel.show();

    connect(&m_panel, SIGNAL(pointsReady(QVector<Point>)), this, SLOT(handlePoints(QVector<Point>)));
    connect(&m_panel, SIGNAL(movement(Movement,size_t)), this, SLOT(handleMovement(Movement,size_t)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handlePoints(QVector<Point> pts)
{
    QString text;

    for(Point &pnt: pts)
    {
        QString one(" " + QString::number(pnt.id()) + " " + QString::number(pnt.position().x) + " " + QString::number(pnt.position().y));
        text += one;
    }

    ui->positionslabel->setText(text);
}

void MainWindow::handleMovement(Movement mvmnt, size_t index)
{
    QString text(QString::number(index) + " "
                 + QString::number(mvmnt.direction().x) + " "
                 + QString::number(mvmnt.direction().y) + " "
                 + QString::number(mvmnt.direction().z) + " "
                                   + mvmnt.size());

    ui->positionslabel->setText(text);
}
