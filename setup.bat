@echo off
setlocal enabledelayedexpansion

REM Check if winget is installed
winget --version > nul 2>&1
if !errorlevel! neq 0 (
    set /p "install_winget=winget is not installed. Do you want to install winget? (Y/N): "
    if /i "!install_winget!"=="Y" (
        REM Download the MSIX installer for winget
        powershell -command "(New-Object System.Net.WebClient).DownloadFile('https://github.com/microsoft/winget-cli/releases/latest/download/Microsoft.DesktopAppInstaller_8wekyb3d8bbwe.msixbundle', 'winget.msixbundle')"

        REM Install winget
        powershell -command "Add-AppxPackage -Path '.\winget.msixbundle'"
        echo Waiting for winget installation to complete...
        timeout /t 10 /nobreak > nul
        del winget.msixbundle
        echo winget has been installed.

        REM Add winget to system PATH
        setx PATH "%PATH%;C:\Program Files\WindowsApps\Microsoft.DesktopAppInstaller_8wekyb3d8bbwe" /M
    ) else (
        echo winget will not be installed.
    )
)

REM Check if Git is installed
git --version > nul 2>&1
if !errorlevel! neq 0 (
    set /p "install_git=Git is not installed. Do you want to install Git? (Y/N): "
    if /i "!install_git!"=="Y" (
        REM Install Git using Windows Package Manager (winget)
        winget install --id Git.Git -e --source winget

        REM Add Git to system PATH
        setx PATH "%PATH%;C:\Program Files\Git\cmd" /M
    ) else (
        echo Git will not be installed.
    )
)

REM Check if vcpkg is installed
vcpkg version > nul 2>&1
if !errorlevel! neq 0 (
    set /p "install_vcpkg=vcpkg is not installed. Do you want to install vcpkg? (Y/N): "
    if /i "!install_vcpkg!"=="Y" (
        REM Step 1: Clone the vcpkg repo
        echo Cloning vcpkg repository...
        cd C:\
        git clone https://github.com/Microsoft/vcpkg.git

        REM Step 2: Run the bootstrap script to build vcpkg
        cd vcpkg
        echo Running vcpkg bootstrap script...
        call bootstrap-vcpkg.bat

        REM Add vcpkg to system PATH
        setx PATH "%PATH%;%CD%" /M
    ) else (
        echo vcpkg will not be installed.
    )
) else (
    echo vcpkg is already installed.
)

REM Using vcpkg with MSBuild / Visual Studio
echo Integrating vcpkg with MSBuild / Visual Studio...
call vcpkg integrate install

REM Check if CMake is installed
cmake --version > nul 2>&1
if !errorlevel! neq 0 (
    set /p "install_cmake=CMake is not installed. Do you want to install CMake? (Y/N): "
    if /i "!install_cmake!"=="Y" (
        REM Download and install CMake using the Windows Installer
        echo CMake is not installed. Downloading and installing CMake...
        powershell -command "(New-Object System.Net.WebClient^).DownloadFile('https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3-windows-x86_64.msi', 'cmake_installer.msi'^)"
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
set /p "install_vscode=Do you want to install Visual Studio Code? (Y/N): "
if /i "!install_vscode!"=="Y" (
    echo Installing Visual Studio Code...
    REM Download VS Code installer using PowerShell
    powershell -command "Invoke-WebRequest -Uri 'https://code.visualstudio.com/sha/download?build=stable&os=win32-x64-user' -OutFile 'vscode_installer.exe'"
    start "" vscode_installer.exe
    echo Waiting for Visual Studio Code installation to complete...
    timeout /t 10 /nobreak > nul
    del vscode_installer.exe
    echo Visual Studio Code has been installed.

    REM Install Visual Studio Code extensions
    REM Modify the extensions installation as needed
    code --install-extension ms-vscode.cpptools
    code --install-extension twxs.cmake
    code --install-extension ms-vscode.cmake-tools
) else (
    echo Visual Studio Code will not be installed.
)

REM Check if Visual Studio is installed
set /p "install_vs=Do you want to install Visual Studio? (Y/N): "
if /i "!install_vs!"=="Y" (
    REM Download and install Visual Studio Community edition web installer
    echo Downloading Visual Studio Community edition web installer...
    powershell -command "(New-Object System.Net.WebClient^).DownloadFile('https://aka.ms/vs/17/release/vs_installer.exe', 'vs_installer.exe'^)"
    
    REM Run the installer with required command-line arguments
    echo Installing Visual Studio Community edition...
    start "" vs_installer.exe --quiet --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended --includeOptional --norestart
    
    echo Waiting for Visual Studio installation to complete...
    timeout /t 30 /nobreak > nul
    del vs_installer.exe
    echo Visual Studio Community edition has been installed.

    REM Install C++ development packages for Visual Studio
    REM Replace with the actual component ID
    vs_installer.exe modify --installPath "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community" --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64
) else (
    echo Visual Studio will not be installed.
)

echo Installation and integration complete.
pause
