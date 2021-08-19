

//---------------------------
// Отпуск нефтепродуктов
//---------------------------
// Аналоговые входы:
//     7017C Vin0+ (11) - датчик давления
//     7017C Vin1+ (13) - датчик температуры
//
//---------------------------
//                        номер входа (команда 'MKS')
//
// INP_VAR D0   7041-DI 0    1   Кн. Старт верхнего налива
//         D1   7041-DI 1    2   Кн. Стоп  верхнего налива
//         D2   7041-DI 2    3   Кн. Старт нижнего налива
//         D3   7041-DI 3    4   Кн. Стоп  нижнего налива
//         D4   7041-DI 4    5   Уровень нижнего налива
//         D5   7041-DI 5    6
//         D6   7041-DI 6    7
//         D7   7041-DI 7    8
//         D8   7041-DI 8    9
//         D9   7041-DI 9   10
//         D10  7041-DI 10  11   УЗА верхнего и нижнего налива
//         D11  7041-DI 11  12   Консоль
//         D12  7041-DI 12  13   Трап
//         D13  7041-DI 13  14   Уровень верхнего налива
//                          15
//                          16
//
// INP_OFP D0               17       Состояние секций , 1 - 6, 1 - ok
//         D1               18
//         D2               19
//         D3               20
//         D4               21
//         D5               22
//         D6               23       Состояние общее , 1 - ok
//         D7               24       Состояние PE    , 1 - ok
//
//---------------------------
//---------------------------
//       1 2 3 4 5 6 7  8
//  "MKO 2 1 7 6 4 3 0  8"

int result_dlv=0;

//int num_out1=1;  //  больший клапан регулирования расхода,пилот нормально открытый , /DN закрыть задвижку (OUT1=0)
//int num_out2=2;  //  меньший клапан регулирования расхода,пилот нормально закрытый ,  UP открыть задвижку (OUT2=1)
int num_out3=7;  //  включение насоса
int num_out4=6;  //  включение air switch (OUT4)
int num_out5=3;  //  больший клапан нижнего  налива  // BIO_1 - исполнительный выход для CL1_SRC
int num_out6=4;  //  меньший клапан нижнего  налива  // BIO_1 - исполнительный выход для CL2_SRC
int num_out7=0;  //  подключение дисплея , по F1
int num_out8=8;  //  сигнал аварии , инверсный , 1 - OK.

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
int num_in14 = 0;    //  Клапан нижнего налива закрыт
int num_in15 = 0;    //  Клапан верхнего налива закрыт

//---------------------------
  // Дискретные входы-выходы
 //  " MKS 8 2 1 3 4 5 6"
  // ВХОДЫ
  // p1...p7.MKS
  // inp1 - ES
  // inp2 - UZA           7060-IN2 (p.6) 2
  // inp3 - датчик уровня 7060-IN1 (p.3) 1
  // inp4 - кн. "Start"   7060-IN3 (p.4) 3
  // inp5 - кн. "Stop"    7060-IN4 (p.4) 4
  // inp6   трап          7188-DI1 (p.7) 5
  // inp7   консоль       7188-DI2 (p.7) 6


//  "MKO 2 1 7 6 4 3 0  8"
  //p1.MKO  OUT1 -      - меньший клапан верхнего налива
  //p2.MKO  OUT2 -      - больший клапан верхнего налива
  //p3.MKO  OUT3 - PUMP - включение насоса , не исп.
  //p4.MKO  OUT4 - Air switch

  //p5.MKO  OUT5 - меньший клапан нижнего налива
  //p6.MKO  OUT6 - больший клапан нижнего налива

  //p7.MKO  OUT7 = CLP0 - самый больший клапан верхнего налива
  //p8.MKO  OUT8 -   - выход сигнала аварии , сигнализация   7060-OUT3  (p.5)


//---------------------------
void f_alarm();
//unsigned int OUT_VAR=0;    // состояние дискретных выходов
unsigned int OUT_VARi0=0;  // состояние дискретных выходов для Slave
//unsigned int INP_VAR=0;    // состояние дискретных входов
//unsigned int INP_inv=0;    // маска инверсии входов
unsigned int INP_VAR_c=0;  // копия состояния дискретных входов
unsigned int INP_VARi0=0;  // состояние дискретных входов  для Slave
unsigned int INP_MSK_OK=INP_MSK_OK_v;        // состояние дискретных входов для верхнего налива
unsigned int INP_MSK_OK_L=INP_MSK_OK_v_L;    // состояние дискретных входов для нижнего налива
unsigned int OUT_VAR_c=0;  // копия состояния дискретных входов

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

   if(LVL_IN ) itmp |= 1;      //D0   - датчик уровня верхнего налива,      1 - ok
   if(UZA_IN ) itmp |= 2;      //D1   - УЗА,                                1 - ok
   if(STRT_IN) itmp |= 4;      //D2   - кнопка "Start" верхнего налива      1 - кнопка нажата
   if(ES_IN  ) itmp |= 8;      //D3   - Аварийная кнопка верхнего налива    1 - ok,0 - кнопка нажата

   if(CONS_IN   ) itmp |=0x10; //D4   - консоль,                            1 - ok
   if(TRAP_IN   ) itmp |=0x20; //D5   - трап,                               1 - ok
// if(          ) itmp |=0x40; //D6 = 0
   if(LVL_IN_L  ) itmp |=0x80; //D7 = - контроллер ограничения наполнения нижнего налива, 1 - ok

   if(STRT_IN_L ) itmp |=0x100;//D8   - кнопка "Start" нижнего налива       1 - кнопка нажата
   if(ES_IN_L   ) itmp |=0x200;//D9   - Аварийная кнопка нижнего налива     1 - ok,0 - кнопка нажата
   if(IN_CLS_H  ) itmp |=0x400;//D10  - задвижка верхнего налива закрыта
   if(IN_CLS_L  ) itmp |=0x800;//D11  - задвижка нижнего налива закрыта

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
// вводит входные сигналы  I7188 и записывает их в
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
{ // проверяет состояние кнопки ES и UZA при отпуске/приеме СУГ
  // сигналы исчезают  - отпуск/прием прерывается
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