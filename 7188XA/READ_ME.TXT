
===========================================
 ��� ������ I7188(D):

 � 䠩�� mmicon.c

 ���������஢��� ��ப�:

//#include"7188xa.h"
//#include"x607.h"

 �᪮�����஢��� ��ப�:

#include"7188.h"
#define I7188D

� 䠩�� 2.bat

tcc -ml -M -Ic:\tc\include -Lc:\tc\lib MMICON.C 7188l.lib 7188xal.lib  > 1.RPT


===========================================
 ��� ������ I7188XA:

 � 䠩�� mmicon.c

 ���������஢��� ��ப�:

//#include"7188.h"
//#define I7188D

 �᪮�����஢��� ��ப�:

#include"7188xa.h"
#include"x607.h"

� 䠩�� 2.bat
tcc -ml -M -Ic:\tc\include -Lc:\tc\lib MMICON.C 7188xal.lib x607.lib  > 1.RPT


