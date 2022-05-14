#ifndef CT_DEPENDS_H
#define CT_DEPENDS_H
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>  //runtime is win
#include <cwchar>
#include <cstdlib>
#include <cstdio>
#include <unordered_map>

#define ESC "\x1b"
#define CSI "\x1b["
#define __DETECT_TAB__

bool EnableVTMode();
void PrintStatusLine(const std::string &, COORD const Size);
std::vector<std::string> split(std::string& a, char& stp);
std::string GetType(const std::string& name);
std::unordered_set<std::string> InitCppType();
std::unordered_set<std::string> InitCppRsv();
std::unordered_set<std::string> InitCppS();
std::string* CppBlue();
std::string* CppPur();
std::string* CppY();
//int getRows();
void replace_all(std::string& a, std::string& b, std::string c);
void cls(HANDLE);
//UINT GetCode(std::string&);
//bool is_utf8(const char*);
//COORD getSize();
#endif //CT_DEPENDS_H
