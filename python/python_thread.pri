

HEADERS += $$PWD/pythonthread.h
SOURCES += $$PWD/pythonthread.cpp

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/include/python
INCLUDEPATH += $$PWD/include/numpy
INCLUDEPATH += $$PWD/include/opencv2
LIBS += -L$$PWD/libs             \
                      -lpython3  \
                      -lpython38 \
                      -lopencv_world452 \
