
//====================================
// список доступных драйверов устройств
/*----------------------*/
struct s_icp_dev  *ICP_dd[]={

#if defined( DIGIMAG )
        &DGM[0],
        &DGM[1],
        &DGM[2],
#endif

#if defined(PMP201)
        &PMP[0],
        &PMP[1],
        &PMP[2],
        &PMP[3],
        &PMP[4],
        &PMP[5],
        &PMP[6],
        &PMP[7],
        &PMP[8],
/*
		&PMP[9],
		&PMP[10],
		&PMP[11],
		&PMP[12],
		&PMP[13],
		&PMP[14],
		&PMP[15],
*/
#endif

#if defined(ICP_7060)
    &I7060[0],  //6
#endif

&MMI, // 1
&WDOG[0], //0

NULL};

int icp_lst_max=(sizeof(ICP_dd) / sizeof(&MMI)) - 1 ;
#define ICP_ERR_N  16
int ICP_error[ICP_ERR_N]={0,0,0,0,0,0,0,0,0,0};
int ICP_error_c[ICP_ERR_N]={0,0,0,0,0,0,0,0,0,0};
int FL_err=0;
//====================================
//  Общие функции для устройств ICP
//====================================

int ffgets_com_ICP( int ii)
{
//  Адрес функции будет устанавлен в указателе  ff_gets_com[Port]
// при посылке команды в устройство ICP ( MMI, 7017C и пр.).
//  Функция принимает символы из порта COM ii  в буфер cb_COM[ii].
//  При получении разделителя команда считается введенной и функция возвращает
// значение 1.
//  Если прошло время ToutAns[ii] мс с момента приема последнего символа
// или с момента передачи команды - фиксируется таймаут, и в указатель
// функции обработчика ff_answ_com записывается адрес функции для обработки
// таймаута ( адрес которой записан в указателе ff_answ_flt).
//

/* ii - номер COM   1...4 */
 char tmp;
 int i;
struct COM_rqst *request;

m1:
     if(IsCom(ii)==0)
     {
        if( f_timer(time_snd[ii],ToutAns[ii]))
        { // истек таймаут приема ответа на команду
           ff_answ_com[ii]=ff_answ_flt[ii];
    //     printf("\n ToutAns = %ld, n_bcom = %d\n",ToutAns[ii],n_bcom[ii]);
    //     for(i=0;i<n_bcom[ii];i++)
    //      putch(cb_COM[ii][i]);

//    request=COM_que[ii].request[COM_que[ii].empt_ptr];
//    ToComBufn(4,request->Cmd,request->cmd_lgth);
//    printf("\nt_send= %ld",time_snd[ii]-request->time_stamp );
//    printf("\nt_cur= %ld",TimeStamp-request->time_stamp );

//         printf("\n");
           return 1;
        }
        return 0;
      }
     tmp=ReadCom(ii);
     cb_COM[ii][n_bcom[ii]]=tmp;
     if( (++n_bcom[ii]) >= n_bufcom )
      {
        cb_COM[ii][n_bufcom-1]=0;
        n_bcom[ii]=0;
  /*
        printf("\n Command buffer overflow !\n--");
        puts(cb_COM1);
  */
      }

   switch(tmp)
   {
    case 0x0:
    case ';':
    case '\n':
    case '\r':
    n_bcom[ii]--;
 cb_COM[ii][n_bcom[ii]]=0;
//printf("\n End string\n");
    return 1;
   }
   time_snd[ii]= TimeStamp;
   goto m1;
}
/* ---------------------------------------- */
int f_empty_gets(int ii)
{
/*  ii - номер COM   1...4 */
//  Функция вызывается, если никакой запрос не послан и ответа не ожидается.
//  Все поступающие из порта символы принимаются и не используются.
//   Например, если после таймаута придут несколько символов , они не должны
//  попасть в ответ на следующий запрос и должны быть считаны.
char tmp;
m1:
  if(IsCom(ii)!=0)
  {
    tmp=ReadCom(ii);
    if(flag_echo_host & flag_Deb)
        printf("\n\r$C%d %02X",ii,(int)tmp & 0xff);
    goto m1;
  }
  return 0;
}
/* ---------------------------------------- */
int f_check_crc_ICP(char *buf)
{
 // функция проверяет команду в буфере *buf на корректность контрольной суммы
 // Возвращает  1 -OK
 //             0 - error CRC
  int i,count;
  unsigned int sum;
  count=strlen(buf);
  if(count<3)
    return 0;
  count-=2;
  sum=0;
  for(i=0;i<count;i++)
     sum+=*buf++;

  if(*buf++ == ( hex_to_ascii[(sum>>4)& 0xf]) )
    if(*buf  == (hex_to_ascii[sum&0xf])) return 1;
  return 0;
}
/*----------------------------*/
int f_prepare7000(char *Cmd,int CRC_flag)
{ //  Если CRC_flag != 0, функция вычисляет конторольную
  //  сумму команды в буфере Cmd
  //  и записывает  ее после команды.
  //  Возвращает 0 - OK,
  //             1 - нет места в буфере

unsigned int sum;
int i,i1;

 i1=strlen(Cmd);

 if(CRC_flag)
 {
   if( i1 >= (n_buf_queue-4)) return 1;
   sum=0;
   for(i=0;Cmd[i];i++)
    sum+=Cmd[i];
  Cmd[i++]=hex_to_ascii[(sum>>4)& 0xf];
  Cmd[i++]=hex_to_ascii[sum&0xf];
  Cmd[i++]='\r';
  Cmd[i]=0;
  return 0;
 }

   if( i1 >= (n_buf_queue-2)) return 1;
   Cmd[i1++]='\r';
   Cmd[i1]=0;
   return 0;
}
/*----------------------------*/
void f_icp_error(struct s_icp_dev *ICP_DEV,int error_type )
{
int i,fl_evt;
int fl_err_new;
 fl_err_new=0;
 fl_evt=0;
 for(i=0;i<icp_lst_max;i++)
 {
   if(ICP_DEV == ICP_dd[i])
   {
     if(error_type)
     {
         ICP_error[i] |= error_type;
         if(ICP_error[i] != ICP_error_c[i])
         {
           fl_evt=1;
         }
     }
     else
         ICP_error[i] = 0;
     ICP_error_c[i]=ICP_error[i];
   }

   if(ICP_error[i]) fl_err_new++;

 }
  if(ICP_error[i]) fl_err_new++;


  if((fl_err_new != 0) && (FL_err == 0)&& (sw_mmi==0)) f_prn_error();

    FL_err=fl_err_new;
    if(FL_err)
    {
      State_SLV=-1;
      WD_PMP_flag=0;
      if(fl_evt)
      {
         f_wr_evt(evt_err);
      }
    }
}
/*----------------------------*/
void f_icp_errS(int error_type )
{
int i,fl_evt;
int fl_err_new;

 fl_err_new=0;
 fl_evt=0;
   if(error_type)
   {
       ICP_error[icp_lst_max] |= error_type;
   }
   else
       ICP_error[icp_lst_max] = 0;


 if(ICP_error[icp_lst_max] != ICP_error_c[icp_lst_max])
  {
    fl_evt=1;
  }

 ICP_error[icp_lst_max+1]=0;

 ICP_error_c[icp_lst_max]=ICP_error[icp_lst_max];

 for(i=0;i<=icp_lst_max;i++)
 {
   if(ICP_error[i]) fl_err_new++;
 }

    if((fl_err_new != 0) && (FL_err == 0))
    {
    if(sw_mmi==0) f_prn_error();
    }

    FL_err=fl_err_new;
    if(FL_err)
    {

      State_SLV=-1;
      WD_PMP_flag=0;
      if(fl_evt)
      {
        f_wr_evt(evt_err);
      }
    }
}
/*----------------------------*/
int f_get_icp_error(struct s_icp_dev *ICP_DEV )
{
int i;
 for(i=0;i<icp_lst_max;i++)
   if(ICP_DEV == ICP_dd[i])
       return ( ICP_error[i]);
 return 0;
}
/*----------------------------*/
void f_cl_error()
{
int i;


#if defined(ICP_7060)
  for(i=0;i<max_7060;i++)
    I7060_fl[i]=0;
#endif

#if defined(RTU_ComSK)
  for(i=0;i<max_SK;i++)
    ComSK_fl[i]=0;
#endif
#if defined(RTU_Delta)
  for(i=0;i<max_Dlt;i++)
    Delta_fl[i]=0;
#endif

#if defined(TSD_GLB)
     TSD_GLB_fl[0]=0;
#endif

  for(i=0;i<ICP_ERR_N;i++)
  {
    ICP_error[i]=0;
    ICP_error_c[i]=0;
  }

  for(i=0;i<max_PMP;i++)
  {
    PMP_inp_err[i]=0;
  }


//  if(FL_err) flag_fst_pmp1=1;
  FL_err=0;
//modf &= ~pmp1_err_bit;

//  if(flag_pmp_init == 0)
  f_wr_evt(evt_clr_err);
//  else flag_pmp_init=0;
}

/*----------------------------*/
// фиктивные структуры для сохранения последовательности данных в EEPROM
/*----------------------------*/
struct s_icp_dev ICP_fict=
{
0,                // status
1,                // port
02,               // addr
"FICT",           // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
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
NULL,   // *request
//--------------------------------
};
/*----------------------------*/
struct COM_rqst Rqst_fict={
//----------------------
0,                     //  состояние запроса
NULL,                  //  gets_com
NULL,                  //  answ_com
NULL,                  //  answ_flt
"FICT",                //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&Host,                 //  *ICP_dd;
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
};
/*----------------------------*/
long int fict_long=0;

