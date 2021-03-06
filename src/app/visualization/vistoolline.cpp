/************************************************************************
 **
 **  @file   vgraphicslineitem.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2014
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

#include "vistoolline.h"
#include "../container/vcontainer.h"
#include "../geometry/vpointf.h"
#include "../tools/drawTools/vdrawtool.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolLine::VisToolLine(const VContainer *data, QGraphicsItem *parent)
    :VisLine(data, parent), point2Id(NULL_ID)
{
    this->mainColor = Qt::red;
}

//---------------------------------------------------------------------------------------------------------------------
VisToolLine::~VisToolLine()
{}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLine::RefreshGeometry()
{
    QLineF line;
    const QSharedPointer<VPointF> first = Visualization::data->GeometricObject<VPointF>(point1Id);
    if (point2Id == NULL_ID)
    {
        line = QLineF(first->toQPointF(), Visualization::scenePos);
    }
    else
    {
        const QSharedPointer<VPointF> second = Visualization::data->GeometricObject<VPointF>(point2Id);
        line = QLineF(first->toQPointF(), second->toQPointF());
    }
    DrawLine(this, line, mainColor, lineStyle);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLine::setPoint2Id(const quint32 &value)
{
    point2Id = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLine::DrawLine(QGraphicsLineItem *lineItem, const QLineF &line, const QColor &color, Qt::PenStyle style)
{
    SCASSERT (lineItem != nullptr);

    lineItem->setPen(QPen(color, qApp->toPixel(qApp->widthMainLine())/factor, style));
    lineItem->setLine(line);
}
