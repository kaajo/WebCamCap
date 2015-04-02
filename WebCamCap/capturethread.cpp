/*
 *
 * Copyright (C) 2014  Miroslav Krajicek, Faculty of Informatics Masaryk University (https://github.com/kaajo).
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

#include "capturethread.h"

using glm::vec2;
using glm::vec3;

worker::worker(QWaitCondition *q, CaptureCamera *cam, QObject *parent) :
  QObject(parent)
{
    running = false;
    line = q;
    this->cam = cam;
}

void worker::do_Work()
{
  forever
  {
      mutex.lock();
      if (!running)
      {
          mutex.unlock();
          break;
      }

      result = cam->RecordNextFrame();

      emit ResultReady(result);

      if(!line->wait(&mutex,1000))
      {
         qDebug() << "signal lost";
      }
      mutex.unlock();

      QCoreApplication::processEvents();
  }

  qDebug() << "Stopping capture thread";
}

void worker::StopWork()
{
    QMutexLocker l(&mutex);
    running = false;
}

void worker::StartWork()
{
    running = true;
    do_Work();
}
