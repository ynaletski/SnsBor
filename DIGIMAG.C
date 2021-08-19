
//  Ввод данных из уровнемера Digimag

// Подключение
//
// 1  +24V   красный
// 2   S+    коричневый
// 3   S-    синий
// 4  -24V   белый

// COM  9600 8 0 1
int State_dgm[max_DGM+1]={0,0,0,0};
long int DGM_ADDR[max_DGM]={11225,15040,15041};

#define DIGIMAG

struct s_DGM_dev s_DGM[max_DGM]=
{
 611.1,     //  float Prod_lvl; // mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 22.67,     // float Temp;     // C
 0,       // float VolLiq  ;
 0,       // float VolGas  ;
 0,       // float MassLiq ;
 0,       // float MassGas ;
 0,       // float Mass    ;
 0,       // float DensLiq ;
 0,       // float DensGas ;
 0,       // float DensReal;
 0,       // float TempReal;
//==========================
 622.2,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 25.2,       // float Temp;     // C
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
 633.3,    //  float Prod_lvl; //  mm
 0,       //  float offset;
 1.0,     //  float scale;
 0,       // float Wat_lvl;  // mm
 27.12,   // float Temp;     // C
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
struct s_icp_dev DGM[max_DGM]=
{
// device 1
0,                // status
2,                // port
1111,                // addr
"Digim/1",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
300,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&DGM_Rqst[0],   // *request
//--------------------------------
// device 2
0,                // status
2,                // port
2222,                // addr
"Digim/2",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
300,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&DGM_Rqst[1],   // *request
//--------------------------------
// device 3
0,                // status
2,                // port
3333,                // addr
"Digim/3",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
300,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&DGM_Rqst[2],   // *request
//--------------------------------
};
/*----------------------------*/
struct COM_rqst DGM_Rqst[max_DGM]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_DGM_rd,     //  answ_com
f_DGM_Flt,            //  answ_flt
"Digim/1",             //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&DGM[0],            //  *ICP_dd
1,                    //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_DGM_rd,     //  answ_com
f_DGM_Flt,            //  answ_flt
"Digim/2",             //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&DGM[1],            //  *ICP_dd
1,                //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
//--------------------------
// device 3
//--------------------------
0,                     //  состояние запроса
ffgets_com_ICP,        //  gets_com
f_DGM_rd,     //  answ_com
f_DGM_Flt,            //  answ_flt
"Digim/3",             //  name
2,                     //  n_dev номер устройства в группе однотипных устройств
&DGM[2],            //  *ICP_dd
1,                //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
};
/*----------------------------*/

/*----------------------------*/
void f_DGM(int ii)
{
  // ii - номер устройства
  // Функция шлет запрос на получение данных из устройства ICP DGMC
  //  ( 'M01') . Период посылки DGM[ii].pool_time мс.
  // В ответ будет передана информация .
  //M01 : 00001=0=+239=15528=1441=238

int i;
char buf_tmp[20];

 if(DGM[ii].status == 0) return;

 if( f_timer(DGM[ii].time_stamp_pool,DGM[ii].pool_time ))
 {
  if((DGM_Rqst[ii].status == Req_Flt) ||
     (DGM_Rqst[ii].status == Req_OK)) goto m1;
  else if( f_timer(DGM[ii].time_stamp_pool,(DGM[ii].pool_time+DGM[ii].pool_time) ))
           goto m1;
   return;
m1:
//         sprintf(buf_tmp,"M%05d\n",DGM[ii].addr);
           sprintf(buf_tmp,"M%05ld\n",DGM_ADDR[ii]);
           buf_tmp[6]=0xd;
           buf_tmp[7]=0;
           strcpy(DGM_Rqst[ii].Cmd,buf_tmp);
           DGM_Rqst[ii].cmd_lgth=7;
           DGM_Rqst[ii].time_stamp=TimeStamp;
           DGM[ii].time_stamp_pool=TimeStamp;
           DGM[ii].n_transaction++;
           f_queue_put(DGM[ii].port, &DGM_Rqst[ii]);

        /*
            if(flag_prn)
            {
             printf("\n\r place in queue\n\r");
             printf("\n\r !!! COM %d ,!!!%s!!! %2x !!! ",DGM[ii].port,buf_tmp,buf_tmp[6]);
             printf("\n\r !!! COM %d",DGM[ii].port);

             printf("\n\r %2x %2x %2x %2x %2x %2x %2x %2x",
             buf_tmp[0],buf_tmp[1],buf_tmp[2],buf_tmp[3],buf_tmp[4],buf_tmp[5],buf_tmp[6],buf_tmp[7]);

            }
          */
 }
}
/*----------------------------*/

unsigned int DGM_inp_err[max_DGM]={0,0};  // счетчик ошибок ввода

// char buf_tmp11[50]=      " 00001=0=+239=15528=1441=238";
// char buf_tmp11[50]=        "00001=0=+239=15528=1441=238";
char buf_tmp11[50]=        "00001=0=+238=15528=1441=237";
//char buf_tmp11[50]=        "00001=1=+000=00000=0000=194";

void f_DGM_rd(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные
//
//
 struct COM_rqst *request;
 int i,nn,itmp;
 long int ltmp1,ltmp2,ltmp3,ltmp4,ltmp5;

//   strcpy(cb_COM[ii],buf_tmp11);
//   n_bcom[ii]=strlen(cb_COM[ii]);

   request=COM_que[ii].request[COM_que[ii].empt_ptr];
   nn=request->n_dev; // номер устройства DGM (0,1..)

   if(nn < 0) nn=0;
   if(nn >= max_DGM ) nn=max_DGM-1;

   if( f_check_crc_DGM(cb_COM[ii],ii) )
   {
 //в ответ на M01
 // приходит: 00001=0=+239=15528=1441=238

            sscanf(cb_COM[ii],"%ld=%ld=%ld=%ld=%ld=%d",
                       &ltmp1,&ltmp2,&ltmp3,&ltmp4,&ltmp5,&itmp);
            State_dgm[nn]=ltmp2;
            s_DGM[nn].Temp     = (float)(ltmp3) * 0.1;
            s_DGM[nn].Prod_lvl = (float)(ltmp4) * 0.1;
            s_DGM[nn].Wat_lvl  = ltmp5;

            f_count_tank(nn);

/*
if(flag_prn)
{
          printf( "\r\n Prod_lvl=%f,Wat_lvl=%f,Temp=%f ",
               s_DGM[nn].Prod_lvl,s_DGM[nn].Wat_lvl ,s_DGM[nn].Temp );
          flag_prn=0;
}
*/
    DGM[nn].n_success++;
    DGM_inp_err[nn]=0;
   }
   else
   {
  //     printf("\n\r CRC error");
         DGM_inp_err[nn]++;
         if(DGM_inp_err[nn]>=DGM_max_inp_err)
         {
           DGM_inp_err[nn]=DGM_max_inp_err;
           f_icp_error(&DGM[nn],RD_ERR );
         }
    DGM[nn].n_CRC_error++;
   }

   f_queue_free(ii,&DGM_Rqst[nn]);
}
/*-------------------------------------------*/
void f_DGM_Flt(int ii)
{
 struct COM_rqst *request;
 int nn;

 int lgth,i;
 char *buf_ttmp;

 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 nn=request->n_dev; // номер устройства DGM (0,1..)
 DGM[nn].time_stamp_rcv=0;
 DGM[nn].n_timeout_error++;
 DGM_inp_err[nn]++;

 if(DGM_inp_err[nn]>=DGM_max_inp_err)
 {
   DGM_inp_err[nn]=DGM_max_inp_err;
   f_icp_error(&DGM[nn],RD_ERR );
 }
// f_queue_free(ii,&DGM_Rqst[nn]);
 f_queue_Flt(ii);

}
/* ---------------------------------------- */
int f_check_crc_DGM(char *buf,int ii)
{ //return 1 - OK

int lgth,i,i1,i_ret;
unsigned int summ,summ1,summ2,summ3;

 char *buf_ttmp;

 //в ответ на M01
 // приходит:      00001=0=+239=15528=1441=238

           lgth=n_bcom[ii];
           for(i=lgth;i>0;i--)
            if(buf[i] == '=')
            {
               i1=i+1;
               break;
            }
 //        summ=4;
           summ=0;

           for(i=0;i<i1;i++)
           {
            summ+= (buf[i]) & 0xff;
           }

           summ2= (summ & 0xff)+  ((summ>>8) & 0xff);
           summ3= (summ2 & 0xff)+  ((summ2>>8) & 0xff);


          sscanf(&buf[i1],"%d",&summ1);
          i_ret=0;
          if( (summ3 & 0xff) == summ1) i_ret=1;

//if((flag_prn) |   (i_ret != 1))

/*============
if(flag_prn)
 {
       buf_ttmp=buf;
//     lgth=n_bcom[ii];
       buf_ttmp[lgth]=0;

       printf("\r\n lgth=%d\r\n",lgth);
       for(i=0;i<lgth;i++)
       {
         printf("%2x ", ((int)buf_ttmp[i])& 0xff  );
         if( (i%16)== 7) printf("  ");
         if( (i%16)== 15) printf("\r\n");
       }
       printf("\r\n%s\r\n",buf_ttmp);

       printf( "lgth=%d,i1=%d,buf[i1]=%c, CRC = %d %d %d %d ",lgth,i1,buf[i1],(summ & 0xff),summ2,summ3,summ1) ;

//     flag_prn=0;
 }

=======*/
       return i_ret;
  //        return 1;
}
/* ---------------------------------------- */
