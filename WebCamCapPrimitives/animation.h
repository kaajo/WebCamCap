/*
 *
 * Copyright (C) 2015  Miroslav Krajicek (https://github.com/kaajo).
 * All Rights Reserved.
 *
 * This file is part of WebCamCap.
 *
 * WebCamCap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WebCamCap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU LGPL version 3
 * along with WebCamCap. If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include "webcamcapprimitives_global.h"

#include "frame.h"

#include <QVector3D>
#include <QString>
#include <QVector>
#include <QObject>
#include <QVariantMap>

/**
 * @author Miroslav Krajicek
 * @brief The Animation class stores captured frames from scene for edit/export.
 */
class WEBCAMCAPPRIMITIVESSHARED_EXPORT Animation : public QObject
{
    Q_OBJECT

    QString m_name;
    long int m_elapsedMilliSecondsTime = 0;
    QVector<Frame> m_frames;
    QVector3D m_roomDimensions;

public:
    /**
     * @brief Animation default constructor
     * @param roomDims dimensions are captured normalized so before exporting,
     * they need to be multiplied again by room dimensions
     * @param name
     * @param parent
     */
    explicit Animation(QVector3D roomDims, QString name = "name", QObject *parent = 0);

    /**
     * @brief This function computes AVERAGE FPS of animation.
     * It is not guaranteed that all frames will be captured in same or almost same time.
     * @return average FPS of animation
     */
    float fps();

    /**
     * @brief
     * @return elapsed time in seconds
     */
    float length();

    /**
     * @brief
     * @return elapsed time in milliseconds
     */
    float lengthMS();

    /**
     * @brief Exports #Animation into specified folder.
     * @param folder
     * @return true if exporting was successful
     */
    bool save(QString folder);

    QVariantMap toVariantMap();
    void fromVariantMap(QVariantMap map);

    QString name() const;
    void setName(const QString &name);

    QVector3D roomDimensions() const;

public slots:
    void addFrame(Frame frame);

private:

};

#endif // ANIMATION_H
