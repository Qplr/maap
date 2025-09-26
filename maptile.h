#ifndef MAPTILE_H
#define MAPTILE_H

#include <QObject>
#include <QLabel>

class MapTile: public QLabel
{
    Q_OBJECT
public:
    void mouseMoveEvent(QMouseEvent *event) override;

    MapTile(QWidget* parent = nullptr);
signals:
    void coords(int x, int y);
};

#endif // MAPTILE_H
