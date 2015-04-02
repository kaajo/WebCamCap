#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "movement.h"
#include "myfifo.h"

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QVector>
#include <QMap>

namespace Ui {
class ControlPanel;
}

/**
 * @author Miroslav Krajicek <409917@mail.muni.cz>
 * @class ControlPanel
 * @brief Qt Designer form class with all settings needed for WebCamCap game.
 * @date 2015
 */

class ControlPanel : public QMainWindow
{
    Q_OBJECT

    MyFifo *m_fifo;
    QTimer *m_reconnectTimer;

    bool m_sendPositions = false;
    bool m_sendMovement = false;
    int m_nextMovementCaptureTime;
    float m_sensitivity, minSize, minAcceleration, minSpeed;

    float difTemp;

    QMap<size_t, Point> points, lastPoints;
    QMap<size_t, Point> diff, lastDiff, acceleration;

    //move
    QTime *m_movementTimer;
    float m_minSizeOfMovement = 0.0f;
    glm::vec3 m_startMovementPoint;
    glm::vec3 m_endMovementPoint;

public:

    /**
     * @brief ControlPanel
     * @param parent
     */

    explicit ControlPanel(QWidget *parent = 0);

    ~ControlPanel();

    /**
     * @brief getFifo
     * @return
     */

    MyFifo *getFifo() const {return m_fifo;}

    /**
     * @brief setFifo
     * @param value
     */

    void setFifo(MyFifo *value) {m_fifo = value;}

public slots:
    void handlePoints(QVector<Point> pts);

private slots:

    void tryConnect();
    void connected();
    void disconnected();



    //gui slots
    void on_positions_stateChanged(int arg1);

    void on_movement_stateChanged(int arg1);

    void on_sensitivity_valueChanged(double arg1);

    void on_speed_valueChanged(double arg1);

    void on_acceleration_valueChanged(double arg1);

    void on_size_valueChanged(double arg1);

    void on_timeout_valueChanged(int arg1);

signals:

    /**
     * @brief movement
     * @param move
     * @param index
     */

    void movement(Movement move, size_t index);

    /**
     * @brief pointsReady
     * @param pts
     */

    void pointsReady(QVector<Point> pts);

private:
    Ui::ControlPanel *ui;

    void makeDiff(QVector<Point> &pts);

    void handlePositions();
    void handleMovements();
};

#endif // CONTROLPANEL_H
