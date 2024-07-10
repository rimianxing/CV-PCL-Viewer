#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include <QApplication>
#include <QSpinBox>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QLabel>
#include <string>
using namespace std;
#include <iostream>
#include <sstream>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTranslator>
#include <QDebug>
#include <QPaintDevice>
#include <QPainter>
#include <QImage>
#include <QtCore/qmath.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "opencv2/imgproc/imgproc_c.h"///for cvSmooth

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), speed(1), delay(0)
{
    ui->setupUi(this);          
    ui->btnPrev->setDisabled(true);
    ui->btnNext->setDisabled(true);

    //视频
    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(updatePosition()));

    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(), this->height());                     // 禁止拖动窗口大小

    QLabel *permanent = new QLabel(this);
    permanent->setObjectName("status");
    permanent->setFrameStyle(QFrame::Box|QFrame::Sunken);
    permanent->setText(tr("欢迎使用！"));
    ui->statusBar->addPermanentWidget(permanent);
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    capture.release();
}

void MainWindow::setTranslator(QTranslator *translator)
{
    this->m_translator = translator;
}

//图片居中显示, 图片大小与label大小相适应
QImage MainWindow::adjustScaleFactor(QImage qImage, QLabel *qLabel) {
    QImage image;
    QSize imageSize = qImage.size();
    QSize labelSize = qLabel->size();

    double dWidthRatio = 1.0 * imageSize.width() / labelSize.width();
    double dHeightRatio = 1.0 * imageSize.height() / labelSize.height();
    if (dWidthRatio > dHeightRatio) {
        image = qImage.scaledToWidth(labelSize.width());
    } else {
        image = qImage.scaledToHeight(labelSize.height());
    }
    return image;
}

//灰度化
QImage MainWindow::gray(cv::Mat& image) {
    cv::cvtColor(image, image, CV_BGR2GRAY);
    return matToQImage(image);
}

//均值滤波
QImage MainWindow::meanFilter(cv::Mat& image) {
    cv::blur(image, image, cv::Size(9, 9));
    return matToQImage(image);
}

//边缘检测
QImage MainWindow::edgeDetector(cv::Mat& image) {
    cv::cvtColor(image, image, CV_BGR2GRAY);
    //Canny边缘检测
    int edgeThresh = 50;
    cv::Canny(image, image, edgeThresh, edgeThresh * 3, 3);
    return matToQImage(image);
}

//边缘+原图
QImage MainWindow::srcWithEdge(cv::Mat& image, double value) {
    cv::Mat edge;
    cv::cvtColor(image, edge, CV_BGR2GRAY);
    //Canny边缘检测
    int edgeThresh = 50;
    cv::Canny(edge, edge, edgeThresh, edgeThresh * 3, 3);

    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            for (int c = 0; c < 3; ++c) {
                image.at<Vec3b>(i, j)[c] = edge.at<uchar>(i, j) * value + image.at<Vec3b>(i, j)[c] * (1 - value);
            }
        }
    }
    return matToQImage(image);
}

QImage MainWindow::gammaTransform(cv::Mat& image, double value) {
    cv::Mat lookup_table(1, 256, CV_8U);
    uchar* p = lookup_table.data;
    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, value) * 255.0);
    }
    cv::LUT(image, lookup_table, image);
    return matToQImage(image);
}

void MainWindow::on_actionImage_triggered() {
    QStringList srcImagePathList_ = QFileDialog::getOpenFileNames(this, tr("选择图片"), "/", tr("图片(*.jpg *.jpeg *.png *.bmp *.tif)"));
    if(srcImagePathList_.size() > 0){
        imageType = 0;
        ui->tabWidget->setCurrentIndex(0);
        srcImagePathList = srcImagePathList_;
        index = 0;
        QString src = srcImagePathList.at(index);
        QImage image(src);
        image = image.convertToFormat(QImage::Format_RGB888);
        QImage Image = adjustScaleFactor(image, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
        srcImagePath = src;
        processedImage = image;

        QImage images = adjustScaleFactor(image, ui->labelFirst);
        ui->labelFirst->setPixmap(QPixmap::fromImage(images));
        ui->labelFirst->setAlignment(Qt::AlignCenter);

        //状态栏显示图片路径
        QLabel *label = ui->statusBar->findChild<QLabel *>("status");
        label->setText(src);
    }
    if(srcImagePathList_.size() >= 3){
        QString src1 = srcImagePathList.at((index+1) % srcImagePathList.size());
        QImage image1(src1);
        QImage Image1 = adjustScaleFactor(image1, ui->labelSecond);
        ui->labelSecond->setPixmap(QPixmap::fromImage(Image1));
        ui->labelSecond->setAlignment(Qt::AlignCenter);

        QString src2 = srcImagePathList.at((index+2) % srcImagePathList.size());
        QImage image2(src2);
        QImage Image2 = adjustScaleFactor(image2, ui->labelThird);
        ui->labelThird->setPixmap(QPixmap::fromImage(Image2));
        ui->labelThird->setAlignment(Qt::AlignCenter);

        ui->btnPrev->setDisabled(false);
        ui->btnNext->setDisabled(false);
        ui->labelSecond->setVisible(true);
        ui->labelThird->setVisible(true);
    } else if(srcImagePathList_.size() == 2){
        QString src1 = srcImagePathList.at((index+1) % srcImagePathList.size());
        QImage image1(src1);
        QImage Image1 = adjustScaleFactor(image1, ui->labelSecond);
        ui->labelSecond->setPixmap(QPixmap::fromImage(Image1));
        ui->labelSecond->setAlignment(Qt::AlignCenter);

        ui->btnPrev->setDisabled(false);
        ui->btnNext->setDisabled(false);
        ui->labelSecond->setVisible(true);
        ui->labelThird->setVisible(false);
     } else if(srcImagePathList_.size() == 1){
        ui->btnPrev->setDisabled(true);
        ui->btnNext->setDisabled(true);
        ui->labelSecond->setVisible(false);
        ui->labelThird->setVisible(false);
    }
}

void MainWindow::on_actionTool_triggered() {
    ui->dockWidget->show();
    ui->dockWidget_2->show();
}

void MainWindow::on_actionSaveImage_triggered() {
    //要加水印
    if(ui->mark->isChecked()){
        if(!processedImage.isNull()){
            QImage markImage(":/images/water_mark.png");

            int imageWidth = processedImage.width();
            int imageHeight = processedImage.height();
            int markWidth = markImage.width();
            int markHeight = markImage.height();
            int r, g, b;

            for(int i = 0; i < markWidth; ++i) {
               for(int j = 0; j < markHeight; ++j) {
                  QColor markColor = QColor(markImage.pixel(i, j));
                  r = markColor.red();
                  g = markColor.green();
                  b = markColor.blue();

                  if(r == 0 && g == 0 && b == 0){
                      processedImage.setPixelColor(imageWidth - markWidth + i, imageHeight - markHeight + j, qRgb(255, 255, 255));
                  }
               }
            }

            //选择保存路径
            QString filename = QFileDialog::getSaveFileName(this, tr("保存图片"), "/", tr("*.jpg;; *.jpeg;; *.png;; *.bmp;; *.tif"));
            if (!filename.isEmpty()) {
                if (!(processedImage.save(filename))) { //保存图像
                    QMessageBox::warning(nullptr, tr("提示"), tr("图片保存失败！"), QMessageBox::Ok);
                    return;
                }
                ui->statusBar->showMessage(tr("图片保存成功！"));
            }
        } else {
            QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
        }
    } else { //不加水印
        if (!processedImage.isNull()) {
            //选择保存路径
            QString filename = QFileDialog::getSaveFileName(this, tr("保存图片"), "/", tr("*.jpg;; *.jpeg;; *.png;; *.bmp;; *.tif"));
            if (!filename.isEmpty()) {
                if (!(processedImage.save(filename))) { //保存图像
                    QMessageBox::warning(nullptr, tr("提示"), tr("图片保存失败！"), QMessageBox::Ok);
                    return;
                }
                ui->statusBar->showMessage(tr("图片保存成功！"));
            }
        } else {
            QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
        }
    }
}

void MainWindow::on_actionGray_triggered() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = gray(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_actionMeanFilter_triggered() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = meanFilter(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_actionEdge_triggered() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = edgeDetector(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_actionWith_triggered() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = srcWithEdge(image, 0.5);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_actionGamma_triggered() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = gammaTransform(image, 2.0);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_actionLanguage_triggered() {
    if (en_US) {
        m_translator->load(":/other_files/en_US.qm");
    } else {
        m_translator->load(":/other_files/zh_CN.qm");
    }
    en_US = !en_US;
    ui->retranslateUi(this);//重新翻译刷新界面
}

void MainWindow::on_actionAbout_triggered() {
    customMsgBox.setWindowTitle(tr("关于本软件"));
    customMsgBox.setStandardButtons(QMessageBox::Ok);
    customMsgBox.setIconPixmap(QPixmap(":/images/about.png"));
    customMsgBox.setText(tr("欢迎使用《多功能图像视频处理工具》！本软件具有简单的图像和视频处理功能。\n\n"
                            "——By rmx"));
    customMsgBox.show();
    customMsgBox.exec();
}

void MainWindow::on_btnSrc_clicked() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        QImage image(srcImagePath);
        image = image.convertToFormat(QImage::Format_RGB888);
        processedImage = image;
        QImage Image = adjustScaleFactor(image, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnGray_clicked() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = gray(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnMeanFilter_clicked() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = meanFilter(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnEdge_clicked() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = edgeDetector(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnWith_clicked() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = srcWithEdge(image, 0.5);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_sliderWithScale_valueChanged(int value) {
    double value1 = (double)value / 100;
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = srcWithEdge(image, value1);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
        ui->labelWithScale2->setText(QString::number(value1).append(":").append(QString::number(1 - value1)));
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnGamma_clicked() {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        processedImage = gammaTransform(image, 2.0);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnTurnLeft_clicked() {
    if (!processedImage.isNull()) {
        imageType = 7;
        QMatrix matrix;
        matrix.rotate(-90.0);//逆时针旋转90度
        processedImage = processedImage.transformed(matrix, Qt::FastTransformation);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnTurnRight_clicked() {
    if (!processedImage.isNull()) {
        imageType = 7;
        QMatrix matrix;
        matrix.rotate(90.0);//顺时针旋转90度
        processedImage = processedImage.transformed(matrix, Qt::FastTransformation);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnHFlip_clicked() {
    if(!processedImage.isNull()){
        imageType = 7;
        processedImage = processedImage.mirrored(true, false);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnVFlip_clicked() {
    if(!processedImage.isNull()){
        imageType = 7;
        processedImage = processedImage.mirrored(false, true);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnSaveImage_clicked() {
    //要加水印
    if(ui->mark->isChecked()){
        if(!processedImage.isNull()){
            QImage markImage(":/images/water_mark.png");

            int imageWidth = processedImage.width();
            int imageHeight = processedImage.height();
            int markWidth = markImage.width();
            int markHeight = markImage.height();
            int r, g, b;

            for(int i = 0; i < markWidth; ++i) {
               for(int j = 0; j < markHeight; ++j) {
                  QColor markColor = QColor(markImage.pixel(i, j));
                  r = markColor.red();
                  g = markColor.green();
                  b = markColor.blue();

                  if(r == 0 && g == 0 && b == 0){
                      processedImage.setPixelColor(imageWidth - markWidth + i, imageHeight - markHeight + j, qRgb(255, 255, 255));
                  }
               }
            }

            //选择保存路径
            QString filename = QFileDialog::getSaveFileName(this, tr("保存图片"), "/", tr("*.jpg;; *.jpeg;; *.png;; *.bmp;; *.tif"));
            if (!filename.isEmpty()) {
                if (!(processedImage.save(filename))) { //保存图像
                    QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
                    return;
                }
                ui->statusBar->showMessage(tr("图片保存成功！"));
            }
        } else {
            QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
        }
    } else { //不加水印
        if (!processedImage.isNull()) {
            //选择保存路径
            QString filename = QFileDialog::getSaveFileName(this, tr("保存图片"), "/", tr("*.jpg;; *.jpeg;; *.png;; *.bmp;; *.tif"));
            if (!filename.isEmpty()) {
                if (!(processedImage.save(filename))) { //保存图像
                    QMessageBox::warning(nullptr, tr("提示"), tr("图片保存失败！"), QMessageBox::Ok);
                    return;
                }
                ui->statusBar->showMessage(tr("图片保存成功！"));
            }
        } else {
            QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
        }
    }
}

void MainWindow::on_sliderBinarization_valueChanged(int value) {
    if (!srcImagePath.isEmpty()) {
        imageType = 0;
        cv::Mat image = cv::imread(srcImagePath.toStdString());
        cvtColor(image, image, CV_BGR2GRAY);
        cv::threshold(image, image, value, 255, cv::THRESH_BINARY);
        processedImage = matToQImage(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
        ui->labelBinarization2->setText(QString::number(value));
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_sliderLight_valueChanged(int value) {
    if (!processedImage.isNull()) {
        if (imageType != 1) {
            if (imageType == 0) {
                QImage src(srcImagePath);
                processedImage = src.convertToFormat(QImage::Format_RGB888);
            }
            imageType = 1;
            sliderImage = QImageToMat(processedImage);
        }
        cv::Mat image = sliderImage.clone();
        image.convertTo(image, -1, 1, value);
        processedImage = matToQImage(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
        ui->labelLight2->setText(QString::number(value));
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_sliderContrast_valueChanged(int value) {
    if (!processedImage.isNull()) {
        if (imageType != 2) {
            if (imageType == 0) {
                QImage src(srcImagePath);
                processedImage = src.convertToFormat(QImage::Format_RGB888);
            }
            imageType = 2;
            sliderImage = QImageToMat(processedImage);
        }
        cv::Mat image = sliderImage.clone();
        double alpha = double(value) / 100;
        //计算输入图像的均值和标准差
        cv::Mat mean, stddev;
        cv::meanStdDev(image, mean, stddev); //mean和stddev都是3*1的矩阵
        //调整对比度
        for (int i = 0; i < image.rows; ++i) {
            for (int j = 0; j < image.cols; ++j) {
                for (int c = 0; c < 3; ++c) {
                    image.at<Vec3b>(i, j)[c] = cv::saturate_cast<uchar>((alpha * (image.at<Vec3b>(i, j)[c] - mean.at<double>(c, 0))) + mean.at<double>(c, 0));
                }
            }
        }
        cv::normalize(image, image, 0, 255, cv::NORM_MINMAX); //归一化到0-255
        processedImage = matToQImage(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
        ui->labelContrast2->setText(QString::number(alpha));
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

QImage MainWindow::adjustSaturation(cv::Mat& image, int value) {
    float increment = float(value) / 100;
    cv::Mat temp = image.clone();
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            uchar b = image.at<Vec3b>(i, j)[0];
            uchar g = image.at<Vec3b>(i, j)[1];
            uchar r = image.at<Vec3b>(i, j)[2];
            float max = std::max(r, std::max(g, b));
            float min = std::min(r, std::min(g, b));
            float delta, value;
            float L, S, alpha;
            delta = (max - min) / 255;
            if (delta == 0)
                continue;
            value = (max + min) / 255;
            L = value / 2;
            if (L < 0.5)
                S = delta / value;
            else
                S = delta / (2 - value);
            if (increment >= 0) {
                if ((increment + S) >= 1)
                    alpha = S;
                else
                    alpha = 1 - increment;
                alpha = 1 / alpha - 1;
                image.at<Vec3b>(i, j)[2] = static_cast<uchar>(r + (r - L * 255) * alpha);
                image.at<Vec3b>(i, j)[1] = static_cast<uchar>(g + (g - L * 255) * alpha);
                image.at<Vec3b>(i, j)[0] = static_cast<uchar>(b + (b - L * 255) * alpha);
            } else {
                alpha = increment;
                image.at<Vec3b>(i, j)[2] = static_cast<uchar>(L * 255 + (r - L * 255) * (1 + alpha));
                image.at<Vec3b>(i, j)[1] = static_cast<uchar>(L * 255 + (g - L * 255) * (1 + alpha));
                image.at<Vec3b>(i, j)[0] = static_cast<uchar>(L * 255 + (b - L * 255) * (1 + alpha));
            }
        }
    }
    return matToQImage(image);
}

void MainWindow::on_sliderSaturation_valueChanged(int value) {
    if (!processedImage.isNull()) {
        if (imageType != 3) {
            if (imageType == 0) {
                QImage src(srcImagePath);
                processedImage = src.convertToFormat(QImage::Format_RGB888);
            }
            imageType = 3;
            sliderImage = QImageToMat(processedImage);
        }
        cv::Mat image = sliderImage.clone();
        processedImage = adjustSaturation(image, value);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
        ui->labelSaturation2->setText(QString::number(value));
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

QImage MainWindow::adjustHighlight(cv::Mat& image, int value) {
    // 生成灰度图
    cv::Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);
    gray.convertTo(gray, CV_32FC1);
    gray /= 255.f;

    // 确定高光区
    cv::Mat thresh = cv::Mat::zeros(gray.size(), gray.type());
    thresh = gray.mul(gray);
    // 取平均值作为阈值
    cv::Scalar t = mean(thresh);
    cv::Mat mask = cv::Mat::zeros(gray.size(), CV_8UC1);
    mask.setTo(255, thresh >= t[0]);

    // 参数设置
    int max = 4;
    float bright = value / 100.0f / max;
    float mid = 1.0f + max * bright;

    // 边缘平滑过渡
    cv::Mat midrate = cv::Mat::zeros(image.size(), CV_32FC1);
    cv::Mat brightrate = cv::Mat::zeros(image.size(), CV_32FC1);
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            if (mask.at<uchar>(i, j) == 255) {
                midrate.at<float>(i, j) = mid;
                brightrate.at<float>(i, j) = bright;
            } else {
                midrate.at<float>(i, j) = (mid - 1.0f) / t[0] * thresh.at<float>(i, j) + 1.0f;
                brightrate.at<float>(i, j) = (1.0f / t[0] * thresh.at<float>(i, j)) * bright;
            }
        }
    }

    // 高光提亮
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            for (int c = 0; c < 3; ++c) {
                float temp = pow(float(image.at<Vec3b>(i, j)[c]) / 255.f, 1.0f / midrate.at<float>(i, j)) * (1.0 / (1 - brightrate.at<float>(i, j)));
                if (temp > 1.0f) {
                    temp = 1.0f;
                }
                if (temp < 0.0f) {
                    temp = 0.0f;
                }
                uchar utemp = uchar(255 * temp);
                image.at<Vec3b>(i, j)[c] = utemp;
            }

        }
    }
    return matToQImage(image);
}

void MainWindow::on_sliderHighlight_valueChanged(int value) {
    if (!processedImage.isNull()) {
        if (imageType != 4) {
            if (imageType == 0) {
                QImage src(srcImagePath);
                processedImage = src.convertToFormat(QImage::Format_RGB888);
            }
            imageType = 4;
            sliderImage = QImageToMat(processedImage);
        }
        cv::Mat image = sliderImage.clone();
        processedImage = adjustHighlight(image, value);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
        ui->labelHighlight2->setText(QString::number(value));
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

QImage MainWindow::adjustShadow(cv::Mat& image, int value) {
    // 生成灰度图
    cv::Mat gray;
    cvtColor(image, gray, CV_BGR2GRAY);
    gray.convertTo(gray, CV_32FC1);
    gray /= 255.f;

    // 确定阴影区
    cv::Mat thresh = cv::Mat::zeros(gray.size(), gray.type());
    thresh = (1.0f - gray).mul(1.0f - gray);
    // 取平均值作为阈值
    cv::Scalar t = mean(thresh);
    cv::Mat mask = cv::Mat::zeros(gray.size(), CV_8UC1);
    mask.setTo(255, thresh >= t[0]);

    // 参数设置
    int max = 4;
    float bright = value / 100.0f / max;
    float mid = 1.0f + max * bright;

    // 边缘平滑过渡
    cv::Mat midrate = cv::Mat::zeros(image.size(), CV_32FC1);
    cv::Mat brightrate = cv::Mat::zeros(image.size(), CV_32FC1);
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            if (mask.at<uchar>(i, j) == 255) {
                midrate.at<float>(i, j) = mid;
                brightrate.at<float>(i, j) = bright;
            } else {
                midrate.at<float>(i, j) = (mid - 1.0f) / t[0] * thresh.at<float>(i, j) + 1.0f;
                brightrate.at<float>(i, j) = (1.0f / t[0] * thresh.at<float>(i, j)) * bright;
            }
        }
    }

    // 阴影提亮
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            for (int c = 0; c < 3; ++c) {
                float temp = pow(float(image.at<Vec3b>(i, j)[c]) / 255.f, 1.0f / midrate.at<float>(i, j)) * (1.0 / (1 - brightrate.at<float>(i, j)));
                if (temp > 1.0f) {
                    temp = 1.0f;
                }
                if (temp < 0.0f) {
                    temp = 0.0f;
                }
                uchar utemp = uchar(255 * temp);
                image.at<Vec3b>(i, j)[c] = utemp;
            }

        }
    }
    return matToQImage(image);
}

void MainWindow::on_sliderShadow_valueChanged(int value) {
    if (!processedImage.isNull()) {
        if (imageType != 5) {
            if (imageType == 0) {
                QImage src(srcImagePath);
                processedImage = src.convertToFormat(QImage::Format_RGB888);
            }
            imageType = 5;
            sliderImage = QImageToMat(processedImage);
        }
        cv::Mat image = sliderImage.clone();
        processedImage = adjustShadow(image, value);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
        ui->labelShadow2->setText(QString::number(value));
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_sliderWarm_valueChanged(int value) {
    if (!processedImage.isNull()) {
        if (imageType != 6) {
            if (imageType == 0) {
                QImage src(srcImagePath);
                processedImage = src.convertToFormat(QImage::Format_RGB888);
            }
            imageType = 6;
            sliderImage = QImageToMat(processedImage);
        }
        cv::Mat image = sliderImage.clone();
        for (int i = 0; i < image.rows; ++i) {
            for (int j = 0; j < image.cols; ++j) {
                image.at<Vec3b>(i, j)[0] = cv::saturate_cast<uchar>(image.at<Vec3b>(i, j)[0] - value);
                image.at<Vec3b>(i, j)[1] = cv::saturate_cast<uchar>(image.at<Vec3b>(i, j)[1] + value);
                image.at<Vec3b>(i, j)[2] = cv::saturate_cast<uchar>(image.at<Vec3b>(i, j)[2] + value);
            }
        }
        processedImage = matToQImage(image);
        QImage Image = adjustScaleFactor(processedImage, ui->labelImageShow);
        ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
        ui->labelImageShow->setAlignment(Qt::AlignCenter);
        ui->labelWarm2->setText(QString::number(value));
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开图片！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnPrev_clicked() {
    imageType = 0;
    index = index + srcImagePathList.size() - 1;
    index %= srcImagePathList.size();
    QString src = srcImagePathList.at(index);
    QImage image(src);
    image = image.convertToFormat(QImage::Format_RGB888);
    QImage Image = adjustScaleFactor(image, ui->labelImageShow);
    ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
    ui->labelImageShow->setAlignment(Qt::AlignCenter);
    srcImagePath = src;
    processedImage = image;

    QImage images = adjustScaleFactor(image, ui->labelFirst);
    ui->labelFirst->setPixmap(QPixmap::fromImage(images));
    ui->labelFirst->setAlignment(Qt::AlignCenter);

    //状态栏显示图片路径
    QLabel *label = ui->statusBar->findChild<QLabel *>("status");
    label->setText(src);

    QString src1 = srcImagePathList.at((index + srcImagePathList.size() - 1) % srcImagePathList.size());
    QImage image1(src1);
    QImage Image1 = adjustScaleFactor(image1, ui->labelSecond);
    ui->labelSecond->setPixmap(QPixmap::fromImage(Image1));
    ui->labelSecond->setAlignment(Qt::AlignCenter);

    if (srcImagePathList.size() >= 3) {
        QString src2 = srcImagePathList.at((index + srcImagePathList.size() - 2) % srcImagePathList.size());
        QImage image2(src2);
        QImage Image2 = adjustScaleFactor(image2, ui->labelThird);
        ui->labelThird->setPixmap(QPixmap::fromImage(Image2));
        ui->labelThird->setAlignment(Qt::AlignCenter);
    }
}

void MainWindow::on_btnNext_clicked() {
    imageType = 0;
    index = index + 1;
    index %= srcImagePathList.size();
    QString src = srcImagePathList.at(index);
    QImage image(src);
    image = image.convertToFormat(QImage::Format_RGB888);
    QImage Image = adjustScaleFactor(image, ui->labelImageShow);
    ui->labelImageShow->setPixmap(QPixmap::fromImage(Image));
    ui->labelImageShow->setAlignment(Qt::AlignCenter);
    srcImagePath = src;
    processedImage = image;

    QImage images = adjustScaleFactor(image, ui->labelFirst);
    ui->labelFirst->setPixmap(QPixmap::fromImage(images));
    ui->labelFirst->setAlignment(Qt::AlignCenter);

    //状态栏显示图片路径
    QLabel *label = ui->statusBar->findChild<QLabel *>("status");
    label->setText(src);

    QString src1 = srcImagePathList.at((index + 1) % srcImagePathList.size());
    QImage image1(src1);
    QImage Image1 = adjustScaleFactor(image1, ui->labelSecond);
    ui->labelSecond->setPixmap(QPixmap::fromImage(Image1));
    ui->labelSecond->setAlignment(Qt::AlignCenter);

    if (srcImagePathList.size() >= 3) {
        QString src2 = srcImagePathList.at((index + 2) % srcImagePathList.size());
        QImage image2(src2);
        QImage Image2 = adjustScaleFactor(image2, ui->labelThird);
        ui->labelThird->setPixmap(QPixmap::fromImage(Image2));
        ui->labelThird->setAlignment(Qt::AlignCenter);
    }
}

void MainWindow::on_actionVideo_triggered() {
    videoPath = QFileDialog::getOpenFileName(this, tr("选择视频"), "/", tr("视频(*.WMV *.mp4 *.rmvb *.flv)"));
    if (!videoPath.isEmpty()) {
        //打开视频文件：其实就是建立一个VideoCapture结构
        capture.open(videoPath.toStdString());
        //检测是否正常打开：成功打开时，isOpened返回ture
        if (!capture.isOpened()) {
            QMessageBox::warning(nullptr, tr("提示"), tr("打开视频失败！"), QMessageBox::Ok);
        }
        ui->tabWidget->setCurrentIndex(1);
        ui->btnPlayStop->setEnabled(true);

        //设置开始帧()
        long frameToStart = 0;
        capture.set(CAP_PROP_POS_FRAMES, frameToStart);

        //获取帧率
        double rate = capture.get(CAP_PROP_FPS);
        delay = 1000 / rate;
        timer.start(delay);
        videoType = 0;
        isStart = true;
        ui->btnPlayStop->setIcon(iconStopPlay);
    }
}

void MainWindow::on_btnSrcVideo_clicked() {
    if (!videoPath.isEmpty()) {
        videoType = 0;
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开视频！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnGrayVideo_clicked() {
    if (!videoPath.isEmpty()) {
        videoType = 1;
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开视频！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnEdgeVideo_clicked() {
    if (!videoPath.isEmpty()) {
        videoType = 2;
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开视频！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnSmooth_clicked() {
    if (!videoPath.isEmpty()) {
        videoType = 3;
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开视频！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnBinarization_clicked() {
    if (!videoPath.isEmpty()) {
        videoType = 4;
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开视频！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnMosaic_clicked() {
    if (!videoPath.isEmpty()) {
        videoType = 5;
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开视频！"), QMessageBox::Ok);
    }
}

void MainWindow::on_btnPlayStop_clicked() {
    if (isStart) {
        timer.stop();
        isStart = false;
        ui->btnPlayStop->setIcon(iconStartPlay);
    } else {
        timer.start(delay);
        isStart = true;
        ui->btnPlayStop->setIcon(iconStopPlay);
    }
}

//秒转分函数
QString MainWindow::stom(int s) {
    QString m;
    if (s / 60 == 0) {
        m = QString::number(s % 60);
    } else {
        m = QString::number(s / 60) + ":" + QString::number(s % 60);
    }
    return m;
}

//马赛克
cv::Mat MainWindow::mosaic(cv::Mat& src) {
    int width = src.rows;	//图片的高度
    int height = src.cols;	//图片的宽度

    //20*20的像素点进行填充
    int arr = 20;

    //i和j代表了矩形区域的左上角的像素坐标
    for (int i = width / 3; i < width / 1.5; i += arr) {
        for (int j = height / 3; j < height / 1.5; j += arr) {
        //对矩形区域内的每一个像素值进行遍历
            for (int k = i; k < i + arr && k < width; ++k) {
                for (int m = j; m < j + arr && m < height; ++m) {
                    for (int c = 0; c < 3; ++c) {
                        src.at<Vec3b>(k, m)[c] = src.at<Vec3b>(i, j)[c];
                    }
                }
            }
        }
    }
    return src;
}

//QImage转Mat
cv::Mat MainWindow::QImageToMat(QImage image) {
    cv::Mat mat;
    switch(image.format()) {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
            break;
        case QImage::Format_RGB888:
            mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
            cv::cvtColor(mat, mat, CV_RGB2BGR);
            break;
        case QImage::Format_Grayscale8:
        case QImage::Format_Indexed8:
            mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            break;
        default:
            break;
    }
    return mat.clone();
}

//Mat转QImage
QImage MainWindow::matToQImage(const cv::Mat& mat) {
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat.type() == CV_8UC1) {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return image.copy();
    } else if (mat.type() == CV_8UC3) { // 8-bits unsigned, NO. OF CHANNELS = 3
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    } else if (mat.type() == CV_8UC4) {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    } else {
        return QImage();
    }
}

//timer触发函数
void MainWindow::onTimeout() {
    double rate = capture.get(CAP_PROP_FPS);
    double curFrameNumber = capture.get(CAP_PROP_POS_FRAMES);
    int curTime = curFrameNumber / rate;
    long totalFrameNumber = capture.get(CAP_PROP_FRAME_COUNT);
    int totalTime = totalFrameNumber / rate;
    ui->labelPlayTime->setText(stom(curTime) + "/" + stom(totalTime));

    Mat frame;
    //读取下一帧
    if (!capture.read(frame)) {
        return;
    }

    if (videoType == 1) { //灰度化
        cvtColor(frame, frame, CV_BGR2GRAY);
    } else if (videoType == 2) { //边缘检测
        cvtColor(frame, frame, CV_BGR2GRAY);
        //Canny边缘检测
        int edgeThresh = 50;
        Canny(frame, frame, edgeThresh, edgeThresh * 3, 3);
    } else if (videoType == 3) { //高斯平滑
        GaussianBlur(frame, frame, Size(9, 9), 0, 0);
    } else if (videoType == 4) { //二值化
        cvtColor(frame, frame, CV_BGR2GRAY);
        adaptiveThreshold(frame, frame, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 31, 6);
    } else if (videoType == 5) { //马赛克
        frame = mosaic(frame);
    }

    QImage image = matToQImage(frame);
    double scale = ui->sliderScaleFactor->value() / 100.0;

    QSize qs = ui->labelVideoShow->rect().size() * scale;
    ui->labelVideoShow->setPixmap(QPixmap::fromImage(image).scaled(qs));
    ui->labelVideoShow->setAlignment(Qt::AlignCenter);
    ui->labelVideoShow->repaint();
}

//进度条随视频移动
void MainWindow::updatePosition() {
    long totalFrameNumber = capture.get(CAP_PROP_FRAME_COUNT);
    ui->sliderPlay->setMaximum(totalFrameNumber);
    long frame = capture.get(CAP_PROP_POS_FRAMES );
    ui->sliderPlay->setValue(frame);
}

void MainWindow::on_sliderPlay_valueChanged(int value) {
    capture.set(CAP_PROP_POS_FRAMES, value);
}

void MainWindow::on_sliderScaleFactor_valueChanged(int value) {
    if (!videoPath.isEmpty()) {
        QString str = QString("%1 X").arg(value / 100.0);
        ui->labelScaleFactor2->setText(str);
    } else {
        QMessageBox::warning(nullptr, tr("提示"), tr("请先打开视频！"), QMessageBox::Ok);
    }
}
