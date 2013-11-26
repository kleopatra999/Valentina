/************************************************************************
 **
 **  @file   version.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef VERSION_H
#define VERSION_H

#include <QtCore/QString>

extern const int MAJOR_VERSION = 0;
extern const int MINOR_VERSION = 2;
extern const int DEBUG_VERSION = 0;

extern const QString APP_VERSION(QStringLiteral("%1.%2.%3").arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(DEBUG_VERSION));
extern const QString WARRANTY("The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE "
                              "WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.");
#endif // VERSION_H