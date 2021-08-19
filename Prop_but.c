// 10 Nov 2010

#include "prop_but.h"

#define  ind_50  (20)
//-------------
  // ��砫쭠� ⥬������ � ⠡���� ������
#define Beg_temp (-50)
  // 蠣 ⥬������� � ⠡���� ������
#define Step_t (5.)

float PermPlus = 8;
float PermMinus= 8;
float t20c     =20.;
float v_Proc   =50.;
int   flag_20C =0;

float Temp_kor=0;
float Comp_kor=1.0;

// ��������� ����� �����
float M_Etan=30.068;
float M_Propan=44.097;
float M_Butan=58.124;

float Density=0;
float Temperature=0;
float Temperature1=0;
float Composition=0;

float ComposMass=0, ComposMol=0;
float CompG=0;

//----------------------------
//----------------------------

float f_get_dens()
{// �����頥� �ਢ������� � 20C ���祭�� ���⭮��, �᫨ flag_20C != 0
 // ��� -1. �� �訡��

 //  s_MVD[0].Dens - ���祭�� ���⭮��
 //  s_MVD[0].Temp - ���祭�� ⥬�������
 float ftmp;

   Temperature=s_MVD[0].Temp;
   Temperature1=Temperature;
   ftmp=s_MVD[0].Dens;
   Density = ftmp;

   ftmp=f_get_composition(ftmp,Temperature);
   if(ftmp<= (-PermMinus)) return -1; // ���⭮��� ᬥ� ����� ���⭮�� ��⠭�
   if(ftmp>= PermPlus) return -1;             // ���⭮��� ᬥ� ����� ���⭮�� �ய���

   Composition=ftmp;
   return s_MVD[0].Dens;

}
//------------------------------------
/*
void f_get_dens_c()
{// ������ � Dens0  �ਢ������� � ⥪�饩 ⥬������ ���祭�� ���⭮��, �᫨ flag_20C == 2

 //  s_MVD[0].Dens - ���祭�� ���⭮��
 //  s_MVD[0].Temp - ���祭�� ⥬�������
 float ftmp;
  if((s_MVD[0].Temp>=-50) && (s_MVD[0].Temp<=50))
  {
   Temperature1=s_MVD[0].Temp;
   Dens0 = s_MVD[0].Dens;
  }
}
*/
//------------------------------------

float f_get_composition(float Dens,float Temp)
{
// �����頥� ���ᮢ� ��⠢ �ய��-��⠭���� ᬥ�

int itmp,itmp1;
float r_propan,r_butan,ftmp;

float mProp,mBut,mlProp,mlBut;
float TempG;


 if(Dens < PropLiq[ind_50]) return -1.0;
 TempG=Temp;

 if(TempG > 50) TempG=50.;
 if(TempG < (-50)) TempG=-50.;

 // ������ ��ꥬ�� ��⠢ �ய��-��⠭���� ᬥ�
  if( TempG == 50.)
  {
   r_propan=PropLiq[ind_50];
   r_butan=ButLiq[ind_50];
  }
  else
  {
    ftmp=(TempG-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;
    // ���⭮��� ������ 䠧� �ய���,��⠭� �� ⥬������ Temp , ��/�3
    r_propan=(PropLiq[itmp1]-PropLiq[itmp]) * ftmp + PropLiq[itmp];
    r_butan =(ButLiq[itmp1] -ButLiq[itmp] ) * ftmp + ButLiq[itmp];

//  printf("\r\n itmp=%d,ftmp=%.3f,r_prop=%.3f,r_but=%.3f",itmp,ftmp,r_propan,r_butan);
  }

// ��ꥬ��� ���� �ய��� � ᬥ� 0...1.0

  Composition =(r_butan-Dens)/(r_butan-r_propan);

 // ���� �ய���, ��⠭� � 1 �3

  mProp=r_propan * Composition;
  mBut =r_butan  * (1.0-Composition);

 // ���ᮢ�� ���� �ய��� � ᬥ� 0...1.0
  ComposMass=mProp/(mProp+mBut);
/*
 // ������⢮ �������� �ய��� , ��⠭� � 1 �3 ᬥ�
  mlProp=mProp/M_Propan;
  mlBut =mBut /M_Butan ;

 // ����ୠ� ���� �ய��� � ᬥ� 0...1.0
  ComposMol=mlProp/(mlProp+mlBut);
*/
  return ComposMass;
}
//-------------
float f_get_Dns(float Compos,float Temp)
{
// �� ���ᮢ��� ��⠢� Compos (0...1.0) � ⥬������ Temp  (-50C ... 50 C)
// ������ ���⭮��� ������ 䠧� �ய��-��⠭���� ᬥ�

int itmp,itmp1;
float r_propan,r_butan,ftmp;
float TempG;


 TempG=Temp;

 if(TempG > 50) TempG=50.;
 if(TempG < (-50)) TempG=-50.;


//if((Compos > 1.0) || (Compos< 0)) return -1;

  if( TempG == 50.)
  {
   r_propan=PropLiq[ind_50];
   r_butan=ButLiq[ind_50];
  }
  else
  {
    ftmp=(TempG-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;

    r_propan=(PropLiq[itmp1]-PropLiq[itmp]) * ftmp + PropLiq[itmp];
    r_butan =(ButLiq[itmp1] -ButLiq[itmp] ) * ftmp + ButLiq[itmp];
  }
  //  r_propan, r_butan - ���⭮�� ������ 䠧 �� �������� ⥬������,��/�3
   // �᫨  Compos - ��ꥬ�� ��⠢  D= ( Compos*r_propan + (1.- Compos) * r_butan );
  return ( r_propan * r_butan / ( r_propan * (1.0-Compos) + Compos * r_butan  ) );
}
//-------------
void f_get_pressCT ( float Compos, float Temp)
{
// �� ���ᮢ��� ��⠢� ������ 䠧� � ⥬������ �����頥�
// �������� ����饭���� ���, ������ ���⭮��� � ���ᮢ� ��⠢ ��஢
// Compos - ���ᮢ�� ���� �ய��� � ᬥ� 0...1.0

int itmp,itmp1;
float r_propan,r_butan,ftmp;
float mProp,mBut,mlProp,mlBut;
float ComposM;
float d_propan,d_butan;
float TempG;

//  printf("\r\nf_get_pressCT Compos=%f,Temp=%f:",Compos,Temp);
 TempG=Temp;

 if(TempG > 50) TempG=50.;
 if(TempG < (-50)) TempG=-50.;

  if( TempG == 50.)
  {
   r_propan=PropanP[ind_50];
   r_butan=ButanP[ind_50];
   d_propan=PropGas[ind_50];
   d_butan =ButGas[ind_50];
  }
  else
  {
    ftmp=(TempG-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;

    ftmp-=(float)itmp;
    itmp1=itmp+1;
    r_propan=(PropanP[itmp1]-PropanP[itmp]) * ftmp + PropanP[itmp];
    r_butan =(ButanP[itmp1] -ButanP[itmp] ) * ftmp + ButanP[itmp];

    d_propan=(PropGas[itmp1]-PropGas[itmp]) * ftmp + PropGas[itmp];
    d_butan =(ButGas[itmp1] -ButGas[itmp] ) * ftmp + ButGas[itmp];
  }

  mlProp=Compos/M_Propan;
  mlBut =(1.- Compos) /M_Butan ;
  ComposM=mlProp/(mlProp+mlBut);

  s_MVD[0].PressG=r_butan*(1.0-ComposM)+(r_propan*ComposM) ;
  s_MVD[0].DensG =d_butan*(1.0-ComposM)+(d_propan*ComposM) ;
  // ���ᮢ� ��⠢ ����
  CompG=(d_propan*ComposM)/s_MVD[0].DensG;

  return;
}
//-------------

void f_get_pressDT(float Dens,float Temp)
{
// �� ���⭮�� � ⥬������
// �����뢠�� ���ᮢ� ��⠢ ᬥ� ������ 䠧� s_MVD[0].Compos
// �������� ����饭���� ��� s_MVD[0].PressG, ���⭮��� � ���ᮢ� ��⠢ ��஢
// s_MVD[0].Compos - ���ᮢ�� ���� �ய��� � ᬥ� 0...1.0

int itmp,itmp1;
float r_propan,r_butan,ftmp,Compos,Ttmp;
float d_propan,d_butan;
float mProp,mBut,mlProp,mlBut;

if((Temp > 50) || (Temp< -50))
 {
  s_MVD[0].PressG=10.00 ;
  s_MVD[0].DensG=1000.;
  return;
 }

  if( Temp == 50.)
  {
   r_propan=PropLiq[ind_50];
   r_butan=ButLiq[ind_50];
  }
  else
  {
    ftmp=(Temp-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;

    r_propan=(PropLiq[itmp1]-PropLiq[itmp]) * ftmp + PropLiq[itmp];
    r_butan =(ButLiq[itmp1] -ButLiq[itmp] ) * ftmp + ButLiq[itmp];
  }

// ��ꥬ��� ���� �ய��� � ᬥ� 0...1.0
  Compos = (r_butan-Dens)/(r_butan-r_propan);

 // ���� �ய���, ��⠭� � 1 �3

  mProp=r_propan * Compos;
  mBut =r_butan  * (1.0-Compos);

 // ���ᮢ�� ���� �ய��� � ᬥ� 0...1.0
  s_MVD[0].Compos=mProp/(mProp+mBut);

 // ������⢮ �������� �ய��� , ��⠭� � 1 �3 ᬥ�
  mlProp=mProp/M_Propan;
  mlBut =mBut /M_Butan ;

 // ����ୠ� ���� �ய��� � ᬥ� 0...1.0
//ComposMol=mlProp/(mlProp+mlBut);
  Compos=mlProp/(mlProp+mlBut);

    Ttmp=Temp + Temp_kor;
    if(Ttmp > 50) Ttmp=50;

  if((Compos<=1.) && (Compos>=0.))
  {
    Compos=Compos * Comp_kor;
    if(Compos>1.)  Compos=1.;

    if( Ttmp == 50.)
    {
     r_propan=PropanP[ind_50];
     r_butan=ButanP[ind_50];
     d_propan= PropGas[ind_50];
     d_butan=  ButGas[ind_50];

    }
    else
    {
    ftmp=(Temp-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;

      r_propan=(PropanP[itmp1]-PropanP[itmp]) * ftmp + PropanP[itmp];
      r_butan =(ButanP[itmp1] -ButanP[itmp] ) * ftmp + ButanP[itmp];

      d_propan=(PropGas[itmp1]-PropGas[itmp]) * ftmp + PropGas[itmp];
      d_butan =(ButGas[itmp1] -ButGas[itmp] ) * ftmp + ButGas[itmp];
    }

    s_MVD[0].PressG=r_butan*(1.0-Compos)+(r_propan*Compos) ;
    s_MVD[0].DensG=d_butan*(1.0-Compos)+(d_propan*Compos) ;
    CompG=(d_propan*Compos)/s_MVD[0].DensG;
  }
  else
  {
  /*
   if( flag_prn )
   {
    flag_prn=0;
    printf("\n\rDens=%f,Temp=%f, Compos=%f,PressG=%f",Dens,Temp,Compos,s_MVD[0].PressG);
   }
  */
    s_MVD[0].PressG=10.00 ;
    s_MVD[0].DensG=1000.;
  }
}

//-------------

float f_get_PD_Etan(float Temp)
{
// �� ⥬������
// �����뢠�� �������� ����饭���� ��� � ���⭮��� �⠭�

 float TempG;
 float ftmp;
 float P_Etan100;
 int itmp,itmp1;

 TempG=Temp;

 if(TempG > 50) TempG=50.;
 if(TempG < (-50)) TempG=-50.;

    ftmp=(TempG-(float)Beg_temp)/Step_t;
    itmp=(int)ftmp;
    ftmp-=(float)itmp;
    itmp1=itmp+1;

    D_Etan100=(EtanGas[itmp1]-EtanGas[itmp]) * ftmp + EtanGas[itmp];
    P_Etan100=(EtanP[itmp1]-EtanP[itmp]) * ftmp + EtanP[itmp];

    return P_Etan100;
}
//--------------------


//--------------------
// ������� �����
//--------------------

//---------------------------------------
//�ய��, ���⭮��� ������ 䠧�, ��/�3
//-50� ... 60�
float PropLiq[]=
{
593,     // -50
587,     // -45
581,     // -40
574.5,   // -35
568,     // -30
561.5,   // -25
555,     // -20
548.5,   // -15
542,     // -10
535,     //  -5
528,     // 0
521,     //   5
514,     //  10
506.5,   //  15
499,     //  20
491,     //  25
483,     //  30
474.25,  //  35
465.5,   //  40
455.75,  //  45
446,     //  50
437,     //  55
434,     //  60
};

//---------------------------------------
// ��⠭, ���⭮��� ������ 䠧�, ��/�3
//-50� ... 60�
float ButLiq[]=
{
647,          // -50
642.5,        // -45
638,          // -40
633.5,        // -35
629,          // -30
624.5,        // -25
620,          // -20
615.5,        // -15
611,          // -10
606,          //  -5
601,          // 0
596,          //   5
590,          //  10
584,          //  15
578,          //  20
572,          //  25
566,          //  30
560,          //  35
554,          //  40
548,          //  45
542,          //  50
536,          //  55
532,          //  60
};

//---------------------------------------
// �����⠭, ���⭮��� ������ 䠧�, ��/�3
//-50� ... 60�
float ButILiq[]=
{
633,          // -50
628.5,        // -45
624,          // -40
619.5,        // -35
615,          // -30
610,          // -25
605,          // -20
600,          // -15
594,          // -10
588,          //  -5
582,          // 0
576,          //   5
570,          //  10
564,          //  15
558,          //  20
552,          //  25
546,          //  30
540,          //  35
534,          //  40
528,          //  45
522,          //  50
515,          //  55
505,          //  60
};
//---------------------------------------
//---------------------------------------
//�⠭, ���⭮��� ��஢�� 䠧�, ��/�3
//-50� ... 60�
float EtanGas[]={
5.99,         // -50
7.25,         // -45
8.72,         // -40
10.42,        // -35
12.38,        // -30
14.62,        // -25
17.19,        // -20
20.08,        // -15
23.37,        // -10
27.06,        //  -5
31.20,        // 0
34.47,        //   5
40.95,        //  10
46.62,        //  15
52.90,        //  20
59.82,        //  25
67.41,        //  30
73.06,        //  35
78.86,        //  40
84.81,        //  45
90.90,        //  50
97.15,        //  55
103.54,       //  60
};
//---------------------------------------
//�ய��, ���⭮��� ��஢�� 䠧�, ��/�3
//-50� ... 60�
float PropGas[]={
1.81,         // -50
2.07,         // -45
2.61,         // -40
3.25,         // -35
3.87,         // -30
4.62,         // -25
5.48,         // -20
6.4,          // -15
7.57,         // -10
9.05,         //  -5
10.34,        // 0
11.9,         //   5
13.6,         //  10
15.51,        //  15
17.74,        //  20
20.15,        //  25
22.8,         //  30
25.3,         //  35
28.6,         //  40
34.5,         //  45
36.8,         //  50
40.22,        //  55
44.6,         //  60
};
//---------------------------------------
//��⠭, ���⭮��� ��஢�� 䠧�, ��/�3
//-50� ... 60�
float ButGas[]={
0.41,         // -50
0.50,         // -45
0.60,         // -40
0.73,         // -35
0.89,         // -30
1.08,         // -25
1.32,         // -20
1.602,        // -15
1.947,        // -10
2.1,          //  -5
2.82,         // 0
3.35,         //   5
3.94,         //  10
4.65,         //  15
5.39,         //  20
6.18,         //  25
7.19,         //  30
8.17,         //  35
9.334,        //  40
10.571,       //  45
12.1,         //  50
12.38,        //  55
15.4,         //  60
};
//------------------------------
//---------------------------------------
//�����⠭, ���⭮��� ��஢�� 䠧�, ��/�3
//-50� ... 60�
float ButIGas[]={
0.73,         // -50
0.89,         // -45
1.08,         // -40
1.32,         // -35
1.49,         // -30
1.65,         // -25
1.96,         // -20
2.5,          // -15
3.04,         // -10
3.59,         //  -5
4.31,         // 0
5.07,         //   5
5.92,         //  10
6.95,         //  15
7.94,         //  20
9.21,         //  25
11.5,         //  30
13,           //  35
14.7,         //  40
16.8,         //  45
18.94,        //  50
20.56,        //  55
24.2,         //  60
};

//---------------------------------------

#define k_Pa (0.0010)

//�⠭, �������� ����饭��� ��஢, M��
//-50� ... 60�
float EtanP[]={
553 * k_Pa,          // -50
655 * k_Pa,          // -45
771 * k_Pa,          // -40
902 * k_Pa,          // -35
1050 * k_Pa,         // -30
1215 * k_Pa,         // -25
1400 * k_Pa,         // -20
1604 * k_Pa,         // -15
1831 * k_Pa,         // -10
2081 * k_Pa,         //  -5
2355 * k_Pa,         // 0
2555 * k_Pa,         //   5
2982 * k_Pa,         //  10
3336 * k_Pa,         //  15
3721 * k_Pa,         //  20
4137 * k_Pa,         //  25
4585 * k_Pa,         //  30
4889 * k_Pa,         //  35
5193 * k_Pa,         //  40
5497 * k_Pa,         //  45
5801 * k_Pa,         //  50
6105 * k_Pa,         //  55
6409 * k_Pa,         //  60
};

//---------------------------------------
//�ய��, �������� ����饭��� ��஢, ���
//-50� ... 60�
float PropanP[]={
70  * k_Pa,          // -50
88  * k_Pa,          // -45
109 * k_Pa,          // -40
134 * k_Pa,          // -35
164 * k_Pa,          // -30
197 * k_Pa,          // -25
236 * k_Pa,          // -20
285 * k_Pa,          // -15
338 * k_Pa,          // -10
399 * k_Pa,          //  -5
465 * k_Pa,          // 0
543 * k_Pa,          //   5
629 * k_Pa,          //  10
725 * k_Pa,          //  15
833 * k_Pa,          //  20
951  * k_Pa,          //  25
1080 * k_Pa,         //  30
1226 * k_Pa,         //  35
1382 * k_Pa,         //  40
1554 * k_Pa,         //  45
1740 * k_Pa,         //  50
1943 * k_Pa,         //  55
2162 * k_Pa,         //  60
};
//---------------------------------------
//��⠭, �������� ����饭��� ��஢, ���
//-50� ... 60�
float ButanP[]={
4  * k_Pa,           // -50
6  * k_Pa,           // -45
8  * k_Pa,           // -40
12 * k_Pa,           // -35
20 * k_Pa,           // -30
32 * k_Pa,           // -25
44 * k_Pa,           // -20
56 * k_Pa,           // -15
68 * k_Pa,           // -10
84 * k_Pa,           //  -5
102 * k_Pa,          // 0
123 * k_Pa,          //   5
146 * k_Pa,          //  10
174 * k_Pa,          //  15
205 * k_Pa,          //  20
240 * k_Pa,          //  25
280 * k_Pa,          //  30
324 * k_Pa,          //  35
374 * k_Pa,          //  40
429 * k_Pa,          //  45
490 * k_Pa,          //  50
557 * k_Pa,          //  55
631 * k_Pa,          //  60

};
//---------------------------------------
//�����⠭, �������� ����饭��� ��஢, ���
//-50� ... 60�
float ButanIP[]={
8  * k_Pa,            // -50
12 * k_Pa,           // -45
20 * k_Pa,           // -40
32 * k_Pa,           // -35
44 * k_Pa,           // -30
56  * k_Pa,          // -25
68  * k_Pa,          // -20
88  * k_Pa,          // -15
107 * k_Pa,          // -10
128 * k_Pa,          //  -5
153 * k_Pa,          // 0
182 * k_Pa,          //   5
215 * k_Pa,          //  10
252 * k_Pa,          //  15
294 * k_Pa,          //  20
341 * k_Pa,          //  25
394 * k_Pa,          //  30
452 * k_Pa,          //  35
518 * k_Pa,          //  40
590 * k_Pa,          //  45
669 * k_Pa,          //  50
759 * k_Pa,          //  55
853 * k_Pa,          //  60
};
//---------------------------------------

