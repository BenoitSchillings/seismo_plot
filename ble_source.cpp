
#include "ble_source.h"

#include <QtEndian>
#include <QDebug>

BLEReader::BLEReader():
    m_currentDevice(QBluetoothDeviceInfo()),
    m_service(0)
{
    connectToService(QString("{35057c80-c233-4fcb-a7c3-1ab2abebea43}"));

}

BLEReader::~BLEReader()
{
}

void BLEReader::deviceSearch()
{
}

void BLEReader::setTarget(QPlotter *target)
{
    vtarget = target;
}

void BLEReader::connectToService(const QString &address)
{
    m_measurements.clear();
    QBluetoothDeviceInfo *fake = new QBluetoothDeviceInfo(QBluetoothUuid(QString("{35057c80-c233-4fcb-a7c3-1ab2abebea43}")), "HeartRateSketch", 0);

     DeviceInfo *dev = new DeviceInfo(*fake);

     m_currentDevice.setDevice(dev->getDevice());

    m_control = new QLowEnergyController(m_currentDevice.getDevice(), this);

    connect(m_control, SIGNAL(serviceDiscovered(QBluetoothUuid)), this, SLOT(serviceDiscovered(QBluetoothUuid)));
    connect(m_control, SIGNAL(discoveryFinished()), this, SLOT(serviceScanDone()));
    connect(m_control, SIGNAL(error(QLowEnergyController::Error)), this, SLOT(controllerError(QLowEnergyController::Error)));
    connect(m_control, SIGNAL(connected()), this, SLOT(deviceConnected()));
    connect(m_control, SIGNAL(disconnected()), this, SLOT(deviceDisconnected()));

    m_control->connectToDevice();
}

void BLEReader::deviceConnected()
{
    m_control->discoverServices();
}


void BLEReader::serviceDiscovered(const QBluetoothUuid &gatt)
{
}

void BLEReader::serviceScanDone()
{
    m_service = m_control->createServiceObject(
                QBluetoothUuid(QBluetoothUuid::HeartRate), this);

    connect(m_service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
            this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));
    connect(m_service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
            this, SLOT(updateValue(QLowEnergyCharacteristic,QByteArray)));
    //connect(m_service, SIGNAL(descriptorWritten(QLowEnergyDescriptor,QByteArray)),
      //      this, SLOT(confirmedDescriptorWrite(QLowEnergyDescriptor,QByteArray)));

    m_service->discoverDetails();
}

void BLEReader::disconnectService()
{
}

void BLEReader::controllerError(QLowEnergyController::Error error)
{
}

void BLEReader::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    switch (s) {
    case QLowEnergyService::ServiceDiscovered:
    {
        const QLowEnergyCharacteristic hrChar = m_service->characteristic(
                    QBluetoothUuid((QBluetoothUuid::LocationAndSpeed)));

         const QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(
                    QBluetoothUuid::ClientCharacteristicConfiguration);
        if (m_notificationDesc.isValid()) {
            m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
        }

        break;
    }
    default:
        break;
    }
}


void BLEReader::updateValue(const QLowEnergyCharacteristic &c,
                                     const QByteArray &value)
{
    const float *xdata = reinterpret_cast<const float *>(value.constData());
    float fvalue = xdata[0];
    vtarget->NewValue(fvalue);
}


void BLEReader::confirmedDescriptorWrite(const QLowEnergyDescriptor &d,
                                         const QByteArray &value)
{
}


