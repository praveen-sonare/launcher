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

import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1

Item {
    id: root
    width: 588
    height: 218

    property int pid: -1

    RowLayout {
        anchors.fill: parent
        spacing: 2
        Repeater {
            id: repeater
            model: shortcutAppModel
            delegate: TopShortcutIcon {
                Layout.fillWidth: true
                Layout.fillHeight: true
                name: shortcutAppModel.getName(model.index)
                icon: shortcutAppModel.getIcon(model.index)
                isBlank: shortcutAppModel.isBlank(model.index)
            }
        }
    }

    Connections {
        target: shortcutAppModel
        onUpdateShortcut: {
            for(var i = 0; i < 3; i++) {
                var item = repeater.itemAt(i)
                item.name = shortcutAppModel.getName(i)
                item.icon = shortcutAppModel.getIcon(i)
                item.isBlank = shortcutAppModel.isBlank(i)
            }
        }
    }
}
