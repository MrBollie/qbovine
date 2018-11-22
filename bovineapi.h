#ifndef BOVINEAPI_H
#define BOVINEAPI_H

#include <QtWebSockets/QtWebSockets>
#include <QTimer>

#include "qvector.h"
#include "QJsonObject"
#include <QComboBox>
#include <QDial>
#include <QPushButton>
#include <QSlider>

#include "bovinemap.h"
#include "mdnslookup.h"


typedef enum _deviceValueTypes {
    NVAL,
    SVAL,
    RVAL,
    VAL
} DeviceValueType;

class BovineAPI : public QObject{
    Q_OBJECT

public:
    explicit BovineAPI();
    ~BovineAPI();

    BovineNode *getDevicePresetFolders();
    BovineNode *getDevicePresets(const QString &folderUID);

    void loadDevicePreset(const QString &uid);
    bool readDevicesSubtree(QJsonObject &obj);
    bool readPresetsSubtree(QJsonObject &obj);
    BovineMap *getPresetPathMap() const;
    BovineMap *getDevicePathMap() const;
    void selectComboBox(QComboBox *cb, int index);
    void setPushButton(QPushButton *pb, bool checked);
    void setSlider(QSlider *sl, int value);
    void setDial(QDial *dial, int value);
    void addDevicePath(QString path, QString PropertyName, QWidget* widget,
                       WidgetType type);
    void connectToDevice();

private slots:
    QString getClientUuid();
    void onWSConfClosed();
    void onWSConfConnected();
    void onWSConfTextMessageReceived(const QString &message);
    void onWSDevClosed();
    void onWSDevConnected();
    void onWSDevTextMessageReceived(const QString &message);
    void on_device_found(QString ip, uint16_t port);
    void send_heartbeat();

private:
    QUuid client_uid;

    BovineMap *devicePathMap;
    QTimer *heartbeatTimer;

    MDNSLookup* mdns;

    unsigned int msgIdConf;
    unsigned int msgIdDev;

    BovineMap *presetPathMap;
    QHash<QString, BovineNode*>* uid2PresetMap;

    QWebSocket webSocketConf;
    QWebSocket webSocketDev;

    unsigned int getMsgIdConf();
    unsigned int getMsgIdDev();

    void updateMeters(const QJsonObject &meter_values);

signals:
    void deviceConnected();
    void updateMeterLevel(int index, double value, double peakValue);
};

#endif // BOVINEAPI_H
