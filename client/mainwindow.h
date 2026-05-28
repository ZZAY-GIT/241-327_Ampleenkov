#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "httpclient.h"
#include "device_json_adapter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onDataReceived(const QByteArray &data);

    void on_GetAll_clicked();

    void on_GetById_clicked();

    void on_create_clicked();

    void on_edit_clicked();

    void on_DeleteByID_clicked();

private:
    Ui::MainWindow *ui;
    HttpClient* m_client;
    DeviceJsonAdapter m_adapter;
    int m_editId = 0;
    bool m_isEditing = false;
};
#endif // MAINWINDOW_H
