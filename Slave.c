//29.04.2021 YN -\\//-
int waterTank = 0;
//              -//\\-


int TankN=0;
void (*ff_Slv)() = NULL;
int CMD_REG=0;
int flag_Slv=0;

int n_res_a=2;
int n_GRK_a=1;

int ComPortSlv = 2;
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
long int WD_PMP_Per=20000; // ����稭� ⠩���� �� �ਥ�� ᮮ�饭��

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


// ��� ����� � ������ ����஫��� ���祭�� ����७��� ���⭮��
// ����室��� ������� ����� � ᮮ⢥�����騥 ��६���� float
//A=1008   ����� ���祭�� ����.1
//  1010   ����� ���祭�� ⥬�.1
//  1012   ����� ���祭�� ����.2
//  1014   ����� ���祭�� ⥬�.2
// ��⥬ � ��६����� Int � ���ᮬ A=16 (Int(16)) ��⠭����� ��⪮��஢���� 䫠� �����.
// D0=1 - ������஢��� ����� १���� 1
// D1=1 - ������஢��� ����� १���� 2
// ��᫥ �ᯥ譮� ��।�� ������ ���� Int(16) ���㫨���
//

void *list_Slv_int[]={

&n_sl_int1,      // I0  �����.��६ int
&n_sl_flt1,      // I1  �����.��६ float
&n_res_a,  // I2   ������⢮ १���஢ � ��⥬� 1...9
&Slv_int[1],     // I3  �����
&sw_ver[0],      // I4  ����� ��, 6 ASCII ᨬ�����
&sw_ver[2],      // I5
&sw_ver[4],      // I6

&ICP_error[0],   // I7
&ICP_error[1],   // I8
&ICP_error[2],   // I9
&ICP_error[3],   // I10
&ICP_error[4],   // I11
&ICP_error[5],   // I12
&ICP_error[6],   // I13
&ICP_error[7],   // I14
&ICP_error[8],   // I15
&ICP_error[12],   // I16

//29.04.2021 YN -\\//-
&waterTank,     //  I17 ���������� १���� ���਩��� �஫����
//              -//\\-

NULL

};

int n_sl_int=(sizeof(list_Slv_int) / sizeof(list_Slv_int[0])) - 1 ;
int n_sl_int1=(sizeof(list_Slv_int) / sizeof(list_Slv_int[0])) - 1 ;

float *list_Slv_flt[]={

&s_DGM[0].Prod_lvl, // F1000   �஢��� �த�� � १���� N1,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[0].Wat_lvl,  // F1002   �஢��� ���� � १���� N1,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[0].Temp    , // F1004   ��������� �த�� � १���� N1, C
&s_DGM[0].DensLiq  ,// F1006   ���⭮��� �த�� � १���� N1,��/�3
&s_DGM[0].VolLiq  , // F1008   ��ꥬ �த�� � १���� N1,�
&s_DGM[0].VolWat,   // F1010   ��ꥬ ���� � १���� N1,�
&s_DGM[0].VolDif,   // F1012   ����� ��ꥬ �த�� � १���� N1,�
&s_DGM[0].MassLiq,  // F1014   ���� ��⮣� �த�� � १���� N1,�


&s_DGM[1].Prod_lvl, // F1016   �஢��� �த�� � १���� N2,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[1].Wat_lvl,  // F1018   �஢��� ���� � १���� N2,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[1].Temp    , // F1020   ��������� �த�� � १���� N2, C
&s_DGM[1].DensLiq  ,// F1022   ���⭮��� �த�� � १���� N2,��/�3
&s_DGM[1].VolLiq  , // F1024   ��ꥬ �த�� � १���� N2,�
&s_DGM[1].VolWat,   // F1026   ��ꥬ ���� � १���� N2,�
&s_DGM[1].VolDif,   // F1028   ����� ��ꥬ �த�� � १���� N2,�
&s_DGM[1].MassLiq,  // F1030   ���� ��⮣� �த�� � १���� N2,�


&s_DGM[2].Prod_lvl, // F1032   �஢��� �த�� � १���� N3,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[2].Wat_lvl,  // F1034   �஢��� ���� � १���� N3,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[2].Temp    , // F1036   ��������� �த�� � १���� N3, C
&s_DGM[2].DensLiq  ,// F1038   ���⭮��� �த�� � १���� N3,��/�3
&s_DGM[2].VolLiq  , // F1040   ��ꥬ �த�� � १���� N3,�
&s_DGM[2].VolWat,   // F1042   ��ꥬ ���� � १���� N3,�
&s_DGM[2].VolDif,   // F1044   ����� ��ꥬ �த�� � १���� N3,�
&s_DGM[2].MassLiq,  // F1046   ���� ��⮣� �த�� � १���� N3,�


&s_DGM[3].Prod_lvl, // F1048   �஢��� �த�� � १���� N4,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[3].Wat_lvl,  // F1050   �஢��� ���� � १���� N4,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[3].Temp    , // F1052   ��������� �த�� � १���� N4, C
&s_DGM[3].DensLiq  ,// F1054   ���⭮��� �த�� � १���� N4,��/�3
&s_DGM[3].VolLiq  , // F1056   ��ꥬ �த�� � १���� N4,�
&s_DGM[3].VolWat,   // F1058   ��ꥬ ���� � १���� N4,�
&s_DGM[3].VolDif,   // F1060   ����� ��ꥬ �த�� � १���� N4,�
&s_DGM[3].MassLiq,  // F1062   ���� ��⮣� �த�� � १���� N4,�


&s_DGM[4].Prod_lvl, // F1064   �஢��� �த�� � १���� N5,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[4].Wat_lvl,  // F1066   �஢��� ���� � १���� N5,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[4].Temp    , // F1068   ��������� �த�� � १���� N5, C
&s_DGM[4].DensLiq  ,// F1070   ���⭮��� �த�� � १���� N5,��/�3
&s_DGM[4].VolLiq  , // F1072   ��ꥬ �த�� � १���� N5,�
&s_DGM[4].VolWat,   // F1074   ��ꥬ ���� � १���� N5,�
&s_DGM[4].VolDif,   // F1076   ����� ��ꥬ �த�� � १���� N5,�
&s_DGM[4].MassLiq,  // F1078   ���� ��⮣� �த�� � १���� N5,�


&s_DGM[5].Prod_lvl, // F1080   �஢��� �த�� � १���� N6,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[5].Wat_lvl,  // F1082   �஢��� ���� � १���� N6,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[5].Temp    , // F1084   ��������� �த�� � १���� N6, C
&s_DGM[5].DensLiq  ,// F1086   ���⭮��� �த�� � १���� N6,��/�3
&s_DGM[5].VolLiq  , // F1088   ��ꥬ �த�� � १���� N6,�
&s_DGM[5].VolWat,   // F1090   ��ꥬ ���� � १���� N6,�
&s_DGM[5].VolDif,   // F1092   ����� ��ꥬ �த�� � १���� N6,�
&s_DGM[5].MassLiq,  // F1094   ���� ��⮣� �த�� � १���� N6,�


&s_DGM[6].Prod_lvl, // F1096   �஢��� �த�� � १���� N7,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[6].Wat_lvl,  // F1098   �஢��� ���� � १���� N7,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[6].Temp    , // F1100   ��������� �த�� � १���� N7, C
&s_DGM[6].DensLiq  ,// F1102   ���⭮��� �த�� � १���� N7,��/�3
&s_DGM[6].VolLiq  , // F1104   ��ꥬ �த�� � १���� N7,�
&s_DGM[6].VolWat,   // F1106   ��ꥬ ���� � १���� N7,�
&s_DGM[6].VolDif,   // F1108   ����� ��ꥬ �த�� � १���� N7,�
&s_DGM[6].MassLiq,  // F1110   ���� ��⮣� �த�� � १���� N7,�


&s_DGM[7].Prod_lvl, // F1112   �஢��� �த�� � १���� N8,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[7].Wat_lvl,  // F1114   �஢��� ���� � १���� N8,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[7].Temp    , // F1116   ��������� �த�� � १���� N8, C
&s_DGM[7].DensLiq  ,// F1118   ���⭮��� �த�� � १���� N8,��/�3
&s_DGM[7].VolLiq  , // F1120   ��ꥬ �த�� � १���� N8,�
&s_DGM[7].VolWat,   // F1122   ��ꥬ ���� � १���� N8,�
&s_DGM[7].VolDif,   // F1124   ����� ��ꥬ �த�� � १���� N8,�
&s_DGM[7].MassLiq,  // F1126   ���� ��⮣� �த�� � १���� N8,�


&s_DGM[8].Prod_lvl, // F1128   �஢��� �த�� � १���� N9,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[8].Wat_lvl,  // F1130   �஢��� ���� � १���� N9,�� (��ࢨ�� ����� �� �஢�����)
&s_DGM[8].Temp    , // F1132   ��������� �த�� � १���� N9, C
&s_DGM[8].DensLiq  ,// F1134   ���⭮��� �த�� � १���� N9,��/�3
&s_DGM[8].VolLiq  , // F1136   ��ꥬ �த�� � १���� N9,�
&s_DGM[8].VolWat,   // F1138   ��ꥬ ���� � १���� N9,�
&s_DGM[8].VolDif,   // F1140   ����� ��ꥬ �த�� � १���� N9,�
&s_DGM[8].MassLiq,  // F1142   ���� ��⮣� �த�� � १���� N9,�




/*
&Vol_max[0],        // F1094  47
&Vol_10[0],         // F1096  48
&Vol_90[0],         // F1098  49
*/

NULL

};
long int Com_Baud_Slave=9600;
long int long_slv   =0;
long int *list_Slv_lng[]={
                    // Addr = 1180
&Com_Baud_Slave,    //  90  ����� ������ �� Modbus
&long_slv,          //  91  �����

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

unsigned long ComBaudSlv =19200L;
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
/*
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

*/
     }
}
//-------------------------------------------

void f_SlaveRTU()
{
int itmp;
//  int  hour,min,sec;
// ������ ������� �� Host (PC) � ��������� �� �� ��⮪��� Modbus RTU

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
    if( ( TCT_T4 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT4");
      goto m1;
    }
    if( ( TCT_T5 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT5");
      goto m1;
    }
    if( ( TCT_T6 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT6");
      goto m1;
    }
    if( ( TCT_T7 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT7");
      goto m1;
    }
    if( ( TCT_T8 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT8");
      goto m1;
    }
    if( ( TCT_T9 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT9");
      goto m1;
    }

    TCT_ARR[0]=&TCT_T1[0];
    TCT_ARR[1]=&TCT_T2[0];
    TCT_ARR[2]=&TCT_T3[0];
    TCT_ARR[3]=&TCT_T4[0];
    TCT_ARR[4]=&TCT_T5[0];
    TCT_ARR[5]=&TCT_T6[0];
    TCT_ARR[6]=&TCT_T7[0];
    TCT_ARR[7]=&TCT_T8[0];
    TCT_ARR[8]=&TCT_T9[0];

}
/*-------------------------------------------*/
void f_free_SlvBuf()
{
  free( bf_Slv );
  free( TCT_T1 );
  free( TCT_T2 );
  free( TCT_T3 );
  free( TCT_T4 );
  free( TCT_T5 );
  free( TCT_T6 );
  free( TCT_T7 );
  free( TCT_T8 );
  free( TCT_T9 );
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
// ������⢮ ���� � ������� Host

/* =============
// �㭪�� ,  ����� � �����.�㬬.
  1,8, // �⥭�� Coil
  3,8, // �⥭�� Int
  5,8, // ������ Coil
  6,8, // ������ Int
  16,NN_fict, // ������ ��᪮�쪨� ���祭��
=============== */

int n_max_rcv=NN_fict;

//-------------------------------------------
int ffgets_SlvRTU(int ComPort)
{

//  �㭪�� �ਭ����� ᨬ���� �� ���� COM ComPort  � ���� cb_COM[ComPort].
//  �� ⠩���� ������� ��⠥��� ��������� � �㭪�� �����頥�
// ���祭�� 1.
//

/* ComPort - ����� COM   1...4 */
 char tmp;
 int tmp1;
 int i;
m1:
     if(IsCom(ComPort)==0)
     {
         if( WD_PMP_Per !=0 )
           if( f_timer(WD_PMP_time,WD_PMP_Per ))
           { // ��� �ᯥ譮�� �ਥ��

            if(WD_PMP_flag != 0)
            {
              WD_PMP_flag=0;
              f_icp_errS(WD_host);
            }
            WD_PMP_time=TimeStamp;
           }

           if( f_timer(WD_PMP_t1,WD_PMP_P1 ))
           { // ��� �ᯥ譮�� �ਥ��
            WD_PMP_Fl=0;
            WD_PMP_t1=TimeStamp;
           }

        if( n_bcom[ComPort] != 0)
         if( f_timer(time_snd[ComPort],byte_timout_slv))
         { // ��⥪ ⠩���� �ਥ�� �⢥� �� �������
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
{// �� ᮤ�ন���� cb_COM[ComPort][] � ������⢮� ���� n_bcom[ComPort]
 // ������ ����� ���뫪� RTU. �᫨ ����� �� ��।����� - �����頥� NN_fict
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
{// �� ᮤ�ন���� cb_COM[ComPort][] � ������⢮� ���� n_bcom[ComPort]
 // ������ ����� ���뫪� RTU. �᫨ ����� �� ��।����� - �����頥� NN_fict

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


  // �ਭ��� ������� �஢�७� �� ����஫��� �㬬� � ��室���� � intrpr.bfcom
  //  (��� ����஫쭮� �㬬�)

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
  {   // �⥭�� Int
    Slv_addrO=f_get_intS(&intrpr.bfcom[2]);
    Slv_nmbO=f_get_intS(&intrpr.bfcom[4]);

    if(Slv_nmbO>125)
        goto m_err_addr;

    if( f_is_int(Slv_addrO) &&  f_is_int(Slv_addrO+Slv_nmbO-1) )
    { // ����襭� ����� int

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
     { // ����襭� ����� float
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
  {   // ������ Int
    Slv_addrO=f_get_intS(&intrpr.bfcom[2]);
    if( f_is_int(Slv_addrO) )
    { // ������ ������ int
      *(int *)list_Slv_int[Slv_addrO]=f_get_intS(&intrpr.bfcom[4]);
      if((exc=f_int_fnc(Slv_addrO)) != 0)
         goto m_err;
      i1=6;
      goto out_b;
    }
    else goto m_err_addr;
  }
  if(fn_cod == 16 )
  {   // ������ ��᪮�쪨� ���祭��
    Slv_addrO=f_get_intS(&intrpr.bfcom[2]);
    Slv_nmbO=f_get_intS(&intrpr.bfcom[4]);
    if( f_is_int(Slv_addrO) &&  f_is_int(Slv_addrO+Slv_nmbO-1) )
    { //   ����� int

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
     { //  ����� float
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
m_err_fn  : // �㭪�� �� �ᯮ�����
 exc=01;
 goto m_err;

m_err_addr: // �������⨬� ����
 exc=02;
 goto m_err;

m_err_val : // �������⨬� �����
 exc=03;
 goto m_err;

m_err_slv : // �訡�� � Slave
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

// fl_slv_out=i2;
 fl_slv_out=0;
 ToComBufn(ComPortSlv,buf_tmpSR,i2);
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

// fl_slv_out=i2;
 fl_slv_out=0;
 ToComBufn(ComPortSlv,buf_tmpSR,i2);

 if(ff_Slv != NULL)
 {
  (*ff_Slv)(); // ���� �㭪樨
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
int ii_ret;
int  f_int_fnc(int Addr)
{ // return 1 - error, 0 - OK
 int i_ret=0;
 ii_ret=Addr;

 /*
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


        case 8: // ���⢥ত���� �⥭�� ������ १���� �ਥ��

         if((Reg_State & RCV_fin) == 0)
            goto m_err;
         else
            goto m_ok;

        case 9 : // ���⢥ত���� �⥭�� ������ १���� ���.���⭮�� � १���� 1
         if(TankN != 1)    goto m_err;
         if((Reg_State & DEN_fin) == 0) goto m_err;
         D1_eee = s_MVD[0].Dens;
         T1_eee = s_MVD[0].Temp ;
         f_wr_Dns();
         goto m_ok;

        case 10: // ���⢥ত���� �⥭�� ������ १���� ���.���⭮�� � १���� 2
         if(TankN != 2)  goto m_err;
         if((Reg_State & DEN_fin) == 0) goto m_err;
         D2_eee = s_MVD[0].Dens;
         T2_eee = s_MVD[0].Temp;
         f_wr_Dns();
         goto m_ok;

        case 11: // �⬥�� �⥭�� ������ १���� ���.���⭮�� � १���� 1,2

         if((Reg_State & (DEN_fin | RCV_fin) ) == 0)
            goto m_err;
         else
            goto m_ok;

        case 12: // ���⢥ত���� �⥭�� ������ १���� ���.���⭮�� � १���� 3
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

      case 21: // ����� ����७�� ���⭮�� � १���� 1
        TankN=1;
        goto mm1;
      case 22: // ����� ����७�� ���⭮�� � १���� 2
        TankN=2;
        goto mm1;
      case 23: // ����� ����७�� ���⭮�� � १���� 2
        TankN=3;
        goto mm1;
      case CMD_DIS_PMP:  // (0x100)  ����� ���� ���᪠ �१ �������
      case 0:      // ࠧ�襭�� ���᪠ �१ ������� � ����祭�� �ਥ��,���.����. �� ���
      case 0x1000: // ࠧ�襭�� ���᪠ �१ �������,����� ����祭�� �ਥ��,���.����. �� ���
mm1:
         if(Reg_State != 0)
            goto m_err;
        m_ok:
             CMD_IN= CMD_REG;
             CMD_OUT =CMD_REG | CMD_CONT ;
             i_ret=0;
             break;

      case 41: // ������ ���祭�� ���⭮�� � १���� 1
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

      case 42: // ������ ���祭�� ���⭮�� � १���� 2
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

      case 43: // ������ ���祭�� ���⭮�� � १���� 1,2
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
 */
 return i_ret;
}
/*-------------------------------------------*/
int f_fls()
{ // return 1 - error, 0 - OK
 flag_Slv=0;

 if(ComPortSlv == ComPortHost)
 {
  flag_Hst=1;
  RestoreCom(ComPortHost);
  InstallCom(ComPortHost,115200L,8,0,1);
  ShowCom(ComPortHost,8,115200L);
 }
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
  // ����
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