#include "device_json_adapter.h"
#include <QJsonValue>
#include <QDebug>

Device DeviceJsonAdapter::toDevice(const QJsonObject &json) const
{
    Device d;

    d.id           = json["id"].toInt();
    d.name         = json["name"].toString();
    d.price        = json["price"].toDouble();
    d.isWireless   = json["is_wireless"].toBool();
    d.condition    = json["condition"].toString();

    QString dateStr = json["purchase_date"].toString();
    if (!dateStr.isEmpty()) {
        d.purchaseDate = QDate::fromString(dateStr, "yyyy-MM-dd");
    }

    return d;
}

QList<Device> DeviceJsonAdapter::toDeviceList(const QJsonArray &json) const
{
    QList<Device> list;
    list.reserve(json.size());

    for (const QJsonValue &value : json) {
        if (value.isObject()) {
            list.append(toDevice(value.toObject()));
        }
    }

    return list;
}

QList<Device> DeviceJsonAdapter::toDeviceListFromRoot(const QJsonObject &root) const
{
    QJsonArray devicesArray = root["devices"].toArray();
    return toDeviceList(devicesArray);
}