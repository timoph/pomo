# pomo.pro

contains(QT_VERSION, ^4.*) {
    QT += core gui quick
    message("building for Qt4")
}

contains(QT_VERSION, ^5.*) {
    QT += core widgets quick multimedia
    DEFINES += QT5BUILD
    message("building for Qt5")
}

macx {
    CONFIG += app_bundle
    ICON = images/pomo.icns
}

win {
    ICON = images/pomo.png
}

qmlui {
    DEFINES += POMOQMLUI
    message("using qml ui")
}

SOURCES += main.cpp \
    engine.cpp \
    mainwindow.cpp \
    mainwidget.cpp \
    settingsdialog.cpp

RESOURCES += \
    resources.qrc

HEADERS += \
    engine.h \
    mainwindow.h \
    mainwidget.h \
    settingsdialog.h

OTHER_FILES += \
    qml/pomo/main.qml
