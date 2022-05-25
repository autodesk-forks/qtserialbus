/****************************************************************************
**
** Copyright (C) 2017 Andre Hartmann <aha_1980@gmx.de>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialBus module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
******************************************************************************/

#ifndef QCANBUSDEVICEINFO_P_H
#define QCANBUSDEVICEINFO_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

QT_BEGIN_NAMESPACE

class QCanBusDeviceInfoPrivate : public QSharedData {
public:
    QCanBusDeviceInfoPrivate() { }

    ~QCanBusDeviceInfoPrivate()
    {
    }

    QString plugin;
    QString name;
    QString description;
    QString serialNumber;
    QString alias;
    int  channel = 0;
    bool hasFlexibleDataRate = false;
    bool isVirtual = false;
};

QT_END_NAMESPACE

#endif // QCANBUSDEVICEINFO_P_H
