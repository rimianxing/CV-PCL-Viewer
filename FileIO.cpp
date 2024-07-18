#include "FileIO.h"
#include <QString>
#include "Tools.h"

MyCloud FileIO::loadPLY(const QFileInfo& fileInfo) {
    MyCloud myCloud;
    string filePath = fromQString(fileInfo.filePath());
    int status = -1;
    status = pcl::io::loadPLYFile<PointT>(filePath, *(myCloud.cloud));
    pcl::io::loadPolygonFilePLY(filePath, *(myCloud.mesh));
    bool hasCloud = (status == 0);
    bool hasMesh = (myCloud.mesh->polygons.size() != 0);
    myCloud.init(fileInfo, hasCloud, hasMesh);
    return myCloud;
}

MyCloud FileIO::loadPCD(const QFileInfo& fileInfo) {
    MyCloud myCloud;
    string filePath = fromQString(fileInfo.filePath());
    int status = -1;
    status = pcl::io::loadPCDFile<PointT>(filePath, *(myCloud.cloud));
    bool hasCloud = (status == 0);
    // There is no polygon mesh loader for pcd file in PCL
    bool hasMesh = false;
    myCloud.init(fileInfo, hasCloud, hasMesh);
    return myCloud;
}

MyCloud FileIO::loadOBJ(const QFileInfo& fileInfo) {
    MyCloud myCloud;
    string filePath = fromQString(fileInfo.filePath());
    int status = -1;
    status = pcl::io::loadOBJFile<PointT>(filePath, *(myCloud.cloud));
    pcl::io::loadPolygonFileOBJ(filePath, *(myCloud.mesh));
    bool hasCloud = (status == 0);
    bool hasMesh = (myCloud.mesh->polygons.size() != 0);
    myCloud.init(fileInfo, hasCloud, hasMesh);
    return myCloud;
}

MyCloud FileIO::load(const QFileInfo& fileInfo) {
    string suffix = fromQString(fileInfo.suffix().toLower());
    if (suffix == "ply") {
        return loadPLY(fileInfo);
    } else if (suffix == "pcd") {
        return loadPCD(fileInfo);
    } else if (suffix == "obj") {
        return loadOBJ(fileInfo);
    } else {
        return MyCloud::getInvalidMyCloud();
    }
}

bool FileIO::savePLY(const MyCloud& myCloud, const QFileInfo& fileInfo, bool isBinaryFormat) {
    string filePath = fromQString(fileInfo.filePath());
    return pcl::io::savePolygonFilePLY(filePath, *(myCloud.mesh), isBinaryFormat);
}

bool FileIO::saveOBJ(const MyCloud& myCloud, const QFileInfo& fileInfo) {
    string filePath = fromQString(fileInfo.filePath());
    // pcl::io::saveOBJFile() does not support binary format
    int status = pcl::io::saveOBJFile(filePath, *(myCloud.mesh));
    return status == 0;
}

// There is no pcl::io::savePolygonFilePCD, so mesh can not be saved.
bool FileIO::savePCD(const MyCloud& myCloud, const QFileInfo& fileInfo, bool isBinaryFormat) {
    string filePath = fromQString(fileInfo.filePath());
    int status = pcl::io::savePCDFile(filePath, *(myCloud.cloud), isBinaryFormat);
    return status == 0;
}

bool FileIO::save(const MyCloud& myCloud, const QFileInfo& fileInfo, bool isBinaryFormat) {
    string suffix = fromQString(fileInfo.suffix().toLower());
    if (suffix == "ply") {
        return savePLY(myCloud, fileInfo, isBinaryFormat);
    } else if (suffix == "obj") {
        return saveOBJ(myCloud, fileInfo);
    } else if (suffix == "pcd") {
        return savePCD(myCloud, fileInfo, isBinaryFormat);
    } else {
        return false;
    }
}

string FileIO::getInputFormatsStr() const {
    vector<string> suffixVec;
    vector<string> formats = {"placeholder"};
    for (auto it = inputFiltersMap.begin(); it != inputFiltersMap.end(); ++it) {
        suffixVec.push_back("*." + it->first);
        formats.push_back(it->second);
    }
    formats[0] = "All Supported Formats (" + joinStrVec(suffixVec, " ") + ")";
    return joinStrVec(formats, ";;");
}

string FileIO::getOutputFormatsStr() const {
    vector<string> formats;
    for (auto it = outputFiltersMap.begin(); it != outputFiltersMap.end(); ++it) {
        formats.push_back(it->second);
    }
    return joinStrVec(formats, ";;");
}
