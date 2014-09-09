/************************************************************************
 **
 **  @file   vtooloptionspropertybrowser.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 8, 2014
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

#ifndef VTOOLOPTIONSPROPERTYBROWSER_H
#define VTOOLOPTIONSPROPERTYBROWSER_H

#include <QObject>
#include <QMap>

#include "../libs/vpropertyexplorer/vproperty.h"
#include "../libs/vpropertyexplorer/vpropertymodel.h"
#include "../libs/vpropertyexplorer/vpropertyformview.h"

class QDockWidget;
class QGraphicsItem;
class QScrollArea;
class VFormula;

using namespace VPE;

class VToolOptionsPropertyBrowser : public QObject
{
    Q_OBJECT
public:
    VToolOptionsPropertyBrowser(QDockWidget *parent);
public slots:
    void itemClicked(QGraphicsItem *item);
    void userChangedData(VProperty* property);
    void UpdateOptions();
private:
    Q_DISABLE_COPY(VToolOptionsPropertyBrowser)

    VPropertyModel* PropertyModel;
    VPropertyFormView* formView;

    QGraphicsItem *currentItem;
    QMap<VProperty *, QString> propertyToId;
    QMap<QString, VProperty *> idToProperty;

    void AddProperty(VProperty *property, const QString &id);
    void ShowItemOptions(QGraphicsItem *item);

    template<class Tool>
    void SetPointName(const QString &name);

    template<class Tool>
    void AddPropertyPointName(Tool *i, const QString &propertyName);

    template<class Tool>
    void AddPropertyLineType(Tool *i, const QString &propertyName);

    void AddPropertyFormula(const QString &propertyName, const VFormula &formula, const QString &attrName);

    QStringList PropertiesList() const;

    void ChangeDataToolSinglePoint(VProperty *property);
    void ChangeDataToolEndLine(VProperty *property);
    void ChangeDataToolAlongLine(VProperty *property);
    void ChangeDataToolArc(VProperty *property);
    void ChangeDataToolBisector(VProperty *property);
    void ChangeDataToolCutArc(VProperty *property);
    void ChangeDataToolCutSpline(VProperty *property);
    void ChangeDataToolCutSplinePath(VProperty *property);
    void ChangeDataToolHeight(VProperty *property);
    void ChangeDataToolLine(VProperty *property);
    void ChangeDataToolLineIntersect(VProperty *property);
    void ChangeDataToolNormal(VProperty *property);
    void ChangeDataToolPointOfContact(VProperty *property);
    void ChangeDataToolPointOfIntersection(VProperty *property);
    void ChangeDataToolShoulderPoint(VProperty *property);
    void ChangeDataToolSpline(VProperty *property);
    void ChangeDataToolSplinePath(VProperty *property);
    void ChangeDataToolTriangle(VProperty *property);

    void ShowOptionsToolSinglePoint(QGraphicsItem *item);
    void ShowOptionsToolEndLine(QGraphicsItem *item);
    void ShowOptionsToolAlongLine(QGraphicsItem *item);
    void ShowOptionsToolArc(QGraphicsItem *item);
    void ShowOptionsToolBisector(QGraphicsItem *item);
    void ShowOptionsToolCutArc(QGraphicsItem *item);
    void ShowOptionsToolCutSpline(QGraphicsItem *item);
    void ShowOptionsToolCutSplinePath(QGraphicsItem *item);
    void ShowOptionsToolHeight(QGraphicsItem *item);
    void ShowOptionsToolLine(QGraphicsItem *item);
    void ShowOptionsToolLineIntersect(QGraphicsItem *item);
    void ShowOptionsToolNormal(QGraphicsItem *item);
    void ShowOptionsToolPointOfContact(QGraphicsItem *item);
    void ShowOptionsToolPointOfIntersection(QGraphicsItem *item);
    void ShowOptionsToolShoulderPoint(QGraphicsItem *item);
    void ShowOptionsToolSpline(QGraphicsItem *item);
    void ShowOptionsToolSplinePath(QGraphicsItem *item);
    void ShowOptionsToolTriangle(QGraphicsItem *item);

    void UpdateOptionsToolSinglePoint();
    void UpdateOptionsToolEndLine();
    void UpdateOptionsToolAlongLine();
    void UpdateOptionsToolArc();
    void UpdateOptionsToolBisector();
    void UpdateOptionsToolCutArc();
    void UpdateOptionsToolCutSpline();
    void UpdateOptionsToolCutSplinePath();
    void UpdateOptionsToolHeight();
    void UpdateOptionsToolLine();
    void UpdateOptionsToolLineIntersect();
    void UpdateOptionsToolNormal();
    void UpdateOptionsToolPointOfContact();
    void UpdateOptionsToolPointOfIntersection();
    void UpdateOptionsToolShoulderPoint();
    void UpdateOptionsToolSpline();
    void UpdateOptionsToolSplinePath();
    void UpdateOptionsToolTriangle();
};

#endif // VTOOLOPTIONSPROPERTYBROWSER_H