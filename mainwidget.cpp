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

#include "mainwidget.h"
#include "engine.h"

#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    pDurationSlider = new QSlider;
    pDurationSlider->setRange(1, 60);
    pDurationSlider->setTickPosition(QSlider::TicksBelow);
    pDurationSlider->setOrientation(Qt::Horizontal);
    pDurationSlider->setValue(Engine::instance()->pomodoroLength());

    pDurationLabel = new QLabel;
    pDurationLabel->setText(QString::number(pDurationSlider->value()));

    pSliderLayout = new QHBoxLayout;
    pSliderLayout->addWidget(pDurationSlider);
    pSliderLayout->addWidget(pDurationLabel);

    pStartButton = new QPushButton(tr("Start"));

    pElapsedTimeLabel = new QLabel(tr("0min 0sec"));

    pResetAlarmButton = new QPushButton(tr("shut up"));
    pResetAlarmButton->setEnabled(false);

    pVoidButton = new QPushButton(tr("Void pomodoro"));
    pVoidButton->setEnabled(false);

    pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(pElapsedTimeLabel);
    pMainLayout->addLayout(pSliderLayout);
    pMainLayout->addWidget(pStartButton);
    pMainLayout->addWidget(pResetAlarmButton);
    pMainLayout->addWidget(pVoidButton);

    setLayout(pMainLayout);

    connect(pStartButton, SIGNAL(clicked()), this, SLOT(startClicked()));
    connect(pResetAlarmButton, SIGNAL(clicked()), this, SLOT(resetClicked()));
    connect(Engine::instance(), SIGNAL(elapsedTimeChanged(QString)),
            pElapsedTimeLabel, SLOT(setText(QString)));
    connect(pResetAlarmButton, SIGNAL(clicked()),
            Engine::instance(), SLOT(stopAlarm()));
    connect(pDurationSlider, SIGNAL(sliderMoved(int)),
            Engine::instance(), SLOT(setPomodoroLength(int)));
    connect(Engine::instance(), SIGNAL(pomodoroLengthChanged(int)),
            pDurationLabel, SLOT(setNum(int)));
    connect(Engine::instance(), SIGNAL(pomodoroEnded()),
            this, SLOT(pomodoroEnded()));
    connect(pVoidButton, SIGNAL(clicked()), this, SLOT(voidPomodoro()));
}

void MainWidget::startClicked()
{
    if(!Engine::instance()->isActive()) {
        Engine::instance()->startPomodoro();
        pDurationSlider->setDisabled(true);
        pVoidButton->setEnabled(true);
    }
}

void MainWidget::resetClicked()
{
    pResetAlarmButton->setEnabled(false);
}

void MainWidget::pomodoroEnded()
{
    pResetAlarmButton->setEnabled(true);
    pDurationSlider->setEnabled(true);
}

void MainWidget::voidPomodoro() {
    if(Engine::instance()->isActive()) {
        Engine::instance()->interruptPomodoro();
        pDurationSlider->setDisabled(false);
        pVoidButton->setEnabled(false);
        pStartButton->setEnabled(true);
    }
}
