/************************************************************************
 **
 **  @file   vtoolcutsplinepath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#include "vtoolcutsplinepath.h"
#include "../../container/calculator.h"
#include "../../dialogs/tools/dialogcutsplinepath.h"
#include "../../geometry/vpointf.h"
#include "../../visualization/vistoolcutsplinepath.h"
#include "vabstractspline.h"

#include "../../geometry/vsplinepath.h"

const QString VToolCutSplinePath::ToolType       = QStringLiteral("cutSplinePath");
const QString VToolCutSplinePath::AttrSplinePath = QStringLiteral("splinePath");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VToolCutSplinePath constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param formula string with formula length first splinePath.
 * @param splinePathId id splinePath (we cut this splinePath) in data container.
 * @param splPath1id id first splinePath after cutting.
 * @param splPath2id id second splinePath after cutting.
 * @param typeCreation way we create this tool.
 * @param parent parent object.
 */
VToolCutSplinePath::VToolCutSplinePath(VPattern *doc, VContainer *data, const quint32 &id,
                                       const QString &formula, const quint32 &splinePathId,
                                       const quint32 &splPath1id, const quint32 &splPath2id, const QString &color,
                                       const Source &typeCreation, QGraphicsItem *parent)
    :VToolCut(doc, data, id, formula, splinePathId, splPath1id, splPath2id, color, parent)
{
    RefreshCurve(firstCurve, curve1id, SimpleCurvePoint::ForthPoint);
    RefreshCurve(secondCurve, curve2id, SimpleCurvePoint::FirstPoint);

    if (typeCreation == Source::FromGui)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setDialog set dialog when user want change tool option.
 */
void VToolCutSplinePath::setDialog()
{
    SCASSERT(dialog != nullptr);
    DialogCutSplinePath *dialogTool = qobject_cast<DialogCutSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QSharedPointer<VPointF> point = VAbstractTool::data.GeometricObject<VPointF>(id);
    dialogTool->SetFormula(formula);
    dialogTool->setSplinePathId(curveCutId);
    dialogTool->SetPointName(point->name());
    dialogTool->SetColor(lineColor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool form GUI.
 * @param dialog dialog.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 */
VToolCutSplinePath* VToolCutSplinePath::Create(DialogTool *dialog, VMainGraphicsScene *scene, VPattern *doc,
                                               VContainer *data)
{
    SCASSERT(dialog != nullptr);
    DialogCutSplinePath *dialogTool = qobject_cast<DialogCutSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    const QString pointName = dialogTool->getPointName();
    QString formula = dialogTool->GetFormula();
    const quint32 splinePathId = dialogTool->getSplinePathId();
    const QString color = dialogTool->GetColor();
    VToolCutSplinePath* point = nullptr;
    point = Create(0, pointName, formula, splinePathId, 5, 10, color, scene, doc, data, Document::FullParse,
                   Source::FromGui);
    if (point != nullptr)
    {
        point->dialog=dialogTool;
    }
    return point;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Create help create tool.
 * @param _id tool id, 0 if tool doesn't exist yet.
 * @param pointName point name.
 * @param formula string with formula length first splinePath.
 * @param splinePathId id of splinePath in data container.
 * @param mx label bias x axis.
 * @param my label bias y axis.
 * @param scene pointer to scene.
 * @param doc dom document container.
 * @param data container with variables.
 * @param parse parser file mode.
 * @param typeCreation way we create this tool.
 */
VToolCutSplinePath* VToolCutSplinePath::Create(const quint32 _id, const QString &pointName, QString &formula,
                                               const quint32 &splinePathId, const qreal &mx, const qreal &my,
                                               const QString &color, VMainGraphicsScene *scene, VPattern *doc,
                                               VContainer *data, const Document &parse, const Source &typeCreation)
{
    const QSharedPointer<VSplinePath> splPath = data->GeometricObject<VSplinePath>(splinePathId);
    SCASSERT(splPath != nullptr);

    const qreal result = CheckFormula(_id, formula, data);

    quint32 id = _id;
    QPointF spl1p2, spl1p3, spl2p2, spl2p3;
    qint32 p1 = 0, p2 = 0;

    const QPointF point = splPath->CutSplinePath(qApp->toPixel(result), p1, p2, spl1p2, spl1p3, spl2p2, spl2p3);
    VPointF *p = new VPointF(point, pointName, mx, my);
    if (typeCreation == Source::FromGui)
    {
        id = data->AddGObject(p);
    }
    else
    {
        data->UpdateGObject(id, p);
    }

    quint32 splPath1id = id + 1;
    quint32 splPath2id = id + 2;

    VSplinePoint splP1 = splPath->at(p1);
    VSplinePoint splP2 = splPath->at(p2);
    const VSpline spl1 = VSpline(splP1.P(), spl1p2, spl1p3, *p, splPath->GetKCurve());
    const VSpline spl2 = VSpline(*p, spl2p2, spl2p3, splP2.P(), splPath->GetKCurve());

    VSplinePath *splPath1 = new VSplinePath();
    VSplinePath *splPath2 = new VSplinePath();

    for (qint32 i = 0; i < splPath->CountPoint(); i++)
    {
        if (i <= p1 && i < p2)
        {
            if (i == p1)
            {
                splPath1->append(VSplinePoint(splP1.P(), splP1.KAsm1(), spl1.GetAngle1()+180, spl1.GetKasm1(),
                                              spl1.GetAngle1()));
                VSplinePoint cutPoint;
                cutPoint = VSplinePoint(*p, spl1.GetKasm2(), spl1.GetAngle2(), spl2.GetKasm1(), spl1.GetAngle2()+180);
                splPath1->append(cutPoint);
                continue;
            }
            splPath1->append(splPath->at(i));
        }
        else
        {
            if (i == p2)
            {
                const VSplinePoint cutPoint = VSplinePoint(*p, spl1.GetKasm2(), spl2.GetAngle1()+180,
                                                           spl2.GetKasm1(), spl2.GetAngle1());
                splPath2->append(cutPoint);
                splPath2->append(VSplinePoint(splP2.P(), spl2.GetKasm2(), spl2.GetAngle2(), splP2.KAsm2(),
                                              spl2.GetAngle2()+180));
                continue;
            }
            splPath2->append(splPath->at(i));
        }
    }
    splPath1->SetKCurve(splPath->GetKCurve());
    splPath2->SetKCurve(splPath->GetKCurve());

    splPath1->setMaxCountPoints(splPath->CountPoint());
    splPath2->setMaxCountPoints(splPath->CountPoint());

    if (typeCreation == Source::FromGui)
    {
        splPath1id = data->AddGObject(splPath1);
        data->AddCurveLength<VSplineLength>(splPath1id, id);

        splPath2id = data->AddGObject(splPath2);
        data->AddCurveLength<VSplineLength>(splPath2id, id);
    }
    else
    {
        data->UpdateGObject(splPath1id, splPath1);
        data->AddCurveLength<VSplineLength>(splPath1id, id);

        data->UpdateGObject(splPath2id, splPath2);
        data->AddCurveLength<VSplineLength>(splPath2id, id);

        if (parse != Document::FullParse)
        {
            doc->UpdateToolData(id, data);
        }
    }

    VDrawTool::AddRecord(id, Tool::CutSplinePath, doc);
    if (parse == Document::FullParse)
    {
        VToolCutSplinePath *point = new VToolCutSplinePath(doc, data, id, formula, splinePathId, splPath1id,
                                                           splPath2id, color, typeCreation);
        scene->addItem(point);
        connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
        connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolCutSplinePath::SetFactor);
        connect(scene, &VMainGraphicsScene::DisableItem, point, &VToolCutSplinePath::Disable);
        doc->AddTool(id, point);
        doc->AddTool(splPath1id, point);
        doc->AddTool(splPath2id, point);
        doc->IncrementReferens(splinePathId);
        return point;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSplinePath::ShowVisualization(bool show)
{
    if (show)
    {
        if (vis == nullptr)
        {
            VisToolCutSplinePath *visual = new VisToolCutSplinePath(getData());
            VMainGraphicsScene *scene = qApp->getCurrentScene();
            connect(scene, &VMainGraphicsScene::NewFactor, visual, &Visualization::SetFactor);
            scene->addItem(visual);

            visual->setPoint1Id(curveCutId);
            visual->setLength(qApp->FormulaToUser(formula));
            visual->RefreshGeometry();
            vis = visual;
        }
        else
        {
            VisToolCutSplinePath *visual = qobject_cast<VisToolCutSplinePath *>(vis);
            if (visual != nullptr)
            {
                visual->show();
            }
        }
    }
    else
    {
        delete vis;
        vis = nullptr;
    }
    if (VAbstractSpline *parentCurve = qobject_cast<VAbstractSpline *>(doc->getTool(curveCutId)))
    {
        parentCurve->ShowFoot(show);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief FullUpdateFromFile update tool data form file.
 */
void VToolCutSplinePath::FullUpdateFromFile()
{
    ReadAttributes();
    RefreshGeometry();

    if (vis != nullptr)
    {
        VisToolCutSplinePath *visual = qobject_cast<VisToolCutSplinePath *>(vis);
        visual->setPoint1Id(curveCutId);
        visual->setLength(qApp->FormulaToUser(formula));
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CurveChoosed send signal about selection splinePath.
 * @param id object id in container.
 */
void VToolCutSplinePath::CurveChoosed(quint32 id)
{
    emit ChoosedTool(id, SceneObject::SplinePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowContextMenu show context menu.
 * @param event context menu event.
 */
void VToolCutSplinePath::ShowContextMenu(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutSplinePath>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief contextMenuEvent handle context menu events.
 * @param event context menu event.
 */
void VToolCutSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu<DialogCutSplinePath>(this, event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveDialog save options into file after change in dialog.
 */
void VToolCutSplinePath::SaveDialog(QDomElement &domElement)
{
    SCASSERT(dialog != nullptr);
    DialogCutSplinePath *dialogTool = qobject_cast<DialogCutSplinePath*>(dialog);
    SCASSERT(dialogTool != nullptr);
    doc->SetAttribute(domElement, AttrName, dialogTool->getPointName());
    doc->SetAttribute(domElement, AttrLength, dialogTool->GetFormula());
    doc->SetAttribute(domElement, AttrSplinePath, QString().setNum(dialogTool->getSplinePathId()));
    doc->SetAttribute(domElement, AttrColor, dialogTool->GetColor());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RefreshCurve refresh curve on scene.
 * @param curve curve.
 * @param curveId curve id.
 */
void VToolCutSplinePath::RefreshCurve(VSimpleCurve *curve, quint32 curveId, SimpleCurvePoint curvePosition,
                                      PathDirection direction)
{
    const QSharedPointer<VSplinePath> splPath = VAbstractTool::data.GeometricObject<VSplinePath>(curveId);
    QPainterPath path;
    path.addPath(splPath->GetPath(direction));
    path.setFillRule( Qt::WindingFill );
    if (curvePosition == SimpleCurvePoint::FirstPoint)
    {
        VSpline spl = splPath->GetSpline(1);
        path.translate(-spl.GetP1().toQPointF().x(), -spl.GetP1().toQPointF().y());
    }
    else
    {
        VSpline spl = splPath->GetSpline(splPath->Count());
        path.translate(-spl.GetP4().toQPointF().x(), -spl.GetP4().toQPointF().y());
    }
    curve->SetCurrentColor(QColor(lineColor));
    curve->setPath(path);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSplinePath::SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)
{
    QSharedPointer<VPointF> point = qSharedPointerDynamicCast<VPointF>(obj);
    SCASSERT(point.isNull() == false);

    doc->SetAttribute(tag, VDomDocument::AttrId, id);
    doc->SetAttribute(tag, AttrType, ToolType);
    doc->SetAttribute(tag, AttrName, point->name());
    doc->SetAttribute(tag, AttrMx, qApp->fromPixel(point->mx()));
    doc->SetAttribute(tag, AttrMy, qApp->fromPixel(point->my()));
    doc->SetAttribute(tag, AttrColor, lineColor);

    doc->SetAttribute(tag, AttrLength, formula);
    doc->SetAttribute(tag, AttrSplinePath, curveCutId);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCutSplinePath::ReadToolAttributes(const QDomElement &domElement)
{
    formula = doc->GetParametrString(domElement, AttrLength, "");
    curveCutId = doc->GetParametrUInt(domElement, AttrSplinePath, NULL_ID_STR);
    lineColor = doc->GetParametrString(domElement, AttrColor, ColorBlack);
}
