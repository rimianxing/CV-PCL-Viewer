﻿#ifndef __MY_CLOUD_H__
#define __MY_CLOUD_H__

#include <pcl/PolygonMesh.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <QFileInfo>
#include <string>
#include "Tools.h"

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

using std::string;
using std::vector;

class MyCloud {
public:
    MyCloud();
    ~MyCloud();

    bool isValid = false;

    PointCloudT::Ptr cloud;      // point cloud pointer
    pcl::PolygonMesh::Ptr mesh;  // polygon mesh pointer

    string filePath;    // dir + file name   e.g. /home/user/hello.min.ply
    string fileDir = "/";     // only dir          e.g. /home/user
    string fileName;    // only file name    e.g. hello.min.ply
    string fileSuffix;  // file name suffx   e.g. ply

    string cloudId;  // cloud id in `viewer`: "cloud-" + fileName
    string meshId;   // mesh id in `viewer`: "mesh-" + fileName

    bool hasCloud = false;
    bool hasMesh = false;

    std::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
    string curMode = "point";  // default show mode
    vector<string> supportedModes;
    int pointSize = 1;

    void setShowMode(const string& mode);
    void showCloud();
    void hideCloud();
    void showMesh();
    void hideMesh();
    void show();
    void hide();

    void init(const QFileInfo& fileInfo, bool hasCloud, bool hasMesh);

    static MyCloud getInvalidMyCloud() {
        MyCloud myCloud;
        myCloud.isValid = false;
        return myCloud;
    }
};

#endif
