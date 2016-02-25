import QtQuick 2.0

Rectangle {
    width: 360
    height: 360
    color: engine.isActive ? "#00ff00" : "#ff0000"

    Connections {
        target: engine

        onPomodoroEnded: {
            console.log("pomodoro ended");
        }
    }

    Text {
        id: titleText
        text: engine.elapsedTime
        anchors.centerIn: parent
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            engine.startPomodoro();
        }
    }

    Rectangle {
        id: ackButton
        width: 100
        height: 50
        radius: 15
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        Text {
            id: ackText
            anchors.centerIn: parent
            text: qsTr("Stop!!")
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log("stop clicked");
            }
        }
    }
}
