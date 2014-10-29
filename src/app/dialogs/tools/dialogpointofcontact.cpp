/************************************************************************
 **
 **  @file   dialogpointofcontact.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "dialogpointofcontact.h"

#include "../../geometry/vpointf.h"
#include "../../container/vcontainer.h"
#include "../../visualization/vistoolpointofcontact.h"
#include "../../widgets/vmaingraphicsscene.h"
#include "../../xml/vpattern.h"

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DialogPointOfContact create dialog
 * @param data container with data
 * @param parent parent widget
 */
DialogPointOfContact::DialogPointOfContact(const VContainer *data, const quint32 &toolId, QWidget *parent)
    :DialogTool(data, toolId, parent), ui(new Ui::DialogPointOfContact), number(0),
      radius(QString()), center(NULL_ID), firstPoint(NULL_ID), secondPoint(NULL_ID), formulaBaseHeight(0), line(nullptr)
{
    ui->setupUi(this);
    InitVariables(ui);
    InitFormulaUI(ui);
    ui->lineEditNamePoint->setText(qApp->getCurrentDocument()->GenerateLabel(LabelType::NewLabel));
    labelEditNamePoint = ui->labelEditNamePoint;
    this->formulaBaseHeight = ui->plainTextEditFormula->height();

    InitOkCancelApply(ui);
    flagFormula = false;
    CheckState();

    FillComboBoxPoints(ui->comboBoxFirstPoint);
    FillComboBoxPoints(ui->comboBoxSecondPoint);
    FillComboBoxPoints(ui->comboBoxCenter);

    connect(ui->toolButtonPutHere, &QPushButton::clicked, this, &DialogPointOfContact::PutHere);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &DialogPointOfContact::PutVal);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &DialogPointOfContact::ValChenged);

    if (qApp->patternType() == MeasurementsType::Standard)
    {
        SizeHeight();
        connect(ui->radioButtonSizeGrowth, &QRadioButton::clicked, this, &DialogTool::SizeHeight);
    }
    else
    {
        radioButtonSizeGrowth->setVisible(false);
        Measurements();
    }
    connect(ui->radioButtonStandardTable, &QRadioButton::clicked, this, &DialogPointOfContact::Measurements);
    connect(ui->radioButtonIncrements, &QRadioButton::clicked, this, &DialogPointOfContact::Increments);
    connect(ui->radioButtonLengthLine, &QRadioButton::clicked, this, &DialogPointOfContact::LengthLines);
    connect(ui->radioButtonLengthArc, &QRadioButton::clicked, this, &DialogPointOfContact::LengthArcs);
    connect(ui->radioButtonLengthSpline, &QRadioButton::clicked, this, &DialogPointOfContact::LengthCurves);
    connect(ui->toolButtonEqual, &QPushButton::clicked, this, &DialogPointOfContact::EvalFormula);
    connect(ui->lineEditNamePoint, &QLineEdit::textChanged, this, &DialogPointOfContact::NamePointChanged);
    connect(ui->plainTextEditFormula, &QPlainTextEdit::textChanged, this, &DialogPointOfContact::FormulaTextChanged);
    connect(ui->pushButtonGrowLength, &QPushButton::clicked, this, &DialogPointOfContact::DeployFormulaTextEdit);
    connect(ui->comboBoxFirstPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfContact::PointNameChanged);
    connect(ui->comboBoxSecondPoint, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfContact::PointNameChanged);
    connect(ui->comboBoxCenter, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &DialogPointOfContact::PointNameChanged);

    line = new VisToolPointOfContact(data);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPointOfContact::~DialogPointOfContact()
{
    delete line;
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::FormulaTextChanged()
{
    this->FormulaChangedPlainText();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::PointNameChanged()
{
    QSet<quint32> set;
    set.insert(getCurrentObjectId(ui->comboBoxFirstPoint));
    set.insert(getCurrentObjectId(ui->comboBoxSecondPoint));
    set.insert(getCurrentObjectId(ui->comboBoxCenter));

    QColor color = okColor;
    if (set.size() != 3)
    {
        flagError = false;
        color = errorColor;
    }
    else
    {
        flagError = true;
        color = okColor;
    }
    ChangeColor(ui->labelFirstPoint, color);
    ChangeColor(ui->labelSecondPoint, color);
    ChangeColor(ui->labelArcCenter, color);
    CheckState();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::ShowVisualization()
{
    if (prepare == false)
    {
        VMainGraphicsScene *scene = qApp->getCurrentScene();
        connect(scene, &VMainGraphicsScene::NewFactor, line, &VisLine::SetFactor);
        scene->addItem(line);
        line->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::DeployFormulaTextEdit()
{
    DeployFormula(ui->plainTextEditFormula, ui->pushButtonGrowLength, formulaBaseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
 * @param id id of point or detail
 * @param type type of object
 */
void DialogPointOfContact::ChosenObject(quint32 id, const SceneObject &type)
{
    if (prepare == false)// After first choose we ignore all objects
    {
        if (type == SceneObject::Point)
        {
            switch (number)
            {
                case 0:
                    if (SetObject(id, ui->comboBoxFirstPoint, tr("Select second point of line")))
                    {
                        number++;
                        line->VisualMode(id);
                    }
                    break;
                case 1:
                    if (SetObject(id, ui->comboBoxSecondPoint, tr("Select point of center of arc")))
                    {
                        number++;
                        line->setLineP2Id(id);
                        line->RefreshGeometry();
                    }
                    break;
                case 2:
                    if (SetObject(id, ui->comboBoxCenter, ""))
                    {
                        line->setRadiusId(id);
                        line->RefreshGeometry();
                        prepare = true;
                        this->setModal(true);
                        this->show();
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPointOfContact::SaveData()
{
    pointName = ui->lineEditNamePoint->text();
    radius = ui->plainTextEditFormula->toPlainText();
    radius.replace("\n", " ");
    center = getCurrentObjectId(ui->comboBoxCenter);
    firstPoint = getCurrentObjectId(ui->comboBoxFirstPoint);
    secondPoint = getCurrentObjectId(ui->comboBoxSecondPoint);

    line->setPoint1Id(firstPoint);
    line->setLineP2Id(secondPoint);
    line->setRadiusId(center);
    line->setRadius(radius);
    line->RefreshGeometry();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setSecondPoint set id second point
 * @param value id
 */
void DialogPointOfContact::setSecondPoint(const quint32 &value)
{
    setPointId(ui->comboBoxSecondPoint, secondPoint, value);
    line->setLineP2Id(secondPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setFirstPoint set id first point
 * @param value id
 */
void DialogPointOfContact::setFirstPoint(const quint32 &value)
{
    setPointId(ui->comboBoxFirstPoint, firstPoint, value);
    line->setPoint1Id(firstPoint);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetCenter set id of center point
 * @param value id
 */
void DialogPointOfContact::setCenter(const quint32 &value)
{
    setPointId(ui->comboBoxCenter, center, value);
    line->setRadiusId(center);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setRadius set formula radius of arc
 * @param value formula
 */
void DialogPointOfContact::setRadius(const QString &value)
{
    radius = value;
    // increase height if needed.
    if (radius.length() > 80)
    {
        this->DeployFormulaTextEdit();
    }
    ui->plainTextEditFormula->setPlainText(radius);
    line->setRadius(radius);
    MoveCursorToEnd(ui->plainTextEditFormula);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setPointName set name of point
 * @param value name
 */
void DialogPointOfContact::setPointName(const QString &value)
{
    pointName = value;
    ui->lineEditNamePoint->setText(pointName);
}
