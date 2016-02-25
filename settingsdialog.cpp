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

#include "settingsdialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QFileDialog>
#include <QDir>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QWidget(parent)
{
    QSettings settings;
    QString file = settings.value("alarm/file", QVariant("")).toString();
    pLayout = new QFormLayout;
    pSoundFileEdit = new QLineEdit;
    pSoundFileEdit->setText(file);
    pLayout->addRow(new QLabel(tr("Alarm sound file")), pSoundFileEdit);

    pButtonLayout = new QHBoxLayout;
    pOkButton = new QPushButton(tr("ok"));
    pCancelButton = new QPushButton(tr("cancel"));
    pSelectButton = new QPushButton(tr("select file..."));
    pButtonLayout->addWidget(pSelectButton);
    pButtonLayout->addWidget(pCancelButton);
    pButtonLayout->addWidget(pOkButton);

    pLayout->addRow(pButtonLayout);
    setLayout(pLayout);

    connect(pSelectButton, SIGNAL(clicked()),
            this, SLOT(selectClicked()));
    connect(pOkButton, SIGNAL(clicked()),
            this, SLOT(okClicked()));
    connect(pCancelButton, SIGNAL(clicked()),
            this, SLOT(cancelClicked()));
}

void SettingsDialog::okClicked()
{
   QSettings settings;
   QString file = settings.value("alarm/file", QVariant("")).toString();
   if(pSoundFileEdit->text() != file && !pSoundFileEdit->text().isEmpty()) {
       settings.setValue("alarm/file", pSoundFileEdit->text());
   }
   emit done(true);
}

void SettingsDialog::selectClicked()
{
    QString filter("sound file (*.wav *.mp3);; all files (*.*)");
    QString file = QFileDialog::getOpenFileName(this, "Select sound file", QDir::homePath(), filter);
    pSoundFileEdit->setText(file);
}

void SettingsDialog::cancelClicked()
{
    emit done(false);
}
