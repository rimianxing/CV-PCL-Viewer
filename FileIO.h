#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include <pcl/io/obj_io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/io/vtk_io.h>
#include <pcl/io/vtk_lib_io.h>  // loadPolygonFileOBJ

#include <QFileInfo>
#include <map>
#include <string>
#include <vector>

#include "MyCloud.h"

using std::map;
using std::string;
using std::vector;

class FileIO {
public:
    MyCloud load(const QFileInfo& fileInfo);
    MyCloud loadPLY(const QFileInfo& fileInfo);
    MyCloud loadPCD(const QFileInfo& fileInfo);
    MyCloud loadOBJ(const QFileInfo& fileInfo);
    bool save(const MyCloud& myCloud, const QFileInfo& fileInfo, bool isBinaryFormat);
    bool savePLY(const MyCloud& myCloud, const QFileInfo& fileInfo, bool isBinaryFormat);
    bool savePCD(const MyCloud& myCloud, const QFileInfo& fileInfo, bool isBinaryFormat);
    bool saveOBJ(const MyCloud& myCloud, const QFileInfo& fileInfo);
    string getInputFormatsStr() const;
    string getOutputFormatsStr() const;

    map<string, string> inputFiltersMap = {
        {"ply", "Stanford Polygon File Format (*.ply)"},
        {"pcd", "PCL Point Cloud Data (*.pcd)"},
        {"obj", "Alias Wavefront Object (*.obj)"}};

    map<string, string> outputFiltersMap = {
        {"ply", "Stanford Polygon File Format (*.ply)"},
        {"pcd", "PCL Point Cloud Data (*.pcd)"},
        {"obj", "Alias Wavefront Object (*.obj)"}};
};

#endif
