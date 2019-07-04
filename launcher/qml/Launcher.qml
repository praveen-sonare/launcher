/*
 * Copyright (C) 2016 The Qt Company Ltd.
 * Copyright (C) 2016, 2017 Mentor Graphics Development (Deutschland) GmbH
 * Copyright (c) 2018,2019 TOYOTA MOTOR CORPORATION
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

ApplicationWindow {
    width: container.width * container.scale
    height: container.height * container.scale

    property int pid: -1
    property bool isLongPress: false

    Item {
        id: container
        anchors.centerIn: parent
        width: 1080
        height: 1920
//        scale: screenInfo.scale_factor()

        Image {
          anchors.fill: parent
          source: './images/background.png'
        }

        Image {
            id: topshortcutArea
            x: 197
            y: 0
            width: 588
            height: 215
            source: './images/top_three_shortcut_background.png'
            TopShortcutArea {
                anchors.fill: parent
            }
        }

        AnimationBar {
            id: animationbar
            x: 0
            y: 1704
            state: 'release'
        }

        BottomShortcutArea {
            id: bottomshortcutarea
            x: 0
            y: 1704
        }

        GridView {
            id: grid
            anchors {
              topMargin: 60 + 218; bottomMargin: 0
              leftMargin: 60; rightMargin: 60
              fill: parent
            }
            contentHeight: 320
            flickableDirection: Flickable.AutoFlickDirection
            snapMode: GridView.SnapOneRow
            visible: true
            cellWidth: 320
            cellHeight: 320
            interactive: false

            model: ApplicationModel { id: applicationModel }
            delegate: IconItem {
                width: grid.cellWidth
                height: grid.cellHeight
            }

            Connections {
                target: homescreenHandler
                onAppListUpdate: {
                    console.warn("applist update in Launcher.qml")
                    applicationModel.updateApplist(info);
                }
            }
            Connections {
                target: homescreenHandler
                onInitAppList: {
                    console.warn("applist init in Launcher.qml")
                    applicationModel.initAppList(data);
                }
            }

            MouseArea {
                id: loc
                anchors.fill: parent
                property string currentId: ''
                property string currentName: ''
                property int oldIndex: -1
                property int newIndex: -1
                property int index: grid.indexAt(loc.mouseX, loc.mouseY)
                x: 62
                y: 264

                onPressAndHold: {
                    oldIndex = index
                    currentId = applicationModel.id(newIndex = index)
                    currentName = applicationModel.name(loc.index)
                    homescreenHandler.hideWindow("homescreen")
                    animationbar.state = 'press'
                    animationbar.startAnimationTimer()
                    isLongPress = true
                }
                onReleased: {
                    if (currentId === '' && loc.index >= 0 ) {
                            homescreenHandler.tapShortcut(applicationModel.appid(loc.index))
                    } else if (loc.mouseY <= 0) {
                        if (loc.mouseX >= 197 && loc.mouseX < 393) {
                            homescreenHandler.registerShortcut(currentId, currentName, "0");
                        } else if (loc.mouseX >= 393 && loc.mouseX < 589) {
                            homescreenHandler.registerShortcut(currentId, currentName, "1");
                        } else if (loc.mouseX >= 589 && loc.mouseX < 785) {
                            homescreenHandler.registerShortcut(currentId, currentName, "2");
                        }
                        if (oldIndex != newIndex){
                            applicationModel.move(newIndex, newIndex = oldIndex)
                        }
                    } else if (loc.mouseY >= 1488) {
                        if (loc.mouseX < 295 ) {
                            console.log("sendAppToMeter", currentId)
                            homescreenHandler.sendAppToMeter(currentId)
                        } else if (loc.mouseX >= 295 && loc.mouseX < 590 ) {
                            console.log("sendAppToHud", currentId)
                            homescreenHandler.sendAppToHud(currentId)
                        } else if (loc.mouseX >= 590 && loc.mouseX < 885 ) {

                        } else if (loc.mouseX >= 885) {
                            uninstallDialog.contents = 'Do you want to uninstall ' + currentName.toUpperCase() + '?'
                            uninstallDialog.uninstallApp = currentId
                            uninstallDialog.visible = true
                        }
                        if (oldIndex != newIndex){
//                            applicationModel.recordAppsPosition(newIndex, oldIndex, 0)
                            applicationModel.move(newIndex, newIndex = oldIndex)
                        }
                    }

                    currentName = ''
                    currentId = ''
                    if (isLongPress) {
                        homescreenHandler.tapShortcut("homescreen")
                        isLongPress = false
                        animationbar.state = 'release'
                        animationbar.stopAnimationTimer()
                    }
                }
                onPositionChanged: {
                    if (loc.currentId === '') return
                    if (index < 0) return
                    if (index === newIndex) return
                    applicationModel.move(newIndex, newIndex = index)
                    applicationModel.recordAppsPosition()
                }
            }
        }
        UninstallDialog {
            id: uninstallDialog
            anchors.fill: parent
            visible: false
        }
    }
}
