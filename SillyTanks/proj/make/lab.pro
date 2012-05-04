QT -= gui core
TARGET = lab
TEMPLATE = app

OBJECTS_DIR = ../../build/make/obj
DESTDIR = ../../build/make/bin
VPATH = ../../src

DEFINES += GL_EXT_PROTOTYPES

INCLUDEPATH += ../../src

linux-g++ {
    LIBS += -lGL -lGLU -lglut 
}
linux-g++-64 {
    LIBS += -lGL -lGLU -lglut 
}

macx {
        LIBS += -framework OpenGL -framework GLUT
}

SOURCES +=  \
types.cpp\
utils.cpp\
time.cpp\
window.cpp\
windowManager.cpp\
application.cpp\
main.cpp\
material.cpp\
light.cpp\
positionalLight.cpp\
directionalLight.cpp\
spotLight.cpp\
camera2D.cpp\
camera3D.cpp\
tgaImage.cpp\
texture.cpp\
tgaTexture.cpp\
textBox.cpp\
plyModel.cpp\
skyDome.cpp\
terrain.cpp\
scene.cpp\

HEADERS += \
defs.hpp\
types.hpp\
utils.hpp\
gl_includes.hpp\
exception.hpp\
time.hpp\
window.hpp\
windowManager.hpp\
application.hpp\
material.hpp\
light.hpp\
positionalLight.hpp\
directionalLight.hpp\
spotLight.hpp\
camera.hpp\
camera2D.hpp\
camera3D.hpp\
tgaImage.hpp\
texture.hpp\
tgaTexture.hpp\
drawable.hpp\
textBox.hpp\
plyModel.hpp\
skyDome.hpp\
terrain.hpp\
scene.hpp\
