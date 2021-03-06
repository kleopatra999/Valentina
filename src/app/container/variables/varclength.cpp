/************************************************************************
 **
 **  @file   varclength.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 7, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "varclength.h"
#include "../geometry/vabstractcurve.h"
#include "../core/vapplication.h"

//---------------------------------------------------------------------------------------------------------------------
VArcLength::VArcLength()
    :VCurveLength()
{
    SetType(VarType::ArcLength);
}

//---------------------------------------------------------------------------------------------------------------------
VArcLength::VArcLength(const quint32 &id, const quint32 &parentId, const VAbstractCurve *arc)
    :VCurveLength(id, parentId, arc)
{
    SetType(VarType::ArcLength);
}

//---------------------------------------------------------------------------------------------------------------------
VArcLength::VArcLength(const VArcLength &var)
    :VCurveLength(var)
{}

//---------------------------------------------------------------------------------------------------------------------
VArcLength &VArcLength::operator=(const VArcLength &var)
{
    if ( &var == this )
    {
        return *this;
    }
    VCurveLength::operator=(var);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VArcLength::~VArcLength()
{}
