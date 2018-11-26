#include "bovineapi.h"
#include "QJsonObject"
#include "QJsonArray"
#include "QDebug"


BovineAPI::BovineAPI() :
    heartbeatTimer(nullptr)
{
    client_uid = QUuid::createUuid();
    msgIdConf = 2000;
    msgIdDev = 0;
    devicePathMap = new DeviceTree();
    devicePathMap->foo();
    presetPathMap = new PresetTree();
    uid2PresetMap = new QHash<QString, BovineNode*>;
    mdns = new MDNSLookup();

    // Connect the callback of mdns.
    connect(mdns, SIGNAL(deviceFound(QString, uint16_t)),
        this, SLOT(on_device_found(QString, uint16_t)));
}

BovineAPI::~BovineAPI()
{
    webSocketConf.disconnect();
    webSocketDev.disconnect();

    delete mdns;
    delete devicePathMap;
    delete presetPathMap;
    delete uid2PresetMap;
}


/**
 * @brief BovineAPI::getDeviePresetFolders
 * @return node on which the folders reside or nullptr
 */
BovineNode *BovineAPI::getDevicePresetFolders()
{
    BovineNodeMapping *pme = presetPathMap->find(
                "/presets/devices/hegel/folders/type/value");
    if (pme != nullptr)
        return pme->getNode();

    return nullptr;
}


/**
 * @brief BovineAPI::getDevicePresets
 * @param folderUID ID of the folder to receive the presets from
 * @return A pointer to the folder node itself. The child nodes
 *         are expected to be the presets itself.
 */
BovineNode *BovineAPI::getDevicePresets(const QString &folderUID)
{
    BovineNodeMapping *pme = presetPathMap->find(
        QString("/presets/devices/hegel/folders/%1/presets/type/value").arg(
            folderUID
        )
    );
    if (pme != nullptr)
        return pme->getNode();

    return nullptr;
}


/**
 * @brief BovineAPI::getDevicePreset
 * @param uid ID of the preset to load
 */
BovineNode *BovineAPI::getDevicePreset(const QString &uid)
{

}

/**
 * @brief BovineAPI::loadDevicePreset
 * @param uid ID of the preset to load
 */
void BovineAPI::loadDevicePreset(const QString &uid)
{
    webSocketConf.sendTextMessage(
        QString("post /presets/devices/hegel/load?message_id=%1 "
                "{\"uid\":\"0123456789abcdef0123456789abcdef\","
                "\"preset_uid\":\"%2\"}").arg(
                    QString::number(getMsgIdConf()),
                    uid
        )
    );
}


/**
 * @brief BovineAPI::connectToDevice
 * @details Try to find the device on the network. If this is successful,
 * 			on_device_found should be called.
 */
void BovineAPI::connectToDevice() {
    mdns->lookup("_hw2015._sub._uadevice._tcp");
}

void BovineAPI::on_device_found(QString ip, uint16_t port) {
    QString wsc = QString("ws://%1:%2/").arg(ip, QString::number(port));
    QString wsd = QString("ws://%1:4792/").arg(ip);

    qDebug() << wsc;
    connect(&webSocketConf, SIGNAL(connected()), this, SLOT(onWSConfConnected()));
    connect(&webSocketConf, SIGNAL(disconnected()), this, SLOT(onWSConfClosed()));
    connect(&webSocketDev, SIGNAL(connected()), this, SLOT(onWSDevConnected()));
    connect(&webSocketDev, SIGNAL(disconnected()), this, SLOT(onWSDevClosed()));

    webSocketConf.open(QUrl(wsc));
    webSocketDev.open(QUrl(wsd));
}


/**
 * @brief BovineAPI::onWSConfConnected
 * @details Once the websocket is connected, this function is being called.
 * It will send some initial commands and subscribe to updates. It also starts
 * the heartbeat timer.
 */
void BovineAPI::onWSConfConnected() {
    connect(&webSocketConf, SIGNAL(textMessageReceived(const QString)), this,
            SLOT(onWSConfTextMessageReceived(const QString)));
    emit deviceConnected();

    webSocketConf.sendTextMessage(
        QStringLiteral("get /status?cmd_id=heartbeat&&message_id=0")
    );

    webSocketConf.sendTextMessage(
        QStringLiteral("get /devices?flatvalue=1&commands=0&cmd_id="
                       "get_devices_subtree&propfilter=type%2Cnormalized_value"
                       "%2Creal_value%2Cstring_value%2Cvalue%2Cfolder%2Cuid%2C"
                       "folder_uid%2Cname%2Cplugin_id%2Cpreset_uid%2C"
                       "preset_uid_dirty%2Cfavorite%2Cfactory%2C"
                       "val%2Cbank%2Ccab_knob&recursive=1&message_id=1"
        )
    );
    webSocketConf.sendTextMessage(
        QStringLiteral("subscribe /devices?recursive=1&cmd_id=subscribe_command"
                       "&message_id=2"
        )
    );
    webSocketConf.sendTextMessage(
        QStringLiteral("get /presets?flatvalue=1&commands=0&cmd_id="
                       "get_presets_subtree&propfilter=type%2Cnormalized_value"
                       "%2Creal_value%2Cstring_value%2Cvalue%2Cfolder%2Cuid%2C"
                       "folder_uid%2Cname%2Cplugin_id%2Cpreset_uid%2C"
                       "preset_uid_dirty%2Cfavorite%2Cfactory%2Cval%2Cbank%2C"
                       "cab_knob&levels=7&message_id=3"
        )
    );
    webSocketConf.sendTextMessage(
        QStringLiteral("subscribe /presets?recursive=1&cmd_id=subscribe_command"
                       "&message_id=4"
        )
    );

    webSocketConf.sendTextMessage(
        QStringLiteral("subscribe /meters/meter_values/value?cmd_id="
                       "subscribe_command&message_id=5")
    );

    // Heartbeat timer
    heartbeatTimer = new QTimer(this);
    connect(heartbeatTimer, SIGNAL(timeout()), this, SLOT(send_heartbeat()));
    heartbeatTimer->setInterval(10000);
    heartbeatTimer->start();
}


/**
 * @brief BovineAPI::onWSConfClosed
 * @details If the connection is being closed, stop the heartbeat timer.
 * @todo Fire some disconnect event to the UI.
 */
void BovineAPI::onWSConfClosed() {

    heartbeatTimer->stop();

}


/**
 * @brief BovineAPI::onWSConfTextMessageReceived
 * @param message Message to be processed
 * @details If we receive stuff from the socket, do some initial branching
 * and pass on JSON object to the processing submethods.
 */
void BovineAPI::onWSConfTextMessageReceived(const QString &message) {
    //qDebug() << message;
    //qDebug() << "\n\n\n";
    QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();

    // Initial device list
    if (jsonObject["path"].toString()
          == "/devices/0123456789abcdef0123456789abcdef/preset_uid/value"
          && jsonObject.contains("data")
          && jsonObject["data"].isString()) {
        QString uid = jsonObject["data"].toString();
        qDebug() << "PRESET UID: " << uid;
        /*BovineNode* n = presetPathMap->findByPropValue(uid);
        if (n)
            qDebug() << "PRESET NAME: " << n->getProperty("name")->toString();*/
    }
    else if (jsonObject["path"].toString().startsWith("/devices")) {
        parseDevicePath(jsonObject);
    }
    else if (jsonObject["path"].toString() == "/presets"
             && jsonObject["parameters"].isObject()) {
        parsePresetPath(jsonObject);
    }
    else if (jsonObject["path"].toString() == "/meters/meter_values/value") {
        parseMeters(jsonObject);
    }
    else {
        qDebug() << "UNHANDLED RESPONSE: " << message;
    }

}


/**
 * @brief BovineAPI::parseMeters
 * @param meter_values
 * @details Helper function to update the meters. Emits on_set_level for
 * the UI to update itself.
 */
void BovineAPI::parseMeters(const QJsonObject &meter_values) {
    if (meter_values.contains("data")) {
        QJsonObject data = meter_values["data"].toObject();
        double peakvals[6] = {0, 0, 0, 0, 0, 0};
        if (data.contains("peaks")) {
            QJsonObject peaks = data["levels"].toObject();
            if (peaks.contains("meter_indexes")
                && peaks["meter_indexes"].isArray()
                && peaks.contains("values")
                && peaks["values"].isArray()
            ) {
                QJsonArray lvl_values = peaks["values"].toArray();
                foreach(QJsonValue v, peaks["meter_indexes"].toArray()) {
                    int idx = v.toInt();
                    if (idx >= 0 && idx < 6)
                        peakvals[idx] = lvl_values[idx].toDouble();
                }

            }
        }
        if (data.contains("levels")) {
            QJsonObject levels = data["levels"].toObject();
            if (levels.contains("meter_indexes")
                && levels["meter_indexes"].isArray()
                && levels.contains("values")
                && levels["values"].isArray()
            ) {
                QJsonArray lvl_values = levels["values"].toArray();
                foreach(QJsonValue v, levels["meter_indexes"].toArray()) {
                    int idx = v.toInt();
                    emit updateMeterLevel(v.toInt(),
                                      lvl_values[idx].toDouble(),
                                      peakvals[idx]
                    );
                }

            }
        }
    }
}


/**
 * @brief BovineAPI::onWSDevClosed
 * @todo needs an own heartbeat timer, that needs to be reset and some event
 * to be fired towards the UI.
 */
void BovineAPI::onWSDevClosed() {

}


void BovineAPI::onWSDevConnected() {
    connect(&webSocketDev, SIGNAL(textMessageReceived(const QString)), this,
            SLOT(onWSDevTextMessageReceived(const QString)));

    qDebug() << "Dev WS connected.";

    webSocketDev.sendTextMessage(
        QStringLiteral("get /status?cmd_id=heartbeat&&message_id=0")
    );
    webSocketDev.sendTextMessage(
        QStringLiteral("get /hardware/thermal?flatvalue=1&commands=0&"
                       "cmd_id=cmd_id_get_all_thermal&recursive=1&message_id=1"
        )
    );
    webSocketDev.sendTextMessage(
        QStringLiteral("subscribe /hardware/thermal?recursive=1&cmd_id="
                       "cmd_id_subscribe_to_thermal&message_id=2"
        )
    );
}

void BovineAPI::onWSDevTextMessageReceived(const QString &message) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QString path = jsonObject["path"].toString();

    if (path == "/hardware/thermal") {
        qDebug() << message;
    }
}




PresetTree *BovineAPI::getPresetPathMap() const
{
    return presetPathMap;
}

DeviceTree *BovineAPI::getDevicePathMap() const
{
    return devicePathMap;
}


unsigned int BovineAPI::getMsgIdConf() {
    if (msgIdConf > 16000) msgIdConf = 0;
    return msgIdConf++;
}

unsigned int BovineAPI::getMsgIdDev() {
    if (msgIdDev > 16000) msgIdDev = 0;
    return msgIdDev++;
}

QString BovineAPI::getClientUuid() {
    QString id = client_uid.toString();
    return id.replace("{", "").replace("}", "");
}


/**
 * @brief BovineAPI::addDevicePath
 * @param path path in the device tree
 * @param propertyName the property the value will be stored in
 * @param widget the widget, that is associated with that property
 * @param type the type of the widget
 * @details Use this to associate your widget with properties in your
 * device tree
 */
void BovineAPI::addDevicePath(QString path, QString propertyName,
                              QWidget *widget, WidgetType type)
{
    devicePathMap->add(path, propertyName, widget, type);
}


/**
 * @brief BovineAPI::parseDevicePath
 * @param obj JSON object to be parsed
 * @return true if it worked
 * @details Takes care about everything related to the device tree
 * @todo Remove bool return value
 */
bool BovineAPI::parseDevicePath(QJsonObject &obj)
{
    if (obj.contains("parameters") && obj["parameters"].isObject()) {
        QJsonObject para = obj["parameters"].toObject();
        if (para.contains("cmd_id") && para["cmd_id"].isString()
                && para["cmd_id"] == "get_devices_subtree") {
            devicePathMap->readInitialTree(obj);

            // Now we need some initial status in our frontend.
            QHash<QWidget*, BovineNodeMapping*> wmaps =
                    devicePathMap->getWidget2pathMap();
            foreach(QWidget* w, wmaps.keys()) {
                WidgetContainer* wc = (WidgetContainer*)wmaps[w]->getUserdata();
                emit updateWidget(w, wc->getType(), wmaps[w]->getValue());
            }
        }
    }
    else if (obj["path"].toString().startsWith(
                 "/devices/0123456789abcdef0123456789abcdef")) {
        QString path = obj["path"].toString();
        if(obj.contains("data")) {
            BovineNodeMapping *bme = devicePathMap->find(path);
            if (bme) {
                bme->updateValue(obj["data"].toVariant());
                WidgetContainer* wc = (WidgetContainer*)bme->getUserdata();
                // Tell the UI to update its stuff
                if (wc) {
                    WidgetType wt = wc->getType();
                    emit updateWidget(wc->getWidget(), wt, bme->getValue());
                }
            }
        }
    }
    return true;
}


/**
 * @brief BovineAPI::parsePresetPath
 * @param obj JSON object to be parsed
 * @return true if it worked
 * @details recursively creates the preset tree we receive initially from
 * the device.
 * @todo Remove bool return value
 */
bool BovineAPI::parsePresetPath(QJsonObject &obj)
{
    if (obj.contains("parameters") && obj["parameters"].isObject()) {
        QJsonObject para = obj["parameters"].toObject();
        if (para.contains("cmd_id") && para["cmd_id"].isString()
                && para["cmd_id"] == "get_presets_subtree") {
            presetPathMap->readInitialTree(obj);
        }
    }
    return true;
}


/**
 * @brief BovineAPI::selectComboBox
 * @param cb ComboBox that has been triggered
 * @param index index of the activated element
 * @details This does a reverse lookup of the path and sends the command via
 * websocket.
 */
void BovineAPI::selectComboBox(QComboBox* cb, int index) {
    BovineNodeMapping *pme = devicePathMap->find(cb);
    if (!pme)
        return;

    QString base_path = pme->getPath();

    QString key = cb->itemData(index).toString();
    bool intVal;
    key.toInt(&intVal);

    QString cmd = QString(
        "post %1/set_touch?message_id=%2 {\"client_uid\":\"%3\",\"touch\":true}"
    ).arg(base_path, QString::number(getMsgIdConf()), getClientUuid());
    webSocketConf.sendTextMessage(cmd);

    if (intVal) {
        cmd = QString(
           "set %1/%2/value/?cmd_id=set_command&message_id=%3 %4"
        ).arg(base_path, pme->getPropName(), QString::number(getMsgIdConf()),
              key);
    }
    else {
        cmd = QString(
            "set %1/%2/value/?cmd_id=set_command&message_id=%3 \"%4\""
        ).arg(base_path, pme->getPropName(), QString::number(getMsgIdConf()),
              key);
    }
    webSocketConf.sendTextMessage(cmd);

    cmd = QString(
        "post %1/set_touch?message_id=%2 {\"client_uid\":\"%3\",\"touch\":false}"
    ).arg(base_path, QString::number(getMsgIdConf()), getClientUuid());
    webSocketConf.sendTextMessage(cmd);
}


/**
 * @brief BovineAPI::setPushButton
 * @param pb pushbutton to be used.
 * @param checked status of the button
 *
 * This does a reverse lookup of the path and sends the command via websocket.
 */
void BovineAPI::setPushButton(QPushButton* pb, bool checked) {
    BovineNodeMapping *pme = devicePathMap->find(pb);
    if (!pme)
        return;

    QString base_path = pme->getPath();

    QString cmd = QString(
        "post %1/set_touch?message_id=%2 {\"client_uid\":\"%3\",\"touch\":true}"
    ).arg(base_path, QString::number(getMsgIdConf()), getClientUuid());
    webSocketConf.sendTextMessage(cmd);

    cmd = QString(
        "set %1/%2/value/?cmd_id=set_command&message_id=%3 %4"
    ).arg(base_path, pme->getPropName(), QString::number(getMsgIdConf()),
          (checked ? "1" : "0"));

    webSocketConf.sendTextMessage(cmd);

    cmd = QString(
        "post %1/set_touch?message_id=%2 {\"client_uid\":\"%3\",\"touch\":false}"
    ).arg(base_path, QString::number(getMsgIdConf()), getClientUuid());
    webSocketConf.sendTextMessage(cmd);
}


/**
 * @brief BovineAPI::setSlider
 * @param pb pushbutton to be used.
 * @param checked status of the button
 *
 * This does a reverse lookup of the path and sends the command via websocket.
 */
void BovineAPI::setSlider(QSlider* sl, int value) {
    BovineNodeMapping *pme = devicePathMap->find(sl);
    if (!pme)
        return;

    QString base_path = pme->getPath();
    qDebug() << "BASE PATH SLIDER: " << base_path;

    double fval = value <= 0 ? 0 : value/100.;

    QString cmd = QString(
        "post %1/set_touch?message_id=%2 {\"client_uid\":\"%3\",\"touch\":true}"
    ).arg(base_path, QString::number(getMsgIdConf()), getClientUuid());
    webSocketConf.sendTextMessage(cmd);

    cmd = QString(
        "set %1/%2/value/?cmd_id=set_command&message_id=%3 %4"
    ).arg(base_path, pme->getPropName(), QString::number(getMsgIdConf()),
          QString::number(fval));

    webSocketConf.sendTextMessage(cmd);
    qDebug() << "Sending " << cmd;

    cmd = QString(
        "post %1/set_touch?message_id=%2 {\"client_uid\":\"%3\",\"touch\":false}"
    ).arg(base_path, QString::number(getMsgIdConf()), getClientUuid());
    webSocketConf.sendTextMessage(cmd);
}


/**
 * @brief BovineAPI::setDial
 * @param dial Pointer to dial widget
 * @param value value to be stored and send via websocket
 */
void BovineAPI::setDial(QDial *dial, int value)
{
    BovineNodeMapping *pme = devicePathMap->find(dial);
    if (!pme)
        return;

    QString base_path = pme->getPath();
    qDebug() << "BASE PATH SLIDER: " << base_path;

    double fval = value <= 0 ? 0 : value/100.;

    QString cmd = QString(
        "post %1/set_touch?message_id=%2 {\"client_uid\":\"%3\",\"touch\":true}"
    ).arg(base_path, QString::number(getMsgIdConf()), getClientUuid());
    webSocketConf.sendTextMessage(cmd);

    cmd = QString(
        "set %1/%2/value/?cmd_id=set_command&message_id=%3 %4"
    ).arg(base_path, pme->getPropName(), QString::number(getMsgIdConf()),
          QString::number(fval));

    webSocketConf.sendTextMessage(cmd);
    qDebug() << "Sending " << cmd;

    cmd = QString(
        "post %1/set_touch?message_id=%2 {\"client_uid\":\"%3\",\"touch\":false}"
    ).arg(base_path, QString::number(getMsgIdConf()), getClientUuid());
    webSocketConf.sendTextMessage(cmd);    
}


/**
 * @brief BovineAPI::send_heartbeat
 * @details callback, that sends a heartbeat message to the other side.
 * @todo store the message id and inform the user if a no reponse occured after
 * a certain amount of time.
 */
void BovineAPI::send_heartbeat()
{
    QString hbm = QString("get /status?cmd_id=heartbeat&message_id=%1").arg(
                QString::number(getMsgIdConf()));

    qDebug() << hbm;
    if (webSocketConf.isValid())
        webSocketConf.sendTextMessage(hbm);
    devicePathMap->printEntries();
}


