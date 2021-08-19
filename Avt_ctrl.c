

//---------------------------
// ���� ���⥯த�⮢
//---------------------------
// ��������� �室�:
//     7017C Vin0+ (11) - ���稪 ��������
//     7017C Vin1+ (13) - ���稪 ⥬�������
//
//---------------------------
//                        ����� �室� (������� 'MKS')
//
// INP_VAR D0   7041-DI 0    1   ��. ���� ���孥�� ������
//         D1   7041-DI 1    2   ��. �⮯  ���孥�� ������
//         D2   7041-DI 2    3   ��. ���� ������� ������
//         D3   7041-DI 3    4   ��. �⮯  ������� ������
//         D4   7041-DI 4    5   �஢��� ������� ������
//         D5   7041-DI 5    6
//         D6   7041-DI 6    7
//         D7   7041-DI 7    8
//         D8   7041-DI 8    9
//         D9   7041-DI 9   10
//         D10  7041-DI 10  11   ��� ���孥�� � ������� ������
//         D11  7041-DI 11  12   ���᮫�
//         D12  7041-DI 12  13   �࠯
//         D13  7041-DI 13  14   �஢��� ���孥�� ������
//                          15
//                          16
//
// INP_OFP D0               17       ����ﭨ� ᥪ権 , 1 - 6, 1 - ok
//         D1               18
//         D2               19
//         D3               20
//         D4               21
//         D5               22
//         D6               23       ����ﭨ� ��饥 , 1 - ok
//         D7               24       ����ﭨ� PE    , 1 - ok
//
//---------------------------
//---------------------------
//       1 2 3 4 5 6 7  8
//  "MKO 2 1 7 6 4 3 0  8"

int result_dlv=0;

//int num_out1=1;  //  ����訩 ������ ॣ㫨஢���� ��室�,����� ��ଠ�쭮 ������ , /DN ������� �������� (OUT1=0)
//int num_out2=2;  //  ����訩 ������ ॣ㫨஢���� ��室�,����� ��ଠ�쭮 ������� ,  UP ������ �������� (OUT2=1)
int num_out3=7;  //  ����祭�� ����
int num_out4=6;  //  ����祭�� air switch (OUT4)
int num_out5=3;  //  ����訩 ������ �������  ������  // BIO_1 - �ᯮ���⥫�� ��室 ��� CL1_SRC
int num_out6=4;  //  ����訩 ������ �������  ������  // BIO_1 - �ᯮ���⥫�� ��室 ��� CL2_SRC
int num_out7=0;  //  ������祭�� ��ᯫ�� , �� F1
int num_out8=8;  //  ᨣ��� ���ਨ , ������� , 1 - OK.

int CLP0=0;

// MKO 0 3 4 0 1 2 0 8 - BIO

//     1  2  3 4 5  6  7  8  9  10  11  12
// MKS 2 11 14 1 0 12 13  4 11   5   0   3

int num_in1  = 17;   //  ES_IN
int num_in2  = 18;   //  UZA_IN
int num_in3  =14;    //  LVL_IN
int num_in4  = 1;    //  STRT_IN
int num_in5  = 0;    //  STOP_IN
int num_in6  = 12;   //  CONS_IN
int num_in7  =13;    //  TRAP_IN
int num_in8  = 4;    //  ES_IN_L
int num_in9  =11;    //  UZA_IN_L
int num_in10 = 5;    //  LVL_IN_L
int num_in11 =0;     //  STOP_IN_L
int num_in12 = 3;    //  STRT_IN_L
int num_in13 =1;     //  O_SH_IN
int num_in14 = 0;    //  ������ ������� ������ ������
int num_in15 = 0;    //  ������ ���孥�� ������ ������

//---------------------------
  // ������ �室�-��室�
 //  " MKS 8 2 1 3 4 5 6"
  // �����
  // p1...p7.MKS
  // inp1 - ES
  // inp2 - UZA           7060-IN2 (p.6) 2
  // inp3 - ���稪 �஢�� 7060-IN1 (p.3) 1
  // inp4 - ��. "Start"   7060-IN3 (p.4) 3
  // inp5 - ��. "Stop"    7060-IN4 (p.4) 4
  // inp6   �࠯          7188-DI1 (p.7) 5
  // inp7   ���᮫�       7188-DI2 (p.7) 6


//  "MKO 2 1 7 6 4 3 0  8"
  //p1.MKO  OUT1 -      - ����訩 ������ ���孥�� ������
  //p2.MKO  OUT2 -      - ����訩 ������ ���孥�� ������
  //p3.MKO  OUT3 - PUMP - ����祭�� ���� , �� ��.
  //p4.MKO  OUT4 - Air switch

  //p5.MKO  OUT5 - ����訩 ������ ������� ������
  //p6.MKO  OUT6 - ����訩 ������ ������� ������

  //p7.MKO  OUT7 = CLP0 - ᠬ� ����訩 ������ ���孥�� ������
  //p8.MKO  OUT8 -   - ��室 ᨣ���� ���ਨ , ᨣ��������   7060-OUT3  (p.5)


//---------------------------
void f_alarm();
//unsigned int OUT_VAR=0;    // ���ﭨ� ������� ��室��
unsigned int OUT_VARi0=0;  // ���ﭨ� ������� ��室�� ��� Slave
//unsigned int INP_VAR=0;    // ���ﭨ� ������� �室��
//unsigned int INP_inv=0;    // ��᪠ �����ᨨ �室��
unsigned int INP_VAR_c=0;  // ����� ���ﭨ� ������� �室��
unsigned int INP_VARi0=0;  // ���ﭨ� ������� �室��  ��� Slave
unsigned int INP_MSK_OK=INP_MSK_OK_v;        // ���ﭨ� ������� �室�� ��� ���孥�� ������
unsigned int INP_MSK_OK_L=INP_MSK_OK_v_L;    // ���ﭨ� ������� �室�� ��� ������� ������
unsigned int OUT_VAR_c=0;  // ����� ���ﭨ� ������� �室��

//-----------------------------------------------------------------
//---------------------------

int  iv_msk[KOL_D_INP+2]  ;
int  inv_msk[KOL_D_INP+2]={0,1,0} ;
int int_fict_11=1;
//---------------------------

void *p_ik[]=
{
&int_fict_11,
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  //  1  2  3  4
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  //  5  6  7  8
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  //  9 10 11 12
&INP_VAR, &INP_VAR, &INP_VAR, &INP_VAR,  // 13 14 15 16
//&INP_OFP ,&INP_OFP ,&INP_OFP ,&INP_OFP , // 17 18 19 20
//&INP_OFP ,&INP_OFP ,&INP_OFP ,&INP_OFP , // 21 22 23 24
&INP_7188,&INP_7188,                     // 17 18
};

int msk_ik[]=
{
   1,
   1    ,2    ,4    ,8,  0x10   ,0x20,  0x40  ,0x80,         //  1  2  3  4  5  6  7  8
 0x100,0x200,0x400,0x800,0x1000, 0x2000,0x4000,0x8000,       //  9 10 11 12 13 14 15 16
//   1    ,2    ,4    ,8,  0x10   ,0x20,  0x40  ,0x80,         // 17 18 19 20 21 22 23 24
   1    ,2                                                   // 25 26
};
//---------------------------
int fict_npn=0;
int *num_pnt[KOL_D_INP+1]={&fict_npn,
&num_in1,&num_in2,&num_in3,&num_in4,&num_in5,&num_in6,&num_in7,&num_in8,
&num_in9,&num_in10,&num_in11,&num_in12,&num_in13,
&num_in14,&num_in15,
//&num_in16,&num_in17,&num_in18,
};
//---------------------------
int en_bot[KOL_D_INP+2];

int f_ik(int i)
{
int ii;
   if(en_bot[i] == 1) return 1;
   else if(en_bot[i] == -1) return 0;
   ii=*num_pnt[i];
   return( ( ( *(int *)p_ik[ii]) ^ iv_msk[i]  ) & msk_ik[ii] );
}
//---------------------------
// #define    mski_inp1     inv_msk[1]
void f_init_mki()
{
int i;

  for(i=1;i<=KOL_D_INP;i++)
    {
     if(inv_msk[i] != 0.) iv_msk[i] = 0xffff;
     else   iv_msk[i] = 0;
    }
}
//-------------------------------
void f_var_i()
{

int itmp=0;

   if(LVL_IN ) itmp |= 1;      //D0   - ���稪 �஢�� ���孥�� ������,      1 - ok
   if(UZA_IN ) itmp |= 2;      //D1   - ���,                                1 - ok
   if(STRT_IN) itmp |= 4;      //D2   - ������ "Start" ���孥�� ������      1 - ������ �����
   if(ES_IN  ) itmp |= 8;      //D3   - ���਩��� ������ ���孥�� ������    1 - ok,0 - ������ �����

   if(CONS_IN   ) itmp |=0x10; //D4   - ���᮫�,                            1 - ok
   if(TRAP_IN   ) itmp |=0x20; //D5   - �࠯,                               1 - ok
// if(          ) itmp |=0x40; //D6 = 0
   if(LVL_IN_L  ) itmp |=0x80; //D7 = - ����஫��� ��࠭�祭�� ���������� ������� ������, 1 - ok

   if(STRT_IN_L ) itmp |=0x100;//D8   - ������ "Start" ������� ������       1 - ������ �����
   if(ES_IN_L   ) itmp |=0x200;//D9   - ���਩��� ������ ������� ������     1 - ok,0 - ������ �����
   if(IN_CLS_H  ) itmp |=0x400;//D10  - �������� ���孥�� ������ ������
   if(IN_CLS_L  ) itmp |=0x800;//D11  - �������� ������� ������ ������

INP_VARi0=  itmp;
itmp=0;

   if(OUT_VAR & OUT1  ) itmp |=0x1;  //
   if(OUT_VAR & OUT2  ) itmp |=0x2;  //
   if(OUT_VAR & OUT3  ) itmp |=0x4;  //
   if(OUT_VAR & OUT4  ) itmp |=0x8;  //
   if(OUT_VAR & OUT5  ) itmp |=0x10; //
   if(OUT_VAR & OUT6  ) itmp |=0x20; //
   if(OUT_VAR & OUT7  ) itmp |=0x40; //
   if(OUT_VAR & OUT8  ) itmp |=0x80; //

OUT_VARi0=itmp;

}

//-----------------------------------------------------------------

unsigned int INP_7188=0;

int do1_t=-1, do2_t=-1;

void f_inp7188()
{
// ������ �室�� ᨣ����  I7188 � �����뢠�� �� �
// D8,D9 INP_VAR
 int do_tmp;
 INP_7188 = 0;
 INP_7188 |= GetDi1();
 INP_7188 |= GetDi2();

   do_tmp=OUT_VAR & 0x0100;

  if(do1_t != do_tmp)
  {
   do1_t = do_tmp;
   if(do1_t  )  SetDo1(1);
   else   SetDo1(0);
  }


  do_tmp=OUT_VAR & 0x0200;

  if(do2_t != do_tmp)
  {
   do2_t = do_tmp;
   if(do2_t  )  SetDo2(1);
   else   SetDo2(0);
  }


}

//------------------------------------


void f_sens()
{ // �஢���� ���ﭨ� ������ ES � UZA �� ���᪥/�ਥ�� ���
  // ᨣ���� ��祧���  - ����/�ਥ� ���뢠����
  // inp1 - ES
  // inp2 - UZA


//  i7060_out[0] = itmp;

  if( ( ES_IN)==0 )
   {
     f_icp_errS(LVL_alm);
   }

  //29.04.2021 YN -\\//-
  if( ( UZA_IN)==0 )
  {
    //f_icp_errS(No_Z_mass_err);
    waterTank = 1;
  }
  else waterTank = 0;
  //              -//\\-

  return;
}
//-----------------------------