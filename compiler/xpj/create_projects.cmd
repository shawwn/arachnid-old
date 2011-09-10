@echo off

REM set XPJ="xpj3.exe"
set XPJ="xpj_pch.exe"


%XPJ% -v 1 -t VC8 -p WIN32 -x arachnid.xpj
%XPJ% -v 1 -t VC9 -p WIN32 -x arachnid.xpj
%XPJ% -v 1 -t VC8 -p WIN64 -x arachnid.xpj
%XPJ% -v 1 -t VC9 -p WIN64 -x arachnid.xpj


goto cleanExit

:pauseExit
pause

:cleanExit
