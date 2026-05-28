#ifndef IDEVICE_ADAPTER_H
#define IDEVICE_ADAPTER_H

#include "device.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QList>

class IDeviceAdapter
{
public:
    virtual ~IDeviceAdapter() = default;

    virtual Device        toDevice(const QJsonObject &json) const = 0;
    virtual QList<Device> toDeviceList(const QJsonArray &json) const = 0;

    // Удобный метод для разбора ответа {"devices": [...]}
    virtual QList<Device> toDeviceListFromRoot(const QJsonObject &root) const = 0;
};

#endif // IDEVICE_ADAPTER_H