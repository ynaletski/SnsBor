int TankN=0;
void (*ff_Slv)() = NULL;
int CMD_REG=0;
int flag_Slv=0;

int n_res_a=2;
int n_GRK_a=1;

int ComPortSlv = 4;
int AddrSlv=1;
int cod_float=1;
int State_SLV=0;
int Last_evt=0;
int sw_mmi_slv=0;
int sw_dlv_slv=0;

int  WD_PMP_flag=0;
int  WD_PMP_Fl=0;
#define WD_PMP_P1 (2000)
long int WD_PMP_time=0;
long int WD_PMP_t1=0;
long int WD_PMP_Per=20000; // величина таймаута по приему сообщения

unsigned char buf_tmpSR[n_buf_RTU_s];

int Slv_fict=0;
int Slv_fict1=0;
int Slv_fict2=0;
int Slv_fict3=0;
int Slv_fict4=0;

int flag_slvrtu=0;
int fl_slv_out=0;

long int byte_timout_slv= 8 ;
long int out_delay_slv = 5 ;


// Для записи в нижний контроллер значений измеренной плотности
// необходимо записать данные в соответствующие переменные float
//A=1008   Новое значение плотн.1
//  1010   Новое значение темп.1
//  1012   Новое значение плотн.2
//  1014   Новое значение темп.2
// затем в переменной Int с адресом A=16 (Int(16)) установить биткодированный флаг записи.
// D0=1 - модифицировать данные резервуара 1
// D1=1 - модифицировать данные резервуара 2
// После успешной передачи данных вниз Int(16) обнулится
//

void *list_Slv_int[]={

&n_sl_int1,      // I0  колич.перем int
&n_sl_flt1,      // I1  колич.перем float
&Slv_int[0],     // I2  Резерв
&Slv_int[1],     // I3  Резерв
&sw_ver[0],      // I4  Версия ПО, 6 ASCII символов
&sw_ver[2],      // I5
&sw_ver[4],      // I6
&CMD_REG,        // I7   Регистр команды
&CMD_OUT      ,  // I8   Регистр подтверждения команды
&Reg_State    ,  // I9   Регистр состояния
&Reg_err      ,  // I10  Регистр ошибок
&i_LVL        ,  // I11  Состояние предельных уровней резервуаров
&OUT_VAR      ,  // I12  Состояние дискретных входов контроллера ГРК N1
&INP_VAR      ,  // I13  Состояние дискретных выходов контроллера ГРК N1
&WD_PMP_Per,     // I14  Период сторожевого таймера Host,ms  0 - таймер отключен
&Reg_State2   ,  // I15  Регистр состояния ГРК2
&Reg_err2     ,  // I16  Регистр ошибок    ГРК2
&OUT_VAR2     ,  // I17  Состояние дискретных входов контроллера ГРК N2
&INP_VAR2     ,  // I18  Состояние дискретных выходов контроллера ГРК N2

&n_res_a      ,  // I19  Количество резервуаров в системе 1...3
&n_GRK_a      ,  // I20  Количество ГРК в системе         1...2

NULL

};

int n_sl_int=(sizeof(list_Slv_int) / sizeof(list_Slv_int[0])) - 1 ;
int n_sl_int1=(sizeof(list_Slv_int) / sizeof(list_Slv_int[0])) - 1 ;

float *list_Slv_flt[]={

&Slv_float[0] ,     // F1000   0 операнд команды
&Slv_float[1],      // F1002   1 копия операнда

&MassTotal   ,      // F1004   2 Масса в операции приема,кг.
&VolTotal    ,      // F1006   3 Объем в операции приема,л.
&DenTotal    ,      // F1008   4 Средняя плотность в операции приема,кг/м3.
&TempTotal   ,      // F1010   5 Средняя температура в операции приема,C.

&s_DGM[0].DensReal ,// F1012   6 Измеренное значение плотности жидкости в резервуаре N1,кг/м3
&s_DGM[0].TempReal ,// F1014   7 Температура при измерении плотности в резервуаре N1,C
&s_DGM[1].DensReal ,// F1016   8 Измеренное значение плотности жидкости в резервуаре N2,кг/м3
&s_DGM[1].TempReal ,// F1018   9 Температура при измерении плотности в резервуаре N2,C

              //                 Данные о состоянии резервуаров
&s_DGM[0].MassLiq , // F1020  10 Масса жидкости в резервуаре N1, кг
&s_DGM[0].VolLiq  , // F1022  11 Объем жидкости в резервуаре N1,л
&s_DGM[0].MassGas , // F1024  12 Масса газа в резервуаре N1, кг
&s_DGM[0].VolGas  , // F1026  13 Объем газа в резервуаре N1,л
&s_DGM[1].MassLiq , // F1028  14 Масса жидкости в резервуаре N2, кг
&s_DGM[1].VolLiq  , // F1030  15 Объем жидкости в резервуаре N2,л
&s_DGM[1].MassGas , // F1032  16 Масса газа в резервуаре N2, кг
&s_DGM[1].VolGas  , // F1034  17 Объем газа в резервуаре N2,л
&s_DGM[0].DensLiq  ,// F1036  18 Плотность жидкой фазы в резервуаре N1,кг/м3
&s_DGM[0].DensGas  ,// F1038  19 Плотность паровой фазы в резервуаре N1,кг/м3
&s_DGM[1].DensLiq  ,// F1040  20 Плотность жидкой фазы в резервуаре N2,кг/м3
&s_DGM[1].DensGas  ,// F1042  21 Плотность паровой фазы в резервуаре N2,кг/м3
&s_DGM[0].Temp    , // F1044  22 Температура жидкой фазы в резервуаре N1, C
&s_DGM[0].Prod_lvl, // F1046  23 Уровень жидкости в резервуаре N1,мм (первичные данные из уровнемера)
&s_DGM[1].Temp    , // F1048  24 Температура жидкой фазы в резервуаре N2, C
&s_DGM[1].Prod_lvl, // F1050  25 Уровень жидкости в резервуаре N2,мм (первичные данные из уровнемера)

              //                 Данные датчиков давления , температуры
&s_MVD[0].Press,    // F1052  26 Давление в расходомере,МПа
&Press1       ,     // F1054  27 Давление в резервуаре N1,МПа
&Press2       ,     // F1056  28 Давление в резервуаре N2,МПа
&Temp1        ,     // F1058  29 Температура паров в резервуаре N1,C
&Temp2        ,     // F1060  30 Температура паров в резервуаре N2,C

                //               Данные из расходомера N1 (жидкая фаза)
&s_MVD[0].MassI,    // F1062  31 Масса инвентарная ( накапливающий счетчик)
&s_MVD[0].MassT,    // F1064  32 Масса в текущей операции отпуска (total)
&s_MVD[0].VolI,     // F1066  33 Объем инвентарный ( накапливающий счетчик)
&s_MVD[0].VolT,     // F1068  34 Объем в текущей операции отпуска (total)
&s_MVD[0].FlowM,    // F1070  35 Массовый расход, кг/ч
&s_MVD[0].Dens,     // F1072  36 Плотность кг/м3
&s_MVD[0].Temp,     // F1074  37 Температура в полости расходомера,C

&s_MVD[2].MassI,    // F1076  38 Масса СУГ отпущенного через ГРК,кг.Накапливающий счетчик.
&s_MVD[2].VolI ,    // F1078  39 Объем СУГ отпущенного через ГРК,л.Накапливающий счетчик.

                //               Данные из расходомера N2 ( паровая фаза )
&s_MVD[1].MassI,    // F1080  40 Масса инвентарная ( накапливающий счетчик)
&s_MVD[1].MassT,    // F1082  41 Масса в текущей операции отпуска (total)
&s_MVD[1].VolI,     // F1084  42 Объем инвентарный ( накапливающий счетчик)
&s_MVD[1].VolT,     // F1086  43 Объем в текущей операции отпуска (total)
&s_MVD[1].FlowM,    // F1088  44 Массовый расход, кг/ч
&s_MVD[1].Dens,     // F1090  45 Плотность кг/м3
&s_MVD[1].Temp,     // F1092  46 Температура,C

&Vol_max[0],        // F1094  47
&Vol_10[0],         // F1096  48
&Vol_90[0],         // F1098  49

&Vol_max[1],        // F1100  50
&Vol_10[1],         // F1102  51
&Vol_90[1],         // F1104  52

//-----------  добавлено для 3-го  резервуара

&s_DGM[2].DensReal ,// F1106  53 Измеренное значение плотности жидкости в резервуаре N2,кг/м3
&s_DGM[2].TempReal ,// F1108  54 Температура при измерении плотности в резервуаре N2,C
&s_DGM[2].MassLiq , // F1110  55 Масса жидкости в резервуаре N3, кг
&s_DGM[2].VolLiq  , // F1112  56 Объем жидкости в резервуаре N3,л
&s_DGM[2].MassGas , // F1114  57 Масса газа в резервуаре N3, кг
&s_DGM[2].VolGas  , // F1116  58 Объем газа в резервуаре N3,л
&s_DGM[2].DensLiq  ,// F1118  59 Плотность жидкой фазы в резервуаре N3,кг/м3
&s_DGM[2].DensGas  ,// F1120  60 Плотность паровой фазы в резервуаре N3,кг/м3
&s_DGM[2].Temp    , // F1122  61 Температура жидкой фазы в резервуаре N3, C (первичные данные из уровнемера)
&s_DGM[2].Prod_lvl, // F1124  62 Уровень жидкости в резервуаре N3,мм (первичные данные из уровнемера)
&Press3       ,     // F1126  63 Давление в резервуаре N3,МПа    (сигнал аналогового датчика)
&Temp3        ,     // F1128  64 Температура паров в резервуаре N3,C   (сигнал аналогового датчика)
&Vol_max[2],        // F1130  65 Общий объем резервуара 2,л
&Vol_10[2],         // F1132  66 Минимальнодопустимый объем жидкой фазы в рез.1,л
&Vol_90[2],         // F1134  67 Максимальнодопустимый объем жидкой фазы в рез.1,л

//-----------  добавлено для 3,4 расходомеров

                //               Данные из расходомера N3 ( ГРК1 , отпуск )

&s_MVD[2].MassI,    // F1136  68 Масса инвентарная ( накапливающий счетчик) == Масса СУГ отпущенного через ГРК1,кг.Накапливающий счетчик.
&s_MVD[2].MassT,    // F1136  69 Масса в текущей операции отпуска (total)
&s_MVD[2].VolI,     // F1140  70 Объем инвентарный ( накапливающий счетчик) == Объем СУГ отпущенного через ГРК1,л. Накапливающий счетчик.
&s_MVD[2].VolT,     // F1142  71 Объем в текущей операции отпуска (total)
&s_MVD[2].FlowM,    // F1144  72 Массовый расход, кг/ч
&s_MVD[2].Dens,     // F1146  73 Плотность кг/м3
&s_MVD[2].Temp,     // F1148  74 Температура в полости расходомера,C

                //               Данные из расходомера N4 ( ГРК2 , отпуск )

&s_MVD[3].MassI,    // F1150  75 Масса инвентарная ( накапливающий счетчик) == Масса СУГ отпущенного через ГРК2,кг.Накапливающий счетчик.
&s_MVD[3].MassT,    // F1152  76 Масса в текущей операции отпуска (total)
&s_MVD[3].VolI,     // F1154  77 Объем инвентарный ( накапливающий счетчик) == Объем СУГ отпущенного через ГРК2,л. Накапливающий счетчик.
&s_MVD[3].VolT,     // F1156  78 Объем в текущей операции отпуска (total)
&s_MVD[3].FlowM,    // F1158  79 Массовый расход, кг/ч
&s_MVD[3].Dens,     // F1160  80 Плотность кг/м3
&s_MVD[3].Temp,     // F1162  81 Температура в полости расходомера,C

&Slv_float[2],      // F1164  82 Резерв
&Slv_float[3],      // F1164  83 Резерв
&New_Dens1c,        // F1168  84 Новое значение плотн.1
&New_Temp1c,        // F1170  85 Новое значение темп.1
&New_Dens2c,        // F1172  86 Новое значение плотн.2
&New_Temp2c,        // F1174  87 Новое значение темп.2
&New_Dens3c,        // F1176  88 Новое значение плотн.3
&New_Temp3c,        // F1178  89 Новое значение темп.3

NULL

};
long int Com_Baud_Slave=9600;
long int long_slv   =0;
long int *list_Slv_lng[]={
                    // Addr = 1180
&Com_Baud_Slave,    //  90  Частота обмена по Modbus
&long_slv,          //  91  Резерв

NULL
};

int n_sl_lng=(sizeof(list_Slv_lng) / sizeof(&FL_err)) - 1 ;
int n_sl_lng1=(sizeof(list_Slv_lng) / sizeof(&FL_err)) - 1 ;

int n_sl_flt=((sizeof(list_Slv_flt) / sizeof(&FL_err)) - 1)+ ((sizeof(list_Slv_lng) / sizeof(&FL_err)) - 1) ;
int n_sl_flt1=((sizeof(list_Slv_flt) / sizeof(&FL_err)) - 1)+ ((sizeof(list_Slv_lng) / sizeof(&FL_err)) - 1) ;
int n_sl_flt0=(sizeof(list_Slv_flt) / sizeof(&FL_err)) - 1 ;

int Slv_int[20]={
  0,0,0,0,0,
  0,0,0,0,0,
};

//--------------------------------
float Slv_float[20]={
0,
0,
2,
3,
4,
5,
};
//--------------------------------
long int Slv_lng[10]={
  0,1,2,3,4,
  5,6,7,8,9,
};

unsigned long ComBaudSlv =9600L;
int Com_lgthSlv  =8;
int Com_paritySlv=0;// None  //0-(None);1-(Even);2-(Odd);3-(MARK, always 1); 4-(SPACE,always 0)
int Com_stopSlv  =1;
struct stor_buf *bf_Slv;
int uk_beg_Slv=0;
int uk_fin_Slv=0;

//-------------------------------------------
int nn_comb=0;
void fun_tim_u(void)
{
     if(flag_Slv != 0 )
     {
        if((flag_slvrtu == 0) && (fl_slv_out==0))
        {
         if(ffgets_SlvRTU(ComPortSlv))
         {
           flag_slvrtu = 1;

          _fmemcpy(intrpr.bfcom,&cb_COM[ComPortSlv][0],(long int)(n_bcom[ComPortSlv]-2) );
          nn_comb=n_bcom[ComPortSlv];
          n_bcom[ComPortSlv]=0;

         }
        }

        if(fl_slv_out != 0)
         {
          if( f_timer(time_snd[ComPortSlv],out_delay_slv))
           {
            ToComBufn(ComPortSlv,buf_tmpSR,fl_slv_out);
            flag_slvrtu=0;
            fl_slv_out=0;
            n_bcom[ComPortSlv]=0;
           }
         }
     }
}
//-------------------------------------------

void f_SlaveRTU()
{
int itmp;
//  int  hour,min,sec;
// вводит команду из Host (PC) и интерпретирует ее по протоколу Modbus RTU

         if((flag_slvrtu != 0) && (fl_slv_out==0))
         {
//          f_stor_Slv(M_RD,cb_COM[ComPortSlv],n_bcom[ComPortSlv]);
            f_intr_SlvRTU();
            flag_slvrtu=0;
         }
}
//-------------------------------------------
//-------------------------------------------
void f_get_SlvBuf()
{
  unsigned long int count=0xffff;

    if( ( bf_Slv = (struct stor_buf *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!Slave");
m1:
      printf(" buffer is not allocated.\n\r");
      RestoreCom(1);
      RestoreCom(2);
      RestoreCom(3);
      while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
      RestoreCom(4);
      exit (0);
    }
    count=max_TCT* sizeof(TCT_sz);
    if( ( TCT_T1 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT1");
      goto m1;
    }
    if( ( TCT_T2 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT2");
      goto m1;
    }
    if( ( TCT_T3 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT3");
      goto m1;
    }

    TCT_ARR[0]=&TCT_T1[0];
    TCT_ARR[1]=&TCT_T2[0];
    TCT_ARR[2]=&TCT_T3[0];

}
/*-------------------------------------------*/
void f_free_SlvBuf()
{
  free( bf_Slv );
  free( TCT_T1 );
  free( TCT_T2 );
  free( TCT_T3 );
}
/*-------------------------------------------*/
void f_stor_Slv( char type,char *buf,int buf_n)
{
  long int nn;

 bf_Slv[uk_fin_Slv].time = TimeStamp;

 if(buf_n >= mess_lgth ) nn=mess_lgth;
 else nn=buf_n;

 bf_Slv[uk_fin_Slv].type_n=type | (char)nn;

 _fmemcpy(bf_Slv[uk_fin_Slv].messg,buf,nn );
 uk_fin_Slv=(uk_fin_Slv+1)&Slv_b_lgth_1;
}
/*-------------------------------------------*/
#define RTU_min 6
#define n_max_fn 5
#define NN_fict 1000
// количество байт в команде Host

/* =============
// функция ,  длина с контр.сумм.
  1,8, // чтение Coil
  3,8, // чтение Int
  5,8, // запись Coil
  6,8, // запись Int
  16,NN_fict, // запись нескольких значений
=============== */

int n_max_rcv=NN_fict;

//-------------------------------------------
int ffgets_SlvRTU(int ComPort)
{

//  Функция принимает символы из порта COM ComPort  в буфер cb_COM[ComPort].
//  При таймауте команда считается введенной и функция возвращает
// значение 1.
//

/* ComPort - номер COM   1...4 */
 char tmp;
 int tmp1;
 int i;
m1:
     if(IsCom(ComPort)==0)
     {
         if( WD_PMP_Per !=0 )
           if( f_timer(WD_PMP_time,WD_PMP_Per ))
           { // нет успешного приема

            if(WD_PMP_flag != 0)
            {
              WD_PMP_flag=0;
              f_icp_errS(WD_host);
            }
            WD_PMP_time=TimeStamp;
           }

           if( f_timer(WD_PMP_t1,WD_PMP_P1 ))
           { // нет успешного приема
            WD_PMP_Fl=0;
            WD_PMP_t1=TimeStamp;
           }

        if( n_bcom[ComPort] != 0)
         if( f_timer(time_snd[ComPort],byte_timout_slv))
         { // истек таймаут приема ответа на команду
m_end:
            if((n_bcom[ComPort] >= RTU_min)&& (cb_COM[ComPort][0]==AddrSlv))
            {
              if(f_check_crc_RTU(cb_COM[ComPort],n_bcom[ComPort])==0)
               {
                  goto m_err;
               }
              n_max_rcv= NN_fict;

   //       WD_PMP_flag=1;
              WD_PMP_time=TimeStamp;
              WD_PMP_t1=TimeStamp;
              WD_PMP_Fl=1;
       /*
              if(ICP_error[icp_lst_max] & WD_host)
              {
               ICP_error[icp_lst_max] &= ~(unsigned int)WD_host;
               ICP_error_c[icp_lst_max]=ICP_error[icp_lst_max];
               for(i=0,tmp1=0;i<=icp_lst_max;i++)
               {
                 tmp1 |=ICP_error[i];
               }
//              printf("\r\n tmp1= %04x",tmp1);

               if(tmp1==0)
               {
                FL_err=0;
                if(sw_mmi==0) f_prn_begin();
               }
              }
       */
              return 1;
            }
            else
             {
m_err:
               n_bcom[ComPort]=0;
               n_max_rcv= NN_fict;
               return 0;
             }
         }
         return 0;
     }
     tmp=ReadCom(ComPort);
     time_snd[ComPort]= TimeStamp;

  #if(Slv_deb_rtu != 0)
   if(flag_prn != 0) // qqqqq
     {
        tmp1=TimeStamp & 0x3ff;
        printf("%04d:%02X ",tmp1, (int)tmp & 0xff);

     }
  #endif

//     ToutAns[ComPort]=byte_timout_rtu[ComPort];

     cb_COM[ComPort][n_bcom[ComPort]]=tmp;
     if( (++n_bcom[ComPort]) >= n_bufcom )
      {
        n_bcom[ComPort]--;
  /*
        printf("\n Command buffer overflow !\n--");
        puts(cb_COM1);
  */
      }

      if(n_bcom[ComPort] == 2)
         n_max_rcv=f_get_numRTU2(ComPort);
      else if ((n_bcom[ComPort] == 7) && (n_max_rcv == NN_fict)  )
         n_max_rcv=f_get_numRTU7(ComPort);

      if(n_bcom[ComPort]>= n_max_rcv) goto m_end;

      goto m1;
}
//-------------------------------------------
int f_get_numRTU2(int ComPort)
{// по содержимому cb_COM[ComPort][] с количеством байт n_bcom[ComPort]
 // вычисляет длину посылки RTU. Если длина не определена - возвращает NN_fict
 int i_ret;
 switch ((int) cb_COM[ComPort][1])
 {
  case 1:
  case 3:
  case 5:
  case 6:
         i_ret=8;
         break;

  default: i_ret=NN_fict;
 }
 return i_ret;
}
/*-------------------------------------------*/
int f_get_numRTU7(int ComPort)
{// по содержимому cb_COM[ComPort][] с количеством байт n_bcom[ComPort]
 // вычисляет длину посылки RTU. Если длина не определена - возвращает NN_fict

 int i_ret;

 if( cb_COM[ComPort][1] == 16 )
   i_ret=(((int)cb_COM[ComPort][6]) & 0xff) + 9;
 else
   i_ret=NN_fict;

 return i_ret;
}
/*-------------------------------------------*/
void  f_intr_SlvRTU()
{
  int exc;
  int fn_cod;
  int addr_beg,i,i1,i2,itmp;
  int Slv_addrO,Slv_nmbO;
  float ftmp;
  union  { float f; unsigned long l; } o;


  // принятая команда проверена на контрольную сумму и находится в intrpr.bfcom
  //  (без контрольной суммы)

//---------------------------------------------------
  fn_cod=intrpr.bfcom[1];

#if(Slv_deb_rtu != 0)
     if(flag_prn != 0) // QQQ
     {
      printf("\r\nN= %d ==",nn_comb);
      for(i=0;i< nn_comb;i++)
      printf(" %02X", (intrpr.bfcom[i] & 0X0FF) );

      printf("\r\n");
     }
#endif

  _fmemcpy(buf_tmpSR,intrpr.bfcom,(long int)(8) );
  if(fn_cod== 3)
  {   // чтение Int
    Slv_addrO=f_get_intS(&intrpr.bfcom[2]);
    Slv_nmbO=f_get_intS(&intrpr.bfcom[4]);

    if(Slv_nmbO>125)
        goto m_err_addr;

    if( f_is_int(Slv_addrO) &&  f_is_int(Slv_addrO+Slv_nmbO-1) )
    { // запрошены данные int

      buf_tmpSR[0]=AddrSlv;
      buf_tmpSR[1]=fn_cod;
      buf_tmpSR[2]=Slv_nmbO+Slv_nmbO;

      for(i1=3,i=0;(i<Slv_nmbO)&&(i1< (n_buf_RTU_s-3)) ;i++)
       {
//       itmp=Slv_int[Slv_addrO+i];
         itmp=*(int *)list_Slv_int[Slv_addrO+i];
         buf_tmpSR[i1++]=(itmp>>8) & 0xff;
         buf_tmpSR[i1++]= itmp & 0xff;
       }
      goto out_b;
    }
    else
     if( f_is_float(Slv_addrO) &&  f_is_float(Slv_addrO+Slv_nmbO-2) )
     { // запрошены данные float
      if(Slv_addrO & 0x01) goto m_err_addr;
      if(Slv_nmbO  & 0x01) goto m_err_val;

      buf_tmpSR[0]=AddrSlv;
      buf_tmpSR[1]=fn_cod;
      buf_tmpSR[2]=Slv_nmbO+Slv_nmbO;
      Slv_nmbO=Slv_nmbO>>1;

      addr_beg=(Slv_addrO-AddrLowFloat)>> 1;
      for(i1=3,i=0;(i<Slv_nmbO)&&(i1< (n_buf_RTU_s-3));i++,i1+=4)
       {
         if((addr_beg+i) < n_sl_flt0)
           f_set_float( &buf_tmpSR[i1],*list_Slv_flt[addr_beg+i],cod_float);
         else
           f_set_float( &buf_tmpSR[i1],(float)*list_Slv_lng[addr_beg+i-n_sl_flt0],cod_float);
//         f_set_long( &buf_tmpSR[i1],*list_Slv_lng[addr_beg+i-n_sl_flt0]);
       }
      goto out_b;

     }
    else goto m_err_addr;
  }
  if(fn_cod== 6)
  {   // запись Int
    Slv_addrO=f_get_intS(&intrpr.bfcom[2]);
    if( f_is_int(Slv_addrO) )
    { // запись данных int
      *(int *)list_Slv_int[Slv_addrO]=f_get_intS(&intrpr.bfcom[4]);
      if((exc=f_int_fnc(Slv_addrO)) != 0)
         goto m_err;
      i1=6;
      goto out_b;
    }
    else goto m_err_addr;
  }
  if(fn_cod == 16 )
  {   // запись нескольких значений
    Slv_addrO=f_get_intS(&intrpr.bfcom[2]);
    Slv_nmbO=f_get_intS(&intrpr.bfcom[4]);
    if( f_is_int(Slv_addrO) &&  f_is_int(Slv_addrO+Slv_nmbO-1) )
    { //   данные int

     for(i1=7,i=0;i<Slv_nmbO;i++,i1+=2)
     {
      *(int *)list_Slv_int[Slv_addrO+i]=f_get_intS(&intrpr.bfcom[i1]);
      if((exc=f_int_fnc(Slv_addrO+i)) != 0)
         goto m_err;
     }

     i1=6;
     goto out_b;

    }
    else
     if( f_is_float(Slv_addrO) &&  f_is_float(Slv_addrO+Slv_nmbO-2) )
     { //  данные float
      if(Slv_addrO & 0x01) goto m_err_addr;
      if(Slv_nmbO  & 0x01) goto m_err_val;

      addr_beg=(Slv_addrO-AddrLowFloat)>> 1;
      Slv_nmbO=Slv_nmbO>>1;
      // i1=7-3;
      for(i1=4,i=0;i<Slv_nmbO;i++,i1+=4 )
      {
         if((addr_beg+i) < n_sl_flt0)
            *list_Slv_flt[addr_beg+i]=f_get_float( &intrpr.bfcom[i1], cod_float);
         else
            *list_Slv_lng[addr_beg+i-n_sl_flt0]=f_get_float( &intrpr.bfcom[i1], cod_float);
      }
      i1=6;
      goto out_b;
     }
    else goto m_err_addr;
  }
  else  goto m_err_fn ;

//---------------------------------------------------
m_err_fn  : // функция не распознана
 exc=01;
 goto m_err;

m_err_addr: // недопустимый адрес
 exc=02;
 goto m_err;

m_err_val : // недопустимые данные
 exc=03;
 goto m_err;

m_err_slv : // ошибка в Slave
 exc=04;
 goto m_err;

m_err:

 fn_cod |= 0x80;

 buf_tmpSR[0]=AddrSlv;
 buf_tmpSR[1]=fn_cod;
 buf_tmpSR[2]=exc;
 i2=3;

#if(Slv_deb != 0)

 printf("\n\r====");
 for(i=0;i<i2;i++)
 {
  printf("%02X ",buf_tmpSR[i]);
 }
 printf("====\n\r");
#endif

// f_stor_Slv(M_WR,buf_tmpSR,i2);
 f_prepareRTU(buf_tmpSR,&i2);

 fl_slv_out=i2;
// ToComBufn(ComPortSlv,buf_tmpSR,i2);
 return;

out_b:

 buf_tmpSR[0]=AddrSlv;
 buf_tmpSR[1]=fn_cod;

 i2=i1;

#if(Slv_deb != 0)

 printf("\n\r====");
 for(i=0;i<i2;i++)
 {
  printf("%02X ",buf_tmpSR[i]);
 }
 printf("====\n\r");

#endif

//  f_stor_Slv(M_WR,buf_tmpSR,i2);
 if(f_prepareRTU1(buf_tmpSR,&i2)) goto m_err_val;

 fl_slv_out=i2;
// ToComBufn(ComPortSlv,buf_tmpSR,i2);

 if(ff_Slv != NULL)
 {
  (*ff_Slv)(); // старт функции
  ff_Slv = NULL;
 }
 return;

}
/*-------------------------------------------*/
/*-------------------------------------------*/
int f_is_int(int addr)
{
 if((addr >= AddrLowInt) && (addr <= AddrHighInt) )
    return 1;
 else return 0;
}
/*-------------------------------------------*/
int f_is_float(int addr)
{
 if((addr >= AddrLowFloat) && (addr <= AddrHighFloat) )
    return 1;
 else return 0;
}
/*-------------------------------------------*/
int  f_int_fnc(int Addr)
{ // return 1 - error, 0 - OK
 int i_ret;
 i_ret=0;
 switch( Addr)
 {
  case Reg_Cmd:
       switch(CMD_REG)
       {
        case 1:
         i_ret= f_start_dlv();
         break;
        case 2:
         i_ret= f_stop_dlv();
         break;
        case 3:
         i_ret= f_alm_stop();
         break;
        case 4:
         i_ret= f_rst_slv();
         break;
        case 5:
         i_ret= f_CE_slv();
         break;

        case 6:
         i_ret= f_Stop_Pmp();
         break;


        case 8: // подтверждение чтения данных результата приема

         if((Reg_State & RCV_fin) == 0)
            goto m_err;
         else
            goto m_ok;

        case 9 : // подтверждение чтения данных результата изм.плотности в резервуаре 1
         if(TankN != 1)    goto m_err;
         if((Reg_State & DEN_fin) == 0) goto m_err;
         D1_eee = s_MVD[0].Dens;
         T1_eee = s_MVD[0].Temp ;
         f_wr_Dns();
         goto m_ok;

        case 10: // подтверждение чтения данных результата изм.плотности в резервуаре 2
         if(TankN != 2)  goto m_err;
         if((Reg_State & DEN_fin) == 0) goto m_err;
         D2_eee = s_MVD[0].Dens;
         T2_eee = s_MVD[0].Temp;
         f_wr_Dns();
         goto m_ok;

        case 11: // отмена чтения данных результата изм.плотности в резервуаре 1,2

         if((Reg_State & (DEN_fin | RCV_fin) ) == 0)
            goto m_err;
         else
            goto m_ok;

        case 12: // подтверждение чтения данных результата изм.плотности в резервуаре 3
         if(TankN != 3)  goto m_err;
         if((Reg_State & DEN_fin) == 0) goto m_err;
         D3_eee = s_MVD[0].Dens;
         T3_eee = s_MVD[0].Temp;
         f_wr_Dns();
         goto m_ok;

        case 106:
         i_ret= f_WE_slv();
         break;
        case 107:
         i_ret= f_Baud_slv();
         break;
        case 17:  // fls
         i_ret= f_fls();
         break;

      case 21: // запуск измерения плотности в резервуаре 1
        TankN=1;
        goto mm1;
      case 22: // запуск измерения плотности в резервуаре 2
        TankN=2;
        goto mm1;
      case 23: // запуск измерения плотности в резервуаре 2
        TankN=3;
        goto mm1;
      case CMD_DIS_PMP:  // (0x100)  запрет старта отпуска через колонку
      case 0:      // разрешение отпуска через колонку и включения приема,изм.плотн. от ВРФ
      case 0x1000: // разрешение отпуска через колонку,запрет включения приема,изм.плотн. от ВРФ
mm1:
         if(Reg_State != 0)
            goto m_err;
        m_ok:
             CMD_IN= CMD_REG;
             CMD_OUT =CMD_REG | CMD_CONT ;
             i_ret=0;
             break;

      case 41: // запись значений плотности в резервуаре 1
          if((New_Dens1c > Dens_min) && (New_Dens1c < Dens_max) )
            if((New_Temp1c > Temp_min) && (New_Temp1c < Temp_max) )
            {

              New_Dens1 = New_Dens1c;
              New_Temp1 = New_Temp1c;
              D1_eee    = New_Dens1c;
              T1_eee    = New_Temp1c;

        m40_ok:
             FlagNewDT= CMD_REG & 0x3;
             if(FlagNewDT & 0x1)
             {

             }
             if(FlagNewDT & 0x2)
             {

             }

             CMD_OUT =CMD_REG | CMD_CONT ;
             i_ret=0;
             break;
            }
        m_err:
           CMD_OUT=CMD_REG | CMD_ERR;
           CMD_IN = CMD_BAD;
           Flag_rd_cmd = CMD_BAD;
           i_ret = RTU_Val;
           break;

      case 42: // запись значений плотности в резервуаре 2
          if((New_Dens2c > Dens_min) && (New_Dens2c < Dens_max) )
            if((New_Temp2c > Temp_min) && (New_Temp2c < Temp_max) )
            {
             New_Dens2 = New_Dens2c;
             New_Temp2 = New_Temp2c;
             D2_eee    = New_Dens2c;
             T2_eee    = New_Temp2c;

             goto m40_ok;
            }
          goto m_err;

      case 43: // запись значений плотности в резервуаре 1,2
          if((New_Dens1c > Dens_min) && (New_Dens1c < Dens_max) )
            if((New_Temp1c > Temp_min) && (New_Temp1c < Temp_max) )
              if((New_Dens2c > Dens_min) && (New_Dens2c < Dens_max) )
                if((New_Temp2c > Temp_min) && (New_Temp2c < Temp_max) )
                {

                   New_Dens1 = New_Dens1c;
                   New_Temp1 = New_Temp1c;
                   New_Dens2 = New_Dens2c;
                   New_Temp2 = New_Temp2c;

                   D1_eee    = New_Dens1c;
                   T1_eee    = New_Temp1c;
                   D2_eee    = New_Dens2c;
                   T2_eee    = New_Temp2c;

             //    ff_Slv = f_wr_Dns();
                   f_wr_Dns();
                   goto m40_ok;
                }
          goto m_err;
//---------------------------------

        default:
           f_cmd_prn();
           i_ret= RTU_Val;
           CMD_OUT=CMD_REG | CMD_ERR;
           CMD_IN= CMD_BAD;
           Flag_rd_cmd = CMD_BAD;
         break;
       }
//---------------------------------
   default:
      break;

 }
 return i_ret;
}
/*-------------------------------------------*/
int f_fls()
{ // return 1 - error, 0 - OK
 flag_Slv=0;

 RestoreCom(ComPortHost);
 InstallCom(ComPortHost,115200L,8,0,1);
 ShowCom(ComPortHost,8,115200L);

 return 0;
}
/*-------------------------------------------*/
int f_WE_slv()
{ // return 1 - error, 0 - OK
 CMD_OUT=CMD_REG;
 ff_Slv=ff_we_slv;
 return 0;
}
/*-------------------------------------------*/
void ff_we_slv()
{
        if(f_wrt_eee()<0)
        {
          f_wr_evt(evt_we_err);
        }
        else
        {
          f_wr_evt(evt_we);
        }
}
/*-------------------------------------------*/
/*-------------------------------------------*/
int f_start_dlv()
{ // return !=0 - error, 0 - OK
  // старт
int i_ret;

   CMD_OUT=CMD_REG;
   i_ret=0;
 //  State_SLV = Cmd_err;

   if(Reg_err)
     i_ret=RTU_Slv_err;
   else if(Slv_float[0] <= 0)
     i_ret = RTU_Val;
   else if(Reg_State != 0)
     i_ret = RTU_Val;
   else if((INP_VAR & MSK_OK) == MSK_OK)
    {
       LIn_val=Slv_float[0];
       Slv_float[1]=Slv_float[0];
       Slv_float[0]=0;
       Reg_State=RCV_prep;
       CMD_IN=CMD_REG;
       CMD_OUT |= CMD_CONT ;
       i_ret=0;
    }
   else
     i_ret = RTU_Slv_err;

   if(i_ret)
   {
        CMD_OUT |=CMD_ERR;
        CMD_IN= CMD_BAD;
        Flag_rd_cmd = CMD_BAD;
   }
   return i_ret;
}
/*-------------------------------------------*/
int f_stop_dlv()
{ // return 1 - error, 0 - OK

int i_ret;

     CMD_OUT=CMD_REG;
//     if((Reg_State & (RCV_act|DEN_act) )== 0)
//       i_ret = RTU_Val;
//    else
     {
       CMD_IN= CMD_REG;
       CMD_OUT |= CMD_CONT ;
       i_ret=0;
     }
   if(i_ret)
   {
        CMD_OUT |=CMD_ERR;
        CMD_IN= CMD_BAD;
        Flag_rd_cmd = CMD_BAD;
   }
    return i_ret;
}
/*-------------------------------------------*/
int f_alm_stop()
{ // return 1 - error, 0 - OK

 CMD_IN=CMD_REG;
 CMD_OUT = CMD_REG | CMD_CONT ;
   f_icp_errS(Hst_alm);
 return 0;
}
/*-------------------------------------------*/
int f_rst_slv()
{ // return 1 - error, 0 - OK

 CMD_IN=CMD_REG;
 CMD_OUT = CMD_REG | CMD_CONT ;
 return 0;
}
/*-------------------------------------------*/
int f_CE_slv()
{ // return 1 - error, 0 - OK
 CMD_IN=CMD_REG; //5
 CMD_OUT = CMD_REG | CMD_CONT ;
 f_cl_error();
 return 0;
}
/*-------------------------------------------*/
int f_Stop_Pmp()
{ // return 1 - error, 0 - OK

int i_ret;

     CMD_OUT=CMD_REG;
   if((Reg_State &  Pump_act)==0)
       i_ret = RTU_Val;
    else
     {
       CMD_IN= CMD_REG;
       CMD_OUT |= CMD_CONT ;
       i_ret=0;
     }
   if(i_ret)
   {
       CMD_OUT |=CMD_ERR;
       CMD_IN= CMD_BAD;
       Flag_rd_cmd = CMD_BAD;
   }
   return i_ret;
}
/*-------------------------------------------*/
/*-------------------------------------------*/
int f_cmd_prn()
{ // return 1 - error, 0 - OK

#if(Slv_deb != 0)
 printf(" \n\r== Command %d ==",*(int *)list_Slv_int[4]);
#endif

 return 0;
}
/*-------------------------------------------*/
int f_Baud_slv()
{
int i_ret;
   if(f_get_nb(Com_Baud_Slave) < 0  )
   {
 fin_e:
    Com_Baud_Slave=ComBaudSlv;
    i_ret = RTU_Val;
   }
   else
   {
     ComBaudSlv = Com_Baud_Slave;

     RestoreCom(ComPortSlv );
     if( InstallCom(ComPortSlv ,ComBaudSlv,Com_lgthSlv,Com_paritySlv,Com_stopSlv) < 0 ) goto fin_e;
     if(ComPortSlv==4)
        ShowCom(ComPortSlv,Com_lgthSlv,ComBaudSlv);

     i_ret = 0;
   }
   return i_ret;
}
