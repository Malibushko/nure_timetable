#pragma once
#include <QObject>
#include <QSettings>
#include <QDebug>
#include "internal/material.h"
namespace SETTINGS_TYPE {
Q_NAMESPACE
/// Type of settings
enum SETTING_TYPE
{
    UNDEFINED = 0,       /// default value
    APP_THEME = 10,      /// app theme color
    APP_ACCENT,          /// app accent color
    APP_PRIMARY,         /// app primary color
    NIGHT_MODE,          /// enable night mode?
    ZAL_COLOR,           /// color of university credit
    LB_COLOR,            /// color of laboratory work
    LC_COLOR,            /// color of lecture color
    PZ_COLOR,            /// color of practice lesson
    ANIMATIONS,          /// enable animations ?
    CACHING,             /// enable caching ?
    LANGUAGE,            /// language list
    AUTOUPDATING,        /// enable timetable autoupdating?
    CLEAR_CACHE_BTN,     /// clear cache button
    RESTORE_DEFAULT_BTN, /// reset settings button
    CHOSEN_LANGUAGE,     /// language chosen by user
    SHOW_TIMER,          /// show timer
};
Q_ENUM_NS( SETTING_TYPE )
} // namespace SETTINGS_TYPE
/// Groups of settings to display
namespace SETTINGS_GROUP {
Q_NAMESPACE
enum SETTING_GROUP
{
    UNDEFINED = 99,    /// default value
    STYLING = 100,     /// Styling (e.g colors)
    TIMETABLE_STYLING, /// Styling of table
    MISCELLANEOUS,     /// Language, caching etc
    GRAPHICS,          /// Something that hit the perfomance (e.g animations)
    BTN_CALLBACKS      /// buttons with specific logic
};

Q_ENUM_NS( SETTING_GROUP )
} // namespace SETTINGS_GROUP
/// Types of controls to display
namespace CONTROL_TYPE {
Q_NAMESPACE
enum CONTROL
{
    BTN = 150, /// Press button
    LIST,      /// List of strings
    SWITCH,    /// Switch (on/off)
    COLOR,     /// Color chooser
    NUMBER     /// Number slider
};
Q_ENUM_NS( CONTROL )
} // namespace CONTROL_TYPE

namespace timetable {
/**
 * @brief The Settings class
 *
 * Class that performs operations on QSettings and
 * updating after changing one GUI
 */
class Settings : public QObject
{
    Q_OBJECT
    mutable QSettings m_settings;

 private:
    /**
     * @brief Set value if it not exist yet, otherwise do nothing
     * @param key Key id
     * @param value Value
     * @param forceDefault force settings of value
     */
    void setIfNotExist( int key, const QVariant& value, bool forceDefault = false )
    {
        if( forceDefault || m_settings.value( QString::number( key ) ) == QVariant{} )
        {
            m_settings.setValue( QString::number( key ), value );
        }
    }

 public:
    /**
     * @brief Default ctor
     * Loads settings from memory
     */
    Settings( QObject* parent = nullptr )
       : QObject{ parent }
    {
        load();
        m_settings.sync();
    }
    /**
     * @brief Get all top level groups of settings
     * @return string list of group names
     */
    Q_INVOKABLE QStringList groups() const
    {
        return m_settings.childGroups();
    }
    /**
     * @brief Get all settings of particular group
     * @param groupName (Enum value)
     * @return QVariantList of settings
     * \note groupName is int because of strange
       Qt behaviour that does no recognize enum even if it was registered in MetaType
     system
     */
    Q_INVOKABLE QVariantList groupKeys( int groupName ) const
    {
        qDebug() << "Settigns location" << m_settings.fileName();
        m_settings.beginGroup( QString::number( groupName ) );
        QVariantList keys;
        keys.reserve( m_settings.allKeys().size() );
        for( const auto& setting : m_settings.allKeys() )
        {
            keys.push_back( QVariant::fromValue(
               QPair< int, QVariant >( setting.toInt(), m_settings.value( setting ) ) ) );
        }
        m_settings.endGroup();
        return keys;
    }
    /**
     * @brief Load settings from memory or create them
     * @param forceDefault force default settings
     */
    Q_INVOKABLE void load( bool forceDefault = false )
    {
        m_settings.beginGroup( QString::number( SETTINGS_GROUP::STYLING ) );
        setIfNotExist(
           SETTINGS_TYPE::APP_THEME, Material::color( Material::Green ), forceDefault );
        setIfNotExist(
           SETTINGS_TYPE::APP_ACCENT, Material::color( Material::Green ), forceDefault );
        setIfNotExist(
           SETTINGS_TYPE::APP_PRIMARY, Material::color( Material::Green ), forceDefault );
        setIfNotExist( SETTINGS_TYPE::NIGHT_MODE, false, forceDefault );
        m_settings.endGroup();

        m_settings.beginGroup( QString::number( SETTINGS_GROUP::TIMETABLE_STYLING ) );
        setIfNotExist( SETTINGS_TYPE::ZAL_COLOR, QColor( "#C2A0B8" ), forceDefault );
        setIfNotExist( SETTINGS_TYPE::LB_COLOR, QColor( "#CDCCFF" ), forceDefault );
        setIfNotExist( SETTINGS_TYPE::LC_COLOR, QColor( "#FEFEEA" ), forceDefault );
        setIfNotExist( SETTINGS_TYPE::PZ_COLOR, QColor( "#DAED9D" ), forceDefault );
        setIfNotExist( SETTINGS_TYPE::SHOW_TIMER, true, forceDefault );
        m_settings.endGroup();

        m_settings.beginGroup( QString::number( SETTINGS_GROUP::GRAPHICS ) );
        setIfNotExist( SETTINGS_TYPE::ANIMATIONS, true, forceDefault );
        setIfNotExist( SETTINGS_TYPE::CACHING, true, forceDefault );
        m_settings.endGroup();

        m_settings.beginGroup( QString::number( SETTINGS_GROUP::MISCELLANEOUS ) );
        setIfNotExist( SETTINGS_TYPE::LANGUAGE, "English;Русский", forceDefault );
        setIfNotExist( SETTINGS_TYPE::AUTOUPDATING, false, forceDefault );
        m_settings.endGroup();

        m_settings.beginGroup( QString::number( SETTINGS_GROUP::BTN_CALLBACKS ) );
        setIfNotExist( SETTINGS_TYPE::CLEAR_CACHE_BTN, "btn", forceDefault );
        setIfNotExist( SETTINGS_TYPE::RESTORE_DEFAULT_BTN, "btn", forceDefault );
        m_settings.endGroup();

        setIfNotExist( SETTINGS_TYPE::CHOSEN_LANGUAGE, "ru", forceDefault );
        m_settings.sync();
    }
    /**
     * @brief Get value of particular setting
     * @param group Settings Group
     * @param key Settings key
     * @param defaultValue Default value to be returned if key do no exist
     */
    Q_INVOKABLE QVariant value( const QString& group, const QString& key,
       const QVariant& defaultValue = QVariant{} ) const
    {
        if( group.size() )
            m_settings.beginGroup( group );

        auto value = m_settings.value( key, defaultValue );

        if( group.size() )
            m_settings.endGroup();
        return value;
    }
    /**
     * @brief Update value
     * @param group Settings group
     * @param name Settings name
     * @param value New value
     */
    Q_INVOKABLE void update( int group, int name, const QVariant& value )
    {
        if( group != -1 )
            m_settings.beginGroup( QString::number( group ) );

        m_settings.setValue( QString::number( name ), value );

        if( group != -1 )
            m_settings.endGroup();
        m_settings.sync();
        emit valueChanged( group, name, value );
    }
    /**
     * @brief Map settings to string
     * @param type Setting type (ENUM)
     * @return String representation of value
     * \note This function implementation was moved to header
       because QtLinguist doesnt update this value otherwise
     */
    Q_INVOKABLE QString stringify( int type ) const
    {
        switch( type )
        {
        case SETTINGS_TYPE::UNDEFINED:
            return tr( "Undefined type" );
        case SETTINGS_GROUP::UNDEFINED:
            return tr( "Undefined group" );
        case SETTINGS_GROUP::STYLING:
            return tr( "Styling" );
        case SETTINGS_TYPE::APP_THEME:
            return tr( "Primary color" );
        case SETTINGS_TYPE::APP_ACCENT:
            return tr( "Accent color" );
        case SETTINGS_TYPE::APP_PRIMARY:
            return tr( "Theme color" );
        case SETTINGS_TYPE::NIGHT_MODE:
            return tr( "Night mode" );
        case SETTINGS_GROUP::TIMETABLE_STYLING:
            return tr( "Timetable styling" );
        case SETTINGS_TYPE::ZAL_COLOR:
            return tr( "Credit" );
        case SETTINGS_TYPE::LB_COLOR:
            return tr( "Laboratory work" );
        case SETTINGS_TYPE::LC_COLOR:
            return tr( "Lecture" );
        case SETTINGS_TYPE::PZ_COLOR:
            return tr( "Practical work" );
        case SETTINGS_GROUP::GRAPHICS:
            return tr( "Perfomance" );
        case SETTINGS_TYPE::ANIMATIONS:
            return tr( "Animations" );
        case SETTINGS_TYPE::CACHING:
            return tr( "Caching" );
        case SETTINGS_GROUP::MISCELLANEOUS:
            return tr( "Miscellaneous" );
        case SETTINGS_TYPE::LANGUAGE:
            return tr( "Language" );
        case SETTINGS_TYPE::AUTOUPDATING:
            return tr( "Autoupdating" );
        case SETTINGS_GROUP::BTN_CALLBACKS:
            return tr( "Restoring" );
        case SETTINGS_TYPE::RESTORE_DEFAULT_BTN:
            return tr( "Clear cache" );
        case SETTINGS_TYPE::CLEAR_CACHE_BTN:
            return tr( "Reset settings" );
        case SETTINGS_TYPE::SHOW_TIMER:
            return tr( "Show timer" );
        default:
            return "";
        };
    }
    /**
     * @brief Map settings to string
     * @param type String containing integer
     * @return String representation of settings
     * \note This is an overloaded function
     */
    Q_INVOKABLE QString stringify( const QString& type ) const
    {
        return stringify( type.toInt() );
    }
 signals:
    /**
     * @brief Signal is emited when settings value changed
     * @param group Settings group
     * @param key Settings key
     * @param value Settings new value
     */
    void valueChanged( int group, int key, const QVariant& value );
};
} // namespace timetable
