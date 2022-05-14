#ifndef CT_EDITOR_H
#define CT_EDITOR_H
#include "depends.h"

/*
 * The implementation if based on line-printing, so the syntax highlight and Chinese
 * edit if hard to make. They could be fixed by printing char by char.
 * 每一次输出一行，这种设计比较难实现语法高亮和中文编辑
 * 有这力气去改不如直接用vim
 * */

class Editor {
public:
    Editor():mode(0){};
    Editor(std::string&);
    void Init(COORD&);
    void Move(int&);
    void Flush(COORD&,const bool&);  //bottom and screen
    void InitLine(COORD&);
    void Detect(COORD&,bool&);
    void FlushBottom(COORD&,const bool&);  //only bottom
    int GetMode(){
        return mode;
    }
    void Save(std::string&);
    void CommandMode(COORD&,int&);
    void InsertMode(COORD& );
    void DetectX(COORD&);
private:
    std::string file_type_;
    std::string file_data_;  //read the file to a single string
    std::vector<std::string> file_lines_;  //split the data to line data
    int lineno_;  //the begin line of print
    int oriLine;  //limit the last lineno
    int mode;     //recognize the mode
    int CursorY=1;  //coordinate
    int CursorX=1;  //coordinate
    int linenum;  //counter for line
};


#endif //CT_EDITOR_H
