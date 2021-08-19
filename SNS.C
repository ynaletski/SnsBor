
/*
     Main

     Display ПМП201  state.
     По значению уровня и геометрии резервуара вычисляется объем
  жидкой и паровой фаз.
     По измеренным значениям  плотности жидкой фазы и температуры измерения определяется
  состав жидкой фазы  ( массовая доля пропана).

     По составу жидкой фазы и текущей температуре жидкой фазы определяется
  текущая плотность жидкой фазы.

     По составу жидкой фазы и температуре паровой фазы определяется
  плотность паровой фазы пропана-бутана (без учета этана) и
  давление паровой фазы пропан-бутана.


     Вычисляется разность давлений в резервуаре и полученного давления.
     Принимается , что разность давлений образуется неучтенным этаном.
     Вычисляется плотность этана по заданному давлению и температуре.

     Общая плотность паровой фазы вычисляется сложением плотности этана и
  вычисленной плотности пропан-бутана.

     По составу жидкой фазы и температуре жидкой фазы определяется
  плотность жидкой фазы в резервуаре.
     По плотности и объему жидкой фазы вычисляется масса жидкой фазы.
     По плотности и объему паровой фазы вычисляется масса паровой фазы.

*/

#include "ALL.h"

//  Верхний котроллер АГЗС

int fl_inv_out=1; // 1 - если переполнение - реле выключено

unsigned int INP_inv=0;    // маска инверсии входов
unsigned int INP_VARi=0;    // состояние дискретных входов
int  flag_fst_gun=0;

char sw_ver[20]="SNS.03 29 Jun 2016";  // COM0 COM1 COM2 COM3 COM4

unsigned long ComBaud[5]={9600,9600,9600,19200,115200};
int Com_lgth[5]={8,8,8,8,8};
int Com_parity[5]={0,0,0,0,0};//0-(None);1-(Even);2-(Odd);3-(MARK, always 1); 4-(SPACE,always 0)

int Com_stop[5]={1,1,1,1,1};

int  ComProt[5]={0,0,0,0,0};

long int baudrate_val[]={1200,1200,1200,1200,2400,4800,9600,19200,38400,57600,115200};

int  MMI_inp_err=0;
int  MMI_out_err=0;

#if defined(MMI_NEW)
int  ZeroPage=2;
int  EmptPage=2 ;
#endif
//------------------------
int f_get_nb( long int baud)
{
int i;
 for(i=3;i<11;i++)
  if(baudrate_val[i]== baud)
    return i;
 return -1;
}
//--------------------------
void ShowPort(int port)
{
  Show5DigitLedWithDot(1,port);
}
void ShowChecksum(int checksum)
{
  Show5DigitLedWithDot(2,checksum);
}
void ShowBaudrate(long baud)
{ int data;

  if(baud>100000) data=baud/1000;
  else data=baud/100;

  Show5DigitLed(5,data%10);
  data/=10;
  Show5DigitLed(4,data%10);
  data/=10;
  Show5DigitLed(3,data%10);
}

void ShowCom( int port,int checksum,long int baud)
{
  ShowPort(port);
  ShowChecksum(checksum);
  ShowBaudrate(baud);

}
//-------------------
int i_ish;
void ShowNumber(long Num)
{ // выводит число в десятичной форме на 5DigitLed
int i,i1,dig;
char buf[10];

i_ish=Num;
return;
/*
  sprintf(buf,"%ld",Num);
  i=strlen(buf);
  if(i>0) i--;

  for(i1=0;i1<5;i1++,i--)
  {
    if(i>=0)
    {
     dig=buf[i]-'0';
    }
    else dig=0;
      Show5DigitLed(i1,dig);
  }
  printf("\n\r==%s==\n\r",buf);
*/

}
//-------------------
int getch_host()
{
while(!IsCom(ComPortHost));
 return(ReadCom(ComPortHost));
}
//-------------------
int puts_host(char *str)
{
int i;
 for(i=0;str[i];i++)
 putch(str[i]);
 return i;
}
//-------------------
long int time_DGM[max_PMP]={0,0,0};

void  f_cycle_cmn()
{
 int ii;
     f_time_dd();
     if( WDOG[0].status)  f_WDOG(0);

     f_inp7188();

 i_LVL=0;
 for(ii=0;ii<max_PMP;ii++)
 {
   if(  (Fl_ch_mm[ii] != 0)  &&   ((  INP_VAR2 & ctr_msk[ii]) == 0))
   {
      if(ICP_error[ii] !=0)
      {
        ii_LVL[ii] |= OUT1 ;
        ii_LVL[ii] |= OUT2 ;
        i_LVL |=ii_LVL[ii];
        break;
      }
   }
   else
   {
        ii_LVL[ii] = 0;
   }

   i_LVL |=ii_LVL[ii];

 }

     if(fl_bl_out == 0)
        {
            // i_LVL == ( OUT1 | OUT2 ) - авария MIN,MAX
                  if(fl_inv_out)
          {   // если переполнение - реле выключено
         // if(FL_err)
         //    OUT_VAR=0;
         // else
               OUT_VAR=i_LVL ^ ( OUT1 | OUT2);
          }
          else
          {   // если переполнение - реле включен
         // if(FL_err)
         //    OUT_VAR=OUT1 | OUT2;
         // else
               OUT_VAR=i_LVL ;
          }
        }


#if defined(ICP_7060)

        i7060_out[0]=OUT_VAR & 0xff;
        if(I7060[0].status)  f_7060(0);
        else I7060_fl[0]=0;
//      if(I7060[1].status)  f_7060(1);
//      else I7060_fl[1]=0;
#endif


#if defined( DIGIMAG )
        if(DGM[0].status)  f_DGM(0);
        else
        {
         if(DGM[0].pool_time > 500)
          if(  f_timer(time_DGM[0],DGM[0].pool_time) == 0);
          else
          {
            time_DGM[0]=TimeStamp;
            f_count_tank(0);
          }
        }

        if(DGM[1].status)  f_DGM(1);
        else
        {
         if(DGM[1].pool_time > 500)
          if(  f_timer(time_DGM[1],DGM[1].pool_time) == 0);
          else
          {
            time_DGM[1]=TimeStamp;
            f_count_tank(1);
          }
        }

        if(DGM[2].status)  f_DGM(2);
        else
        {
         if(DGM[2].pool_time > 500)
          if(  f_timer(time_DGM[2],DGM[2].pool_time) == 0);
          else
          {
            time_DGM[2]=TimeStamp;
            f_count_tank(2);
          }
        }
#endif

//-----------------------
#if defined(PMP201)


        if(PMP[0].status)  f_PMP(0);
        else
        {
         if(PMP[0].pool_time > 500)
          if(  f_timer(time_DGM[0],PMP[0].pool_time) == 0);
          else
          {
            time_DGM[0]=TimeStamp;
            f_count_tank(0);
          }
        }

      if(Tank_nn >1)
        if(PMP[1].status)  f_PMP(1);
        else
        {
         if(PMP[1].pool_time > 500)
          if(  f_timer(time_DGM[1],PMP[1].pool_time) == 0);
          else
          {
            time_DGM[1]=TimeStamp;
            f_count_tank(1);
          }
        }

      if(Tank_nn >2)
        if(PMP[2].status)  f_PMP(2);
        else
        {
         if(PMP[2].pool_time > 500)
          if(  f_timer(time_DGM[2],PMP[2].pool_time) == 0);
          else
          {
            time_DGM[2]=TimeStamp;
            f_count_tank(2);
          }
        }

      if(Tank_nn >3)
        if(PMP[3].status)  f_PMP(3);
        else
        {
         if(PMP[3].pool_time > 500)
          if(  f_timer(time_DGM[3],PMP[3].pool_time) == 0);
          else
          {
            time_DGM[3]=TimeStamp;
            f_count_tank(3);
          }
        }

      if(Tank_nn >4)
        if(PMP[4].status)  f_PMP(4);
        else
        {
         if(PMP[4].pool_time > 500)
          if(  f_timer(time_DGM[4],PMP[4].pool_time) == 0);
          else
          {
            time_DGM[4]=TimeStamp;
            f_count_tank(4);
          }
        }

      if(Tank_nn >5)
        if(PMP[5].status)  f_PMP(5);
        else
        {
         if(PMP[5].pool_time > 500)
          if(  f_timer(time_DGM[6],PMP[6].pool_time) == 0);
          else
          {
            time_DGM[5]=TimeStamp;
            f_count_tank(5);
          }
        }


      if(Tank_nn >6)
        if(PMP[6].status)  f_PMP(6);
        else
        {
         if(PMP[6].pool_time > 500)
          if(  f_timer(time_DGM[6],PMP[6].pool_time) == 0);
          else
          {
            time_DGM[6]=TimeStamp;
            f_count_tank(6);
          }
        }


      if(Tank_nn >7)
        if(PMP[7].status)  f_PMP(7);
        else
        {
         if(PMP[7].pool_time > 500)
          if(  f_timer(time_DGM[7],PMP[7].pool_time) == 0);
          else
          {
            time_DGM[7]=TimeStamp;
            f_count_tank(7);
          }
        }

      if(Tank_nn >8)
        if(PMP[8].status)  f_PMP(8);
        else
        {
         if(PMP[8].pool_time > 500)
          if(  f_timer(time_DGM[8],PMP[8].pool_time) == 0);
          else
          {
            time_DGM[8]=TimeStamp;
            f_count_tank(8);
          }
        }

#endif
//-----------------------


     if((flag_Slv !=0 )&&(ComPortSlv == 1 ) );
     else if((flag_Hst  !=0 )&&(ComPortHost == 1) );
//   else if((flag_Slv2 !=0 )&&(ComPortSlv2 == 1 ));
//   else if((flag_Slv3 !=0 )&&(ComPortSlv3 == 1 ));
     else  ServiceCOM(1);

     if((flag_Slv !=0 )&&(ComPortSlv == 2 ));
     else if((flag_Hst  !=0 )&&(ComPortHost == 2) );
//   else if((flag_Slv2 !=0 )&&(ComPortSlv2 == 2 ));
//   else if((flag_Slv3 !=0 )&&(ComPortSlv3 == 2 ));
     else  ServiceCOM(2);


     if((flag_Slv !=0 )&&(ComPortSlv == 3 ));
     else if((flag_Hst  !=0 )&&(ComPortHost == 3) );
//   else if((flag_Slv2 !=0 )&&(ComPortSlv2 == 3 ));
//   else if((flag_Slv3 !=0 )&&(ComPortSlv3 == 3 ));
     else
     {
        ServiceCOM(3);
     }


     if((flag_Slv !=0 )&&(ComPortSlv == 4 ));
     else if((flag_Hst  !=0 )&&(ComPortHost == 4) );
//   else if((flag_Slv2 !=0 )&&(ComPortSlv2 == 4 ));
//   else if((flag_Slv3 !=0 )&&(ComPortSlv3 == 4 ));
     else  ServiceCOM(4);


       f_one_sec();
       if(ff_serv != NULL) (*ff_serv)(); // сканирование магистрали,
       f_store_dt();
}
/* ================================================== */

/* ================================================== */
int f_cycle()
{  // основной цикл

     f_cycle_cmn();

     if(MMI.status)
     {
       if(f_menu_MMI()) return -1;
       f_MMI();
     }

     f_sens();

     if(flag_Slv !=0 )
           f_SlaveRTU();
/*
     if(flag_Slv2 !=0 )
           f_Slave2RTU();

     if(flag_Slv3 !=0 )
           f_Slave3RTU();
*/

     if((flag_Slv !=0 )&&(ComPortSlv == ComPortHost) );
//   else if((flag_Slv2 !=0 )&&(ComPortSlv2 == ComPortHost) );
//   else if((flag_Slv3 !=0 )&&(ComPortSlv3 == ComPortHost) );
     else if(flag_Hst !=0)
             if(f_host()) return -1;
//     trace();
     return 0;
}
/* ================================================== */
int f_cycle0()
{   // без вызова f_host()
     f_cycle_cmn();

       if(MMI.status)
       {
         if(f_menu_MMI()) return -1;
         f_MMI();
       }

//     trace();
     return 0;
}
//-------------------
int f_cycle1()
{   // без вызова f_MMI()

     f_cycle_cmn();

     if((flag_Slv !=0 )&&(ComPortSlv == ComPortHost ) );
     else if(flag_Hst !=0)
             if(f_host()) return -1;

     if(flag_Slv !=0 )
           f_SlaveRTU();

//     trace();
     return 0;
}
/* ================================================== */

/* ---- main ------------------------------------------------------------- */

  int ttt1,ttt2,ttt3,ttt4;
  int itt1,itt2,itt3,itt4;
  long int rltt1,rltt2,rltt3,rltt4;
  long int ltt1,ltt2,ltt3,ltt4;
  int  flag_prn=0;
  int tmp_mn0=0,tmp_mn1=1,tmp_mn2=2;

  unsigned long ttt1_l,ttt2_l,ttt3_l;
int FlagWinSum=0;
  int flag_H=0;
void main(void)
{
  double dtmp1,dtmp2,dtmp3,dtmp4;
  float ftmp,ftmp1,ftmp2,ftmp3,ftmp4;
  int iitmp1,iitmp2,iitmp3;
  int i,j;

  char cmd[40];
//  int checksum=1;
  int key;
  int iRet;
  int year,month,day,hour,min,sec;

  int itmp;
/*
  if(!Is7188xa())
  {
    printf("MMICON.EXE must run on I-7188xa");
    exit (0);
  }
  else
    printf("7188xa is found. Flash size %d\n",itmp);
*/

  InitLib();
  tzset();
  f_ns_count();

  daylight=0;
  timezone=0;

  ComPortHost=4;
//ComPortHost=3;

  *(long int *)&(TimeStamp)=0;
  InstallCom(ComPortHost,BaudHost,8,0,1);

  f_get_SlvBuf();
  f_zer_tct();

  Com_Baud_Slave=ComBaudSlv;
  Com_Baud_Slave2=ComBaudSlv2;
  Com_Baud_Slave3=ComBaudSlv3;

  for(;TimeStamp < 600;)
  {
    if(!IsCom(ComPortHost)) continue;
    key=ReadCom(ComPortHost);
    if(key == '\b')
    {
      flag_H=1;
      printf("\n\r BS pushed.RTU Slave disabled.");
      while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
      flag_Hst=1;
      break;
    }
    if(key == 0x1b )
    {
      flag_H=2;
      printf("\n\r ESC pushed  ");
      while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
      flag_Hst=1;
      break;
    }
  }

  RestoreCom(ComPortHost);

//  X607_Init();
  InitEEPROM();

  f_get_flash_p();

  f_wr_evt(evt_t_off);
  f_wr_evt(evt_t_on);


  flag_echo_host=1;

  f_queue_init();
  f_ee_num_init();
  Init5DigitLed();


 if( flag_H == 2)  goto m_n_ee;
  if(f_chk_EEE_CRC()<0)
  {
 m_n_ee:
    InstallCom(ComPortHost,BaudHost,8,0,1);
    ShowCom(ComPortHost,8,BaudHost);
    printf("CRC Error.Default loaded\n");
    f_icp_errS(EEE_CRC_error);
    sw_mmi=99;
  }
  else
  {
    f_rd_eee();

    f_rd_tct();


    if( flag_H != 0)
    {
      flag_Slv=0;
      flag_Slv2=0;
      flag_Hst=1;
    }
    if((flag_Slv !=0)&&(ComPortHost == ComPortSlv ) );
//  else if((flag_Slv2 !=0)&&(ComPortHost == ComPortSlv2 ) );
//  else if((flag_Slv3 !=0)&&(ComPortHost == ComPortSlv3 ) );
    else if(flag_Hst != 0)
    {
     if(f_get_nb(ComBaud[ComPortHost]) <0 ) ComBaud[ComPortHost]=115200L;
     InstallCom(ComPortHost,BaudHost,8,0,1);
     ShowCom(ComPortHost,8,BaudHost);
    }

    if(flag_Slv==1)
    {
     if(f_get_nb(ComBaudSlv) <0 ) ComBaudSlv=9600L;
     InstallCom(ComPortSlv,ComBaudSlv,Com_lgthSlv,Com_paritySlv,Com_stopSlv);
     if(ComPortSlv==ComPortHost)
       ShowCom(ComPortSlv,Com_lgthSlv,ComBaudSlv);
    }
/*
    if(flag_Slv2==1)
    {
     if(f_get_nb(ComBaudSlv2) <0 ) ComBaudSlv2=9600L;
     InstallCom(ComPortSlv2,ComBaudSlv2,Com_lgthSlv2,Com_paritySlv2,Com_stopSlv2);
     if(ComPortSlv2==ComPortHost)
       ShowCom(ComPortSlv2,Com_lgthSlv2,ComBaudSlv2);
    }

    if(flag_Slv3==1)
    {
     if(f_get_nb(ComBaudSlv3) <0 ) ComBaudSlv3=9600L;
     InstallCom(ComPortSlv3,ComBaudSlv3,Com_lgthSlv3,Com_paritySlv3,Com_stopSlv3);
     if(ComPortSlv3==ComPortHost)
       ShowCom(ComPortSlv3,Com_lgthSlv3,ComBaudSlv3);
     n_GRK_a=2;
    }
    else
     n_GRK_a=1;
*/

    if(FlagWinSum) init_win_sum(1);
  }

  f_clr_scr_MMI();

  n_GRK_a=0;

  f_prep_VM( tmp_mn0 );
  f_prep_VM( tmp_mn1 );
  f_prep_VM( tmp_mn2 );

  Com_Baud_Slave=ComBaudSlv;
  Com_Baud_Slave2=ComBaudSlv2;
  Com_Baud_Slave3=ComBaudSlv3;

           if( f_check_id(n_id) == 0)
           {
            id_ok=1;
           }
           else
           {
            id_ok=0;
           }
/*=========
    if(EepType==16)
     printf("EEPROM 24LC16 is found (2k bytes)\n");
    else if (EepType==128)
     printf("EEPROM 24WC128 is found (16k bytes)\n");
    else
     printf("Unknown EEPROM.\n");
===========*/
//  TimerOpen();

   if((flag_Slv==0)||(ComPortHost != ComPortSlv ) )
   { if(flag_Hst!=0)
     {
      printf("\nFacom Ver.%s",sw_ver);
      if(id_ok==0)
            printf("\n\r == Not correct ID !");
     }
   }

  for(i=1;i<5;i++)
  {
   if ( (flag_Hst!=0) && (i == ComPortHost)) ;
   else if((flag_Slv !=0)&&(i == ComPortSlv ) );
// else if((flag_Slv2 !=0)&&(i == ComPortSlv2 ) );
// else if((flag_Slv3 !=0)&&(i == ComPortSlv3 ) );
   else
   {
    if(f_get_nb(ComBaud[i]) <0)  ComBaud[i]=9600L;
    InstallCom(i,ComBaud[i],Com_lgth[i],Com_parity[i],Com_stop[i]);
   }
  }

 if((flag_Slv !=0)&&(ComPortHost == ComPortSlv ) );
// else if((flag_Slv2 !=0)&&(ComPortHost == ComPortSlv2 ) );
 else if(flag_Hst!=0)
   printf("\n\r>");

  one_sec_time=TimeStamp;
/*
  if(f_rd_Dns() )
   {
    FlagNewDT=7;
    New_Dens1=D1_eee;
    New_Temp1=T1_eee;
    New_Dens2=D2_eee;
    New_Temp2=T2_eee;
    New_Dens3=D3_eee;
    New_Temp3=T3_eee;
   }


    if(flag_Slv3==1)
      n_GRK_a=2;
    else
      n_GRK_a=1;
*/

  n_res_a=Tank_nn;

  SetDisplayPage(ZeroPage);

 // if(sw_mmi==0 )
  {
   f_prn_begin();
  }

  f_init_mki();

  InstallUserTimerFunction_us(10000,fun_tim_u);

  while(1)
  {
     if(f_cycle()<0) break;
  }

printf("\n\rExit\n\r");

  StopUserTimerFun();
  RestoreCom(1);
  RestoreCom(2);
  RestoreCom(3);
  while(!IsCom4OutBufEmpty()) ;  /* Wait all message sent out of COM4 */
  RestoreCom(4);
  f_free_SlvBuf();
}

/* ================================================== */
int last_out[5]={-1,-1,-1,-1,-1};
long int scom_tim[5]={30,30,30,30,30};
long int Tm_snd[5]= {0,0,0,0,0};
//unsigned long ToutAns[5]= {200,200,200,200,200};

void ServiceCOM ( int ii)
/*
  Функция принимает ответ из SLAVE контроллера
  Функция принимает ответ на команду посланную в  COM ii.
  После завершения приема команды, принятая команда обрабатывается
  потребителем, пославшим команду в Slave
*/
{
int i,Addr_rcv,itmp;
struct COM_rqst *request;
struct s_icp_dev *ICPl_DEV;
int  tmp;

 if(scom_tim[ii]!=0)
  if(  f_timer(time_rcv[ii],scom_tim[ii]) == 0)
  {
   if(IsCom(ii)!=0)
   {
     while(IsCom(ii)!=0)
     {
      tmp= ReadCom(ii);

    if(flag_Deb)
      printf("\n%02X",tmp&0xff);
      time_rcv[ii]= TimeStamp;
     }
   }
   return;
  }

  if(flag_Deb)
      printf("\n sServ1");

  if(COM_que[ii].busy_ptr != COM_que[ii].empt_ptr)
  {
    if(last_out[ii] != COM_que[ii].empt_ptr)
    {
      request=COM_que[ii].request[COM_que[ii].empt_ptr];
      if(request==NULL)
         goto m1;

      last_out[ii]=COM_que[ii].empt_ptr;

      // подготовка к обслуживанию запроса и вывод в порт

      ff_gets_com[ii]=request->gets_com;
      ff_answ_com[ii]=request->answ_com;
      ff_answ_flt[ii]=request->answ_flt;
      ToutAns[ii]=request->timeout;
      time_snd[ii]= TimeStamp;
      Tm_snd[ii]= TimeStamp;

      ICPl_DEV=request->ICP_dd;
      ICPl_DEV->time_stamp_snd=TimeStamp-request->time_stamp;

      // вывод в порт
//  ltt1=TimeStamp;
//  tim1();

#if defined(I7188D)
  Set485DirToTransmit(ii); //qqq
#endif

   ToComBufn(ii,request->Cmd,request->cmd_lgth);

          if(flag_prn)
            if(ii == 3)
            {
             printf("\n\r Out to COM");
             printf(" %d\n\r",ii);

             for(i=0;i<request->cmd_lgth;i++)
               printf(" %2x",(int) (request->Cmd[i] & 0xff) );
            }

/*
   if(ii == 3)
   {  printf("\n\rOUT_C3=");
      for(i=0;i<request->cmd_lgth;i++)
      putch(request->Cmd[i]);
   }
*/
#if defined(I7188D)
  WaitTransmitOver(ii);
  Set485DirToReceive(ii);
#endif

//  tim2();
//  ltt2=TimeStamp;

//   itt1=ttt2-ttt1-10;  // 10 следует отнять - это время присутствует
//   if(itt1 < 0) itt1+=10000;
//   rltt1=ltt2-ltt1;

//   if(flag_prn)
//   {
//     flag_prn=0;
//     printf("\n %d bytes send, time=%ld ms, %.1f mks",request->cmd_lgth,rltt1,(float)(itt1)/10.);
//   }
      request->status=2;
    }
  }

m1:
  if( (*ff_gets_com[ii])(ii) )
 /* Ввод ответа на команду. Указатель на функцию ввода
    устанавливает функция, инициировавшая передачу команды
 */
  {
     time_rcv[ii]= TimeStamp;
     request=COM_que[ii].request[COM_que[ii].empt_ptr];
     ICPl_DEV=request->ICP_dd;
     ICPl_DEV->time_stamp_rcv=TimeStamp - request->time_stamp;

   /* в cb_COM[ii][] находится команда длиной  n_bcom[ii]  */

   /* Обработка команды. Указатель на функцию обработки устанавливает
      функция, инициирующая передачу
   */

     ff_gets_com[ii]=f_empty_gets;
     (*ff_answ_com[ii])(ii);

     ff_answ_com[ii]=f_empty;
     n_bcom[ii]=0;
  }
}
/* ================================================== */
int f_timer(unsigned long int time_stamp,unsigned long int delay)
{
// если время с момента записи временного штампа превышает
 // delay мс - возвращает 1, иначе 0.
// if((TimeStamp-time_stamp) > delay) return 1;
// else return 0;
 if((TimeStamp-time_stamp - delay) & 0x80000000 ) return 0;
 else return 1;
}
/* ================================================== */

unsigned long int time_snd[5]={0,0,0,0,0};
unsigned long int time_rcv[5]={0,0,0,0,0};


 char cb_COM[5][n_bufcom];
 int ghj=0;
 int n_bcom[5]={0,0,0,0,0};

void (*ff_serv)() = NULL;

int  (*ff_gets_com[5])(int ii) = {f_empty_gets,f_empty_gets,f_empty_gets,f_empty_gets,f_empty_gets};
void (*ff_answ_com[5])(int ii) = {f_empty,f_empty,f_empty,f_empty,f_empty};
void (*ff_answ_flt[5])(int ii) = {f_empty,f_empty,f_empty,f_empty,f_empty};
int i_empty;
void f_empty(int ii)
{
 i_empty=ii;
}
/* ---------------------------------------- */
/*================================================================*/
/*-----------------------------------------*/
long int time1=0,dd_time,dd_max=-1,dd_min=10000000;
void f_time_dd()
{
long int ltmp;
ltmp=TimeStamp;
dd_time=ltmp-time1;
time1=ltmp;
if(dd_time > dd_max) dd_max=dd_time;
else if(dd_time < dd_min) dd_min=dd_time;
}
//---
void f_print_dd()
{
 printf(" %ld , min= %ld, max=%ld",dd_time,dd_min,dd_max);
 dd_min=10000000;
 dd_max=-1;
}
/*-----------------------------------------*/
unsigned long int one_sec_time=0;

void f_one_sec()
{
union  { float f; char c[4]; } o;
  int win_sum;
  TIME_DATE t;

  if( f_timer(one_sec_time,1000L) == 0)  return;

  one_sec_time+=1000;

  GetTimeDate(&t);


  if(FlagWinSum)
   {
      if((t.sec <= 2)&& (t.minute==0))
        if((t.day >= 25)&&(t.weekday==0))
        {
          if( (t.month==3)&& (t.hour==2) )
          // в 2 часа ,псл Вск, mart,ч+=1
          {  /*переход на летнее время */

           win_sum = ReadNVRAM( nr_win_sum )  ;
           if(win_sum==0)
           { // еще не переведено
            f_wr_evt(evt_none);
            t.hour++;
            win_sum=1;  // 1 - лето
            SetTime((int)t.hour,(int)t.minute,(int)t.sec);

            WriteNVRAM((int)nr_win_sum,(int)win_sum );
            f_wr_evt(evt_ch_sum);
           }
          }
          else if((t.month==10)&& (t.hour==3) )
           // в 3 часа ,псл Вск, oct,ч-=1
          { //переход на зимнее время

            win_sum  = ReadNVRAM( nr_win_sum )  ;
            if(win_sum)
            { // еще не переведено
              f_wr_evt(evt_none);
              t.hour--;
              win_sum=0; // зимнее
              SetTime((int)t.hour,(int)t.minute,(int)t.sec);
              WriteNVRAM((int)nr_win_sum,(int)win_sum );
              f_wr_evt(evt_ch_win);
            }
          }
        }
   }
    WriteNVRAM((int)nr_year   ,(int)(t.year-2000));
    WriteNVRAM((int)nr_month  ,(int)t.month  );
    WriteNVRAM((int)nr_day    ,(int)t.day    );
    WriteNVRAM((int)nr_hour   ,(int)t.hour   );
    WriteNVRAM((int)nr_min    ,(int)t.minute );
    WriteNVRAM((int)nr_sec    ,(int)t.sec    );

    o.f=  s_DGM[0].VolLiq;
    WriteNVRAM((int) nr_v0   ,(int)o.c[0] );
    WriteNVRAM((int) nr_v1   ,(int)o.c[1] );
    WriteNVRAM((int) nr_v2   ,(int)o.c[2] );
    WriteNVRAM((int) nr_v3   ,(int)o.c[3] );
    o.f=  s_DGM[0].MassLiq;
    WriteNVRAM((int) nr_m0   ,(int)o.c[0] );
    WriteNVRAM((int) nr_m1   ,(int)o.c[1] );
    WriteNVRAM((int) nr_m2   ,(int)o.c[2] );
    WriteNVRAM((int) nr_m3   ,(int)o.c[3] );

}
/*-----------------------------------------*/
float cl_val_c=-100000;
float cl_t_c=-100000;
int num_clp_sv=1; // 1...3 номер клапана сохраняемого в NVRAM
                  // 1 - самый маленький

//#define CL_MassP  (*lst_Mass[num_clp_sv])
//#define CL_VolP    (*lst_Vol[num_clp_sv])

float CL_MassP=0,CL_VolP=0 ;

/*
void f_wr_cor()
{
float ftmp;
union  { float f; char c[4]; } o;

    ftmp=CL_MassP+CL_VolP;

    if( (cl_val_c != CL_MassP) || (cl_t_c != CL_VolP ) )
    {
       o.f= CL_MassP ;
       cl_val_c=o.f;
       WriteNVRAM((int) nr_dv0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_dv1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_dv2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_dv3   ,(int)o.c[3] );

       o.f= CL_VolP;
       cl_t_c=o.f;
       WriteNVRAM((int) nr_dt0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_dt1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_dt2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_dt3   ,(int)o.c[3] );

       o.f= ftmp;
       WriteNVRAM((int) nr_sm0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_sm1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_sm2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_sm3   ,(int)o.c[3] );
    }
}
*/
/*-----------------------------------------*/
/*
void f_rd_cor()
{
union  { float f; char c[4]; } o;
float ftmp,ftmp1,ftmp2,ftmp3;

 o.c[0]= ReadNVRAM( nr_dv0 );
 o.c[1]= ReadNVRAM( nr_dv1 );
 o.c[2]= ReadNVRAM( nr_dv2 );
 o.c[3]= ReadNVRAM( nr_dv3 );
 ftmp = o.f;
 o.c[0]= ReadNVRAM( nr_dt0 );
 o.c[1]= ReadNVRAM( nr_dt1 );
 o.c[2]= ReadNVRAM( nr_dt2 );
 o.c[3]= ReadNVRAM( nr_dt3 );
 ftmp1 = o.f;
 ftmp3=ftmp+ftmp1;
 o.c[0]= ReadNVRAM( nr_sm0 );
 o.c[1]= ReadNVRAM( nr_sm1 );
 o.c[2]= ReadNVRAM( nr_sm2 );
 o.c[3]= ReadNVRAM( nr_sm3 );
 ftmp2 = o.f;
 if(ftmp2== ftmp3)
 {
   CL_MassP=ftmp;
   CL_VolP=ftmp1;
 }
}
*/
/*-----------------------------------------*/

/*-----------------------------------------*/
int init_win_sum(int cor)
{
//  возвращает признак зимнего или летнего времени
//  для текущего момента: 1 - летнее время , 0 - зимнее время
//  если cor != 0 корректирует текущее время при переходе зим./летн.

  int year,month,day,hour,min,sec;
  int win_sum;
  time_t ltime1,ltime2;
  int prev_ws;

  struct tm *tdat;

 int i,i1;
 TIME_DATE t;
struct arch_str_t tim_0;

 f_prepare_t(&tim_0);
 GetTimeDate(&t);
/*
 tim_0.year  = t.year-2000;
 tim_0.month = t.month;
 tim_0.day   = t.day;
 tim_0.hour  = t.hour;
 tim_0.min   = t.minute;
 tim_0.sec   = t.sec;
*/

// printf("\n\r %02d.%02d.%02d  " ,tim_0.day,tim_0.month,tim_0.year);
// printf("%02d.%02d.%02d -- ",(int)tim_0.hour,(int)tim_0.min,(int)tim_0.sec);

 ltime1=f_get_time_t(&tim_0); // текущее время

  switch((int)tim_0.month)
  {
   case 11:
   case 12:
   case 1:
   case 2: // зима
         win_sum=0;
         break;
   case 4:
   case 5:
   case 6:
   case 7:
   case 8:
   case 9: // лето
         win_sum=1;
         break;
   case 3:  // март
        if(t.day>= 10) i=t.day-t.weekday;
        else i=t.day+7-t.weekday;
        for(;i<=31;i+=7)
         {
          i1=i;
         }
//        printf("\n\r==Last Sunday - %d Mart ",i1);
      // i1 - число последнего воскресенья марта
         tim_0.day=i1;
         tim_0.hour=2;
         tim_0.min=0;
         tim_0.sec=0;

    ltime2=f_get_time_t(&tim_0); // время Ч

    if(ltime1<ltime2)
         win_sum=0;
    else
         win_sum=1;
    break;
   case 10:  // октябрь
        if(t.day>= 10) i=t.day-t.weekday;
        else i=t.day+7-t.weekday;
        for(;i<=31;i+=7)
         {
          i1=i;
         }
      // i1 - число последнего воскресенья октября
         tim_0.day=i1;
         tim_0.hour=3;
         tim_0.min=0;
         tim_0.sec=0;
 //       printf("\n\r==Last Sunday - %d Oct ",i1);

// printf("\n\r %02d.%02d.%02d  " ,tim_0.day,tim_0.month,tim_0.year);
// printf("%02d.%02d.%02d -- ",(int)tim_0.hour,(int)tim_0.min,(int)tim_0.sec);

    ltime2=f_get_time_t(&tim_0); // время Ч
//    printf("\n\rltime1=%ld,ltime2=%ld",ltime1,ltime2);

    if( (ltime2>ltime1)&& ((ltime2 - ltime1)< 3600L))
    {
      win_sum=ReadNVRAM(nr_win_sum);
      break;
    }
    if(ltime1<ltime2)
    {
         win_sum=1;
    }
    else
    {
         win_sum=0;
    }
    break;
  }

  if(cor)
  {
    prev_ws=ReadNVRAM(nr_win_sum);
    if((prev_ws==0)&&(win_sum!=0))
    {// переход на летнее время
      ltime1+=3600L;

      tdat = localtime( &ltime1 );
      year   = tdat->tm_year+1900;
      month  = tdat->tm_mon+1;
      day    = tdat->tm_mday ;
      hour   = tdat->tm_hour;
      min    = tdat->tm_min;
      sec    = tdat->tm_sec;

      SetTime(hour, min,sec);
      SetDate(year, month, day);
      f_wr_evt(evt_ch_sum);
    }
    else if((prev_ws!=0)&&(win_sum==0))
    {// переход на зимнее время
      ltime1-=3600L;

      f_wr_evt(evt_ch_win);

      tdat = localtime( &ltime1 );
      year   = tdat->tm_year+1900;
      month  = tdat->tm_mon+1;
      day    = tdat->tm_mday ;
      hour   = tdat->tm_hour;
      min    = tdat->tm_min;
      sec    = tdat->tm_sec;
      SetTime(hour,min,sec);
      SetDate( year, month, day);
    }
  }
  WriteNVRAM((int)nr_win_sum,(int)win_sum );
  return win_sum;
}
/*-----------------------------------------*/
float D1_eee=-1,T1_eee=-100,D2_eee=-1,T2_eee=-100;
float D3_eee=-1,T3_eee=-100;

float D1_eee_c=-10,T1_eee_c=-1000,D2_eee_c=-10,T2_eee_c=-1000;
float D3_eee_c=-10,T3_eee_c=-1000;
/*-----------------------------------------*/
int f_rd_Dns()
{
union  { float f; char c[4]; } o;
float ftmp,ftmp1,ftmp2,ftmp3,ftmp4,ftmp5;//,ftmp6;

 o.c[0]= ReadNVRAM( nr_D10 );
 o.c[1]= ReadNVRAM( nr_D11 );
 o.c[2]= ReadNVRAM( nr_D12 );
 o.c[3]= ReadNVRAM( nr_D13 );
 ftmp = o.f;
 o.c[0]= ReadNVRAM( nr_T10 );
 o.c[1]= ReadNVRAM( nr_T11 );
 o.c[2]= ReadNVRAM( nr_T12 );
 o.c[3]= ReadNVRAM( nr_T13 );
 ftmp1 = o.f;

 o.c[0]= ReadNVRAM( nr_D20 );
 o.c[1]= ReadNVRAM( nr_D21 );
 o.c[2]= ReadNVRAM( nr_D22 );
 o.c[3]= ReadNVRAM( nr_D23 );
 ftmp2= o.f;
 o.c[0]= ReadNVRAM( nr_T20 );
 o.c[1]= ReadNVRAM( nr_T21 );
 o.c[2]= ReadNVRAM( nr_T22 );
 o.c[3]= ReadNVRAM( nr_T23 );
 ftmp3 = o.f;

 o.c[0]= ReadNVRAM( nr_D30 );
 o.c[1]= ReadNVRAM( nr_D31 );
 o.c[2]= ReadNVRAM( nr_D32 );
 o.c[3]= ReadNVRAM( nr_D33 );
 ftmp4= o.f;
 o.c[0]= ReadNVRAM( nr_T30 );
 o.c[1]= ReadNVRAM( nr_T31 );
 o.c[2]= ReadNVRAM( nr_T32 );
 o.c[3]= ReadNVRAM( nr_T33 );
 ftmp5 = o.f;

// ftmp6=ftmp+ftmp1+ftmp2+ftmp3+ftmp4+ftmp5;

 o.c[0]= ReadNVRAM( nr_sm0 );
 o.c[1]= ReadNVRAM( nr_sm1 );
 o.c[2]= ReadNVRAM( nr_sm2 );
 o.c[3]= ReadNVRAM( nr_sm3 );

// if(ftmp6 == o.f)
 {
   D1_eee  =ftmp;
   D1_eee_c=ftmp;
   T1_eee  =ftmp1;
   T1_eee_c=ftmp1;
   D2_eee  =ftmp2;
   D2_eee_c=ftmp2;
   T2_eee  =ftmp3;
   T2_eee_c=ftmp3;
   D3_eee  =ftmp4;
   D3_eee_c=ftmp4;
   T3_eee  =ftmp5;
   T3_eee_c=ftmp5;
   return 1;
 }


// return 0;
}
/*-----------------------------------------*/
void f_wr_Dns()
{
//float ftmp;
union  { float f; char c[4]; } o;
/*
if(
    ( D1_eee != D1_eee_c ) ||
    ( T1_eee != T1_eee_c ) ||
    ( D2_eee != D2_eee_c ) ||
    ( T2_eee != T2_eee_c )
    ( D3_eee != D3_eee_c ) ||
    ( T3_eee != T3_eee_c )
  )
*/
  {
//     ftmp=D1_eee+T1_eee+D2_eee+T2_eee+D3_eee+T3_eee;
//       ftmp=D1_eee+T1_eee+D2_eee+T2_eee+D3_eee+T3_eee;

//   f_slaves();
//  if( D1_eee != D1_eee_c )
    {
       o.f= D1_eee ;
       D1_eee_c=o.f;
       WriteNVRAM((int) nr_D10   ,(int)o.c[0] );
       WriteNVRAM((int) nr_D11   ,(int)o.c[1] );
       WriteNVRAM((int) nr_D12   ,(int)o.c[2] );
       WriteNVRAM((int) nr_D13   ,(int)o.c[3] );
    }
//  if( T1_eee != T1_eee_c )
    {
       o.f= T1_eee;
       T1_eee_c=o.f;
       WriteNVRAM((int) nr_T10   ,(int)o.c[0] );
       WriteNVRAM((int) nr_T11   ,(int)o.c[1] );
       WriteNVRAM((int) nr_T12   ,(int)o.c[2] );
       WriteNVRAM((int) nr_T13   ,(int)o.c[3] );
    }
//  if( D2_eee != D2_eee_c )
    {
       o.f= D2_eee ;
       D2_eee_c=o.f;
       WriteNVRAM((int) nr_D20   ,(int)o.c[0] );
       WriteNVRAM((int) nr_D21   ,(int)o.c[1] );
       WriteNVRAM((int) nr_D22   ,(int)o.c[2] );
       WriteNVRAM((int) nr_D23   ,(int)o.c[3] );
    }

//  if( T2_eee != T2_eee_c )
    {
       o.f= T2_eee;
       T2_eee_c=o.f;
       WriteNVRAM((int) nr_T20   ,(int)o.c[0] );
       WriteNVRAM((int) nr_T21   ,(int)o.c[1] );
       WriteNVRAM((int) nr_T22   ,(int)o.c[2] );
       WriteNVRAM((int) nr_T23   ,(int)o.c[3] );
    }

//  if( D3_eee != D3_eee_c )
    {
       o.f= D3_eee ;
       D3_eee_c=o.f;
       WriteNVRAM((int) nr_D30   ,(int)o.c[0] );
       WriteNVRAM((int) nr_D31   ,(int)o.c[1] );
       WriteNVRAM((int) nr_D32   ,(int)o.c[2] );
       WriteNVRAM((int) nr_D33   ,(int)o.c[3] );
    }

//  if( T3_eee != T3_eee_c )
    {
       o.f= T3_eee;
       T3_eee_c=o.f;
       WriteNVRAM((int) nr_T30   ,(int)o.c[0] );
       WriteNVRAM((int) nr_T31   ,(int)o.c[1] );
       WriteNVRAM((int) nr_T32   ,(int)o.c[2] );
       WriteNVRAM((int) nr_T33   ,(int)o.c[3] );
    }

 /*
       o.f= ftmp;
       WriteNVRAM((int) nr_sm0   ,(int)o.c[0] );
       WriteNVRAM((int) nr_sm1   ,(int)o.c[1] );
       WriteNVRAM((int) nr_sm2   ,(int)o.c[2] );
       WriteNVRAM((int) nr_sm3   ,(int)o.c[3] );

*/
  }
}
//-----------------------------------------
struct dis_set_MMI ds_list1[]=
{

//-----------
  list1_dsr,
  13,
  &flag_Slv,
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  31,
  &flag_Hst,
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  15,
  &Tank_nn,
  1.,
  16.,
  T_INT,
//-----------
  list1_dsr,
  32,
  &num_in1,
  0,
  KOL_INP,
  T_INT,
//----------------
  list1_dsr,
  33,
  &mski_inp1,
  0,
  1,
  T_INT,
/*----------------*/
  list1_dsr,
  44,
  &flag_en_er,
  0,
  BIG_P,
  T_INT,
//----------------

#if defined(DIGIMAG)
//-------------------
//-----------
  list1_dsr,
  1,             // Адрес уровнемер 1
  &DGM_ADDR[0],
  0,
  BIG_P,
  T_INT_L,
//-----------
  list1_dsr,
  2,
  &DGM_ADDR[1],
  0,
  BIG_P,
  T_INT_L,
//-----------

  list1_dsr,
  3,            // Адрес уровнемер 3
  &DGM_ADDR[2],
  0,
  BIG_P,
  T_INT_L,

//-----------
#endif
//-----------------------
#if defined(PMP201)
//-------------------
  list1_dsr,
  54,
  &Lvl_offs[0],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  list1_dsr,
  55,
  &Lvl_offs[1],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  list1_dsr,
  56,
  &Lvl_offs[2],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  list1_dsr,
  57,
  &Lvl_offs[3],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  list1_dsr,
  58,
  &Lvl_offs[4],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  list1_dsr,
  59,
  &Lvl_offs[5],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  list1_dsr,
  60,
  &Lvl_offs[6],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  list1_dsr,
  61,
  &Lvl_offs[7],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------
  list1_dsr,
  62,
  &Lvl_offs[8],
  -BIG_P,
  BIG_P,
  T_FLOAT,
//-------------------

  list1_dsr,
  1,            // Адрес уровнемер 1
  &PMP[0].addr,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  2,            // Адрес уровнемер 2
  &PMP[1].addr,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  3,            // Адрес уровнемер 3
  &PMP[2].addr,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  19,           // Адрес уровнемер 4
  &PMP[3].addr,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  20,           // Адрес уровнемер 5
  &PMP[4].addr,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  21,           // Адрес уровнемер 6
  &PMP[5].addr,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  22,           // Адрес уровнемер 7
  &PMP[6].addr,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  23,           // Адрес уровнемер 8
  &PMP[7].addr,
  0,
  BIG_P,
  T_INT,
//-----------
  list1_dsr,
  24,           // Адрес уровнемер 9
  &PMP[8].addr,
  0,
  BIG_P,
  T_INT,
//-----------
#endif

#if defined(DIGIMAG)
//-----------
  list1_dsr,
  7,
  &DGM[0].status,
  0.,
  1.,
  T_INT,
//-----------
  list1_dsr,
  8,
  &DGM[1].status,
  0.,
  1.,
  T_INT,
//-----------
/*
  list1_dsr,
  9,
  &DGM[2].status,
  0.,
  1.,
  T_INT,
*/
//-----------

#endif
//-----------------------
#if defined(PMP201)
  list1_dsr,
  7,
  &PMP[0].status,
  0.,
  2.,
  T_INT,
//-----------
  list1_dsr,
  8,
  &PMP[1].status,
  0.,
  2.,
  T_INT,
//-----------
  list1_dsr,
  9,
  &PMP[2].status,
  0.,
  2.,
  T_INT,
//-----------
  list1_dsr,
  25,
  &PMP[3].status,
  0.,
  2.,
  T_INT,
//-----------
  list1_dsr,
  26,
  &PMP[4].status,
  0.,
  2.,
  T_INT,
//-----------
  list1_dsr,
  27,
  &PMP[5].status,
  0.,
  2.,
  T_INT,
//-----------
  list1_dsr,
  28,
  &PMP[6].status,
  0.,
  2.,
  T_INT,
//-----------
  list1_dsr,
  29,
  &PMP[7].status,
  0.,
  2.,
  T_INT,
//-----------
  list1_dsr,
  30,
  &PMP[8].status,
  0.,
  2.,
  T_INT,
//-----------
//-----------
  list1_dsr,
  35,
  &PMP[0].port,
  1.,
  4.,
  T_INT,
//-----------
  list1_dsr,
  36,
  &PMP[1].port,
  1.,
  4.,
  T_INT,
//-----------
  list1_dsr,
  37,
  &PMP[2].port,
  1.,
  4.,
  T_INT,
//-----------
  list1_dsr,
  38,
  &PMP[3].port,
  1.,
  4.,
  T_INT,
//-----------
  list1_dsr,
  39,
  &PMP[4].port,
  1.,
  4.,
  T_INT,
//-----------
  list1_dsr,
  40,
  &PMP[5].port,
  1.,
  4.,
  T_INT,
//-----------
  list1_dsr,
  41,
  &PMP[6].port,
  1.,
  4.,
  T_INT,
//-----------
  list1_dsr,
  42,
  &PMP[7].port,
  1.,
  4.,
  T_INT,
//-----------
  list1_dsr,
  43,
  &PMP[8].port,
  1.,
  4.,
  T_INT,
//-----------

#endif
//-----------------------
//-- Номер выхода --
  list1_dsr,
  4,               // OUT MIN, N вых.
  &num_out1,
  0,
  16,
  T_INT,
//-----------
  list1_dsr,
  5,               // OUT MAX, N вых.
  &num_out2,
  0,
  16,
  T_INT,
//-----------
  list1_dsr,
  14,               // Инверсия выходов
  &fl_inv_out,
  0,
  1,
  T_INT,
//-----------

//-----------
  list1_dsr,
  10,
  &prc10[0],
  0,
  1.01,
  T_FLOAT,
//-----------
  list1_dsr,
  11,
  &prc90[0],
  0,
  1.01,
  T_FLOAT,
//-----------
//-----------
  list1_dsr,
  63,
  &prc10[1],
  0,
  1.01,
  T_FLOAT,
//-----------
  list1_dsr,
  64,
  &prc90[1],
  0,
  1.01,
  T_FLOAT,
//-----------
//-----------
  list1_dsr,
  65,
  &prc10[2],
  0,
  1.01,
  T_FLOAT,
//-----------
  list1_dsr,
  66,
  &prc90[2],
  0,
  1.01,
  T_FLOAT,
//-----------
//-----------
  list1_dsr,
  67,
  &prc10[3],
  0,
  1.01,
  T_FLOAT,
//-----------
  list1_dsr,
  68,
  &prc90[3],
  0,
  1.01,
  T_FLOAT,
//-----------
//-----------
  list1_dsr,
  69,
  &prc10[4],
  0,
  1.01,
  T_FLOAT,
//-----------
  list1_dsr,
  70,
  &prc90[4],
  0,
  1.01,
  T_FLOAT,
//-----------
//-----------
  list1_dsr,
  71,
  &prc10[5],
  0,
  1.01,
  T_FLOAT,
//-----------
  list1_dsr,
  72,
  &prc90[5],
  0,
  1.01,
  T_FLOAT,
//-----------
//-----------
  list1_dsr,
  73,
  &prc10[6],
  0,
  1.01,
  T_FLOAT,
//-----------
  list1_dsr,
  74,
  &prc90[6],
  0,
  1.01,
  T_FLOAT,
//-----------
//-----------
  list1_dsr,
  75,
  &prc10[7],
  0,
  1.01,
  T_FLOAT,
//-----------
  list1_dsr,
  76,
  &prc90[7],
  0,
  1.01,
  T_FLOAT,
//-----------
//-----------
  list1_dsr,
  77,
  &prc10[8],
  0,
  1.01,
  T_FLOAT,
//-----------
  list1_dsr,
  78,
  &prc90[8],
  0,
  1.01,
  T_FLOAT,
//-----------

  list1_dsr,
  45,               // Инверсия выходов
  &Fl_ch_mm[0],
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  46,               // Инверсия выходов
  &Fl_ch_mm[1],
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  47,               // Инверсия выходов
  &Fl_ch_mm[2],
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  48,               // Инверсия выходов
  &Fl_ch_mm[3],
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  49,               // Инверсия выходов
  &Fl_ch_mm[4],
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  50,               // Инверсия выходов
  &Fl_ch_mm[5],
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  51,               // Инверсия выходов
  &Fl_ch_mm[6],
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  52,               // Инверсия выходов
  &Fl_ch_mm[7],
  0,
  1,
  T_INT,
//-----------
  list1_dsr,
  53,               // Инверсия выходов
  &Fl_ch_mm[8],
  0,
  1,
  T_INT,
//-----------

/*
  list1_dsr,
  6,
  &I7060[0].status,
  0.,
  1.,
  T_INT,
//-----------

//-----------
  list1_dsr,
  16,
  &Sl3_time1,
  0,
  BIG_P,
  T_INT_L,
//-------------------
  list1_dsr,
  17,
  &Sl3_time2 ,
  0,
  BIG_P,
  T_INT_L,
//-------------------
  list1_dsr,
  18,
  &Sl3_time3 ,
  0,
  BIG_P,
  T_INT_L,
//-------------------
*/
//-----------
  list1_dsr,
  34,
  &Bot_time,
  0,
  BIG_P,
  T_INT_L,
//-----------

//-----------
  list1_dsr, // Переход летнее время
  12,
  &FlagWinSum,
  0,
  1,
  T_INT,
//-----------
//-----------
  NULL,
  0,
  NULL,
  0,
  1,
  T_INT,
//-----------

};
//-----------------------------------------
int max_list1=(sizeof(ds_list1)/sizeof(ds_list1[0])) -1;
//-----------------------------------------

long int Sl3_time1=0;
long int Sl3_time1_l=0;
long int t_store1=0;

long int Sl3_time2=0;
long int Sl3_time2_l=0;
long int t_store2=0;

long int Sl3_time3=0;
long int Sl3_time3_l=0;
long int t_store3=0;

void f_store_dt()
{

  if( Sl3_time1 >0 )
  {
   if( f_timer(t_store1,Sl3_time1_l ) )
    {
      t_store1=TimeStamp;
      f_wr_evt(evt_store1);
    }
  }
  if( Sl3_time2 >0 )
  {
   if( f_timer(t_store2,Sl3_time2_l ) )
    {
      t_store1=TimeStamp;
      f_wr_evt(evt_store2);
    }
  }
  if( Sl3_time3 >0 )
  {
   if( f_timer(t_store3,Sl3_time3_l ) )
    {
      t_store3=TimeStamp;
      f_wr_evt(evt_store3);
    }
  }

}

/* ---------------------------------------- */
//-----------------------------------------

// см.  device.h

//#include "mmi.c"
#if defined(MMI_ICP)
#include "MMI.c"
#endif

#if defined(MMI_NEW)
#include "MMI_new.c"
#endif

#include "i7060.c"
#include "icp.c"
#include "queue.c"
#include "avt_ctrl.c"
#include "mvd.c"
//#include "ref_t.c"

//-----------------------------------------
