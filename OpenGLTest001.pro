QT += core opengl
QT -= gui

CONFIG += c++11

TARGET = OpenGLTest001
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cxx \
    glad.c \
    shader.cxx \
    camera.cxx

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR+=$$PWD/Debug

INCLUDEPATH +=  D:/cxxlib/assimp-3.3.1/include \
                D:/cxxlib/SDL2-2.0.9/include \
                D:/cxxlib/glad/include \
                D:/cxxlib/glfw-3.2.1.bin.WIN64/glfw-3.2.1.bin.WIN64/include \
                D:/cxxlib/glm \
                D:/cxxlib/opencv/build/include \
                D:\cxxlib/glew-2.1.0/include
debug:LIBS+= -LD:/cxxlib/glfw-3.2.1.bin.WIN64/glfw-3.2.1.bin.WIN64/lib-vc2015 \
            -LD:/cxxlib/SDL2-2.0.9/lib/x64 \
            -LD:/cxxlib/opencv/build/x64/vc14/lib 、
            #-LD:/cxxlib/glew-2.1.0/lib/Release/x64

debug:LIBS+=-lSDL2 \
            -lSDL2main \
            -lglfw3dll \
            -lglfw3 \
            -lopencv_world343d \
            #-lglew32

HEADERS += \
    shader.hxx \
    camera.hxx \
    testdata.hxx

DISTFILES += \
    7.3.camera.vs
