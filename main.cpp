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

#include <QApplication>
#ifdef POMOQMLUI
#include <QQuickView>
#include <QQmlContext>
#else
#include "mainwindow.h"
#endif

#include "engine.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/pomo.png"));
    app.setOrganizationDomain("timoph.fi");
    app.setOrganizationName("timoph");
#ifdef POMOQMLUI
    QQuickView view;
    view.rootContext()->setContextProperty("engine", Engine::instance());
    view.setSource(QStringLiteral("qrc:///qml/pomo/main.qml"));
#else
    MainWindow view;
#endif
    QObject::connect(Engine::instance(), SIGNAL(windowVisibleChanged(bool)),
                     &view, SLOT(setVisible(bool)));

    view.show();
    return app.exec();
}
