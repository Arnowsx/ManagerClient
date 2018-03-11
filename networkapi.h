#ifndef NETWORKAPI_H
#define NETWORKAPI_H

#define LOGIN_URL "http://47.94.192.188/com_sys/actions/user_login.action"
#define LOGOUT_URL "http://47.94.192.188/com_sys/actions/user_signOut.action"
#define GET_DEVICE_URL "http://47.94.192.188/com_sys/actions/device_getByIds.action"
#define GET_DEVICE_K_URL "http://47.94.192.188/com_sys/actions/device_getByProjectAndName.action"
#define ADD_DEVICE_URL "http://47.94.192.188/com_sys/actions/device_add.action"
#define DELETE_DEVICE_URL "http://47.94.192.188/com_sys/actions/device_deleteByIds.action"
#define UPDATE_DEVICE_URL "http://47.94.192.188/com_sys/actions/device_updateByIds.action"
#define ADD_REPORT_URL "http://47.94.192.188/com_sys/actions/reportRecord_add.action"
#define GET_REPORT_URL "http://47.94.192.188/com_sys/actions/reportView_getByDeviceId.action"
#define ADD_ALARM_URL "http://47.94.192.188/com_sys/actions/alarmRecord_add.action"
#define GET_ALARM_URL "http://47.94.192.188/com_sys/actions/alarmView_getByDeviceId.action"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include "globle.h"

class NetworkApi : public QObject
{
    Q_OBJECT
public:

    static NetworkApi* getInstance();

    void loginRequest(QJsonObject info);
    void logoutRequest();
    void getDeviceRequest();
    void addDeviceRequest(QJsonObject info);
    void deleteDeviceRequest(QJsonObject info);
    void updateDeviceRequest(QJsonObject info);
    void getKRequest(QString deviceId);
    void addReportRequest(QJsonObject info);
    void getReportRequest(QString deviceId);
    void addAlarmRequest(QJsonObject info);
    void getAlarmRequest(QString deviceId);
//    void getReportRecordRequest(QJsonObject info);
private:
    QNetworkAccessManager *networkManager;
    static NetworkApi* instance;

    explicit NetworkApi(QObject *parent = nullptr);
    NetworkApi(const NetworkApi&);
    NetworkApi& operator =(const NetworkApi&);
    ~NetworkApi();

    QByteArray getCookie();
signals:
    void signalLoginRequestFinished(QJsonObject *reply);
    void signalLoginRequestError(QString errorInfo);
    void signalGetDeviceRequestFinished(QJsonObject *reply);
    void signalGetDeviceRequestError(QString errorInfo);
    void signalLogoutRequestSuccess();
    void signalLogoutRequestError(QString errorInfo);
    void signalAddDeviceRequestFinished();
    void signalAddDeviceReuqestError(QString errorInfo);
    void signalDeleteDeviceReuqestFinished();
    void signalDeleteDeviceReuqestError(QString errorInfo);
    void signalUpdateDeviceRequestFinished();
    void signalUpdateDeviceRequestError(QString errorInfo);
    void signalGetKFinished(int k);
    void signalAddReportRequestFinished();
    void signalAddReportRequestError(QString errorInfo);
    void signalGetReportRequestFinished();
    void signalGetReportRequestError(QString errorInfo);
    void signalAddAlarmRequestFinished();
    void signalAddAlarmRequestError(QString errorInfo);
    void signalGetAlarmRequestFinished();
    void signalGetAlarmRequestError(QString errorInfo);
private slots:
    void slotRequestFinished(QNetworkReply *reply);
};

#endif // NETWORKAPI_H
