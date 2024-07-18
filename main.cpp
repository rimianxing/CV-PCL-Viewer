#include <thread>
#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QLabel>
#include <QMovie>
#include <QTranslator>
using namespace std;
#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(":/other_files/zh_CN.qm");
    a.installTranslator(&translator);
    QPixmap pixmap(":/images/loading.gif");
    QSplashScreen splash(pixmap);
    QLabel label(&splash);
    QMovie mv(":/images/loading.gif");
    label.setMovie(&mv);
    mv.start();
    splash.show();
    splash.setCursor(Qt::BlankCursor);
    for(int i = 0; i < 5000; i += mv.speed())
    {
        QCoreApplication::processEvents();
        // usleep(500 * static_cast<useconds_t>(mv.speed()));
        this_thread::sleep_for(500 * chrono::microseconds(mv.speed()));

    }
    MainWindow w;
    w.setTranslator(&translator); //保存翻译器指针，用于动态翻译
    w.setWindowTitle("CV-PCL Viewer");
    w.show();
    splash.finish(&w);
    return a.exec();
}
