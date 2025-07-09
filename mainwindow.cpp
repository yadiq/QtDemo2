#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
//FFmpeg 是 C 库
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 获取所有可用的摄像头信息
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (cameras.isEmpty()) {
        qDebug() << "没有找到可用的摄像头";
        return;
    }

    // 创建 QCameraViewfinder，用于显示摄像头画面
    QCameraViewfinder *viewfinder = new QCameraViewfinder(this);
    // 通过 layout 的方式将 viewfinder 嵌入
    QVBoxLayout *layout = new QVBoxLayout(ui->viewfinderContainer);
    layout->addWidget(viewfinder);

    // 使用第一个可用的摄像头
    camera = new QCamera(cameras.first(), this);
    // 将摄像头的视图设置为 QCameraViewfinder
    camera->setViewfinder(viewfinder);
    // 启动摄像头
    camera->start();
    // 创建图像捕捉对象
    imageCapture = new QCameraImageCapture(camera, this);

    // 连接拍照按钮的点击事件
    connect(ui->btnTakePhoto, &QPushButton::clicked, this, &MainWindow::takePhoto);
    connect(ui->btnGetCameraInfo, &QPushButton::clicked, this, &MainWindow::getCameraInfo);

    testFFmpeg();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete camera;
    delete imageCapture;
}

void MainWindow::takePhoto()
{
    // 弹出文件保存对话框
    QString filePath = QFileDialog::getSaveFileName(this, "保存照片", "", "Images (*.png *.jpg *.bmp)");
    if (filePath.isEmpty()) {
        return;
    }
    // 拍照并保存图像
    imageCapture->capture(filePath);
    // 显示成功消息
    QMessageBox::information(this, "拍照成功", "图片已保存：" + filePath);
}

void MainWindow::getCameraInfo()
{
    // 获取所有可用的摄像头信息
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (cameras.isEmpty()) {
        qDebug() << "没有找到可用的摄像头";
        return;
    } else {
        for (const QCameraInfo &cameraInfo : cameras) {
            qDebug() << "摄像头设备名称：" << cameraInfo.deviceName();
            qDebug() << "描述：" << cameraInfo.description();
            //qDebug() << "摄像头设备类型：" << (cameraInfo.isDefault() ? "默认摄像头" : "非默认摄像头");
            qDebug() << "摄像头位置：" << cameraInfo.position();
            qDebug() << "====================";
        }
    }
}

void MainWindow::testFFmpeg() {
    qDebug() << "FFmpeg version:" << av_version_info();
}

