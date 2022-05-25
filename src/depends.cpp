#include "depends.h"
std::vector<std::string> split(std::string& a, char& stp) {
    std::vector<std::string> tmp;
    std::stringstream ip(a);
    std::string tps;
    while (std::getline(ip,tps,stp)){
        tmp.push_back(tps);
    }
    return tmp;
}
std::string GetType(const std::string& name){
    std::string rtS;
    rtS=name.substr(name.find_last_of('.'));
    return rtS;
}
std::unordered_set<std::string> InitCppType(){
    std::unordered_set<std::string> rtS{"int","bool","char","float","double","void","unsigned","long","auto","short","operator","enum"};
    return rtS;
}
std::unordered_set<std::string> InitCppRsv(){
    std::unordered_set<std::string> rtS{
            "if","else","while","throw","const","virtual","for","do","typedef","static","friend","new","extern","try","template","default"
            ,"catch","delete"
    };
    return rtS;
}
std::unordered_set<std::string> InitCppS(){
    std::unordered_set<std::string> rtS{
            "break","continue","class","return","inline","switch","public","private","protected"
    };
    return rtS;
}
void replace_all(std::string& a, std::string& b, std::string c) {
    std::string::size_type bs = b.length();
    std::string tp="$";
    for (int i = 1; i < bs; i++) {
        tp += "$";
    }
    std::string::size_type ps(0);
    for (ps = a.find(b); ps != std::string::npos; ps = a.find(b)) {
        a.replace(ps,bs, tp);
    }
    for (ps = a.find(tp); ps != std::string::npos; ps = a.find(tp)) {
        a.replace(ps, bs, c);
    }
}
void cls1(HANDLE hConsole)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}
void cls(HANDLE hConsole)
{
    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
                                    (TCHAR)' ',      // Character to write to the buffer
                                    dwConSize,       // Number of cells to write
                                    coordScreen,     // Coordinates of first cell
                                    &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
                                    csbi.wAttributes, // Character attributes to use
                                    dwConSize,        // Number of cells to set attribute
                                    coordScreen,      // Coordinates of first cell
                                    &cCharsWritten))  // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(hConsole, coordScreen);
}
//enable the virtual terminal
bool EnableVTMode(){
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return false;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return false;
    }
    return true;
}
void PrintStatusLine(const std::string& a, COORD const Size)
{
    printf(CSI "%d;1H", Size.Y);
    printf(CSI "K"); // clear the line
    std::cout<<a;
}
std::string* CppBlue(){
    auto * p=new std::string[12]{
            "int","bool","char","float","double","void","unsigned","long","auto","short","operator","enum"
    };
    return p;
}
std::string* CppPur(){
    auto * p=new std::string[18]{
            "if","else","while","throw","const","virtual","for","do","typedef","static","friend","new","extern","try","template","default"
            ,"catch","delete"
    };
    return p;
}
std::string* CppY(){
     std::string* p=new std::string[9]{
            "break","continue","class","return","inline","switch","public","private","protected"
    };
    return p;
}
/*int getRows(){
    int rt;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
    GetConsoleScreenBufferInfo(hOut, &ScreenBufferInfo);
    COORD Size;
    Size.Y = ScreenBufferInfo.srWindow.Bottom - ScreenBufferInfo.srWindow.Top + 1;
    rt=Size.Y;
    return rt;
}
int getY(){
    return getSize().Y;
}
COORD getSize(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO ScreenBufferInfo;
    GetConsoleScreenBufferInfo(hOut, &ScreenBufferInfo);
    COORD Size;
    return Size;
}bool is_utf8(const char * string)
{
    if (!string)
        return true;

    const unsigned char * bytes = (const unsigned char *)string;
    int num;

    while (*bytes != 0x00)
    {
        if ((*bytes & 0x80) == 0x00)
        {
            // U+0000 to U+007F
            num = 1;
        }
        else if ((*bytes & 0xE0) == 0xC0)
        {
            // U+0080 to U+07FF
            num = 2;
        }
        else if ((*bytes & 0xF0) == 0xE0)
        {
            // U+0800 to U+FFFF
            num = 3;
        }
        else if ((*bytes & 0xF8) == 0xF0)
        {
            // U+10000 to U+10FFFF
            num = 4;
        }
        else
            return false;

        bytes += 1;
        for (int i = 1; i < num; ++i)
        {
            if ((*bytes & 0xC0) != 0x80)
                return false;
            bytes += 1;
        }
    }

    return true;
}
UINT GetCode(std::string& filename){
    UINT rt;
    std::ifstream handle(filename);
    std::string str;
    handle>>str;
    if(is_utf8(str.c_str())){
        rt=65001;
    }else{
        rt=936;
    }
    handle.close();
    return rt;
}*/
bool is_connected(){
    DWORD flag;
    bool connection= false;
    connection= InternetGetConnectedState(&flag,0);
    return connection;
}
bool ExistWinget(){
    printf(CSI "?1049h");
    int temp= system("winget");
    std::cout<<CSI"2J";
    printf(CSI "?1049l");
    return !temp;
}
LPCWSTR String2LPCWSTR(const std::string& input){
    int len=input.length();
    int lenbf=MultiByteToWideChar(CP_ACP,0,input.c_str(),len,0,0);
    wchar_t* buffer=new wchar_t [lenbf];
    MultiByteToWideChar(CP_ACP,0,input.c_str(),len,buffer,sizeof(wchar_t)*lenbf);
    buffer[len]=0;
    return buffer;
}
bool GetWinget(){
    std::cout<<"To use the tool, I will install winget to your computer"<<std::endl;
    std::cout<<R"(The temporary installer will be download to "D:\\a.msixdunble". accept(y) or enter a new directory(should not be C:\))"<<std::endl;
    std::string temp;
    std::cin>>temp;
    if(temp=="y"){
        temp="D:\\a.msixdunble";
    }
    LPCWSTR buffer= String2LPCWSTR(temp);
    //download
    bool a=(URLDownloadToFileW(NULL,
                               L"https://github.com/microsoft/winget-cli/releases/download/v1.3.1391-preview/Microsoft.DesktopAppInstaller_8wekyb3d8bbwe.msixbundle",
                               buffer,
                               0, 0)==S_OK);
    std::cout<<"waiting: "<<ESC "[ ? 25 l"<<CSI"34m"<<CSI"s";
    int pe=0;
    while(pe<101){
        std::cout<<pe<<"/100"<<CSI"u";
    }
    std::cout<<CSI"0m";
    printf(ESC "[ ? 25 h");
    if(!a){
        std::cerr<<"error installation";
        return false;
    }
    //call the installer
    system(temp.c_str());
    return true;
}