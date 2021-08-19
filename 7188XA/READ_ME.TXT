
===========================================
 Для процессора I7188(D):

 В файле mmicon.c

 закомментировать строки:

//#include"7188xa.h"
//#include"x607.h"

 раскомментировать строки:

#include"7188.h"
#define I7188D

в файле 2.bat

tcc -ml -M -Ic:\tc\include -Lc:\tc\lib MMICON.C 7188l.lib 7188xal.lib  > 1.RPT


===========================================
 Для процессора I7188XA:

 В файле mmicon.c

 закомментировать строки:

//#include"7188.h"
//#define I7188D

 раскомментировать строки:

#include"7188xa.h"
#include"x607.h"

в файле 2.bat
tcc -ml -M -Ic:\tc\include -Lc:\tc\lib MMICON.C 7188xal.lib x607.lib  > 1.RPT


