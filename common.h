#ifndef COMMON_H
#define COMMON_H

#include <math.h>
#include <QDebug>

class TileDef
{
    double _lon, _lat; // radians
    int _zoom;
    int _x, _y, _n;
public:
    TileDef(double lon, double lat, int zoom)
    {
        // https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#Derivation_of_tile_names
        _zoom = zoom;
        _n = pow(2, _zoom);
        // to radians
        _lon = lon / 180.0 * M_PI;
        _lat = lat / 180.0 * M_PI;
        qInfo() << lon << lat << _lon << _lat;
        // to mercator projection
        double d_x = _lon;
        //double d_y = pow(sinh(tan(_lat)), -1);
        double d_y = log(tan(_lat) + 1.0 / cos(_lat));
        qInfo() << d_x << d_y;

        // offset and normalize
        d_x = (1 + d_x / M_PI) / 2;
        d_y = (1 - d_y / M_PI) / 2;

        // tile int coords
        _x = floor(_n * d_x);
        _y = floor(_n * d_y);
        qInfo() << _x << _y;

        // _x = _n * (lon + 180) / 360;
        // qDebug() << _x;
        // _y = _n * (1 - (log(tan(_lat) + 1.0 / cos(_lat)) / M_PI)) / 2;
        // qDebug() << _y;
    }
    int zoom()const {return _zoom;}
    int x()const {return _x;}
    int y()const {return _y;}
};

#endif // COMMON_H
