# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial

if(android_app OR
    (QT_FEATURE_commandlineparser
        AND NOT ANDROID
        AND NOT IOS
        AND NOT WASM
        AND NOT rtems))
        add_subdirectory(canbusutil)
endif()
