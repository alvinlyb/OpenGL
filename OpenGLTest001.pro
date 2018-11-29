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
    camera.cxx \
    model.cxx \
    mesh.cxx

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#DEFINES += THIRDPARTY
THIRDPARTY = D:/cxxlib
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#QMAKE_CXXFLAGS += -Zi
#QMAKE_CFLAGS_DEBUG += -Zi -MDd
#QMAKE_CXXFLAGS_DEBUG += -Zi -MDd
#QMAKE_LFLAGS_DEBUG  = /DEBUG /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib

DESTDIR+=$$PWD/Debug

INCLUDEPATH +=  $$THIRDPARTY/Assimp/include \
                $$THIRDPARTY/SDL2-2.0.9/include \
                $$THIRDPARTY/glad/include \
                $$THIRDPARTY/glfw-3.2.1.bin.WIN64/glfw-3.2.1.bin.WIN64/include \
                $$THIRDPARTY/glm \
                $$THIRDPARTY/opencv/build/include \
                $$THIRDPARTY/glew-2.1.0/include \
                $$THIRDPARTY/soil2/include \
                $$THIRDPARTY/boost_1_68_0
CONFIG(debug,debug|release){
    #LIBS+=F:/script/QtProject/SOIL2/Debug/SOIL2.lib
    LIBS+=D:/cxxlib/soil2/lib/soil2d.lib

    LIBS+= -L$$THIRDPARTY/glfw-3.2.1.bin.WIN64/glfw-3.2.1.bin.WIN64/lib-vc2015 \
            -L$$THIRDPARTY/SDL2-2.0.9/lib/x64 \
            -L$$THIRDPARTY/opencv/build/x64/vc14/lib \
            -L$$THIRDPARTY/glew-2.1.0/lib/Release/x64 \
            -L$$THIRDPARTY/Assimp/lib/x64
            #-LD:/cxxlib/glew-2.1.0/lib/Release/x64

    LIBS+=-lSDL2 \
            -lSDL2main \
            -lopengl32 \
            -lglfw3dll \
            -lglfw3 \
            -lopencv_world343d \
            -lglew32 \
            -lassimp-vc140-mt
            #-lSOIL2
}else{
    #LIBS+=F:/script/QtProject/SOIL2/Debug/SOIL2.lib
    LIBS+=D:/cxxlib/soil2/lib/soil2d.lib

    LIBS+= -L$$THIRDPARTY/glfw-3.2.1.bin.WIN64/glfw-3.2.1.bin.WIN64/lib-vc2015 \
            -L$$THIRDPARTY/SDL2-2.0.9/lib/x64 \
            -L$$THIRDPARTY/opencv/build/x64/vc14/lib \
            -L$$THIRDPARTY/glew-2.1.0/lib/Release/x64
            #-LD:/cxxlib/glew-2.1.0/lib/Release/x64

    LIBS+=-lSDL2 \
            -lSDL2main \
            -lglfw3dll \
            -lglfw3 \
            -lopencv_world343 \
            -lglew32
            #-lSOIL2
}
HEADERS += \
    shader.hxx \
    camera.hxx \
    testdata.hxx \
    stb_image.h \
    model.hxx \
    mesh.hxx

DISTFILES += \
    7.3.camera.vs
