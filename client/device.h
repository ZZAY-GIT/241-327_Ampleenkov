#ifndef DEVICE_H
#define DEVICE_H

#include <QDate>

struct Device
{
    int         id = 0;
    QString     name;
    double      price = 0.0;
    bool        isWireless = false;
    QString     condition;
    QDate       purchaseDate;

    bool isValid() const { return id > 0 && !name.isEmpty(); }
};

#endif // DEVICE_H