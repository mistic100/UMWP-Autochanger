#ifndef CREATESHORTCUT_H
#define CREATESHORTCUT_H

#include <Windows.h>
#include <Objbase.h>
#include <atlcomcli.h>

/**********************************************************************
* Function......: CreateShortcut
* Parameters....: lpszFileName - string that specifies a valid file name
*          lpszDesc - string that specifies a description for a
                             shortcut
*          lpszShortcutPath - string that specifies a path and
                                     file name of a shortcut
* Returns.......: S_OK on success, error code on failure
* Description...: Creates a Shell link object (shortcut)
**********************************************************************/
HRESULT CreateShortcut(/*in*/ LPCTSTR lpszFileName,
                    /*in*/ LPCTSTR lpszWorkingDir,
                    /*in*/ LPCTSTR lpszShortcutPath)
{
    HRESULT hRes = E_FAIL;
    DWORD dwRet = 0;
    CComPtr<IShellLink> ipShellLink;
        // buffer that receives the null-terminated string
        // for the drive and path
    TCHAR szPath[MAX_PATH];
        // buffer that receives the address of the final
        //file name component in the path
    LPTSTR lpszFilePart;
    WCHAR wszTemp[MAX_PATH];

    // Retrieve the full path and file name of a specified file
    dwRet = GetFullPathName(lpszFileName,
                       sizeof(szPath) / sizeof(TCHAR),
                       szPath, &lpszFilePart);
    if (!dwRet)
        return hRes;

    // Get a pointer to the IShellLink interface
    hRes = CoCreateInstance(CLSID_ShellLink,
                            NULL,
                            CLSCTX_INPROC_SERVER,
                            IID_IShellLink,
                            (void**)&ipShellLink);

    if (SUCCEEDED(hRes))
    {
        // Get a pointer to the IPersistFile interface
        CComQIPtr<IPersistFile> ipPersistFile(ipShellLink);

        // Set the path to the shortcut target and add the working directory
        hRes = ipShellLink->SetPath(szPath);
        if (FAILED(hRes))
            return hRes;

        hRes = ipShellLink->SetWorkingDirectory(lpszWorkingDir);
        if (FAILED(hRes))
            return hRes;

        // IPersistFile is using LPCOLESTR, so make sure
                // that the string is Unicode
#if !defined _UNICODE
        MultiByteToWideChar(CP_ACP, 0,
                       lpszShortcutPath, -1, wszTemp, MAX_PATH);
#else
        wcsncpy_s(wszTemp, lpszShortcutPath, MAX_PATH);
#endif

        // Write the shortcut to disk
        hRes = ipPersistFile->Save(wszTemp, TRUE);
    }

    return hRes;
}

#endif // CREATESHORTCUT_H
