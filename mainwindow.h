#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void takePhoto();
    void getCameraInfo();

private:
    Ui::MainWindow *ui;
    QCamera *camera;
    QCameraImageCapture *imageCapture;

};
#endif // MAINWINDOW_H
