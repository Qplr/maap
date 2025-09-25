#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QTimer>

#include "common.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel* _tile;
    QDoubleSpinBox* _lon, *_lat;
    QSpinBox* _zoom;

    QNetworkAccessManager* netManager = nullptr;
    QTimer* _scrollIdleTimer = nullptr;

    void requestTile(const TileDef& tile);
    void requestSelectedTile();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
