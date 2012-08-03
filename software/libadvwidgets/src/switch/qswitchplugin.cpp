

#include "qswitch.h"
#include "qswitchplugin.h"

#include <QtPlugin>

QSwitchPlugin::QSwitchPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QSwitchPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool QSwitchPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QSwitchPlugin::createWidget(QWidget *parent)
{
    return new QSwitch(parent);
}

QString QSwitchPlugin::name() const
{
    return "QSwitch";
}

QString QSwitchPlugin::group() const
{
    return "Lab Widgets";
}

QIcon QSwitchPlugin::icon() const
{
    return QIcon();
}

QString QSwitchPlugin::toolTip() const
{
    return "";
}

QString QSwitchPlugin::whatsThis() const
{
    return "";
}

bool QSwitchPlugin::isContainer() const
{
    return false;
}

QString QSwitchPlugin::domXml() const
{
    return "<widget class=\"QSwitch\" name=\"qSwitch\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>100</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string>Binary Switch</string>\n"
           " </property>\n"
           " <property name=\"value\" >\n"
           " <bool>false</bool>\n"
           " </property>\n"
 		   " <property name=\"type\" >\n"
           " <enum>QSwitch::Vertical</enum>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string>Switch widget</string>\n"
           " </property>\n"
           "</widget>\n";
}

QString QSwitchPlugin::includeFile() const
{
    return "qswitch.h";
}

Q_EXPORT_PLUGIN2(customwidgetplugin, QSwitchPlugin)
