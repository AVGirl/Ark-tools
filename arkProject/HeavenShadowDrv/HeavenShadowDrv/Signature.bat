rem ��������Ϊ64λ����ǩ��
rem @echo off

set iPath=%1
set iDate=%date:~0,10%

echo ���Խ�������ǩ��...

if exist "C:\Windows\CSignTool.exe" goto SIGN

echo δ�ҵ�ǩ�����ߣ�����ǩ����
goto END

:SIGN
echo.

echo �޸�ϵͳ����Ϊ 2011/11/11
date 2011/11/11

CSignTool.exe sign /r iDriverSign /f %iPath% /ac

echo �ָ�ϵͳ���ڵ� %iDate%
date %iDate%

:END

	








