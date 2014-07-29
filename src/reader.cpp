#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <QString>
using namespace std;

#define PATH "C:\\Users\\Damien\\AppData\\Roaming\\Realtime Soft\\UltraMon\\3.2.1\\Wallpapers\\default.wallpaper"

enum WALLPAPER
{
    W_DESKTOP = 0,
    W_MONITOR = 1
};
enum BACKGROUND
{
    BG_SOLID = 0,
    BG_GRAD_HORZ = 1,
    BG_GRAD_VERT = 2
};
enum IMAGE
{
    IM_CENTER = 0,
    IM_TILE = 1,
    IM_STRETCH = 2,
    IM_STRETCH_PROP = 3
};
struct WP_MONITOR_FILE
{
    BACKGROUND bgType;
    COLORREF color1;
    COLORREF color2;
    IMAGE imgStyle;
    wchar_t imgFile[260];
};

int main2(int argc, char *argv[])
{
    ifstream ifs(PATH, ios::in | ios::binary);

    // signature
    char* signature = new char[5];
    ifs.read(signature, 4);
    signature[4] = '\0';
    cout<<signature<<endl;
    delete[] signature;

    // version
    short int version;
    ifs.read((char*)&version, 2);
    cout<<"version: "<<version<<endl;

    // active desktop
    bool activedesk;
    ifs.read((char*)&activedesk, 1);
    cout<<"activedesk: "<<activedesk<<endl;

    // nb monitors
    DWORD nbmonitors;
    ifs.read((char*)&nbmonitors, 4);
    cout<<"nbmonitors: "<<nbmonitors<<endl;

    // rect monitors
    for (int i=0; i<nbmonitors; i++)
    {
        RECT monitor;
        ifs.read((char*)&monitor, sizeof(RECT));
        cout<<"   "<<monitor.top<<" "<<monitor.right<<" "<<monitor.bottom<<" "<<monitor.left<<endl;
    }

    // wp style
    WALLPAPER wpstyle;
    ifs.read((char*)&wpstyle, sizeof(WALLPAPER));
    cout<<"wpstyle: "<<wpstyle<<endl;

    // nb wall
    DWORD nbwall;
    ifs.read((char*)&nbwall, 4);
    cout<<"nbwall: "<<nbwall<<endl;

    // wp_monitor_files
    cout<<endl;
    for (int i=0; i<nbwall; i++)
    {
        WP_MONITOR_FILE* wpfile = (WP_MONITOR_FILE*)malloc(sizeof(WP_MONITOR_FILE));
        ifs.read((char*)wpfile, sizeof(WP_MONITOR_FILE));

        cout<<QString::fromWCharArray(wpfile->imgFile).toStdString();
        cout<<" | type: "<<wpfile->bgType<<" color1: "<<wpfile->color1;
        cout<<" color2: "<<wpfile->color2<<" style: "<<wpfile->imgStyle<<endl;
        free(wpfile);
    }

    ifs.close();

    return 0;
}
