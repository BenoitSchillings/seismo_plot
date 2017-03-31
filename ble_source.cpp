
#include "ble_source.h"

#include <QtEndian>
#include <QDebug>


#define  SAMPLEFILTER_TAP_NUM 55

typedef struct {
  double history[SAMPLEFILTER_TAP_NUM];
  unsigned int last_index;
} SampleFilter;


SampleFilter f;


void SampleFilter_init(SampleFilter* f);
void SampleFilter_put(SampleFilter* f, double input);
double SampleFilter_get(SampleFilter* f);



BLEReader::BLEReader():
    m_currentDevice(QBluetoothDeviceInfo()),
    m_service(0)
{
    SampleFilter_init(&f);
    v0 = 0;
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
    case QLowEnergyService::InvalidService:
    {
        connectToService(QString("{35057c80-c233-4fcb-a7c3-1ab2abebea43}"));
        break;
    }
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

    for (int i = 0; i < 4; i++) {
        float fvalue1 = xdata[i];
        //SampleFilter_put(&f, fvalue1);
        //vtarget->NewValue(SampleFilter_get(&f));
        vtarget->NewValue(fvalue1);
    }
}


void BLEReader::confirmedDescriptorWrite(const QLowEnergyDescriptor &d,
                                         const QByteArray &value)
{
}

static double filter_taps[SAMPLEFILTER_TAP_NUM] = {
  0.7086563293903965,
  -2.0786841742853785,
  3.848873036437792,
  -7.187881476626651,
  12.01254103305559,
  -18.9632048710678,
  28.63595273982041,
  -41.35866771664035,
  57.957767786052564,
  -78.60219954866037,
  104.01426122498845,
  -134.20176457783947,
  169.52467093297304,
  -209.7506304322909,
  254.7002648273218,
  -303.82627505886575,
  356.3297692712371,
  -411.33644702648024,
  467.466754439447,
  -523.6043547210323,
  577.9499598420956,
  -629.3274526658712,
  675.7751379874607,
  -716.3024322672572,
  749.0907633898216,
  -773.5847640309954,
  788.3928175640427,
  -792.583952538711,
  788.3928175640427,
  -773.5847640309954,
  749.0907633898216,
  -716.3024322672572,
  675.7751379874607,
  -629.3274526658712,
  577.9499598420956,
  -523.6043547210323,
  467.466754439447,
  -411.33644702648024,
  356.3297692712371,
  -303.82627505886575,
  254.7002648273218,
  -209.7506304322909,
  169.52467093297304,
  -134.20176457783947,
  104.01426122498845,
  -78.60219954866037,
  57.957767786052564,
  -41.35866771664035,
  28.63595273982041,
  -18.9632048710678,
  12.01254103305559,
  -7.187881476626651,
  3.848873036437792,
  -2.0786841742853785,
  0.7086563293903965
};

void SampleFilter_init(SampleFilter* f) {
  int i;
  for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void SampleFilter_put(SampleFilter* f, double input) {
  f->history[f->last_index++] = input;
  if(f->last_index == SAMPLEFILTER_TAP_NUM)
    f->last_index = 0;
}

double SampleFilter_get(SampleFilter* f) {
  double acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i) {
    index = index != 0 ? index-1 : SAMPLEFILTER_TAP_NUM-1;
    acc += f->history[index] * filter_taps[i];
  };
  return acc;
}
