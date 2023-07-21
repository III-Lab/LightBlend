#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMetaObject>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QSettings>

#include "itemlabel.h"
#include "reflect.h"
#include "mitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QStringLiteral("激光干扰光斑注入软件"));

    Reflect::registerClass<MItemMask10>();
    Reflect::registerClass<MItemMask20>();
    Reflect::registerClass<MItemMask30>();
    Reflect::registerClass<MItemMask40>();
    Reflect::registerClass<MItemMask50>();

    itemDockWidgetInit();

    QList<int> sizes1;
    sizes1 << 80000 << 25000;
    ui->splitter->setSizes(sizes1);

    pScene = new MScene(this);
    connect(pScene, &MScene::sigGetMaskCenter, this, &MainWindow::slotGetMaskCenter);
    ui->view->setScene(pScene);

    pixmapItem = new QGraphicsPixmapItem();
    pScene->addItem(pixmapItem);
    pScene->update();

    connect(ui->listView, &QListView::doubleClicked, this, [=](const QModelIndex &index){
        QString currentItemText = index.data(Qt::DisplayRole).toString();
        QString filepath = QDir(mOpenDir).filePath(currentItemText);
        showImage(filepath);

        QImage image = QImage(256,256,QImage::Format_RGB32);
        image.fill(Qt::black);
        sigShowBlendImage(image);
    });

    connect(this, &MainWindow::sigShowBlendImage, ui->displayWidget, &DisplayWidget::slotGetImage);

    connect(ui->actionShortCut_Key, &QAction::triggered, this, [=](){
        QMessageBox::information(this, "ShortCut Key", QStringLiteral("打开图片\t\t\tCtrl+O\n设置保存路径\t\t\tCtrl+D\n下一张\t\t\t\tS\n上一张\t\t\t\tW\n保存\t\t\t\tCtrl+S"));
    });

    QSettings settings("./config.ini", QSettings::IniFormat);
    QString ppath = settings.value("PythonInterpreterPath").toString();
    pythonBlend = new PythonThread;
    bool ret = pythonBlend->init(ppath.toStdWString(), "algorithm.blend", "Blend");
    if(!ret)
    {
        qDebug() << "init python failed.";
    }
    else {
        qDebug() << "init python success.";
    }
    qDebug() << "Software Version: LightBlend V1.1";
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * 初始化图元的选项栏
 */
void MainWindow::itemDockWidgetInit()
{
    QString styleSheet = "QWidget { background-color: black; }";
    ui->dockWidgetContents->setStyleSheet(styleSheet);
    QVBoxLayout *vlayout = new QVBoxLayout(ui->dockWidgetContents);
    ui->dockWidgetContents->setLayout(vlayout);

    QStringList itemList = getItemsList("./items.txt");

    int size = itemList.size();
    for (int i = 0; i < size; i++) {
        QString name = itemList.at(i);
        ItemLabel *itemLabel = new ItemLabel(name, getItemPixmap(name), ui->dockWidgetContents);
        vlayout->addWidget(itemLabel);
    }
}

/*
    从配置文件items.txt中读出所有的图元列表
*/
QStringList MainWindow::getItemsList(const QString &filepath) const
{
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);

    QStringList list;

    while (!stream.atEnd()) {
        QString item;
        stream >> item;
        list << item;
    }
    return list;
}

/*
 * 通过图元的名字获取其图片的路径
*/
QString MainWindow::getItemPixmap(const QString &name) const
{
    static QString pixmap_path = "./images/";
    static QString pixmap_suffix = ".png";
    return pixmap_path+name+pixmap_suffix;
}

void MainWindow::showImage(const QString &filepath)
{
    QImage image(filepath);
    if(image.isNull()) return;
    pScene->setSceneRect(-image.width()/2,-image.height()/2,image.width()*2, image.height()*2);
    pixmapItem->setPos(0,0);
    pixmapItem->setPixmap(QPixmap::fromImage(image));
    pScene->update();
}


#include <QFileDialog>
#include <QStandardItemModel>
void MainWindow::on_btnOpenDir_clicked()
{
    QString openFolderPath = QFileDialog::getExistingDirectory(this, "Select Image path", "", QFileDialog::ShowDirsOnly);
    if (!openFolderPath.isEmpty()) {
        qDebug() << QStringLiteral("图片路径: %1").arg(openFolderPath);
        mOpenDir = openFolderPath;
        QDir directory(openFolderPath);
        QStringList filters;
        filters << "*.jpg" << "*.png" << "*.bmp"; // 添加你需要的图片格式的过滤器
        directory.setNameFilters(filters);

        QStandardItemModel *model = new QStandardItemModel();

        QFileInfoList fileList = directory.entryInfoList();
        for (const QFileInfo &fileInfo : fileList) {
            QString absoluteFilePath = fileInfo.absoluteFilePath();
            QStandardItem *item = new QStandardItem(QIcon(absoluteFilePath), fileInfo.fileName());
            item->setEditable(false);
            model->appendRow(item);
        }
        qDebug() << QStringLiteral("图片总数: %1").arg(QString::number(model->rowCount()));
        ui->listView->setModel(model);
    }
}

void MainWindow::on_btnChangeSaveDir_clicked()
{
    QString saveFolderPath = QFileDialog::getExistingDirectory(this, "Select save folder", "", QFileDialog::ShowDirsOnly);
    if (!saveFolderPath.isEmpty()) {
        mSaveDir = saveFolderPath;
        qDebug() << QStringLiteral("图片保存路径: %1").arg(saveFolderPath);
    }
}

void MainWindow::on_btnNextImage_clicked()
{
    int index = ui->listView->currentIndex().row();
    QModelIndex modelIndex = ui->listView->model()->index(index+1, 0);
    ui->listView->setCurrentIndex(modelIndex);
    QString currentItemText = ui->listView->currentIndex().data(Qt::DisplayRole).toString();
    QString filepath = QDir(mOpenDir).filePath(currentItemText);
    showImage(filepath);
}

void MainWindow::on_btnPrevImage_clicked()
{
    int index = ui->listView->currentIndex().row();
    QModelIndex modelIndex = ui->listView->model()->index(index-1, 0);
    ui->listView->setCurrentIndex(modelIndex);
    QString currentItemText = ui->listView->currentIndex().data(Qt::DisplayRole).toString();
    QString filepath = QDir(mOpenDir).filePath(currentItemText);
    showImage(filepath);
}

void MainWindow::on_btnSave_clicked()
{
    if(mSaveDir.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Not set save the path");
        return;
    }

    QString currentItemText = ui->listView->currentIndex().data(Qt::DisplayRole).toString();
    QString filepath = QDir(mSaveDir).filePath(currentItemText);

    saveImage.save(filepath);
    qDebug() << QStringLiteral("已保存: %1").arg(filepath);
}

void MainWindow::slotGetMaskCenter(QPointF p, int r)
{
    QImage image_ret;

    int index = ui->listView->currentIndex().row();
    QModelIndex modelIndex = ui->listView->model()->index(index, 0);
    ui->listView->setCurrentIndex(modelIndex);
    QString currentItemText = ui->listView->currentIndex().data(Qt::DisplayRole).toString();
    QString filepath = QDir(mOpenDir).filePath(currentItemText);

    int ret = pythonBlend->blend(filepath, r, int(p.x()), int(p.y()), image_ret);
    if(!ret)
    {
        qDebug() << pythonBlend->errorString();
        return;
    }
    saveImage = image_ret;
    emit sigShowBlendImage(image_ret);
}
