@echo off
setlocal

REM Request elevated privileges
NET SESSION >NUL 2>NUL
if %errorlevel% neq 0 (
    echo This script requires administrator privileges. Restarting with elevated permissions...
    powershell -command "Start-Process '%0' -Verb RunAs"
    exit /B
)

REM Check if Git is installed
git --version > nul 2>&1
if %errorlevel% neq 0 (
    set /p install_git=Git is not installed. Do you want to install Git? (Y/N): 
    if /i "%install_git%"=="Y" (
        REM Download Git installer using PowerShell and BITS
        powershell -command "(New-Object System.Net.WebClient).DownloadFile('https://github.com/git-for-windows/git/releases/download/v2.33.0.windows.2/Git-2.33.0-64-bit.exe', 'GitInstaller.exe')"

        REM Install Git
        start "" GitInstaller.exe /VERYSILENT
        echo Waiting for Git installation to complete...
        timeout /t 10 /nobreak > nul
        del GitInstaller.exe
        echo Git has been installed.
    ) else (
        echo Git will not be installed.
    )
)

REM Add Git to system PATH if installed
git --version > nul 2>&1
if %errorlevel% eq 0 (
    set "git_path=C:\Program Files\Git\cmd"
    echo Adding Git to system PATH...
    setx PATH "%PATH%;%git_path%" /M
)

REM Check if vcpkg is installed
vcpkg version > nul 2>&1
if %errorlevel% neq 0 (
    set /p install_vcpkg=vcpkg is not installed. Do you want to install vcpkg? (Y/N): 
    if /i "%install_vcpkg%"=="Y" (
        REM Step 1: Clone the vcpkg repo
        echo Cloning vcpkg repository...
        git clone https://github.com/Microsoft/vcpkg.git

        REM Step 2: Run the bootstrap script to build vcpkg
        cd vcpkg
        echo Running vcpkg bootstrap script...
        .\bootstrap-vcpkg.bat
    ) else (
        echo vcpkg will not be installed.
    )
) else (
    echo vcpkg is already installed.
)

REM Install libraries for your project
echo Installing libraries for your project...
.\vcpkg.exe install [packages to install]

REM Using vcpkg with MSBuild / Visual Studio
echo Integrating vcpkg with MSBuild / Visual Studio...
.\vcpkg.exe integrate install

REM Check if CMake is installed
cmake --version > nul 2>&1
if %errorlevel% neq 0 (
    set /p install_cmake=CMake is not installed. Do you want to install CMake? (Y/N): 
    if /i "%install_cmake%"=="Y" (
        REM Download and install CMake using the Windows Installer
        echo CMake is not installed. Downloading and installing CMake...
        powershell -command "(New-Object System.Net.WebClient).DownloadFile('https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3-windows-x86_64.msi', 'cmake_installer.msi')"
        start "" msiexec /i cmake_installer.msi /quiet
        echo Waiting for CMake installation to complete...
        timeout /t 10 /nobreak > nul
        del cmake_installer.msi
        echo CMake has been installed.
    ) else (
        echo CMake will not be installed.
    )
) else (
    echo CMake is already installed.
)

REM Install Visual Studio Code using PowerShell
set /p install_vscode=Visual Studio Code is not installed. Do you want to install Visual Studio Code? (Y/N): 
if /i "%install_vscode%"=="Y" (
    echo Installing Visual Studio Code...
    powershell -command "Start-BitsTransfer -Source 'https://code.visualstudio.com/sha/download?build=stable&os=win32-x64-user' -Destination 'vscode_installer.exe'"
    start "" vscode_installer.exe
    echo Waiting for Visual Studio Code installation to complete...
    timeout /t 10 /nobreak > nul
    del vscode_installer.exe
    echo Visual Studio Code has been installed.

    REM Install Visual Studio Code extensions
    code --install-extension ms-vscode.cpptools
    code --install-extension twxs.cmake
    code --install-extension ms-vscode.cmake-tools
) else (
    echo Visual Studio Code will not be installed.
)

REM Check if Visual Studio is installed
set /p install_vs=Visual Studio is not installed. Do you want to install Visual Studio? (Y/N): 
if /i "%install_vs%"=="Y" (
    REM Download and install Visual Studio Community edition web installer
    echo Downloading Visual Studio Community edition web installer...
    powershell -command "(New-Object System.Net.WebClient).DownloadFile('https://aka.ms/vs/17/release/vs_installer.exe', 'vs_installer.exe')"
    
    REM Run the installer with required command-line arguments
    echo Installing Visual Studio Community edition...
    start "" vs_installer.exe --quiet --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended --includeOptional --norestart
    
    echo Waiting for Visual Studio installation to complete...
    timeout /t 30 /nobreak > nul
    del vs_installer.exe
    echo Visual Studio Community edition has been installed.

    REM Install C++ development packages for Visual Studio
    vs_installer.exe modify --installPath "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community" --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64
) else (
    echo Visual Studio will not be installed.
)

echo Installation and integration complete.
pause
