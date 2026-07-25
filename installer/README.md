1. Use windeployqt to gather necessary DLL
  ```
  windeployqt.exe src\build\Desktop_Qt_6_11_1_MSVC2022_64bit_Release\release\UMWP_Autochanger.exe --dir installer\data --release --no-translations --no-system-d3d-compiler --no-opengl-sw --compiler-runtime
  ```
2. Copy `UMWP Autochanger.exe` in the `data` directory (with a space, not an underscore)
3. Copy ([Visual C++ Redistributable x64](https://aka.ms/vc14/vc_redist.x64.exe)) in the `redist` directory
3. Run `setup.iss` and `package.ps1`
