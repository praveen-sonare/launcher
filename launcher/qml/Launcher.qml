/*
 * Copyright (C) 2016 The Qt Company Ltd.
 * Copyright (C) 2016, 2017 Mentor Graphics Development (Deutschland) GmbH
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
import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0
import AppModel 1.0
import "ChangeItemPosition.js" as Cip

ApplicationWindow {
    id: root

    property int pid: -1
    property int firstPox: -1
    property bool iconMoved: false

    Timer {
        id: timer
        interval: 650
        repeat: false
        onTriggered: {
            var centerItem = Cip.getCenterItem()
            centerItem.state = "pos14"
            centerImage.opacity = 1.0
        }
    }

    Repeater {
        id: repeater
        model: ApplicationModel { id: applicationModel }

        property int currentItem

        IconItem {
            id: rect1
            property int currentState: 1

            width: 300
            height: 590

            smooth: true

            anchors.centerIn: parent

            Behavior on opacity {
                NumberAnimation { duration: 200 }
            }

            Behavior on z {
                NumberAnimation { duration: 300 }
            }
            transform: [
                Rotation {
                    id: rotate
                    angle: 0
                    origin.y: 243
                    origin.x: 160
                    axis { x: 0; y: 1; z: 0 }
                    Behavior on angle {
                        NumberAnimation { easing.overshoot: 1; easing.type: Easing.OutBack; duration: 600 }
                    }
                },
                Translate {
                    id: trans
                    x: 0
                    y: 0
                    Behavior on x {
                        NumberAnimation { easing.overshoot: 1; easing.type: Easing.OutBack; duration: 600 }
                    }
                    Behavior on y {
                        NumberAnimation { easing.overshoot: 1; easing.type: Easing.OutBack; duration: 600 }
                    }
                },
                Scale {
                    id: scale
                    origin.x: 160
                    origin.y: 243
                    xScale: 1
                    yScale: 1
                    Behavior on xScale {
                        NumberAnimation { easing.overshoot: 1; easing.type: Easing.OutBack; duration: 600 }
                    }
                    Behavior on yScale {
                        NumberAnimation { easing.overshoot: 1; easing.type: Easing.OutBack; duration: 600 }
                    }
                }
            ]

            states: [
                State {
                    name: "pos0"
                    PropertyChanges {
                        target: rotate
                        angle: 80
                    }
                    PropertyChanges {
                        target: trans
                        x: -1050
                        y: 270
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 0
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 1.2
                        yScale: 1.2
                    }
                },
                State {
                    name: "pos1"
                    PropertyChanges {
                        target: rotate
                        angle: 75
                    }
                    PropertyChanges {
                        target: trans
                        x: -700
                        y: 0
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 1.2
                        yScale: 1.2
                    }
                },
                State {
                    name: "pos2"
                    PropertyChanges {
                        target: rotate
                        angle: 60
                    }
                    PropertyChanges {
                        target: trans
                        x: -820
                        y: 0
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 0.79
                        yScale: 0.79
                    }
                },
                State {
                    name: "pos3"
                    PropertyChanges {
                        target: rotate
                        angle: 40
                    }
                    PropertyChanges {
                        target: trans
                        x: -780
                        y: 0
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 0.55
                        yScale: 0.55
                    }
                },
                State {
                    name: "pos4"
                    PropertyChanges {
                        target: rotate
                        angle: 15
                    }
                    PropertyChanges {
                        target: trans
                        x: -470
                        y: 0
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 0.45
                        yScale: 0.45
                    }
                },
                State {
                    name: "pos5"
                    PropertyChanges {
                        target: rotate
                        angle: 0
                    }
                    PropertyChanges {
                        target: trans
                        x: 0
                        y: 0
                    }
                    PropertyChanges {
                        target: rect1
                        z: 1
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 0.42
                        yScale: 0.42
                    }
                },
                State {
                    name: "pos6"
                    PropertyChanges {
                        target: rotate
                        angle: -15
                    }
                    PropertyChanges {
                        target: trans
                        x: 470
                        y: 0
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 0.45
                        yScale: 0.45
                    }
                },
                State {
                    name: "pos7"
                    PropertyChanges {
                        target: rotate
                        angle: -40
                    }
                    PropertyChanges {
                        target: trans
                        x: 780
                        y: 0
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 0.55
                        yScale: 0.55
                    }
                },
                State {
                    name: "pos8"
                    PropertyChanges {
                        target: rotate
                        angle: -60
                    }
                    PropertyChanges {
                        target: trans
                        x: 820
                        y: 0
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 0.79
                        yScale: 0.79
                    }
                },
                State {
                    name: "pos9"
                    PropertyChanges {
                        target: rotate
                        angle: -75
                    }
                    PropertyChanges {
                        target: trans
                        x: 700
                        y: 0
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 1.2
                        yScale: 1.2
                    }
                },
                State {
                    name: "pos10"
                    PropertyChanges {
                        target: rotate
                        angle: -80
                    }
                    PropertyChanges {
                        target: trans
                        x: 1050
                        y: 270
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 0
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 1.2
                        yScale: 1.2
                    }
                },
                State {
                    name: "pos11"
                    PropertyChanges {
                        target: rotate
                        angle: 0
                    }
                    PropertyChanges {
                        target: trans
                        x: 1050
                        y: 540
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 0
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 1
                        yScale: 1
                    }
                },
                State {
                    name: "pos12"
                    PropertyChanges {
                        target: rotate
                        angle: 0
                    }
                    PropertyChanges {
                        target: trans
                        x: 0
                        y: 540
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 0
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 1
                        yScale: 1
                    }
                },
                State {
                    name: "pos13"
                    PropertyChanges {
                        target: rotate
                        angle: 0
                    }
                    PropertyChanges {
                        target: trans
                        x: -1050
                        y: 540
                    }
                    PropertyChanges {
                        target: rect1
                        z: 0
                        opacity: 0
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 1
                        yScale: 1
                    }
                },
                State {
                    name: "pos14"
                    PropertyChanges {
                        target: rotate
                        angle: 0
                    }
                    PropertyChanges {
                        target: trans
                        x: 0
                        y: -50
                    }
                    PropertyChanges {
                        target: rect1
                        z: 1
                        opacity: 1
                    }
                    PropertyChanges {
                        target: scale
                        xScale: 1
                        yScale: 1
                    }
                }
            ]
        }

    }
    Component.onCompleted: {
        repeater.currentItem = 5;
        var count = repeater.count;
        for ( var i = 0; i < repeater.count; i++)
        {
            var item = repeater.itemAt(i);
            item.currentState = i + 1;
            if ( item.currentState >= 1 && item.currentState <= 10 )
            {
                item.state = "pos" + item.currentState;
                if ( item.currentState === 5 )
                    item.state = "pos14";
            } else if ( item.currentState === 15 ){
                item.state = "pos11";
            } else if ( item.currentState === 16 ){
                item.state = "pos12";
            } else if ( item.currentState === 17 ){
                item.state = "pos13";
            } else if ( item.currentState >= 18 ){
                item.state = "pos0";
            } else {
                item.state = "pos10";
            }
        }
    }

    Image {
        id: centerImage
        source: './images/center_background.png'
        height: 590
        width: 350
        anchors.centerIn: parent
        opacity: 1.0
    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        z: 2

        onPressed: {
            firstPox = mouseX;
            iconMoved = false;
            centerImage.opacity = 0.0;
            if(mouseX >= 800 && mouseX <= 1120)
            {
                var item = Cip.getCenterItem()
                item.iconPressed()
            }
        }

        onReleased: {
            timer.start()
            if(mouseX >= 800 && mouseX <= 1120)
            {
                var item = Cip.getCenterItem()
                item.iconReleased()
            }
        }

        onClicked: {
            if (iconMoved)
            {
                mouse.accepted = true
            } else {
                mouse.accepted = false
            }
        }

        onPositionChanged: {
            var item = Cip.getCenterItem()
            Cip.move(mouseX, item)
        }
    }

}
