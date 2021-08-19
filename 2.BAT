SET PATH=C:\TC\BIN;C:\TC\INCLUDE;C:\PE2;
REM dos_win dos_win.c dos_winw.c
REM dos_win AVT_CTRL.C AVT_CTRw.C
tcc -ml -M -N -Ic:\tc\include -Lc:\tc\lib SNS.C MOD1.C MOD2.C 7188xal.lib > 1.RPT
rem del *.obj
REM pe2.bat 1.rpt
