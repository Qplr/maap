#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QLabel* _tile;

    QDoubleSpinBox* _lon, *_lat, *_zoom;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
