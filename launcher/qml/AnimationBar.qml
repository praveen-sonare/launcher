import QtQuick 2.0

Image {
    id: root
    width: 1080
    height: 70
    source: './images/Screentransition_bar.png'
    state: 'release'
    property int index: 0

    ListModel {
        id: model
        ListElement {
            name: "0"
            picturePath: './images/BG_undericon2.svg'
        }
        ListElement {
            name: "1"
            picturePath: './images/BG_undericon1.svg'
        }
        ListElement {
            name: "2"
            picturePath: './images/BG_undericon3.svg'
        }
        ListElement {
            name: "3"
            picturePath: './images/BG_undericon4.svg'
        }
    }

    Timer {
        id: animationTimer
        interval: 250
        running: false
        repeat: true
        onTriggered: {
                if (index < model.count) {
                    aniImage.source = model.get(index).picturePath
                    index++;
                    if (index == model.count)
                        index = 0
                }
        }
    }

    Image {
        id: aniImage
        anchors.centerIn: parent
    }

    function startAnimationTimer() {
        animationTimer.restart()
        console.log("animationTimer.start()")
    }

    function stopAnimationTimer() {
        animationTimer.stop()
        console.log("animationTimer.stop()")
    }

    states: [
        State {
            name: 'release'
            PropertyChanges {
                target: root
                y: 1704
            }
        },
        State {
            name: 'press'
            PropertyChanges {
                target: root
                y: 1634
            }
        }
    ]
    transitions: Transition {
        NumberAnimation {
            target: root
            property: 'y'
            easing.type: 'OutQuad'
            duration: 450
        }
    }
}
