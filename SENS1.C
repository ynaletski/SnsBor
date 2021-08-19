
//  Ввод данных из уровнемера Sens ПМП-201

// Подключение
//
// A  RxD    синий
// Z  TxD    коричневый
// _L GND    экран

// COM  19200 8 0 1
int State_PMP[max_PMP+1]={0,0,0,0,0,0,0,0,0};
long int PMP_ADDR[max_PMP]={1,2,3,4,1,2,3,4,5};

#define PMP201

struct s_PMP_dev s_PMP[max_PMP]=
{
 0.1,     //  float Prod_lvl; // mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float `at_lvl;  // mm
 22.1,     // float Temp;     // C
 0,       // float VolLiq  ;
 0,       // float VolWat  ;
 0,       // float VolDif  ;
 0,       // float MassLiq ;
 0,       // float Mass    ;
 0,       // float DensLiq ;
 0,       // float DensGas ;
 0,       // float DensReal;
 0,       // float TempReal;
//==========================
 0.2,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 22.2,       // float Temp;     // C
 0,       // float VolLiq;
 0,       // float VolGas;
 0,       // float MassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========
//==========================
 0.3,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 22.3,   // float Temp;     // C
 0,       // float VolLiq;
 0,       // float VolGas;
 0,       // float MassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========
 0.4,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 22.4,   // float Temp;     // C
 0,       // float VolLiq;
 0,       // float VolGas;
 0,       // floatMassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========
 0.5,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 22.5,   // float Temp;     // C
 0,       // float VolLiq;
 0,       // float VolGas;
 0,       // float MassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========
 0.6,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 22.6,   // float Temp;     // C
 0,       // float VolLiq;
 0,       // float VolGas;
 0,       // float MassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========
 0.7,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 22.7,   // float Temp;     // C
 0,       // float VolLiq;
 0,       // float VolGas;
 0,       // float MassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========
 0.8,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 22.8,   // float Temp;     // C
 0,       // float VolLiq;
 0,       // float VolGas;
 0,       // float MassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========
 0.9,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 22.9,   // float Temp;     // C
 0,       // float VolLiq;
 0,       // float VolGas;
 0,       // float MassLiq;
 0,       // float MassGas;
 0,       // float Mass;
 0,       // float DensLiq;
 0,       // float DensGas;
 0,       // float DensReal;
 0,       // float TempReal;
//==========
};

//---------------
struct s_icp_dev PMP[max_PMP]=
{
// device 1
0,                // status
3,                // port
1,                // addr
"P201/1",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
1000,             // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&PMP_Rqst[0],   // *request
//--------------------------------
// device 2
0,                // status
3,                // port
2,                // addr
"P201/2",       // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
1000,             // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&PMP_Rqst[1],   // *request
//--------------------------------
// device 3
0,                // status
3,                // port
3,                // addr
"P201/3",       // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
1000,             // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&PMP_Rqst[2],   // *request
//--------------------------------
// device 4
0,                // status
3,                // port
4,                // addr
"P201/4",       // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
1000,             // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&PMP_Rqst[3],   // *request
//--------------------------------
// device 5
0,                // status
3,                // port
5,                // addr
"P201/5",       // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
1000,             // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&PMP_Rqst[4],   // *request
//--------------------------------
// device 6
0,                // status
3,                // port
6,                // addr
"P201/6",       // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
1000,             // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&PMP_Rqst[5],   // *request
//--------------------------------
// device 7
0,                // status
3,                // port
7,                // addr
"P201/7",       // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
1000,             // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&PMP_Rqst[6],   // *request
//--------------------------------
// device 8
0,                // status
3,                // port
8,                // addr
"P201/8",       // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
1000,             // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&PMP_Rqst[7],   // *request
//--------------------------------
// device 9
0,                // status
3,                // port
9,                // addr
"P201/9",       // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
1000,             // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&PMP_Rqst[8],   // *request
//--------------------------------

};
/*----------------------------*/
struct COM_rqst PMP_Rqst[max_PMP]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_PMP,        //  gets_com
f_PMP_rd,     //  answ_com
f_PMP_Flt,            //  answ_flt
"P201/1",            //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&PMP[0],            //  *ICP_dd
1,                    //  текущая функция
1000,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_PMP,        //  gets_com
f_PMP_rd,     //  answ_com
f_PMP_Flt,            //  answ_flt
"P201/2",            //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&PMP[1],            //  *ICP_dd
1,                //  текущая функция
1000,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
//--------------------------
// device 3
//--------------------------
0,                     //  состояние запроса
ffgets_com_PMP,        //  gets_com
f_PMP_rd,     //  answ_com
f_PMP_Flt,            //  answ_flt
"P201/3",            //  name
2,                     //  n_dev номер устройства в группе однотипных устройств
&PMP[2],            //  *ICP_dd
1,                //  текущая функция
1000,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------

//--------------------------
// device 4
//--------------------------
0,                     //  состояние запроса
ffgets_com_PMP,        //  gets_com
f_PMP_rd,     //  answ_com
f_PMP_Flt,            //  answ_flt
"P201/4",            //  name
3,                     //  n_dev номер устройства в группе однотипных устройств
&PMP[3],            //  *ICP_dd
1,                //  текущая функция
1000,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------



//----------------------
// device 5
//--------------------------
0,                     //  состояние запроса
ffgets_com_PMP,        //  gets_com
f_PMP_rd,     //  answ_com
f_PMP_Flt,            //  answ_flt
"P201/5",            //  name
4,                     //  n_dev номер устройства в группе однотипных устройств
&PMP[4],            //  *ICP_dd
1,                    //  текущая функция
1000,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 6
//--------------------------
0,                     //  состояние запроса
ffgets_com_PMP,        //  gets_com
f_PMP_rd,     //  answ_com
f_PMP_Flt,            //  answ_flt
"P201/6",            //  name
5,                     //  n_dev номер устройства в группе однотипных устройств
&PMP[5],            //  *ICP_dd
1,                //  текущая функция
1000,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
//--------------------------
// device 7
//--------------------------
0,                     //  состояние запроса
ffgets_com_PMP,        //  gets_com
f_PMP_rd,     //  answ_com
f_PMP_Flt,            //  answ_flt
"P201/7",            //  name
6,                     //  n_dev номер устройства в группе однотипных устройств
&PMP[6],            //  *ICP_dd
1,                //  текущая функция
1000,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
//--------------------------
// device 8
//--------------------------
0,                     //  состояние запроса
ffgets_com_PMP,        //  gets_com
f_PMP_rd,     //  answ_com
f_PMP_Flt,            //  answ_flt
"P201/8",            //  name
7,                     //  n_dev номер устройства в группе однотипных устройств
&PMP[7],            //  *ICP_dd
1,                //  текущая функция
1000,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
//--------------------------
// device 9
//--------------------------
0,                     //  состояние запроса
ffgets_com_PMP,        //  gets_com
f_PMP_rd,     //  answ_com
f_PMP_Flt,            //  answ_flt
"P201/9",            //  name
8,                     //  n_dev номер устройства в группе однотипных устройств
&PMP[8],            //  *ICP_dd
1,                //  текущая функция
1000,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------


};
/*----------------------------*/

/*----------------------------*/
void f_PMP(int ii)
{
  // ii - номер устройства
  // Функция шлет запрос на получение данных из устройства ICP PMPC
  //  ( 'M01') . Период посылки PMP[ii].pool_time мс.
  // В ответ будет передана информация .
  //M01 : 00001=0=+239=15528=1441=238

int i;
char buf_tmp[20];
long int i1;
int i2;

 if(PMP[ii].status == 0) return;

 if( f_timer(PMP[ii].time_stamp_pool,PMP[ii].pool_time ))
 {
  if((PMP_Rqst[ii].status == Req_Flt) ||
     (PMP_Rqst[ii].status == Req_OK)) goto m1;
  else if( f_timer(PMP[ii].time_stamp_pool,(PMP[ii].pool_time+PMP[ii].pool_time) ))
           goto m1;
   return;
m1:
//
//                       B5 01 04 2F 01 02 06 F0 2E
// index в буфере :      00 01 02 03 04 05 06 07 08
//                      PRE  A  L  C -- DATA---- CRC
//                           D  E  O
//                           D  N  M
//                           R  G  M
//                              T  A
//                              H  N
//                                 D


//
//                       B5 01 05 2F 01 02 06 08 F0 2E
// index в буфере :      00 01 02 03 04 05 06 07 08
//                      PRE  A  L  C -- DATA------- CRC
//                           D  E  O
//                           D  N  M
//                           R  G  M
//                              T  A
//                              H  N
//                                 D

           buf_tmp[0]=0xb5;
           buf_tmp[1]=(int)PMP[ii].addr;
           buf_tmp[2]=5;
           buf_tmp[3]=0x2f;
           buf_tmp[4]=1;
           buf_tmp[5]=2;
           buf_tmp[6]=6;
           buf_tmp[7]=8;
           buf_tmp[8]=0xf0;


           i1=9;
           i2=i1;

           if(f_prepare_PMP(buf_tmp,&i2)) return;
           _fmemcpy(PMP_Rqst[ii].Cmd,buf_tmp,(long)i2);


           PMP_Rqst[ii].cmd_lgth=i2;
           PMP_Rqst[ii].time_stamp=TimeStamp;
           PMP[ii].time_stamp_pool=TimeStamp;
           PMP[ii].n_transaction++;
           f_queue_put(PMP[ii].port, &PMP_Rqst[ii]);

            if(flag_prn)
            {
             printf("\n\r place in queue\n\r");
             printf("\n\r !!! COM %d",PMP[ii].port);

             printf("\n\r %2x %2x %2x %2x %2x %2x %2x %2x",
             buf_tmp[0],buf_tmp[1],buf_tmp[2],buf_tmp[3],buf_tmp[4],buf_tmp[5],buf_tmp[6],buf_tmp[7]);

             printf("\n\r %2x %2x %2x %2x %2x %2x %2x %2x",
             buf_tmp[8],buf_tmp[9],buf_tmp[10],buf_tmp[11],buf_tmp[12],buf_tmp[13],buf_tmp[14],buf_tmp[15]);

            }

 }
}
/*----------------------------*/

unsigned int PMP_inp_err[max_PMP]={0,0};  // счетчик ошибок ввода

// char buf_tmp11[50]=      " 00001=0=+239=15528=1441=238";
// char buf_tmp11[50]=        "00001=0=+239=15528=1441=238";
char buf_tmp11[50]=        "00001=0=+238=15528=1441=237";
//char buf_tmp11[50]=        "00001=1=+000=00000=0000=194";

int nnn=0;
void f_PMP_rd(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные
//
//
 struct COM_rqst *request;
 int i,itmp;
 long int ltmp1,ltmp2,ltmp3,ltmp4,ltmp5;
 unsigned char *buf;

//   strcpy(cb_COM[ii],buf_tmp11);
//   n_bcom[ii]=strlen(cb_COM[ii]);

if(flag_prn)
 {

          printf("\nf_PMP_rd");

 }


   request=COM_que[ii].request[COM_que[ii].empt_ptr];
   nnn=request->n_dev; // номер устройства PMP (0,1..)

   if(nnn < 0) nnn=0;
   if(nnn >= max_PMP ) nnn=max_PMP-1;

   if( f_check_crc_PMP(cb_COM[ii],ii) )
   {
 //в ответ на M01
 // приходит:

 //  b5  1  10  8f    1 25  2 40    2 f0 cc 41    6 d0 ff 3e     8 d0 ff 3e     f0  0  0  0
 //   0  1   2   3    4  5  6  7    8  9 10 11   12 13 14 15    16 17 18 19     20 21 22 23

 buf=cb_COM[ii];
 if(buf[0]  != 0xb5) goto m_err;
 if(buf[1]  != (PMP[nnn].addr & 0xff )  ) goto m_err;

 if(buf[2]  != 0x14) goto m_err;
 if(buf[4]  != 1  ) goto m_err;
 if(buf[8]  != 2  ) goto m_err;
 if(buf[12] != 6  ) goto m_err;
 if(buf[16] != 8  ) goto m_err;
 if(buf[20] != 0xf0  ) goto m_err;


            s_PMP[nnn].Prod_lvl = f_get_float_PMP(&buf[5])* 1000.;
            s_PMP[nnn].Temp     = f_get_float_PMP(&buf[9]);
//          s_PMP[nnn].DensReal  = f_get_float_PMP(&buf[13])* 1000.;
            s_PMP[nnn].DensLiq   = f_get_float_PMP(&buf[13])* 1000.;

//QQQQQQ
        s_PMP[nnn].Wat_lvl   = f_get_float_PMP(&buf[17]) * 1000.;
  //        s_PMP[nnn].Wat_lvl   = f_get_float_PMP(&buf[17]);
            State_PMP[nnn]= f_get_long_PMP(&buf[21]) & 0xffff;

            f_count_tank(nnn);


if(flag_prn)
{
          printf( "\r\n Prod_lvl=%f,Wat_lvl=%f,Dens=%f,Temp=%f ",
               s_PMP[nnn].Prod_lvl,s_PMP[nnn].Wat_lvl,s_PMP[nnn].DensReal  ,s_PMP[nnn].Temp );
          flag_prn=0;
}

    PMP[nnn].n_success++;
    PMP_inp_err[nnn]=0;
   }
   else
   {
m_err:
  //     printf("\n\r CRC error");
         PMP_inp_err[nnn]++;
         if(PMP_inp_err[nnn]>=PMP_max_inp_err)
         {
           PMP_inp_err[nnn]=PMP_max_inp_err;
           f_icp_error(&PMP[nnn],RD_ERR );
         }
    PMP[nnn].n_CRC_error++;
   }

   f_queue_free(ii,&PMP_Rqst[nnn]);
}



/*-------------------------------------------*/
/*-------------------------------------------*/
/*----------------------------*/

/*
void f_PMP1(int ii)
{
  // ii - номер устройства
  // Функция шлет запрос на получение данных из устройства ICP PMPC
  //  ( 'M01') . Период посылки PMP[ii].pool_time мс.
  // В ответ будет передана информация .
  //M01 : 00001=0=+239=15528=1441=238

int i;
char buf_tmp[20];
long int i1;
int i2;

 if(PMP[ii].status == 0) return;

 if( f_timer(PMP[ii].time_stamp_pool,PMP[ii].pool_time ))
 {
  if((PMP_Rqst[ii].status == Req_Flt) ||
     (PMP_Rqst[ii].status == Req_OK)) goto m1;
  else if( f_timer(PMP[ii].time_stamp_pool,(PMP[ii].pool_time+PMP[ii].pool_time) ))
           goto m1;
   return;
m1:
//
//                       B5 01 04 2F 01 02 06 F0 2E
// index в буфере :      01 23 45 67 89 AB CD EF 0
//                      PRE  A  L  C -- DATA---- CRC
//                           D  E  O
//                           D  N  M
//                           R  G  M
//                              T  A
//                              H  N
//                                 D

           buf_tmp[0]=0xb5;
//           buf_tmp[1]=(int)PMP_ADDR[ii];
           buf_tmp[1]=(int)PMP[ii].addr;
           buf_tmp[2]=4;
           buf_tmp[3]=0x2f;
           buf_tmp[4]=1;
           buf_tmp[5]=2;
           buf_tmp[6]=6;
           buf_tmp[7]=0xf0;


           i1=8;
           i2=i1;

           if(f_prepare_PMP(buf_tmp,&i2)) return;
           _fmemcpy(PMP_Rqst[ii].Cmd,buf_tmp,(long)i2);


           PMP_Rqst[ii].cmd_lgth=i2;
           PMP_Rqst[ii].time_stamp=TimeStamp;
           PMP[ii].time_stamp_pool=TimeStamp;
           PMP[ii].n_transaction++;
           f_queue_put(PMP[ii].port, &PMP_Rqst[ii]);

 // qqqqq
            if(flag_prn)
            {
             printf("\n\r place in queue\n\r");
             printf("\n\r !!! COM %d",PMP[ii].port);

             printf("\n\r %2x %2x %2x %2x %2x %2x %2x %2x",
             buf_tmp[0],buf_tmp[1],buf_tmp[2],buf_tmp[3],buf_tmp[4],buf_tmp[5],buf_tmp[6],buf_tmp[7]);

             printf("\n\r %2x %2x %2x %2x %2x %2x %2x %2x",
             buf_tmp[8],buf_tmp[9],buf_tmp[10],buf_tmp[11],buf_tmp[12],buf_tmp[13],buf_tmp[14],buf_tmp[15]);
            }

 }
}

*/
/*----------------------------*/
/*

void f_PMP_rd1(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные
//
//
 struct COM_rqst *request;
 int i,nn,itmp;
 long int ltmp1,ltmp2,ltmp3,ltmp4,ltmp5;
 unsigned char *buf;

//   strcpy(cb_COM[ii],buf_tmp11);
//   n_bcom[ii]=strlen(cb_COM[ii]);

   request=COM_que[ii].request[COM_que[ii].empt_ptr];
   nn=request->n_dev; // номер устройства PMP (0,1..)

   if(nn < 0) nn=0;
   if(nn >= max_PMP ) nn=max_PMP-1;

   if( f_check_crc_PMP(cb_COM[ii],ii) )
   {
 //в ответ на M01
 // приходит:

 //  b5  1  10  8f    1 25  2 40    2 f0 cc 41    6 d0 ff 3e    f0  0  0  0
 //   0  1   2   3    4  5  6  7    8  9 10 11   12 13 14 15    16 17 18 19

 buf=cb_COM[ii];
 if(buf[0]  != 0xb5) goto m_err;
// if(buf[1]  != (PMP_ADDR[nn] & 0xff )  ) goto m_err;
 if(buf[1]  != (PMP[nn].addr & 0xff )  ) goto m_err;

 if(buf[2]  != 0x10) goto m_err;
 if(buf[4]  != 1  ) goto m_err;
 if(buf[8]  != 2  ) goto m_err;
 if(buf[12] != 6  ) goto m_err;
 if(buf[16] != 0xf0  ) goto m_err;



            s_PMP[nn].Prod_lvl = f_get_float_PMP(&buf[5])* 1000.;
            s_PMP[nn].Temp     = f_get_float_PMP(&buf[9]);
            s_PMP[nn].DensReal  = f_get_float_PMP(&buf[13])* 1000.;


            State_PMP[nn]= f_get_long_PMP(&buf[17]) & 0xffff;

            f_count_tank(nn);


if(flag_prn)
{
          printf( "\r\n Prod_lvl=%f,Wat_lvl=%f,Dens=%f,Temp=%f ",
               s_PMP[nn].Prod_lvl,s_PMP[nn].Wat_lvl,s_PMP[nn].DensReal  ,s_PMP[nn].Temp );
       // flag_prn=0;
}

    PMP[nn].n_success++;
    PMP_inp_err[nn]=0;
   }
   else
   {
m_err:
  //     printf("\n\r CRC error");
         PMP_inp_err[nn]++;
         if(PMP_inp_err[nn]>=PMP_max_inp_err)
         {
           PMP_inp_err[nn]=PMP_max_inp_err;
           f_icp_error(&PMP[nn],RD_ERR );
         }
    PMP[nn].n_CRC_error++;
   }

   f_queue_free(ii,&PMP_Rqst[nn]);
}

*/
/*-------------------------------------------*/

/*-------------------------------------------*/
/*-------------------------------------------*/
void f_PMP_Flt(int ii)
{
 struct COM_rqst *request;

 int lgth,i;
 char *buf_ttmp;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 nnn=request->n_dev; // номер устройства PMP (0,1..)
 PMP[nnn].time_stamp_rcv=0;
 PMP[nnn].n_timeout_error++;
 PMP_inp_err[nnn]++;

 if(PMP_inp_err[nnn]>=PMP_max_inp_err)
 {
   PMP_inp_err[nnn]=PMP_max_inp_err;
   f_icp_error(&PMP[nnn],RD_ERR );
 }
// f_queue_free(ii,&PMP_Rqst[nnn]);
 f_queue_Flt(ii);

if(flag_prn)
 {

          printf("\nf_PMP_Flt");

 }
}
/* ---------------------------------------- */
int f_prepare_PMP(char *Cmd,int *length)
{ //  функция вычисляет конторольную
  //  сумму команды в буфере Cmd
  //  и записывает  ее после команды.
  //  Возвращает 0 - OK,
  //             1 - нет места в буфере

unsigned int sum;
int i,i1;

 i1=*length;

   if( i1 >= (n_buf_queue-3)) return 1;
   sum=0;
   for(i=1;i<i1;i++)
    sum+=Cmd[i];
  Cmd[i]=sum;
  *length+=1;
  return 0;

}
/*----------------------------*/
/* ---------------------------------------- */
int f_check_crc_PMP(char *buf,int ii)
{ //return 1 - OK

int lgth,i,i1,i_ret;
unsigned int summ,summ1,summ2,summ3;

 char *buf_ttmp;
 char *buf_tmp;

 //в ответ на M01
 // приходит:      00001=0=+239=15528=1441=238


           summ=0;
           lgth=n_bcom[ii]-1;

           for(i=1;i<lgth;i++)
           {
            summ+= (buf[i]) & 0xff;
           }


          i_ret=0;
          if( (summ & 0xff) == (buf[lgth] & 0xff) ) i_ret=1;


//------------------------------------------
//qqqqq
if(flag_prn)
 {
       buf_ttmp=buf;


       printf("\r\n RECEIVED:");
       if(i_ret != 0)
          printf("OK:");
       else
          printf("FAULT:");

       printf("\r\n lgth=%d\r\n",lgth);
       for(i=0;i<lgth;i++)
       {
         printf("%2x ", ((int)buf_ttmp[i])& 0xff  );
         if( (i%16)== 7) printf("  ");
         if( (i%16)== 15) printf("\r\n");
       }

//     flag_prn=0;
 }

//------------------------------------------

       return i_ret;
  //        return 1;
}
/* ---------------------------------------- */
#define n_PMP_min 3

int ffgets_com_PMP( int ii)
{
//  Адрес функции будет устанавлен в указателе  ff_gets_com[Port]
// при посылке команды в устройство Modbus RTU.
//  Функция принимает символы из порта COM ii  в буфер cb_COM[ii].
//  При таймауте команда считается введенной и функция возвращает
// значение 1.
//

/* ii - номер COM   1...4 */
 char tmp;
m1:
     if(IsCom(ii)==0)
     {
         if( f_timer(time_snd[ii],ToutAns[ii]))
         { // истек таймаут приема ответа на команду

            if(n_bcom[ii] < n_PMP_min)
              ff_answ_com[ii]=ff_answ_flt[ii];

            return 1;
         }
         return 0;
     }
     tmp=ReadCom(ii);
     ToutAns[ii]=byte_timout_rtu[ii];

     cb_COM[ii][n_bcom[ii]]=tmp;
     if( (++n_bcom[ii]) >= n_bufcom )
      {
        n_bcom[ii]=0;
  /*
        printf("\n Command buffer overflow !\n--");
        puts(cb_COM1);
  */
      }
   time_snd[ii]= TimeStamp;
   goto m1;
}
//-----------------------------
float f_get_float_PMP(unsigned char *buf)
{
 union  { float f; char ch[4]; unsigned long l; } o;

     o.ch[0]=0;
     o.ch[1]=buf[0];
     o.ch[2]=buf[1];
     o.ch[3]=buf[2];

  if(( o.l == 0x7fa00000) ||( o.l == 0xffffff00) || ( o.l == 0x00007fa0))

     o.f= 0.00001;

     return o.f;

}
/* ---------------------------------------- */
long int f_get_long_PMP(unsigned char *buf)
{
 union  { long int l; char ch[4]; } o;

    //             [3][4][5][6]
     o.ch[3]=0;
     o.ch[0]=buf[0];
     o.ch[1]=buf[1];
     o.ch[2]=buf[2];
     return o.l;

}
