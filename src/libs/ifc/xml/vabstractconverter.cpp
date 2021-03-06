/************************************************************************
 **
 **  @file   vabstractconverter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 12, 2014
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

#include "vabstractconverter.h"
#include "exception/vexception.h"
#include "exception/vexceptionwrongid.h"

#include <QFile>

//---------------------------------------------------------------------------------------------------------------------
VAbstractConverter::VAbstractConverter(const QString &fileName)
    :VDomDocument(), ver(0x0), fileName(fileName)
{
    this->setXMLContent(fileName);
    const QString version = GetVersionStr();
    ver = GetVersion(version);
}

//---------------------------------------------------------------------------------------------------------------------
VAbstractConverter::~VAbstractConverter()
{}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::Convert()
{
    if (ver == MaxVer())
    {
        return;
    }

    QString error;
    const QString backupFileName = fileName +".backup";
    if (SafeCopy(fileName, backupFileName, error) == false)
    {
        const QString errorMsg(tr("Error creation backup file: %1.").arg(error));
        throw VException(errorMsg);
    }

    ApplyPatches();

    QFile file(backupFileName);
    file.remove();
}

//---------------------------------------------------------------------------------------------------------------------
QString VAbstractConverter::GetVersionStr() const
{
    const QDomNodeList nodeList = this->elementsByTagName(TagVersion);
    if (nodeList.isEmpty())
    {
        const QString errorMsg(tr("Couldn't get version information."));
        throw VException(errorMsg);
    }

    if (nodeList.count() > 1)
    {
        const QString errorMsg(tr("Too many tags <%1> in file.").arg(TagVersion));
        throw VException(errorMsg);
    }

    const QDomNode domNode = nodeList.at(0);
    if (domNode.isNull() == false && domNode.isElement())
    {
        const QDomElement domElement = domNode.toElement();
        if (domElement.isNull() == false)
        {
            return domElement.text();
        }
    }
    return QString(QStringLiteral("0.0.0"));
}

//---------------------------------------------------------------------------------------------------------------------
int VAbstractConverter::GetVersion(const QString &version) const
{
    ValidateVersion(version);

    QStringList ver = version.split(".");

    bool ok = false;
    int major = ver.at(0).toInt(&ok);
    if (ok == false)
    {
        return 0x0;
    }

    ok = false;
    int minor = ver.at(1).toInt(&ok);
    if (ok == false)
    {
        return 0x0;
    }

    ok = false;
    int patch = ver.at(2).toInt(&ok);
    if (ok == false)
    {
        return 0x0;
    }

    return (major<<16)|(minor<<8)|(patch);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::ValidateVersion(const QString &version) const
{
    QRegExp rx(QStringLiteral("^(0|([1-9][0-9]*)).(0|([1-9][0-9]*)).(0|([1-9][0-9]*))$"));

    if (rx.exactMatch(version) == false)
    {
        const QString errorMsg(tr("Version \"%1\" invalid.").arg(version));
        throw VException(errorMsg);
    }

    if (version == QLatin1String("0.0.0"))
    {
        const QString errorMsg(tr("Version \"0.0.0\" invalid."));
        throw VException(errorMsg);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::CheckVersion(int ver) const
{
    if (ver < MinVer())
    {
        const QString errorMsg(tr("Invalid version. Minimum supported version is %1").arg(MinVerStr()));
        throw VException(errorMsg);
    }

    if (ver > MaxVer())
    {
        const QString errorMsg(tr("Invalid version. Maximum supported version is %1").arg(MaxVerStr()));
        throw VException(errorMsg);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractConverter::SaveDocument(const QString &fileName, QString &error) const
{
    try
    {
        TestUniqueId();
    }
    catch (const VExceptionWrongId &e)
    {
        error = tr("Error no unique id.");
        return false;
    }

    return VDomDocument::SaveDocument(fileName, error);
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::Save() const
{
    QString error;
    if (SaveDocument(fileName, error) == false)
    {
        VException e(error);
        throw e;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractConverter::SetVersion(const QString &version)
{
    if (setTagText(TagVersion, version) == false)
    {
        VException e(tr("Could not change version."));
        throw e;
    }
}
