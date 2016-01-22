`setup.iss`: create installer .exe  
`package.bat`: create .zip with 7-Zip

Put these files in `data` directory before running `setup.iss` or `package.bat`:

* UMWP Autochanger.exe (with a space, not an underscore)
* redist/vcredist_x86.exe ([Visual C++ 2013 Redistributable x86](https://www.microsoft.com/en-US/download/details.aspx?id=40784))
* Qt5Core.dll
* Qt5Concurrent.dll
* Qt5Gui.dll
* Qt5Network.dll
* Qt5Widgets.dll
* Qt5Xml.dll
* imageformats/qgif.dll
* imageformats/qjpeg.dll
* platforms/qwindows.dll

All DLL must be the 32bits versions.
