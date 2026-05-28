#ifndef CREATEDEVICEDIALOG_H
#define CREATEDEVICEDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include "device.h"

class QLineEdit;
class QDoubleSpinBox;
class QComboBox;
class QCheckBox;
class QDateEdit;

class CreateDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDeviceDialog(QWidget *parent = nullptr);
    explicit CreateDeviceDialog(const Device& device, QWidget *parent = nullptr);  // ← добавить
    QJsonObject deviceData() const;

private:
    QLineEdit       *m_name;
    QDoubleSpinBox  *m_price;
    QCheckBox       *m_isWireless;
    QComboBox       *m_condition;
    QDateEdit       *m_purchaseDate;
};

#endif // CREATEDEVICEDIALOG_H