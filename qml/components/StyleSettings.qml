import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Material 2.12
import lib 1.0

Item {
    id: root_
    property bool isPortrait: root.width >= root.height
    property var appTheme:  (mainSettings.value(SETTINGS_GROUP.STYLING,SETTINGS_TYPE.NIGHT_MODE) === "true"?
                                 Material.Dark : Material.Light)

    property bool isLight: (appTheme === Material.Light)
    property int screenWidth: root.width
    property int screenHeight: root.height
    property double margin: Math.max((isPortrait ? screenWidth : screenHeight) * 0.05,5)
    property double padding: (isPortrait ? screenWidth : screenHeight) * 0.05
    property double rowHeight: {
            return (!isPortrait? screenHeight : screenWidth) * 0.1;
    }
    property double iconSize: Math.max(rowHeight/4,25)
    property double rowWidth: screenWidth
    property double iconScaleDefault: 1;
    property double iconScalePressed: 1.2;

    property color darkLight: Qt.darker("white",1.2);

    property color iconColor: (isLight ? "white" : darkLight)
    property color iconRowButtonColor: (isLight ? Material.color(Material.Grey) : darkLight)
    property color themeColor: (isLight? mainSettings.value(SETTINGS_GROUP.STYLING,SETTINGS_TYPE.APP_THEME)
                                       : Material.backgroundColor)
    property color accentColor: Qt.darker(mainSettings.value(SETTINGS_GROUP.STYLING,SETTINGS_TYPE.APP_ACCENT),
                                          isLight ? 1.5 : 1)
    property color primaryColor: (isLight? mainSettings.value(SETTINGS_GROUP.STYLING,SETTINGS_TYPE.APP_PRIMARY)
                                         : Material.backgroundColor)
    property color utilityColor: Material.color(Material.Grey)
    property color componentColor: (isLight ? "white" : Material.backgroundColor)
    property color textColor: (isLight ?  "black" : darkLight)

    property bool animationsEnabled: mainSettings.value(SETTINGS_GROUP.GRAPHICS,SETTINGS_TYPE.ANIMATIONS)
    property bool cachingEnabled: mainSettings.value(SETTINGS_GROUP.GRAPHICS,SETTINGS_TYPE.ANIMATIONS)
    property bool showTimer: mainSettings.value(SETTINGS_GROUP.TIMETABLE_STYLING,SETTINGS_TYPE.SHOW_TIMER)

    property color pz_color: mainSettings.value(SETTINGS_GROUP.TIMETABLE_STYLING,SETTINGS_TYPE.PZ_COLOR);
    property color lc_color: mainSettings.value(SETTINGS_GROUP.TIMETABLE_STYLING,SETTINGS_TYPE.LC_COLOR);
    property color lb_color: mainSettings.value(SETTINGS_GROUP.TIMETABLE_STYLING,SETTINGS_TYPE.LB_COLOR);
    property color zal_color: mainSettings.value(SETTINGS_GROUP.TIMETABLE_STYLING,SETTINGS_TYPE.ZAL_COLOR);

    Connections {
        target: mainSettings
        function onValueChanged(group,key,value) {
            switch (group) {
            case SETTINGS_GROUP.STYLING:
                switch (key) {
                case SETTINGS_TYPE.APP_THEME:
                    themeColor = value;
                    break;
                case SETTINGS_TYPE.APP_PRIMARY:
                    primaryColor = value;
                    break;
                case SETTINGS_TYPE.APP_ACCENT:
                    accentColor = value;
                    break;
                case SETTINGS_TYPE.NIGHT_MODE:
                    appTheme = (value === true ? Material.Dark : Material.Light);
                    break;
                }
                break;
            case SETTINGS_GROUP.GRAPHICS:
                switch (key) {
                case SETTINGS_TYPE.ANIMATIONS:
                    animationsEnabled = value;
                    break;
                case SETTINGS_TYPE.CACHING:
                    cachingEnabled = value;
                    break;
                }
                break;
            case SETTINGS_GROUP.TIMETABLE_STYLING:
                switch (key) {
                case SETTINGS_TYPE.PZ_COLOR:
                    pz_color = value;
                    break;
                case SETTINGS_TYPE.LB_COLOR:
                    lb_color = value;
                    break;
                case SETTINGS_TYPE.LC_COLOR:
                    lc_color = value;
                    break;
                case SETTINGS_TYPE.ZAL_COLOR:
                    zal_color = value;
                    break;
                case SETTINGS_TYPE.SHOW_TIMER:
                    showTimer = value;
                    break;
                }
                break;
            }
        }
    }
}
