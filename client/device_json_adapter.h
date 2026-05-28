#ifndef DEVICE_JSON_ADAPTER_H
#define DEVICE_JSON_ADAPTER_H

#include "idevice_adapter.h"

class DeviceJsonAdapter : public IDeviceAdapter
{
public:
    Device        toDevice(const QJsonObject &json) const override;
    QList<Device> toDeviceList(const QJsonArray &json) const override;
    QList<Device> toDeviceListFromRoot(const QJsonObject &root) const override;
};

#endif // DEVICE_JSON_ADAPTER_H