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

#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QFile>
#include <QSystemTrayIcon>

class QMenu;
class QAction;

class Engine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isActive READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QString elapsedTime READ elapsedTime NOTIFY elapsedTimeChanged)
    Q_PROPERTY(quint8 pomodoroLength READ pomodoroLength WRITE setPomodoroLength NOTIFY pomodoroLengthChanged)
    Q_PROPERTY(bool windowVisible READ windowVisible NOTIFY windowVisibleChanged)

public:
    static Engine *instance();
    explicit Engine(QObject *parent = 0);
    bool isActive() const { return mActive; }
    QString elapsedTime() const { return mElapsed; }
    int pomodoroLength() const { return mPomodoroLength; }
    bool windowVisible() const { return mWindowVisible; }
    Q_INVOKABLE void startPomodoro();
    Q_INVOKABLE void interruptPomodoro();
    bool alarmFileSet() const;
    
signals:
    void activeChanged(bool active);
    void pomodoroStarted();
    void pomodoroEnded();
    void pomodoroInterupted();
    void pomodoroLengthChanged(int length);
    void elapsedTimeChanged(const QString &elapsed);
    void windowVisibleChanged(bool visible);
    void settingsTriggered();

public slots:
    void setActive(bool active);
    void setPomodoroLength(int length);
    void setWindowVisible(bool visible);
    void toggleVisibility();
    void systrayClicked(QSystemTrayIcon::ActivationReason reason);
    void readSettings();

private slots:
    void tick();
    void setElapsed(const QString &elapsed);

private:
    static Engine *pInstance;
    bool mActive;
    QString mElapsed;
    int mPomodoroLength;
    QTimer *pTimer;
    QTime mStartTime;
    int mElapsedSeconds;
    QSystemTrayIcon *pSystray;
    QMenu *pMenu;
    QAction *pVisibilityAction;
    bool mWindowVisible;
    QAction *pQuitAction;
    //QAction *pSettingsAction;
};

#endif // ENGINE_H
