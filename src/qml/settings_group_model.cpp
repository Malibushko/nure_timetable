#include "qml/settings_group_model.h"
#include "src/settings.h"
namespace timetable {

SettingsGroupModel::SettingsGroupModel( QObject* )
{
}

void SettingsGroupModel::setItems( const QVariantList& otherSettings )
{
    beginResetModel();
    m_settingTitles.clear();
    m_settingTitles.reserve( otherSettings.size() );
    for( const QVariant& item : otherSettings )
    {
        m_settingTitles.push_back( qvariant_cast< QPair< int, QVariant > >( item ) );
    }
    endResetModel();
}

int SettingsGroupModel::columnCount( const QModelIndex& ) const
{
    return 1;
}

int SettingsGroupModel::rowCount( const QModelIndex& ) const
{
    return m_settingTitles.size();
}

QVariant SettingsGroupModel::data( const QModelIndex& index, int role ) const
{
    if( index.row() < 0 || index.row() >= m_settingTitles.size() )
    {
        return {};
    }
    const auto& name = m_settingTitles[index.row()];
    switch( role )
    {
    case SettingsRole::NAME:
        return name.first;
    case SettingsRole::VALUE:
        if( name.second.canConvert< QString >()
           && name.second.toString().contains( ';' ) )
            return name.second.toString();
        if( name.second.canConvert< bool >() )
            return name.second.toString() == "true";
        return name.second;
    case SettingsRole::TYPE:
    {
        if( name.second.toString() == "btn" )
        {
            return CONTROL_TYPE::CONTROL::BTN;
        }
        else if( name.second.canConvert< QString >()
           && name.second.toString().contains( ';' ) )
        {
            return CONTROL_TYPE::CONTROL::LIST;
        }
        else if( ( name.second.toString() == "true"
                    || name.second.toString() == "false" ) )
        {
            return CONTROL_TYPE::CONTROL::SWITCH;
        }
        else if( name.second.canConvert< int >() )
        {
            return CONTROL_TYPE::CONTROL::NUMBER;
        }
        else if( name.second.canConvert< QColor >() )
        {
            return CONTROL_TYPE::CONTROL::COLOR;
        }
        else
            return "none";
    }
    default:
        break;
    }
    return {};
}
QHash< int, QByteArray > SettingsGroupModel::roleNames() const
{
    QHash< int, QByteArray > roles{ { SettingsRole::NAME, "name" },
       { SettingsRole::VALUE, "value" }, { SettingsRole::TYPE, "type" } };

    return roles;
}
} // namespace timetable
