#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkapi.h"
#include <QMessageBox>
#include <QDebug>
#include <Qcryptographichash>
#include <QStackedLayout>
#include <QWidget>

#include "projectmanagerwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initWindow();
    void refresh();

private:
    Ui::MainWindow *ui;
    NetworkApi *loginAccess;

    QWidget *widget;
    ProjectManagerWindow *projectManagerWindow;
    QStackedLayout *mainLayout;

    void connects();

signals:
    void signalsLoginPushButtonCliked();
private slots:
    void slotsClickLoginPushButton();
    void slotsLoginRequestFinished(QJsonObject *reply);
    void slotsLoginRequestError(QString errorInfo);
};

#endif // MAINWINDOW_H
