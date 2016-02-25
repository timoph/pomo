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

#include "mainwindow.h"
#include "mainwidget.h"
#include "engine.h"
#include "settingsdialog.h"

#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QTimer>
#include <QCloseEvent>
#include <QStackedLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    pMainWidget = new MainWidget;
    pSettingsWidget = new SettingsDialog;
    pHolderWidget = new QWidget;
    pLayout = new QStackedLayout;
    pLayout->addWidget(pMainWidget);
    pLayout->addWidget(pSettingsWidget);
    pHolderWidget->setLayout(pLayout);
    setCentralWidget(pHolderWidget);

#ifdef Q_OS_MAC
    pSettingsMenu = new QMenu("Settings");
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_ShowWithoutActivating, true);
#else
    pSettingsMenu = menuBar()->addMenu("Settings");
#endif
    pSettingsAction = pSettingsMenu->addAction("Preferences");

    connect(Engine::instance(), SIGNAL(pomodoroEnded()), this, SLOT(reactToPomodoroEnd()));
    connect(pSettingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(Engine::instance(), SIGNAL(settingsTriggered()), this, SLOT(showSettings()));
    connect(pSettingsWidget, SIGNAL(done(bool)),
            this, SLOT(showMain(bool)));
}

void MainWindow::reactToPomodoroEnd()
{
    showNormal();
    activateWindow();
    raise();
}

void MainWindow::showSettings()
{
    pLayout->setCurrentIndex(1);
}

void MainWindow::showMain(bool settingsChanged)
{
    pLayout->setCurrentIndex(0);
    if(settingsChanged) {
        Engine::instance()->readSettings();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Engine::instance()->toggleVisibility();
    event->ignore();
}

