@ECHO OFF

SETLOCAL

set BOOST_DIR=%1
set PROJECT_DIR=%~dp0
set CUR_DIR="%cd%"
REM set URL_BOOST_HOMOSAPIENS="https://boostorg.jfrog.io/ui/native/main/release/1.71.0/source/"
REM set URL_BOOST="https://boostorg.jfrog.io/native/main/release/1.71.0/source/boost_1_71_0.zip"
IF "%BOOST_DIR%"=="" goto :show_args

IF NOT "%2"=="" set PROJECT_DIR=%2

if NOT EXIST "%BOOST_DIR%/" goto :error_boost_dir
if NOT EXIST "%PROJECT_DIR%/" goto :error_project_dir
cd /D "%BOOST_DIR%"
b2 install --prefix="%PROJECT_DIR%/ThirdParty/" --with-filesystem
goto :end

:error_boost_dir
echo "%BOOST_DIR%" is not exist.
goto :end

:error_project_dir
echo "%PROJECT_DIR%" is not exist.
goto :end

:show_args
echo %0 "boost_dir"  "project_dir"(optional)
echo e.g. %0 "T:/boost_1_71_0"
goto :end

:end
cd /D "%CUR_DIR%"

set set BOOST_DIR=
set PROJECT_DIR=
set CUR_DIR=
set URL_BOOST=

echo End of batch program.