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
 * @author Miroslav Krajicek <mirokrajicek@gmail.com>
 * @class ControlPanel controlpanel.h
 * @brief Qt Designer form class with all settings needed for WebCamCap external application.
 * @date 2015
 */

class ControlPanel : public QMainWindow
{
    Q_OBJECT

    MyFifo *m_fifo = nullptr;
    QTimer *m_reconnectTimer = nullptr;

    bool m_sendPositions = false;
    bool m_sendMovement = false;
    int m_nextMovementCaptureTime;
    float m_sensitivity, minSize, minAcceleration, minSpeed;

    float difTemp;

    QMap<size_t, Marker> points, lastPoints;
    QMap<size_t, Marker> diff, lastDiff, acceleration;

    //move
    QTime *m_movementTimer = nullptr;
    float m_minSizeOfMovement = 0.0f;
    QVector3D m_startMovementPoint;
    QVector3D m_endMovementPoint;

public:

    /**
     * @brief ControlPanel constructs ControlPanel object
     * @param parent Default QWidget parameter
     */
    explicit ControlPanel(QWidget *parent = 0);

    ~ControlPanel();

public slots:

    /**
     * @brief Used to handle points in actual frame
     * @param pts Points from MyFifo
     */
    void handleFrame(Frame frame);

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
     * @brief Movement signal is emited every time when some point make Movement that fits parameters in ControlPanel gui (size, speed, ...).
     * This signal is emitted only if checkbox "Movements" is checked and Timeout(ms) period elapsed.
     * @param move Movement
     * @param index index of point which made Movement
     */
    void movement(Movement move, size_t index);

    /**
     * @brief This signal transfer Points in last captured frame.
     * This signal is emitted only if checkbox "Positions" is checked.
     * @param pts Points
     */
    void pointsReady(QVector<Marker> pts);

private:
    Ui::ControlPanel *ui;

    void makeDiff(QVector<Marker> pts);

    void handlePositions();
    void handleMovements();
};

#endif // CONTROLPANEL_H
