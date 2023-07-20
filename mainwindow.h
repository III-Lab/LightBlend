#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mview.h"
#include "mscene.h"

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

private slots:

    void on_btnOpenDir_clicked();

    void on_btnChangeSaveDir_clicked();

    void on_btnNextImage_clicked();

    void on_btnPrevImage_clicked();

    void on_btnSave_clicked();

private:
    Ui::MainWindow *ui;
    MScene *pScene;
    MView *view;
    QGraphicsPixmapItem* pixmapItem;

    QString mSaveDir;
    QString mOpenDir;

};
#endif // MAINWINDOW_H
