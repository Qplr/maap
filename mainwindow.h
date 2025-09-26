#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QTimer>

#include "common.h"
#include "maptile.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    MapTile* _tile;
    TileDef currentTile;
    QDoubleSpinBox* _lon, *_lat;
    QSpinBox* _zoom;

    QNetworkAccessManager* netManager = nullptr;
    QTimer* _scrollIdleTimer = nullptr;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void requestCurrentTile();
    void requestSelectedTile();
public slots:
    void newCoords(int x, int y);
};
#endif // MAINWINDOW_H
