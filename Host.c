

int EchoComN=0;
union {unsigned long int l;
unsigned int i[2];
} ul;
unsigned long int *Flash_ptr;
unsigned int seg=0xc000,offset=0;
int AddrHost=0;
int ComPortHost=4;
long int BaudHost=115200L;
long int ttct1,ttct2;

int ComPortEcho=1;
int CRC_flag_Echo=1;
int OUT_VAR=0,INP_VAR=0;

int flag_echo_host=1;
int flag_Deb=0;
int flag_host_send=0; //1 - у Host есть что послать Slave
int msk_int[]={0,1,2,4,8,0x10,0x20,0x40,0x80,0x100,0x200,
0x400,0x800,0x1000,0x2000,0x4000,0x8000};

float cost_v=1000;       // Unit price
float amount_f=0;        // Amount

float volume_f=0;        // Volume execute
float vol_f=0;           // Volume execute

float summ_v=1000;       // Amount Current value

char *list_pt[]={
"pressure",
"temperature ",
};

// unsigned long int adr_strt= 0xfffff0L;

unsigned long int adr_strt= 0xffff0L; //-- WORK
//---------------------------------
struct s_icp_dev Host=
{
// device 1
0,                // status
1,                // port
0,                // addr
"Host",           // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"V1.1",           // firmware
0,                // protocol
1,                // CRC_flag
150,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&Host_Rqst        // *request
};
//--------------------------------
//---------------------------------
struct COM_rqst Host_Rqst={
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_send_to_Host,        //  answ_com
f_empty_ans_echo,      //  answ_flt
"Host",                //  name
0,                     //  n_dev
&Host,                 //  *ICP_dd
0,                     //  текущая функция
ToutAnsHost,           //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
struct COM_rqst Host_Rqst_rtu={
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_send_to_Host_rtu,    //  answ_com
f_send_to_Host_rtu,    //  answ_flt
"Host",                //  name
0,                     //  n_dev
&Host,                 //  *ICP_dd
0,                     //  текущая функция
ToutAnsHost_rtu,       //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
struct COM_rqst Host_Rqst_sk={
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_send_to_Host_sk,    //  answ_com
f_send_to_Host_sk,    //  answ_flt
"Host",                //  name
0,                     //  n_dev
&Host,                 //  *ICP_dd
0,                     //  текущая функция
ToutAnsHost_rtu,       //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
#define max_wd 2
//---------------
struct s_icp_dev WDOG[max_wd]=
{
// device 1
0,                // status
1,                // port
0,                // addr
"WDOG/1",         // name[8]
06,               // baudrate_cod
00,               // type
0,                // format
0,                // channels
"V1.1",           // firmware
0,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&WDOG_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
2,                // port
0,                // addr
"WDOG/2",         // name[8]
06,               // baudrate_cod
00,              // type
00,             // format
0x0,             // channels
"V1.1",           // firmware
0,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&WDOG_Rqst[1],   // *request
};
/*----------------------------*/
/*----------------------------*/
struct COM_rqst WDOG_Rqst[max_wd]={
//---------------------------------------
0,                     //  состояние запроса
ffgets_no_com_ICP,     //  gets_com
f_WD_end,              //  answ_com
f_WD_end,              //  answ_flt
"WDOG/1",              //  name
0,                     //  n_dev
&WDOG[0],              //  *ICP_dd
0,                     //  текущая функция
2,                     //  timeout
0,                     //  time_stamp
6,                     //  com_lgth
1,                     //  CRC_flag
"~**D2\r",             //  com[]
//-------------------------------------
//---------------------------------------
0,                     //  состояние запроса
ffgets_no_com_ICP,     //  gets_com
f_WD_end,              //  answ_com
f_WD_end,              //  answ_flt
"WDOG/2",              //  name
1,                     //  n_dev
&WDOG[1],              //  *ICP_dd
0,                     //  текущая функция
2,                     //  timeout
0,                     //  time_stamp
6,                     //  com_lgth
1,                     //  CRC_flag
"~**D2\r"              //  com[]
//-------------------------------------
};
/*----------------------------*/
void f_WDOG(int ii)
{
  //  ii - номер устройства
  //  Функция передает команду HOST OK ( '~**') .
  //  Период посылки WDOG[ii].pool_time мс.
  //  После передачи ожидается опорожнение буфера и задержка
  //  WDOG_Rqst[ii].timeout мс

int i,fn,sw;
char buf_tmp[20];

//  if(WDOG[ii].status == 0) return;

  if(( WDOG_Rqst[ii].status == Req_OK)||( WDOG_Rqst[ii].status == Req_Flt))
  {
   if( f_timer( WDOG[ii].time_stamp_pool, WDOG[ii].pool_time ))
    {
m1:
     strcpy(buf_tmp,"~**");
     if(f_prepare7000(buf_tmp, WDOG[ii].CRC_flag)) return;
     strcpy( WDOG_Rqst[ii].Cmd,buf_tmp);
     WDOG_Rqst[ii].cmd_lgth=strlen(buf_tmp);
     WDOG_Rqst[ii].time_stamp=TimeStamp;
     WDOG[ii].time_stamp_pool=TimeStamp;
     WDOG[ii].n_transaction++;
     f_queue_put( WDOG[ii].port, &WDOG_Rqst[ii]) ;
    }
  }
  else
   if( f_timer( WDOG[ii].time_stamp_pool, WDOG[ii].pool_time+WDOG[ii].pool_time ))
   {
     if(f_queue_chk(WDOG[ii].port) ) goto m1;
   }
  return;
}
/*----------------------------*/

int host_wd_sw[5]={0,0,0,0,0};
int ffgets_no_com_ICP( int ii)
{
//  Адрес функции будет устанавлен в указателе  ff_gets_com[Port]
//  при посылке команды ~**  в устройства ICP (  7060 и пр.).
//  Функция не ожидает ответа .
//  Если прошло время WDOG_Rqst[ii].timeout мс с момента окончания
// передачи последнего символа функция успешно завершается
//

/* ii - номер COM   1...3 */


   if( !IsTxBufEmpty(ii) )
   {
       host_wd_sw[ii] =0;
       return 0;
   }
   if(host_wd_sw[ii] != 1 )
   {
       time_snd[ii]=TimeStamp;
       host_wd_sw[ii]=1;
   }
   else if (host_wd_sw[ii] == 1 )
   {
        if( f_timer(time_snd[ii],ToutAns[ii]))
        { // истекло время ожидания

           host_wd_sw[ii]=0;
           return 1;
        }
   }
   return 0;
}
/* --------------------------- */
void  f_WD_end (int ii)
{
 struct COM_rqst *request;
 int nn;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства  (0,1..)
  WDOG[nn].n_success++;
  f_queue_free(ii,&WDOG_Rqst[nn]);

}
/* --------------------------- */
int f_123( int n)

/* Проверяет является ли n-ный символ в буфере именем координаты,
   если нет - возвращает -1  , иначе номер координаты по порядку */

{
int i;
           switch(intrpr.wrd[n-1])
           {
            case '1': i=0; goto m_k1;
            case '2': i=1; goto m_k1;
            case '3': i=2; goto m_k1;
            case '4': i=3; goto m_k1;
            case '5': i=4; goto m_k1;
            case '6': i=5; goto m_k1;
            case '7': i=6; goto m_k1;
            case '8': i=7; goto m_k1;
            case '9': i=8; goto m_k1;
            case 'A': i=9; goto m_k1;
            case 'B': i=10; goto m_k1;
            case 'C': i=11; goto m_k1;
            case 'D': i=12; goto m_k1;
            case 'E': i=13; goto m_k1;
            case 'F': i=14; goto m_k1;

			case 'G': i=15; goto m_k1;
            case 'H': i=16; goto m_k1;
            case 'I': i=17; goto m_k1;
            case 'J': i=18; goto m_k1;
            case 'K': i=19; goto m_k1;
           }
           return -1;
   m_k1:   return i;
}
/*----------------------------------------------*/
/*----------------------------*/
int f_host()
{
//  int  hour,min,sec;
     // вводит команду из Host (PC) и интерпретирует ее
         if(ffgets_host(ComPortHost))
         {
          strcpy(intrpr.bfcom,cb_COM[ComPortHost]);
          n_bcom[ComPortHost]=0;
          return  f_intr();
         }
         return 0;
}
/*-------------------------------------------*/
void f_send_to_Host(int ii)
{
/* пересылает принятые из порта ii данные в Host
*/

// char *Result;
// int j,key,nextidx;
// int count;
//  count=n_bcom[ii];  // длина команды

 if(flag_echo_host )
     puts("   ");
 puts(cb_COM[ii]);
 putch('\n\r');
 f_queue_free(ii,&Host_Rqst);
 f_prompt();
}
/*-------------------------------------------*/
void f_prompt()
{
 if(flag_echo_host )
    if(( EchoComN==0)||( EchoComN==4))
     printf("\n\r>");
    else
     printf("\n\rCOM%d>",ComPortEcho);
}

/*-------------------------------------------*/
void f_empty_ans_echo(int ii)
{ // таймаут по ответу

 int count,i;

 if(flag_echo_host )
 {
     printf(" -?- ");
 }
 f_prompt();
 f_queue_free(ii,&Host_Rqst);
}
/*-------------------------------------------*/
/*
int ffgets_com_rtu( int ii)
{
//  Адрес функции будет устанавлен в указателе  ff_gets_com[Port]
// при посылке команды в устройство Modbus RTU.
//  Функция принимает символы из порта COM ii  в буфер cb_COM[ii].
//  При таймауте команда считается введенной и функция возвращает
// значение 1.
//

// ii - номер COM   1...4
 char tmp;
m1:
     if(IsCom(ii)==0)
     {
        if( f_timer(time_snd[ii],ToutAns[ii]))
        { // истек таймаут приема ответа на команду
          //   ff_answ_com[ii]=ff_answ_flt[ii];
           return 1;
        }
        return 0;
      }
     tmp=ReadCom(ii);
     time_snd[ii]= TimeStamp;

// if(ComProt[ii]==RTU)
//     printf(" &%02X", tmp & 0xff);

     cb_COM[ii][n_bcom[ii]]=tmp;
     if( (++n_bcom[ii]) >= n_bufcom )
      {
        cb_COM[ii][n_bufcom-1]=0;
        n_bcom[ii]=0;
  //
  //    printf("\n\r Command buffer overflow !\n\r--");
  //    puts(cb_COM1);
  //
      }
   goto m1;
}
*/
/* ---------------------------------------- */
/*-------------------------------------------*/
#define max_rd_str 40


void f_send_to_Host_sk(int ii)
{
/* пересылает принятые из SK или MVD данные в Host
*/
 // ii - номер COM порта

 struct COM_rqst *request;
 int i,im,fn,i1;
 char buf_tmp[max_rd_str];
 int count;
 unsigned long int val;
 union  { float f; unsigned long l; } o;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  fn=request->function;

  count=n_bcom[ii];  // длина команды

  if(count<3) goto m_err;

  if(f_check_crc_RTU(cb_COM[ii],count)==0)
     goto m_err;
 switch(fn)
 {
  case RD_INT:
     if(cb_COM[ii][1] != 3) goto m_err;
     if(cb_COM[ii][2] != 2) goto m_err;
     val=(  (unsigned long int)cb_COM[ii][4] & 0x00ff) |
         ( ((unsigned long int)cb_COM[ii][3] << 8) & 0x00ff00) ;
     printf("  %ld (0x%lX)",val,val);
     break;
  case RD_LONG:
     if(cb_COM[ii][1] != 3) goto m_err;
     if(cb_COM[ii][2] != 4) goto m_err;
     val=
         ( (unsigned long)(cb_COM[ii][6]) & 0x000000ff) |
         ( ((unsigned long)cb_COM[ii][5] << 8) &0xff00) |
         ( ((unsigned long)cb_COM[ii][4] << 16) &0xff0000)  |
         ( ((unsigned long)cb_COM[ii][3] << 24) &0xff000000);
     printf(" %ld (0x%lX)",val,val);
     break;
  case RD_COIL:
     if(cb_COM[ii][1] != 1) goto m_err;
     im=cb_COM[ii][2];
     i1=3;
     for(i=0;i<im ;i++,i1++)
     {
       printf(" %02X",cb_COM[ii][i1]&0xff);
     }
     break;
  case RD_FLOAT:
      //
     if(cb_COM[ii][1] != 3) goto m_err;
     if(cb_COM[ii][2] != 4) goto m_err;

     o.f=f_get_float( cb_COM[ii], 1);
/* ============================
     switch(MVD_fl_ord[0])  // ( Reg 0521 )
     {
     case 0:
    //  code == 0 : 0  1  2  3
    //             [3][4][5][6]
     o.l=
         ( (unsigned long)cb_COM[ii][6]  &0xff) |                // 3
         ( ((unsigned long)cb_COM[ii][5] << 8) &0xff00) |        // 2
         ( ((unsigned long)cb_COM[ii][4] << 16) &0xff0000)  |    // 1
         ( ((unsigned long)cb_COM[ii][3] << 24) &0xff000000);    // 0
        break;
     case 1:
    //  code == 1 : 2  3  0  1
    //             [3][4][5][6]
     o.l=
         ( (unsigned long)cb_COM[ii][4]  &  0x000000ff) |         // 3
         ( ((unsigned long)cb_COM[ii][3] << 8) &  0x0000ff00) |   // 2
         ( ((unsigned long)cb_COM[ii][6] << 16) & 0x00ff0000)  |  // 1
         ( ((unsigned long)cb_COM[ii][5] << 24) & 0xff000000);    // 0
         break;
      case 2:
    //  code == 2 : 1  0  3  2
    //             [3][4][5][6]

     o.l=
         ( (unsigned long)cb_COM[ii][5]  &0xff) |              // 3
         ( ((unsigned long)cb_COM[ii][6] << 8) &0xff00) |      // 2
         ( ((unsigned long)cb_COM[ii][3] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)cb_COM[ii][4] << 24) &0xff000000);  // 0
         break;
      case 3:
    //  code == 3 : 3  2  1  0
    //             [3][4][5][6]
     o.l=( (unsigned long)(cb_COM[ii][3]) & 0x000000ff) |      // 3
         ( ((unsigned long)cb_COM[ii][4] << 8) &0xff00) |      // 2
         ( ((unsigned long)cb_COM[ii][5] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)cb_COM[ii][6] << 24) &0xff000000);  // 0
       break;
     }
======================*/
//  printf(">> %02X",cb_COM[ii][3]);
//  printf(" %02X",cb_COM[ii][4]);
//  printf(" %02X",cb_COM[ii][5]);
//  printf(" %02X<<\n\r",cb_COM[ii][6]);

    if(( o.l == 0x7fa00000) || ( o.l == 0x00007fa0))
       printf(" !bad process variable (%08lX)",o.l);
    else
     //  printf(" %9g",o.f);
       printf(" %f",o.f);
     //printf(" %g (%08lX)",o.f,o.l);
    break;

  case RD_STRING:

     if(cb_COM[ii][1] != 3) goto m_err;
     im=cb_COM[ii][2];
     i1=3;
     for(i=0;(i<im)&&(i< (max_rd_str-1)) ;i++,i1++)
     {
      buf_tmp[i]=cb_COM[ii][i1];
     }
     buf_tmp[i]=0;
     printf(" %s",buf_tmp);
     break;

  case WR_INT:
    if(cb_COM[ii][1] != 6) goto m_err;
    break;
  case WR_LONG:
  case WR_FLOAT:
  case WR_STRING:
    if(cb_COM[ii][1] != 16) goto m_err;
    break;

  case WR_COIL:
    if(cb_COM[ii][1] != 5) goto m_err;
    break;

 default: goto m_err;
 }
     f_prompt();
     f_queue_free(ii,&Host_Rqst_sk);
     return;

    // ничего не послано в ответ
m_err:


    if(cb_COM[ii][1] & 0x80 )
    {
      if(cb_COM[ii][2]==1) {printf(" !! Illegal function");goto m2;}
      if(cb_COM[ii][2]==2) {printf(" !! Illegal data address");goto m2;}
      if(cb_COM[ii][2]==3) {printf(" !! Illegal data value");goto m2;}
      if(cb_COM[ii][2]==6) {printf(" !! Busy, rejected message");goto m2;}
    }
/*
    if(count>0) printf(" !! ");
    for(i=0;i<count;i++)
     printf(" %02X",cb_COM[ii][i]&0xff);
*/

      printf(" -?- ");
m2:
     f_prompt();
     f_queue_free(ii,&Host_Rqst_sk);
}
/*-------------------------------------------*/
void f_send_to_Host_rtu(int ii)
{
/* пересылает принятые из порта ii данные в Host
*/

// char *Result;
// int j,key,nextidx;
// int count;
//  count=n_bcom[ii];  // длина команды

 int count,i;

 count=n_bcom[ii];  // длина команды

 if(flag_echo_host )
 {
   if(count)
   {
       puts("   ");
    for(i=0;i<count;i++)
       printf(" %02X",(cb_COM[ii][i])&0xff );
       f_prompt();
//      printf("\n\rCOM%d>",ComPortEcho);
   }
   else
    { // ничего не послано в ответ
       printf(" -?- ");
       f_prompt();
    }
 }
 else
 {
  for(i=0;i<count;i++)
     putch(Host_Rqst.Cmd[i]);
 }

 f_queue_free(ii,&Host_Rqst_rtu);
}
/*-------------------------------------------*/
int (*ff_putch[5])(int ch)= {ToCom1,ToCom1,ToCom2,ToCom3,ToCom4};

int ffgets_host( int port)
{
//  Функция принимает команду от Host контроллера (PC)
//  выходная строка  cb_COM[port];
//  длина строки   n_bcom[port];

 char tmp;
m1:
 if(!IsCom(port))
 {
  return 0;
 }
 tmp=ReadCom(port);
   if( tmp == '\b')
    {
     if(n_bcom[port])
      {
       if(flag_echo_host )
        {
         (*ff_putch[port])(tmp);
         (*ff_putch[port])(' ');
         (*ff_putch[port])(tmp);
        }
       n_bcom[port]--;
      }
     goto m1;
    }
   else
    {
//     if ((tmp >= 'a') && (tmp <= 'z')) tmp &= 0xdf;
     cb_COM[port][n_bcom[port]]=tmp;

     if( (++n_bcom[port]) >= n_bufcom )
      {
        cb_COM[port][n_bufcom-1]=0;
        n_bcom[port]=n_bufcom-1;
      }
    }
   switch(tmp)
   { // указать все терминаторы
 //   case 0x0:
 //   case ';':
    case '\n':
    case '\r':
    n_bcom[port]--;
    cb_COM[port][n_bcom[port]]=0;
    return 1;
   }
   if(flag_echo_host)
         (*ff_putch[port])(tmp);
   goto m1;
  }

/* ================================================== */
/* ------------------------------------------- */
struct intr_r intrpr={0,0,0,0,0,0,0,0,0};
/* ========================================================= */
int f_inw()

/*   Из строки intrpr.bfcom   вводится слово в строку intrpr.wrd
   Начало анализа начинается с символа с индексом  intrpr.i3.
     Начальные пробелы и разделители игнорируются , слово вводится до первого
   пробела или разделителя за непробельным символом .
     Указатель  intrpr.i1 устанавливается на первый символ введенного
   слова в intrpr.bfcom.
     Указатель  intrpr.i2 устанавливается на последний символ введенного
   слова в intrpr.bfcom.
     Указатель  intrpr.i3 устанавливается на первый символ не пробел и не
   запятая  в intrpr.bfcom.
     В intrpr.wdl - длина введенного слова .
     Возвращается 0 если нет слова и достигнут конец строки  иначе 1.
*/

{
        intrpr.i1=intrpr.i3;
        intrpr.wdl=0;

//      printf("\n\r f_inw(),intrpr.i3=%d",intrpr.i3);

sw0:    switch(intrpr.bfcom[intrpr.i1])
        {
         case ' ':
         case 0x9:
         case ',':
         case ':':
         case ';':
         case '>':
         case '"':
         case '<':
         case '=':
         case '\n\r':
         case '\r': intrpr.i1++;goto sw0;

/*       case '*':  */
         case 0  :
//     printf("\n\r f_inw(),return(0),intrpr.i1=%d,intrpr.i1=%d,intrpr.i3=%d",intrpr.i1,intrpr.i2,intrpr.i3);
              return 0;
        }

        intrpr.i2=intrpr.i1;

sw1:    switch(intrpr.bfcom[intrpr.i2])
        {
         case ' ':
         case 0x9:
         case ',':
         case ':':
         case ';':
         case '=':
         case '\r':
         case '\n\r':
         case 0  : goto m3;
        }

         intrpr.wrd[intrpr.wdl]=intrpr.bfcom[intrpr.i2];
         if(++intrpr.wdl==wrd_w)  intrpr.wdl-- ;
         intrpr.i2++;
         goto sw1;

m3:      intrpr.i3=intrpr.i2;
         intrpr.i2--;
         intrpr.wrd[intrpr.wdl]=0;

sw2:    switch(intrpr.bfcom[intrpr.i3])
        {
         case ',':
         case ' ':
         case 0x9:
         case '\n\r':
         case '\r': intrpr.i3++;goto sw2;
        }

//      printf("\n\r f_inw(),return(1),intrpr.i3=%d",intrpr.i3);
        return 1;

}
/* ========================================================= */
int f_innum(char *strg)
/* вводит число в intrr[pp].num_var  */
{
    char cp;
    char *strg1;
    char *stopptr;
    float tmp;
    int i1;
    tmp=1.0;

//---------------------
            i1=0;
            if( strg[0] == '-')
             {
               tmp=-1.0;
               i1=1;
             }
            else if( strg[0] == '~')
             {
               tmp=-2.0;
               i1=1;
             }
//---------------------

    if( (strg[i1] == '0') && (strg[i1+1] == 'x')  || (strg[i1+1] == 'X') )
    {
      strg1=&strg[i1+2];
      while(*strg1)
       {
         cp= *strg1++;
         if( isxdigit(cp) );
         else return 0;
       }

      strg1=&strg[i1];
      if(tmp!=-2.0)
        intrpr.num_var=tmp*(float)strtoul(strg1,&stopptr,16);
      else
       {
        intrpr.num_var=(~(strtoul(strg1,&stopptr,16))) & HEX_MAX; ;
       }

      return 1;
    }
    else
    {
      strg1=&strg[i1];
      while(*strg1)
       {
         cp= *strg1++;
         if( isdigit(cp) || (cp=='+')||(cp=='.')||(cp==' ') || (cp=='-'));
         else return 0;
       }
      strg1=&strg[i1];
      if(tmp!=-2.0)
       {
//       intrpr.num_var = tmp *(float)atof(strg1);
         intrpr.num_var = tmp *(float)strtod(strg1,NULL);

       }
      else
       {
       //  tmp= (float)atof(strg1);
         tmp= (float)strtod(strg1,NULL);

          intrpr.num_var=( ( ~((int)tmp)) ) & HEX_MAX;
       }
    }
    return 1;
}
/*---------------------------------------------------------------------*/
int f_inhex(char *strg)
/* вводит hex число в intrr[pp].num_var  */
{
    char cp;
    char *strg1;
    char *stopptr;

      strg1=strg;

      while(*strg1)
       {
         cp= *strg1++;
         if( isxdigit(cp) );
         else return 0;
       }

      strg1=strg;
      intrpr.num_var=(float)strtoul(strg1,&stopptr,16);

    return 1;
}
/*---------------------------------------------------------------------*/
/*----------------------------------------*/
float f_lim(float vvr,float lim_low,float lim_high)
{
  if(vvr > lim_high) vvr=lim_high;
  else if(vvr < lim_low) vvr=lim_low;
  return vvr;
}
/*----------------------------------------*/
/*---------------------------------------------------*/
/* Отображение/ввод значений */
int f_dis_set( struct dis_set *ds_mas,int ib,int nn)
/*
      ib - длина команды
      nn - количество параметров
*/
{
int i,i_rez,i1;
char *strg;
char str_tmp[15];
long int l_tmp;
double dbl_tmp;

    i_rez=0;
    if(intrpr.wdl==ib)
       if(f_inw()) strg=intrpr.wrd;
       else
        {
         for(i=0;i<nn;i++)
         switch(ds_mas[i].type)
         {
          case T_FLOAT:
             printf(" %g", *(float*)ds_mas[i].op );
//           printf(" %f", *(float*)ds_mas[i].op );
             break;
          case T_INT:
             printf(" %d", *(int*)ds_mas[i].op );
             break;
          case T_INT_L:
             printf(" %ld", *(long int*)ds_mas[i].op );
             break;
          case T_HEX:
             printf(" %X", *(int*)ds_mas[i].op );
             break;
          case T_HEX_L:
             printf(" %08lX", *(long int*)ds_mas[i].op );
             break;
          case T_HX2:
             printf(" %02X", *(int*)ds_mas[i].op );
             break;
          case T_HX4:
             printf(" %04X", *(int*)ds_mas[i].op );
             break;
          case T_STR:
             printf(" %s", (char*)ds_mas[i].op );
             break;
          case T_DBL:
             printf(" %lg", *(double*)ds_mas[i].op );
             break;

         }
         return 0;
        }
    else
        strg=&intrpr.wrd[ib];


    switch(ds_mas[0].type)
     {
         case T_FLOAT:
         case T_INT:
          i1=f_innum(strg);
          break;
         case T_INT_L:
          i1=sscanf(strg,"%ld",&l_tmp);
          break;
          case T_DBL:
          i1=sscanf(strg,"%lg",&dbl_tmp);
          break;
        case T_HEX_L:
          i1=sscanf(strg,"%lx",&l_tmp);
          break;
        case T_HEX:
        case T_HX2:
        case T_HX4:
          i1=f_inhex(strg);
          break;
        case T_STR:
          i1=1;
          break;
     }
    if(i1)
     {
        switch(ds_mas[0].type)
       {
        case T_FLOAT:
          *(float*)ds_mas[0].op= (float)f_lim(intrpr.num_var,ds_mas[0].low_l,ds_mas[0].high_l);
          break;
        case T_INT:
        case T_HEX:
        case T_HX2:
        case T_HX4:
          *(int*)ds_mas[0].op= (int)f_lim(intrpr.num_var,ds_mas[0].low_l,ds_mas[0].high_l);
          break;
          case T_DBL:
          *(double*)ds_mas[0].op= dbl_tmp;
          break;

        case T_INT_L:
        case T_HEX_L:
             if((float)l_tmp < ds_mas[0].low_l) l_tmp =ds_mas[0].low_l;
             else if((float)l_tmp > ds_mas[0].high_l) l_tmp=ds_mas[0].high_l;
             *(long int*)ds_mas[0].op=l_tmp;
          break;
        case T_STR:
          strcpy((char*)ds_mas[0].op, strg);
          break;
       }
       i_rez++;
     }
    else goto dis_e;

    if (nn==1) return i_rez;

    for(;i_rez < nn;i_rez++)
     {
       if(!f_inw()) return i_rez;

       switch(ds_mas[i_rez].type)
        {
            case T_FLOAT:
            case T_INT:
             i1=f_innum(intrpr.wrd);
             break;

          case T_DBL:
          i1=sscanf(intrpr.wrd,"%lg",&dbl_tmp);
          break;

           case T_INT_L:
            i1=sscanf(intrpr.wrd,"%ld",&l_tmp);
            break;
        case T_HEX_L:
            i1=sscanf(intrpr.wrd,"%lx",&l_tmp);
            break;
           case T_HEX:
           case T_HX2:
           case T_HX4:
             i1=f_inhex(intrpr.wrd);
             break;
           case T_STR:
             i1=1;
             break;
        }
       if(i1)
        {
           switch(ds_mas[i_rez].type)
          {
           case T_FLOAT:
             *(float*)ds_mas[i_rez].op= (float)f_lim(intrpr.num_var,ds_mas[i_rez].low_l,ds_mas[i_rez].high_l);
             break;
          case T_DBL:
          *(double*)ds_mas[i_rez].op= dbl_tmp;
          break;
           case T_INT:
           case T_HEX:
           case T_HX2:
           case T_HX4:
             *(int*)ds_mas[i_rez].op= (int)f_lim(intrpr.num_var,ds_mas[i_rez].low_l,ds_mas[i_rez].high_l);
             break;
        case T_INT_L:
        case T_HEX_L:
             if(l_tmp < ds_mas[i_rez].low_l) l_tmp =ds_mas[i_rez].low_l;
             else if(l_tmp > ds_mas[i_rez].high_l) l_tmp=ds_mas[i_rez].high_l;
             *(long int*)ds_mas[i_rez].op=l_tmp;
          break;
           case T_STR:
             strcpy((char*)ds_mas[i_rez].op, intrpr.wrd);
             break;
          }
        }
       else goto dis_e;
     }

 // if(f_inw()) goto dis_e;
    return i_rez;

dis_e:
//    (*ff_putch[ComPortHost])('?');
    putch('?');
    return -1;
}
/*------------------------------*/
float par1=12,par2=13,par3=14,par4=15.345;
struct dis_set  ds_par[4]=
{
//-------------------
  &par1,
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &par2,
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &par3,
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &par4,
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
};
//----------------------
struct dis_set  ds_zp[]=
{
//--------------
  &ZeroPage,
  0,
  BIG_P,
  T_INT,
//--------------
  &EmptPage,
  0,
  BIG_P,
  T_INT,
//--------------
};
//----------------------
struct dis_set  ds_pass=
{
//--------------
  &Password,
  0,
  BIG_P,
  T_INT_L,
//--------------
};
//----------------------
//----------------------
unsigned long int p_mem=0xffff0;
unsigned char *mem_ptr=NULL;
struct dis_set  ds_dm=
{
//-------------------
  &p_mem,
 -BIG_P,
  BIG_P,
  T_HEX_L,
//-------------------
};
//----------------------

//----------------------

struct dis_set  ds_scan[4]=
{
//-------------------
  &ComPortEcho,
  1,
  3,
  T_INT,
//-------------------
  &ToutLicpScan,
  0.,
  BIG_I,
  T_INT,
//-------------------
  &licp_amax,
  0.,
  255,
  T_INT,
/*--------------*/
  &LICP_Rqst.CRC_flag,
  0,
  1,
  T_INT,
//-------------------
};
//----------------------
struct dis_set  ds_mko[]=
{
//-------------------
  &num_out1,
  0,
  16,
  T_INT,
//-------------------
  &num_out2,
  0,
  16,
  T_INT,
//-------------------
};
//----------------------
//----------------------
struct dis_set  ds_mks[]=
{
//----------------
  &num_in1,
  0,
  KOL_INP,
  T_INT,
//----------------
  &num_in2,
  0,
  KOL_INP,
  T_INT,
//----------------
};
//----------------------
struct dis_set  ds_mki[]=
{
/*----------------*/
  &mski_inp1,
  0,
  255,
  T_INT,
/*----------------*/
  &mski_inp2,
  0,
  255,
  T_INT,
/*----------------*/
};
//----------------------
//----------------------
struct dis_set  ds_hm[]=
{
//----------------
  &H_Cyl_M[0],
  0,
  BIG_P,
  T_FLOAT,
//----------------
  &H_Cyl_M[1],
  0,
  BIG_P,
  T_FLOAT,
//----------------
  &H_Cyl_M[2],
  0,
  BIG_P,
  T_FLOAT,
//----------------
  &H_Cyl_M[3],
  0,
  BIG_P,
  T_FLOAT,
//----------------
  &H_Cyl_M[4],
  0,
  BIG_P,
  T_FLOAT,
//----------------
  &H_Cyl_M[5],
  0,
  BIG_P,
  T_FLOAT,
//----------------
  &H_Cyl_M[6],
  0,
  BIG_P,
  T_FLOAT,
//----------------
  &H_Cyl_M[7],
  0,
  BIG_P,
  T_FLOAT,
//----------------
  &H_Cyl_M[8],
  0,
  BIG_P,
  T_FLOAT,
//----------------

};
//----------------------


//----------------------
struct dis_set  ds_at[]=
{
//-------------------
  &alpha_Tank[0],
  0,
  0.01,
  T_FLOAT,
//-------------------
  &alpha_Tank[1],
  0,
  0.01,
  T_FLOAT,
//-------------------
  &alpha_Tank[2],
  0,
  0.01,
  T_FLOAT,
//-------------------
  &T0_Tank[0],
  0.,
  100,
  T_FLOAT,
//-------------------
  &T0_Tank[1],
  0.,
  100,
  T_FLOAT,
//-------------------
  &T0_Tank[2],
  0.,
  100,
  T_FLOAT,
//-------------------
};
//----------------------
struct dis_set  ds_al[]=
{
//-------------------
  &alpha_LVL[0],
  0,
  0.01,
  T_FLOAT,
//-------------------
  &alpha_LVL[1],
  0,
  0.01,
  T_FLOAT,
//-------------------
  &alpha_LVL[2],
  0,
  0.01,
  T_FLOAT,
//-------------------
  &T0_LVL[0],
  0.,
  100,
  T_FLOAT,
//-------------------
  &T0_LVL[1],
  0.,
  100,
  T_FLOAT,
//-------------------
  &T0_LVL[2],
  0.,
  100,
  T_FLOAT,
//-------------------
};
//----------------------
//----------------------
struct dis_set  ds_off[]=
{
//-------------------
  &Lvl_offs[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &Lvl_offs[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &Lvl_offs[2],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &Lvl_offs[3],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &Lvl_offs[4],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &Lvl_offs[5],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &Lvl_offs[6],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &Lvl_offs[7],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &Lvl_offs[8],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
};

//----------------------
//----------------------
struct dis_set  ds_str[]=
{
//-------------------
  &Bot_time,
  0,
  BIG_P,
  T_INT_L,
//-----------
//-------------------
  &Sl3_time1,
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &Sl3_time2 ,
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &Sl3_time3 ,
  0,
  BIG_P,
  T_INT_L,
//-------------------
};
//----------------------
//----------------------
#if defined( DIGIMAG )

struct dis_set  ds_dgm[]=
{
//----------------
  &s_DGM[0].Prod_lvl,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_DGM[1].Prod_lvl,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_DGM[2].Prod_lvl,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_DGM[0].Temp ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_DGM[1].Temp ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_DGM[2].Temp ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
/*
  &s_DGM[0].Wat_lvl ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
*/
//----------------
/*
  &s_DGM[1].Wat_lvl ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
*/
//----------------
  &State_dgm[0],
  0.,
  BIG_P,
  T_INT,
//----------------
  &State_dgm[1],
  0.,
  BIG_P,
  T_INT,
//----------------
  &State_dgm[2],
  0.,
  BIG_P,
  T_INT,
//----------------
//----------------
//----------------
};
//----------------------
struct dis_set  ds_dgma[]=
{
//-------------------
  &DGM_ADDR[0],
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &DGM_ADDR[1] ,
  -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &DGM_ADDR[2] ,
  -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
};
struct dis_set  ds_dgmo[]=
{
//-------------------
  &s_DGM[0].offset,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_DGM[1].offset,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_DGM[2].offset,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_DGM[0].scale,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_DGM[1].scale,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_DGM[2].scale,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
};
//----------------------


#endif
//----------------------

//-----------------------
#if defined(PMP201)
int nn_rsv=1;
float Prod_lvl,Wat_lvl,DensLiq,TempLiq;
struct dis_set  ds_gn[]=
{

//----------------
  &nn_rsv,
  1.,
  max_PMP,
  T_INT,
//----------------
  &Prod_lvl,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &Wat_lvl,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &DensLiq,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &TempLiq,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------

};


struct dis_set  ds_PMP[]=
{
//----------------
  &s_PMP[0].Prod_lvl,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_PMP[1].Prod_lvl,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_PMP[2].Prod_lvl,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_PMP[0].Temp ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_PMP[1].Temp ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_PMP[2].Temp ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_PMP[0].DensReal ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_PMP[1].DensReal,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
  &s_PMP[2].DensReal,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//----------------
/*
  &s_PMP[0].Wat_lvl ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
*/
//----------------
/*
  &s_PMP[1].Wat_lvl ,
  -BIG_P,
  BIG_P,
  T_FLOAT,
*/
//----------------
  &State_PMP[0],
  0.,
  BIG_P,
  T_INT,
//----------------
  &State_PMP[1],
  0.,
  BIG_P,
  T_INT,
//----------------
  &State_PMP[2],
  0.,
  BIG_P,
  T_INT,
//----------------
//----------------
//----------------
};
//----------------------
struct dis_set  ds_PMPa[]=
{
//-------------------
  &PMP_ADDR[0],
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &PMP_ADDR[1] ,
  -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &PMP_ADDR[2] ,
  -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
};
//----------------------
struct dis_set  ds_PMPo[]=
{
//-------------------
  &s_PMP[0].offset,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_PMP[1].offset,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_PMP[2].offset,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_PMP[0].scale,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_PMP[1].scale,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &s_PMP[2].scale,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
};
//----------------------




#endif
//-----------------------
long int sk_reg_n=1;
long int sk_reg_q=1;
//----------------------
#if defined(RTU_ComSK)
struct dis_set  ds_sk[6]=
{
//-------------------
/*
  &ComSK_fn[0],
  0.,
  32,
  T_INT,
*/
//-------------------
//-------------------
  &ComSK_Ctrl[0],
  0.,
  BIG_P,
  T_HX4,
//-------------------
  &DriveSV[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
  &DriveSA[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
  &DriveSD[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
  &DriveState[0],
  0.,
  BIG_P,
  T_HX4,
//-------------------
  &DriveError[0],
  0.,
  BIG_P,
  T_HX4,
//-------------------
};
//----------------------
struct dis_set  ds_rsk[2]=
{
//-------------------
  &sk_reg_n,
  1.,
  65536,
  T_INT_L,
//-------------------
  &sk_reg_q,
  0.,
  65535,
  T_INT_L,
//-------------------
};
//----------------------
struct dis_set  ds_skwd[4]=
{
//-------------------
  &sk_pool_period[0],
  10.,
  BIG_P,
  T_INT_L,
//-------------------
  &ComSK_WD_Rqst[0].timeout,
  10.,
  BIG_P,
  T_INT_L,
//-------------------
  &sk_pool_period[1],
  10.,
  BIG_P,
  T_INT_L,
//-------------------
  &ComSK_WD_Rqst[1].timeout,
  10.,
  BIG_P,
  T_INT_L,
//-------------------
};
#endif  // if defined(RTU_ComSK)
//----------------------
#if defined(RTU_Delta)
struct dis_set  ds_dlt[6]=
{
//-------------------
  &Delta_Ctrl[0],
  0.,
  BIG_P,
  T_HX4,
//-------------------
  &DriveSV[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
  &DriveSA[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
  &DriveSD[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
  &DriveState[0],
  0.,
  BIG_P,
  T_HX4,
//-------------------
  &DriveError[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------
struct dis_set  ds_dltn[3]=
{
//-------------------
  &Drive_SV_beg[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
  &Drive_SA_beg[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
  &Drive_SD_beg[0],
  0.,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------
unsigned int dlt_reg_n=1;
long int dlt_reg_q=1;
struct dis_set  ds_rdlt[2]=
{
//-------------------
  &dlt_reg_n,
  0.,
  BIG_P,
  T_HX4,
//-------------------
  &dlt_reg_q,
  0.,
  BIG_P,
  T_INT_L,
//-------------------
};
//----------------------
#endif  // if defined(RTU_Delta)
//----------------------
//----------------------
long int mvd_reg_n=1;
long int mvd_reg_q=1;
struct dis_set  ds_rmvd[2]=
{
//-------------------
  &mvd_reg_n,
  1.,
  65536,
  T_INT_L,
//-------------------
  &mvd_reg_q,
  0.,
  65535,
  T_INT_L,
//-------------------
};
//----------------------
long int wmvd_reg_v=1;
struct dis_set  ds_wmvd[2]=
{
//-------------------
  &mvd_reg_n,
  1.,
  65536,
  T_INT_L,
//-------------------
  &wmvd_reg_v,
  0.,
  BIG_P,
  T_INT_L,
//-------------------
};
//----------------------
char str_mvd[40];
struct dis_set  ds_wmvds[3]=
{
//-------------------
  &mvd_reg_n,
  1.,
  65536,
  T_INT_L,
//-------------------
  &mvd_reg_q,
  0.,
  65535,
  T_INT_L,
//-------------------
  &str_mvd[0],
  0,
  65536,
  T_STR,
//-------------------
};
//----------------------
float fl_id=0,fl_id1=1;
char chr_id[20]="Default";
struct dis_set  ds_bid[3]=
{
//-------------------
  &fl_id,
 -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &fl_id1,
 -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  &chr_id[0],
  0,
  65536,
  T_STR,
//-------------------
};
//----------------------

struct dis_set  ds_id=
{
//-------------------
  &n_id,
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
};
//----------------------
//----------------------
float wmf_v=0;
 union  { float f; unsigned long l; } mvd;
struct dis_set  ds_wmvf[2]=
{
//-------------------
  &mvd_reg_n,
  1.,
  65536,
  T_INT_L,
//-------------------
  &mvd.f,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
};
//----------------------
struct dis_set  ds_de[]=
{
//-------------------
  &ICP_error[0],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[1],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[2],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[3],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[4],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[5],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[6],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[7],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[8],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[9],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[10],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[11],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[12],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[13],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[14],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
  &ICP_error[15],
  -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
};
//----------------------
//----------------------
struct dis_set  ds_ioch[2]=
{
//--------------
  &I7060[0].channels,
  0.,
  16,
  T_INT,
//--------------
  &I7060[1].channels,
  0.,
  16,
  T_INT,
//--------------
};
//----------------------
//----------------------
struct dis_set  ds_io[]=
{
//-------------------
  &OUT_VAR,
  0.,
  BIG_P,
  T_HX4,
//-------------------
  &INP_VAR,
  0.,
  BIG_P,
  T_HX4,
//-------------------
  &INP_7188,
  0.,
  BIG_P,
  T_HX4,
//-------------------
};
//----------------------
//----------------------
//----------------------
//----------------------
float dsc_tmp=0;
float offs_cor=0;
float fct_offs=0,fct_delay=0;
//----------------------
unsigned int fle;
struct dis_set  ds_fle=
{
//-------------------
  &fle,
  0xC000,
  0xE000,
  T_HX4,
//-------------------
};
//----------------------
int n_beg,n_end;
struct dis_set ds_pda[2]=
{
//-------------------
  &n_beg,
  1,
  BIG_P,
  T_INT,
//-------------------
  &n_end,
  1,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------
struct dis_set ds_flch[]=
{
//-------------------
  &Fl_ch_mm[0],
  0,
  1,
  T_INT,
//-----------
  &Fl_ch_mm[1],
  0,
  1,
  T_INT,
//-----------
  &Fl_ch_mm[2],
  0,
  1,
  T_INT,
//-----------
  &Fl_ch_mm[3],
  0,
  1,
  T_INT,
//-----------
  &Fl_ch_mm[4],
  0,
  1,
  T_INT,
//-----------
  &Fl_ch_mm[5],
  0,
  1,
  T_INT,
//-----------
  &Fl_ch_mm[6],
  0,
  1,
  T_INT,
//-----------
  &Fl_ch_mm[7],
  0,
  1,
  T_INT,
//-----------
  &Fl_ch_mm[8],
  0,
  1,
  T_INT,
//-----------
};
//----------------------
int n_lst=1;
struct dis_set ds_pdl=
{
//-------------------
  &n_lst,
  1,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------
int n_dnn;
struct dis_set ds_pdda=
{
//-------------------
  &n_dnn,
  1,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------
int d_lst=1;
struct dis_set ds_pddl=
{
//-------------------
  &d_lst,
  1,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------
int tt_sec=0,tt_min=56,tt_hour=1,tt_day=30,tt_month=7,tt_year=9;
struct dis_set ds_pddp[]=
{
//-------------------
  &tt_sec,
  0,
  59,
  T_INT,
//-------------------
  &tt_min,
  0,
  59,
  T_INT,
//-------------------
  &tt_hour,
  0,
  23,
  T_INT,
//-------------------
  &tt_day,
  1,
  31,
  T_INT,
//-------------------
  &tt_month,
  1,
  12,
  T_INT,
//-------------------
  &tt_year,
  0,
  99,
  T_INT,
//-------------------
  &s_mode,
  0,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------
int pdn_tmp;
struct dis_set ds_pdn=
{
//-------------------
  &pdn_tmp,
  0,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------

//----------------------
struct dis_set  ds_br[4]=
{
//-------------------
  &ComBaud[1],
  1200,
  115200,
  T_INT_L,
//-------------------
  &ComBaud[2],
  1200,
  115200,
  T_INT_L,
//-------------------
  &ComBaud[3],
  1200,
  115200,
  T_INT_L,
//-------------------
  &ComBaud[4],
  1200,
  115200,
  T_INT_L,
//-------------------
};
//----------------------
struct dis_set  ds_cp[4]=
{
//-------------------
  &ComProt[1],
  0,
  3,
  T_INT,
//-------------------
  &ComProt[2],
  0,
  3,
  T_INT,
//-------------------
  &ComProt[3],
  0,
  3,
  T_INT,
//-------------------
  &ComProt[4],
  0,
  3,
  T_INT,
//-------------------
};
//----------------------
struct dis_set ds_trtu[4]=
{
//-------------------
  &byte_timout_rtu[1],
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &byte_timout_rtu[2],
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &byte_timout_rtu[3],
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &byte_timout_rtu[4],
  0,
  BIG_P,
  T_INT_L,
//-------------------
};
//----------------------
struct dis_set ds_sct[]=
{
//-------------------
  &scom_tim[1],
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &scom_tim[2],
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &scom_tim[3],
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &scom_tim[4],
  0,
  BIG_P,
  T_INT_L,
//-------------------
};
//----------------------
int cm_lgth,cm_parity,cm_stop;
long int cm_baud=9600;
struct dis_set  ds_cm[]=
{
//-------------------
  &cm_baud,
  1200,
  115200L,
  T_INT_L,
//-------------------
  &cm_lgth,
  7,
  8,
  T_INT,
//-------------------
  &cm_parity,
  0,
  4,
  T_INT,
//-------------------
  &cm_stop,
  1,
  2,
  T_INT,
//-------------------
};
//----------------------
int status,port,addr,CRC_flag,pool_time,timeout;

struct dis_set  ds_dev[]=
{
//-------------------
  &status,
  0,
  2,
  T_INT,
//-------------------
  &port,
  1,
  4,
  T_INT,
//-------------------
  &addr,
  0,
  BIG_P,
  T_INT,
//-------------------
  &CRC_flag,
  0.,
  1,
  T_INT,
//-------------------
  &pool_time,
  0.,
  BIG_I,
  T_INT,
//-------------------
  &timeout,
  0.,
  BIG_I,
  T_INT,
//-------------------
//-------------------
};
long int baud=9600;
int TT_par,FF_par,addr0;
//----------------------
struct dis_set  ds_mod[]=
{
//-------------------
  &addr,
  0,
  255,
  T_INT,
//-------------------
  &baud,
  1200.,
  115200,
  T_INT_L,
//-------------------
  &TT_par,
  0.,
  BIG_I,
  T_HX2,
//-------------------
  &FF_par,
  0.,
  BIG_I,
  T_HX2,
//-------------------
};
//----------------------
//----------------------
struct dis_set  ds_dnm[]=
{
//----------------------
  &Tank_nn,
  1,
  9,
  T_INT,
//----------------------
  &flag_en_er,
  0,
  BIG_P,
  T_INT,
//----------------------
  &fl_inv_out,
  0,
  1,
  T_INT,
};
//----------------------
//----------------------
struct dis_set  ds_dtc[]=
{
//-------------------
  &Density,
  0,
  BIG_P,
  T_FLOAT,
//-------------------
  &Temperature,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
};
//----------------------
struct dis_set  ds_lim[]=
{
//-------------------
  &prc10[0],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc90[0],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc10[1],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc90[1],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc10[2],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc90[2],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc10[3],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc90[3],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc10[4],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc90[4],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc10[5],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc90[5],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc10[6],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc90[6],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc10[7],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc90[7],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc10[8],
  0,
  1.01,
  T_FLOAT,
//-------------------
  &prc90[8],
  0,
  1.01,
  T_FLOAT,
//-------------------
  
};
//----------------------
//----------------------
//----------------------
struct dis_set  ds_wins=
{
//-------------------
  &FlagWinSum,
  0,
  1,
  T_INT,
//-----------
};
//----------------------
//----------------------
#if defined(ICP_7017C)
//----------------------
struct dis_set  ds_adc[8]=
{
//-------------------
  &ADC[0],
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &ADC[1],
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &ADC[2],
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &ADC[3],
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &ADC[4],
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &ADC[5],
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &ADC[6],
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
  &ADC[7],
  0.,
  BIG_P,
  T_FLOAT,
//-------------------
};
//----------------------
struct dis_set  ds_adci[]=
{
//-------------------
  &dat_i[0],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &dat_i[1],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &dat_i[2],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &dat_i[3],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &dat_i[4],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &dat_i[5],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &dat_i[6],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &dat_i[7],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
/*
  &dat_i[8],
 -BIG_P,
  BIG_P,
  T_INT_L,

  &dat_i[9],
 -BIG_P,
  BIG_P,
  T_INT_L,

  &dat_i[10],
 -BIG_P,
  BIG_P,
  T_INT_L,

  &dat_i[11],
 -BIG_P,
  BIG_P,
  T_INT_L,
  */
//-------------------
};
//----------------------
struct dis_set  ds_aio=
{
//-------------------
  &INP_7017,
 -BIG_P,
  BIG_P,
  T_HX4,
//-------------------
};
//----------------------
struct dis_set  ds_adcl[]=
{
//-------------------
  &Lvl_7017[0],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &Lvl_7017[1],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &Lvl_7017[2],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &Lvl_7017[3],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &Lvl_7017[4],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &Lvl_7017[5],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &Lvl_7017[6],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
  &Lvl_7017[7],
 -BIG_P,
  BIG_P,
  T_INT_L,
//-------------------
/*
  &Lvl_7017[8],
 -BIG_P,
  BIG_P,
  T_INT_L,

  &Lvl_7017[9],
 -BIG_P,
  BIG_P,
  T_INT_L,

  &Lvl_7017[10],
 -BIG_P,
  BIG_P,
  T_INT_L,

  &Lvl_7017[11],
 -BIG_P,
  BIG_P,
  T_INT_L,
*/
//-------------------
};
//----------------------
//----------------------
/*
struct dis_set  ds_adc1[8]=
{
  &ADC[8],
  0.,
  BIG_P,
  T_FLOAT,

  &ADC[9],
  0.,
  BIG_P,
  T_FLOAT,

  &ADC[10],
  0.,
  BIG_P,
  T_FLOAT,

  &ADC[11],
  0.,
  BIG_P,
  T_FLOAT,

  &ADC[12],
  0.,
  BIG_P,
  T_FLOAT,

  &ADC[13],
  0.,
  BIG_P,
  T_FLOAT,

  &ADC[14],
  0.,
  BIG_P,
  T_FLOAT,

  &ADC[15],
  0.,
  BIG_P,
  T_FLOAT,
};
*/
//----------------------
#endif
//----------------------
struct dis_set  ds_com=
{
//-------------------
  &EchoComN,
  0,
  4,
  T_INT,
//-------------------
};
//----------------------
struct dis_set  ds_echo[2]=
{
//-------------------
  &flag_Deb,
  0,
  1,
  T_INT,
//-------------------
  &flag_echo_host,
  0,
  1,
  T_INT,
//-------------------
};
//----------------------
int fl_wr_eee=0;
struct dis_set  ds_fls[]=
{
//-------------------
  &flag_Slv,
  0,
  1,
  T_INT,
//-------------------
  &flag_Hst,
  0,
  1,
  T_INT,
//-------------------
  &fl_wr_eee,
  0,
  1,
  T_INT,
//-------------------

};

//----------------------
struct dis_set  ds_slv[]=
{
//----------------
  &ComPortSlv ,
  1,
  4,
  T_INT,
//----------------
  &AddrSlv,
  0,
  255,
  T_INT,
//----------------
  &WD_PMP_Per,
  0,
  BIG_P,
  T_INT_L,
//-------------------
  &byte_timout_slv,
  0,
  BIG_P,
  T_INT_L,
/*----------------*/
  &out_delay_slv,
  0,
  BIG_P,
  T_INT_L,
/*----------------*/
};

//-----------------------------------
struct dis_set  ds_fls2[2]=
{
//-------------------
  &flag_Slv2,
  0,
  1,
  T_INT,
//-------------------
  &fl_wr_eee,
  0,
  1,
  T_INT,
//-------------------
};

//----------------------
struct dis_set  ds_slv2[]=
{
//----------------
  &ComPortSlv2 ,
  1,
  4,
  T_INT,
//----------------
  &AddrSlv2,
  0,
  255,
  T_INT,
//----------------
  &WD_PMP2_Per,
  0,
  BIG_P,
  T_INT_L,
//-------------------
};

struct dis_set  ds_fls3[2]=
{
//-------------------
  &flag_Slv3,
  0,
  1,
  T_INT,
//-------------------
  &fl_wr_eee,
  0,
  1,
  T_INT,
//-------------------
};

//----------------------
struct dis_set  ds_slv3[]=
{
//----------------
  &ComPortSlv3 ,
  1,
  4,
  T_INT,
//----------------
  &AddrSlv3,
  0,
  255,
  T_INT,
//----------------
  &WD_PMP2_Per,
  0,
  BIG_P,
  T_INT_L,
//-------------------
};

//-----------------------------------

//-----------------------------------
  /* для trace()  */
//-----------------------------------

long int i_nn_d=5.;
int fl_trace=0;
long int i_nn=0.;
int dim_xyz=dim_xyz_c; /* длина буфера записи */
int fl_trace_loop=0;

//float *buf_xyz=(float *)0x30000000;
long int buf_xyz[dim_xyz_c];
int uk_buf=0;
long int *uk_ns[5]={NULL,NULL,NULL,NULL,NULL};
int f_ns[5]={10,1,4,9,11};
int cnt_after=0;
float ns_fictf=99999999;
long int ns_fict=0;
//-------------------
void *list_ns[]={
&ns_fictf,          // 0
(void *)0x40006c,   // 10 TimeStamp
&INP_VAR,           // 21
&OUT_VAR,           // 22


&ns_fictf,           // 0
};
int list_ns_type[]={
T_FLOAT,      // 0   ns_fict

T_INT_L,      // 10   TimeStamp

T_HX2,        // 21  INP_VAR
T_HX2,        // 22  OUT_VAR

T_FLOAT,      // 0   ns_fict

};

/* ---------------------------------------- */
void PrintOp(int num_op)
{
char fst_str[20];

if(num_op > (sizeof(list_ns) / sizeof(&summ_v)-1) )
    sprintf(fst_str,"====" );
else
  switch (list_ns_type[num_op])
  {
          case T_FLOAT:
    sprintf(fst_str,"%g", *(float*)list_ns[num_op] );
             break;
          case T_INT:
    sprintf(fst_str,"%d", *(int*)list_ns[num_op] );
             break;
          case T_INT_L:
    sprintf(fst_str,"%ld", *(long int*)list_ns[num_op] );
             break;
          case T_HEX:
    sprintf(fst_str,"%X", *(int*)list_ns[num_op] );
             break;
          case T_HX2:
    sprintf(fst_str,"%02X", *(int*)list_ns[num_op] );
             break;
          case T_HX4:
    sprintf(fst_str,"%04X", *(int*)list_ns[num_op] );
             break;
          case T_STR:
    sprintf(fst_str,"%s", (char*)list_ns[num_op] );
             break;
          case T_DBL:
    sprintf(fst_str,"%lg", *(double*)list_ns[num_op] );
             break;
  }
  printf(" %s",fst_str);
}
/* ---------------------------------------- */
int i_num_var=0;
struct dis_set  ds_ivar=  /* 1 значениe */
{
/*----------------*/
  &i_num_var,
  0,
  10,
  T_INT,
/*----------------*/
};
int f_num_var=1000;
struct dis_set  ds_fvar=  /* 1 значениe */
{
/*----------------*/
  &f_num_var,
  1000,
  10,
  T_INT,
/*----------------*/
};
//----------------------
//----------------------
int v_num_var=9;
//-------------------
struct dis_set  ds_var[2]=  /* 1 значениe */
{
//-------------------
  &v_num_var,
  1,
 (float)(sizeof(list_ns) / sizeof(&summ_v)-1),
  T_INT,
//-------------------
  &ns_fict,           // 0
  -BIG_P,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------
struct dis_set  ds_ag[]=
{
//--------------
  &s_MVD[0].Press,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//--------------
  &Press1,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//--------------
  &Press2,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//--------------
  &Temp1,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//--------------
  &Temp2,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//--------------
  &Press3,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//--------------
  &Temp3,
  -BIG_P,
  BIG_P,
  T_FLOAT,
//--------------
};
//----------------------
//----------------------
struct dis_set  ds_agp[]=
{
//-------------------
  &RQ2,
  0,
  1,
  T_INT,
//-------------------
  &PMP_sw,           // 0
  -BIG_P,
  BIG_P,
  T_INT,
//-------------------
};
//----------------------
//----------------------
//-------------------
struct dis_set  ds_ns[8]=  /* 1 значениe */
{
//-------------------
  &f_ns[0],
  0.,
 (float)(sizeof(list_ns) / sizeof(&summ_v)-1),
  T_INT,
//-------------------
  &f_ns[1],
  0.,
 (float)(sizeof(list_ns) / sizeof(&summ_v)-1),
  T_INT,
//-------------------
  &f_ns[2],
  0.,
 (float)(sizeof(list_ns) / sizeof(&summ_v)-1),
  T_INT,
//-------------------
  &f_ns[3],
  0.,
 (float)(sizeof(list_ns) / sizeof(&summ_v)-1),
  T_INT,
//-------------------
  &f_ns[4],
  0.,
 (float)(sizeof(list_ns) / sizeof(&summ_v)-1),
  T_INT,
//-------------------
  &i_nn_d,
  0.,
  BIG_P,
  T_INT_L,
//-------------------
  &dim_xyz,
  0.,
  dim_xyz_c,
  T_INT,
//------------------
  &fl_trace_loop,
  0.,
  1.0,
  T_INT,
//------------------
};
//-------------------
//-------------------
void f_ns_count()
{
int i;
  for(i=0;i<5;i++)
     if(f_ns[i] > 0)
      {
        if( f_ns[i] <= (int)(sizeof(list_ns) / sizeof(&summ_v)-1) )
          uk_ns[i]=(long int *)list_ns[(int)f_ns[i]];
        else
        {
         f_ns[i] = 0;
         uk_ns[i]=(long int *)&ns_fictf;
        }
      }
     else
      {
       f_ns[i] = 0;
       uk_ns[i]=(long int *)&ns_fictf;
      }
}
/*----------------------------------------------*/
void trace()      // store trace for variables  specified in f_ns[]
{
/*----------------------------------*/
 if(fl_trace_loop != 0)
  {
  if (FL_err+ fl_trace)
   {  if(cnt_after >= Val_after)  return;
      else  cnt_after++;
   }
  else cnt_after=0;


  if(uk_buf > (dim_xyz-6.)) uk_buf=0;
  if( (i_nn+=1.) >= i_nn_d)
    {
//----------------------------------------------
//----------------------------------------------
      i_nn=0.;
      if(f_ns[0] == 0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[0];
      if(f_ns[1] == 0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[1];
      if(f_ns[2]==0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[2];
      if(f_ns[3]==0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[3];
      if(f_ns[4]==0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[4];
//---------------------------------------------
    }

  }
 else
  if(uk_buf < (dim_xyz-6.))
   if( (i_nn+=1.) >= i_nn_d)
    {
      i_nn=0.;
      if(f_ns[0] == 0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[0];
      if(f_ns[1] == 0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[1];
      if(f_ns[2]==0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[2];
      if(f_ns[3]==0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[3];
      if(f_ns[4]==0) goto m_end;
      buf_xyz[uk_buf++]=*uk_ns[4];
    }
m_end:
    return;
}
/* ============================================== */
/* ---------------------------------------- */
// сканирование магистрали

int licp_sw=0;
int licp_addr;
int licp_fnd;
int flag_print_licp=0;
int licp_port=1;
//----------------------------

int ToutLicpScan = 30;
int licp_amax=10;  //QQQQ
int licp_crc_flag=1;
int licp_t_conf=0;

struct icp_mod ICP_MOD[5][icp_mod_max];
int  ICP_mod_fnd[5]={0,0,0,0,0};

//----------------------------
void f_look_ICP()
{
int i;
char buf_tmp[20];
switch(licp_sw)
 {  /* сканирование ICP slave контроллеров на магистрали */
   case 0: break;
   case 1:
          licp_addr=0;
          licp_fnd=0;
          licp_sw=2;
          LICP_Rqst.answ_com=f_result_Licp;
          LICP_Rqst.answ_flt=f_empty_ans_Licp;
          LICP_Rqst.timeout=ToutLicpScan;

   case 2:
      if(licp_addr>=licp_amax)
      { // сканирование закончено
         ICP_mod_fnd[licp_port]=licp_fnd;
         if(licp_fnd)
         {
            LICP_Rqst.answ_com=f_ans_Licp_cfg;
            LICP_Rqst.answ_flt=f_ans_Licp_Flt;
            licp_t_conf=0;

            LICP_Rqst.timeout=ToutLicpScan*5;
            licp_sw=3;
            if(flag_print_licp) puts("\n\r");
            break;
         }
         else
         {
           if(flag_print_licp)
           {
             printf("\n\r COM%d : устройства ICP не найдены\n\r",licp_port);
           }
           if(ff_serv==f_look_ICP) ff_serv=NULL;
           licp_sw=0;
           break;
         }
      }
      if( f_queue_chk(licp_port) && (LICP_Rqst.status == Req_OK) )
      {
       // послать команду "read name" по текущему адресу
           sprintf(buf_tmp,"$%02XM",licp_addr);
           f_prepare7000(buf_tmp,LICP_Rqst.CRC_flag);
           strcpy(LICP_Rqst.Cmd,buf_tmp);
           LICP_Rqst.cmd_lgth=strlen(LICP_Rqst.Cmd);
           LICP_Rqst.time_stamp=TimeStamp;
           f_queue_put(licp_port, &LICP_Rqst);
      }
      break;

   case 3: // опрос конфигурации найденных устройств
      if(licp_t_conf >= licp_fnd)
      {
           licp_sw=4;
           break;
      }
      _fmemset(&ICP_MOD[licp_port][licp_t_conf].TT,0xaa,sizeof(int)*3);
      if( f_queue_chk(licp_port) && (LICP_Rqst.status == Req_OK) )
      {
       // послать команду опроса конфигурации найденным устройствам
           sprintf(buf_tmp,"$%02X2",ICP_MOD[licp_port][licp_t_conf].addr0);
           f_prepare7000(buf_tmp,LICP_Rqst.CRC_flag);
           strcpy(LICP_Rqst.Cmd,buf_tmp);
           LICP_Rqst.cmd_lgth=strlen(LICP_Rqst.Cmd);
           LICP_Rqst.time_stamp=TimeStamp;
           f_queue_put(licp_port, &LICP_Rqst);
      }
      break;

   case 4:

       if(flag_print_licp && licp_fnd)
       {
          printf("\n\r COM%d,найдены устройства ICP:",licp_port);
          f_mod_list();
       }
       if(ff_serv==f_look_ICP) ff_serv=NULL;
       licp_sw=0;
       break;
 }
 return;
}
//-------------------------------------------------
// структура запроса при сканировании магистрали
//-------------------------------------------------
struct COM_rqst LICP_Rqst={
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_result_Licp,         //  answ_com
f_empty_ans_Licp,      //  answ_flt
"SCAN bus",            //  name
0,                     //  n_dev
&Host,                 // *ICP_dd
0,                     //  текущая функция
50,                    //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
/*----------------------------*/
void f_result_Licp(int ii)
{
/* заносит принятые данные в массив структур
    ICP_MOD[]
*/
 int i;
 long int count;
 if(licp_crc_flag==1)
    count=n_bcom[ii]-5;  // длина имени
 else
    count=n_bcom[ii]-3;  // длина имени

//в ответ на $00M принимается  !00MMIFCT41
 if(licp_crc_flag==0) goto m1;
 if(f_check_crc_ICP(cb_COM[ii]))
 {
m1:
  _fmemcpy( ICP_MOD[ii][licp_fnd].name,&cb_COM[ii][3],count);
  ICP_MOD[ii][licp_fnd].name[count]=0;

  cb_COM[ii][3]=0;
  sscanf(&cb_COM[ii][1],"%2x",&i);
  ICP_MOD[ii][licp_fnd].addr=i;
  ICP_MOD[ii][licp_fnd].addr0=licp_addr;
  ICP_MOD[ii][licp_fnd].port=ii;
  licp_fnd++;
 }
 if(licp_fnd == icp_mod_max)
  {
    licp_addr=licp_amax;
  }
  else
    licp_addr++;
 f_queue_free(ii,&LICP_Rqst);
 if(flag_print_licp) putch('x');
}
/*-------------------------------------------*/
void f_empty_ans_Licp(int ii)
{ // таймаут по ответу
 licp_addr++;
 f_queue_free(ii,&LICP_Rqst);
 if(flag_print_licp) putch('.');
}
/*-------------------------------------------*/
/*-------------------------------------------*/
void f_ans_Licp_cfg(int ii)
{
/* заносит принятые данные о конфигурации
   в ICP_MOD[]
*/
 char *result;
 int *dat;
 int i;

// в ответ на $002 приходит
//  !00 01 06 40  AC
 result=cb_COM[ii];
 if(licp_crc_flag==0) goto m1;
 if(f_check_crc_ICP(result))
 {
m1:
  sscanf(&result[1],"%2X",&i);
  if(((i != ICP_MOD[ii][licp_t_conf].addr)&& (ICP_MOD[ii][licp_t_conf].addr !=0) )||(*result != '!')) goto m_flt;

  dat=&ICP_MOD[ii][licp_t_conf].TT;
  sscanf(&result[3],"%2x%2x%2x",&dat[0],&dat[1],&dat[2]);
  ICP_MOD[ii][licp_t_conf].port=ii;
 }
m_flt:
 licp_t_conf++;
 f_queue_free(ii,&LICP_Rqst);
// if(flag_print_licp) putch('+');
}
/*-------------------------------------------*/
void f_ans_Licp_Flt(int ii)
{ // таймаут по ответу
 licp_t_conf++;
 f_queue_free(ii,&LICP_Rqst);
// if(flag_print_licp) putch('-');
}
//-------------------------------
void f_look_RTU()
{
int i,i2;
char buf_tmp[20];
switch(licp_sw)
 {  /* сканирование ICP slave контроллеров на магистрали */
   case 0: break;
   case 1:
          licp_addr=0;
          licp_fnd=0;
          licp_sw=2;
          LICP_Rqst_RTU.answ_com=f_result_Licp_RTU;
          LICP_Rqst_RTU.answ_flt=f_result_Licp_RTU;
          LICP_Rqst_RTU.timeout=ToutLicpScan;

   case 2:
      if(licp_addr>=licp_amax)
      { // сканирование закончено
         ICP_mod_fnd[licp_port]=licp_fnd;
         if(licp_fnd)
         {
            licp_t_conf=0;
            licp_sw=4;
            if(flag_print_licp) puts("\n\r");
            break;
         }
         else
         {
           if(flag_print_licp)
           {
             printf("\n\r COM%d : устройства RTU не найдены\n\r",licp_port);
           }
           if(ff_serv==f_look_RTU) ff_serv=NULL;
           licp_sw=0;
           break;
         }
      }
      if( f_queue_chk(licp_port) && (LICP_Rqst_RTU.status == Req_OK) )
      {
       // послать команду "читать регистр" по текущему адресу
            i2=6;
            buf_tmp[0]=licp_addr;
            buf_tmp[1]=3;
            buf_tmp[2]=0x00; //(ComSK_SV[ii]>>8)&0xff;
            buf_tmp[3]=0x01; //ComSK_SV[ii] & 0xff;
            buf_tmp[4]= 0x00;
            buf_tmp[5]= 0x01;

           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(LICP_Rqst_RTU.Cmd,buf_tmp,(long)i2);
           LICP_Rqst_RTU.cmd_lgth=i2;
           LICP_Rqst_RTU.time_stamp=TimeStamp;
           f_queue_put(licp_port, &LICP_Rqst_RTU) ;
      }
      break;


   case 4:

       if(flag_print_licp && licp_fnd)
       {
          printf("\n\r COM%d,найдены устройства RTU:",licp_port);
          f_mod_list();
       }
       if(ff_serv==f_look_RTU) ff_serv=NULL;
       licp_sw=0;
       break;
 }
 return;
}
//-------------------------------------------------
//-------------------------------
struct COM_rqst LICP_Rqst_RTU={
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_result_Licp_RTU,     //  answ_com
f_result_Licp_RTU,     //  answ_flt
"SCAN RTU",            //  name
0,                     //  n_dev
&Host,                 // *ICP_dd
0,                     //  текущая функция
50,                    //  timeout
0,                     //  time_stamp
6,                     //  com_lgth
1,                     //  CRC_flag
{01,03,00,01,00,01},                     //  com[]
};
/*----------------------------*/
void f_result_Licp_RTU(int ii)
{
/* заносит принятые данные в массив структур
    ICP_MOD[]
*/
 int i;
 long int count;
 count=n_bcom[ii];  // длина имени
 if(count==0)
 {
  if(flag_print_licp) putch('.');
  goto m_end;
 }
//в ответ на $00M принимается  !00MMIFCT41
 if(f_check_crc_RTU(cb_COM[ii],count))
 {
m1:
  ICP_MOD[ii][licp_fnd].addr=cb_COM[ii][0] & 0xff;
  sprintf(ICP_MOD[ii][licp_fnd].name,"RTU A%02X",cb_COM[ii][0] & 0xff);
  ICP_MOD[ii][licp_fnd].TT=0;
  ICP_MOD[ii][licp_fnd].FF=0;
  ICP_MOD[ii][licp_fnd].CC=f_get_nb(ComBaud[ii]);
  ICP_MOD[ii][licp_fnd].port=ii;
  licp_fnd++;
 if(flag_print_licp) putch('x');
 }
 else
  if(flag_print_licp) putch('?');

m_end:
 if(licp_fnd == icp_mod_max)
  {
    licp_addr=licp_amax;
  }
  else
    licp_addr++;
 f_queue_free(ii,&LICP_Rqst_RTU);
}
/*-------------------------------------------*/
//-------------------------------
void  f_print_que()
{
int i;
    struct COM_rqst *request;
    struct s_icp_dev *ICPl_DEV;
    struct icp_mod *ICPl_MOD;

      printf("\n\r Ports queue:");
      for(i=1;i<4;i++)
      {
        printf("\n\rCOM%d: ",i);
        printf(" busy_ptr=%d,empty_ptr=%d,last_out=%d",COM_que[i].busy_ptr,COM_que[i].empt_ptr,last_out[i]);
        request=COM_que[i].request[COM_que[i].empt_ptr];
        if(request != NULL)
          printf("\n\r   Last srv: %s,status %d",request->name,request->status);
        request=COM_que[i].request[COM_que[i].busy_ptr];
           if(request != NULL)
          printf("\n\r   Last put: %s,status %d",request->name,request->status);
      }
      printf("\n\r ICP device(s):");

 printf(
"\n\rName     Status Port Addr CRC Pool   Send  Success Tout_err CRC_err T_snd T_ans");

      for(i=0;i<icp_lst_max;i++)
      {
        ICPl_DEV=ICP_dd[i];
 printf(
  "\n\r%10s    %1d   %1d   %3d  %d  %d",ICPl_DEV->name,ICPl_DEV->status,ICPl_DEV->port,ICPl_DEV->addr,ICPl_DEV->CRC_flag,ICPl_DEV->pool_time);

 printf("%8ld %8ld %6ld %6ld %6ld %6ld",ICPl_DEV->n_transaction,ICPl_DEV->n_success,ICPl_DEV->n_timeout_error,ICPl_DEV->n_CRC_error,
   ICPl_DEV->time_stamp_snd,ICPl_DEV->time_stamp_rcv);
      }
      printf("\n\r time %.3f sec",(float)(TimeStamp-TimeClear)/1000.);
/*
      GetTime(&hour,&min,&sec);
      printf("  %2d:%2d:%2d" ,hour,min,sec);
*/
}
//-------------------------------
void f_drv_list()
{ // показать список драйверов устройств
int i;
  struct s_icp_dev *ICPl_DEV;
  struct COM_rqst *request;

  printf("\n\rНомер   Имя       status  port   addr CRC_flag pool,ms timeout,ms");
    for(i=0;i<icp_lst_max;i++)
    {
      ICPl_DEV=ICP_dd[i];
      request=ICPl_DEV->request;
      printf("\n\r%2d  %10s      %d      %d   %3d     %d",i+1,ICPl_DEV->name,ICPl_DEV->status,ICPl_DEV->port,ICPl_DEV->addr,ICPl_DEV->CRC_flag);
      printf("      %4ld    %4ld", ICPl_DEV->pool_time,request->timeout);
    }
//    printf("\n\r");
}
//-------------------------------
struct icp_mod *f_get_mod_str(int num)
{ // функция возвращает указатель на структуру модудя
  // с номером num . num = 0...
  //
  int i,i1,nn;
  struct icp_mod *ICPl_MOD;

  nn=num+1;
  i1=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
  if((i1==0)|| (nn>i1) )return NULL;

  for(i=1;i<4;i++)
  {
   if(nn<= ICP_mod_fnd[i])
    {
      i1=nn-1;
      break;
    }
   else nn-=ICP_mod_fnd[i];
  }
  ICPl_MOD=&ICP_MOD[i][i1];
  return ICPl_MOD;
}
//-------------------------------
void f_mod_list()
{ // показать список обнаруженных модулей
int i,i1,i2;
 // int ICP_mod_fnd[5];
  struct icp_mod *ICPl_MOD;

  i1=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
  if(i1==0) printf("  список пуст,используйте SCAN (SCANP)\n\r");
  else
  {
  printf("\n\rНомер    Имя      порт адрес скорость  TT  FF");
  i2=1;
    for(i=0;i<i1;i++)
    {
      ICPl_MOD=f_get_mod_str(i);
      if(ICPl_MOD==0) break;
     if(ICPl_MOD->addr0 != ICPl_MOD->addr)
      printf("\n\r%3d %10s      %d \(%02X\)%02X  %6ld",i2,ICPl_MOD->name,ICPl_MOD->port,ICPl_MOD->addr0,ICPl_MOD->addr,baudrate_val[ICPl_MOD->CC]);
     else
      printf("\n\r%3d %10s      %d    %02X   %6ld",i2,ICPl_MOD->name,ICPl_MOD->port,ICPl_MOD->addr,baudrate_val[ICPl_MOD->CC]);

      printf("   %02X  %02X",ICPl_MOD->TT,ICPl_MOD->FF);
      i2++;
    }
      printf("\n\r");
  }
}
//-------------------------------
//-------------------------------
void  f_icp_hlp()
{
  puts("\n\r ICP modules commands:\n\r");
  puts(" $AAM - read module name\n\r");
  puts(" $AA2 - read module configuration\n\r");
  puts(" ~AAO(Name) - change module name\n\r");
  puts(" %AANNTTCCFF - change module configuration\n\r");
  puts(" $AA5VV - enable analog channels VV (for I7017)\n\r");
  puts(" $AA6 - read channel enable status (for I7017)\n\r");
}
int nn;
unsigned long int ee_nn;
unsigned long int ee_nn1;
//----------------------
void f_bridge_24()
{
 int tmp;

     if(IsCom(2)!=0)
     {
        tmp=ReadCom(2);
        ToCom(4,tmp);
     }
     if(IsCom(4)!=0)
     {
        tmp=ReadCom(4);
        ToCom(2,tmp);
     }
}
void f_bridge_34()
{
 int tmp;

     if(IsCom(3)!=0)
     {
        tmp=ReadCom(3);
        ToCom(4,tmp);
     }
     if(IsCom(4)!=0)
     {
        tmp=ReadCom(4);
        ToCom(3,tmp);
     }
}

/*----------------------------------------------*/
long int id_i3;
long int  bas_1=0;
#define  bas_n  19
long int f_dnum (long int number)
{
long int i;
id_i3 += number+bas_1;
if(id_i3 >= bas_n) id_i3 -=bas_n;
return (id_i3 & 0x0f);
}
/*----------------------------------------------*/
long int n_id=0,f_res_id=0;
int id_ok=0;

/*----------------------------------------------*/
int  f_check_id(long int n_id)
/*
    Return 1 - not correct ID.
           0 - correct ID
*/
{
unsigned long int i,i1,i2;
unsigned long int result,i3,i4;

           i  =*( unsigned long int *)(ID_ADDR);
           i1 =*( unsigned long int *)(ID_ADDR+4);

           id_i3=0;
           bas_1= (i+i1+7) & 0x0f;

           i2= ( (i >> 28) + (i1 >> 28)  ) & 0xf;
           f_dnum(i2);

           i2= ( (i >> 24) + (i1 >> 24)  ) & 0xf;
           f_dnum(i2);

           i2= ( (i >> 20) + (i1 >> 20)  ) & 0xf;
           i4= f_dnum(i2);
           result = ( i4 << 20) ;

           i2= ( (i >> 16) + (i1 >> 16)  ) & 0xf;
           i4= f_dnum(i2);
           result |= (i4 << 16);

           i2= ( (i >> 12) + (i1 >> 12)  ) & 0xf;
           i4= f_dnum(i2);
           result |= (i4 << 12) ;

           i2= ( (i >>  8) + (i1 >>  8)  ) & 0xf;
           i4= f_dnum(i2);
           result |= (i4 <<  8) ;

           i2= ( (i >>  4) + (i1 >>  4)  ) & 0xf;
           i4= f_dnum(i2);
           result |= (i4 <<  4) ;

           i2= ( i + i1 ) & 0xf;
           i4= f_dnum(i2);
           result |= i4 ;

           result &= 0x7fffff;
           f_res_id=result;

           if(f_res_id != n_id)  i3= 1;
           else i3=0;
 return i3;
}
/*----------------------------------------------*/
//----------------------
union
{
char ch[4];
long int lng;
} *lng_chr;

struct arch_str ar_str0_;
unsigned long int ee_tmp,ee_tmp1;

int f_intr()
{
  unsigned long int count=0xffff;
  unsigned int ii1;
  int i,i1,i2,i3,j,fn;
  long int i_l,i_l1,i_l2;
  char *ch_ptr;
  struct COM_rqst *request;
  struct s_icp_dev *ICPl_DEV;
  struct icp_mod *ICPl_MOD;
  char buf_tmp[50];
  char ch_tmp;
  int  hour,min,sec;
  struct arch_str *a_str;
  float ftmp;
  long int *id_ptr;
  struct s_TCT *tct_uk;
  unsigned int seg;

union  { float f; unsigned long l; } o,o1;

        intrpr.i3=0;
        if(f_inw()==0)
        {
          intrpr.n_bcom=0;
          goto fin;
        }

//---------------------------------
        if ((intrpr.wrd[0]=='Q') || (intrpr.wrd[0]=='q'))
        {
            printf ("\n\r QQQ \n\r");
            return 1;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"RES" ))
        {   //'RES'


  StopUserTimerFun();
  RestoreCom(1);
  RestoreCom(2);
  RestoreCom(3);
  while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
  RestoreCom(4);
  f_free_SlvBuf();

          _asm cli
          _asm hlt
//        (*(void (*)())adr_strt)();
          goto fin;
        }
//---------------------------------
//---------------------------------
      if (!strcmp(intrpr.wrd,"ID" ))
        {   //'ID' - Board ID
m1:
           if(f_dis_set(&ds_id,2,1) == 0)
            {
             puts(" ");
             printf("%08lX %08lX",*( unsigned long int *)(ID_ADDR),*( unsigned long int *)(ID_ADDR+4) );
            }
           if( f_check_id(n_id) == 0)
           {
            id_ok=1;
            printf(" == ID OK");
           }
           else
           {
            id_ok=0;
            printf(" == Not correct ID !");
           }
        goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"BID315" ))
        {   //'BID315' - Board ID

           f_check_id(n_id);
           n_id=f_res_id;
           f_check_id(n_id);

           if( f_check_id(n_id) == 0)
           {
            id_ok=1;
           }
           else
           {
            id_ok=0;
           }

         goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"PDN" ))
        {   //'PDN'
          pdn_tmp= f_get_max_n_arch();
          f_dis_set(&ds_pdn,3,1);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"FLCH" ))
        {   //'PDN'
          f_dis_set(ds_flch,4,9);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"JRN" ))
        {   //'JRN'
         pdn_tmp= f_get_max_n_arch();
         i2=sizeof(ar_str0_);

         i_l = (long int)pdn_tmp * (long int)i2;
//       printf("\r\n i2 = %d, Lgth=%ld\r\n ",i2,i_l);

         i_l+=sizeof(fl_id)+sizeof(fl_id)+sizeof(chr_id)+sizeof(sw_ver);
         putch(1);

         putch4(i_l);

         for(i1=0;i1<20;i1++)
          putch(sw_ver[i1]);

         o1.f=fl_id;
         putch4( o1.l);
         o1.f=fl_id1;
         putch4( o1.l);

         for(i1=0;i1<20;i1++)
          putch(chr_id[i1]);

         for(i1=0;i1<pdn_tmp;i1++)
         {

           if((i1%10)== 0)  f_cycle();

           if( (a_str= f_get_astr(i1)) == NULL )
           {
            a_str=f_get_astr(0);
           }
           ch_ptr=(char *)a_str;
           for(j=0;j<i2;j++)
           {
            ch_tmp= *ch_ptr++;
            putch(ch_tmp);
           }

           WaitTransmitOver(ComPortHost);

         }

          goto fin;
        }
//---------------------------------
//---------------------------------
      if (!strcmp(intrpr.wrd,"PDA" ))
        {   //'PDA'
         i1= f_get_max_n_arch();
         n_end=i1;
         if( f_dis_set(ds_pda,3,2))
          {
           if(n_end > i1)
           {
            n_end=i1;
           }
           if(n_beg>n_end) n_beg=n_end;

           for(i=n_beg-1;i<n_end;i++)
             f_print_a_str(i);
          }
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"PDL" ))
        {   //'PDL'
         i1= f_get_max_n_arch();
         n_lst=i1;
         if( f_dis_set(&ds_pdl,3,1))
          {
     m_pdl:
           if(n_lst > i1)
           {
            n_lst=i1;
           }
           for(i=i1-n_lst;i<i1;i++)
             f_print_a_str(i);
          }
         else
         {
           n_lst=20;
           goto m_pdl;
         }
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"FLS" ))
        {   //'FLS' флаг включения протокола SlaveRTU
         if( f_dis_set(ds_fls,3,3))
         {

          WD_PMP_time=TimeStamp;
          if(fl_wr_eee)
            if(f_wrt_eee()<0) goto fin_e;

          if(flag_Slv !=0 )
            {
              cm_baud  = ComBaudSlv    ;
              cm_lgth  = Com_lgthSlv   ;
              cm_parity= Com_paritySlv ;
              cm_stop  = Com_stopSlv   ;
              RestoreCom(ComPortSlv );
              if( InstallCom(ComPortSlv ,cm_baud,cm_lgth,cm_parity,cm_stop) < 0 ) goto fin_e;
              if(ComPortSlv==4)
                  ShowCom(ComPortSlv,Com_lgthSlv,ComBaudSlv);
              if(ComPortSlv == ComPortHost)
              {
                flag_Hst=0;
                goto fin;
              }
            }

            if(flag_Slv == 0 )
              if(ComPortSlv == ComPortHost)
              {
                flag_Hst=1;
              }

            if(flag_Hst !=0 )
            {
               RestoreCom(ComPortHost );
               InstallCom(ComPortHost,115200L,8,0,1);
               ShowCom(ComPortSlv,8,115200L);
            }
            else
            {
             RestoreCom( ComPortHost );
             i=ComPortHost;
             InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]);
            }

         }
         goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"SLV" ))
        {   //'SLV'
          f_dis_set(ds_slv,3,5);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"FLS2" ))
        {   //'FLS2' флаг включения протокола Slave2RTU
         if( f_dis_set(ds_fls2,4,2))
         {

          WD_PMP2_time=TimeStamp;
          if(fl_wr_eee)
            if(f_wrt_eee()<0) goto fin_e;

          if(flag_Slv2 !=0 )
            {
             cm_baud  = ComBaudSlv2   ;
             cm_lgth  = Com_lgthSlv2  ;
             cm_parity= Com_paritySlv2;
             cm_stop  = Com_stopSlv2  ;
             RestoreCom(ComPortSlv2 );
             if( InstallCom(ComPortSlv2 ,cm_baud,cm_lgth,cm_parity,cm_stop) < 0 ) goto fin_e;

             if(ComPortSlv2==4)
                ShowCom(ComPortSlv2,Com_lgthSlv2,ComBaudSlv2);

            }
         }
         goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"SLV2" ))
        {   //'SLV2'
         f_dis_set(ds_slv2,4,3);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"FLS3" ))
        {   //'FLS3' флаг включения протокола Slave3RTU
         if( f_dis_set(ds_fls3,4,2))
         {

          WD_PMP2_time=TimeStamp;
          if(fl_wr_eee)
            if(f_wrt_eee()<0) goto fin_e;

          if(flag_Slv3 !=0 )
            {
             cm_baud  = ComBaudSlv3   ;
             cm_lgth  = Com_lgthSlv3  ;
             cm_parity= Com_paritySlv3;
             cm_stop  = Com_stopSlv3  ;
             RestoreCom(ComPortSlv3 );
             if( InstallCom(ComPortSlv3 ,cm_baud,cm_lgth,cm_parity,cm_stop) < 0 ) goto fin_e;

             if(ComPortSlv3==4)
                ShowCom(ComPortSlv3,Com_lgthSlv3,ComBaudSlv3);

            }
         }
         goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"SLV3" ))
        {   //'SLV3'
         f_dis_set(ds_slv3,4,3);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"NS" ))
        {   //'NS'
          if(f_dis_set(ds_ns,2,8) >0)
               f_ns_count();
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"AG" ))
        {   //'AG' AnaloG values
          f_dis_set(ds_ag,2,7);
          goto fin;
        }
//------------------------------------------
      if (!strcmp(intrpr.wrd,"AGP" ))
        {   //'AGP'
          f_dis_set(ds_agp,3,2);
          goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"WE" ))
       {   //'WE'
        if(f_wrt_eee()<0)
        {
          f_wr_evt(evt_we_err);
          goto fin_e;
        }
        else
        {
          f_wr_evt(evt_we);
          if(flag_echo_host )
            printf(" %d",ee_members);
        }
  //      f_wr_cor();
        goto fin;
       }
//---------------------------------
       if (!strcmp(intrpr.wrd,"RE" ))
       {   //'RE'
     // if(f_chk_EEE_CRC()<0) goto fin_e;
        f_chk_EEE_CRC();
        if(f_rd_eee()<0)
        {
   flag_Slv=0;
   flag_Hst=1;
           f_wr_evt(evt_re_err);
           goto fin_e;
        }
        if(dim_xyz>=dim_xyz_c) dim_xyz=dim_xyz_c-6;
 //       f_rd_cor();
   flag_Slv=0;
   flag_Hst=1;
        f_wr_evt(evt_re);
        goto fin;
       }
//---------------------------------
       if (!strcmp(intrpr.wrd,"VT" ))
       {   //'VT' Variable Transmit
         f_chk_EEE_CRC();// count  ee_lgth_byte
         putch(0);
         i1 = ee_lgth_byte;
         i2 = ee_lgth_byte+2+3;
         putch(i2&0xff); putch((i2>>8)&0xff);
         putch('V');  putch('L');  putch(0xd);
         putch(i1&0xff); putch((i1>>8)&0xff);

        Eeprom_summ=0;
        for(i=0,i1=0;EE_Dat[i].ptr!=0;i++)
        {
          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;
          for(j=0;j<i2;j++)
          {
            ch_tmp= *ch_ptr++;
            putch(ch_tmp);
 //         putch(*ch_ptr++);
            if(EE_Dat[i].ptr!= &Eeprom_summ)
//            Eeprom_summ +=buf_eep[i1];
//            Eeprom_summ +=((unsigned long)(buf_eep[i1])) & 0xffL;
              Eeprom_summ +=((unsigned long)(ch_tmp)) & 0xffL;

            i1++;

          }
        }
        goto fin;
       }
//---------------------------------
       if (!strcmp(intrpr.wrd,"VL" ))
       {   //'VL'  Variable Load
         i_l=n_id;
         i1 = getch();
         i2 = getch();
         i1= (i1 & 0xff) | ((i2<<8)&0xff00);
         ch_ptr=buf_eep;
         ee_nn1=0;
          for(i=0;(i<(i1-4)) && (i<max_buf_eep);i++)
          {
            ch_tmp=getch();
            *ch_ptr++ =ch_tmp;
            ee_nn1 +=((unsigned long)(ch_tmp)) & 0xff;
          }
          ch_ptr=(char *)&ee_nn;

        for(;(i<i1) && (i<max_buf_eep);i++)
        {
            ch_tmp=getch();
            *ch_ptr++ =ch_tmp;
        }
          if(ee_nn1 != ee_nn) goto fin_e;
          f_rd_eee();

        if(dim_xyz>=dim_xyz_c) dim_xyz=dim_xyz_c-6;
        n_id=i_l;
        goto fin;
       }
//---------------------------------
//------------------------------------------
        if( id_ok==0) goto m1;
//---------------------------------
        if  ((intrpr.wrd[0]=='C') &&(intrpr.wrd[1]=='M') )
        {   //"CM" 'CM' - Com Mode
         if( ((i=f_123(3)) >= 0 ) &&  (i< 5) )
         {
          i++;
          cm_baud  = ComBaud[i]   ;
          cm_lgth  = Com_lgth[i]  ;
          cm_parity= Com_parity[i];
          cm_stop  = Com_stop[i]  ;

          if((i1=f_dis_set(ds_cm,3,4)) >0 )
          {
//           printf("\n\r i= %d, %ld %d %d %d",i,cm_baud,cm_lgth,cm_parity,cm_stop   );
            if(f_get_nb(cm_baud) < 0  ) goto fin_e;

            ComBaud[i]    = cm_baud   ;
            Com_lgth[i]   = cm_lgth   ;
            Com_parity[i] = cm_parity ;
            Com_stop[i]   = cm_stop   ;

            RestoreCom(i);
            if( InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]) < 0 ) goto fin_e;
          }

          ShowCom(ComPortHost,Com_lgth[ComPortHost],ComBaud[ComPortHost]);
          goto fin;
         }
        }
//---------------------------------
        if  ((intrpr.wrd[0]=='D') &&(intrpr.wrd[1]=='R')&&(intrpr.wrd[2]=='V'))
        {   //'DRV'
         if( ((i=f_123(4)) >= 0 ) &&  (i< icp_lst_max ) )
         {
           ICPl_DEV = ICP_dd[i];
           request=ICPl_DEV->request;

           status    = ICPl_DEV->status    ;
           port      = ICPl_DEV->port      ;

#if defined( DIGIMAG )
        if(i<3)
           addr      = DGM_ADDR[i] ;
         else
#endif
           addr      = ICPl_DEV->addr      ;
           CRC_flag  = ICPl_DEV->CRC_flag  ;
           pool_time = ICPl_DEV->pool_time ;
           timeout    = request->timeout;
          if(f_dis_set(ds_dev,4,6) >= 0)
          {
           ICPl_DEV->status    = status    ;
           ICPl_DEV->port      = port      ;
           ICPl_DEV->addr      = addr      ;
           ICPl_DEV->CRC_flag  = CRC_flag  ;
           ICPl_DEV->pool_time = pool_time ;
           request->timeout= timeout;
          }
          goto fin;
         }
        }
//---------------------------------
        if  ((intrpr.wrd[0]=='M') &&(intrpr.wrd[1]=='O') && (intrpr.wrd[2]=='D'))
        {   //'MOD'
         i1=ICP_mod_fnd[1]+ICP_mod_fnd[2]+ICP_mod_fnd[3];
         if( ((i=f_123(4)) >= 0 ) &&  (i< i1 ) )
         {
           ICPl_MOD=f_get_mod_str(i);

           addr0  = ICPl_MOD->addr0;
           addr  = ICPl_MOD->addr;
           baud   = baudrate_val[ICPl_MOD->CC];
           TT_par = ICPl_MOD->TT ;
           FF_par = ICPl_MOD->FF ;

          if((i2=f_dis_set(ds_mod,4,4)) > 0)
          {
            i=f_get_nb(baud);
            if(i<0) goto fin_e;

            ICPl_MOD->addr =  addr  ;
            ICPl_MOD->TT   =  TT_par ;
            ICPl_MOD->CC   = i;
            ICPl_MOD->FF   =  FF_par ;
            if( f_queue_chk(ICPl_MOD->port ) )
            {
             // послать команду "set cfg" по старому адресу устройства
               sprintf(buf_tmp,"%c%02X%02X%02X%02X%02X",'%',addr0,ICPl_MOD->addr,ICPl_MOD->TT,ICPl_MOD->CC,ICPl_MOD->FF);
               f_prepare7000(buf_tmp,LICP_Rqst.CRC_flag);
               strcpy(Host_Rqst.Cmd,buf_tmp);
               Host_Rqst.cmd_lgth=strlen(Host_Rqst.Cmd);
               Host_Rqst.time_stamp=TimeStamp;
               f_queue_put(ICPl_MOD->port, &Host_Rqst);
               return 0;
            }
          }
          goto fin;
         }
        }
//---------------------------------
//---------------------------------
        if  ((intrpr.wrd[0]=='C') &&(intrpr.wrd[1]=='O')&&(intrpr.wrd[2]=='M'))
        {   //'COM'
          if(f_dis_set(&ds_com,3,1) > 0)
          {
           if((EchoComN==4)||(EchoComN==0) )
           {
            EchoComN=0;
            goto fin;
           }
             ComPortEcho=EchoComN;
          }
          goto fin;
        }
//---------------------------------
     if  ((intrpr.wrd[0]=='S') &&(intrpr.wrd[1]=='C')&&(intrpr.wrd[2]=='A')&&(intrpr.wrd[3]=='N') &&(intrpr.wrd[4]=='P'))
   //    if (!strcmp(intrpr.wrd,"SCANP" ))
        {   //'SCANP'
          f_dis_set(ds_scan,5,4);
          goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"BR" ))
        {   //'BR' Baud Rate for COM1...COM4

          if((i1=f_dis_set(ds_br,2,4)) !=0 )
          {
            for(i=1;i<=i1;i++)
             if(f_get_nb(ComBaud[i]) < 0  ) goto fin_e;
            for(i=1;i<=i1;i++)
                SetBaudrate(i,ComBaud[i]);
          }
          goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"BRD24" ))
        {   //'BRidge 24'

          while(1)
          {
           f_bridge_24();
          }
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"BRD34" ))
        {   //'BRidge 34'

          while(1)
          {
           f_bridge_34();
          }
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"CP" ))
        {   //'CP' Com Protocol
          f_dis_set(ds_cp,2,3);
          goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"TRTU" ))
        {   //'TRTU' byte Timeout RTU
          f_dis_set(ds_trtu,4,3);
          goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"SCT" ))
        {   //'SCT'  Time delay for ServiceCom
          f_dis_set(ds_sct,3,4);
          goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"P" ))
        {   //'P ' - вывести измеренное время
          flag_prn ^= 1;
          printf(" %d",flag_prn);
          goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"OEE" ))
        {   //'OEE' Send 2K EEP
         if(EE_MultiRead_L(0,2048,buf_eep)<0 ) goto fin_e;
         putch(0);
         i1 = 2048;
         i2 = 2048+2+4;
         putch(i2&0xff);
         putch((i2>>8)&0xff);
         putch('S');
         putch('E');
         putch('E');
         putch(0xd);
         putch(i1&0xff);
         putch((i1>>8)&0xff);

         for(i=0;i<i1;i++) putch(buf_eep[i]);

 /*      putch(0);
         i1 = 2048;
         putch(i1&0xff); putch((i1>>8)&0xff);
         for(i=0;i<i1;i++)
         {
          ch_tmp= EE_RandomRead(i>>8,i & 0xff);
          putch(ch_tmp);
         }
 */
         goto fin;
        }
//---------------------------------
       if (!strcmp(intrpr.wrd,"SEE" ))
        {   //'SEE' store 2K EEP

         i1 = getch();
         i2 = getch();
         i1=(i1 & 0xff) | ((i2<<8)&0xff00);
         for(i=0;i<i1;i++)
           buf_eep[i]=getch();

  printf(" %d",i1);

 EE_WriteEnable();
   i2= EE_MultiWrite_L(0,2048,buf_eep);
 EE_WriteProtect();
        if(i2<0) goto fin_e;
         goto fin;
        }
   if (!strcmp(intrpr.wrd,"SCAN" ))
   {    //'SCAN'

    if(ComProt[ComPortEcho]==0)
    {
      licp_sw=1;
      licp_port=ComPortEcho;
      ff_serv=f_look_ICP;
      flag_print_licp=1;
      LICP_Rqst.timeout=ToutLicpScan;
      printf("\n\r Scanning ICP COM%d: tout %d ms, max addr %d,CRC_CHK=%d",ComPortEcho,ToutLicpScan,licp_amax,LICP_Rqst.CRC_flag);
      licp_crc_flag=LICP_Rqst.CRC_flag;
    }
    else
    {
      licp_sw=1;
      licp_port=ComPortEcho;
      ff_serv=f_look_RTU;
      flag_print_licp=1;
      LICP_Rqst_RTU.timeout=ToutLicpScan;
      printf("\n\r Scanning RTU COM%d: tout %d ms, max addr %d",ComPortEcho,ToutLicpScan,licp_amax);
    }
    goto fin;
   }
//---------------------------------
      if (!strcmp(intrpr.wrd,"SQ" ))
          {   // 'SQ '
           f_print_que();
           goto fin;
          }
//---------------------------------
      if (!strcmp(intrpr.wrd,"HELP" ))
          {   // 'HELP '
           f_icp_hlp() ;
           goto fin;
          }
//---------------------------------
   if (!strcmp(intrpr.wrd,"CQ" ))
          {
           printf("\n\r Queues are cleared");

    for(i=0;i<icp_lst_max;i++)
    {
      ICPl_DEV=ICP_dd[i];
      request=ICPl_DEV->request;
      request->status=0;
    }

           f_queue_clr(1);
           f_queue_clr(2);
           f_queue_clr(3);
           goto fin;
          }
//---------------------------------
   if (!strcmp(intrpr.wrd,"ECHO" ))
     //'ECHO'
          {
            f_dis_set(ds_echo,4,2);
           if(!flag_echo_host ) puts("\n\r");
            goto fin;
          }
//---------------------------------
   if (!strcmp(intrpr.wrd,"DRV" ))
          { // 'DRV' - drivers list
            f_drv_list();
            goto fin;
          }
//---------------------------------
   if (!strcmp(intrpr.wrd,"MOD" ))
          { // 'MOD' - modules list
            f_mod_list();
            goto fin;
          }
//---------------------------------
   if (!strcmp(intrpr.wrd,"WINS" ))
          { // 'WINS' - Winter-summer time
            f_dis_set(&ds_wins,4,1);
            goto fin;
          }
//---------------------------------
      if (!strcmp(intrpr.wrd,"BID" ))
        {   //'ID'
          f_dis_set(ds_bid,3,3);
          goto fin;
        }
//---------------------------------
          if (!strcmp(intrpr.wrd,"DTCT2" ))
         /* -----  display Tank Correction Table 2 --- */
           {
           i=1;
           goto m_DTCT;
           }
//---------------------------------
          if (!strcmp(intrpr.wrd,"DTCT3" ))
         /* -----  display Tank Correction Table 3 --- */
           {
           i=2;
           goto m_DTCT;
           }
//---------------------------------
          if (!strcmp(intrpr.wrd,"DTCT4" ))
         /* -----  display Tank Correction Table 3 --- */
           {
           i=3;
           goto m_DTCT;
           }
//---------------------------------
          if (!strcmp(intrpr.wrd,"DTCT5" ))
         /* -----  display Tank Correction Table 3 --- */
           {
           i=4;
           goto m_DTCT;
           }
//---------------------------------
          if (!strcmp(intrpr.wrd,"DTCT6" ))
         /* -----  display Tank Correction Table 3 --- */
           {
           i=5;
           goto m_DTCT;
           }
//---------------------------------
          if (!strcmp(intrpr.wrd,"DTCT7" ))
         /* -----  display Tank Correction Table 3 --- */
           {
           i=6;
           goto m_DTCT;
           }
//---------------------------------
          if (!strcmp(intrpr.wrd,"DTCT8" ))
         /* -----  display Tank Correction Table 3 --- */
           {
           i=7;
           goto m_DTCT;
           }
//---------------------------------
          if (!strcmp(intrpr.wrd,"DTCT9" ))
         /* -----  display Tank Correction Table 3 --- */
           {
           i=8;
           goto m_DTCT;
           }
//---------------------------------
                 if (!strcmp(intrpr.wrd,"ZP" ))
        {   //'ZP'

          f_dis_set(ds_zp,2,2);
          goto fin;
        }
//------------------------------------------
      if (!strcmp(intrpr.wrd,"PASS" ))
        {   //'PASS'Password
          f_dis_set(&ds_pass,4,1);
          goto fin;
        }
//------------------------------------------
          if (!strcmp(intrpr.wrd,"DTCT1" ))
         /* -----  display Tank Correction Table 1 --- */
           {
           i=0;
m_DTCT:
           par1=1;
           par2=n_TCT[i];
           if( f_dis_set(ds_par,5,2)> 0)
           {
             if(par1<1) par1=1;
             if(par2 > n_TCT[i]) par2=n_TCT[i];
           }
           printf("\n\r");

           i1=par1-1;
           i2=par2;

      //   i2=n_TCT[i];
           tct_uk=TCT_ARR[i];

           printf(" %d\n\r",n_TCT[i]);
           for(;i1<i2;i1++)
           {
            printf(" %10.03f %10.03f\n\r",tct_uk[i1].level,tct_uk[i1].volume);
           }

          goto fin;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TCT2" ))
         /* -----  receive Tank Correction Table 2 --- */
           {
           i=1;
           goto m_TCT;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TCT3" ))
         /* -----  receive Tank Correction Table 3 --- */
           {
           i=2;
           goto m_TCT;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TCT1" ))
         /* -----  receive Tank Correction Table 1 --- */
           {
           i=0;
  m_TCT:
             tct_uk=TCT_ARR[i];

             intrpr.i3=4;

             if(!f_inw()) { goto fin_e; }
             if(f_innum(intrpr.wrd) ) i2 = intrpr.num_var;   /* количество пар значений */
             else  { goto fin_e; }

             n_TCT[i]=i2;

             if ( i2 >= max_TCT)  { goto fin_e; }

             for(i1=0;i1<i2;i1++)
             {
                 f_get_wrd();
                 if(f_innum(intrpr.wrd) )
                  { // запись очередного значения level
                    tct_uk[i1].level=intrpr.num_var;
                  }
                 else   goto fin_e;

                 f_get_wrd();

                 if(f_innum(intrpr.wrd) )
                  { // запись очередного значения vlume
                    tct_uk[i1].volume=intrpr.num_var;
                  }
                 else   goto fin_e;

             }
//-----------------------------

            goto fin;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TC2" ))
         /* -----  receive Tank Correction Table 2 --- */
           {
           i3=1;
           goto m_TC;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TC3" ))
         /* -----  receive Tank Correction Table 2 --- */
           {
           i3=2;
           goto m_TC;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TC4" ))
         /* -----  receive Tank Correction Table 2 --- */
           {
           i3=3;
           goto m_TC;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TC5" ))
         /* -----  receive Tank Correction Table 2 --- */
           {
           i3=4;
           goto m_TC;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TC6" ))
         /* -----  receive Tank Correction Table 2 --- */
           {
           i3=5;
           goto m_TC;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TC7" ))
         /* -----  receive Tank Correction Table 2 --- */
           {
           i3=6;
           goto m_TC;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TC8" ))
         /* -----  receive Tank Correction Table 2 --- */
           {
           i3=7;
           goto m_TC;
           }
    /*----------------------------------------------*/
          if (!strcmp(intrpr.wrd,"TC9" ))
         /* -----  receive Tank Correction Table 2 --- */
           {
           i3=8;
           goto m_TC;
           }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
       if (!strcmp(intrpr.wrd,"TC1" ))
       {   //'TC1' TCT data load

         i3=0;
m_TC:
         i1 = getch();
         i2 = getch();
         i1= (i1 & 0xff) | ((i2<<8)&0xff00);
         ch_ptr= (char *) &TCT_ARR[i3][0];
         ee_tmp=0;

    count=max_TCT* sizeof(TCT_sz)-8;
          for(i=0;(i<(i1-4)) && (i < count);i++)
          {
            ch_tmp=getch();
            *ch_ptr++ =ch_tmp;
            ee_tmp +=((unsigned long)(ch_tmp)) & 0xff;
          }

        ch_ptr=(char *)&ee_tmp1;

//    printf("\n i=%d ,i1= %d,count =%ld",i,i1,count);

        for(; i<i1 ;i++)
        {
            ch_tmp=getch();
            *ch_ptr++ =ch_tmp;
        }

        if(ee_tmp != ee_tmp1)
        {

         printf(" TCT received not correct, %d bytes ,%d lines, CRC_rcv=%8lx , CRC_snd = %8lx",i1,(int)((i1-4)/ 8),ee_tmp,ee_tmp1 );
          goto fin_e;
        }
        else
        {
         n_TCT[i3]=(i1-4) / 8;
         printf(" TCT%d OK,%d lines, CRC_rcv=%8lx , CRC_snd = %8lx",i3,n_TCT[i3],ee_tmp,ee_tmp1 );

         f_prep_VM(i3);
        }
        goto fin;
       }
    /*----------------------------------------------*/
      if(!strcmp(intrpr.wrd,"HM" ))
       {   //'HM'
          f_dis_set(ds_hm,2,9);
          goto fin;
       }
     /*----------------------------------------------*/



           /*----------------------------------------------*/
      if (!strcmp(intrpr.wrd,"MKO" ))
        {   //'MKO'
          f_dis_set(ds_mko,3,2);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"MKS" ))
        {   //'MKI'
//       if( f_dis_set(ds_mks,3,KOL_D_INP) > 0)
         if( f_dis_set(ds_mks,3,2) > 0)
             f_init_mki();
         goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"MKI" ))
        {   //'MKI'
//        f_dis_set(ds_mki,3,KOL_D_INP);
          f_dis_set(ds_mki,3,2);
          f_init_mki();
          goto fin;
        }
//---------------------------------
    /*----------------------------------------------*/
//---------------------------------
#if defined(ICP_7017C)
//---------------------------------
/*
   if (!strcmp(intrpr.wrd,"ADC1" ))
          { //'ADC1'
            f_dis_set(ds_adc1,4,8);
            goto fin;
          }
*/
   if (!strcmp(intrpr.wrd,"ADCI" ))
          { //'ADCI'
            f_dis_set(ds_adci,4,8);
            goto fin;
          }
   if (!strcmp(intrpr.wrd,"ADCL" ))
          { //'ADCI'
            f_dis_set(ds_adcl,4,8);
            goto fin;
          }
   if (!strcmp(intrpr.wrd,"ADC"))
          { //'ADC'
            f_dis_set(ds_adc,3,8);
            goto fin;
          }
   if (!strcmp(intrpr.wrd,"AIO"))
          { //'AIO'
            f_dis_set(&ds_aio,3,1);
            goto fin;
          }
//---------------------------------
#endif
//---------------------------------
      if (!strcmp(intrpr.wrd,"DE" ))
        {   //'DE'

//         for(i=0;i<=icp_lst_max;i++)
//           printf(" %04x",ICP_error[i]);
           f_dis_set(ds_de,2,icp_lst_max+2);
         goto fin;
        }
//---------------------------------
//---------------------------------
//---------------------------------
      if (!strcmp(intrpr.wrd,"CE" ))
        {   //'CE'

         CMD_IN=5; //5
         CMD_OUT = CMD_REG | CMD_CONT ;
         f_cl_error();
         goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"AT" ))
        {   //'AT'

          f_dis_set(ds_at,2,6);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"AL" ))
        {   //'AL'

          f_dis_set(ds_al,2,6);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"STR" ))
        {   //'STR'

          f_dis_set(ds_str,3,3);

   Sl3_time1_l=Sl3_time1 * 1000;
   Sl3_time2_l=Sl3_time2 * 1000;
   Sl3_time3_l=Sl3_time3 * 1000;


          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"OFF" ))
        {   //'OFF'

          f_dis_set(ds_off,3,9);
          goto fin;
        }
//---------------------------------
#if defined( DIGIMAG )
      if (!strcmp(intrpr.wrd,"DGM" ))
        {   //'DGM'

          f_dis_set(ds_dgm,3,9);
          goto fin;
        }

      if (!strcmp(intrpr.wrd,"DGMA" ))
        {   //'DGMA'

          f_dis_set(ds_dgma,4,3);
          goto fin;
        }
      if (!strcmp(intrpr.wrd,"DGMO" ))
        {   //'DGMO'

          f_dis_set(ds_dgmo,4,6);
          goto fin;
        }
#endif
//---------------------------------
#if defined(PMP201)

      if (!strcmp(intrpr.wrd,"PMP" ))
        {   //'PMP'

          f_dis_set(ds_PMP,3,12);
          goto fin;
        }

      if (!strcmp(intrpr.wrd,"PMPA" ))
        {   //'PMPA'

          f_dis_set(ds_PMPa,4,3);
          goto fin;
        }
      if (!strcmp(intrpr.wrd,"PMPO" ))
        {   //'PMPO'

          f_dis_set(ds_PMPo,4,6);
          goto fin;
        }



#endif
//---------------------------------
      if (!strcmp(intrpr.wrd,"WTV" ))
        {   //'WTV'
          f_wr_tct();
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"RTV" ))
        {   //'RTV'
          f_rd_tct();
          f_prep_VM(0);
          f_prep_VM(1);
          f_prep_VM(2);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"$ETV" ))
        {   //'$ETV'

          seg=seg_TCT;
          if( FlashErase(seg) != 0) goto fin_e;
          seg=seg_TCT2;
          if( FlashErase(seg) != 0) goto fin_e;

          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"GN" ))
        {   //'GN'

          f_dis_set(ds_gn,2,5);
              i1=nn_rsv-1;
              s_DGM[i1].Prod_lvl= Prod_lvl;
              s_DGM[i1].Wat_lvl = Wat_lvl ;
              s_DGM[i1].DensLiq = DensLiq ;
              s_DGM[i1].Temp    = TempLiq ;

          ttct1=TimeStamp;
          f_count_tank(i1);
          ttct2=TimeStamp;
          printf( "\n\r t cnt = %ld ms",ttct2-ttct1);

          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"GV" ))
        {   //'GV'

          par1= f_get_vol(0,s_DGM[0].Prod_lvl)- f_get_vol(0,s_DGM[0].Wat_lvl);
          par2= f_get_vol(1,s_DGM[1].Prod_lvl)- f_get_vol(1,s_DGM[1].Wat_lvl);
          par3= f_get_vol(2,s_DGM[2].Prod_lvl)- f_get_vol(2,s_DGM[2].Wat_lvl);
          par4=par1+par2+par3;

          f_dis_set(ds_par,2,4);
          ttct1=TimeStamp;
          f_count_tank(0);
          ttct2=TimeStamp;
          printf( "\n\r t cnt = %ld ms",ttct2-ttct1);

          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"GVP" ))
        {   //'GVP'
          par1= f_get_vol(0,s_DGM[0].Prod_lvl)- f_get_vol(0,s_DGM[0].Wat_lvl);
          par2= f_get_vol(1,s_DGM[1].Prod_lvl)- f_get_vol(1,s_DGM[1].Wat_lvl);
          par3= f_get_vol(2,s_DGM[2].Prod_lvl)- f_get_vol(2,s_DGM[2].Wat_lvl);

          f_dis_set(ds_par,3,3);
          goto fin;
        }
//---------------------------------
//---------------------------------
//---------------------------------
      if (!strcmp(intrpr.wrd,"IOCH" ))
        {   //'IOCH'
          f_dis_set(ds_ioch,4,2);
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"IO" ))
        {   //'IO'
          f_dis_set(ds_io,2,3);
          goto fin;
        }
//---------------------------------
//---------------------------------
      if (!strcmp(intrpr.wrd,"FLE" ))
        {   //'FLE'
         if( f_dis_set(&ds_fle,3,1))
          {
           seg=fle;
           if( FlashErase(seg) != 0) goto fin_e;
          }
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"FLD" ))
        {   //'FLD'
        seg=0x8000;
        offset=0;
        Flash_ptr=(unsigned long int *) _MK_FP_(seg,offset);
        printf("\n\r %04X:%04X %08lX (%08lX)",seg,offset,*Flash_ptr,(long int)(Flash_ptr));

        seg=0x9000;
        Flash_ptr=(unsigned long int *) _MK_FP_(seg,offset);
        printf("\n\r %04X:%04X %08lX",seg,offset,*Flash_ptr);

        seg=0xA000;
        Flash_ptr=(unsigned long int *) _MK_FP_(seg,offset);
        printf("\n\r %04X:%04X %08lX",seg,offset,*Flash_ptr);

        seg=0xB000;
        printf("\n\r %04X:%04X %08lX",seg,offset,*Flash_ptr);

        seg=0xC000;
        Flash_ptr=(unsigned long int *) _MK_FP_(seg,offset);
        printf("\n\r %04X:%04X %08lX",seg,offset,*Flash_ptr);

        seg=0xD000;
        Flash_ptr=(unsigned long int *) _MK_FP_(seg,offset);
        printf("\n\r %04X:%04X %08lX",seg,offset,*Flash_ptr);

        seg=0xE000;
        Flash_ptr=(unsigned long int *) _MK_FP_(seg,offset);
        printf("\n\r %04X:%04X %08lX",seg,offset,*Flash_ptr);

        seg=0xF000;
        Flash_ptr=(unsigned long int *) _MK_FP_(seg,offset);
        printf("\n\r %04X:%04X %08lX",seg,offset,*Flash_ptr);

          goto fin;
        }
      if (!strcmp(intrpr.wrd,"SN" ))
        {   //'SN' - serial number

         printf(" Addr = %08lX  ",(long int)(SystemSerialNumber) );
         printf("\n\r SN= ");
         for(i=0;i<16;i++)
           printf(" %02X",SystemSerialNumber[i] );
         id_ptr=(long int *)SystemSerialNumber;
           printf("\n\r SN = %08lX %08lX",id_ptr[0],id_ptr[1] );

         goto fin;
        }
//------------------------------------------
      if (!strcmp(intrpr.wrd,"CB" ))
        {   //'CB'
          uk_buf=0;
          goto fin;
        }
//------------------------------------------
//------------------------------------------
      if (!strcmp(intrpr.wrd,"I" ))
        {   //'I'
          ds_ivar.high_l=n_sl_int-1;
          if( f_dis_set(&ds_ivar,1,1)<0) goto fin;


          ds_var[1].op= list_Slv_int[i_num_var];
          ds_var[1].type = T_INT;

          intrpr.wdl=intrpr.i3;

          if( f_dis_set(&ds_var[1],intrpr.i3,1 ) > 0)
          {
            if( (i= f_int_fnc(i_num_var)) != 0)
            {
              printf(" ? : ");
              if(i==1)
                 printf("Command not correct");
              else if(i==2)
                 printf("Addr not correct");
              else if(i==3)
                 printf("Value not correct");
              else if(i==4)
                 printf("Error in slave");

            }

          }

      /*
         if(ff_Slv != NULL)
         {
          (*ff_Slv)(); // старт функции
          ff_Slv = NULL;
         }
      */
         goto fin;

        }
//------------------------------------------
      if (!strcmp(intrpr.wrd,"F" ))
        {   //'F'
          ds_fvar.high_l=1000 + 2*(n_sl_flt0-1) ;
          if( f_dis_set(&ds_fvar,1,1)<0) goto fin;

          i=(f_num_var-1000)/2;
          f_num_var=1000 + 2 * i;

          ds_var[1].op= list_Slv_flt[i];
          ds_var[1].type = T_FLOAT;

          intrpr.wdl=intrpr.i3;
          f_dis_set(&ds_var[1],intrpr.i3,1 );
          goto fin;

        }
//------------------------------------------
//------------------------------------------
      if (!strcmp(intrpr.wrd,"VAR" ))
        {   //'VAR'

          if( f_dis_set(ds_var,3,1)<0) goto fin;

          ds_var[1].op= list_ns[v_num_var];
          ds_var[1].type= list_ns_type[v_num_var];

          intrpr.wdl=intrpr.i3;
          f_dis_set(&ds_var[1],intrpr.i3,1 );
          goto fin;

        }
//------------------------------------------
      if (!strcmp(intrpr.wrd,"VARC" ))
        {   //'VARC'

          strcpy(intrpr.bfcom,"VARC");
          intrpr.i3=4;
          intrpr.wdl=intrpr.i3;
          f_dis_set(ds_var,intrpr.i3,1 );

          ds_var[1].op= list_ns[v_num_var];
          ds_var[1].type= list_ns_type[v_num_var];
          printf("  \r\n");
          i_l=TimeStamp;
          while(kbhit()==0)
          {
            f_cycle0();
          intrpr.i3=4;
          intrpr.wdl=intrpr.i3;

           if( f_timer( i_l, 100L ))
           {
            f_dis_set(&ds_var[1],intrpr.i3,1 );
            printf(" \r");
            i_l=TimeStamp;
           }
          }
          getch();
          goto fin;
        }
//------------------------------------------
      if (!strcmp(intrpr.wrd,"O" ))
         /* - 'O '-  команда Передать наверх буфер записи ---- */
          {
            if(fl_trace_loop == 0)
            {
              putch(0);
              j= uk_buf;
              i_l=(long int)j * 4L;
              putch2(i_l);

              for(i2=0;i2<5;i2++)
              {
                if(f_ns[i2] == 0.) break;
              }
            // i2 - количество переменных
              for(i=0,i3=0;i<j;i++,i3++)
                {
                if((i%50)== 0)  f_cycle();
                 if(i3>=i2) i3=0;
                 o.l= buf_xyz[i];
                 switch(list_ns_type[f_ns[i3]]  )
                 {
                  case T_FLOAT:
                    o1.f=o.f;
                    break;
                  case T_INT_L:
                  case T_HEX_L:
                    o1.f=(float)o.l;
                    break;
                  case T_INT:
                  case T_HEX:
                  case T_HX2:
                  case T_HX4:
                    o1.f=(float) *( (int *)(&o.l));
                    break;
                 }
                 putch4( o1.l);
                }
              goto fin;
            }
            else
            {
              fl_trace=1;
              putch(0);
              for(i2=0;i2<5;i2++)
              {
                if(f_ns[i2] == 0) break;
              }
              j= (int)((dim_xyz-6)/i2) * i2;
              i_l=(long int)j * 4L;
              putch2(i_l);
              for(i=uk_buf,i1=0,i3=0;i1<j;i1++,i++,i3++)
                {
                if((i1%50)== 0)  f_cycle();
                   if(i>=j) i=0;
                   if(i3>=i2) i3=0;
                 o.l= buf_xyz[i];
                 switch(list_ns_type[f_ns[i3]]  )
                 {
                  case T_FLOAT:
                    o1.f=o.f;
                    break;
                  case T_INT_L:
                    o1.f=(float)o.l;
                    break;
                  case T_INT:
                    o1.f=(float) *( (int *)(&o.l));
                    break;
                 }
                 putch4( o1.l);
                }
              fl_trace=0;
              goto fin;
            }
          }
/*----------------------------------------------*/
//---------------------------------
      if (!strcmp(intrpr.wrd,"CMS" ))
        {   //"CMS" 'CM' - Com Mode for Slave

          cm_baud  = ComBaudSlv    ;
          cm_lgth  = Com_lgthSlv   ;
          cm_parity= Com_paritySlv ;
          cm_stop  = Com_stopSlv   ;

          if((i1=f_dis_set(ds_cm,3,4)) >0 )
          {
           if(f_get_nb(cm_baud) < 0  ) goto fin_e;

           ComBaudSlv     = cm_baud   ;
           Com_Baud_Slave = ComBaudSlv;
           Com_lgthSlv    = cm_lgth   ;
           Com_paritySlv  = cm_parity ;
           Com_stopSlv    = cm_stop   ;

           if(flag_Slv == 0) goto fin;
           RestoreCom(ComPortSlv );
           if( InstallCom(ComPortSlv ,cm_baud,cm_lgth,cm_parity,cm_stop) < 0 ) goto fin_e;
          }
          goto fin;

        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"CMS2" ))
        {   //"CMS2" 'CM' - Com Mode for Slave2

          cm_baud  = ComBaudSlv2    ;
          cm_lgth  = Com_lgthSlv2   ;
          cm_parity= Com_paritySlv2 ;
          cm_stop  = Com_stopSlv2   ;

          if((i1=f_dis_set(ds_cm,4,4)) >0 )
          {
           if(f_get_nb(cm_baud) < 0  ) goto fin_e;

           ComBaudSlv2     = cm_baud   ;
           Com_Baud_Slave2 = ComBaudSlv;
           Com_lgthSlv2    = cm_lgth   ;
           Com_paritySlv2  = cm_parity ;
           Com_stopSlv2    = cm_stop   ;

           if(flag_Slv2 == 0) goto fin;
           RestoreCom(ComPortSlv2 );
           if( InstallCom(ComPortSlv2 ,cm_baud,cm_lgth,cm_parity,cm_stop) < 0 ) goto fin_e;
          }
          goto fin;

        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"CMS3" ))
        {   //"CMS3" 'CM' - Com Mode for Slave3

          cm_baud  = ComBaudSlv3    ;
          cm_lgth  = Com_lgthSlv3   ;
          cm_parity= Com_paritySlv3 ;
          cm_stop  = Com_stopSlv3   ;

          if((i1=f_dis_set(ds_cm,4,4)) >0 )
          {
           if(f_get_nb(cm_baud) < 0  ) goto fin_e;

           ComBaudSlv3     = cm_baud   ;
           Com_Baud_Slave3 = ComBaudSlv;
           Com_lgthSlv3    = cm_lgth   ;
           Com_paritySlv3  = cm_parity ;
           Com_stopSlv3    = cm_stop   ;

           if(flag_Slv3 == 0) goto fin;
           RestoreCom(ComPortSlv3 );
           if( InstallCom(ComPortSlv3 ,cm_baud,cm_lgth,cm_parity,cm_stop) < 0 ) goto fin_e;
          }
          goto fin;

        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"2" ))
        {   //'2 '
          f_print_dd();
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"DM" ))
        {   //'DM'

          f_dis_set(&ds_dm,2,1);
          mem_ptr=(unsigned char *)p_mem;
          for(i=0;i<64;i++)
          {
            if((i%8)==0)
              printf("\n\r%08lX  ",(unsigned long int)(p_mem+i));
            printf("  %02x",mem_ptr[i] &0xff);
          }
          goto fin;
        }
//---------------------------------
      if (!strcmp(intrpr.wrd,"VER" ))
        {   //'VER'
         printf(" %s",sw_ver);
         goto fin;
        }
//---------------------------------
//------------------------------------------
//------------------------------------------
//------------------------------------------
      if (!strcmp(intrpr.wrd,"DNM" ))
        {   //'DNM' Tank_nn
          f_dis_set(ds_dnm,3,3);
          goto fin;
        }
//------------------------------------------
//------------------------------------------
      if (!strcmp(intrpr.wrd,"DTC" ))
        {   //'DTC' Density,Temperature,Composition,Density exec.
          f_dis_set(ds_dtc,3,2);
          goto fin;
        }
//------------------------------------------
      if (!strcmp(intrpr.wrd,"LIM" ))
        {   //'LIM' Min-Max volume 
          f_dis_set(ds_lim,3,10);
          goto fin;
        }
//------------------------------------------
      if (!strcmp(intrpr.wrd,"LIM6" ))
        {   //'LIM' Min-Max volume 
          f_dis_set(&ds_lim[10],4,8);
          goto fin;
        }
//------------------------------------------
//------------------------------------------
      if (!strcmp(intrpr.wrd,"REP" ))
        {   //'REP'
          f_rep();
          goto fin;
        }
//---------------------------------

//---------------------------------
//---------------------------------
      if (!strcmp(intrpr.wrd,"DNS" ))
          {
           for(i=0;i<5;i++)
            if(f_ns[i]) PrintOp(f_ns[i]);
            else break;
           goto fin;
          }
//---------------------------------
        if (intrpr.wrd[0]=='?')
          {
           f_menu();
           goto fin;
          }
//---------------------------------
fin_e:
         if(( EchoComN==0)||( EchoComN==4))
         {
          if(flag_echo_host )
           printf("?\n\r>");
           return 0;
         }
         else
         { // посылка неопознанной команды в COM (ComPortEcho)
      //   ComPortEcho=EchoComN;

          if(ComProt[ComPortEcho]== ICP)
          {
           if( f_queue_chk(ComPortEcho) && (Host_Rqst.status == Req_OK) )
           {
              { // стать в очередь доступа к COM

                f_prepare7000(intrpr.bfcom,LICP_Rqst.CRC_flag);
                strcpy(Host_Rqst.Cmd,intrpr.bfcom);
                Host_Rqst.timeout= ToutLicpScan;
                Host_Rqst.cmd_lgth=strlen(Host_Rqst.Cmd);
                Host_Rqst.time_stamp=TimeStamp;
                f_queue_put(ComPortEcho, &Host_Rqst);
              }
           }
           else printf(" -- no place in queue COM%d-- \n\r",ComPortEcho);
          }
          else if(ComProt[ComPortEcho]== RTU)
          {
           if( f_queue_chk(ComPortEcho) && (Host_Rqst_rtu.status == Req_OK) )
           {
              { // стать в очередь доступа к COM

                if( (i1=f_chg_toRTU(intrpr.bfcom)) > 0 )
                {
                  f_prepareRTU(intrpr.bfcom,&i1);
                  _fmemcpy(Host_Rqst_rtu.Cmd,intrpr.bfcom,(long int)i1);
                  Host_Rqst_rtu.cmd_lgth=i1;
                  Host_Rqst_rtu.time_stamp=TimeStamp;
                  f_queue_put(ComPortEcho, &Host_Rqst_rtu);
                }
                else goto fin_ee;
              }
           }
           else
           {
             printf(" -- no place in queue COM%d-- \n\r",ComPortEcho);
             goto fin_ee;
           }
          }
          else if(ComProt[ComPortEcho]== 2)
          {  // передача в канал без добавления контрольной суммы
           if( f_queue_chk(ComPortEcho) && (Host_Rqst_rtu.status == Req_OK) )
           {
              { // стать в очередь доступа к COM

                if( (i1=f_chg_toRTU(intrpr.bfcom)) > 0 )
                {
           //     f_prepareRTU(intrpr.bfcom,&i1);
                  _fmemcpy(Host_Rqst_rtu.Cmd,intrpr.bfcom,(long int)i1);
                  Host_Rqst_rtu.cmd_lgth=i1;
                  Host_Rqst_rtu.time_stamp=TimeStamp;
                  f_queue_put(ComPortEcho, &Host_Rqst_rtu);
                }
                else goto fin_e;
              }
           }
           else
           {
             printf(" -- no place in queue COM%d-- \n\r",ComPortEcho);
             goto fin_ee;
           }

          }
          else if(ComProt[ComPortEcho]== 3)
          {  // передача в канал символьной информации без добавления контрольной суммы
             // в конце строки добавляется 0xd
           if( f_queue_chk(ComPortEcho) && (Host_Rqst.status == Req_OK) )
           {
              { // стать в очередь доступа к COM

                strcpy(Host_Rqst.Cmd,intrpr.bfcom);
                Host_Rqst.timeout= ToutLicpScan;
                i1=strlen(Host_Rqst.Cmd);
                Host_Rqst.Cmd[i1]=0xd;
                Host_Rqst.Cmd[i1+1]=0;
                Host_Rqst.cmd_lgth=i1+1;
                Host_Rqst.time_stamp=TimeStamp;
                f_queue_put(ComPortEcho, &Host_Rqst);
              }
           }
           else
           {
             printf(" -- no place in queue COM%d-- \n\r",ComPortEcho);
             goto fin_ee;
           }
          }

         }
         return 0;
fin_ee:
 if(flag_echo_host )
   putch('?');
fin:
 if(flag_echo_host )
         if(( EchoComN==0)||( EchoComN==4))
          printf("\n\r>");

         else
          printf("\n\rCOM%d>",ComPortEcho);
          return 0;

     }
/* ---------------------------------------- */
void f_rep()
{

 struct s_icp_dev *ICPl_DEV;
int i,i2=0;
int itmp;

 for(i=0;i<=icp_lst_max;i++)
   if(ICP_error[i])
   {
    i2 |=ICP_error[i];
    ICPl_DEV=ICP_dd[i];
  if(i != icp_lst_max)
   printf("\r\nОшибка в драйвере %9s:",ICPl_DEV->name );
  else
   printf("\r\nОшибка системы учета: ");

    itmp=ICP_error[i];
  if(i != icp_lst_max)
  {
    if(itmp & RD_ERR)
      printf("\r\nОшибка чтения устройства");
    if(itmp & WR_ERR)
      printf("\r\nОшибка записи в устройство");
    if(itmp & WD_ERR)
      printf("\r\nОшибка Watch Dog");
    if(itmp & Drive_ERR)
      printf("\r\nОшибка в устройстве,E%02X", (unsigned int) ((itmp>>8) & 0xff) );
  }
  else //(i == icp_lst_max)
  {
    if(itmp == EEE_CRC_error)
       printf("\r\nОшибка CRC EEPROM процессора");
    if(itmp & Flash_erase_error )
       printf("\r\nОшибка стирания FLASH");
    if(itmp & Flash_wr_error  )
       printf("\r\nОшибка записи во FLASH");
    if(itmp & No_F_dat_err )
       printf("\n\rНет обновл.данных для журнала");
    if(itmp == WD_host )
       printf("\r\nСторожевой таймер HOST");
    if(itmp == No_flow )
       printf("\nПоток прекратился");
    if(itmp & ES_off )
       printf("\nНажата аварийная кнопка (ES) при отпуске/приеме");
    if(itmp & UZA_off )
       printf("\nНет сигнала УЗА при отпуске/приеме");
     //  printf("\nНет УЗА при отпуске");
    // printf("\nУровень в автоцистерне выше допустимого");
    //printf("\nНекорректный ID ");

    if((itmp & Hst_alm)==Hst_alm)
      printf("\nАвария от HOST ");
    if((itmp & No_Drv)==No_Drv)
      printf("\nДрайверы MVD,7060 не включены");
  }

   }
   if(i2==0)
       printf("\r\n Ошибок нет");

}
/* ---------------------------------------- */

char *list_mnu[]={
//char list_mnu[90][82]={
 "\n\rPDL - вывод последних p1 событий ",
 "\n\rQ - выход ",
"",
};

void   f_menu()
{
int i;
 for(i=0;list_mnu[i][0]!=0;i++)
 {
  puts(list_mnu[i]);
  if(((i%22) == 0)&& (i !=0)  )
  {
    printf("\n\r-- more --");
    while(kbhit()==0) f_cycle0();
    if(getch()== 0xd) return;
  }
 }

}
//-------------------------------
void putch2(long int i1 )
{
  putch(i1&0xff); putch((i1>>8)&0xff);
}
//-------------------------------
void putch4(long int i1 )
{
  putch(i1&0xff);
  putch((i1>>8)&0xff);
  putch((i1>>16)&0xff);
  putch((i1>>24)&0xff);
}
//-------------------------------
void f_get_wrd()
{
                 while(!f_inw())
                 {
                   puts ("\n\r");

                   while(!ffgets_host(ComPortHost));

                   strcpy(intrpr.bfcom,cb_COM[ComPortHost]);
                   n_bcom[ComPortHost]=0;
                   intrpr.i3=0;
                 }
}
//-------------------------------
