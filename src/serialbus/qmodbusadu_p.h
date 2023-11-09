// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QMODBUSADU_P_H
#define QMODBUSADU_P_H

#include <QtSerialBus/qmodbuspdu.h>
#include <QtCore/private/qglobal_p.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

QT_BEGIN_NAMESPACE

class QModbusSerialAdu
{
public:
    enum Type {
        Ascii,
        Rtu
    };

    inline QModbusSerialAdu(Type type, const QByteArray &data)
        : m_type(type), m_data(data), m_rawData(data)
    {
        if (m_type == Ascii)
            m_data = QByteArray::fromHex(m_data.mid(1, m_data.size() - 3));
    }

    inline int size() const {
        if (m_type == Ascii)
            return m_data.size() - 1; // one byte, LRC
        return m_data.size() - 2; // two bytes, CRC
    }
    inline QByteArray data() const { return m_data.left(size()); }

    inline int rawSize() const { return m_rawData.size(); }
    inline QByteArray rawData() const { return m_rawData; }

    inline int serverAddress() const {
        Q_ASSERT_X(!m_data.isEmpty(), "QModbusAdu::serverAddress()", "Empty ADU.");
        return quint8(m_data.at(0));
    }

    inline QModbusPdu pdu() const {
        Q_ASSERT_X(!m_data.isEmpty(), "QModbusAdu::pdu()", "Empty ADU.");
        return QModbusPdu(QModbusPdu::FunctionCode(m_data.at(1)), m_data.mid(2, size() - 2));
    }

    template <typename T>
    auto checksum() const -> decltype(T()) {
        Q_ASSERT_X(!m_data.isEmpty(), "QModbusAdu::checksum()", "Empty ADU.");
        if (m_type == Ascii)
            return quint8(m_data[m_data.size() - 1]);
        return quint16(quint8(m_data[m_data.size() - 2]) << 8 | quint8(m_data[m_data.size() - 1]));
    }

    inline bool matchingChecksum() const {
        Q_ASSERT_X(!m_data.isEmpty(), "QModbusAdu::matchingChecksum()", "Empty ADU.");
        if (m_type == Ascii)
            return QModbusSerialAdu::calculateLRC(data(), size()) == checksum<quint8>();
        return QModbusSerialAdu::calculateCRC(data(), size()) == checksum<quint16>();
    }

    /*!
        \internal
        \fn quint8 QModbusSerialAdu::calculateLRC(const char *data, qint32 len)

        Returns the LRC checksum of the first \a len bytes of \a data. The checksum is independent
        of the byte order (endianness).
    */
    inline static quint8 calculateLRC(const char *data, qint32 len)
    {
        quint32 lrc = 0;
        while (len--)
            lrc += *data++;
        return -(quint8(lrc));
    }

    /*!
        \internal
        \fn quint16 QModbusSerialAdu::calculateCRC(const char *data, qint32 len) const

        Returns the CRC checksum of the first \a len bytes of \a data.

        \note The code used by the function was generated with pycrc. There is no copyright assigned
        to the generated code, however, the author of the script requests to show the line stating
        that the code was generated by pycrc (see implementation).
    */
    inline static quint16 calculateCRC(const char *data, qint32 len)
    {
        // Generated by pycrc v0.8.3, https://pycrc.org
        // Width = 16, Poly = 0x8005, XorIn = 0xffff, ReflectIn = True,
        // XorOut = 0x0000, ReflectOut = True, Algorithm = bit-by-bit-fast

        quint16 crc = 0xFFFF;
        while (len--) {
            const quint8 c = *data++;
            for (qint32 i = 0x01; i & 0xFF; i <<= 1) {
                bool bit = crc & 0x8000;
                if (c & i)
                    bit = !bit;
                crc <<= 1;
                if (bit)
                    crc ^= 0x8005;
            }
            crc &= 0xFFFF;
        }
        crc = crc_reflect(crc & 0xFFFF, 16) ^ 0x0000;
        return (crc >> 8) | (crc << 8); // swap bytes
    }

    inline static QByteArray create(Type type, int serverAddress, const QModbusPdu &pdu,
                                    char delimiter = '\n') {
        QByteArray result;
        QDataStream out(&result, QIODevice::WriteOnly);
        out << quint8(serverAddress) << pdu;

        if (type == Ascii) {
            out << calculateLRC(result, result.size());
            return ":" + result.toHex() + "\r" + delimiter;
        } else {
            out << calculateCRC(result, result.size());
        }
        return result;
    }

private:
    inline static quint16 crc_reflect(quint16 data, qint32 len)
    {
        // Generated by pycrc v0.8.3, https://pycrc.org
        // Width = 16, Poly = 0x8005, XorIn = 0xffff, ReflectIn = True,
        // XorOut = 0x0000, ReflectOut = True, Algorithm = bit-by-bit-fast

        quint16 ret = data & 0x01;
        for (qint32 i = 1; i < len; i++) {
            data >>= 1;
            ret = (ret << 1) | (data & 0x01);
        }
        return ret;
    }

private:
    Type m_type = Rtu;
    QByteArray m_data;
    QByteArray m_rawData;
};

QT_END_NAMESPACE

#endif // QMODBUSADU_P_H
