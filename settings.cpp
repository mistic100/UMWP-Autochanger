#include <Shlobj.h>
#include <Knownfolders.h>
#include <atlbase.h>
#include <iostream>
#include <fstream>
#include <direct.h>
#include "settings.h"
#include "createshortcut.h"
#include <QDebug>


/*
 * constructor
 */
Settings::Settings()
{
    // default configuration
    m_iState = UM_OK;
    m_bUnsaved = false;
    m_sExePath = "";
    m_sWallPath = "";
    m_sBMPPath = "";
    m_sUMVersion = "unknown";
    m_sStartLnkPath = "";
    m_iNbWallpapers = 1;
    m_iNbMonitors = 1;
    m_oWindowSize = QSize(460, 240);
    m_iDelay = 60;
    m_bMinimize = true;
    m_bCheckFiles = true;
    m_iMsgCount = 0;

    vReadXML();
    vInit();

    //vDumpConfig();
}

/*
 * destructor
 */
Settings::~Settings()
{
    vDeleteAll();
}

/*
 * display config to the console
 */
void Settings::vDumpConfig()
{
    qDebug()<<"state : "<<m_iState;
    qDebug()<<"UltraMon "<<m_sUMVersion<<" : "<<m_sExePath;
    qDebug()<<m_iNbWallpapers<<" wallpapers - "<<iNbSets()<<" sets";
    qDebug()<<m_iDelay<<" secondes - "<<iNbFiles()<<" files";
    qDebug()<<m_sExePath;
    qDebug()<<m_sWallPath;
}

/*
 * init settings
 */
void Settings::vInit()
{
    CRegKey oReg;

    // SEARCH ULTRAMON EXE
    if (m_sExePath.isEmpty())
    {
        wchar_t* sPath1 = (wchar_t*) malloc(256);
        DWORD dwLen = 256;
        int iResult;

        iResult = oReg.Open( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion"), KEY_READ | KEY_WOW64_64KEY );
        if (iResult == ERROR_SUCCESS)
        {
            iResult = oReg.QueryStringValue( _T("ProgramFilesDir"), sPath1, &dwLen );
            if (iResult == ERROR_SUCCESS)
            {
                 m_sExePath = QString::fromWCharArray(sPath1, dwLen-1); // remove the \0 termination
                 m_sExePath.append("\\Realtime Soft\\UltraMon\\UltraMonDesktop.exe");
            }
        }
        oReg.Close();

        free(sPath1);
    }

    if (m_sExePath.isEmpty() || !bFileExists(m_sExePath.toStdString(), false))
    {
        m_iState|= UM_NOT_INSTALLED;
    }


    // SEARCH ULTRAMON VERSION
    wchar_t* sVer = (wchar_t*) malloc(32);
    DWORD dwLen = 32;
    int iResult;

    iResult = oReg.Open( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Realtime Soft\\UltraMon"), KEY_READ | KEY_WOW64_64KEY );
    if (iResult == ERROR_SUCCESS)
    {
        iResult = oReg.QueryStringValue( _T("CurrentVersion"), sVer, &dwLen );
        if (iResult == ERROR_SUCCESS)
        {
             m_sUMVersion = QString::fromWCharArray(sVer, dwLen-1); // remove the \0 termination
        }
    }
    oReg.Close();

    free(sVer);

    if (m_sUMVersion.compare("unknown")==0)
    {
        m_iState|= UM_BAD_VERSION;
    }
    // TODO : check version


    // SEARCH BMP PATH
    wchar_t* sPath2 = (wchar_t*) malloc(256);

    iResult = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, NULL, &sPath2 );
    if (iResult == S_OK)
    {
        m_sBMPPath = QString::fromWCharArray(sPath2);
        m_sBMPPath.append("\\Realtime Soft\\UltraMon\\UltraMon Wallpaper.bmp");
    }

    CoTaskMemFree(sPath2);


    // SEARCH WALLPAPER PATH
    if (!(m_iState & UM_BAD_VERSION))
    {
        wchar_t* sPath3 = (wchar_t*) malloc(256);

        int iResult = SHGetKnownFolderPath( FOLDERID_RoamingAppData, 0, NULL, &sPath3 );
        if (iResult == S_OK)
        {
            m_sWallPath = QString::fromWCharArray(sPath3);
            m_sWallPath.append("\\Realtime Soft\\UltraMon\\"+m_sUMVersion+"\\Wallpapers\\");
        }

        CoTaskMemFree(sPath3);
    }

    // CHECK WALLPAPER FILE
    if (m_sWallPath.isEmpty())
    {
        m_iState|= UM_FILE_NOT_FOUND;
    }
    else
    {
        std::string file_src = m_sWallPath.toStdString().append("default.wallpaper");
        std::string file_dst = m_sWallPath.toStdString().append(APP_WALLPAPER_FILE);

        if (!bFileExists(file_src))
        {
            m_iState|= UM_FILE_NOT_FOUND;
        }
        else if (!bFileExists(file_dst))
        {
            // copy file
            std::ifstream ifs(file_src, std::ios::in | std::ios::binary);
            std::ofstream ofs(file_dst, std::ios::out | std::ios::binary);
            ofs<<ifs.rdbuf();
            ifs.close();
            ofs.close();
        }
    }

    // READ WALLPAPER FILE
    if (!(m_iState & UM_FILE_NOT_FOUND))
    {
        vReadNbWalls();
    }


    // SEARCH SHORTCUT FILE
    wchar_t* sPath4 = (wchar_t*) malloc(256);

    iResult = SHGetKnownFolderPath( FOLDERID_Startup, 0, NULL, &sPath4 );
    if (iResult == S_OK)
    {
        m_sStartLnkPath = QString::fromWCharArray(sPath4);
        m_sStartLnkPath.append("\\UMWP Autochanger.lnk");
    }

    CoTaskMemFree(sPath4);
}

/*
 * read nb of wallpapers and monitors from .wallpaper file
 */
void Settings::vReadNbWalls()
{
    std::string file = m_sWallPath.toStdString().append(APP_WALLPAPER_FILE);
    std::ifstream ifs(file, std::ios::in | std::ios::binary);

    ifs.ignore(7); // "UMWP",  version, activedesktop

    ifs.read((char*)&m_iNbMonitors, sizeof(DWORD)); // number of monitors

    ifs.ignore(m_iNbMonitors*sizeof(RECT) + sizeof(WALLPAPER)); // one RECT for each monitor, wallpaper style

    ifs.read((char*)&m_iNbWallpapers, sizeof(DWORD)); // number of files

    ifs.close();
}

/*
 * load contents of the settings file
 */
void Settings::vReadXML()
{
    // initialize domdocument
    QDomDocument* pDom = new QDomDocument();
    QFile* pXML = new QFile(QString::fromAscii(APP_CONFIG_FILE));

    if (!pXML->open(QIODevice::ReadOnly))
    {
        return;
    }

    pDom->setContent(pXML);

    // read settings (main) node
    QDomElement settings = pDom->documentElement();
    QDomElement setting = settings.firstChild().toElement();

    vDeleteAll();

    while (!setting.isNull())
    {
        // window node
        if (setting.tagName() == "window")
        {
            m_oWindowSize.setWidth(setting.attribute("width").toInt());
            m_oWindowSize.setHeight(setting.attribute("height").toInt());
        }
        // config node
        else if (setting.tagName() == "config")
        {
            QDomElement config = setting.firstChild().toElement();

            while (!config.isNull())
            {
                // umpath
                if (config.tagName() == "umpath")
                {
                    m_sExePath = config.text().trimmed();
                }
                // minimize
                else if (config.tagName() == "minimize")
                {
                    m_bMinimize = config.text().toInt();
                }
                // delay
                else if (config.tagName() == "delay")
                {
                    m_iDelay = config.text().toInt();
                }
                // check
                else if (config.tagName() == "check")
                {
                    m_bCheckFiles = config.text().toInt();
                }
                // msgcount
                else if (config.tagName() == "msgcount")
                {
                    m_iMsgCount = config.text().toInt();
                }

                config = config.nextSibling().toElement();
            }
        }
        // sets node
        else if (setting.tagName() == "sets")
        {
            QDomElement set = setting.firstChild().toElement();

            while (!set.isNull())
            {
                // set node
                if (set.tagName() == "set")
                {
                    oAddSet(
                            set.text().trimmed(),
                            set.attribute("name"),
                            set.attribute("active").toInt()
                        );
                }

                set = set.nextSibling().toElement();
            }
        }

        setting = setting.nextSibling().toElement();
    }

    pXML->close();

    delete pDom;
    delete pXML;

    m_bUnsaved = false;
}

/*
 * write parameters to the settings file
 */
void Settings::vWriteXML()
{
    // initialize domdocument
    QDomDocument* pDom = new QDomDocument();
    QDomProcessingInstruction header = pDom->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    pDom->appendChild(header);

    // settings (main) node
    QDomElement settings = pDom->createElement("settings");

    // window node
    QDomElement size = pDom->createElement("window");
    size.setAttribute("width", QString::number(m_oWindowSize.width()));
    size.setAttribute("height", QString::number(m_oWindowSize.height()));
    settings.appendChild(size);

    // config node
    QDomElement config = pDom->createElement("config");

    addSimpleTextNode(pDom, &config, "umpath", m_sExePath);
    addSimpleTextNode(pDom, &config, "minimize", QString::number(m_bMinimize));
    addSimpleTextNode(pDom, &config, "delay", QString::number(m_iDelay));
    addSimpleTextNode(pDom, &config, "check", QString::number(m_bCheckFiles));
    addSimpleTextNode(pDom, &config, "msgcount", QString::number(m_iMsgCount));

    settings.appendChild(config);

    // sets node
    QDomElement sets = pDom->createElement("sets");

    for (QVector<Set*>::iterator it = m_oSets.begin(); it != m_oSets.end(); ++it)
    {
        Set* poSet = *it;

        // set node
        QDomElement set = pDom->createElement("set");
        set.setAttribute("name", poSet->name());
        set.setAttribute("active", poSet->isActive());
        setDomNodeValue(pDom, &set, poSet->path());

        sets.appendChild(set);
    }

    settings.appendChild(sets);

    pDom->appendChild(settings);

    // save file
    QFile* pXML = new QFile(QString::fromAscii(APP_CONFIG_FILE));
    pXML->open(QIODevice::WriteOnly);

    QTextStream stream(pXML);
    stream<<pDom->toString();

    pXML->close();

    delete pDom;
    delete pXML;

    m_bUnsaved = false;
}

/*
 * change the path of UltraMonDesktop.exe
 */
bool Settings::bSetExePath(const QString &_sPath)
{
    if (bFileExists(_sPath.toStdString()))
    {
        QString filename = _sPath.section('\\', -1);
        if (filename.compare("UltraMonDesktop.exe")==0)
        {
            m_sExePath = _sPath;
            m_bUnsaved = true;
            m_iState&= ~UM_NOT_INSTALLED;
            return true;
        }
    }

    return false;
}

/*
 * add a set from path
 */
Set* Settings::oAddSet(QString _sPath)
{
    return oAddSet(_sPath, sGetDirName(_sPath), true);
}

/*
 * add a set
 */
Set* Settings::oAddSet(QString _sPath, QString _sName, bool _bActive)
{
    vAddTrailingSlash(&_sPath);

    if (!bDirectoryExists(_sPath.toStdString()))
    {
        return NULL;
    }

    m_bUnsaved = true;

    Set* poSet = new Set(_sPath, _sName, _bActive);
    m_oSets.push_back(poSet);

    return poSet;
}

/*
 * delete a set
 */
void Settings::vDeleteSet(short int _i)
{
    if ( _i < m_oSets.size())
    {
        m_bUnsaved = true;

        Set* poSet = m_oSets.at(_i);
        m_oSets.remove(_i);
        delete poSet;
    }
}

void Settings::vDeleteSets(const QList<int> &_list)
{
    for (QList<int>::const_iterator i=_list.begin(); i!=_list.end(); i++)
    {
        vDeleteSet(*i);
    }
}

/*
 * delete all sets
 */
void Settings::vDeleteAll()
{
    if (!m_oSets.empty())
    {
        m_bUnsaved = true;

        for (int i=0; i<m_oSets.size(); i++)
        {
            delete m_oSets.at(i);
        }

        m_oSets.clear();
    }
}

/*
 * rename a set
 */
QString Settings::sRenameSet(short int _i, const QString &_name, bool _returnFull)
{
    if ( _i < m_oSets.size())
    {
        m_bUnsaved = true;

        Set* poSet = m_oSets.at(_i);
        poSet->vSetName(_name);

        if (_returnFull)
        {
            return poSet->sGetFullName();
        }
        else
        {
            return poSet->name();
        }
    }

    return "";
}

/*
 * set the state of a set
 */
void Settings::vSetState(short int _i, bool _a)
{
    m_bUnsaved = true;
    m_oSets.at(_i)->vSetActive(_a);
}

/*
 * remove unexisting sets and update file lists
 */
void Settings::vUpdateSets()
{
    for (QVector<Set*>::iterator it=m_oSets.begin(); it!=m_oSets.end(); )
    {
        if (!bDirectoryExists((*it)->path().toStdString()))
        {
            m_bUnsaved = true;
            m_oSets.erase(it);
        }
        else
        {
            (*it)->vPopulateFiles();
            it++;
        }
    }
}

/*
 * get total files among active sets
 */
int const Settings::iNbFiles()
{
    int iTotalFiles = 0;
    for (QVector<Set*>::iterator it=m_oSets.begin(); it!=m_oSets.end(); ++it)
    {
        if ((*it)->isActive())
        {
            iTotalFiles+= (*it)->count();
        }
    }
    return iTotalFiles;
}

/*
 * create the startup shortcut
 */
void Settings::vCreateShortcut()
{
    if (!m_sStartLnkPath.isEmpty())
    {
        wchar_t* sPath = (wchar_t*) malloc(256);
        wchar_t* sPathE = (wchar_t*) malloc(256);

        GetModuleFileName(NULL, sPath, 256);
        wcscpy(sPathE, sPath);
        PathRemoveFileSpec(sPathE);

        CreateShortcut(sPath, sPathE, (LPCWSTR)m_sStartLnkPath.utf16());

        free(sPath);
        free(sPathE);
    }
}

/*
 * delete the startup shortcut
 */
void Settings::vDeleteShortcut()
{
    if (bIsAutostart())
    {
        remove(m_sStartLnkPath.toStdString().c_str());
    }
}
