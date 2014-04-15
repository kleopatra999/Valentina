/************************************************************************
 **
 **  @file   dialogline.cpp
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

#include "dialogline.h"
#include "ui_dialogline.h"

#include <QPushButton>

DialogLine::DialogLine(const VContainer *data, QWidget *parent)
    :DialogTool(data, parent), ui(new Ui::DialogLine), number(0), firstPoint(0), secondPoint(0), typeLine(QString())
{
    ui->setupUi(this);
    InitOkCansel(ui);

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxTypeLine(ui->comboBoxLineType);

    number = 0;
}

DialogLine::~DialogLine()
{
    delete ui;
}

void DialogLine::setSecondPoint(const quint32 &value)
{
    secondPoint = value;
    const VPointF *point = data->GeometricObject<const VPointF *>(value);
    qint32 index = ui->comboBoxSecondPoint->findText(point->name());
    if (index != -1)
    {
        ui->comboBoxSecondPoint->setCurrentIndex(index);
    }
}

void DialogLine::setTypeLine(const QString &value)
{
    typeLine = value;
    SetupTypeLine(ui->comboBoxLineType, value);
}

void DialogLine::setFirstPoint(const quint32 &value)
{
    firstPoint = value;
    const VPointF *point = data->GeometricObject<const VPointF *>(value);
    qint32 index = ui->comboBoxFirstPoint->findText(point->name());
    if (index != -1)
    {
        ui->comboBoxFirstPoint->setCurrentIndex(index);
    }
}


void DialogLine::DialogAccepted()
{
    qint32 index = ui->comboBoxFirstPoint->currentIndex();
    firstPoint = qvariant_cast<quint32>(ui->comboBoxFirstPoint->itemData(index));
    index = ui->comboBoxSecondPoint->currentIndex();
    secondPoint = qvariant_cast<quint32>(ui->comboBoxSecondPoint->itemData(index));
    typeLine = GetTypeLine(ui->comboBoxLineType);
    DialogClosed(QDialog::Accepted);
}

void DialogLine::ChoosedObject(quint32 id, const Valentina::Scenes &type)
{
    if (type == Valentina::Point)
    {
        const VPointF *point = data->GeometricObject<const VPointF *>(id);
        if (number == 0)
        {
            qint32 index = ui->comboBoxFirstPoint->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxFirstPoint->setCurrentIndex(index);
                number++;
                emit ToolTip(tr("Select second point"));
                return;
            }
        }
        if (number == 1)
        {
            qint32 index = ui->comboBoxSecondPoint->findText(point->name());
            if ( index != -1 )
            { // -1 for not found
                ui->comboBoxSecondPoint->setCurrentIndex(index);
                number = 0;
                emit ToolTip("");
            }
            if (isInitialized == false)
            {
                this->show();
            }
        }
    }
}