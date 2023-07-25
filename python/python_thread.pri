
CONFIG += release

HEADERS += $$PWD/pythonthread.h
SOURCES += $$PWD/pythonthread.cpp


contains(QMAKE_CC, cl) {
    INCLUDEPATH += $$PWD/Python/include                \
                   $$PWD/Python/include/python         \
                   $$PWD/Python/include/numpy          \
                   $$PWD/OpenCV/msvc64/include         \
                   $$PWD/OpenCV/msvc64/include/opencv2 \

    LIBS += -L$$PWD/Python/libs -lpython3 \
                                -lpython38
    LIBS += -L$$PWD/OpenCV/msvc64/libs -lopencv_world452
}

contains(QMAKE_CC, gcc) {

    INCLUDEPATH += $$PWD/Python/include                \
                   $$PWD/Python/include/python         \
                   $$PWD/Python/include/numpy          \
                   $$PWD/OpenCV/mingw64/include         \
                   $$PWD/OpenCV/mingw64/include/opencv2 \

    LIBS += -L$$PWD/Python/libs -lpython3 \
                                -lpython38
    LIBS += -L$$PWD/OpenCV/mingw64/libs -llibopencv_world452
}

## 依赖的头文件路径，使用条件判断选择不同的目录
#win32:CONFIG(release, debug|release): {
#    INCLUDEPATH += $$PWD/Python/include                \
#                   $$PWD/Python/include/python         \
#                   $$PWD/Python/include/numpy          \
#                   $$PWD/OpenCV/msvc64/include         \
#                   $$PWD/OpenCV/msvc64/include/opencv2 \
#} else {
#    INCLUDEPATH += $$PWD/Python/include                \
#                   $$PWD/Python/include/python         \
#                   $$PWD/Python/include/numpy          \
#                   $$PWD/OpenCV/mingw64/include         \
#                   $$PWD/OpenCV/mingw64/include/opencv2 \
#}

## 依赖的静态链接库，使用条件判断选择不同的库文件
#win32:CONFIG(release, debug|release): {
#    LIBS += -L$$PWD/Python/libs -lpython3 \
#                                -lpython38
#    LIBS += -L$$PWD/OpenCV/msvc64/libs -lopencv_world452
#} else {
#    LIBS += -L$$PWD/Python/libs -lpython3 \
#                                -lpython38
#    LIBS += -L$$PWD/OpenCV/msvc64/libs -llibopencv_world452
#}

