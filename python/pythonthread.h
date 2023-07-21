#ifndef PYTHONTHREAD_H
#define PYTHONTHREAD_H


#include <QThread>
#include <QImage>
#include <QMutex>
#include <QQueue>
#include <QDebug>
#include <QTimer>

#include <string>
#include <iostream>
#include <vector>

extern "C"
{
#include <python/Python.h>
#include <numpy/ndarrayobject.h>
}

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
class PythonThread : public QObject
{
    Q_OBJECT
public:
    explicit PythonThread(QObject *parent = nullptr);
    ~PythonThread();

    bool init(const std::wstring& pythonHome, const std::string& moduleName,
                            const std::string& className);
    bool blend(const QString &imagePath, int radius, int center_x, int center_y, QImage &detImage);
private:
    bool initializePythonInterpreter(const std::wstring& pythonHome);
    bool importPythonModule(const std::string& moduleName, const std::string& className,
                            PyObject*& pModule, PyObject*& pClass);
    bool instantiatePythonClass(PyObject* pClass, PyObject*& pDetect);
    bool importNumPy();
    bool PyObject_ToStringList(PyObject* obj, std::vector<std::string>& outList);
    bool PyObject_ToStringList(PyObject* obj, QStringList& outList);
    const char* PyUnicode_AsUTF8(PyObject* obj);
    void deinit();

public slots:
    QString errorString();

private:
    QString errorStr;
    PyObject *pDetect = nullptr;   /* python检测图像类 */
};


#endif /* PYTHONTHREAD_H */
