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
import QtQuick.Window 2.11
import AppModel 1.0

ApplicationWindow {
    width: container.width * container.scale
    height: container.height * container.scale

    property int pid: -1
    property string app_id: ''

    Item {
        id: container
        anchors.centerIn: parent
        width: Screen.width
        height: Screen.height
        scale: 1

        Image {
          anchors.centerIn: parent
          source: './images/AGL_HMI_Blue_Background_Car-01.png'
        }

    GridView {
        id: grid
        anchors {
          topMargin: 60; bottomMargin: 0 
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

	Timer {
		id: timer_start_show
		interval: 500
		running: false
		repeat: false

		onTriggered: {
			console.log("Activating app_id " + app_id)
			shell.activate_app(Window.window, app_id)
			app_id = ''
		}
	}

        MouseArea {
            id: loc
            anchors.fill: parent
            property string currentId: ''
            property int newIndex: -1
            property int index: grid.indexAt(loc.mouseX, loc.mouseY)
            x: 62
            y: 264
            onPressAndHold: currentId = applicationModel.id(newIndex = index)
            onReleased: {
                if(loc.index < 0) {
                    return
                }
                if (currentId === '') {
                    //homescreenHandler.tapShortcut(applicationModel.appid(loc.index))
		    if (app_id === '') {
			    app_id = applicationModel.appid(loc.index)
			    console.log("Trying to launch " + app_id)

			    if (pid === -1) {
				    console.log("pid is not set: " + pid)
				    pid = homescreenHandler.launch(app_id)
			    } else {
				    console.log("pid is already set: " + pid)
				    pid = homescreenHandler.launch(app_id)
			    }

			    if (pid > 0) {
				    console.log("Activating timer to activate app " + app_id)
				    timer_start_show.running = true
			    } else {
				    app_id = ''
			    }
		    } else {
			    console.log("app_id is not empty = " + app_id)
			    app_id = ''
		    }
                } else {
		    console.log("Current_id is not empty!")
                    currentId = ''
                }
            }
            onPositionChanged: {
                if (loc.currentId === '') return
                if (index < 0) return
                if (index === newIndex) return
                    applicationModel.move(newIndex, newIndex = index)
            }
        }
    }
}
}
