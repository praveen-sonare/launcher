import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.0

Item {
    id: root
    width: 1080
    height: 1920
    property string contents: ''
    property string uninstallApp: ''
    property int pid: -1

    MouseArea {
        anchors.fill: parent
        onPressAndHold: {}
        onPressed: {}
        onReleased: {}
        onClicked: {}
    }

    Rectangle {
        id: mainform
        height: 300
        width: 1000
        radius:2
        anchors.centerIn: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#12262E" }
            GradientStop { position: 1.0; color: "#18899B" }
        }

        ColumnLayout {
            anchors {
              topMargin: 10; bottomMargin:10
              leftMargin: 20; rightMargin: 20
              fill: parent
            }
            spacing: 2

            ColumnLayout {
                id: title_part
                anchors {
                    top: parent.top
                    left: parent.left
                        topMargin: 10
                }

                Label {
                    id: title
                    text: 'Uninstall Dialog'
                    color: "white"
                    font.pixelSize: 32
                    font.bold: true
                    maximumLineCount: 1
                    wrapMode: Text.Wrap
                    elide: Text.ElideRight
                    horizontalAlignment: Label.AlignHCenter
                    verticalAlignment: Label.AlignVCenter
                    Layout.preferredWidth:  960
                    Layout.preferredHeight:  40
                }

                Image {
                    source: './images/DividingLine.svg'
                    anchors.left: title.left
                    anchors.top: title.bottom
                }
            }

            RowLayout {
                id: contents_part
                anchors {
                  left: parent.left; leftMargin: 20
                  right: parent.right; rightMargin: 20
                }
                Layout.preferredWidth: 920
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                spacing: 10
                Image {
                    id: dsp_mark
                    source: './images/shotcuticon_dustbox.svg'
                    Layout.maximumHeight: 120
                    Layout.maximumWidth:  120
                }
                Label {
                    text: root.contents
                    color: "white"
                    font.pixelSize: 24
                    wrapMode: Text.Wrap
                    elide: Text.ElideRight
                    horizontalAlignment: Label.AlignLeft
                    verticalAlignment: Label.AlignVCenter
                    Layout.preferredWidth: 780
                    Layout.preferredHeight: 160
                }
            }

            RowLayout {
                id: btn_area
                spacing: 60
                anchors {
                  horizontalCenter: parent.horizontalCenter
                }
                Layout.preferredWidth: parent.width*0.75
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                Button {
                    id: btn1
                    text: 'YES'
                    onReleased: {
                        btn1.highlighted = false
                        root.visible = false
                        pid = homescreenHandler.uninstallApplication(uninstallApp)
                        if(pid < 1) {
                            console.warn("app cannot be uninstaled!")
                        }
                    }
                    onPressed: {
                        btn1.highlighted = true
                    }
                    onCanceled: {
                        btn1.highlighted = false
                    }
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                Button {
                    id: btn2
                    text: 'NO'
                    onReleased: {
                        btn2.highlighted = false
                        root.visible = false
                    }
                    onPressed: {
                        btn2.highlighted = true
                    }
                    onCanceled: {
                        btn2.highlighted = false
                    }
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }
            }

            Rectangle {
                id: footer
                opacity: 0
                width: parent.width
                height: 5
                anchors {
                    bottom: parent.bottom
                }
            }
        }

    }

}
