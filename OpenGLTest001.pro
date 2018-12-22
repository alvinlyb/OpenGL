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
    mesh.cxx \
    SDLProcessEvent.cxx

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#QMAKE_CXXFLAGS += -Zi
#QMAKE_CFLAGS_DEBUG += -Zi -MDd
#QMAKE_CXXFLAGS_DEBUG += -Zi -MDd
#QMAKE_LFLAGS_DEBUG  = /DEBUG /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib

DESTDIR+=$$PWD/Debug

win32{
    LIBDIR = C:/cxxlib
    THIRDPARTY = $$PWD/ThirdParty
    INCLUDEPATH +=  $$LIBDIR/Assimp/include \
            $$LIBDIR/openGL/glad/include \
            $$LIBDIR/openGL/glfw-3.2.1.bin.WIN64/include \
            $$LIBDIR/openGL/glew-2.1.0/include \
            $$LIBDIR/openGL/glm \
            $$LIBDIR/openG/soil2/include \
            $$LIBDIR/opencv/build/include \
            $$LIBDIR/SDL2-2.0.9/include \
            $$LIBDIR/boost_1_68_0

    CONFIG(debug,debug|release){
        #LIBS+=F:/script/QtProject/SOIL2/Debug/SOIL2.lib
        LIBS+=$$LIBDIR/openGL/soil2/lib/soil2d.lib

        LIBS+= -L$$LIBDIR/openGL/glfw-3.2.1.bin.WIN64/lib-vc2015 \
                -L$$LIBDIR/openGL/glew-2.1.0/lib/Release/x64 \
                -L$$LIBDIR/Assimp/lib/x64 \
                -L$$LIBDIR/SDL2-2.0.9/lib/x64 \
                -L$$LIBDIR/opencv/build/x64/vc14/lib

        LIBS+=-lSDL2 \
                -lSDL2main \
                -lopengl32 \
                -lglfw3dll \
                -lglfw3 \
                -lopencv_world400d \
                -lglew32 \
                -lassimp-vc140-mt
                #-lSOIL2
    }else{
        #LIBS+=F:/script/QtProject/SOIL2/Debug/SOIL2.lib
        LIBS+=-L$$LIBDIR/openGL/soil2/lib/soil2d.lib

        LIBS+= -L$$LIBDIR/openGL/glfw-3.2.1.bin.WIN64/glfw-3.2.1.bin.WIN64/lib-vc2015 \
                -L$$LIBDIR/SDL2-2.0.9/lib/x64 \
                -L$$LIBDIR/opencv/build/x64/vc14/lib \
                -L$$LIBDIR/openGL/glew-2.1.0/lib/Release/x64
                #-LD:/cxxlib/glew-2.1.0/lib/Release/x64

        LIBS+=-lSDL2 \
                -lSDL2main \
                -lglfw3dll \
                -lglfw3 \
                -lopencv_world400d \
                -lglew32
                #-lSOIL2
    }
}
unix{

}

HEADERS += \
    shader.hxx \
    camera.hxx \
    testdata.hxx \
    stb_image.h \
    model.hxx \
    mesh.hxx \
    SDLProcessEvent.hxx \
    import_opengl3.hxx

DISTFILES += \
    7.3.camera.vs \
    light.frag \
    fragment.frag \
    LightVertex.vert \
    vertex.vert
