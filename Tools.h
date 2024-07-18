#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <QString>
#include <QTime>
#include <string>
#include <vector>

using std::string;
using std::vector;

class Tools {
public:
    Tools();
    ~Tools();
};

string getFileName(string file_name);
void timeStart();
QString timeOff();

// string to QString
QString toQString(const string& s);
// QString to string
string fromQString(const QString& qs);

string joinStrVec(const vector<string> v, string splitor = " ");
#endif
