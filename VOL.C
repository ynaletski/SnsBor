
//  Сталь 09Г2С , 11.4 e-6 при 100C, 12.2 e-6 при 200C

struct s_TCT TCT_sz;
struct s_TCT *TCT_T1=NULL;
struct s_TCT *TCT_T2=NULL;
struct s_TCT *TCT_T3=NULL;
struct s_TCT *TCT_T4=NULL;
struct s_TCT *TCT_T5=NULL;
struct s_TCT *TCT_T6=NULL;
struct s_TCT *TCT_T7=NULL;
struct s_TCT *TCT_T8=NULL;
struct s_TCT *TCT_T9=NULL;

float  proc10 = 0.100;
float  proc90 = 0.850;

float  prc10[max_nn]={0.10,0.10,0.10,0.10,0.10,0.10,0.10,0.10,0.10};
float  prc90[max_nn]={0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85,0.85};


//int  num_out1 = 1, num_out2 = 2;
int  num_out1 = 9, num_out2 = 10; // I7188XA Out1,Out2

struct s_TCT *TCT_ARR[max_nn]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
int n_TCT[max_nn]={10,10,10,10,10,10,10,10,10};

//float alpha_LVL[max_nn]  = {11.4 * 0.001 * 0.001,11.4 * 0.001 * 0.001,11.4 * 0.001 * 0.001}; // коэффициент линейного расширения
//float alpha_Tank[max_nn] = {11.4 * 0.001 * 0.001,11.4 * 0.001 * 0.001,11.4 * 0.001 * 0.001}; // коэффициент линейного расширения

float alpha_LVL[max_nn]  = {0., 0.,0., 0.,0., 0.,0., 0.,0.}; // коэффициент линейного расширения
float alpha_Tank[max_nn] = {0., 0.,0., 0.,0., 0.,0., 0.,0.}; // коэффициент линейного расширения

float T0_LVL[max_nn] = {20,20,20,20,20,20,20,20,20};           // температура калибровки
float T0_Tank[max_nn]= {20,20,20,20,20,20,20,20,20};           // температура калибровки
float H_Cyl_M[max_nn]={25000.,25000.,25000.,25000.,25000.,25000.,25000.,25000.,25000.}; // максимальное значение уровня


//#define D_R (1.0)


float f_get_vol(int ii,float Lvl)
{
//  по значению уровня возвращает значение текущего объема жидкой фазы с учетом
//  теплового расширения оболочки резервуара
//  ii - индекс резервуара

int nn,nn1,nn2,nn3;
struct s_TCT *tct_uk,*tct_ukc;

 float Level;
 float vol;
 float dt_Tank;

 Level=Lvl;
 if((ii < 0)|| (ii>= max_nn) )
 {
   return 0.;
 }
 tct_uk=TCT_ARR[ii];

// Level = s_DGM[ii].Prod_lvl*s_DGM[ii].scale+s_DGM[ii].offset;

/*
 dt_Tank = s_DGM[ii].Temp - T0_Tank[ii];

 if( (alpha_LVL[ii] == alpha_Tank[ii] ) && (T0_LVL[ii]==T0_Tank[ii]));
 else
  {

      Level *=  ( 1.0 + alpha_LVL[ii]  * (s_DGM[ii].Temp  - T0_LVL[ii])) /
                ( 1.0 + alpha_Tank[ii] * dt_Tank) ;
  }

*/


 if( Level < 0.)
           Level=0.;

// nn=n_TCT[ii]-2; // количество записей qqqq
 nn=n_TCT[ii]-1; // количество записей

 if(nn <= 0) return 0.;

 if( Level > tct_uk[nn].level )
           Level=tct_uk[nn].level;

         nn1=0;

 for(nn2=max_ord;nn2>0;nn2=nn2>>1)
 {
  nn3=nn1+nn2;
  if(nn3> nn) continue;

  if(tct_uk[nn3].level > Level);
  else nn1=nn3;
 }
 // в nn1 индекс записи с  TCT_Tx[nn1].level <= Level ;

  if(nn1 >= nn)
    nn1=nn-1;

  tct_ukc=&tct_uk[nn1];

  if(tct_ukc[1].level == tct_ukc[0].level)
   vol= tct_ukc[1].volume;
  else
   vol = (tct_ukc[0].volume + (Level - tct_ukc[0].level) * (tct_ukc[1].volume - tct_ukc[0].volume) /(tct_ukc[1].level - tct_ukc[0].level)) ;


//  vol *= ( 1.0 + 3.0 * alpha_Tank[ii] * dt_Tank) ;

 return vol;
}
//------------------------------
// ----------------------------------------

int  f_wr_tct()
{// записывает 3 TCT во флэш

unsigned int seg;
unsigned int i,i1,ii;
long int i2,i3;
int ret_val;
char *data;

 ret_val=0;
 seg=seg_TCT;
 if( FlashErase(seg) != 0) return 1;

 data=(char *)&n_TCT[0];
 for(i=0;i<2;i++,data++)
   if (FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 data=(char *)&TCT_T1[0];
 i2=sizeof(TCT_sz) * max_TCT;
 for(i=4,i1=0;i1<i2;i++,i1++,data++)
 {
   if(FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 }


 data=(char *)&n_TCT[1];
 for(i=0,ii=0x4000;i<2;i++,data++,ii++)
   if (FlashWrite(seg, ii, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 data=(char *)&TCT_T2[0];
// i2=sizeof(TCT_sz) * max_TCT;
 for(i=0x4004,i1=0;i1<i2;i++,i1++,data++)
 {
   if(FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 }

 data=(char *)&n_TCT[2];
 for(i=0,ii=0x8000;i<2;i++,data++,ii++)
   if (FlashWrite(seg, ii, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 data=(char *)&TCT_T3[0];
// i2=sizeof(TCT_sz) * max_TCT;
 for(i=0x8004,i1=0;i1<i2;i++,i1++,data++)
 {
   if(FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 }


 data=(char *)&n_TCT[3];
 for(i=0,ii=0xC000;i<2;i++,data++,ii++)
   if (FlashWrite(seg, ii, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 data=(char *)&TCT_T4[0];
// i2=sizeof(TCT_sz) * max_TCT;
 for(i=0xC004,i1=0;i1<i2;i++,i1++,data++)
 {
   if(FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 }

seg=seg_TCT2;

 if( FlashErase(seg) != 0) return 1;

 data=(char *)&n_TCT[4];
 for(i=0;i<2;i++,data++)
   if (FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 data=(char *)&TCT_T5[0];
 i2=sizeof(TCT_sz) * max_TCT;
 for(i=4,i1=0;i1<i2;i++,i1++,data++)
 {
   if(FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 }

 data=(char *)&n_TCT[5];
 for(i=0,ii=0x4000;i<2;i++,data++,ii++)
   if (FlashWrite(seg, ii, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 data=(char *)&TCT_T6[0];
// i2=sizeof(TCT_sz) * max_TCT;
 for(i=0x4004,i1=0;i1<i2;i++,i1++,data++)
 {
   if(FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 }

 data=(char *)&n_TCT[6];
 for(i=0,ii=0x8000;i<2;i++,data++,ii++)
   if (FlashWrite(seg, ii, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 data=(char *)&TCT_T7[0];
// i2=sizeof(TCT_sz) * max_TCT;
 for(i=0x8004,i1=0;i1<i2;i++,i1++,data++)
 {
   if(FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 }


 data=(char *)&n_TCT[7];
 for(i=0,ii=0xC000;i<2;i++,data++,ii++)
   if (FlashWrite(seg, ii, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }

 data=(char *)&TCT_T8[0];
 i3=sizeof(TCT_sz) * max_TCT /2;
 i2=sizeof(TCT_sz) * n_TCT[7];

 if(i2 > i3) i2=i3;

 for(i=0xC004,i1=0;i1<i2;i++,i1++,data++)
 {
   if(FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 }


 data=(char *)&n_TCT[8];
 for(i=0,ii=0xE000;i<2;i++,data++,ii++)
   if (FlashWrite(seg, ii, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 data=(char *)&TCT_T9[0];
// i2=sizeof(TCT_sz) * max_TCT/2;

 i2=sizeof(TCT_sz) * n_TCT[8];
 if(i2 > i3) i2=i3;

 for(i=0xE004,i1=0;i1<i2;i++,i1++,data++)
 {
   if(FlashWrite(seg, i, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 }

 return ret_val;
}

//----------------------------------
void  f_zer_tct()
{
int ii;
 for(ii=0;ii< max_nn; ii++)
 {
   if(TCT_ARR[ii]==NULL) return;

     n_TCT[ii]=2;
     TCT_ARR[ii][0].level=0.;
     TCT_ARR[ii][0].volume=0.;
     TCT_ARR[ii][1].level=3000.;
     TCT_ARR[ii][1].volume=3000.;

 }
}

//----------------------------------
unsigned int Beg_tct[max_nn] = {       0,  0x4000,  0x8000,   0xc000,        0,  0x4000,  0x8000,   0xc000,   0xe000  };
unsigned int Sect_tct[max_nn]= { seg_TCT, seg_TCT, seg_TCT,  seg_TCT, seg_TCT2,seg_TCT2,seg_TCT2, seg_TCT2, seg_TCT2  };

void  f_rd_tct()
{// считывает 3 TCT из флэш

unsigned int seg;
int *i_ptr,i,ii;
struct s_TCT *TCT_ptr;


 for(ii=0;ii< Tank_nn; ii++)
 {

   if(TCT_ARR[ii]==NULL) return;

   seg=Sect_tct[ii];
   i_ptr=(int *) _MK_FP_(seg,Beg_tct[ii]);
   n_TCT[ii]=*i_ptr;

   if((n_TCT[ii] <= 1) || (n_TCT[ii] >= max_TCT))
   {
     n_TCT[ii]=2;
     TCT_ARR[ii][0].level=0.;
     TCT_ARR[ii][0].volume=0.;
     TCT_ARR[ii][1].level=3000.;
     TCT_ARR[ii][1].volume=3000.;
   }
   else
   {
   TCT_ptr=(struct s_TCT *) _MK_FP_(seg,Beg_tct[ii]+4 );

   for(i=0;i<n_TCT[ii];i++)

     TCT_ARR[ii][i]=TCT_ptr[i];
   //   TCT_T1[i]=TCT_ptr[i];
   }

 }


/*

 if(TCT_ARR[1]==NULL) return;

 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0x4000);
 n_TCT[1]=*i_ptr;

 if((n_TCT[1] <= 1) || (n_TCT[1] >= max_TCT))
 {
   n_TCT[1]=2;
   TCT_T2[0].level=0.;
   TCT_T2[0].volume=0.;

   TCT_T2[1].level=3000.;
   TCT_T2[1].volume=3000.;
 }
 else
 {

  TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)0x4004 );

  for(i=0;i<n_TCT[1];i++)
    TCT_T2[i]=TCT_ptr[i];
 }

 if(TCT_ARR[2]==NULL) return;

 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0x8000);
 n_TCT[2]=*i_ptr;

 if((n_TCT[2] <= 1) || (n_TCT[2] >= max_TCT))
 {
   n_TCT[2]=2;
   TCT_T3[0].level=0.;
   TCT_T3[0].volume=0.;

   TCT_T3[1].level=3000.;
   TCT_T3[1].volume=3000.;
 }
 else
 {

  TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)0x8004 );

  for(i=0;i<n_TCT[2];i++)
    TCT_T3[i]=TCT_ptr[i];
 }


 if(TCT_ARR[3]==NULL) return;

 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0xC000);
 n_TCT[3]=*i_ptr;

 if((n_TCT[3] <= 1) || (n_TCT[3] >= max_TCT))
 {
   n_TCT[3]=2;
   TCT_T4[0].level=0.;
   TCT_T4[0].volume=0.;

   TCT_T4[1].level=3000.;
   TCT_T4[1].volume=3000.;
 }
 else
 {

  TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)0xC004 );

  for(i=0;i<n_TCT[3];i++)
    TCT_T4[i]=TCT_ptr[i];
 }

 seg=seg_TCT2;

 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0);

 if(TCT_ARR[4]==NULL) return;

 n_TCT[4]=*i_ptr;

 if((n_TCT[4] <= 1) || (n_TCT[4] >= max_TCT))
 {
   n_TCT[4]=2;
   TCT_T5[0].level=0.;
   TCT_T5[0].volume=0.;
   TCT_T5[1].level=3000.;
   TCT_T5[1].volume=3000.;
 }
 else
 {
 TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)4 );

 for(i=0;i<n_TCT[4];i++)
   TCT_T5[i]=TCT_ptr[i];
 }

 if(TCT_ARR[5]==NULL) return;

 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0x4000);
 n_TCT[5]=*i_ptr;

 if((n_TCT[5] <= 1) || (n_TCT[5] >= max_TCT))
 {
   n_TCT[5]=2;
   TCT_T6[0].level=0.;
   TCT_T6[0].volume=0.;

   TCT_T6[1].level=3000.;
   TCT_T6[1].volume=3000.;
 }
 else
 {

  TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)0x4004 );

  for(i=0;i<n_TCT[1];i++)
    TCT_T6[i]=TCT_ptr[i];
 }

 if(TCT_ARR[6]==NULL) return;

 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0x8000);
 n_TCT[6]=*i_ptr;

 if((n_TCT[6] <= 1) || (n_TCT[6] >= max_TCT))
 {
   n_TCT[6]=2;
   TCT_T7[0].level=0.;
   TCT_T7[0].volume=0.;

   TCT_T7[1].level=3000.;
   TCT_T7[1].volume=3000.;
 }
 else
 {
  TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)0x8004 );

  for(i=0;i<n_TCT[6];i++)
    TCT_T7[i]=TCT_ptr[i];
 }


 if(TCT_ARR[7]==NULL) return;

 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0xC000);
 n_TCT[7]=*i_ptr;

 if((n_TCT[7] <= 1) || (n_TCT[7] >= max_TCT))
 {
   n_TCT[7]=2;
   TCT_T8[0].level=0.;
   TCT_T8[0].volume=0.;

   TCT_T8[1].level=3000.;
   TCT_T8[1].volume=3000.;
 }
 else
 {
  TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)0xC004 );

  for(i=0;i<n_TCT[7];i++)
    TCT_T8[i]=TCT_ptr[i];
 }

 if(TCT_ARR[8]==NULL) return;

 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0xE000);
 n_TCT[8]=*i_ptr;

 if((n_TCT[8] <= 1) || (n_TCT[8] >= max_TCT))
 {
   n_TCT[8]=2;
   TCT_T9[0].level=0.;
   TCT_T9[0].volume=0.;

   TCT_T9[1].level=3000.;
   TCT_T9[1].volume=3000.;
 }
 else
 {
  TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)0xE004 );

  for(i=0;i<n_TCT[8];i++)
    TCT_T8[i]=TCT_ptr[i];
 }

*/


}
// ----------------------------------------
void f_slaves()
{
     if(flag_Slv !=0 )
           f_SlaveRTU();

     if(flag_Slv2 !=0 )
           f_Slave2RTU();

     if(flag_Slv3 !=0 )
           f_Slave3RTU();
}

//------------------------------
float lvl_mm=0;
void f_prep_VM(int ii)
{

 float ftmp,ftmp1;

//     ftmp1=s_DGM[ii].Prod_lvl;
//     s_DGM[ii].Prod_lvl=MM_LVL;
     lvl_mm=MM_LVL;

     ftmp=f_get_vol(ii,lvl_mm);
     Vol_max[ii]=ftmp;

     Vol_10[ii]= ftmp * prc10[ii];
     Vol_90[ii]= ftmp * prc90[ii];

     flag_maxV[ii]=1;
//     s_DGM[ii].Prod_lvl=ftmp1;

}

//------------------------------
//------------------------------
int flag_maxV[max_nn]={0,0};
float Vol_max[max_nn]={20000.,20000.,20000.};
float Vol_10[max_nn]={9000.,9000.,9000.};
float Vol_90[max_nn]={1000.,1000.,1000.};

//float *val_Temp[max_nn]={&Temp1  ,&Temp2 ,&Temp3 };
// температура паровой фазы
float *val_Temp[max_nn]={&s_DGM[0].Temp  ,&s_DGM[1].Temp  ,&s_DGM[2].Temp  };
float *val_Press[max_nn]={&Press1,&Press2,&Press3};

float D_Etan100,D_Etan,P_Etan;
float lvl_rab=0;
int Fl_ch_mm[max_nn]={0,0,0,0,0,0,0,0,0};

void f_count_tank(int ii)
{ // обсчет резервуара ii
int i;
float Vol_liq,ftmp,ftmp1;
float Vol_wat;
float Compos;

//  По текущим значениям уровня , температуре жидкой и паровой фаз,
//  давлению паровой фызы вычисляются:
//  объем,плотность и масса жидкой и паровой фаз.
//  Используется информация о геометрии резервуара и измеренное
//  значение плотности жидкой фазы и температура при измерении плотности.

//  По значению уровня и геометрии резервуара вычисляется объем
//  жидкой и паровой фаз.

   f_slaves();
   lvl_rab= s_DGM[ii].Prod_lvl;
   Vol_liq=f_get_vol(ii,lvl_rab);

   s_DGM[ii].VolLiq=Vol_liq;


   lvl_rab= s_DGM[ii].Wat_lvl;
   Vol_wat= f_get_vol(ii,lvl_rab);
   s_DGM[ii].VolWat=Vol_wat;

   s_DGM[ii].VolDif= Vol_liq-Vol_wat;


   if(Fl_ch_mm[ii] != 0)
   {
   if(flag_maxV[ii] == 0)
      f_prep_VM(ii);

   ii_LVL[ii] = 0;
   i_LVL=0;

   if( (INP_VAR2 & ctr_msk[ii]) == 0)
   {
    if(( s_DGM[ii].VolLiq < Vol_10[ii] ) || (ICP_error[ii] !=0)  ) ii_LVL[ii] |= OUT1 ;
    if(( s_DGM[ii].VolLiq > Vol_90[ii] ) || (ICP_error[ii] !=0)  ) ii_LVL[ii] |= OUT2 ;
   }

   for(i=0;i<max_nn;i++)
     if(( ( INP_VAR2) & ctr_msk[i]) == 0) i_LVL |=ii_LVL[i];

   }


/*
   if((INP_VAR & 0xC000) == 0)
      i_LVL=ii_LVL[0] | ii_LVL[1];
   else if(( (INP_VAR | INP_VAR2)  & 0xC000) == 0x8000)
      i_LVL=ii_LVL[0] ;
   else if((INP_VAR & 0xC000) == 0x4000)
      i_LVL= ii_LVL[1];
// else if((INP_VAR & 0xC000) == 0xC000) ;
*/

//  По измеренным значениям  плотности жидкой фазы и температуры измерения определяется
//  состав жидкой фазы  ( массовая доля пропана).

   f_slaves();

// s_DGM[ii].DensLiq=s_DGM[ii].DensReal;
   s_DGM[ii].MassLiq=s_DGM[ii].VolDif * s_DGM[ii].DensLiq/1000.;

   return;

/*
m_err:

// s_DGM[ii].VolLiq=-1;
// s_DGM[ii].VolGas=-1;
   s_DGM[ii].MassLiq=-1;
   s_DGM[ii].MassGas=-1;
   s_DGM[ii].DensLiq=-1;
   s_DGM[ii].DensGas=-1;

 return;
*/

}
//------------------------------

