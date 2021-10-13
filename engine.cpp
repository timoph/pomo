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

#include "engine.h"

#include <QDebug>
#include <QMenu>
#include <QApplication>

#include <QSettings>

Engine *Engine::pInstance = 0;

Engine::Engine(QObject *parent) :
    QObject(parent)
{
    pTimer = new QTimer(this);
    pTimer->setInterval(1000);
    mActive = false;
    mPomodoroLength = 25;
    mElapsed = "0:00";
    mElapsedSeconds = 0;
    mWindowVisible = true;

    pSystray = new QSystemTrayIcon(this);
    pSystray->setIcon(QIcon(":/images/pomo.png"));
    pMenu = new QMenu;
    pVisibilityAction = pMenu->addAction("hide");
    pQuitAction = pMenu->addAction("quit");
    pSystray->setContextMenu(pMenu);
    pSystray->setVisible(true);

    readSettings();

    connect(pTimer, SIGNAL(timeout()), this, SLOT(tick()));
    connect(pVisibilityAction, SIGNAL(triggered()), this, SLOT(toggleVisibility()));
    connect(pQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(pSystray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systrayClicked(QSystemTrayIcon::ActivationReason)));
}

void Engine::startPomodoro()
{
    mStartTime = QTime::currentTime();
    mElapsedSeconds = 0;
    setActive(true);
    pTimer->start();

    if(pVisibilityAction->text() == "hide") {
        QTimer::singleShot(500, this, SLOT(toggleVisibility()));
    }

    QSettings settings;
    settings.setValue("pomodoro/duration", pomodoroLength());
}

void Engine::interruptPomodoro()
{
    setElapsed("0:00");
    setActive(false);
    pTimer->stop();

}

void Engine::setActive(bool active)
{
    if(mActive != active) {
        mActive = active;
        emit activeChanged(mActive);
    }
}

void Engine::setPomodoroLength(int length)
{
    if(mPomodoroLength != length) {
        mPomodoroLength = length;
        emit pomodoroLengthChanged(mPomodoroLength);
    }
}

void Engine::setWindowVisible(bool visible)
{
    mWindowVisible = visible;
    emit windowVisibleChanged(mWindowVisible);
}

void Engine::toggleVisibility()
{
    if(pVisibilityAction->text() == "hide") {
        pVisibilityAction->setText("show");
        setWindowVisible(false);
    }
    else {
        pVisibilityAction->setText("hide");
        setWindowVisible(true);
    }
}

void Engine::readSettings()
{
    QSettings settings;
    int duration = settings.value("pomodoro/duration", QVariant(25)).toInt();
    setPomodoroLength(duration);
}

void Engine::tick()
{
    mElapsedSeconds++;

    // update elapsed string
    int secs = mElapsedSeconds % 60;
    int mins = mElapsedSeconds / 60;
    QString str = QString("%1:%2").arg(mins).arg(secs, 2, 10, QChar('0'));
    setElapsed(str);

    // check if ended
    if(mElapsedSeconds / 60 == mPomodoroLength) {
        pTimer->stop();
        setActive(false);

        QApplication::beep();

        if(pVisibilityAction->text() == "show") {
            toggleVisibility();
        }

        emit pomodoroEnded();
    }
}

void Engine::setElapsed(const QString &elapsed)
{
    if(mElapsed != elapsed) {
        mElapsed = elapsed;
        emit elapsedTimeChanged(mElapsed);
    }
}

Engine *Engine::instance()
{
    if(!pInstance) {
        pInstance = new Engine;
    }
    return pInstance;
}

void Engine::systrayClicked(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick) {
        toggleVisibility();
    }
}
