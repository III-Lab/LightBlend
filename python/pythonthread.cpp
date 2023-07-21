
#include "pythonthread.h"

#define ERROR(str) QString("%1:%2 %3").arg(__FILE__).arg(__LINE__).arg(str)

PythonThread::PythonThread(QObject *parent) : QObject (parent)
{

}

PythonThread::~PythonThread()
{
    deinit();
}

bool PythonThread::PyObject_ToStringList(PyObject* obj, std::vector<std::string>& outList) {
    if (!PyList_Check(obj)) {
        return false;
    }

    Py_ssize_t size = PyList_Size(obj);
    outList.reserve(size);

    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* item = PyList_GetItem(obj, i);
        if (!item || !PyUnicode_Check(item)) {
            return false;
        }
        PyObject* utf8StrObj = PyUnicode_AsUTF8String(item);
        if (!utf8StrObj) {
            return false;
        }
        const char* utf8Str = PyBytes_AsString(utf8StrObj);
        if (!utf8Str) {
            Py_DECREF(utf8StrObj);
            return false;
        }
        outList.emplace_back(utf8Str);
        Py_DECREF(utf8StrObj);
    }

    return true;
}

// 辅助函数: 将 PyObject 转换为 QString 列表
bool PythonThread::PyObject_ToStringList(PyObject* obj, QStringList& outList) {
    if (!PyList_Check(obj)) {
        return false;
    }

    Py_ssize_t size = PyList_Size(obj);
    outList.reserve(size);

    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject* item = PyList_GetItem(obj, i);
        if (!item || !PyUnicode_Check(item)) {
            return false;
        }
        PyObject* utf8StrObj = PyUnicode_AsUTF8String(item);
        if (!utf8StrObj) {
            return false;
        }
        const char* utf8Str = PyBytes_AsString(utf8StrObj);
        if (!utf8Str) {
            Py_DECREF(utf8StrObj);
            return false;
        }
        outList.append(QString::fromUtf8(utf8Str));
        Py_DECREF(utf8StrObj);
    }

    return true;
}

// 辅助函数: 将 PyObject 转换为 UTF-8 编码的字符串
const char* PythonThread::PyUnicode_AsUTF8(PyObject* obj) {
    PyObject* utf8StrObj = PyUnicode_AsUTF8String(obj);
    if (!utf8StrObj) {
        return nullptr;
    }

    const char* utf8Str = PyBytes_AsString(utf8StrObj);
    Py_DECREF(utf8StrObj);

    return utf8Str;
}

bool PythonThread::blend(const QString &imagePath, int radius, int center_x, int center_y, QImage &detImage)
{
//    qDebug() << imagePath << radius << center_x << center_y;
    // 获取Python函数对象
    PyObject* pFun = PyObject_GetAttrString(pDetect, "blend");
    if (!(pFun && PyCallable_Check(pFun))) {
        errorStr = ERROR("Failed to get detect function");
        PyErr_Clear(); // 清除异常状态
        Py_XDECREF(pFun);
        return false;
    }

    // 创建参数元组
    PyObject* argList = PyTuple_New(4);
    if (!argList) {
        errorStr = ERROR("Failed to create argument tuple");
        Py_XDECREF(pFun);
        return false;
    }

    // 转换QString为Python的字节对象
    QByteArray byteArray = imagePath.toUtf8();
    PyObject* pystr = PyBytes_FromStringAndSize(byteArray.constData(), byteArray.size());
    if (!pystr) {
        errorStr = ERROR("Failed to create Python byte object");
        Py_XDECREF(pFun);
        Py_XDECREF(argList);
        return false;
    }

    PyTuple_SetItem(argList, 0, pystr);
    // 设置其他参数
    PyTuple_SetItem(argList, 1, PyLong_FromLong(radius));
    PyTuple_SetItem(argList, 2, PyLong_FromLong(center_x));
    PyTuple_SetItem(argList, 3, PyLong_FromLong(center_y));


    // 调用Python函数
    Py_INCREF(pFun);
    Py_INCREF(argList);
    PyObject* pRet = PyObject_CallObject(pFun, argList);

    if (!pRet) {
        PyErr_Print();
        errorStr = ERROR("Failed to call Python function");
        return false;
    }
    // 检查返回值类型
    if (!PyTuple_Check(pRet)) {
        PyErr_Print();
        errorStr = ERROR("Invalid Python return value");
        Py_DECREF(pRet);
        return false;
    }

    // 获取返回的图像数据并克隆到detImage
    PyObject* ret_array = PyTuple_GetItem(pRet, 0);
    cv::Mat frame;
    // 检查返回的数组对象类型和维度
    if (PyArray_Check(ret_array) && PyArray_NDIM(ret_array) == 3 && PyArray_TYPE(ret_array) == NPY_UINT8) {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(ret_array);
        frame = cv::Mat(PyArray_SHAPE(arr)[0], PyArray_SHAPE(arr)[1], CV_8UC3, PyArray_DATA(arr)).clone();
//        cv::imwrite("./test.jpg", frame);
//        frame.convertTo(frame, CV_8U);
        detImage = QImage(frame.data, frame.cols, frame.rows, QImage::Format_RGB888).copy().rgbSwapped();
    } else {
        errorStr = ERROR("Invalid Python return array");
        Py_DECREF(pRet);
        return false;
    }

    Py_DECREF(pystr);    // 释放pystr
    Py_DECREF(argList);  // 释放argList
    Py_DECREF(pFun);     // 释放pFun
    Py_DECREF(pRet);     // 释放pRet
    return true;
}

QString PythonThread::errorString()
{
    return errorStr;
}

bool PythonThread::initializePythonInterpreter(const std::wstring& pythonHome) {
    Py_SetPythonHome(pythonHome.c_str()); // 设置 python 环境目录
    Py_Initialize(); // 初始化

    if (Py_IsInitialized() == 0) {
        std::cerr << "Python initialization failed." << std::endl;
        return false;
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/')");                 // 设置 python 文件搜索路径
    PyRun_SimpleString("sys.path.append('./yolov5')");          // 将算法添加进 python 搜索路径

    return true;
}

bool PythonThread::importPythonModule(const std::string& moduleName, const std::string& className,
                        PyObject*& pModule, PyObject*& pClass) {
    pModule = PyImport_ImportModule(moduleName.c_str());         // 调用的文件名
    if (!pModule) {
        qDebug() << __LINE__;
//        std::cerr << "Failed to import python module." << std::endl;
        return false;
    }

    PyObject *pDict = PyModule_GetDict(pModule);                // 加载文件中的函数名、类名
    if (!pDict) {
        qDebug() << __LINE__;
//        std::cerr << "Failed to get the module dictionary." << std::endl;
        Py_XDECREF(pModule);
        return false;
    }

    pClass = PyDict_GetItemString(pDict, className.c_str());    // 获取类名
    if (!pClass) {
        qDebug() << __LINE__;
//        std::cerr << "Failed to get class name." << std::endl;
        Py_XDECREF(pModule);
        Py_XDECREF(pDict);
        return false;
    }

    return true;
}

bool PythonThread::instantiatePythonClass(PyObject* pClass, PyObject*& pDetect) {
    pDetect = PyObject_CallObject(pClass, nullptr);             // 实例化对象，相当于调用'__init__(self)',参数为null
    if (!pDetect) {
        qDebug() << __LINE__;
//        std::cerr << "Failed to instantiate the python class." << std::endl;
        Py_XDECREF(pClass);
        return false;
    }

    return true;
}

bool PythonThread::importNumPy() {
    if (_import_array() < 0) {
        qDebug() << __LINE__;
//        std::cerr << "Failed to import numpy." << std::endl;
        return false;                      // 加载 numpy 模块
    }

    return true;
}

bool PythonThread::init(const std::wstring& pythonHome, const std::string& moduleName,
                        const std::string& className) {
    if (!initializePythonInterpreter(pythonHome)) {
        errorStr = ERROR("Python initialization failed.");
        return false;
    }

    PyObject* pModule = nullptr;
    PyObject* pClass = nullptr;
    if (!importPythonModule(moduleName, className, pModule, pClass)) {
        Py_Finalize();
        errorStr = ERROR("Failed to import python module.");
        return false;
    }

    pDetect = nullptr;
    if (!instantiatePythonClass(pClass, pDetect)) {
        Py_XDECREF(pModule);
        Py_Finalize();
        errorStr = ERROR("Failed to instantiate the python class.");
        return false;
    }

    if (!importNumPy()) {
        Py_XDECREF(pDetect);
        Py_XDECREF(pClass);
        Py_XDECREF(pModule);
        Py_Finalize();
        errorStr = ERROR("Failed to import numpy.");
        return false;
    }

    return true;
}


void PythonThread::deinit()
{
    _Py_XDECREF(pDetect);
    Py_Finalize();
}
