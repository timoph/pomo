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
import QtQuick.Window 2.12

Window {
    id: window
    height: 150
    width: 240
    title: "Pomodoro"
    visible: engine.windowVisible

    onClosing: {
        close.accepted = false
        engine.toggleVisibility()
    }

    Connections {
        target: engine

        onPomodoroEnded: {
            if(!window.visible) {
                engine.toggleVisibility()
            }
        }
    }

    PomoSetupView {
        anchors.fill: parent
    }
}
