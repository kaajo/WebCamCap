#include "wccmainwindow.h"
#include "ui_wccmainwindow.h"

WccMainWindow::WccMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WccMainWindow)
{
    ui->setupUi(this);
}

WccMainWindow::~WccMainWindow()
{
    delete ui;
}
