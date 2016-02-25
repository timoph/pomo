/*
 Pomo - Crossplatform Pomodoro application
 Copyright (c) 2013 Timo Härkönen

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "engine.h"

#include <QDebug>
#include <QMenu>
#include <QApplication>

#ifdef QT5BUILD
#include <QMediaPlayer>
#include <QMediaContent>
#endif

#include <QSettings>

Engine *Engine::pInstance = 0;

Engine::Engine(QObject *parent) :
    QObject(parent)
{
    pTimer = new QTimer(this);
    pTimer->setInterval(1000);
    mActive = false;
    mPomodoroLength = 25;
    mElapsed = "0";
    mElapsedSeconds = 0;
    mWindowVisible = true;

    pSystray = new QSystemTrayIcon(this);
#ifdef Q_OS_WIN
    pSystray->setIcon(QIcon(":/images/pomo.svg"));
#elif defined(Q_OS_MAC)
    pSystray->setIcon(QIcon(":/images/pomogs.png"));
#else
    pSystray->setIcon(QIcon(":/images/pomo.png"));
#endif
    pMenu = new QMenu;
    pVisibilityAction = pMenu->addAction("hide");
    pSettingsAction = pMenu->addAction("settings");
    pQuitAction = pMenu->addAction("quit");
    pSystray->setContextMenu(pMenu);
    pSystray->setVisible(true);

#ifdef QT5BUILD
    pPlayer = new QMediaPlayer(this);
#endif

    readSettings();

    connect(pTimer, SIGNAL(timeout()), this, SLOT(tick()));
    connect(pVisibilityAction, SIGNAL(triggered()), this, SLOT(toggleVisibility()));
    connect(pQuitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(pSettingsAction, SIGNAL(triggered()), this, SIGNAL(settingsTriggered()));
    connect(pSystray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(systrayClicked(QSystemTrayIcon::ActivationReason)));
}

void Engine::startPomodoro()
{
    mStartTime = QTime::currentTime();
    mElapsedSeconds = 0;
    setElapsed("0min 0sec");
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
    setElapsed("0min 0sec");
    setActive(false);
    pTimer->stop();

}

bool Engine::alarmFileSet() const
{
    QSettings settings;
    return !settings.value("alarm/file", QVariant("")).toString().isEmpty();
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

void Engine::stopAlarm()
{
#ifdef QT5BUILD
    pPlayer->stop();
#endif
}

void Engine::setAlarmFile(const QString &file)
{
#ifdef QT5BUILD
    pPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(file)));
#endif
}

void Engine::readSettings()
{
    QSettings settings;
    QString file = settings.value("alarm/file", QVariant("")).toString();
    setAlarmFile(file);
    int duration = settings.value("pomodoro/duration", QVariant(25)).toInt();
    setPomodoroLength(duration);
}

void Engine::tick()
{
    mElapsedSeconds++;

    // update elapsed string
    int secs = mElapsedSeconds % 60;
    int mins = mElapsedSeconds / 60;
    QString str = QString("%1min %2sec").arg(mins).arg(secs);
    setElapsed(str);

    // check if ended
    if(mElapsedSeconds / 60 == mPomodoroLength) {
        pTimer->stop();
        setActive(false);

        pSystray->showMessage("Time's up", "Stop working faggot", QSystemTrayIcon::Critical, 30000);
#ifdef QT5BUILD
        pPlayer->play();
#endif

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
