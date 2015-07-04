`setup.iss`: create installer .exe  
`package.bat`: create .zip with 7-Zip

Put these files in `data` directory before running `setup.iss` or `package.bat`:

* UMWP Autochanger.exe (with a space, not an underscore)
* msvcp100.dll
* msvcr100.dll
* QtCore4.dll
* QtGui4.dll
* QtNetwork4.dll
* QtXml4.dll
* imageformats/qgif4.dll
* imageformats/qjpeg4.dll
