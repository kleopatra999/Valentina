/************************************************************************
 **
 **  @file   dialognormal.h
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

#ifndef DIALOGNORMAL_H
#define DIALOGNORMAL_H

#include "dialogtool.h"

namespace Ui
{
    class DialogNormal;
}

/**
 * @brief The DialogNormal class dialog for ToolNormal. Help create point and edit option.
 */
class DialogNormal : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogNormal create dialog
     * @param data container with data
     * @param parent parent widget
     */
    DialogNormal(const VContainer *data, QWidget *parent = nullptr);
    ~DialogNormal();
    /**
     * @brief getPointName return name of point
     * @return name
     */
    QString          getPointName() const;
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void             setPointName(const QString &value);
    /**
     * @brief getTypeLine return type of line
     * @return type
     */
    QString          getTypeLine() const;
    /**
     * @brief setTypeLine set type of line
     * @param value type
     */
    void             setTypeLine(const QString &value);
    /**
     * @brief getFormula return string of formula
     * @return formula
     */
    QString          getFormula() const;
    /**
     * @brief setFormula set string of formula
     * @param value formula
     */
    void             setFormula(const QString &value);
    /**
     * @brief getAngle return aditional angle of normal
     * @return angle in degree
     */
    qreal            getAngle() const;
    /**
     * @brief setAngle set aditional angle of normal
     * @param value angle in degree
     */
    void             setAngle(const qreal &value);
    /**
     * @brief getFirstPointId return id of first point
     * @return id
     */
    quint32          getFirstPointId() const;
    /**
     * @brief setFirstPointId set id of first point
     * @param value id
     * @param id don't show this id in list
     */
    void             setFirstPointId(const quint32 &value, const quint32 &id);
    /**
     * @brief getSecondPointId return id of second point
     * @return id
     */
    quint32          getSecondPointId() const;
    /**
     * @brief setSecondPointId set id of second point
     * @param value id
     * @param id don't show this id in list
     */
    void             setSecondPointId(const quint32 &value, const quint32 &id);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void     ChoosedObject(quint32 id, const Valentina::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void     DialogAccepted();
private:
    Q_DISABLE_COPY(DialogNormal)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogNormal *ui;
    /**
     * @brief number number of handled objects
     */
    qint32           number;
    /**
     * @brief pointName name of point
     */
    QString          pointName;
    /**
     * @brief typeLine type of line
     */
    QString          typeLine;
    /**
     * @brief formula formula
     */
    QString          formula;
    /**
     * @brief angle aditional angle of normal
     */
    qreal            angle;
    /**
     * @brief firstPointId id first point of line
     */
    quint32          firstPointId;
    /**
     * @brief secondPointId id second point of line
     */
    quint32          secondPointId;
};

inline QString DialogNormal::getPointName() const
{
    return pointName;
}

inline QString DialogNormal::getTypeLine() const
{
    return typeLine;
}

inline QString DialogNormal::getFormula() const
{
    return formula;
}

inline qreal DialogNormal::getAngle() const
{
    return angle;
}

inline quint32 DialogNormal::getFirstPointId() const
{
    return firstPointId;
}

inline quint32 DialogNormal::getSecondPointId() const
{
    return secondPointId;
}

#endif // DIALOGNORMAL_H