#include <QtEndian>
#include "modkam.h"

QByteArray ActionsModkam::TemperatureOffset::request(const QString &, const QVariant &data)
{
    qint16 value = qToLittleEndian <qint16> (data.toInt());
    return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_16BIT_SIGNED, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
}

QByteArray ActionsModkam::HumidityOffset::request(const QString &, const QVariant &data)
{
    qint16 value = qToLittleEndian <qint16> (data.toInt());
    return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_16BIT_SIGNED, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
}

QByteArray ActionsModkam::PressureOffset::request(const QString &, const QVariant &data)
{
    qint32 value = qToLittleEndian <qint32> (data.toInt() * 10);
    return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_32BIT_SIGNED, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
}

QByteArray ActionsModkam::CO2Settings::request(const QString &name, const QVariant &data)
{
    int index = m_actions.indexOf(name);

    switch (index)
    {
        case 0: // autoCalibration
        case 1: // ledFeedback
        {
            quint8 value = data.toBool() ? 0x01 : 0x00;
            m_attributes = {static_cast <quint16> (index == 0 ? 0x0202 : 0x0203)};
            return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_BOOLEAN, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
        }

        case 2: // thresholdLow
        case 3: // thresholdHigh
        {
            quint16 value = qToLittleEndian <quint16> (data.toInt());
            m_attributes = {static_cast <quint16> (index == 2 ? 0x0204 : 0x0205)};
            return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_16BIT_UNSIGNED, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
        }
    }

    return QByteArray();
}

QByteArray ActionsModkam::Geiger::request(const QString &name, const QVariant &data)
{
    int index = m_actions.indexOf(name);

    switch (index)
    {
        case 0: // sensitivity
        {
            quint16 value = qToLittleEndian <quint16> (data.toInt());
            m_attributes = {0xF000};
            return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_16BIT_UNSIGNED, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
        }

        case 1: // ledFeedback
        case 2: // buzzerFeedback
        {
            quint8 value = data.toBool() ? 0x01 : 0x00;
            m_attributes = {static_cast <quint16> (index == 0 ? 0xF001 : 0xF002)};
            return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_BOOLEAN, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
        }

        case 3: // sensorCount
        {
            quint8 value = static_cast <quint8> (data.toInt());
            m_attributes = {0xF003};
            return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_8BIT_UNSIGNED, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
        }

        case 4: // sensorType
        {
            QList <QString> list = {"SBM-20/STS-5/BOI-33", "SBM-19/STS-6", "other"};
            qint8 value = static_cast <qint8> (list.indexOf(data.toString()));

            if (value < 0)
                return QByteArray();

            m_attributes = {0xF004};
            return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_8BIT_ENUM, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
        }

        case 5: // threshold
        {
            quint32 value = qToLittleEndian <quint32> (data.toInt());
            m_attributes = {0xF005};
            return writeAttributeRequest(m_transactionId++, m_manufacturerCode, m_attributes.at(0), DATA_TYPE_32BIT_UNSIGNED, QByteArray(reinterpret_cast <char*> (&value), sizeof(value)));
        }
    }

    return QByteArray();
}
