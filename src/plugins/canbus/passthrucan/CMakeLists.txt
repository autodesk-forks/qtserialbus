# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial

#####################################################################
## PassThruCanBusPlugin Plugin:
#####################################################################

qt_internal_add_plugin(PassThruCanBusPlugin
    OUTPUT_NAME qtpassthrucanbus
    PLUGIN_TYPE canbus
    SOURCES
        j2534passthru.cpp j2534passthru.h
        main.cpp
        passthrucanbackend.cpp passthrucanbackend.h
        passthrucanio.cpp passthrucanio.h
    LIBRARIES
        Qt::Network
        Qt::SerialBus
)
