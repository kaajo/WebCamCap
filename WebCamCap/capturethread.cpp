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
    m_line = q;
    m_camera = cam;
}

void worker::do_Work()
{
  forever
  {
      m_mutex.lock();
      if (!m_running)
      {
          m_mutex.unlock();
          break;
      }

      m_result = m_camera->RecordNextFrame();

      emit ResultReady(m_result);

      if(!m_line->wait(&m_mutex,1000))
      {
         qDebug() << "signal lost in thread: " << this->thread();
      }
      m_mutex.unlock();

      QCoreApplication::processEvents();
  }

  qDebug() << "Stopping capture thread";
}

void worker::StopWork()
{
    QMutexLocker l(&m_mutex);
    m_running = false;
}

void worker::StartWork()
{
    m_running = true;
    do_Work();
}
