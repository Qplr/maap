#include "mainwindow.h"

#include <QGridLayout>

#include <QPushButton>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _lon = new QDoubleSpinBox();
    _lat = new QDoubleSpinBox();
    _zoom = new QDoubleSpinBox();

    _lon->setRange(-180, 180);
    _lat->setRange(-90, 90);
    _zoom->setRange(0, 18);

    _tile = new QLabel();

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(_tile, 0, 0, 1, 3);
    layout->addWidget(_lon, 1, 0);
    layout->addWidget(_lat, 1, 1);
    layout->addWidget(_zoom, 1, 2);

    // test //
    QPushButton * getButton = new QPushButton("get");
    layout->addWidget(getButton, 2, 1);

    QString url = "https://tile.openstreetmap.org/12/2042/1364.png";
    QNetworkAccessManager* man = new QNetworkAccessManager();

    connect(man, &QNetworkAccessManager::finished, this, [this](QNetworkReply * reply)
    {
        QPixmap pm;
        pm.loadFromData(reply->readAll());
        _tile->setPixmap(pm);
    });
    connect(getButton, &QPushButton::clicked, man, [url, man, this]
    {
        QNetworkRequest req(url);
        req.setRawHeader("User-Agent", "MAAP: Tile Viewer");
        man->get(req);
    });
    // test //

    QWidget * widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{

}
