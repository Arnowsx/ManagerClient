#include "networkapi.h"

NetworkApi* NetworkApi::instance = new NetworkApi();
NetworkApi* NetworkApi::getInstance(){
    return instance;
}

NetworkApi::NetworkApi(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager();

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotRequestFinished(QNetworkReply*)));

}

NetworkApi::~NetworkApi(){

}

NetworkApi::NetworkApi(const NetworkApi&){

}

NetworkApi& NetworkApi::operator =(const NetworkApi&){

}


void NetworkApi::slotRequestFinished(QNetworkReply *reply){
    if(reply->url() == QUrl(QString(LOGIN_URL))){
        QString cookie = QString(reply->rawHeader("Set-Cookie"));
        qDebug()<<cookie;
        QRegExp rx("JSESSIONID=([a-zA-Z0-9]+)");
        cookie.indexOf(rx);
        Globle::cookieNumber = rx.cap(0);
        qDebug()<<rx.cap(0);
        QByteArray bytes = reply->readAll();
        qDebug()<<bytes;
        QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201){
            emit signalLoginRequestFinished(&replyJson);
        }
        else{
            QString error = replyJson.value("error").toString();
            emit signalLoginRequestError(error);
        }
    }
    else if(reply->url() == QUrl(GET_DEVICE_URL)){
        QByteArray bytes = reply->readAll();
        qDebug() << bytes;
        QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
        QJsonArray resultList = replyJson.value("resultList").toArray();
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200){
            qDebug() << "code : 200";
            if(resultList.at(0).toObject().size() == 1){
                int k =resultList.at(0).toObject().value("k").toInt();
                emit signalGetKFinished(k);
            }
            else{
                emit signalGetDeviceRequestFinished(&replyJson);
            }
        }
        else{
            QString error = replyJson.value("error").toString();
            qDebug() << error;
            emit signalGetDeviceRequestError(error);
        }
    }
    else if(reply->url() == QUrl(LOGOUT_URL)){
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201){
            emit signalLogoutRequestSuccess();
        }
        else {
            QByteArray bytes = reply->readAll();
            QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
            QString error = replyJson.value("error").toString();
            emit signalGetDeviceRequestError(error);
        }
    }
    else if(reply->url() == QUrl(ADD_DEVICE_URL)){
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201){
            emit signalAddDeviceRequestFinished();
        }
        else{
            QByteArray bytes = reply->readAll();
            QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
            QString error = replyJson.value("error").toString();
            emit signalAddDeviceReuqestError(error);
        }
    }
    else if(reply->url() == QUrl(DELETE_DEVICE_URL)){
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 204){
            emit signalDeleteDeviceReuqestFinished();
        }
        else{
            QByteArray bytes = reply->readAll();
            qDebug()<< bytes;
            QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
            QString error = replyJson.value("error").toString();
            emit signalDeleteDeviceReuqestError(error);
        }
    }
    else if(reply->url() == QUrl(UPDATE_DEVICE_URL)){
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201){
            emit signalUpdateDeviceRequestFinished();
        }else {
            QByteArray bytes = reply->readAll();
            QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
            QString error = replyJson.value("error").toString();
            emit signalDeleteDeviceReuqestError(error);
        }
    }
    else if(reply->url() == QUrl(ADD_REPORT_URL)){
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201){
            emit signalAddReportRequestFinished();
        }else{
            QByteArray bytes = reply->readAll();
            QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
            QString error = replyJson.value("error").toString();
            emit signalAddReportRequestError(error);
        }
    }
    else if(reply->url() == QUrl(GET_REPORT_URL)){
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200){
            emit signalGetReportRequestFinished();
        }else {
            QByteArray bytes = reply->readAll();
            QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
            QString error = replyJson.value("error").toString();
            emit signalGetReportRequestError(error);
        }
    }
    else if(reply->url() == QUrl(ADD_ALARM_URL)){
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 201){
            emit signalAddAlarmRequestFinished();
        }else{
            QByteArray bytes = reply->readAll();
            QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
            QString error = replyJson.value("error").toString();
            emit signalAddReportRequestError(error);
        }
    }
    else if(reply->url() == QUrl(GET_ALARM_URL)){
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200){
            emit signalGetAlarmRequestFinished();
        }else {
            QByteArray bytes = reply->readAll();
            QJsonObject replyJson = QJsonDocument::fromJson(bytes).object();
            QString error = replyJson.value("error").toString();
            emit signalGetReportRequestError(error);
        }
    }
}
void NetworkApi::loginRequest(QJsonObject info){
    QByteArray array;
    array.append("userName="+info.value("userName").toString().toUtf8().toPercentEncoding());
    array.append("&password="+info.value("password").toString().toUtf8().toPercentEncoding());

    QNetworkRequest networkRequest;
    networkRequest.setUrl(QUrl(LOGIN_URL));
    networkManager->post(networkRequest, array);
}

void NetworkApi::getDeviceRequest(){
    QByteArray array;
    array.append("project="+Globle::projectId);

    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie", getCookie());
    networkRequest.setUrl(QUrl(GET_DEVICE_URL));
    networkManager->post(networkRequest, array);
}

void NetworkApi::logoutRequest(){
    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie", getCookie());
    networkRequest.setUrl(QUrl(LOGOUT_URL));
    networkManager->post(networkRequest, "");
}

void NetworkApi::addDeviceRequest(QJsonObject info){
    QByteArray array;
    array.append("deviceId="+info.value("deviceId").toString().toUtf8().toPercentEncoding());
    array.append("&name="+info.value("name").toString().toUtf8().toPercentEncoding());
    array.append("&pipeDiameter="+info.value("pipeDiameter").toString().toUtf8().toPercentEncoding());
    array.append("&pipeDescribe="+info.value("pipeDescribe").toString().toUtf8().toPercentEncoding());
    array.append("&address="+info.value("address").toString().toUtf8().toPercentEncoding());
    array.append("&number="+info.value("number").toString().toUtf8().toPercentEncoding());
    array.append("&remarks="+info.value("remarks").toString().toUtf8().toPercentEncoding());
    array.append("&k="+info.value("k").toString().toUtf8().toPercentEncoding());
    array.append("&cDeviceState=1");
    array.append("&project="+QString::number(Globle::projectId));

    qDebug()<<array;
    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie",getCookie());
    networkRequest.setUrl(QUrl(ADD_DEVICE_URL));
    networkManager->post(networkRequest, array);
}

void NetworkApi::deleteDeviceRequest(QJsonObject info){
    QByteArray array;
    array.append("ids="+info.value("ids").toString().toUtf8().toPercentEncoding());
    qDebug() << array;

    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie", getCookie());
    networkRequest.setUrl(QUrl(DELETE_DEVICE_URL));

    networkManager->post(networkRequest, array);
}

void NetworkApi::updateDeviceRequest(QJsonObject info){
    QByteArray array;
    array.append("ids="+info.value("ids").toString().toUtf8().toPercentEncoding());
    array.append("&keys="+info.value("keys").toString().toUtf8().toPercentEncoding());
    QStringList keys = info.value("keys").toString().split("+");
    for(int i = 0; i < keys.size(); i++){
        array.append("&"+keys.at(i)+"="+info.value(keys.at(i)).toString().toUtf8().toPercentEncoding());
    }

    qDebug() << array;
    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie", getCookie());
    networkRequest.setUrl(QUrl(UPDATE_DEVICE_URL));

    networkManager->post(networkRequest, array);
}

//void NetworkApi::getReportRecordRequest(QJsonObject info){

//}

void NetworkApi::getKRequest(QString deviceId){
    QByteArray array;
    array.append("keys=k");
    array.append("&ids="+deviceId);
    qDebug()<<array;

    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie", getCookie());
    networkRequest.setUrl(QUrl(GET_DEVICE_URL));

    qDebug()<<"getting K!!!";
    networkManager->post(networkRequest, array);
}

void NetworkApi::addReportRequest(QJsonObject info)
{
    QByteArray array;
    array.append("date=" + info.value("date").toString().toUtf8().toPercentEncoding());
    array.append("&pressure=" + QString::number(info.value("pressure").toDouble()));
    array.append("&temperature=" + QString::number(info.value("temperature").toDouble()));
    array.append("&averageFlow=" + QString::number(info.value("averageFlow").toDouble()));
    array.append("&totalFlow=" + QString::number(info.value("totalFlow").toDouble()));
    array.append("&judge1=" + info.value("judge1").toString());
    array.append("&judge2=" + info.value("judge1").toString());
    array.append("&device=" + info.value("device").toString());

    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie", getCookie());
    networkRequest.setUrl(QUrl(ADD_REPORT_URL));

    networkManager->post(networkRequest, array);
}

void NetworkApi::getReportRequest(QString deviceId){
    QByteArray array;
    array.append("deviceId=" + deviceId);

    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie", getCookie());
    networkRequest.setUrl(QUrl(GET_REPORT_URL));

    networkManager->post(networkRequest, array);
}

void NetworkApi::addAlarmRequest(QJsonObject info){
    QByteArray array;
    array.append("date=" + info.value("date").toString().toUtf8().toPercentEncoding());
    array.append("&temperatureAlarm=" + info.value("temperatureAlarm").toString().toUtf8().toPercentEncoding());
    array.append("&deviceTrouble=" + info.value("deviceTrouble").toString().toUtf8().toPercentEncoding());
    array.append("&device=" + info.value("device").toString().toUtf8().toPercentEncoding());

    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie", getCookie());
    networkRequest.setUrl(QUrl(ADD_ALARM_URL));

    networkManager->post(networkRequest, array);
}

void NetworkApi::getAlarmRequest(QString deviceId){
    QByteArray array;
    array.append("deviceId=" + deviceId);

    QNetworkRequest networkRequest;
    networkRequest.setRawHeader("Set-Cookie", getCookie());
    networkRequest.setUrl(QUrl(GET_ALARM_URL));

    networkManager->post(networkRequest, array);
}

QByteArray NetworkApi::getCookie(){
    QByteArray cookie;
    cookie.append(Globle::cookieNumber);
    return cookie;
}
