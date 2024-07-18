#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QTimer>
#include <QImage>
#include <QMessageBox>
#include <QTranslator>

#include <pcl/ModelCoefficients.h>
#include <pcl/PolygonMesh.h>
#include <pcl/Vertices.h>
#include <pcl/common/io.h>
#include <pcl/common/transforms.h>
#include <pcl/features/normal_3d.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/search/kdtree.h>
#include <pcl/surface/gp3.h>
#include <pcl/surface/poisson.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/common/common.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/filters/passthrough.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <boost/thread/thread.hpp>

#include <vtkAutoInit.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPNGWriter.h>
#include <vtkPointPicker.h>
#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>

#include "Tools.h"
#include "FileIO.h"
#include "MyCloud.h"

using std::map;
using std::string;
using std::vector;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setTranslator(QTranslator *translator);
    QImage gray(cv::Mat& image);//灰度化
    QImage setRGB(QImage image, int valueR, int valueG, int valueB);//调整rgb
    QImage adjustContrast(QImage image, int value);//调整对比度
    QImage adjustScaleFactor(QImage qImage, QLabel *qLabel);//调整图片比例
    QImage adjustSaturation(QImage image, int value);//调整饱和度
    QImage edgeDetector(cv::Mat& image);//边缘检测
    cv::Mat mosaic(cv::Mat& image);//马赛克
    cv::Mat QImageToMat(QImage image);
    QImage matToQImage(const cv::Mat& mat);
    QString stom(int s);//秒转分
    QImage meanFilter(cv::Mat& image);//均值滤波
    QImage srcWithEdge(cv::Mat& image, double value);//边缘+原图
    QImage gammaTransform(cv::Mat& image, double value);//gamma变换
    QImage adjustSaturation(cv::Mat& image, int value);//调整饱和度
    QImage adjustHighlight(cv::Mat& image, int value);//调整高光
    QImage adjustShadow(cv::Mat& image, int value);//调整阴影
    void createMesh(PointCloudT::Ptr cloud, pcl::PolygonMesh::Ptr mesh);

private slots:
    void onTimeout();

    void updatePosition();

    void on_actionImage_triggered();

    void on_actionSaveImage_triggered();

    void on_actionVideo_triggered();

    void on_actionTool_triggered();

    void on_actionLanguage_triggered();

    void on_actionAbout_triggered();

    void on_btnSrc_clicked();

    void on_btnGray_clicked();

    void on_btnMeanFilter_clicked();

    void on_btnEdge_clicked();

    void on_btnWith_clicked();

    void on_sliderWithScale_valueChanged(int value);

    void on_btnGamma_clicked();

    void on_btnTurnLeft_clicked();

    void on_btnTurnRight_clicked();

    void on_btnHFlip_clicked();

    void on_btnVFlip_clicked();

    void on_sliderBinarization_valueChanged(int value);

    void on_sliderLight_valueChanged(int value);

    void on_sliderContrast_valueChanged(int value);

    void on_sliderSaturation_valueChanged(int value);

    void on_sliderHighlight_valueChanged(int value);

    void on_sliderShadow_valueChanged(int value);

    void on_sliderWarm_valueChanged(int value);

    void on_btnPrev_clicked();

    void on_btnNext_clicked();

    void on_btnSaveImage_clicked();

    void on_sliderPlay_valueChanged(int value);

    void on_btnSrcVideo_clicked();

    void on_btnMosaic_clicked();

    void on_sliderScaleFactor_valueChanged(int value);

    void on_btnPlayStop_clicked();

    void on_btnBinarization_clicked();

    void on_btnGrayVideo_clicked();

    void on_btnEdgeVideo_clicked();

    void on_btnSmooth_clicked();

    void on_actionPointCloud_triggered();

    void on_actionSaveCloud_triggered();

    void on_btnSaveCloud_clicked();

    void on_sliderPointSize_valueChanged(int value);

    void on_btnPoint_clicked();

    void on_btnMeshSurface_clicked();

    void on_btnMeshWire_clicked();

    void on_btnPointMesh_clicked();

    void on_btnCreateMesh_clicked();

private:
    Ui::MainWindow *ui;
    QTranslator *m_translator;
    bool en_US = true;
    bool isStart = false;
    QStringList srcImagePathList;//图片list
    int index = 0;//图片index
    int videoType = 0;//视频操作类型
    QString srcImagePath;//当前处理图片的原图路径
    QImage processedImage;//处理后的图片
    int imageType = 0;//图片处理类型
    cv::Mat sliderImage;//滑动条调整前的图片
    QString videoPath;//视频路径
    cv::VideoCapture capture; //用来读取视频结构
    QTimer timer;//视频播放的定时器
    int speed;//调节播放速率
    int delay;//帧延迟时间
    QMessageBox customMsgBox;//关于
    QIcon iconStartPlay = QIcon(":/images/start_play.png");
    QIcon iconStopPlay = QIcon(":/images/stop_play.png");
    QString imageDir = "/";
    QString videoDir = "/";

    MyCloud myCloud;
    std::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
    FileIO fileIO;
    bool withMesh = false;
};

#endif // MAINWINDOW_H
