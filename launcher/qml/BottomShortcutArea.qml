/*
 * Copyright (C) 2016 The Qt Company Ltd.
 * Copyright (C) 2016, 2017 Mentor Graphics Development (Deutschland) GmbH
 * Copyright (c) 2017 TOYOTA MOTOR CORPORATION
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1

Image {
    id: root
    width: 1080
    height: 215
    source: './images/Screentransition_Background.png'

    Item {
        id: meterDisplay
        x: 0
        y: 0
        width: 295
        height: 215
        Image {
            anchors.centerIn: parent
            source: './images/shotcuticon_meter.svg'
        }
    }
    Item {
        id: hud
        anchors.left: meterDisplay.right
        anchors.top: meterDisplay.top
        width: 295
        height: 215
        Image {
            anchors.centerIn: parent
            source: './images/shotcuticon_hud.svg'
        }
    }
    Item {
        id: rse
        anchors.left: hud.right
        anchors.top: hud.top
        width: 295
        height: 215
        Image {
            anchors.centerIn: parent
            source: './images/shotcuticon_rse.svg'
        }
    }
    Item {
        id: uninstall
        anchors.left: rse.right
        anchors.top: rse.top
        width: 185
        height: 215
        Image {
            anchors.centerIn: parent
            source: './images/shotcuticon_dustbox.svg'
        }
    }
}
