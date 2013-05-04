#include <Shlobj.h>
#include <Knownfolders.h>
#include <atlbase.h>
#include <iostream>
#include <fstream>
#include <direct.h>

#include "settings.h"
#include "createshortcut.h"


/*
 * constructor
 */
Settings::Settings()
{
    // default configuration
    m_iState = UM_OK;
    m_bUnsaved = false;

    m_options["umpath"] = QVariant();
    m_options["window_width"] = 430;
    m_options["window_height"] = 240;
    m_options["delay"] = 60;
    m_options["minimize"] = true;
    m_options["check"] = true;
    m_options["check_updates"] = true;
    m_options["msgcount"] = 0;

    m_env["wallpath"] = QVariant();
    m_env["bmppath"] = QVariant();
    m_env["umversion"] = "unknown";
    m_env["startlinkpath"] = QVariant();
    m_env["nb_walls"] = 1;
    m_env["nb_monitors"] = 1;

    vReadXML();
    vInit();

    //qDebug()<<m_options;
    //qDebug()<<m_env;
}

/*
 * destructor
 */
Settings::~Settings()
{
    vDeleteAll();
}


/*
 * getters
 */
bool const Settings::bParam(QString const key) const
{
    return m_options.value(key, false).toBool();
}

int const Settings::iParam(QString const key) const
{
    return m_options.value(key, 0).toInt();
}

QString const Settings::sParam(QString const key) const
{
    return m_options.value(key, "").toString();
}

bool const Settings::bEnv(QString const key) const
{
    return m_env.value(key, false).toBool();
}

int const Settings::iEnv(QString const key) const
{
    return m_env.value(key, 0).toInt();
}

QString const Settings::sEnv(QString const key) const
{
    return m_env.value(key, "").toString();
}

QSize const Settings::oWindowSize() const
{
    return QSize(m_options["window_width"].toInt(), m_options["window_height"].toInt());
}

bool const Settings::bCanAddShortcut() const
{
    return !m_env["startlinkpath"].isNull();
}

bool const Settings::bIsAutostart() const
{
    return bFileExists(m_env["startlinkpath"].toString());
}


/*
 * setters
 */
void Settings::vSetParam(QString const _key, QVariant _val)
{
    m_options[_key] = _val;
    m_bUnsaved = true;
}

void Settings::vSetWindowSize(const QSize &_size)
{
    m_options["window_width"] = _size.width();
    m_options["window_height"] = _size.height();
}

void Settings::vAddMsgCount()
{
    m_options["msgcount"] = m_options["msgcount"].toInt()+1;
}


/*
 * init environnement variables
 */
void Settings::vInit()
{
    CRegKey oReg;
    int iResult;
    DWORD dwLen;


    // SEARCH ULTRAMON EXE
    if (m_options["umpath"].isNull())
    {
        wchar_t* sPath1 = (wchar_t*) malloc(256);
        dwLen = 256;

        iResult = oReg.Open( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion"), KEY_READ | KEY_WOW64_64KEY );
        if (iResult == ERROR_SUCCESS)
        {
            iResult = oReg.QueryStringValue( _T("ProgramFilesDir"), sPath1, &dwLen );
            if (iResult == ERROR_SUCCESS)
            {
                 QString sExePath = QString::fromWCharArray(sPath1, dwLen-1); // remove the \0 termination
                 sExePath.append("\\Realtime Soft\\UltraMon\\UltraMonDesktop.exe");
                 m_options["umpath"] = sExePath;
            }
        }
        oReg.Close();

        free(sPath1);
    }

    if (m_options["umpath"].isNull() || !bFileExists(m_options["umpath"].toString(), false))
    {
        m_iState|= UM_NOT_INSTALLED;
    }


    // SEARCH ULTRAMON VERSION
    wchar_t* sVer = (wchar_t*) malloc(16);
    dwLen = 16;

    iResult = oReg.Open( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Realtime Soft\\UltraMon"), KEY_READ | KEY_WOW64_64KEY );
    if (iResult == ERROR_SUCCESS)
    {
        iResult = oReg.QueryStringValue( _T("CurrentVersion"), sVer, &dwLen );
        if (iResult == ERROR_SUCCESS)
        {
            m_env["umversion"] = QString::fromWCharArray(sVer, dwLen-1); // remove the \0 termination
        }
    }
    oReg.Close();

    free(sVer);

    if (m_env["umversion"] == "unknown")
    {
        m_iState|= UM_BAD_VERSION;
    }
    // TODO : check version


    // SEARCH BMP PATH
    wchar_t* sPath2 = (wchar_t*) malloc(256);

    iResult = SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, NULL, &sPath2 );
    if (iResult == S_OK)
    {
        QString sBMPPath = QString::fromWCharArray(sPath2);
        sBMPPath.append("\\Realtime Soft\\UltraMon\\UltraMon Wallpaper.bmp");
        m_env["bmppath"] = sBMPPath;
    }
    // TODO : error check

    CoTaskMemFree(sPath2);


    // SEARCH WALLPAPER FOLDER
    if (!(m_iState & UM_BAD_VERSION))
    {
        wchar_t* sPath3 = (wchar_t*) malloc(256);

        int iResult = SHGetKnownFolderPath( FOLDERID_RoamingAppData, 0, NULL, &sPath3 );
        if (iResult == S_OK)
        {
            QString sWallPath = QString::fromWCharArray(sPath3);
            sWallPath.append("\\Realtime Soft\\UltraMon\\"+m_env["umversion"].toString()+"\\Wallpapers\\");
            m_env["wallpath"] = sWallPath;
        }

        CoTaskMemFree(sPath3);
    }

    // CHECK WALLPAPER FILE
    if (m_env["wallpath"].isNull())
    {
        m_iState|= UM_FILE_NOT_FOUND;
    }
    else
    {
        QString file_src = m_env["wallpath"].toString().append("default.wallpaper");
        QString file_dst = m_env["wallpath"].toString().append(APP_WALLPAPER_FILE);

        if (!bFileExists(file_src))
        {
            m_iState|= UM_FILE_NOT_FOUND;
        }
        else if (!bFileExists(file_dst))
        {
            QFile::copy(file_src, file_dst);
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
        QString sStartLnkPath = QString::fromWCharArray(sPath4);
        sStartLnkPath.append("\\UMWP Autochanger.lnk");
        m_env["startlinkpath"] = sStartLnkPath;
    }

    CoTaskMemFree(sPath4);
}

/*
 * read nb of wallpapers and monitors from .wallpaper file
 */
void Settings::vReadNbWalls()
{
    QString file = m_env["wallpath"].toString().append(APP_WALLPAPER_FILE);
    std::ifstream ifs(file.toStdString(), std::ios::in | std::ios::binary);

    ifs.ignore(7); // "UMWP",  version, activedesktop

    int iNbMonitors;
    ifs.read((char*)&iNbMonitors, sizeof(DWORD)); // number of monitors
    m_env["nb_monitors"] = iNbMonitors;

    ifs.ignore(iNbMonitors*sizeof(RECT) + sizeof(WALLPAPER)); // one RECT for each monitor, wallpaper style

    int iNbWallpapers;
    ifs.read((char*)&iNbWallpapers, sizeof(DWORD)); // number of files
    m_env["nb_walls"] = iNbWallpapers;

    ifs.close();
}


/*
 * load contents of the settings file
 */
void Settings::vReadXML()
{
    // initialize domdocument
    QDomDocument oDom;
    QFile oXML(QString::fromAscii(APP_CONFIG_FILE));

    if (!oXML.open(QIODevice::ReadOnly))
    {
        vWriteXML();
        return;
    }

    oDom.setContent(oXML.readAll());

    oXML.close();

    // read settings (main) node
    QDomElement main_node = oDom.documentElement();
    QDomElement setting_node = main_node.firstChild().toElement();

    vDeleteAll();

    while (!setting_node.isNull())
    {
        // config node
        if (setting_node.tagName() == "config")
        {
            QDomElement config_node = setting_node.firstChild().toElement();

            while (!config_node.isNull())
            {
                m_options[ config_node.tagName() ] = config_node.text();

                config_node = config_node.nextSibling().toElement();
            }
        }
        // sets node
        else if (setting_node.tagName() == "sets")
        {
            QDomElement set_node = setting_node.firstChild().toElement();

            while (!set_node.isNull())
            {
                // set node
                if (set_node.tagName() == "set")
                {
                    QString sPath = set_node.text().trimmed();
                    if (bDirectoryExists(sPath))
                    {
                        oAddSet(
                            sPath,
                            set_node.attribute("name"),
                            set_node.attribute("active").toInt()
                            );
                    }
                }

                set_node = set_node.nextSibling().toElement();
            }
        }

        setting_node = setting_node.nextSibling().toElement();
    }

    m_bUnsaved = false;
}

/*
 * write parameters to the settings file
 */
void Settings::vWriteXML()
{
    // initialize domdocument
    QDomDocument oDom;
    QDomProcessingInstruction header = oDom.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    oDom.appendChild(header);

    // settings (main) node
    QDomElement main_node = oDom.createElement("settings");

    // config node
    QDomElement config_node = oDom.createElement("config");

    for (QHash<QString, QVariant>::iterator it = m_options.begin(); it != m_options.end(); ++it)
    {
        addSimpleTextNode(&oDom, &config_node, it.key(), it.value().toString());
    }

    main_node.appendChild(config_node);

    // sets node
    QDomElement sets_node = oDom.createElement("sets");

    for (QVector<Set*>::iterator it = m_oSets.begin(); it != m_oSets.end(); ++it)
    {
        Set* poSet = *it;

        // set node
        QDomElement set = oDom.createElement("set");
        set.setAttribute("name", poSet->name());
        set.setAttribute("active", poSet->isActive());
        setDomNodeValue(&oDom, &set, poSet->path());

        sets_node.appendChild(set);
    }

    main_node.appendChild(sets_node);

    oDom.appendChild(main_node);

    // save file
    QFile oXML(QString::fromAscii(APP_CONFIG_FILE));
    oXML.open(QIODevice::WriteOnly);

    QTextStream stream(&oXML);
    oDom.save(stream, 2);

    oXML.close();

    m_bUnsaved = false;
}


/*
 * change the path of UltraMonDesktop.exe
 */
bool Settings::bSetExePath(const QString &_sPath)
{
    if (bFileExists(_sPath))
    {
        QString filename = _sPath.section('\\', -1);
        if (filename.compare("UltraMonDesktop.exe")==0)
        {
            vSetParam("umpath", _sPath);
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
Set* Settings::oAddSet(QString _sPath, QString &_sName, bool _bActive)
{
    vAddTrailingSlash(&_sPath);

    if (!bDirectoryExists(_sPath))
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
void Settings::vDeleteSet(int _i)
{
    if ( _i < m_oSets.size())
    {
        m_bUnsaved = true;

        Set* poSet = m_oSets.at(_i);
        m_oSets.remove(_i);
        delete poSet;
    }
}

/*
 * delete a list of sets
 */
void Settings::vDeleteSets(QList<int> _list)
{
    qSort(_list);
    int off=0;
    for (QList<int>::iterator i=_list.begin(); i!=_list.end(); i++)
    {
        vDeleteSet(*i-off);
        off++;
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
void Settings::vRenameSet(int _i, QString const &_sName)
{
    if ( _i < m_oSets.size())
    {
        m_bUnsaved = true;

        Set* poSet = m_oSets.at(_i);
        poSet->vSetName(_sName);
    }
}

/*
 * set the state of a set
 */
void Settings::vSetState(int _i, bool _a)
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
        if (!bDirectoryExists((*it)->path()))
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
    if (!m_env["startlinkpath"].isNull())
    {
        wchar_t* sPath = (wchar_t*) malloc(256);
        wchar_t* sPathE = (wchar_t*) malloc(256);

        GetModuleFileName(NULL, sPath, 256);
        wcscpy(sPathE, sPath);
        PathRemoveFileSpec(sPathE);

        CreateShortcut(sPath, sPathE, (LPCWSTR)m_env["startlinkpath"].toString().utf16());

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
        QFile::remove(m_env["startlinkpath"].toString());
    }
}
