#include "mainwindow.h"

#include <QGridLayout>

#include <QPushButton>
#include <QNetworkReply>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>

void MainWindow::requestCurrentTile()
{
    QString url = "https://tile.openstreetmap.org/%1/%2/%3.png";
    url = url
        .arg(currentTile.zoom())
        .arg(currentTile.x())
              .arg(currentTile.y());
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", "MAAP: Tile Viewer");
    netManager->get(req);
}

void MainWindow::requestSelectedTile()
{
    currentTile = TileDef(_lon->value(), _lat->value(), _zoom->value());
    requestCurrentTile();
}

void MainWindow::newCoords(int x, int y)
{
    QPointF pos_deg = currentTile.pixelsToDegrees(x, y);
    setWindowTitle(QString("%1, %2").arg(pos_deg.x()).arg(pos_deg.y()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _lat = new QDoubleSpinBox();
    _lon = new QDoubleSpinBox();
    _zoom = new QSpinBox();

    _lat->setValue(55.769469);
    _lon->setValue(37.580724);
    _zoom->setValue(13);

    _lat->setSingleStep(0.01);
    _lon->setSingleStep(0.01);

    _lat->setRange(-90, 90);
    _lon->setRange(-180, 180);
    _zoom->setRange(0, 18);

    _lat->setDecimals(5);
    _lon->setDecimals(5);

    _tile = new MapTile();
    _tile->setFixedHeight(TILE_SIZE);
    _tile->setFixedWidth(TILE_SIZE);
    _tile->setMouseTracking(true);
    connect(_tile, &MapTile::coords, this, &MainWindow::newCoords);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(_tile, 0, 0, 1, 3);
    layout->addWidget(_lat, 1, 0);
    layout->addWidget(_lon, 1, 1);
    layout->addWidget(_zoom, 1, 2);

    netManager = new QNetworkAccessManager();

    connect(netManager, &QNetworkAccessManager::finished, this, [this](QNetworkReply * reply)
    {
        if(reply->error() != QNetworkReply::NetworkError::NoError)
        {
            QMessageBox::critical(this, "Error", "Requesting a tile failed");
            return;
        }
        QPixmap pm;
        pm.loadFromData(reply->readAll());
        _tile->setPixmap(pm);
    });

    _scrollIdleTimer = new QTimer();
    _scrollIdleTimer->setInterval(100);
    _scrollIdleTimer->setSingleShot(true);
    connect(_scrollIdleTimer, &QTimer::timeout, this, &MainWindow::requestSelectedTile);

    QWidget * widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);

    // test //
    auto waitForScroll = [this](){_scrollIdleTimer->start();};
    connect(_lat, &QDoubleSpinBox::valueChanged, this, waitForScroll);
    connect(_lon, &QDoubleSpinBox::valueChanged, this, waitForScroll);
    connect(_zoom, &QSpinBox::valueChanged, this, waitForScroll);
    requestSelectedTile();
}

MainWindow::~MainWindow()
{

}
