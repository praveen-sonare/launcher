/*
 * Copyright (C) 2016 The Qt Company Ltd.
 * Copyright (c) 2018 TOYOTA MOTOR CORPORATION
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
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import "ChangeItemPosition.js" as Cip

Item {
    id: main
    width: 300
    height: 300
    property string icon: model.icon
    property int pid: -1
    property bool isBlank: false
    property bool isPressing: false

    Timer {
        id: launchTimer
        interval: 650
        repeat: false
        onTriggered: {
            launchApp()
        }
    }

    Item {
        id: container
        width: main.width
        height: main.height
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(main.state === "pos14") {
                    launchApp()
                } else if(main.state === "pos4") {
                    Cip.prev()
                    launchTimer.start()
                } else if(main.state === "pos3") {
                    Cip.prev()
                    Cip.prev()
                    launchTimer.start()
                } else if(main.state === "pos2") {
                    Cip.prev()
                    Cip.prev()
                    Cip.prev()
                    launchTimer.start()
                } else if(main.state === "pos1") {
                    Cip.prev()
                    Cip.prev()
                    Cip.prev()
                    Cip.prev()
                    launchTimer.start()
                } else if(main.state === "pos6") {
                    Cip.next()
                    launchTimer.start()
                } else if(main.state === "pos7") {
                    Cip.next()
                    Cip.next()
                    launchTimer.start()
                } else if(main.state === "pos8") {
                    Cip.next()
                    Cip.next()
                    Cip.next()
                    launchTimer.start()
                } else if(main.state === "pos9") {
                    Cip.next()
                    Cip.next()
                    Cip.next()
                    Cip.next()
                    launchTimer.start()
                }
            }
        }

        Image {
            id: item
            anchors.fill: parent
            source: './images/%1_active.png'.arg(model.icon)
            property string initial: model.name.substring(0,1).toUpperCase()
            property bool hasIcon: model.icon !== 'blank'

            Label {
                style: Text.Outline
                styleColor: 'white'
                color: 'transparent'
                font.pixelSize: 225
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -50
                text: item.initial
                visible: item.hasIcon ? false : true
            }

            Label {
                id: name
                anchors.bottom: item.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 20
                font.pixelSize: 25
                font.letterSpacing: 5
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                text: qsTr(model.name.toUpperCase())
            }

        }
    }
    function iconPressed() {
        item.source = './images/%1_inactive.png'.arg(model.icon)
    }
    function iconReleased() {
        item.source = './images/%1_active.png'.arg(model.icon)
    }
    function launchApp() {
//        pid = launcher.launch(model.id)
//        if (1 < pid) {
//        } else {
//            console.warn("app cannot be launched!")
//        }
        homescreenHandler.tapShortcut(model.name)
    }
}
