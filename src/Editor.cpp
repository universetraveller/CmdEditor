#include "Editor.h"
Editor::Editor(std::string & filename):mode(0) {
    std::ifstream file(filename);
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file_data_= std::move(data);
    file_type_= GetType(filename);
    file.close();
}

void Editor::Init(COORD& Size) {
    bool fSuccess = EnableVTMode();
    if (!fSuccess)
    {
        printf("Unable to enter VT processing mode. Quitting.\n");
        std::cerr<<"init failed";
    }
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        printf("Couldn't get the console handle. Quitting.\n");
        std::cerr<<"get handle failed";
    }
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
    GetConsoleScreenBufferInfo(hOut, &ScreenBufferInfo);


    // Enter the alternate buffer
    printf(CSI "?1049h");
    // Clear screen, tab stops, set, stop at columns 16, 32
    printf(CSI "1;1H");
    printf(CSI "2J"); // Clear screen


    // Set scrolling margins to 1, h-1
    printf(CSI "1;%dr", Size.Y - 1);

    printf(CSI "1;1H");
    printf(CSI"94m");
    for(int i=0;i<Size.Y-1;i++ ){
        std::cout<<"~"<<"\n";
    }
    printf(CSI"0m");
    printf(CSI "1;1H");
#ifdef __DETECT_TAB__
    /*const*/ std::string b="\t";
    /*const*/ std::string c="        ";
    replace_all(file_data_,b,c);
#endif
#ifndef __HIGHLIGHT__
    std::cout<<file_data_;
#else
    //Syntax Highlight with bug
    if(file_type_==".txt"){
        std::cout<<file_data_;
    }else if (file_type_==".cpp"||file_type_==".c"){
        //BUG: 检测的是特定字符，所以单词里的相同字串也会高亮
        //有办法修复，但是懒得修
        int i;
        auto blue=CppBlue();
        auto pur=CppPur();
        auto ye=CppY();
        for(i=0;i<12;i++){
            replace_all(file_data_,*blue,CSI"34m"+(*blue)+CSI"0m");
            blue++;
        }
        for(i=0;i<18;i++){
            replace_all(file_data_,*pur,CSI"35m"+(*pur)+CSI"0m");
            pur++;
        }
        for(i=0;i<9;i++){
            replace_all(file_data_,*ye,CSI"33m"+(*ye)+CSI"0m");
            ye++;
        }
        blue-=12;
        pur-=18;
        ye-=9;
        delete []blue;
        delete []pur;
        delete []ye;

        std::cout<<file_data_;
    }else{
        //other formats are waiting for update
        std::cout<<file_data_;

    }
#endif
    this->InitLine(Size);
}
void Editor::Move(int &ip) {
    switch (ip) {
        case 104:
            printf(CSI"1D");
            this->mode=10;
            break;
            //104:h:left
        case 106:
            printf(CSI"1B");
            this->mode=11;
            break;
            //106:j:down
        case 107:
            printf(CSI"1A");
            this->mode=12;
            break;
            //107:k:up
        case 108:
            static int Iter;
            Iter=lineno_+CursorY-2;
            if(CursorX<=file_lines_[Iter].size()) {
                printf(CSI"1C");
                this->mode = 13;
            }
            break;
            //108:l:right
        case 58:
            this->mode=1;
            break;
            //command m
        case 105:
            this->mode=2;
            //insert m
        default:
            break;
    }
}
void Editor::Flush(COORD& Size,const bool& flush) {
    if(!flush){
        return;
    }
    printf(CSI "1;1H");
    std::string Hinfo="";
    switch (mode) {
        case 0:
            Hinfo="Read Only";
            break;
        case 1:
            Hinfo=":";
            break;
        case 2:
            Hinfo="--INSERT--";
            break;
        default:
            Hinfo="Default";
            break;
    }
    Hinfo+="                                ";
    Hinfo+=std::to_string(linenum);
    Hinfo+='-';
    Hinfo+=std::to_string(file_lines_.size());
    int iter=lineno_-1;
    int lineC=1;
    for(;lineC<=Size.Y-2;iter++,lineC++){
        std::cout<<CSI"2K"<<file_lines_[iter]<<"\n";
    }
    printf(CSI "105;30m");
    PrintStatusLine(Hinfo, Size);
    printf(CSI "0m");
    Hinfo=CSI+ std::to_string(CursorY)+";"+ std::to_string(CursorX)+"H";
    std::cout<<Hinfo;
    //oriLine=file_lines_.size()-Size.Y+3;
}
void Editor::InitLine(COORD& Size) {
    char splC='\n';
    file_lines_= split(file_data_,splC);
    /*if(file_type_==".txt"){
        file_lines_.pop_back();
    }*/
    lineno_=file_lines_.size()-Size.Y+3;
    if(lineno_<1){
        lineno_=1;
    }
    oriLine=lineno_;
    linenum=oriLine;
}
void Editor::Detect(COORD& Size,bool& flush) {
    this->DetectX(Size);
    switch (mode) {
        case 0:
            break;
        case 11:
            if(CursorY<=Size.Y-3){
                CursorY++;
                linenum++;
            } else{
                if(lineno_<oriLine){
                    lineno_++;
                    linenum++;
                    flush= true;
                }
            }
            mode=0;
            break;
        case 12:
            if (CursorY>1){
                CursorY--;
                linenum--;
            } else{
                if(lineno_>1){
                    lineno_--;
                    linenum--;
                    flush= true;
                }
            }
            mode=0;
            break;
        case 10:
            if(CursorX>1){
                CursorX--;
            }
            mode=0;
            break;
        case 13:
            CursorX++;
            mode=0;
            break;
        default:
            break;
    }
}
void Editor::FlushBottom(COORD &Size, const bool &flush) {
    if(flush){
        return;
    }
    printf(CSI "1;1H");
    std::string Hinfo="";
    switch (mode) {
        case 0:
            Hinfo="Read Only";
            break;
        case 1:
            Hinfo=":";
            break;
        case 2:
            Hinfo="--INSERT--";
            break;
        default:
            Hinfo="Default";
            break;
    }
    Hinfo+="                                ";
    Hinfo+=std::to_string(linenum);
    Hinfo+='-';
    Hinfo+=std::to_string(file_lines_.size());
    printf(CSI "105;30m");
    PrintStatusLine(Hinfo, Size);
    printf(CSI "0m");
    Hinfo=CSI+ std::to_string(CursorY)+";"+ std::to_string(CursorX)+"H";
    std::cout<<Hinfo;
}
void Editor::Save(std::string& filename) {
    static std::string Command;
    Command="del /F /Q "+filename;
    const char* tempC=Command.c_str();
    system("echo. > edtTemp.txt");
    //可以用FILE*优化
    std::ofstream op("edtTemp.txt");
    if(!op.is_open()){
        std::cerr<<"save error";
    }
    for(int i=0;i<file_lines_.size();i++){
        op<<file_lines_[i]<<"\n";
    }
    op.close();
    system(tempC);
    Command="ren edtTemp.txt "+filename;
    tempC=Command.c_str();
    system(tempC);
}
void Editor::CommandMode(COORD &Size,int& exit) {
    static std::unordered_map<std::string,int> COMMANDS{
        {"q",1},{"wq",2},{"q!",3},{"w!",4},{"wq!",5},{"w",6}
    };
    const static bool fls= false;
    std::string Hinfo;
    Hinfo=CSI+ std::to_string(CursorY)+";"+ std::to_string(CursorX)+"H";
    std::string command;
    printf(CSI "%d;2H", Size.Y);
    printf(CSI "105;30m");
    std::cin>>command;
    int ValidCommand=COMMANDS.count(command);
    if(!ValidCommand){
        printf(CSI"K");
        std::cout<<"Invalid Command!";
        exit=-1;
        printf(CSI "0m");
        std::cout<<Hinfo;
        mode=0;
        return;
    }
    this->FlushBottom(Size,fls);
    printf(CSI "0m");
    std::cout<<Hinfo;
    ValidCommand=COMMANDS[command];
    switch (ValidCommand) {
        case 1:
            exit=1;
            break;
        case 2:
            exit=2;
            break;
        case 3:
            exit=1;
            break;
        case 4:
            exit=3;
            break;
        case 5:
            exit=2;
            break;
        case 6:
            exit=3;
            break;
        default:
            exit=0;
    }
    mode=0;
}
void Editor::InsertMode(COORD &Size) {
    //等待实现，时间因素
    //Only support ASCII code now
    //似乎对UTF-8的实现比较难
    //BUG:Cpp语法高亮占用字符
    const static bool flsA = true;
    const static bool flsB = false;
    std::string Hinfo="--INSERT--";
    int iter;
    int sgChar;
    while(true){
        sgChar=_getch();
        //Esc
        if(sgChar==27){
            break;
        }
        //BackSpace
        if(sgChar==8){
            iter=lineno_+CursorY-2;
            if(CursorX==1&&CursorY!=1){
                file_lines_[iter-1]+=file_lines_[iter];
                std::vector<std::string>::iterator getIter=file_lines_.begin();
                getIter+=iter;
                file_lines_.erase(getIter);
                //lineno_--;
                linenum--;
                oriLine--;
                CursorY--;
                iter--;
                CursorX=file_lines_[iter].size()+1;
                this->Flush(Size,flsA);
            }else if(CursorX!=1){
                CursorX--;
                file_lines_[iter].erase(CursorX-1,1);
                std::cout<<CSI"1D"<<CSI"s"<<CSI"1G"<<CSI"2K"<<file_lines_[iter]<<CSI"u";
            } else if(linenum!=1){
                //处理翻页
                file_lines_[iter-1]+=file_lines_[iter];
                std::vector<std::string>::iterator getIter=file_lines_.begin();
                getIter+=iter;
                file_lines_.erase(getIter);  //1
                if(linenum<=4){
                    lineno_=1;
                }else{
                    lineno_=linenum-3;
                }
                if(linenum<4){
                    CursorY=linenum-1;
                } else{
                    CursorY=3;
                }
                linenum--;
                oriLine--;

                iter--;
                CursorX=file_lines_[iter].size()+1;
                this->Flush(Size,flsA);
            }
            continue;
        }
        //Enter
        //Linux is \n Windows if \r\n
        if(sgChar==13){
            iter=lineno_+CursorY-2;
            std::vector<std::string>::iterator getIter=file_lines_.begin();
            getIter+=iter;
            getIter+=1;
            if(CursorX>=file_lines_[iter].size()+1){
                file_lines_.emplace(getIter,"\0");
                oriLine++;
                linenum++;
                CursorY++;
                CursorX=1;
            } else{
                CursorX--;  //方便后面使用，还会重置为1
                auto str1=file_lines_[iter].substr(CursorX);
                file_lines_[iter].erase(CursorX);
                file_lines_.emplace(getIter,str1);
                oriLine++;
                linenum++;
                CursorX=1;
                CursorY++;
            }
            this->Flush(Size,flsA);
            continue;
        }
        //Common Char
        if(sgChar<32||sgChar>126){
            Hinfo="--INSERT--";
            Hinfo+="Sorry, only Support ASCII now.                                ";
            Hinfo+=std::to_string(linenum);
            Hinfo+='-';
            Hinfo+=std::to_string(file_lines_.size());
            printf(CSI "%d;1H", Size.Y);
            printf(CSI "105;30m");
            PrintStatusLine(Hinfo, Size);
            printf(CSI "0m");
            Hinfo=CSI+ std::to_string(CursorY)+";"+ std::to_string(CursorX)+"H";
            std::cout<<Hinfo;
            continue;
        }
        iter=lineno_+CursorY-2;  //the line of Y
        file_lines_[iter].insert(CursorX-1,1,(char)sgChar);
        CursorX++;
        //this->Flush(Size,flsA);
        std::cout<<CSI"1C"<<CSI"s"<<CSI"1G"<<CSI"2K"<<file_lines_[iter]<<CSI"u";
    }
    mode=0;
    this->FlushBottom(Size,flsB);
}
void Editor::DetectX(COORD &Size) {
    //limit the X
    int Iter=lineno_+CursorY-2;
    int limit=file_lines_[Iter].length()+1;
    if(CursorX>limit){
        CursorX=limit;
        std::string Hinfo=CSI+ std::to_string(CursorY)+";"+ std::to_string(CursorX)+"H";
        std::cout<<Hinfo;
    }

}