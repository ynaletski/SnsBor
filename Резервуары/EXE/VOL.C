
struct s_TCT TCT_sz;
struct s_TCT *TCT_T1=NULL;
struct s_TCT *TCT_T2=NULL;

int  num_out1 = 6, num_out2 = 7;

struct s_TCT *TCT_ARR[max_nn]={NULL,NULL};
int n_TCT[max_nn]={10,10};
int flag_vol[max_nn]={0,0};
//float (*ff_get_vol[max_nn])(int ii) = {f_get_vol,f_get_vol};



//     see "vol.h"
//#define D_R (1.0)
//#define LVL_MAX (1450.0)


float H_Cyl_M[max_nn]={LVL_MAX,LVL_MAX}; // максимальное значение уровня


int n_Cyl[max_nn]={3,3};             // количество цилиндрических элементов в резервуаре

struct s_Cyl Vol1[cyl_nn]={    // описание цилиндрических элементов резервуара

 1000.,           // float Radius;
 1200.,           // float Radius1;
 1000.,           // float Lgth;
 1100.,           // float Offs;    // смещение центра оси от базы TCT

 500.,            // float Radius;
 650.,           // float Radius1;
 80.,             // float Lgth;
 1150.,           // float Offs;    // смещение от оси

 200.,            // float Radius;
 250.,            // float Radius1;
 100.,            // float Lgth;
 1100.,           // float Offs;    // смещение от оси
};

struct s_Cyl Vol2[cyl_nn]={

 1000.,           // float Radius;
 1000.,           // float Lgth;
 1100.,           // float Radius1;
 1110.,              // float Offs;    // смещение от оси

 500.,            // float Radius;
 550.,            // float Radius1;
 80.,             // float Lgth;
 1150.,           // float Offs;    // смещение от оси

 200.,            // float Radius;
 250.,            // float Radius1;
 100.,            // float Lgth;
 1080.,           // float Offs;    // смещение от оси
};

struct s_Cyl *Cyl_ARR[max_nn]={&Vol1[0],&Vol2[0]};

//#define D_R (0.1)

double f_vol_cyl12 ( double Level, struct s_Cyl *Cyl_str_ptr)
{
// вычисляет объем для данного уровня для усеченного конуса
 int ii,nn;
 double dR,dL;
 double R1; // меньший радиус
 double R2; // больший радиус
 double VV,Lev,R_cur,h;
 int kk;

 if(Cyl_str_ptr->Radius > Cyl_str_ptr->Radius1 )
 {
   R1=Cyl_str_ptr->Radius1;
   R2=Cyl_str_ptr->Radius;
 }
 else
 {
   R1=Cyl_str_ptr->Radius;
   R2=Cyl_str_ptr->Radius1;
 }
 nn= (R2-R1)/ D_R;
 if(nn<2) nn=2;

 dR= (R2-R1)/nn;
 dL= Cyl_str_ptr->Lgth / nn;
 VV=0;

 Lev = Level -  Cyl_str_ptr->Offs;

  for(kk=0,R_cur=R1+0.5*dR;R_cur< R2; R_cur+=dR,kk++ )
   {
       h  = Lev + R_cur;
       VV+=f_vol_cyl(h,R_cur,dL);
//   if(flag_prn)
//         if(kk == 1000){ printf("\n %lf",R_cur); kk=0;}
   }
   return VV;
}

//-------------------------

double f_vol_cyl(double h, double Rr, double Ll)
{
 //
 // Рассматривается цилиндр с горизонтальной осью заполненный до определенного
 // уровня. Уровень заполнения h. Rr - радиус цилиндра. Li - длина цилиндра .
 // h измеряется от нижней точки цилиндра.
 // h, Rr, Ll в мм.
 // Возвращает значение объема в нижней части, л.

 double Bb;
 double Vv;
 double Hh,Hh1;
 double Ss;

 if(h <= 0 )
        return 0.;

 Hh= Rr*2. - h;

 if(Hh <=  0 )
 {
   Ss = (PI * Rr * Rr ) /1000000.;
   Vv=  Ll * Ss ;
   return Vv;
 }

 if(Hh >= Rr)
 {
  Hh1=Hh-Rr;
  if(Hh1 > Rr) Hh1=Rr;
  Bb= acos(Hh1/Rr);
  Ss =( Rr * ( Rr * Bb - Hh1 * sin (Bb)) ) /1000000.;
 }
 else
 {
  Hh1=Rr-Hh;
  if(Hh1 > Rr) Hh1=Rr;

  Bb= acos(Hh1/Rr);
  Ss = Rr * ( Rr * Bb - Hh1 * sin (Bb));
  Ss = (PI * Rr * Rr - Ss) /1000000.;
 }

 Vv=  Ll * Ss;
 return Vv;

}
//------------------------------

float f_get_vol(int ii,float lvl)
{
// по значению уровня возвращает значение текущего объема жидкой фазы
// ii - индекс резервуара

int nn,nn1,nn2,nn3;
struct s_TCT *tct_uk,*tct_ukc;

 float Level,vol;

 if((ii < 0)|| (ii>= max_nn) )
 {
   return 0.;
 }
 tct_uk=TCT_ARR[ii];

 Level=lvl;

 if( Level > H_Cyl_M[ii] )
           Level=H_Cyl_M[ii];

 if( Level < 0.)
           Level=0.;

 nn=n_TCT[ii]-1; // количество записей

 if(nn <= 0) return 0.;

 nn1=0;

 for(nn2=max_ord;nn2>0;nn2=nn2>>1)
 {
  nn3=nn1+nn2;
  if(nn3> nn) continue;

  if(tct_uk[nn3].level > Level);
  else nn1=nn3;
 }
 // в nn1 индекс записи с  TCT_Tx[nn1].level <= Level ;

  if(nn1 >= (n_src-1))
    nn1=n_src-2;
  tct_ukc=&tct_uk[nn1];

  if(tct_ukc[1].level == tct_ukc[0].level)
   vol= tct_ukc[1].volume;
  else
   vol = (tct_ukc[0].volume + (Level - tct_ukc[0].level) * (tct_ukc[1].volume - tct_ukc[0].volume) /(tct_ukc[1].level - tct_ukc[0].level)) ;

 return vol;
}


//------------------------------
void f_prep_vol (int ii)
{
 int jj,kk;
 float vol;
 float Rr,h;

 float step;
 float Level;

  struct s_Cyl *Cyl_ptr;
  struct s_TCT *tct_uk;

  tct_uk=TCT_ARR[ii];
  Cyl_ptr=Cyl_ARR[ii];

  step = H_Cyl_M[ii] / (max_TCT-2);

  for( Level=0.,kk=0; Level <= H_Cyl_M[ii]; Level +=step,kk++ )
  {
     vol=0;
     for(jj=0;jj< n_Cyl[ii];jj++)
     {

      if(Cyl_ptr[jj].Radius == Cyl_ptr[jj].Radius1)
      {
       Rr   = Cyl_ptr[jj].Radius;
       h    = Level - ( Cyl_ptr[jj].Offs - Rr );
       vol += f_vol_cyl((double) h, (double) Rr, (double)Cyl_ptr[jj].Lgth );
      }
      else
      {

        vol += f_vol_cyl12((double) Level,&Cyl_ptr[jj]);
      }
                if(kbhit()) break;
     }
     tct_uk[kk].volume=vol;
     tct_uk[kk].level=Level;
     printf("\n %d  L=%f,   V= %f",kk,Level,vol);

     if(flag_prn)
       if((kk %100)==0) printf(":");

                if(kbhit()) break;
  }

  n_TCT[ii]=kk;
}
//------------------------------
void f_prep_vol_tcb ()
{
 int ii,jj,kk;
 float vol;
 float Rr,h;

 float step;
 float Level=0,Volume;

  struct s_TCT *tct_uk;

  tct_uk=TCT_ARR[0];

     tct_uk[0].volume=0.;
     tct_uk[0].level=0.;
  for( ii=0,jj=1; ii<n_src; ii++ )
  {
     vol=0;
       kk=ii;
       Volume=VV[kk];

     if(L1[kk] <= Level) continue;
     Level=L1[kk];
     tct_uk[jj].volume=Volume;
     tct_uk[jj].level=Level;
     jj++;

     printf("\n %d  L=%f,   V= %f",jj,tct_uk[jj-1].level,tct_uk[jj-1].volume);

     if(flag_prn)
       if((kk %100)==0) printf(":");

                if(kbhit()) break;
  }

  n_TCT[0]=jj;
}
//------------------------------
void f_prep_vol1 ()
{
 int ii,jj,kk;
 float vol;
 float Rr,h;

 float step;
 float Level=0,Volume;

  struct s_Cyl *Cyl_ptr;
  struct s_TCT *tct_uk;

  tct_uk=TCT_ARR[0];

     tct_uk[0].volume=0.;
     tct_uk[0].level=0.;
  for( ii=0,jj=1; ii<n_src; ii++ )
  {
     vol=0;
     if(L1[0] > L1[n_src-1])
     {
       kk=n_src-ii-1;
       Volume=VV[n_src-1]-VV[kk];
     }
     else
     {
       kk=ii;
       Volume=VV[kk]-VV[0];
     }

     if(L1[kk] <= Level) continue;
     Level=L1[kk];
     tct_uk[jj].volume=Volume;
     tct_uk[jj].level=Level;
     jj++;

     printf("\n %d  L=%f,   V= %f",jj,tct_uk[jj-1].level,tct_uk[jj-1].volume);

     if(flag_prn)
       if((kk %100)==0) printf(":");

                if(kbhit()) break;
  }

  n_TCT[0]=jj;
}
//------------------------------

void f_prep_vol2 ()
{
 int ii,jj,kk;
 float vol;
 float Rr,h;

 float step;
 float Level=0,Volume;

  struct s_Cyl *Cyl_ptr;
  struct s_TCT *tct_uk;

  tct_uk=TCT_ARR[0];
     tct_uk[0].volume=0.;
     tct_uk[0].level=0.;

  for( ii=0,jj=1; ii<n_src; ii++ )
  {
     vol=0;
     if(L2[0] > L2[n_src-1])
     {
       kk=n_src-ii-1;
       Volume=VV[n_src-1]-VV[kk];
     }
     else
     {
       kk=ii;
       Volume=VV[kk]-VV[0];
     }

     if(L2[kk] <= Level) continue;
     Level=L2[kk];
     tct_uk[jj].volume=Volume;
     tct_uk[jj].level=Level;
     jj++;

     printf("\n %d  L=%f,   V= %f",jj,tct_uk[jj-1].level,tct_uk[jj-1].volume);

     if(flag_prn)
       if((kk %100)==0) printf(":");

                if(kbhit()) break;
  }

  n_TCT[0]=jj;
}
//------------------------------
/*
float f_get_volD(int ii)
{
 struct s_Cyl *Cyl_ptr;

 int jj;
 float vol;
 float Rr,h;
 float Level;

 Level=s_DGM[ii].Prod_lvl*s_DGM[ii].scale+s_DGM[ii].offset;

 if( Level > H_Cyl_M[ii] )
           Level=H_Cyl_M[ii];

 Cyl_ptr=Cyl_ARR[ii];

     vol=0;
     for(jj=0;jj< n_Cyl[ii];jj++)
     {
       Rr   = Cyl_ptr[jj].Radius;
       h    = Level - ( Cyl_ptr[jj].Offs - Rr );
       vol += f_vol_cyl((double) h, (double) Rr, (double)Cyl_ptr[jj].Lgth );
     }
   return vol;
}
*/
//------------------------------
// ----------------------------------------
/*
int  f_wr_tct()
{// записывает 2 TCT во флэш

unsigned int seg;
unsigned int i,i1,ii;
long int i2;
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
 for(i=0,ii=0x8000;i<2;i++,data++,ii++)
   if (FlashWrite(seg, ii, *data) != 0)
   {
     ICP_error[icp_lst_max] |= Flash_wr_error;
     ret_val=1;
     break;
   }
 data=(char *)&TCT_T2[0];
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
 return ret_val;
}
//----------------------------------
void  f_rd_tct()
{// считывает 2 TCT из флэш

unsigned int seg;
int *i_ptr,i;
struct s_TCT *TCT_ptr;

 if(TCT_ARR[0]==NULL) return;

 seg=seg_TCT;
 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0);
 n_TCT[0]=*i_ptr;

 if((n_TCT[0] <= 1) || (n_TCT[0] >= max_TCT))
 {
   n_TCT[0]=2;
   TCT_T1[0].level=0.;
   TCT_T1[0].volume=0.;
   TCT_T1[1].level=3000.;
   TCT_T1[1].volume=3000.;
// ff_get_vol[0]=f_get_volD;
 }
 else
 {

// ff_get_vol[0]=f_get_vol;

 TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)4 );

 for(i=0;i<n_TCT[0];i++)
   TCT_T1[i]=TCT_ptr[i];
 }


 if(TCT_ARR[1]==NULL) return;

 i_ptr=(int *) _MK_FP_(seg,(unsigned int)0x8000);
 n_TCT[1]=*i_ptr;

 if((n_TCT[1] <= 1) || (n_TCT[1] >= max_TCT))
 {
   n_TCT[1]=2;
   TCT_T2[0].level=0.;
   TCT_T2[0].volume=0.;

   TCT_T2[1].level=3000.;
   TCT_T2[1].volume=3000.;
// ff_get_vol[1]=f_get_volD;
 }
 else
 {

//ff_get_vol[1]=f_get_vol;
  TCT_ptr=(struct s_TCT *) _MK_FP_(seg,(unsigned int)0x8004 );

  for(i=0;i<n_TCT[1];i++)
    TCT_T2[i]=TCT_ptr[i];
 }
}
// ----------------------------------------
void set_ff_get()
{
  if(flag_vol[0])
    ff_get_vol[0]=f_get_vol;
  else
    ff_get_vol[0]=f_get_volD;

  if(flag_vol[1])
    ff_get_vol[1]=f_get_vol;
  else
    ff_get_vol[1]=f_get_volD;
}
//------------------------------
void f_slaves()
{
     if(flag_Slv !=0 )
           f_SlaveRTU();

     if(flag_Slv2 !=0 )
           f_Slave2RTU();
}
*/
//------------------------------
void f_prep_VM(int ii)
{

 float ftmp,ftmp1;

     ftmp1=s_DGM[ii].Prod_lvl;
     s_DGM[ii].Prod_lvl=MM_LVL;

     ftmp=(*ff_get_vol[ii])(ii);
     Vol_max[ii]=ftmp;

     Vol_10[ii]= ftmp * proc10;
     Vol_90[ii]= ftmp * proc90;

     flag_maxV[ii]=1;
     s_DGM[ii].Prod_lvl=ftmp1;

}

//------------------------------
//------------------------------
int flag_maxV[max_nn]={0,0};
float Vol_max[max_nn]={20000.,20000.};
float Vol_10[max_nn]={9000.,9000.};
float Vol_90[max_nn]={1000.,1000.};

//float *val_Temp[max_nn]={&Temp1,&Temp2};
//float *val_Press[max_nn]={&Press1,&Press2};

float D_Etan100,D_Etan,P_Etan;
/*
void f_count_tank(int ii)
{ // обсчет резервуара ii

float Vol_liq,ftmp,ftmp1;
float Vol_gas;
float Compos;

//  По текущим значениям уровня , температуре жидкой и паровой фаз,
//  давлению паровой фызы вычисляются:
//  объем,плотность и масса жидкой и паровой фаз.
//  Используется информация о геометрии резервуара и измеренное
//  значение плотности жидкой фазы и температура при измерении плотности.


//  По значению уровня и геометрии резервуара вычисляется объем
//  жидкой и паровой фаз.

   f_slaves();

   Vol_liq=(*ff_get_vol[ii])(ii);
   s_DGM[ii].VolLiq=Vol_liq;

   if(flag_maxV[ii] == 0)
                  f_prep_VM(ii);

   ii_LVL[ii] = 0;
   if( s_DGM[ii].VolLiq < Vol_10[ii] )  ii_LVL[ii] |= OUT1 ;
   if( s_DGM[ii].VolLiq > Vol_90[ii] )  ii_LVL[ii] |= OUT2 ;
   i_LVL=ii_LVL[0] | ii_LVL[1];

   Vol_gas=Vol_max[ii]-Vol_liq;
   s_DGM[ii].VolGas=Vol_gas;


//  По измеренным значениям  плотности жидкой фазы и температуры измерения определяется
//  состав жидкой фазы  ( массовая доля пропана).

   f_slaves();
   if(s_DGM[ii].DensReal <= 446) goto m_err; // 446 кг/м3  - плотность жидкого пропана при 50 C

    Compos = f_get_composition(s_DGM[ii].DensReal,s_DGM[ii].TempReal);

   if(Compos < 0) goto m_err;

//  По составу жидкой фазы и текущей температуре жидкой фазы определяется
//  текущая плотность жидкой фазы.

   f_slaves();

   ftmp=f_get_Dns(Compos,s_DGM[ii].Temp);

   if(ftmp < 0) goto m_err;
   s_DGM[ii].DensLiq=ftmp;

//  По составу жидкой фазы и температуре паровой фазы определяется
//  плотность паровой фазы пропана-бутана (без учета этана) и
//  давление паровой фазы пропан-бутана

   ftmp=*val_Temp[ii];
   if(ftmp < s_DGM[ii].Temp) ftmp=s_DGM[ii].Temp;

   f_slaves();

   f_get_pressCT(Compos,ftmp);

//  printf("\n\r Press_gas=%f, Dens_gas=%f",s_MVD[0].PressG , s_MVD[0].DensG);
 // результат в s_MVD[0].PressG , s_MVD[0].DensG

   P_Etan= *val_Press[ii] - s_MVD[0].PressG;
   if(P_Etan > 0)
   {
     ftmp1=f_get_PD_Etan(ftmp);
     // результат плотности в D_Etan100
     D_Etan=D_Etan100 * P_Etan/ftmp1;

//     printf("\n\r D_Etan=%f",D_Etan);

   }
   else
    D_Etan=0;

   f_slaves();

   s_DGM[ii].DensGas=s_MVD[0].DensG+D_Etan;

   s_DGM[ii].MassLiq=Vol_liq*s_DGM[ii].DensLiq/1000.;
   s_DGM[ii].MassGas=Vol_gas * s_DGM[ii].DensGas/1000.;
   return;

m_err:

// s_DGM[ii].VolLiq=-1;
// s_DGM[ii].VolGas=-1;
   s_DGM[ii].MassLiq=-1;
   s_DGM[ii].MassGas=-1;
   s_DGM[ii].DensLiq=-1;
   s_DGM[ii].DensGas=-1;

 return;
}
//------------------------------
*/
