#ifndef WCCMAINWINDOW_H
#define WCCMAINWINDOW_H

#include <QtWidgets/QMainWindow>

#include "ivirtualroom.h"

#include "src/virtualroom.h"
#include "src/markercamera.h"

#include "iserver.h"

namespace Ui {
class WccMainWindow;
}

class WccMainWindow : public QMainWindow
{
    Q_OBJECT

    IVirtualRoom *currentProject = nullptr;

    VirtualRoom * room = new VirtualRoom(this);

    QVector<IServer*> m_servers;

    QWidget *m_scrollWidget;
public:
    explicit WccMainWindow(QWidget *parent = 0);
    ~WccMainWindow();

private:
    Ui::WccMainWindow *m_ui = nullptr;

    bool addServer(IServer *server);
    bool removeServer(QString name);

private slots:
    void newProject();
    void openProject();
    void editCurrentProject();
    void saveCurrentProject();

    void addNewServer();
    void showAboutPage();
};

#endif // WCCMAINWINDOW_H
