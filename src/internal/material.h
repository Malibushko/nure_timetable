#pragma once
#include <QtGui/qcolor.h>
#include <QObject>
/**
 * @brief The Material class
 *
 * Contains Material Design color palette and provides mapping from enum to color values
 * \note Analogue of Material enum in QML
 */
class Material : public QObject
{    
    Q_OBJECT

public:

    enum Theme {
        Light,
        Dark
    };

    enum Color {
        Red,
        Pink,
        Purple,
        DeepPurple,
        Indigo,
        Blue,
        LightBlue,
        Cyan,
        Teal,
        Green,
        LightGreen,
        Lime,
        Yellow,
        Amber,
        Orange,
        DeepOrange,
        Brown,
        Grey,
        BlueGrey
    };

    enum Shade {
        Shade50,
        Shade100,
        Shade200,
        Shade300,
        Shade400,
        Shade500,
        Shade600,
        Shade700,
        Shade800,
        Shade900,
        ShadeA100,
        ShadeA200,
        ShadeA400,
        ShadeA700,
    };

    Q_ENUM(Theme)
    Q_ENUM(Color)
    Q_ENUM(Shade)
    /**
     * @brief Map Color enum to real value
     * @param color enum value to map
     * @param shade shaed to add to the color
     * @return QColor object that represents the color
     */
    static QColor color(Color color, Shade shade = Shade500);
    /**
     * @brief Applies shade for chosen color
     * @param color origin QColor object
     * @param shade Shade to apply
     * @return QColor with applied shade param
     */
    static QColor shade(const QColor &color, Shade shade);

};
