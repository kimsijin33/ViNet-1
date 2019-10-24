#include "canimatedlabel.h"

void CAnimatedLabel::setColor (QColor color)
{
    setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
}

QColor CAnimatedLabel::color()
{
    return Qt::black; // getter is not really needed for now
}
