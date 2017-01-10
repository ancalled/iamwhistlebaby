//
// Created by ancalled on 1/10/17.
//

#include <iostream>
#include <dirent.h>

using namespace std;

#ifndef WHISTLE_STRINGUTIL_H
#define WHISTLE_STRINGUTIL_H


#endif //WHISTLE_STRINGUTIL_H

string cut(const string &str, string fromSym, string toSym) {

    string::size_type from;
    if (fromSym.empty()) {
        from = string::npos;
    } else {
        from = str.find_last_of(fromSym);
    }

    string::size_type to;
    if (toSym.empty()) {
        to = str.length();
    } else {
        to = str.find(toSym, from != string::npos ? from : 0);
        if (to == string::npos) to = str.length();
    }

    if (from == string::npos) {
        return str.substr(0, to);
    } else {
        return str.substr(from + 1, to - from - 1);
    }
}

string find_mes(const string &fname) {
    string tmp = cut(fname, "/\\", ".");
    return cut(tmp, "", "_");
}

long fsize(FILE *pFile) {
    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);
    return lSize;
}

vector<string> listFiles(const char *folder) {
    vector<string> res;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == 0x8) {
                const string fname = ent->d_name;
                res.push_back(fname);
            }
        }
        closedir(dir);
    }

    return res;
}
