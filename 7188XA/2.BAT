//dos_win MMICON.C MMICONW.C
dos_win dos_win.c dos_winw.c
tcc -ml -M -Ic:\tc\include -Lc:\tc\lib MMICON.C 7188xal.lib x607.lib  > 1.RPT
//del *.obj
pe2.bat 1.rpt

