/*
 Pomo - a simple Pomodoro application
 Copyright (c) 2013, 2021 Timo Härkönen

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtQuick.Controls 2.4

Item {
    implicitHeight: 100
    implicitWidth: 80

    Rectangle {
        anchors.fill: parent
        color: "#000"

        Column {
            id: column
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5
            width: parent.width - 10
            height: parent.height - 10
            spacing: 5

            Text {
                id: timeText
                text: engine.elapsedTime
                color: "#fff"
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                font.pointSize: 20
            }

            Text {
                text: "Pomodoro length"
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                color: "#fff"
            }

            SpinBox {
                id: minutesSpin
                width: parent.width
                from: 1
                to: 60
                value: engine.pomodoroLength

                contentItem: Text {
                    font: minutesSpin.font
                    text: minutesSpin.value + " " + qsTr("minutes")
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }

                onValueChanged: engine.pomodoroLength = value
            }

            Button {
                text: engine.isActive ? "Stop" : "start"
                width: parent.width

                onClicked: {
                    engine.isActive ? engine.interruptPomodoro() : engine.startPomodoro()
                }
            }
        }
    }
}
