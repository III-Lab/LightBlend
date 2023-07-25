#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mview.h"
#include "mscene.h"
#include "observermitem.h"

#include "./python/pythonthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QPushButton;
class QDebug;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void itemDockWidgetInit();

    QStringList getItemsList(const QString &filepath) const;
    QString getItemPixmap(const QString &name) const;

    void showImage(const QString &filepath);


signals:
    void sigShowBlendImage(const QImage &image);

private slots:

    void on_btnOpenDir_clicked();

    void on_btnChangeSaveDir_clicked();

    void on_btnNextImage_clicked();

    void on_btnPrevImage_clicked();

    void on_btnSave_clicked();
public slots:
    void slotGetMaskCenter(QPointF,int);

private:
    Ui::MainWindow *ui;
    MScene *pScene;
    MView *view;
    QGraphicsPixmapItem* pixmapItem;

    QString mSaveDir;
    QString mOpenDir;

    PythonThread *pythonBlend;

    QImage saveImage;

};
#endif // MAINWINDOW_H
