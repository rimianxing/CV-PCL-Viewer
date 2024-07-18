#include "MyCloud.h"
#include "pcl/conversions.h"

MyCloud::MyCloud() {
    cloud.reset(new PointCloudT);
    mesh.reset(new pcl::PolygonMesh);
}

MyCloud::~MyCloud() = default;

void MyCloud::init(const QFileInfo& fileInfo, bool hasCloudParam, bool hasMeshParam) {
    hasCloud = hasCloudParam;
    hasMesh = hasMeshParam;

    filePath = fromQString(fileInfo.filePath());
    fileDir = fromQString(fileInfo.path());
    fileName = fromQString(fileInfo.fileName());
    fileSuffix = fromQString(fileInfo.suffix());

    if (!hasCloud && !hasMesh) {
        isValid = false;
        return;
    }

    isValid = true;
    meshId = "mesh-" + fileName;
    cloudId = "cloud-" + fileName;
    if (hasMesh && !hasCloud) {
        pcl::fromPCLPointCloud2(mesh->cloud, *cloud);
        supportedModes = {"point", "mesh", "point+mesh"};
    }
    if (!hasMesh && hasCloud) {
        supportedModes = {"point"};
    }
    if (hasMesh && hasCloud) {
        supportedModes = {"point", "mesh", "point+mesh"};
    }
    // default show node
    curMode = "point";
}

void MyCloud::setShowMode(const string& mode) {
    curMode = mode;
    show();
}

void MyCloud::showCloud() {
    viewer->removePointCloud(cloudId);
    viewer->addPointCloud(cloud, cloudId);
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,
                                             pointSize, cloudId);
}

void MyCloud::hideCloud() {
    viewer->removePointCloud(cloudId);
}

void MyCloud::showMesh() {
    viewer->removePolygonMesh(meshId);
    viewer->addPolygonMesh(*mesh, meshId);
}

void MyCloud::hideMesh() {
    viewer->removePolygonMesh(meshId);
}

void MyCloud::show() {
    if (curMode == "point") {
        hideMesh();
        showCloud();
    } else if (curMode == "mesh") {
        hideCloud();
        showMesh();
    } else if (curMode == "point+mesh") {
        showCloud();
        showMesh();
    }
}

void MyCloud::hide() {
    hideCloud();
    hideMesh();
}
