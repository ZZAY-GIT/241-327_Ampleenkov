#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "httpclient.h"
#include "createdevicedialog.h"

#include <QInputDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Получаем синглтон — не создаём новый объект
    m_client = HttpClient::getInstance();

    // Подключаем сигнал dataReceived к отображению данных в textEdit
    QObject::connect(
        m_client, &HttpClient::dataReceived,
            this, &MainWindow::onDataReceived);
    }


void MainWindow::onDataReceived(const QByteArray& data)
{

    if (data.isEmpty()) {
        ui->textEdit->append("Готово.");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        ui->textEdit->append("Готово.");
        return;
    }

    // Режим редактирования — открываем диалог с заполненными полями
    if (m_isEditing && doc.isObject()) {
        m_isEditing = false;
        Device d = m_adapter.toDevice(doc.object());

        CreateDeviceDialog dialog(d, this);  // ← передаём устройство
        if (dialog.exec() == QDialog::Accepted) {
            QJsonObject json = dialog.deviceData();
            json["id"] = m_editId;
            QJsonDocument updDoc(json);
            QByteArray updData = updDoc.toJson(QJsonDocument::Compact);
            qDebug() << "Обновляем устройство id:" << m_editId << updData;
            m_client->update(m_editId, updData);
        }
        return;
    }

    ui->textEdit->clear();

    if (doc.isArray()) {
        // GET /devices → массив напрямую
        QList<Device> devices = m_adapter.toDeviceList(doc.array());
        for (const Device& d : devices) {
            ui->textEdit->append(
                QString("ID: %1 | %2 | %3 ₽ | %4")
                    .arg(d.id)
                    .arg(d.name)
                    .arg(d.price, 0, 'f', 2)
                    .arg(d.condition));
        }
    } else if (doc.isObject()) {
        // Если сервер вернёт {"devices": [...]}
        QJsonObject root = doc.object();
        if (root.contains("devices")) {
            QList<Device> devices = m_adapter.toDeviceListFromRoot(root);
            for (const Device& d : devices) {
                ui->textEdit->append(
                    QString("ID: %1 | %2 | %3 ₽ | %4")
                        .arg(d.id)
                        .arg(d.name)
                        .arg(d.price, 0, 'f', 2)
                        .arg(d.condition));
            }
        } else {
            // Одиночный девайс (GET /devices/{id})
            Device d = m_adapter.toDevice(root);
            ui->textEdit->append(
                QString("ID: %1 | %2 | %3 ₽ | %4")
                    .arg(d.id)
                    .arg(d.name)
                    .arg(d.price, 0, 'f', 2)
                    .arg(d.condition));
        }
    } else {
        // Ответ не JSON (например, 201 Created без тела)
        ui->textEdit->append(QString::fromUtf8(data));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_GetAll_clicked()
{
    qDebug() << "[MainWindow] Запрос списка устройств...";
    m_client->list();
}


void MainWindow::on_GetById_clicked()
{
    bool ok;
    int id = QInputDialog::getInt(this, "Get Device by ID", "ID девайса:", 1, 1, 99999, 1, &ok);
    qDebug() << "[MainWindow] Запрос устройства с id:" << id;
    m_client->get(id);
}


void MainWindow::on_create_clicked()
{
    CreateDeviceDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QJsonObject json = dialog.deviceData();
        QJsonDocument doc(json);
        QByteArray data = doc.toJson(QJsonDocument::Compact);

        qDebug() << "Создаём устройство:" << data;

        HttpClient::getInstance()->create(data);
    }
}

void MainWindow::on_edit_clicked()
{
    bool ok;
    int id = QInputDialog::getInt(this, "Edit Device", "ID девайса:", 1, 1, 99999, 1, &ok);
    if (!ok) return;

    m_editId = id;
    m_isEditing = true;
    m_client->get(id);

    CreateDeviceDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QJsonObject json = dialog.deviceData();
        QJsonDocument doc(json);
        QByteArray data = doc.toJson(QJsonDocument::Compact);

        qDebug() << "Обновляем устройство id:" << id << data;
        m_client->update(id, data);
    }
}

void MainWindow::on_DeleteByID_clicked()
{
    bool ok;
    int id = QInputDialog::getInt(this, "Delete Device", "ID девайса:", 1, 1, 99999, 1, &ok);
    if (!ok) return;

    qDebug() << "[MainWindow] Удаляем устройство с id:" << id;
    m_client->remove(id);
}

