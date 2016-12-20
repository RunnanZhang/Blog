QT += core xml
QT -= gui

CONFIG += c++11

TARGET = BlogSitemapGenerator

TEMPLATE = app

SOURCES += main.cpp \
    blogsitemapgenerator.cpp \
    setting.cpp

HEADERS += \
    blogsitemapgenerator.h \
    setting.h
