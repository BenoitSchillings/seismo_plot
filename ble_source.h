#ifndef BLE_SOURCE_H
#define BLE_SOURCE_H

#include "deviceinfo.h"

#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QVector>
#include <QTimer>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include "qplotter.h"

QT_USE_NAMESPACE
class BLEReader: public QObject
{
    Q_OBJECT

public:
    BLEReader();
    ~BLEReader();
    void setTarget(QPlotter *target);

public slots:
    void deviceSearch();
    void connectToService(const QString &address);
    void disconnectService();


private slots:
    void serviceDiscovered(const QBluetoothUuid &);
    void serviceScanDone();
    void controllerError(QLowEnergyController::Error);
    void deviceConnected();


    //QLowEnergyService
    void serviceStateChanged(QLowEnergyService::ServiceState s);
    void updateValue(const QLowEnergyCharacteristic &c,
                              const QByteArray &value);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &d,
                              const QByteArray &value);

Q_SIGNALS:

private:
    DeviceInfo m_currentDevice;
    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent;
    QLowEnergyDescriptor m_notificationDesc;
    QList<QObject*> m_devices;
    QString m_info;
    QVector<quint16> m_measurements;
    QLowEnergyController *m_control;
    QLowEnergyService *m_service;
    QPlotter    *vtarget;
    float       v0;
};

#endif // HEARTRATE_H
