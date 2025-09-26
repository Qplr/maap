#ifndef COMMON_H
#define COMMON_H

#include <math.h>

#include <qpoint.h>

#define TILE_SIZE 256

class TileDef
{
    double _lon, _lat; // radians
    int _zoom;
    int _x, _y, _n;
public:
    TileDef(){}
    TileDef(double lon_deg, double lat_deg, int zoom)
    {
        // https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#Derivation_of_tile_names
        _zoom = zoom;
        _n = 1 << _zoom;
        // to radians
        _lon = lon_deg / 180.0 * M_PI;
        _lat = lat_deg / 180.0 * M_PI;
        // to mercator projection
        double d_x = _lon;
        double d_y = log(tan(_lat) + 1.0 / cos(_lat));

        // offset and normalize
        d_x = (1 + d_x / M_PI) / 2;
        d_y = (1 - d_y / M_PI) / 2;

        // tile int coords
        d_x = _n * d_x;
        d_y = _n * d_y;
        _x = floor(d_x);
        _y = floor(d_y);
    }
    int zoom()const {return _zoom;}
    int x()const {return _x;}
    int y()const {return _y;}

    QPointF pixelsToDegrees(int x, int y)
    {
        double n = M_PI - 2.0 * M_PI * (_y + float(y) / TILE_SIZE) / (double)(1 << _zoom);
        float retx = (_x + float(x) / TILE_SIZE) / (double)(1 << _zoom) * 360.0 - 180;
        float rety = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
        return QPointF(rety, retx);
    }
};

#endif // COMMON_H
