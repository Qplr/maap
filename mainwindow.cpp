#include "mainwindow.h"

#include <QGridLayout>

#include <QPushButton>
#include <QNetworkReply>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <QGeoCoordinate>
#include <QSpacerItem>

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

    setWindowTitle(WGS84(pos_deg) + " / " + UTM(pos_deg));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{   
    _lat = new QDoubleSpinBox();
    _lon = new QDoubleSpinBox();
    _zoom = new QSpinBox();

    _lat->setAlignment(Qt::AlignTop| Qt::AlignLeft);
    _lat->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    _lon->setAlignment(Qt::AlignTop| Qt::AlignLeft);
    _lon->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    _zoom->setAlignment(Qt::AlignTop| Qt::AlignLeft);
    _zoom->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    _lat->setSingleStep(0.01);
    _lon->setSingleStep(0.01);

    _lat->setRange(-90, 90);
    _lon->setRange(-180, 180);
    _zoom->setRange(0, 18);

    _lat->setDecimals(6);
    _lon->setDecimals(6);

    _lat->setValue(55.752523);
    _lon->setValue(37.612770);
    _zoom->setValue(18);

    _tile = new MapTile();
    _tile->setFixedHeight(TILE_SIZE);
    _tile->setFixedWidth(TILE_SIZE);
    _tile->setMouseTracking(true);
    connect(_tile, &MapTile::coords, this, &MainWindow::newCoords);

    QGridLayout* inputLayout = new QGridLayout;
    inputLayout->addWidget(new QLabel("Lat: "), 0, 0);
    inputLayout->addWidget(new QLabel("Lon: "), 1, 0);
    inputLayout->addWidget(new QLabel("Zoom: "), 2, 0);
    inputLayout->addWidget(_lat, 0, 1);
    inputLayout->addWidget(_lon, 1, 1);
    inputLayout->addWidget(_zoom, 2, 1);
    inputLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding),3, 3, 3, 3);

    QHBoxLayout* horizontal = new QHBoxLayout;
    horizontal->addWidget(_tile);
    horizontal->addLayout(inputLayout);
    horizontal->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QVBoxLayout* vertical = new QVBoxLayout;
    vertical->addLayout(horizontal);
    vertical->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

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
    widget->setLayout(vertical);
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

QString MainWindow::WGS84(QPointF pos_deg)
{
    const double lon = pos_deg.x(), lat = pos_deg.y();
    QString lonStr = QString::number(abs(lon)) + (lon < 0 ? " W": " E");
    QString latStr = QString::number(abs(lat)) + (lat < 0 ? " S": " N");

    return QString("%1, %2").arg(latStr).arg(lonStr);
}

QString MainWindow::UTM(QPointF pos_deg)
{
    // //correct range to positive-only
    const double lon = pos_deg.x(), lat = pos_deg.y();

    // utm zone is 6 deg across and starts with 1
    const int utmZone = floor((lon + 180) / 6) + 1;

    QGeoCoordinate equatorial, lateral, point;
    point.setLatitude(lat);
    point.setLongitude(lon);

    equatorial.setLongitude(lon);
    equatorial.setLatitude(0);

    lateral.setLongitude((utmZone - 1) * 6 - 180);
    lateral.setLatitude(lat);

    const bool south = lat < 0;
    const double easting = point.distanceTo(lateral);
    const double northing = point.distanceTo(equatorial);
    return QString("%1%2 %3m E %4m N").arg(utmZone).arg(south ? 'S': 'N').arg(int(easting)).arg(int(northing));
}
