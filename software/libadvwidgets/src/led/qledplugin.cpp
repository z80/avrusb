/***************************************************************************
 *   Copyright (C) 2008 by P. Sereno                                       *
 *   http://www.sereno-online.com                                          *
 *   http://www.qt4lab.org                                                 *
 *   http://www.qphoton.org                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "qled.h"
#include "qledplugin.h"

#include <QtPlugin>

QLedPlugin::QLedPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QLedPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool QLedPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QLedPlugin::createWidget(QWidget *parent)
{
    return new QLed(parent);
}

QString QLedPlugin::name() const
{
    return "QLed";
}

QString QLedPlugin::group() const
{
    return "Lab Widgets";
}

QIcon QLedPlugin::icon() const
{
    return QIcon(":qled.png");
}

QString QLedPlugin::toolTip() const
{
    return "";
}

QString QLedPlugin::whatsThis() const
{
    return "";
}

bool QLedPlugin::isContainer() const
{
    return false;
}

QString QLedPlugin::domXml() const
{
    return "<widget class=\"QLed\" name=\"qLed\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>100</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string>Binary Led</string>\n"
           " </property>\n"
           " <property name=\"value\" >\n"
           " <bool>false</bool>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>Led widget</string>\n"
           " </property>\n"
 		   " <property name=\"color\" >\n"
           " <enum>QLed::Red</enum>\n"
           " </property>\n"                     
           "</widget>\n";
}

QString QLedPlugin::includeFile() const
{
    return "qled.h";
}

Q_EXPORT_PLUGIN2(customwidgetplugin, QLedPlugin)
