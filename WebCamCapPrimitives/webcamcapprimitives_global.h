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

#ifndef WEBCAMCAPPRIMITIVES_GLOBAL_H
#define WEBCAMCAPPRIMITIVES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WEBCAMCAPPRIMITIVES_LIBRARY)
#  define WEBCAMCAPPRIMITIVESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define WEBCAMCAPPRIMITIVESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WEBCAMCAPPRIMITIVES_GLOBAL_H
