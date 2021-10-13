# pomo.pro
TARGET = pomo

QT += core gui quick

equals(QT_MAJOR_VERSION, 5) {
    QT += widgets
}

macx {
    CONFIG += app_bundle
    ICON = images/pomo.icns
}

win {
    ICON = images/pomo.png
}

SOURCES += main.cpp \
    engine.cpp

RESOURCES += \
    resources.qrc

HEADERS += \
    engine.h

