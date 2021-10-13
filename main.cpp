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

#include <QApplication>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QQmlContext>


#include "engine.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/pomo.png"));
    app.setOrganizationDomain("timoph.fi");
    app.setOrganizationName("timoph");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("engine", Engine::instance());
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

//    QQuickView view;
//    view.rootContext()->setContextProperty("engine", Engine::instance());
//    view.setSource(QStringLiteral("qrc:/qml/main.qml"));

//    QObject::connect(Engine::instance(), SIGNAL(windowVisibleChanged(bool)),
//                     &view, SLOT(setVisible(bool)));

//    view.show();
    return app.exec();
}
