#include "maptile.h"

#include <QMouseEvent>
#include <QApplication>

void MapTile::mouseMoveEvent(QMouseEvent *event)
{
    if(event->type() == QEvent::MouseMove)
    {
        auto pos = event->position();
        emit coords(pos.x(), pos.y());
    }
    else
        event->ignore();
}

MapTile::MapTile(QWidget* parent): QLabel(parent) {}
