
/*-------------------*/
int cod_er_acs=1;
int flag_p=0,n_lin=0,fl_pcom=0;
/*-----------------------------*/
float n_ver=8.0;

#define Version_subnumber (74)

char VerNameDate[]={"13 Nov 2010"};

char wrd_tmp[20]={""};
/*-----------------------------*/
// doubled.asm
float ddd1=0,ddd2=0,ddd3=0,ddd4=0;
float ddd[8]={12123454,567891,1,12,0,0,6};
float ddt[8];
float ddf[8]={1.,0.};

float dbl_one[2]={1.0,0};
float dbl_two[2]={10.0,0};
float dbl_ppm[2];

float dbl_op1[2];
float dbl_op2[2];
float dbl_op3[2];

 float dd_res[6];
/*-----------------------------*/
int intr_bsy = 0;
int flag_ea=0;
unsigned int ii2;
int ED_addr=0;
int EW_addr=0;
int J_com_n=-1,J_com_c=-1,J_;
int J_com_cc;

void interpr()
/* --------------------------- */
{    // beg_int

int *EFI;

 #if (Cor_XY != 0)
  float S_cur;
  float c_ii,c_jj;
  float c_mlx,c_mly;
  float v1,v2,v3,v4;
  float F_Addr;
  int F_Addr1;

  union { float fl;
          int   it;
        } uu_cf1,uu_cf2,uu_cf3,uu_cf4;

  unsigned int mask1;
  unsigned int mask0;
  float corXY_scale;
  struct s_cor_xy *uk_xy;

 #endif // (Cor_XY != 0)

float *ftmp_ptr;
float k_r_c;
char *strg;
int i,i1,i2,j;
int tmp,tmp1,itmp;
float ftmp,ftmp1,ftmp2,ftmp3,ftmp4;
unsigned int *uk_int,i_tmp;
int itmp0,itmp1;
struct bt_filtr *uk_bt;
//------------------------------
#if(TWO_MS != 0)

int QuadNum,qc,NewCnt;
int SS;
int AA,BB,AR,BR,I0,NewCor;

#endif
//------------------------------

        intr_bsy = 1;
        intrpr.i3=0;

        if(f_inw()==0)
        {
          intrpr.n_bcom=0;
          goto fin;
        }
     intrpr.n_bcom=0;
     intrpr.bfcom[0]=0xff;
    /*-------------------------------------------------*/
#if(Id_check != 0)
      if(id_ok==0)
       {
    /*-------------------------------------------------*/
        if ( (intrpr.wrd[0]=='I') && (intrpr.wrd[1]=='D')&& (intrpr.wrd[2]=='3')&& (intrpr.wrd[3]=='1')&& (intrpr.wrd[4]=='5'))
         /* --------  command 'ID315' ------------ */
         if(intrpr.wdl==5)
          {
           f_check_id(n_id);
           n_id=f_res_id;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I') && (intrpr.wrd[1]=='D') )
         /* --------  command 'ID' ------------ */
          {
           if(f_dis_set(&ds_id,2,1) == 0)
            {
             puts(" ");
             printf("%d %d",*( unsigned int *)(ID_ADDR),*( unsigned int *)(ID_ADDR+7) );
            }

           if( f_check_id(n_id) == 0) id_ok=1;
           else id_ok=0;

           goto fin;
          }
    /*----------------------------------------------*/
          f_delay_p(500000.);
          printf("\n!Not ");
          f_delay_p(500000.);
          printf("correct ");
          f_delay_p(1000000.);
          printf("ID !!!\n");
          f_delay_p(500000.);
       }
#endif
    /*-------------------------------------------------*/
        if ( intrpr.wrd[0]=='2' )
         /* --------  команда Время 2  '2 ' ------------ */
          {
             f_tim3();
             print_t2();
           min_t1=0x7fffffff;max_t1=0;
           min_t2=0x7fffffff;max_t2=0;
           min_t3=0x7fffffff;max_t3=0;
             goto fin;
          }
    /*----------------------------------------------*/
#if(PR_SENS != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G') && (intrpr.wrd[1]=='P'))
         /* --------  команда 'GP' ------------ */
         /* Display Pressure signal  */
          if(intrpr.wdl==2)
            {
              if(f_inw()) goto fin_e;
              itmp=Analog_pr*10.+0.5;
              if(itmp<0) itmp=0;
              if(itmp>9999) itmp=9999;
              printf("%d",itmp);
              goto fin;
            }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G')&&(intrpr.wrd[1]=='P')&&(intrpr.wrd[2]=='R')  )
         /* -----   command 'GPR'        ---- */
           {
             if(f_dis_set(ds_gpr,3,4)>0)
               f_cfg_count();
             goto fin;
           }
#endif   // PR_SENS != 0
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&& (intrpr.wrd[1]=='B') )
         /* --------  'CB' - команда Очистить  буфер записи ---- */
          {
           if(fl_trace_loop == 0)
              uk_buf=0;
           min_t1=0x7fffffff;max_t1=0;
           min_t2=0x7fffffff;max_t2=0;
           min_t3=0x7fffffff;max_t3=0;
#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
           f_clear_buf();  // Clear USB debug buffers
#endif

           goto fin;
          }
    /*----------------------------------------------*/

      if(fl_ep == 0)
        if ( ( flag_err[0] + flag_err[1] + flag_err[2] + flag_err[3]+ flag_err[4]+ flag_err[5]) != 0. )
         {
            fl_err=0.;
         }
  /*-------------------------------------------------*/
  /* в intrpr.wrd     -  команда                     */
  /*-------------------------------------------------*/
    /*- проверка на внешние команды -----------------*/

       if(flag_ea == 0)
        if( (kol_com != 0) )
        {
          strcpy(tmp_str,intrpr.wrd);

          for(i1=0;i1<wrd_w;i1++)
           {
             if( isalpha(tmp_str[i1]) );
             else break;
           }
          tmp_str[i1]=0;

          for(j=0;j<kol_com;j++)
            if(strcmp(tmp_str,com[j].name ) == 0)  goto ext1;

/*----------------------------------------------*/
#if(D_T != 0)
     if(flag_p!=0)
     {
          if(isdigit(intrpr.wrd[0]))
           {
              for(i=0;i<19;i++)
              {
                if(isdigit(intrpr.wrd[i]))
                  wrd_tmp[i]=intrpr.wrd[i];
                else
                {
                  wrd_tmp[i]=0;
                  if(f_innum(wrd_tmp) )
                  {
                    n_lin=intrpr.num_var;
                    if((intrpr.wrd[i]=='L') || ((intrpr.wrd[i]==' ')&&(intrpr.wrd[i+1]=='L')  ) )
                          goto fin_pp0;
                    else goto fin_pp;
                  }
                  else goto fin_e;
                }
              }
           }

        if (  (intrpr.wrd[0]=='E' ) && (intrpr.wdl==1))
         /*  -----   command  'E' 'E ' "E"  ------------ */
        {
             if(f_inw()!=0) goto fin_e;
             flag_p=0;
             n_lin=1;
             goto fin;
        }
   /*----------------------------------------------*/
        if (  (intrpr.wrd[0]=='L' ) && (intrpr.wrd[1]=='/' ) &&
 (intrpr.wrd[2]=='M' ) && (intrpr.wrd[3]=='S' )&& (intrpr.wrd[4]=='C' ) )

         /*  -----   command  'L/MSC' "L/MSC"  ------------ */
        {

          for(n_lin=616;n_lin<626;n_lin++)
            printf("\n%d:rem",n_lin);
          n_lin=616;
          goto fin_pp;
        }
   /*----------------------------------------------*/
        if (  (intrpr.wrd[0]=='L' ) )
         /*  -----   command  'L ' "L "  ------------ */
        {
               if(intrpr.wdl==1)
                  if(f_inw()) strg=intrpr.wrd;
                  else
                    {
                      n_lin--;
                      goto fin;
                    }
               else
                   strg=&intrpr.wrd[1];
               if(f_innum(strg) )
                {
                  n_lin=intrpr.num_var;
                  goto fin_pp;
                }
               goto fin_e;
        }
   /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='X'))
         /* --------  command 'X'  ---- */
           {
            if(kol_err) goto fin_e;
            intrpr.i3=1;
            goto px1;
           }
   /*----------------------------------------------*/
        goto fin_e;
     }
#endif // (D_T != 0)
/*----------------------------------------------*/
          goto m_mn;
     ext1:  // отработка внешней команды
         // j - номер команды
         // i1 - длина команды

          if(intrpr.wdl==i1)
             if(f_inw()) strg=intrpr.wrd;
             else
              {
               for(i=0;i<com[j].n_dsp;i++)
                 printf(" %f", var[com[j].mas_dsp[i]] );
               if(com[j].flag) var[com[j].flag] = 0.;

               if(com[j].proc_flag < 0) goto ext2;

               goto fin;
              }
          else
              strg=&intrpr.wrd[i1];

          for(i=0;i<com[j].n_wrt;i++)
            if(f_innum(strg) )
             {
              var[com[j].mas_wrt[i]]=intrpr.num_var;
              if(com[j].flag) var[com[j].flag]=1.+i;
              if(f_inw() == 0) goto ext2;
             }
            else  goto fin_e;
   ext2:   // проверка на необходимость запуска процесса

              if(com[j].proc_flag == 0) goto fin;
              else
               {
                 for(i=0;i<n_max;i++)
                  if(intrr[i].sw)
                    if(strcmp(intrr[i].name,com[j].proc_name ) == 0)  goto ext3;
//--------------------
#if(AUTOM != 0)
             for(i=0;i<N_aut;i++ )
               if(strcmp(intrr[i].name,autom_nm[i]) == 0)
                  for(ii=0;ii<N_aut;ii++)
                  {
                   if(ff_autom[ii] == ff_automp[i])
                   {
              //----------------
                       switch( (int)fabs((float)com[j].proc_flag) )
                       {
                         case 1: goto fin;
                         case 2: goto fin_e;
                         case 7:
                         case 3: {ff_autom[ii]=NULL; goto fin_e;}
                         case 4: if( f_run( com[j].proc_name) ) goto fin;
                                 else  goto fin_e;

                         default: goto fin_e;
                       }

                       goto fin;
              //----------------
                   }
                  }
#endif // (AUTOM != 0)
//--------------------------------------
                 // процесс не запущен
                 if( f_run( com[j].proc_name) == 0)  goto fin_e;
                 else
                    intr_bsy = 0;

                 if( fabs((float)com[j].proc_flag) == 7)
                  {
         ext21:
                   if(kbhit() )
                       if(getch()==BS) goto ext3;
                   f_int_ea0();
                   for(i=0;i<n_max;i++)
                     if(intrr[i].sw)
                       if(strcmp(intrr[i].name,com[j].proc_name ) == 0)  goto ext21;

#if(AUTOM != 0)
                   if(ff_automl != NULL)
                   {
                     for(ii=0;ii<N_aut;ii++)
                     {
                      if(ff_autom[ii] == ff_automl)  goto ext21;
                     }
                   }
#endif // (AUTOM != 0)

             // if  (flag-variable < 0 ) -->  error prompt "?\n>"

                   if(com[j].flag)
                     if(var[com[j].flag] < 0.)  goto fin_e;

                  }

               }
               goto fin;

   ext3:   // требуемый процесс уже запущен

         switch( (int)fabs((float)com[j].proc_flag) )
         {
           case 1: goto fin;
           case 2: goto fin_e;
           case 7:
           case 3: fcloss(i); goto fin_e;
           case 4: fcloss(i);
                 if( f_run( com[j].proc_name) ) goto fin;
                 else  goto fin_e;

           default: goto fin_e;
         }

         goto fin;

        }
    /*----------------------------------------------*/
  m_mn:

#if(LOTIS == 0)
/* ----------------------- */
        if (intrpr.wrd[0]=='J')
         /* --------  command 'J '  ------------ */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;
/*
           printf(" com_num = %d ;\n",*com_num);
           if((*com_num)>35) *com_num=35;
           J_COM = (struct com_jjf *) (&intrpr.bfcom[3]) ;

           for(i=0;i < *com_num;i++)
           {
            printf("FL_COM=%4x;par1= %f;par2=%f;par3=%f;par4=%f;par5=%f;\n",\
            J_COM[i].com,J_COM[i].par[0],J_COM[i].par[1], \
            J_COM[i].par[2],J_COM[i].par[3],J_COM[i].par[4],J_COM[i].par[5]);
           }
*/
  //    J_com_n=*com_num;
          if((J_com_n)>Max_com_bl)
          {
            flag_err[0] = (int)flag_err[0] | ERR_N_BL;
            goto fin0;
          }
           J_COM = (struct com_jjf *) (&intrpr.bfcom[3]) ;

           for(J_com_c=0;J_com_c < J_com_n;J_com_c++,J_COM++)
           {
            J_com_cc=J_COM->com;// код и флаги текущей команды

            while(fl_new_m)
             {
              f_usb_sh();
              if(fl_new_m==0) break;
              //--------------------------------
              // --  электроавтоматика
              if (pp++ >= n_max) pp=0;
                 if(intrr[pp].sw)
                 {
                   f_interpr(pp);
                 }
              // -------------------------------
             }
            switch( J_com_cc & 0xff )
            {
             case 1: goto jm1;       // команда линейного перемещения
             case 2: goto jm2;       // команда перемещения по дуге
             case 3: goto jm3;       // команда задания скорости-ускорения
             default: goto jm_err;
            }
 jm1:  // команда линейного перемещения
       // ввод параметров
        // установка флагов сопряжения p1.CO
        var[i_fl_cc]= J_com_cc >> 14; // D1.CO = D15, D0.CO=D14;

        if( (J_com_cc & msk[0]) != 0)
           sx_0 =J_COM->par[0] - move_p[m_fin].x_b;
        else sx_0=0;

        if( (J_com_cc & msk[1]) != 0)
           sy_0 =J_COM->par[1] - move_p[m_fin].y_b;
        else sy_0=0;

        if( (J_com_cc & msk[2]) != 0)
           sz_0 =J_COM->par[2] - move_p[m_fin].z_b;
        else sz_0=0;

        if( (J_com_cc & msk[3]) != 0)
           sc_0 =J_COM->par[3] - move_p[m_fin].c_b;
        else sc_0=0;

        if( (J_com_cc & msk[4]) != 0)
           sb_0 =J_COM->par[4] - move_p[m_fin].b_b;
        else sb_0=0;

    #if(Loop_x != 0)

           if( fabs(sx_0) < (Per_loop * 1.5 ) )
            if( fabs(sx_0) > (Per_loop * 0.5))
             if(sx_0 < 0) sx_0 += Per_loop;
             else sx_0 -= Per_loop;
    #endif

    #if(Loop_c != 0)

           if( fabs(sc_0) < (Per_loop * 1.5 ) )
            if( fabs(sc_0) > (Per_loop * 0.5))
             if(sc_0 < 0) sc_0 += Per_loop;
             else sc_0 -= Per_loop;
    #endif

           Flag_arc=0;
           Flag_delay=0;

mJ_inp_str:
           if((itmp1=f_prep_str())== 0) continue; // команда введена успешно
           else if(itmp1<0)
           { // выход за пределы рабочей зоны
              flag_err[itmp1+N_int_ax] =(int)flag_err[itmp1+N_int_ax] | LIM_DIST;
              goto fin0;
           }
           else
           {
           // нет места в буфере перемещений
            flag_err[1] = (int)flag_err[1] | ERR_N_BL;
            goto fin0;
          }
//======================================
 jm2:  // команда перемещения по дуге

        if( (J_com_cc & 0x0700) != 0x0700)
        { // задано менее 3 параметров команды 'AA'
            flag_err[2] = (int)flag_err[2] | ERR_N_BL;
            goto fin0;

        }
       // установка флага сопряжения p1.ARS
        var[i_fl_arc_s]= J_com_cc >> 15; //  D0.ARS=D15;

       // ввод 3 обязательных параметров
        sx_0 =J_COM->par[0];
        sy_0 =J_COM->par[1];
        arc_angle =J_COM->par[2];

       // ввод необязательного параметра (смещение по Z)
        if( (J_com_cc & msk[3]) != 0)
           sz_0 =J_COM->par[3];
        else sz_0=0;

        Flag_arc=1;
        Flag_delay=0;
        goto mJ_inp_str;
//======================================
 jm3:  // команда задания скорости-ускорения

        if( (J_com_cc & 7) != 0)
        {
          if( (J_com_cc & msk[0]) != 0)
             var[Ind_FA_V] =J_COM->par[0];

          if( (J_com_cc & msk[1]) != 0)
             var[Ind_FA_A] =J_COM->par[1];

          if( (J_com_cc & msk[2]) != 0)
             e_m_s3 =J_COM->par[2];
          f_v_a();
        }
        continue;
//======================================
 jm_err:
    flag_err[0] = (int)flag_err[0] | ERR_UNKN_BL;
    goto fin0;
    /*
            printf("FL_COM=%4x;par1= %f;par2=%f;par3=%f;par4=%f;par5=%f;\n",\
            J_COM[i].com,J_COM[i].par[0],J_COM[i].par[1], \
            J_COM[i].par[2],J_COM[i].par[3],J_COM[i].par[4],J_COM[i].par[5]);
    */
           }
           goto fin0;
          }
/*-----------------------------------------------------*/
/* ----------------------- */
        if( var[Warning] != 0 )  goto m_disp;
        if(fl_err == 0.) goto m_disp;

        if((fl_move_off!=0) && (flag_ea == 0)) goto m_disp1;

        if(!Flag_otr) goto m_disp;
        if( (flag_st_d != 0.) || (flag_USB_int != 0.) || ( f_iter == f_USB_int  )  )
           // Step/Dir board connected
           // some command not performed
             goto m_disp1;
/* ----------------------- */
#if(D_T != 0)
        if ( (intrpr.wrd[0]=='B')&&(intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='T')&&( (intrpr.wdl==3)|| (intrpr.wdl==4) ) )
         /* --------  command 'BXT'   ---- */
         /* --------  command 'BXTI'  ---- */
           {
             if(  ( (m_fin-m_beg)&(sz_move-1)  ) > ( sz_move - 10) ) goto mm_bz;

             if  (intrpr.wrd[3]=='I')
             {
               if(flag_chg==1)  goto bxt1;

               i1 = var[Beg_IS + X_N];
               if( (i1 < 1 ) || ( i1 > MAX_IK ) ) goto fin_e; // number of input is not correct

               sx_0 = (int)pf_ik[i1] - (int)&var[0];
               sz_0 = mskf_ik[i1];
               sy_0 = sz_0;

               sb_0=0;

     //        sx_0                // variable number
     //        sy_0                // variable value
     //        sz_0                // variable AND mask
               sc_0 = 2;           // flag WV command
     //        sb_0                // delay from previous wait command,in interrupt pass

               Flag_arc=0;
               Flag_delay=3;

               itmp1=f_prep_str();
             }
             else if(intrpr.wdl==3)
             {
bxt1:         itmp1=f_bxt();
              goto struct_i1;
             }
             else if(intrpr.wrd[3]=='Z')
             {
         /* --------  command 'BXTZ'   ---- */
               goto bxtz;
             }
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='B')&&(intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='T')&& (intrpr.wrd[3]=='Z') && (intrpr.wrd[4]=='I') )
         /* --------  command 'BXTZI'  ---- */
           {
             if(  ( (m_fin-m_beg)&(sz_move-1)  ) > ( sz_move - 10) ) goto mm_bz;

             i1 = var[Beg_IS + X_N];
             if( (i1 < 1 ) || ( i1 > MAX_IK ) ) goto fin_e; // number of input is not correct

             sx_0 = (int)pf_ik[i1] - (int)&var[0];
             sz_0 = mskf_ik[i1];
             sy_0 = sz_0;

             sb_0=0;

     //      sx_0                // variable number
     //      sy_0                // variable value
     //      sz_0                // variable AND mask
             sc_0 = 2;           // flag WV command
     //      sb_0                // delay from previous wait command,in interrupt pass

             Flag_arc=0;
             Flag_delay=3;

             itmp1=f_prep_str();
      bxtz:
              itmp1=f_bxtz();
              goto struct_i1;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='B')&&(intrpr.wrd[1]=='X')&& ((intrpr.wdl==2)||(intrpr.wdl==3) ) )
         /* --------  command 'BX'  ---- */
         /* --------  command 'BXI'  ---- */
           {
             if(  ( (m_fin-m_beg)&(sz_move-1)  ) > ( sz_move - 10) ) goto mm_bz;
             if  (intrpr.wrd[2]=='I')
             {
               i1 = var[Beg_IS + X_N];
               if( (i1 < 1 ) || ( i1 > MAX_IK ) ) goto fin_e; // number of input is not correct

               sx_0 = (int)pf_ik[i1] - (int)&var[0];
               sz_0 = mskf_ik[i1];
               sy_0 = sz_0;

               sb_0=0;

     //        sx_0                // variable number
     //        sy_0                // variable value
     //        sz_0                // variable AND mask
               sc_0 = 2;           // flag WV command
     //        sb_0                // delay from previous wait command,in interrupt pass

               Flag_arc=0;
               Flag_delay=3;

               itmp1=f_prep_str();
             }

             var[Ind_FA_V]= K_VDT * var[Beg_V + V_N];     // скорость в мм/с
             var[Ind_FA_A]= K_ADT * var[Beg_A + V_N];     // ускорение в м/с2

             if(var[Ind_FA_V]<=0)  var[Ind_FA_V]=1.;
             if(var[Ind_FA_A]<=0)  var[Ind_FA_A]=1.;

             if(var[Beg_SM+V_N]<=0)  var[Beg_SM+V_N]=1.;
             var[Ind_FA_E]= var[Ind_FA_A] * 1000./ var[Beg_SM+V_N] ;     // ускорение в м/с2

             f_v_a();

            if( var[Beg_RP + X_N] == 0)
            {
             sy_0 = K_XDT * var[Beg_AP + X_N] - move_p[m_fin].y_b;
            }
            else
            {
               sy_0 = K_XDT * var[Beg_RP + X_N] ;
               var[Beg_AP + X_N]= K_XDT1 * (move_p[m_fin].y_b + sy_0);
               var[Beg_RP + X_N]=0;
            }
             sx_0 = 0;
             sz_0 = 0;
             sc_0 = 0;
             sb_0 = 0;

             Flag_arc=0;
             Flag_delay=0;
             goto struct_i;

           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='B')&&(intrpr.wrd[1]=='Z')&&(intrpr.wrd[2]=='S') )
         /* --------  command 'BZS'  ---- */
           {
             if(  ( (m_fin-m_beg)&(sz_move-1)  ) > ( sz_move - 10) ) goto mm_bz;

             itmp1=f_sftlnd();
             goto struct_i1;

           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='B')&&(intrpr.wrd[1]=='Z')&& ((intrpr.wdl==2)||(intrpr.wdl==3) ) )
         /* --------  command 'BZ'  ---- */
         /* --------  command 'BZI'  ---- */
           {
             if(  ( (m_fin-m_beg)&(sz_move-1)  ) > ( sz_move - 8 ) )
               {
 mm_bz:
                 if(flag_ea == 0)
                  {
                    putch_ea('!');
                    goto fin;
                  }
                 else goto fin_e;
               }

             if  (intrpr.wrd[2]=='I')
             {
               i1 = var[Beg_IS + Z_N];
               if( (i1 < 1 ) || ( i1 > MAX_IK ) ) goto fin_e; // number of input is not correct

               sx_0 = (int)pf_ik[i1] - (int)&var[0];
               sz_0 = mskf_ik[i1];
               sy_0 = sz_0;

               sb_0=0;

     //        sx_0                // variable number
     //        sy_0                // variable value
     //        sz_0                // variable AND mask
               sc_0 = 2;           // flag WV command
     //        sb_0                // delay from previous wait command,in interrupt pass

               Flag_arc=0;
               Flag_delay=3;

               itmp1=f_prep_str();
             }

             itmp1=f_bz();
             goto struct_i1;

           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='B')&&(intrpr.wrd[1]=='T')&& ((intrpr.wdl==2)||(intrpr.wdl==3) ) )
         /* --------  command 'BT'  ---- */
         /* --------  command 'BTI'  ---- */
           {
             if(  ( (m_fin-m_beg)&(sz_move-1)  ) > ( sz_move - 10) ) goto mm_bz;
             if  (intrpr.wrd[2]=='I')
             {
               i1 = var[Beg_IS + Y_N];
               if( (i1 < 1 ) || ( i1 > MAX_IK ) ) goto fin_e; // number of input is not correct

               sx_0 = (int)pf_ik[i1] - (int)&var[0];
               sz_0 = mskf_ik[i1];
               sy_0 = sz_0;

               sb_0=0;

     //        sx_0                // variable number
     //        sy_0                // variable value
     //        sz_0                // variable AND mask
               sc_0 = 2;           // flag WV command
     //        sb_0                // delay from previous wait command,in interrupt pass

               Flag_arc=0;
               Flag_delay=3;

               itmp1=f_prep_str();
             }


             var[Ind_FA_V]= K_VDT * var[Beg_V + V_N];     // скорость в мм/с
             var[Ind_FA_A]= K_ADT * var[Beg_A + V_N];     // ускорение в м/с2

             if(var[Ind_FA_V]<=0)  var[Ind_FA_V]=1.;
             if(var[Ind_FA_A]<=0)  var[Ind_FA_A]=1.;

             if(var[Beg_SM+V_N]<=0)  var[Beg_SM+V_N]=1.;
             var[Ind_FA_E]= var[Ind_FA_A] * 1000./ var[Beg_SM+V_N] ;     // JERK  m/s3

             f_v_a();

            if(var[Beg_RP + Y_N] == 0)
            {
             sx_0 = K_XDT * var[Beg_AP + Y_N] - move_p[m_fin].x_b;
            }
            else
            {
               sx_0 = K_XDT * var[Beg_RP + Y_N] ;
               var[Beg_AP + Y_N]= K_XDT1 * (move_p[m_fin].x_b + sx_0);
               var[Beg_RP + Y_N]=0;
            }
             sy_0 = 0;
             sz_0 = 0;
             sc_0 = 0;
             sb_0 = 0;

             Flag_arc=0;
             Flag_delay=0;
             goto struct_i;

           }
    /*--------------*/
        if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='S')&&(intrpr.wdl==2)  )
         /* --------  command 'PS' ---- */
           {
             goto ss0;
           }
    /*----------------------------------------------*/
    /*--------------*/

#endif // (D_T != 0)
/* ----------------------- */


//-------------------
#if(TR_DOUBLE == 0)

        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='R') )
         /* --------  команда 'PR'  ------------ */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;
           sx_0=0.;
           sy_0=0.;
           sz_0=0.;
           sc_0=0.;
           sb_0=0.;
           itmp1=f_dis_set(ds_pr,2,5);
           if(itmp1<0) goto fin_e;
           if(itmp1==0) goto fin;

           Flag_arc=0;
           Flag_delay=0;

struct_i:
         f_tim2();
           itmp1=f_prep_str();
         f_tim3();
         min_max_t2();
struct_i1:

           if(itmp1 == 1 )
            {
              if(flag_ea == 0)
               {
                 putch_ea('!');
                 goto fin;
               }
              else goto fin_e;
            }
           else if(itmp1 <0 )
            {
               if(flag_ea == 0)
               {
                   flag_err[itmp1+N_int_ax] =(int)flag_err[itmp1+N_int_ax] | LIM_DIST;
                   goto fin_e;
               }
               else
                    f_err_ea(L_DIST,itmp1+N_int_ax);
               goto fin_e1;
            }
           goto fin;
pr_bad:
   /*      puts("\n Плохие операнды ");  */
           goto fin_e;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&& (intrpr.wrd[1]=='A') )
         /* --------  команда 'AA'  ------------ */
          {
           if(fl_trace_loop == 0)
             if(uk_buf >= (dim_xyz-11)) uk_buf=0;
           sz_0=0.;
           arc_angle=0.;

           itmp1=f_dis_set(ds_aa,2,4);
           if(itmp1<3) goto fin_e;

//         if( (sz_0 != 0.) && (Flag_sys[2] == 0)) goto fin_e;

           Flag_arc=1;
           Flag_delay=0;
           goto struct_i;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='A') )
         /* --------  command 'PA'  ------------ */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

           sx_0 =move_p[m_fin].x_b;
           sy_0 =move_p[m_fin].y_b;
           sz_0 =move_p[m_fin].z_b;
           sc_0 =move_p[m_fin].c_b;
           sb_0 =move_p[m_fin].b_b;

           itmp1=f_dis_set(ds_pr,2,5);
           if(itmp1<0) goto fin_e;

           if(itmp1==0) goto fin;

     //    sx_0 = round_lp(sx_0);

           sx_0 -=move_p[m_fin].x_b;
           sy_0 -=move_p[m_fin].y_b;
           sz_0 -=move_p[m_fin].z_b;
           sc_0 -=move_p[m_fin].c_b;
           sb_0 -=move_p[m_fin].b_b;

    #if(Loop_x != 0)

           if( fabs(sx_0) < (Per_loop * 1.5 ) )
            if( fabs(sx_0) > (Per_loop * 0.5))
             if(sx_0 < 0) sx_0 += Per_loop;
             else sx_0 -= Per_loop;
    #endif

    #if(Loop_c != 0)

           if( fabs(sc_0) < (Per_loop * 1.5 ) )
            if( fabs(sc_0) > (Per_loop * 0.5))
             if(sc_0 < 0) sc_0 += Per_loop;
             else sc_0 -= Per_loop;
    #endif

           for(i=NN_AX;i>=itmp1;i--)
              {
                *uk_zadp1[i]=0.;
                *uk_zadp1d[i]=0.;
              }

           Flag_arc=0;
           Flag_delay=0;
           goto struct_i;

          }
    /*-----------------------------------------------------*/
        if ( (intrpr.wrd[0]=='B')&& (intrpr.wrd[1]!='F') && (flag_DT_BR == 0) )
         /* --------  command 'B '  ------------ */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

           sx_0 =move_p[m_fin].x_b;
           sy_0 =move_p[m_fin].y_b;
           sz_0 =move_p[m_fin].z_b;
           sc_0 =move_p[m_fin].c_b;
           sb_0 =move_p[m_fin].b_b;

           itmp1=f_inp_b();

           if(itmp1<0) goto fin_e;
           if(itmp1==0) goto fin;

           sx_0 -=  move_p[m_fin].x_b;
           sy_0 -=  move_p[m_fin].y_b;
           sz_0 -=  move_p[m_fin].z_b;
           sc_0 -=  move_p[m_fin].c_b;
           sb_0 -=  move_p[m_fin].b_b;

    #if(Loop_x != 0)

           if( fabs(sx_0) < (Per_loop * 1.5 ) )
            if( fabs(sx_0) > (Per_loop * 0.5))
             if(sx_0 < 0) sx_0 += Per_loop;
             else sx_0 -= Per_loop;
    #endif

    #if(Loop_c != 0)

           if( fabs(sc_0) < (Per_loop * 1.5 ) )
            if( fabs(sc_0) > (Per_loop * 0.5))
             if(sc_0 < 0) sc_0 += Per_loop;
             else sc_0 -= Per_loop;
    #endif

           Flag_arc=0;
           Flag_delay=0;
           goto struct_i;
          }
/*-----------------------------------------------------*/
        if (intrpr.wrd[0]=='Y')
         /* --------  command 'Y '  ------------ */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

           sz_0=0.;
           arc_angle=0.;

           itmp1=f_inp_y();

           if(itmp1<0) goto fin_e;
           if(itmp1==0) goto fin;

           Flag_arc=1;
           Flag_delay=0;
           goto struct_i;
          }
/*-----------------------------------------------------*/
#else  //(TR_DOUBLE == 1)
//-------------------------------
//-------------------------------
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='R') )
         /* --------  команда 'PR'  ------------ */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;
           sx_0=0.;
           sx_0d=0.;
           sy_0=0.;
           sy_0d=0.;
           sz_0=0.;
           sz_0d=0.;
           sc_0=0.;
           sc_0d=0.;
           sb_0=0.;
           sb_0d=0.;
           itmp1=f_dis_setD(dsd_pr,2,5);
           if(itmp1<0) goto fin_e;
           if(itmp1==0) goto fin;

           for(i=0;i<itmp1;i++)
              {
                 add12(*uk_zadp1[i],*uk_zadp1d[i],dbl_op1);
                 *uk_zadp1[i] = dbl_op1[0];
                 *uk_zadp1d[i]= dbl_op1[1];
              }
           Flag_arc=0;
           Flag_delay=0;

struct_i:
           itmp1=f_prep_str();
struct_i1:

           if(itmp1 == 1 )
            {
              if(flag_ea == 0)
               {
                 putch_ea('!');
                 goto fin;
               }
              else goto fin_e;
            }
           else if(itmp1 <0 )
            {
               if(flag_ea == 0)
               {
                   flag_err[itmp1+N_int_ax] =(int)flag_err[itmp1+N_int_ax] | LIM_DIST;
                   goto fin_e;
               }
               else
                    f_err_ea(L_DIST,itmp1+N_int_ax);
               goto fin_e1;
            }
           goto fin;
pr_bad:
   /*      puts("\n Плохие операнды ");  */
           goto fin_e;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&& (intrpr.wrd[1]=='A') )
         /* --------  команда 'AA'  ------------ */
          {
           if(fl_trace_loop == 0)
             if(uk_buf >= (dim_xyz-11)) uk_buf=0;
           sz_0=0.;
    //     arc_angle=0.;
           ang_dd[0]=0;
           ang_dd[1]=0;
           itmp1=f_dis_setD(dsd_aa,2,4);
           if(itmp1<3) goto fin_e;

//         if( (sz_0 != 0.) && (Flag_sys[2] == 0)) goto fin_e;


            add12(sx_0,sx_0d,dbl_op1);
            sx_0 = dbl_op1[0];
            sx_0d= dbl_op1[1];

            add12(sy_0,sy_0d,dbl_op1);
            sy_0 = dbl_op1[0];
            sy_0d= dbl_op1[1];

            add12(ang_dd[0],ang_dd[1],arc_angle);

            add12(sz_0,sz_0d,dbl_op1);
            sz_0 = dbl_op1[0];
            sz_0d= dbl_op1[1];

           Flag_arc=1;
           Flag_delay=0;
           goto struct_i;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='A') )
         /* --------  command 'PA'  ------------ */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

           sx_0  =move_p[m_fin].x_b;
           sx_0d =move_p[m_fin].x_bd;
           sy_0 =move_p[m_fin].y_b;
           sy_0d =move_p[m_fin].y_bd;
           sz_0 =move_p[m_fin].z_b;
           sz_0d =move_p[m_fin].z_bd;
           sc_0 =move_p[m_fin].c_b;
           sc_0d =move_p[m_fin].c_bd;
           sb_0 =move_p[m_fin].b_b;
           sb_0d =move_p[m_fin].b_bd;

           itmp1=f_dis_setD(dsd_pr,2,5);
           if(itmp1<0) goto fin_e;

           if(itmp1==0) goto fin;
pa1d:
           for(i=0;i<itmp1;i++)
              {
                 add12(*uk_zadp1[i],*uk_zadp1d[i],dbl_op1);
                 *uk_zadp1[i] = dbl_op1[0];
                 *uk_zadp1d[i]= dbl_op1[1];
              }

     //    sx_0 = round_lp(sx_0);

//         sx_0 -=move_p[m_fin].x_b;
           dbl_op1[0]=sx_0;
           dbl_op1[1]=sx_0d;
           dbl_sub(dbl_op1,&move_p[m_fin].x_b,dbl_op1);
           sx_0 = dbl_op1[0];
           sx_0d= dbl_op1[1];

//         sy_0 -=move_p[m_fin].y_b;
           dbl_op1[0]=sy_0;
           dbl_op1[1]=sy_0d;
           dbl_sub(dbl_op1,&move_p[m_fin].y_b,dbl_op1);
           sy_0 = dbl_op1[0];
           sy_0d= dbl_op1[1];

//         sz_0 -=move_p[m_fin].z_b;
           dbl_op1[0]=sz_0;
           dbl_op1[1]=sz_0d;
           dbl_sub(dbl_op1,&move_p[m_fin].z_b,dbl_op1);
           sz_0 = dbl_op1[0];
           sz_0d= dbl_op1[1];

//         sc_0 -=move_p[m_fin].c_b;
           dbl_op1[0]=sc_0;
           dbl_op1[1]=sc_0d;
           dbl_sub(dbl_op1,&move_p[m_fin].c_b,dbl_op1);
           sc_0 = dbl_op1[0];
           sc_0d= dbl_op1[1];

//         sb_0 -=move_p[m_fin].b_b;
           dbl_op1[0]=sb_0;
           dbl_op1[1]=sb_0d;
           dbl_sub(dbl_op1,&move_p[m_fin].b_b,dbl_op1);
           sb_0 = dbl_op1[0];
           sb_0d= dbl_op1[1];

    #if(Loop_x != 0)

           if( fabs(sx_0+sx_0d) < (Per_loop * 1.5 ) )
            if( fabs(sx_0+sx_0d) > (Per_loop * 0.5))
             if( (sx_0+sx_0d) < 0)
             {
            //   sx_0 += Per_loop;
              dbl_op1[0]=sx_0;
              dbl_op1[1]=sx_0d;
              add12(Per_loop,0,dbl_op2);
              dbl_add(dbl_op1,dbl_op2,dbl_op1);
              sx_0 = dbl_op1[0];
              sx_0d= dbl_op1[1];
             }
             else
             {
            //  sx_0 -= Per_loop;
              dbl_op1[0]=sx_0;
              dbl_op1[1]=sx_0d;
              add12(Per_loop,0,dbl_op2);
              dbl_sub(dbl_op1,dbl_op2,dbl_op1);
              sx_0 = dbl_op1[0];
              sx_0d= dbl_op1[1];
             }
    #endif

    #if(Loop_c != 0)

           if( fabs(sc_0+sc_0d) < (Per_loop * 1.5 ) )
            if( fabs(sc_0+sc_0d) > (Per_loop * 0.5))
             if( (sc_0+sc_0d) < 0)
             {
            //   sc_0 += Per_loop;
              dbl_op1[0]=sc_0;
              dbl_op1[1]=sc_0d;
              add12(Per_loop,0,dbl_op2);
              dbl_add(dbl_op1,dbl_op2,dbl_op1);
              sc_0 = dbl_op1[0];
              sc_0d= dbl_op1[1];
             }
             else
             {
            //  sc_0 -= Per_loop;
              dbl_op1[0]=sc_0;
              dbl_op1[1]=sc_0d;
              add12(Per_loop,0,dbl_op2);
              dbl_sub(dbl_op1,dbl_op2,dbl_op1);
              sc_0 = dbl_op1[0];
              sc_0d= dbl_op1[1];
             }
    #endif

           for(i=NN_AX;i>=itmp1;i--)
              {
                *uk_zadp1[i]=0.;
                *uk_zadp1d[i]=0.;
              }

           Flag_arc=0;
           Flag_delay=0;
           goto struct_i;

          }
    /*-----------------------------------------------------*/
        if ( (intrpr.wrd[0]=='B')&& (intrpr.wrd[1]!='F') && (flag_DT_BR == 0) )
         /* --------  command 'B '  ------------ */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

           sx_0  =move_p[m_fin].x_b;
           sx_0d =move_p[m_fin].x_bd;
           sy_0 =move_p[m_fin].y_b;
           sy_0d =move_p[m_fin].y_bd;
           sz_0 =move_p[m_fin].z_b;
           sz_0d =move_p[m_fin].z_bd;
           sc_0 =move_p[m_fin].c_b;
           sc_0d =move_p[m_fin].c_bd;
           sb_0 =move_p[m_fin].b_b;
           sb_0d =move_p[m_fin].b_bd;

           itmp1=f_inp_b();

           if(itmp1<0) goto fin_e;
           if(itmp1==0) goto fin;
           goto pa1d;

          }
/*-----------------------------------------------------*/

//--------------------------------
//--------------------------------
/*-----------------------------------------------------*/
#endif  //(TR_DOUBLE == 0)

    /*-----------------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='L') )
         /* --------  command 'DL' ----------- */
          {
           if((itmp1= f_dis_set(&ds_dl,2,1)) <= 0) goto fin_e;

           sx_0 = delay_time *  1000. * k_t_mks/t_int ;
           delay_time=0.;
           Flag_delay=1;
           goto struct_i;
          }
    /*----------------------------------------------*/
#else
  // LOTIS != 0
/* ---------------------------------------- */
        if ( (intrpr.wrd[0]=='Q') )
         /* --------  command 'Q '  ------------ */
          {
           if(fl_trace_loop == 0)
             if(uk_buf >= (dim_xyz-11)) uk_buf=0;


           if(  (var[Warning] != 0 ) || (fl_err == 0.) || (!Flag_otr)  )
           {
              var[ Lot_tim+3 ] +=1.;
              putch('?');
              goto fin0;
           }

           if(delay_ff == 0.)
           {
               sx_0 =move_p[m_fin].x_b;
               sy_0 =move_p[m_fin].y_b;
               sz_0 =move_p[m_fin].z_b;
               sc_0 =0;
               sb_0 =0;

               f_inp_q();


               sx_0 -=  move_p[m_fin].x_b;
               sy_0 -=  move_p[m_fin].y_b;
               sz_0 -=  move_p[m_fin].z_b;
               sc_0 = 0;
               sb_0 = 0;

               Flag_arc=0;
               Flag_delay=0;

               if( (itmp=f_prep_str()) == 1 )
                 {
                  var[ Lot_tim+2 ] +=1.;
                  putch('!');
                  goto fin0;
                 }
               else if (itmp<0)
               {
                  flag_err[itmp1+N_int_ax] =(int)flag_err[itmp1+N_int_ax] | LIM_DIST;

                  var[ Lot_tim+3 ] +=1.;
                  putch('?');
                  goto fin0;
               }
                 putch('>');
                 var[ Lot_tim+4 ] +=1.;
               goto fin0;
           }
           else
           {
               sx_0 =move_ltp[ltp_fin].x_b;
               sy_0 =move_ltp[ltp_fin].y_b;
               sz_0 =move_ltp[ltp_fin].z_b;
               sc_0 =0;
               sb_0 =0;

               f_inp_q();

               sx_0 -= move_ltp[ltp_fin].x_b;
               sy_0 -= move_ltp[ltp_fin].y_b;
               sz_0 -= move_ltp[ltp_fin].z_b;
               sc_0 = 0;
               sb_0 = 0;

               Flag_arc=0;
               Flag_delay=0;

               if( (itmp=f_prep_str_ltp()) == 1 )
                 {
                   putch('!');
                   var[ Lot_tim+2 ] +=1.;
                   goto fin0;
                 }
               else if (itmp<0)
               {
                  flag_err[itmp1+N_int_ax] =(int)flag_err[itmp1+N_int_ax] | LIM_DIST;
                  putch('?');
                  var[ Lot_tim+3 ] +=1.;
                  goto fin0;
               }
                    putch('>');
                    var[ Lot_tim+4 ] +=1.;
                  goto fin0;
           }
          }
 /*-----------------------------------------------------*/
        if( var[Warning] != 0 )  goto m_disp;
        if(fl_err == 0.) goto m_disp;

        if(!Flag_otr) goto m_disp;
        if( (flag_st_d != 0.) || (flag_USB_int != 0.) || ( f_iter == f_USB_int  )  )
           // Step/Dir board connected
           // some command not performed
             goto m_disp1;
/* ---------------------------------------- */
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='R') )
         /* --------  command 'PR'  ------------ */
          {
           if(fl_trace_loop == 0)
             if(uk_buf >= (dim_xyz-11)) uk_buf=0;


           if(delay_ff == 0.)
           {

                sx_0=0.;
                sy_0=0.;
                sz_0=0.;
                sc_0=0.;
                sb_0=0.;

                itmp1=f_dis_set(ds_pr,2,5); // input parameters

                if(itmp1<0) goto fin_e;
                if(itmp1==0) goto fin;
                Flag_arc=0;
                Flag_delay=0;

      // struct_i1: for(i=0;i<NN_AX;i++)
      //             if( (Flag_sys[i] != FS) && (*uk_zadp1[i] != 0. ) ) goto fin_e;
      struct_i:
           if((itmp1=f_prep_str()) == 1 ) putch_ea('!');
           else if(itmp1 <0 )
            {
              flag_err[itmp1+N_int_ax] =(int)flag_err[itmp1+N_int_ax] | LIM_DIST;
              goto fin_e;
            }
           goto fin;
           }
           else
           {
                sx_0=0.;
                sy_0=0.;
                sz_0=0.;
                sc_0=0.;
                sb_0=0.;

                itmp1=f_dis_set(ds_pr,2,5); // input parameters

                if(itmp1<0) goto fin_e;
                if(itmp1==0) goto fin;
                Flag_arc=0;
                Flag_delay=0;

      struct_i_ltp:
                if( (itmp=f_prep_str_ltp()) ==1 ) putch('!');
                else if (itmp<0)
                 {
                   flag_err[itmp1+N_int_ax] =(int)flag_err[itmp1+N_int_ax] | LIM_DIST;
                   goto fin_e;
                 }
                goto fin;
           }

pr_bad:
           goto fin_e;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&& (intrpr.wrd[1]=='A') )
         /* --------  command 'AA'  ------------ */
          {
           if(delay_ff == 0.)
           {
            if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;
              sz_0=0.;
              arc_angle=0.;
              sx_0 =move_p[m_fin].x_b;
              sy_0 =move_p[m_fin].y_b;
              sc_0 =0;
              sb_0 =0;
              itmp1=f_dis_set(ds_aa,2,4);
              if(itmp1<3) goto fin_e;

              Flag_arc=1;
              Flag_delay=0;
              goto struct_i;
           }
           else goto fin_e;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='A') )
         /* --------  command 'PA'  ------------ */
          {
           if(fl_trace_loop == 0)
             if(uk_buf >= (dim_xyz-11)) uk_buf=0;

           if(delay_ff == 0.)
           {
              sx_0 =move_p[m_fin].x_b;
              sy_0 =move_p[m_fin].y_b;
              sz_0 =move_p[m_fin].z_b;
              sc_0 =move_p[m_fin].c_b;
              sb_0 =move_p[m_fin].b_b;

              itmp1=f_dis_set(ds_pr,2,5);
              if(itmp1<0) goto fin_e;

              if(itmp1==0) goto fin;

              sx_0 -=move_p[m_fin].x_b;
              sy_0 -=move_p[m_fin].y_b;
              sz_0 -=move_p[m_fin].z_b;
              sc_0 -=move_p[m_fin].c_b;
              sb_0 -=move_p[m_fin].b_b;

              for(i=NN_AX;i>=itmp1;i--)
              {
                *uk_zadp1[i]=0.;
                *uk_zadp1d[i]=0.;
              }

              Flag_arc=0;
              Flag_delay=0;
              goto struct_i;
           }
           else
           {
              sx_0 =move_ltp[ltp_fin].x_b;
              sy_0 =move_ltp[ltp_fin].y_b;
              sz_0 =move_ltp[ltp_fin].z_b;
              sc_0 =0;
              sb_0 =0;

              itmp1=f_dis_set(ds_pr,2,5);
              if(itmp1<0) goto fin_e;

              if(itmp1==0) goto fin;

              sx_0 -=move_ltp[ltp_fin].x_b;
              sy_0 -=move_ltp[ltp_fin].y_b;
              sz_0 -=move_ltp[ltp_fin].z_b;
              sc_0 =0;
              sb_0 =0;

              for(i=NN_AX;i>=itmp1;i--)
              {
                *uk_zadp1[i]=0.;
                *uk_zadp1d[i]=0.;
              }
              Flag_arc=0;
              Flag_delay=0;
              goto struct_i_ltp;
           }
          }
 /*-----------------------------------------------------*/
        if ( (intrpr.wrd[0]=='B')&& (intrpr.wrd[1]!='F') )
         /* --------  command 'B '  ------------ */
          {
           if(fl_trace_loop == 0)
             if(uk_buf >= (dim_xyz-11)) uk_buf=0;

           if(delay_ff == 0.)
           {
               sx_0 =move_p[m_fin].x_b;
               sy_0 =move_p[m_fin].y_b;
               sz_0 =move_p[m_fin].z_b;
               sc_0 =move_p[m_fin].c_b;
               sb_0 =move_p[m_fin].b_b;

               itmp1=f_inp_b();

               if(itmp1<0) goto fin_e;
               if(itmp1==0) goto fin;

               sx_0 -=  move_p[m_fin].x_b;
               sy_0 -=  move_p[m_fin].y_b;
               sz_0 -=  move_p[m_fin].z_b;
               sc_0 -=  move_p[m_fin].c_b;
               sb_0 -=  move_p[m_fin].b_b;

        #if(Loop_x != 0)

               if( fabs(sx_0) < (Per_loop * 1.5 ) )
                if( fabs(sx_0) > (Per_loop * 0.5))
                 if(sx_0 < 0) sx_0 += Per_loop;
                 else sx_0 -= Per_loop;
        #endif

        #if(Loop_c != 0)

               if( fabs(sc_0) < (Per_loop * 1.5 ) )
                if( fabs(sc_0) > (Per_loop * 0.5))
                 if(sc_0 < 0) sc_0 += Per_loop;
                 else sc_0 -= Per_loop;
        #endif

               Flag_arc=0;
               Flag_delay=0;

               if( (itmp=f_prep_str()) ==1 ){ putch('!'); return;}
               else if (itmp<0)
               {

                  flag_err[itmp1+N_int_ax] =(int)flag_err[itmp1+N_int_ax] | LIM_DIST;
                  putch('?');
                  return;
               }
               putch('>');
               return;
           }
           else
           {
               sx_0 =move_ltp[ltp_fin].x_b;
               sy_0 =move_ltp[ltp_fin].y_b;
               sz_0 =0;
               sc_0 =0;
               sb_0 =0;

               itmp1=f_inp_b();

               if(itmp1<0) goto fin_e;
               if(itmp1==0) goto fin;

               sx_0 -= move_ltp[ltp_fin].x_b;
               sy_0 -= move_ltp[ltp_fin].y_b;
               sz_0 = 0;
               sc_0 = 0;
               sb_0 = 0;

               Flag_arc=0;
               Flag_delay=0;

               if( (itmp=f_prep_str_ltp()) ==1 ) {putch('!');return;}
               else if (itmp<0)
               {
                  flag_err[itmp1+N_int_ax] =(int)flag_err[itmp1+N_int_ax] | LIM_DIST;
                  putch('?');
                  return;
               }
                  putch('>');
                  return;
           }
          }
 /*-----------------------------------------------------*/
 /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='L') )
         /* --------  command 'DL' ----------- */
          {
           if(delay_ff == 0.)
           {

              if((itmp1= f_dis_set(&ds_dl,2,1)) <= 0) goto fin_e;

              sx_0 = delay_time *  1000. * k_t_mks/t_int ;
              delay_time=0.;
              Flag_delay=1;
              goto struct_i;
           }
           else goto fin_e;
          }
    /*----------------------------------------------*/

#endif // (LOTIS == 0)
    /*----------------------------------------------*/

#if ( TR_INDEP != 0)
         // Набор команд генератора профиля движения для независимых осей
    /*----------------------------------------------*/
        if ((intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='P')&& (intrpr.wrd[2]=='A') )
         /* --------  command 'IPA'  ------------ */
          { // задание абсолютной координаты
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

          if( (i=f_xyzcbu(4)) >= 0 )
          {

           sx_0 =s_end_i[i];
           if(f_dis_set(ds_pr,4,1) <= 0) goto fin;

           #if(Loop_x != 0)
              if(i==0)
              {
                  ftmp= sx_0 - s_end[0];
                  if( fabs(ftmp) < (Per_loop * 1.5 ) )
                   if( fabs(ftmp) > (Per_loop * 0.5))
                    if(ftmp < 0) ftmp += Per_loop;
                    else ftmp -= Per_loop;
                  sx_0=ftmp+s_end[0];
               }
           #endif
    #if(Loop_c != 0)
          if(i==3)
          {
           ftmp= sx_0 - s_end[3];
           if( fabs(ftmp) < (Per_loop * 1.5 ) )
            if( fabs(ftmp) > (Per_loop * 0.5))
             if(ftmp < 0) ftmp += Per_loop;
             else ftmp -= Per_loop;
           sx_0=ftmp+s_end[3];
          }
    #endif
             if(fl_ind_ax[i] != 0)
             {
              itmp1= f_prep_ind(sx_0,i);
              if(itmp1<0) goto struct_i1;
             }
             goto fin;
          }

           sx_0 =s_end_i[0];
           sy_0 =s_end_i[1];
           sz_0 =s_end_i[2];
           sc_0 =s_end_i[3];
           sb_0 =s_end_i[4];

           itmp1=f_dis_set(ds_pr,3,5);
           if(itmp1<=0) goto fin;

           for(i=itmp1;i<5;i++)
           {
            *uk_zadp1[i]=s_end[i];
           }


    #if(Loop_x != 0)
           ftmp= sx_0 - s_end[0];
           if( fabs(ftmp) < (Per_loop * 1.5 ) )
            if( fabs(ftmp) > (Per_loop * 0.5))
             if(ftmp < 0) ftmp += Per_loop;
             else ftmp -= Per_loop;
           sx_0=ftmp+s_end[0];
    #endif

    #if(Loop_c != 0)

           ftmp= sc_0 - s_end[3];
           if( fabs(ftmp) < (Per_loop * 1.5 ) )
            if( fabs(ftmp) > (Per_loop * 0.5))
             if(ftmp < 0) ftmp += Per_loop;
             else ftmp -= Per_loop;
           sc_0=ftmp+s_end[3];
    #endif

//         for(i=0;i<n_ax;i++)
           for(i=0;i<5;i++)
            {
             if(fl_ind_ax[i] != 0)
             {
              itmp1= f_prep_ind(*uk_zadp1[i],i);
              if(itmp1<0) goto struct_i1;
             }
            }
            goto fin;
          }
    /*-----------------------------------------------------*/
        if ((intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='P')&& (intrpr.wrd[2]=='R') )
         /* --------  command 'IPR'  ------------ */
          {  // задание приращения координаты
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

          if( (i=f_xyzcbu(4)) >= 0 )
          {
             sx_0 =s_end[i];;
             if(f_dis_set(ds_pr,4,1) <= 0) goto fin;
             if(ind_start1[i]==0)
                sx_0 +=ss_dem_i[i];
             else
                sx_0 +=s_end[i];

             if(fl_ind_ax[i] != 0)
               {
                itmp1= f_prep_ind(sx_0,i);
                if(itmp1<0) goto struct_i1;
               }
             goto fin;
          }

           sx_0 =s_end[0]; // координаты точки принятой к исполнению
           sy_0 =s_end[1];
           sz_0 =s_end[2];
           sc_0 =s_end[3];
           sb_0 =s_end[4];

           itmp1=f_dis_set(ds_pr,3,5);
//         if(itmp1<0) goto fin_e;
           if(itmp1<=0) goto fin;

           for(i=itmp1;i<5;i++)
           {
            *uk_zadp1[i]=0.;
            *uk_zadp1d[i]=0.;
           }
          if(ind_start1[0]==0)
           sx_0 +=ss_dem_i[0];
          else
           sx_0 +=s_end[0];

          if(ind_start1[1]==0)
           sy_0 +=ss_dem_i[1];
          else
           sy_0 +=s_end[1];

          if(ind_start1[2]==0)
           sz_0 +=ss_dem_i[2];
          else
           sz_0 +=s_end[2];


          if(ind_start1[3]==0)
           sc_0 +=ss_dem_i[3];
          else
           sc_0 +=s_end[3];

          if(ind_start1[4]==0)
           sb_0 +=ss_dem_i[4];
          else
           sb_0 +=s_end[4];

//         for(i=0;i<n_ax;i++)
           for(i=0;i<5;i++)
            {
              itmp1= f_prep_ind(*uk_zadp1[i],i);
              if(itmp1<0) goto struct_i1;
            }
            goto fin;

          }
    /*-----------------------------------------------------*/
        if ((intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='R')&& (intrpr.wrd[2]=='B') )
         /* --------  command 'IRB'  ------------ */
          {  // задание приращения координаты относительно буферированного интерполятора
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

           if( (i=f_xyzcbu(4)) >= 0 )
           {
              sx_0 =s_dem_buf[i];;
              if(f_dis_set(ds_pr,4,1) <= 0) goto fin;

              sx_0 +=s_dem_buf[i];

              if(fl_ind_ax[i] != 0)
                {
                 itmp1= f_prep_ind(sx_0,i);
                 if(itmp1<0) goto struct_i1;
                }
              goto fin;
           }

           sx_0 =s_dem_buf[0]; // координаты точки в интерполяторе буферированных команд
           sy_0 =s_dem_buf[1];
           sz_0 =s_dem_buf[2];
           sc_0 =s_dem_buf[3];
           sb_0 =s_dem_buf[4];

           itmp1=f_dis_set(ds_pr,3,5);
//         if(itmp1<0) goto fin_e;
           if(itmp1<=0) goto fin;

           for(i=itmp1;i<5;i++)
           {
            *uk_zadp1[i]=0.;
            *uk_zadp1d[i]=0.;

           }
           sx_0 +=s_dem_buf[0];
           sy_0 +=s_dem_buf[1];
           sz_0 +=s_dem_buf[2];
           sc_0 +=s_dem_buf[3];
           sb_0 +=s_dem_buf[4];

//         for(i=0;i<n_ax;i++)
           for(i=0;i<5;i++)
            {
              itmp1= f_prep_ind(*uk_zadp1[i],i);
              if(itmp1<0) goto struct_i1;
            }
            goto fin;
          }
    /*-----------------------------------------------------*/
        if ((intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='G')&& (intrpr.wrd[2]=='S'))
         /* --------  command 'IGS'  ------------ */
          { // начать движение независимой оси со стробированием датчиком

          if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

          i1=2;
 igs1:
           if( (i=f_xyzcbu(2+i1)) >= 0 )
             {
              if(f_ind_start(i,i1) < 0) goto fin_e;
              goto fin;
             }

           if(f_dis_set(ds_kf,1+i1,1)<=0 ) goto fin;
           if((kk_p < 0) || (kk_p> 31)) goto fin_e;
           uk_int=(unsigned int *)&mask[1];
           itmp0=kk_p;
           for(i=0;i<5;i++)
           {
             if(itmp0 & uk_int[i])
             {
               if(f_ind_start(i,i1) < 0) goto fin_e;
             }
           }
           goto fin;
          }
    /*-----------------------------------------------------*/
        if ((intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='G'))
         /* --------  command 'IG'  ------------ */
          { // начать движение независимой оси
            if(fl_trace_loop == 0)
                if(uk_buf >= (dim_xyz-11)) uk_buf=0;
            i1=1;
            goto igs1;
          }

#endif   // ( TR_INDEP != 0)
    /*----------------------------------------------*/

    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='N') )
         /* --------  command 'PN' ------------ */
          {
           if(f_dis_set(&ds_pn,2,1) <=0) goto fin_e;
           sx_0 =delay_time ;
           sc_0 = 0;
           delay_time=0.;
           Flag_delay=2;

           goto struct_i;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='V') )
         /* --------  command 'PV' ------------ */
          {
           sx_0=0.;
           sy_0=0.;
           sz_0=-1.;
           sc_0=0.;
           if(f_dis_set(ds_pv,2,4) <= 0) goto fin_e;
           if( (sx_0 < 0) || (sx_0 > n_svar))  goto fin_e;

           if(sc_0 > 0)
                sb_0 = sc_0 *  1000. * k_t_mks/t_int ;
           else sb_0=0;

        // sx_0 = - variable number
        // sy_0 = - variable value
        // sz_0 = - variable mask
           sc_0 = 2; // flag 'PV' command
        // sb_0 = -  delay from previous 'PV' command,in interrupt pass

           delay_time=0.;
           Flag_delay=2;
           goto struct_i;
          }
    /*----------------------------------------------*/
  #if(TR_JERK == 0)
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='C')&& (intrpr.wrd[2]=='V') )
         /* --------  command 'PCV' -- Put Variable through movement buffer,don't stop smoothly movement ---- */
          {
           sx_0=0.;
           sy_0=0.;
           sz_0=-1.;
           if(f_dis_set(ds_pv,3,3) <= 0) goto fin_e;
           if( (sx_0 < 0) || (sx_0 >= n_svar))  goto fin_e;

           sb_0=0;

        // sx_0 = - variable number
        // sy_0 = - variable value
        // sz_0 = - variable mask
           sc_0 = 2; // flag 'PV' command
        // sb_0 = -  delay from previous 'PV' command,in interrupt pass
           delay_time=0.;
           Flag_delay=2;
        // goto struct_i;

           m_fin_m2=(m_fin - 2) & (sz_move-1);  /* previous segment */
           mm_fin_m2= &move_p[m_fin_m2];

           if(m_fin==m_beg) goto struct_i;
           if(mm_fin_m2->p_pcc != &fict_pp ) goto struct_i;

           if(sz_0==-1.)
           {
            mm_fin_m2->or_msk = sy_0;
            mm_fin_m2->and_msk= sy_0;
           }
           else
           {
            mm_fin_m2->or_msk= (int)sy_0  & (int)sz_0;
            mm_fin_m2->and_msk=(int)sy_0  | ~(int)sz_0;
           }

           mm_fin_m2->p_pcc=&var[(int)sx_0];

           if(m_fin==m_beg)
             if(mm_fin_m2->p_pcc == &var[(int)sx_0])
             {
                mm_fin_m2->p_pcc=&fict_pp;
                goto struct_i;
             }
           goto fin;

          }
  #endif //(TR_JERK  == 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='W')&& (intrpr.wrd[1]=='N') )
         /* --------  command 'WN'  ------------ */
          {
           if(f_dis_set(&ds_pn,2,1) <=0) goto fin_e;
           sx_0 =delay_time ;
           sc_0 = 0;           // flag WN
           delay_time=0.;
           Flag_delay=3;
           goto struct_i;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='W')&& (intrpr.wrd[1]=='I') )
         /* --------  command 'WIL','WIH'  ------------ */
          {
           if(intrpr.wrd[2]=='H')  sy_0 = 1;
           else if(intrpr.wrd[2]=='L') sy_0 = 0;
           else  goto m_wil_e;

           delay_time=0;
           sx_0 =-1;

           if( f_dis_set(ds_wil,3,2) <0 ) goto fin_e;
           if( (sx_0 < 1 ) || ( sx_0 > MAX_IK ) ) goto fin_e;
           i1= sx_0;

           sx_0 = (int)pf_ik[i1] - (int)&var[0];
           sz_0 = mskf_ik[i1];

           if(sy_0 == 0);
           else sy_0 = sz_0;

           if(delay_time > 0)
                sb_0 = delay_time *  1000. * k_t_mks/t_int ;
           else sb_0=0;

     //    sx_0                // variable number
     //    sy_0                // variable value
     //    sz_0                // variable AND mask
           sc_0 = 2;           // flag WV command
     //    sb_0                // delay from previous wait command,in interrupt pass

           Flag_arc=0;
           Flag_delay=3;
           goto struct_i;
 m_wil_e:  ftmp=ftmp;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='W')&& (intrpr.wrd[1]=='V') )
         /* --------  command 'WV'  ------------ */
          {
           sx_0=-1;
           sy_0=0;
           sz_0=-1;
           sc_0=0;

           if(  f_dis_set(&ds_pr,2,4) < 0 ) goto fin_e;
           if( (sx_0 < 0) || (sx_0 > n_svar)) goto fin_e;
           if(sc_0 > 0)
                sb_0 = sc_0 *  1000. * k_t_mks/t_int ;
           else sb_0=0;
           sc_0 = 2;
     //    sx_0                // variable number
     //    sy_0                // variable value
     //    sz_0                // variable AND mask
           sc_0 = 2;           // flag WIL/WIH
     //    sb_0                // delay from previous wait command,in interrupt pass
           delay_time=0.;
           Flag_delay=3;
           goto struct_i;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E')&& (intrpr.wrd[1]=='H')&& (intrpr.wrd[2]=='D') )
         /* --------  command 'EHD'  ------------ */
          {
           Flag_ehl1=0;
           Flag_ehl2=-1;
           Flag_ehl=-0.1;
           i_tmp = n_iter;
           while (i_tmp == n_iter)
             if(kbhit()) {getch();goto fin_e;}

           tmpf1=Flag_ehl;
           if( (itmp = f_dis_set(ds_ehl,3,3)) < 0 ) goto fin_e;
           if(itmp > 0)
            {
             Flag_ehl1=0;
             Flag_ehl2=-1;
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E')&& (intrpr.wrd[1]=='H')&& (intrpr.wrd[2]=='R') )
         /* --------  command 'EHR'  ------------ */
          {
           if(fl_trace_loop == 0)  uk_buf=0;
           tmpf1=Flag_ehl;
           if( (itmp = f_dis_set(ds_ehl,3,1)) < 0 ) goto fin_e;
           if(itmp > 0)
            {
             Flag_ehl1=0;
             Flag_ehl2=-1;
             Flag_ehl=tmpf1;

            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E')&& (intrpr.wrd[1]=='H')&& (intrpr.wdl ==2) )
         /* --------  command 'EH'  ------------ */
          {
           if(fl_trace_loop == 0)
                  uk_buf=0;
             Flag_ehl1=0;
             Flag_ehl2=-1;
             Flag_ehl= -1;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='W')&& (intrpr.wrd[1]=='I')&& (intrpr.wrd[2]=='G') )
         /* --------  command 'WIG'  ------------ */
          {
           if( f_dis_set(&ds_wig,3,1) <0 ) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
#if ( NOVS != 0)

       // выравнивание портала до заданного положения var[I_g_dem+5]
        if ( (intrpr.wrd[0]=='A')&& (intrpr.wrd[1]=='P') )
         /* --------  command 'AP'  Alignment of portal --- */

          if((State_s[0]==REP_FND)&&(State_s[5]==REP_FND))
           {
            sp_dem=*ss_dem[5];
            if((itmp1=f_dis_set(&ds_ap,2,1)) >0)
            {
             f_sensXUP=f_sensXU2;
             Statef_s[0]=90;
             Flag_sens++;
            }
            else if(itmp1<0) goto fin_e;
            goto fin;
           }
          else goto fin_e;

#endif  //( NOVS != 0)
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&& (intrpr.wrd[1]=='L') )
         /* --------  команда 'CL'  ------------ */
         /* --  CL - CaliBrate linear sensors signales  -- */
         // 'CLX','CLY','CLZ','CLC','CLU'
         /* двигатель ходит одновременно по координатам X,Y,Z
            вперед на  s_cal  мкм , затем возвращается.При этом
            накапливает MIN-MAX значения по вводимым сигналам  из АЦП .
            Затем рассчитываются   Nul[i],N_a[i]
                    AA = (Analog[0] - Nul[0] ) * N_a[0];    */
          {

  #if(TWO_MS == 0)
           if( (i=f_xyzcbu(3)) >= 0 )  // 0 - X , 1 - Y ,2 - Z, 3 - C
  #else  // (TWO_MS != 0)
           if( (i=f_xyij(3)) >= 0 )  // 0 - X , 1 - Y ,2 - I, 3 - J
  #endif
            {
             if( (i1=f_cal_adc(i)) != 0)
               if(i1 > 0)
                {
                  printf ("- Not correct SIN / COS  signales ! ");
                  goto fin_e;
                }
             goto fin;
            }

          }
    /*----------------------------------------------*/
#if(PLANAR != 0 )
        /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S') && (intrpr.wrd[1]=='L') && (intrpr.wdl==2))
         /* --------  command 'SL'  ------------ */
         /* --  SL - Set Lining base          -- */
          {
            printf(".X");
            if(f_cal_adc(0) != 0)
              {
m_sl_1:        printf ("- Not correct Hall sensor signales ! ");
               goto fin_e;
              }
            itmp=f_np(0);
            if(itmp ) { np_err_print(0,itmp); goto fin_e; }
            printf(".Y");
            if(f_cal_adc(1) != 0) goto m_sl_1;
            itmp1=f_np(1);
            if(itmp1 ) { np_err_print(1,itmp1);   goto fin_e;}
            if( f_ln_s() ) goto fin_e;
   /*
            i=2;

            for(;i<n_ax;i++)
            {
             printf(".%c",letter[i]);
             if(f_cal_adc(i) != 0) goto m_sl_1;
             itmp1=f_np(i);
             if(itmp1 )
              {
                 np_err_print(i,itmp1);
                 goto fin_e;
              }
            }
   */
           if(sl_dis == 0 ) goto m_we_00;
           goto fin;
          }
        /*----------------------------------------------*/

#endif  // (PLANAR != 0 )
    /*----------------------------------------------*/
 m_disp:
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&& (intrpr.wrd[1]=='L')&&(intrpr.wrd[2]=='S') )
          {
           // 'CLS' "CLS"
           /*--------------------------------------------------*/
           /*----Отображение / ввод параметров v_mm_s_c,t_cb --*/
           /*- v_mm_s_c  -   Скорость калибровки мм/с          */
           /*- s_cal     -   длина калибровки , мкм            */
           /*- a_m_s2_c  -   ускорение , м/с2
           //  T_INCR    - delay between 1 discrete DAC increasing
           /*--------------------------------------------------*/
           if(f_dis_set(ds_cl,3,4) <0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
#if(TWO_MS == 0)
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='G' )
         /* --------  команда 'GX','GY','GZ','GC','GB','GU' ----- */
          {
           if( (i=f_xyzcbu(2)) >= 0 )
            {

#if ( PALLET != 0)
         if(i==1) goto fin_e;
         if(i==2) goto fin_e;
         if(i==3) goto fin_e;
#endif // ( PALLET != 0)

              Flag_fst1[i]=0;
              Flag_fst2[i]=0;

   #if(PLANAR != 0)
    if(i<2)
    {
              Flag_fst1[0]=0;
              Flag_fst2[0]=0;

              Flag_fst1[1]=0;
              Flag_fst2[1]=0;
    }
   #endif  // (PLANAR != 0)

              if(intrpr.wdl==2)
                 if(f_inw()) strg=intrpr.wrd;
                 else
                  {
m_gx:
                    if( (State_s[i]==0) || (State_s[i]==REP_FND) )
                    {

//-------------------------------------
                       /* выключение пульта */
                       if(Flag_pult)
                       {
                        Flag_pult = 0;
                        f_delay_p(100000.);
                       }
//-------------------------------------

               // сброс ошибки по координате

                     flag_err[i]=0;
                     if(flag_err[0] + flag_err[1] + flag_err[2] + flag_err[3] + flag_err[4]) ;
                     else   fl_err=1.;
//---------------------------------------------------------------------
 #if(PLANAR == 0)
                     if( (Flag_sys[i] != FS) && (mskf_sens[i]==0)  )
                      {
                           Statef_s[i]=0.1; /* не проверять концевые датчики */

                           f_FS(i);        /* разрешение и замыкание  системы */

                           if( Flag_sys[i] != FS ) {Statef_s[i]=0;goto fin_e;}
                           f_delay_p(300000.);
                           if( Flag_sys[i] != FS ) {Statef_s[i]=0;goto fin_e;}

                           Statef_s[i]=1; /* начало поиска */
                           Flag_sens++;
                      }
                     else if( (Flag_sys[i] == 0)  )
                      {
                           Statef_s[i]=0.1; /* не проверять концевые датчики */

                           if((Reper_type[i]==2.)|| (Reper_type[i]==4.))
                            {
                               f_FS2(i);
                            }
                           else
                           {
                             f_FS(i);        /* разрешение и замыкание  системы */
                             if( Flag_sys[i] != FS ) {Statef_s[i]=0;goto fin_e;}
                             f_delay_p(300000.);
                             if( Flag_sys[i] != FS ) {Statef_s[i]=0;goto fin_e;}
                           }

#if ( PALLET != 0)
         if(i==0)
         {
          Statef_s[0]=0;
          Statef_s[1]=0;
          Statef_s[2]=0;
          Statef_s[3]=0;
          Statef_s[5]=1;
          Flag_sens++;
         }
         else
         {
          Statef_s[i]=1; /* начало поиска */
          Flag_sens++;
         }
#else // ( PALLET == 0)

        Statef_s[i]=1; /* начало поиска */
        Flag_sens++;

#endif // ( PALLET != 0)

                      }

//---------------------------------------------------------------------
 #else //  (PLANAR != 0)
                     if( (Flag_sys[i] != FS) && (mskf_sens[i]==0)  )
                      {
                         Statef_s[i]=0.1; /* не проверять концевые датчики */

                         Flag_axp_g[i]=1;
                         f_FS(i);        /* разрешение и замыкание  системы */

                      }
                     else if( (Flag_sys[i] == 0)  )
                      {
                           Statef_s[i]=0.1; /* не проверять концевые датчики */

                           if(Reper_type[i]==2.)
                            {
                               f_FS2(i);
                               Statef_s[i]=1; /* начало поиска */
                               Flag_sens++;
                            }
                           else
                           {
                              Flag_axp_g[i]=1;
                              f_FS(i);        /* разрешение и замыкание  системы */
                           }
                      }
 #endif  // (PLANAR != 0)
//---------------------------------------------------------------------
                     else
                      {

#if ( PALLET != 0)
         if(i==0)
         {
          Statef_s[0]=0;
          Statef_s[1]=0;
          Statef_s[2]=0;
          Statef_s[3]=0;
          Statef_s[5]=1;
          Flag_sens++;
         }
         else
         {
          Statef_s[i]=1; /* начало поиска */
          Flag_sens++;
         }
#else // ( PALLET == 0)

        Statef_s[i]=1; /* начало поиска */
        Flag_sens++;

#endif // ( PALLET != 0)
                      }

                    }
                    else goto fin_e;
                    goto fin;
                  }
              else
                  strg=&intrpr.wrd[2];

              if(f_innum(strg) )
                  {
                   if(intrpr.num_var == 0.)

#if ( PALLET == 0)
                     if( (State_s[i] != 0) && (State_s[i] != REP_FND) )
#endif  // ( PALLET == 0)

                     { /* останов поиска репера */
#if(PLANAR == 0)
                      if(i<2) flag_xy=0;
#endif  // (PLANAR != 0)


#if ( PALLET != 0)
         if((i==0)||(i==1)||(i==2) ||(i==3))
         {

          if( (State_s[0] == 0) || (State_s[0] == REP_FND) );
          else goto m1;
          if( (State_s[1] == 0) || (State_s[1] == REP_FND) );
          else goto m1;
          if( (State_s[2] == 0) || (State_s[2] == REP_FND) );
          else goto m1;
          if( (State_s[3] == 0) || (State_s[3] == REP_FND) );
          else goto m1;
          if( (State_s[5] == 0) || (State_s[5] == REP_FND) );
          else goto m1;
          goto fin;
m1:
          disable_int();
         /*
          *ss_dem[0]=s_dem_s[5];
          s_dem1[i]=*ss_dem[i];
          s_dem2[i]=*ss_dem[i];
          a_zd_1[i]=0.;
         */
          Statef_s[0]=0;
          Statef_s[1]=0;
          Statef_s[2]=0;
          Statef_s[3]=0;
          Statef_s[5]=0;
          maxc_err[0]=max_err[0];
          maxc_err[1]=max_err[1];
          maxc_err[2]=max_err[2];
          maxc_err[3]=max_err[3];
         }
         else if( (State_s[i] != 0) && (State_s[i] != REP_FND) )
         {
                      disable_int();
                      *ss_dem[i]=s_dem_s[i];
                      s_dem1[i]=*ss_dem[i];
                      s_dem2[i]=*ss_dem[i];
                      a_zd_1[i]=0.;
                      Statef_s[i]=0;
                      maxc_err[i]=max_err[i];
         }
#else  // ( PALLET == 0)
                      disable_int();
                      *ss_dem[i]=s_dem_s[i];
                      s_dem1[i]=*ss_dem[i];
                      s_dem2[i]=*ss_dem[i];
                      a_zd_1[i]=0.;
                      Statef_s[i]=0;
                      maxc_err[i]=max_err[i];

#endif // ( PALLET != 0)

#if ( MELT != 0)
      if(i==0)
      {
                      i=2;
  m_gx0:
                      *ss_dem[i]=s_dem_s[i];
                      s_dem1[i]=*ss_dem[i];
                      s_dem2[i]=*ss_dem[i];
                      a_zd_1[i]=0.;
                      Statef_s[i]=0;
                      maxc_err[i]=max_err[i];
      }
      else if(i==2)
      {
       i=0;goto m_gx0;
      }

#endif  // ( MELT != 0)

                      if(Flag_sens>0) Flag_sens--;
                   /*--------------*/
#if ( TR_INDEP != 0)

if(fl_ind_ax[i] != 0)
  {
        f_clr_ind(i);
  }
else
    f_clr_buf();
/*--------------*/
#else
  /*--------------*/
    f_clr_buf();
  /*--------------*/
#endif   // ( TR_INDEP != 0)
                   /*--------------*/
                   // enable_int();
                     }
#if ( PALLET == 0)
                     else ;
#endif  // ( PALLET == 0)

                   else goto fin_e;
                  }
              else  goto fin_e;
              goto fin;
            }
          }
    /*----------------------------------------------*/
#else    //(TWO_MS != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G')&& (intrpr.wrd[1]=='W') )
         /* --------  command 'GW' ----------------------------*/
         /*----------------------------------------------------*/
         /* ---- 'GWI','GWJ' position window define --- */
          {
             if(intrpr.wrd[2]=='P')
             {
                if(f_dis_set( ds_gwp,3,7)<0 ) goto fin_e;
                goto fin;
             }

             if( (itmp1=f_ij(3)) >= 0 )
             {
               if(flag_md != 0) goto fin_e;
               i=itmp1+2;
               if( Flag_sys[0] != FS ) goto fin_e;
               if( (A_Sin[3] < MIN_AB[3]) || (A_Sin[4] < MIN_AB[4]) || (A_Sin[5] < MIN_AB[5]) ) goto fin_e;

              if(intrpr.wdl==3)
                 if(f_inw()) strg=intrpr.wrd;
                 else
                  {
                   Statef_s[i]=101; /* начало поиска */
                   Flag_sens++;
                   goto fin;
                  }
              else
                  strg=&intrpr.wrd[2];

              if(f_innum(strg) )
                  {
                   if(intrpr.num_var == 0.)
                     if(State_s[i] != 0)
                     { /* останов процесса */

                      disable_int();
                      *ss_dem[itmp1]=s_dem_s[i];
                      s_dem1[itmp1]=*ss_dem[itmp1];
                      s_dem2[itmp1]=*ss_dem[itmp1];
                      a_zd_1[itmp1]=0.;
                      Statef_s[i]=0;
                      if(i==2) Statef_s[4]=0;
                      maxc_err[itmp1]=max_err[itmp1];

                      if(Flag_sens>0) Flag_sens--;
                   /*--------------*/
                      f_clr_buf();
                   /*--------------*/
                   // enable_int();
                     }
                     else ;
                   else goto fin_e;
                  }
              else  goto fin_e;
              goto fin;
             }
          }
    /*-----------------------------------------------*/
        if ( intrpr.wrd[0]=='G' )
         /* --------  команда 'GX','GY','GI','GJ' ----- */
          {                  // 0    1    2    3
           if( (i=f_xyij(2)) >= 0 )
            {
             if(fl_trace_loop == 0)
                 uk_buf=0;  ///

              if (i >= 2)
              {
               if(flag_md == 0) goto fin_e;
               itmp1=i-2;
              }
              else itmp1=i;

              if(intrpr.wdl==2)
                 if(f_inw()) strg=intrpr.wrd;
                 else
                  {
m_gx:
                    if( (State_s[i]==0) || (State_s[i]==REP_FND) )
                    {

//-------------------------------------
                       /* выключение пульта */
                       if(Flag_pult)
                       {
                        Flag_pult = 0;
                        f_delay_p(100000.);
                       }
//-------------------------------------

               // сброс ошибки по координате

                     flag_err[itmp1]=0;
                     if(flag_err[0] + flag_err[1] + flag_err[2] + flag_err[3] + flag_err[4]) ;
                     else   fl_err=1.;
//-------------------------------------
                     if( (Flag_sys[itmp1] != FS) && (mskf_sens[i]==0)  )
                      {
                           Statef_s[i]=0.1; /* не проверять концевые датчики */
                           f_FS(itmp1);        /* разрешение и замыкание  системы */

                           if( Flag_sys[itmp1] != FS ) {Statef_s[i]=0;goto fin_e;}
                           f_delay_p(300000.);

                           Statef_s[i]=1; /* начало поиска */
                           Flag_sens++;
                      }
                     else if( (Flag_sys[itmp1] == 0)  )
                      {
                           Statef_s[i]=0.1; /* не проверять концевые датчики */

                           if(Reper_type[i]==2.)
                            {
                               f_FS2(itmp1);
                            }
                           else
                           {
                             f_FS(itmp1);        /* разрешение и замыкание  системы */
                             if( Flag_sys[itmp1] != FS ) {Statef_s[i]=0;goto fin_e;}
                             f_delay_p(300000.);
                           }
                           Statef_s[i]=1; /* начало поиска */
                           Flag_sens++;
                      }
                     else
                      {

                         Statef_s[i]=1; /* начало поиска */
                         Flag_sens++;
                      }

                    }
                    else goto fin_e;
                    goto fin;
                  }
              else
                  strg=&intrpr.wrd[2];

              if(f_innum(strg) )
                  {
                   if(intrpr.num_var == 0.)
                     if( (State_s[i] != 0) && (State_s[i] != REP_FND) )
                     { /* останов поиска репера */

                      disable_int();
                      *ss_dem[itmp1]=s_dem_s[i];
                      s_dem1[itmp1]=*ss_dem[itmp1];
                      s_dem2[itmp1]=*ss_dem[itmp1];
                      a_zd_1[itmp1]=0.;
                      Statef_s[i]=0;
                      if(i==2) Statef_s[4]=0;
                      maxc_err[itmp1]=max_err[itmp1];

                      if(Flag_sens>0) Flag_sens--;
                   /*--------------*/
                      f_clr_buf();
                   /*--------------*/
                   // enable_int();
                     }
                     else ;
                   else goto fin_e;
                  }
              else goto fin_e;
              goto fin;
            }
          }
    /*----------------------------------------------*/

#endif  //(TWO_MS == 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='T') )
         /* --------  команда 'PT' ------------ */
          {
           if( ((int)pult_type != PULT_TTL) && ((int)pult_type != PULT_24V) )
             goto fin_e;
           if(intrpr.wdl==2)
              if(f_inw()) strg=intrpr.wrd;
              else
                {
                 if(Flag_pult)
                    printf(" %d",(int)i_vp+1);
                 else
                    printf(" 0");
                  goto fin;
                }
           else
               strg=&intrpr.wrd[2];
           if(f_innum(strg) )
            {
              if(intrpr.num_var== 0.)
               {
               /* выключение пульта */
                Flag_pult = 0;
                vp_zad[0]=0.;
                vp_zad[1]=0.;
                vp_zad[2]=0.;
                vp_zad[3]=0.;
                vp_zad[4]=0.;
                vp_zad[5]=0.;
               }
              else if(intrpr.num_var >= 1.)
               {
                 if((intrpr.num_var >= 2.)&& (intrpr.num_var <= 10.))
                   i_vp = intrpr.num_var-1.;
                 f_pult_on();
               }
                   else goto fin_e;
            }
           else  goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
m_disp1:
  /*----------------------------------------------*/
#if( TR_INDEP != 0)

         // Набор команд генератора профиля движения для независимых осей
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I')&&(intrpr.wrd[1]=='F')&&(intrpr.wrd[2]=='L')   )
         /* --------  'IFL'Independent axis FLag ---- */
          {  // флаги наличия независимых осей

            if( (i=f_xyzcbu(4)) >= 0 )
            {
             ftmp_fin[0]= fl_ind_ax[i];
             if(f_dis_set(ds_ifl,4,1)>0 )
             {
              if((int)ftmp_fin[0] != (int)fl_ind_ax[i])
               if( f_set_indp(i,(int)ftmp_fin[0]) != 0 ) goto fin_e;
             }
             goto fin;
            }

           ftmp_fin[0]= fl_ind_ax[0];
           ftmp_fin[1]= fl_ind_ax[1];
           ftmp_fin[2]= fl_ind_ax[2];
           ftmp_fin[3]= fl_ind_ax[3];
           ftmp_fin[4]= fl_ind_ax[4];

           if(f_dis_set(ds_ifl,3,5)>0 )
           {
            for(i=0;i<5;i++)
             {
              if((int)ftmp_fin[i] != (int)fl_ind_ax[i])
               if( f_set_indp(i,(int)ftmp_fin[i]) != 0 ) goto fin_e;
             }
           }
           goto fin;
          }
    /*-----------------------------------------------------*/
        if ((intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='M')&& (intrpr.wrd[2]=='V') )
         /* --------  command 'IMV'  ------------ */
          { // состояние независимых осей
            if( (i=f_xyzcb(4)) >= 0 )
            {
             sx_0 =ind_start1[i];
             f_dis_set(ds_pr,4,1);
             goto fin;
            }

             sx_0 =ind_start1[0];
             sy_0 =ind_start1[1];
             sz_0 =ind_start1[2];
             sc_0 =ind_start1[3];
             sb_0 =ind_start1[4];
             f_dis_set(ds_pr,3,5);
             goto fin;
          }
    /*----------------------------------------------*/
        if ((intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='V') )
         /* --------  command 'IV'  ------------ */
          { // скорости независимых осей
            if( (i=f_xyzcb(3)) >= 0 )
            {
             if(f_dis_set(&ds_iv[i],3,1) >0)
                f_v_az_i();
             goto fin;
            }

             if(f_dis_set(ds_iv,2,5) >0)
                f_v_az_i();
             goto fin;
          }
    /*----------------------------------------------*/
        if ((intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='A') )
         /* --------  command 'IA'  ------------ */
          { // ускорения независимых осей
            if( (i=f_xyzcb(3)) >= 0 )
            {
             if(f_dis_set(&ds_ia[i],3,1) >0)
                f_v_az_i();
             goto fin;
            }

             if(f_dis_set(ds_ia,2,5) >0)
                f_v_az_i();
             goto fin;
          }
    /*----------------------------------------------*/
    /*-----------------------------------------------------*/
        if ((intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='S')&& (intrpr.wrd[2]=='T'))
         /* --------  command 'IST'  ------------ */
          { // остановить движение независимой оси

          if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

          if( (i=f_xyzcbu(4)) >= 0 )
            {
               stop_ind[i]=1;
               goto fin;
            }

          if(f_dis_set(ds_kf,3,1)<=0 ) goto fin;
//          if((kk_p < 0) || (kk_p> 31)) goto fin_e;
          uk_int=(unsigned int *)&mask[1];
           itmp0=kk_p;
           for(i=0;i<5;i++)
           {

             if(itmp0 & uk_int[i])
             {
               stop_ind[i]=1;
             }
           }
           goto fin;
          }
    /*-----------------------------------------------------*/
    /*-----------------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='K')&&(intrpr.wrd[2]=='Q')   )
         /* --------  'MKQ'sensor number ---- */
          {  // номер сигнала стробирования старта оси
           if(f_dis_set(ds_mkq,3,5)>0 ) f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I')&&(intrpr.wrd[1]=='Q')   )
         /* --------  команда 'IQ' ------------ */
          {  // признак инверсии сигнала старта оси
           if(f_dis_set(ds_iq,2,5) > 0)
                f_inv_msk();
           goto fin;
          }
    /*-----------------------------------------------------*/


#endif   // ( TR_INDEP != 0)
    /*----------------------------------------------*/
#if ( OUT_VEL != 0)
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='A')&&(intrpr.wrd[2]=='C')&&(intrpr.wrd[3]=='V') )
         /* --------  'DACV'   ------------ */
          {
            if( (itmp1= f_dis_set(ds_dacv,4,4) ) > 0)
             {

      ftmp = 1000. * k_t_mks/t_int ;
      k_dacv = ftmp*(var[i_Uend] - var[i_U0] ) / var[i_Vend];
      DAC_vc =mas_dac[(int)N_V_DAC ]; // var[169+N_V_DAC], N_V_DAC = 1...32

//             f_cnt_dec();
//             f_cfg_cnt();
             }
            else if(itmp1<0) goto fin_e;
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='O')&&(intrpr.wrd[1]=='V') )
         /* --------  'OV'   ------------ */
          {
            f_dis_set(&ds_ov,2,1);
            goto fin;
          }
    /*----------------------------------------------*/
#endif  //( OUT_VEL != 0)
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='G' )
         /* --------  команда 'G ' -------- */
          {
           if(intrpr.wdl==1)
             if(f_inw()) goto fin_e;
             else
              {
               printf(" %d %d %d %d %d %d",State_s[0],State_s[1],State_s[2],State_s[3],State_s[4],State_s[5]);
               goto fin;
              }
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G')&& (intrpr.wrd[1]=='S') )
           /* --------  command 'GS'  ------------ */
           /*----------------------------------------------------*/
         /* ---- 'GSX','GSY','GSZ','GSC' searching start point parameters --- */
          {
             if(intrpr.wdl==2)
             {
               flag_xy=1;
               i=0;
               itmp1=0;
               goto m_gx;
             }
             if( (i=f_xyzcbu(3)) >= 0 )
             {
              if((itmp1= f_dis_set( ds_gs[i],3,6)) > 0)
                         f_cnt_dec();
              else if(itmp1<0) goto fin_e;
              goto fin;
             }
          }
    /*-----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='N') )
         /* --------  command 'DN'  ------------ */
          {
 printf(" %d %d",( (PT_PORT_OUT_c >> 18) & 0x0f),( PT_PORT_IN_s >> 24 ) & 0x0f);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&& (intrpr.wrd[1]=='K') )
         /* --------  command 'SK'  ------------ */
          {
           if(flag_err[0]+flag_err[1]+flag_err[2]+flag_err[3]+flag_err[4]+warn )
                    printf("-1");
           else if(m_fin==m_beg)
                    printf("0");
           else
                    printf("1");
           goto fin;
          }
    /*----------------------------------------------*/
#if(LOTIS == 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='B')&&(intrpr.wrd[1]=='F') )
         /* --------  команда 'BF' ------------ */
          {
           if(intrpr.wdl==2)
              if(f_inw()) strg=intrpr.wrd;
              else
                {
                 if(flag_err[0]+flag_err[1]+flag_err[2]+flag_err[3]+flag_err[4]+warn )

#if(TR_JERK != 0)
                    printf(" %d",-(  (m_fin-m_beg) & (sz_move-1)  ) );
#else
                    printf(" %d",-(  (m_fin-m_beg) & (sz_move-1)  )/2 );
#endif // (TR_JERK != 0)

                 else
#if(TR_JERK != 0)
                    printf(" %d",(  (m_fin-m_beg) & (sz_move-1)  ) );
#else
                    printf(" %d",(  (m_fin-m_beg) & (sz_move-1)  )/2 );
#endif // (TR_JERK != 0)

                  goto fin;
                }
           else
               strg=&intrpr.wrd[2];
           if(f_innum(strg) )
            if(intrpr.num_var== 0.) f_clr_buf();
            else  goto fin_e;
           else  goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
#else
    /*----------------------------------------------*/
    //  LOTIS != 0
        if ( (intrpr.wrd[0]=='B')&&(intrpr.wrd[1]=='F') )
         /* --------  command 'BF' ------------ */
          {

           if(delay_ff == 0.)
           {

               if(intrpr.wdl==2)
                  if(f_inw()) strg=intrpr.wrd;
                  else
                    {
                      printf(" %d",(  (m_fin-m_beg) & (sz_move-1)  )/2 );
                      goto fin;
                    }
               else
                   strg=&intrpr.wrd[2];
               if(f_innum(strg) )
                if(intrpr.num_var== 0.)
                {
                  disable_int();
                  fl_err=1.;
                  flag_err[0]=0;
                  flag_err[1]=0;
                  flag_err[2]=0;
                  flag_err[3]=0;
                  flag_err[4]=0;
                  f_clr_buf();    // clear moving buffer
                  enable_int();
                }
                else  goto fin_e;
               else  goto fin_e;
               goto fin;
           }
           else
           {
               if(intrpr.wdl==2)
                  if(f_inw()) strg=intrpr.wrd;
                  else
                    {

                      itmp0=  (lt_fin-lt_beg) & (sz_move_lt-1) ;
                      itmp1=  (ltp_fin-ltp_beg) & (sz_move_ltp-1);
                      printf(" %d",itmp0 + itmp1);
                 //   printf(" %d = %d + %d",itmp0 + itmp1,itmp1,itmp0);
                      goto fin;
                    }
               else
                   strg=&intrpr.wrd[2];
               if(f_innum(strg) )
                if(intrpr.num_var== 0.)
                {
                  disable_int();
                  fl_err=1.;
                  flag_err[0]=0.;
                  flag_err[1]=0.;
                  flag_err[2]=0.;
                  flag_err[3]=0.;
                  flag_err[4]=0.;
                  f_clr_buf_ltp();    // clear moving buffer
                  enable_int();
                }
                else  goto fin_e;
               else  goto fin_e;
               goto fin;
           }
          }
    /*----------------------------------------------*/

    /*----------------------------------------------*/
#endif
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&&(intrpr.wrd[1]=='A')&&(intrpr.wrd[2]=='T'))
         /* --------  команда 'SAT'  ------------ */
         /* ----  'SAT' -  ARCTG table -- */
          {
           itmp1=f_dis_set(ds_sat,3,13);
           if(itmp1>0)
            {
              puts(" - Recount -");
              disable_int();
              f_arctg();
              enable_int();
            }
            else if(itmp1<0) goto fin_e;

           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='U')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='B')&&(intrpr.wrd[3]=='S'))
         /* --------  команда 'USBS'  ------------ */
         /* ----  'USBS' -  USB State -- */
          {

              f_usb_state();
   /*
              if(usbhit00())
              {
                printf("\n Buffer0 = \n");
                usb_read0(usb_buf0,16);
              for (i=0;i<18;i++)
              printf(" %2x",usb_buf0[i]);

              }
              if(usbhit1())
              {
                printf("\n Buffer1 = \n");
                usb_read1(usb_buf1,16);
              }
              if(usbhit2())
              {
                printf("\n Buffer2 = \n");
                usb_read2(usb_buf2,16);
              }

              usbWhit0();
              usbWhit1();
              usbWhit2();
   */
           goto fin;
          }
    /*----------------------------------------------*/
#if((DEB_USB != 0) && ( USB_DEV != ISP ))

      if ( (intrpr.wrd[0]=='U')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='B')&&(intrpr.wrd[3]=='R'))
       /* --------  команда 'USBR'  ------------ */
       /* ----  'USBR' -  USB Resume -- */
        {

       //   USB Resume
         printf(" Usb Resume\n");
         USB->COM = USB_RESUME;
         f_dd();
         goto fin;
        }
  /*----------------------------------------------*/
      if ( (intrpr.wrd[0]=='U')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='B')&&(intrpr.wrd[3]=='E'))
       /* --------  команда 'USBE'  ------------ */
       /* ----  'USBE' -  USBE  -- */
        {

         //   USB Resume
           printf(" Usb Enable\n");

   f_dd();
   USB->COM = USB_SETADDR_WR;
   f_dd();
   USB->DAT = 0x80;
   f_dd();
   f_usb_setup=0;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='U')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='B')&&(intrpr.wrd[3]=='M'))
         /* --------  команда 'USBM'  ------------ */
         /* ----  'USBM' -  USBM  -- */
          {

         //   USB Mode set
           printf(" Usb Mode set\n");
//   USB Mode set

   USB->COM = USB_MODE;
   f_dd();
   USB->DAT = 0x10 | (int)(usb_mode0 * 64);
//    D7,D6 - Endpoint configuration
// 0  D5 = 0 ,reserved
// x  D4 SoftConnect
// 0  D3 - interrupt mode = 0 - only OK report
// 1  D2  internal Clock running during Suspend , 1 - always run
// 1  D1 - NoLazyclock - 1 - CLKout always run
// 0  D0 = 0 ,reserved
   f_dd();
// USB->DAT = 0x4b;
   USB->DAT = 0x0b;
// 0  D7   SOF only interrupt mode
// 1  D6   Set to one , 0 after Power on reset, must set 1 prior DMA operation
// 0  D5   reserved
// 0  D4   reserved
// b  D3...D0 Clock division for CLKout, no influence
//   f_dd();
//
           goto fin;
          }

#endif // (DEB_USB != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='R')&&(intrpr.wrd[1]=='U')&&(intrpr.wrd[2]=='U'))
         /* --------  команда 'RUU'  ------------ */
         /* ----  'RU' -  Reinit USB  -- */
          {

            if(intrpr.wdl==3)
            {

   m_ru:
              if(flag_usb != 0)
              {
                close_usb();
                f_clear_buf();
                f_usb_setup=0;
                USB_SW=0;
              }

             if(flag_usbH != 0)
              {
                  f_delay_p_nea(250000);  // mks
                  flag_usb=1;
                  init_usb();
                // задает момент времени до которого будет активен сигнал SoftConnect
                 USB_tmp1 = Ss_TIM +  (int)(TIM_SC_ON*2 *1000.* k_t_mks);

                // задает момент времени , с которого начнет проверяться SUSPEND и BUSRESET
                 USB_tmp2 = Ss_TIM +  (int)(TIM_SC_ON * 1000.* k_t_mks);

              }
              goto fin;
            }
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='U')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='B'))
         /* --------  команда 'USB'  ------------ */
         /* ----  'USB' -  USB on/off -- */
          {
          //---- Read addr  --------------
            f_dd();
            USB->COM = USB_SETADDR_RD;
            f_dd();
            usb_adr=USB->DAT;
            f_dd();
            usb_adr_f = usb_adr & 0x7f;
          //------------------------------

           itmp1=f_dis_set(ds_usb,3,5);
           if(itmp1>0)
            {
              flag_usb = (int)flag_usb & flag_usbH;
              if(flag_usb != 0)
              {

                if(flag_usbH != 0)
                 {
                   flag_usb=1;
                   goto m_ru;
                   init_usb();        // endpoint 1,2 are generic
                 }


//              init_usb();        // endpoint 1,2 are generic

              }
              else
              {
                close_usb();
                f_clear_buf();
                f_usb_setup=0;
                USB_SW=0;
              }
            }
           else
           {


#if(DEB_USB != 0)
   f_dd();
   USB->COM = USB_SET_DMA_RD;
   f_dd();
   itmp=USB->DAT& 0xffff;

             printf(" SW=%d,DMA=%2x ",USB_SW,itmp&0xff);
#endif
//           printf(" SW=%d",USB_SW);
           }

           if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
         /* --------  команда 'OUSB'  ------------ */
         /* ----  'OUSB' - out to  USB  -- */
/*
        if ( (intrpr.wrd[0]=='O')&& (intrpr.wrd[1]=='U')&&(intrpr.wrd[2]=='S')&&(intrpr.wrd[3]=='B'))
          {
           if(f_dis_set(ds_ousb,4,3)<0) goto fin_e;
           else
            {
             if(usb_p1 !=0 )
             {
                disable_int();
                usb_write0(usb_buf0,(int)usb_p1);
                enable_int();
             }
             if(usb_p2 !=0 )
             {
                disable_int();
                usb_write1(usb_buf1w,(int)usb_p2);
                enable_int();
             }
             if(usb_p3 !=0 )
             {
                disable_int();
                usb_write2(usb_buf2,(int)usb_p3);
                enable_int();
             }

            }
           goto fin;
          }
*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='N')&&(intrpr.wrd[2]=='N'))
         /*  -----   command  'CNN' ------------ */
          {
           ftmp1=Contr_NN;
           if(f_dis_set(&ds_cnn,3,1) <0) goto fin_e;
           if(ftmp1 != Contr_NN) goto m_ru;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='A')&&(intrpr.wrd[2]=='X'))
         /*  -----   command  'NAX' ------------ */
          {
           if((itmp1=f_dis_set(&ds_nax,3,1)) > 0)
             {
               f_cfg_count();
           disable_int();
               f_arctg();
           enable_int();
             }
           else if(itmp1<0) goto fin_e;

#if ( TR_INDEP != 0)
  if(f_init_ind()<0) goto fin_e;
#endif // ( TR_INDEP != 0)

           goto fin;
          }
    /*----------------------------------------------*/
#if ( TR_INDEP != 0)
         // Набор команд генератора профиля движения для независимых осей



#endif   // ( TR_INDEP != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='A'))
          { // 'NA'
           if(f_dis_set(ds_na,2,12) < 0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='F'))
          {        //'NF'
           if(f_dis_set(ds_nf,2,12) < 0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
#if ( TR_INDEP != 0)
         // Набор команд генератора профиля движения для независимых осей
        if ( (intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='K')&& (intrpr.wrd[2]=='V') )
         /* --------  command 'IKV'  ------------ */
          {
           if( f_dis_set(&ds_ikv,3,1) <0 ) goto fin_e;
           goto fin;
          }
#endif   // ( TR_INDEP != 0)
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I')&& (intrpr.wrd[1]=='K') )
         /* --------  command 'IK'  ------------ */
         /* ---  IK - Input chanel   ---- */
          {
           if( (int)*p_ik[n_k1] & msk_ik[n_k1]  ) i = 1;
           else i=0;

           if(intrpr.wdl==2)
              if(f_inw()==0)
               {
                printf(" %d %d",n_k1,i);
                goto fin;
               }
           kk_p= n_k1;
           intrpr.i3=0;
           f_inw();
           if( f_dis_set( ds_kf,2,1) != 1) goto fin_e;
           if( (kk_p > MAX_IK) || (kk_p < 1)) goto fin_e;
//------------------------------------
//------------------------------------
           n_k1 = kk_p;

           if( (int)*p_ik[n_k1] & msk_ik[n_k1]  ) i= 1;
           else i=0;
           printf(" %d",i);
           goto fin;
          }
    /*----------------------------------------------*/
#if defined(TR_SMOOTH)
    /*----------------------------------------------*/
        if( (intrpr.wrd[0]=='P') && (intrpr.wrd[1]=='C')&& (intrpr.wrd[2]=='C') )
         /* --------  command 'PCC'  ------------ */
         /* ---  PCC - Put chanel through movement buffer,don't stop smoothly movement ---- */
          {
           kk_p= n_k;
           if( (int)*p_pk[n_k] & msk_pk[n_k]) i= 1;
           else i=0;

           if(intrpr.wdl==3)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
                printf(" %d %d",n_k,i);
                goto fin;
               }
           else
              strg=&intrpr.wrd[3];
           if(f_innum(strg) )
            {
             kk_p=intrpr.num_var;
             if( (kk_p > MAX_PK) || (kk_p < 1.)) goto fin_e;
             n_k = kk_p;
             if( (int)*p_pk[n_k] & msk_pk[n_k]) i= 1;
             else i=0;
            }
           else goto pr_bad;

           if(f_inw())
             if(f_innum(intrpr.wrd) )
               {

  m_fin_m2=(m_fin - 2) & (sz_move-1);  /* previous segment           */
  mm_fin_m2= &move_p[m_fin_m2];
               tmp1=kk_p;

                kk_i=intrpr.num_var;

                if(kk_i==1.)
                {
                  sx_0 = kk_p+100.;
                  sy_0 = 1.;
                }
                else if (kk_i==0.)
                {
                  sx_0=kk_p;
                  sy_0 = 1.;
                }
                else goto fin_e;


                if(m_fin==m_beg) goto pcc1;
                if(mm_fin_m2->p_pcc != &fict_pp ) goto pcc1;

                m_fin_m2 = (m_fin - 2) & (sz_move-1);  /* previous segment */
                mm_fin_m2= &move_p[m_fin_m2];

                if(kk_i==1.)
                {
                  mm_fin_m2->or_msk=msk_pk[tmp1];
                  mm_fin_m2->and_msk=0xffffffff;
                }
                else if (kk_i==0.)
                {
                  mm_fin_m2->or_msk=0;
                  mm_fin_m2->and_msk=~msk_pk[tmp1];
                }

                mm_fin_m2->p_pcc=p_pk[tmp1];

                if(m_fin==m_beg)
                  if(mm_fin_m2->p_pcc == p_pk[tmp1])
                  {
                     mm_fin_m2->p_pcc=&fict_pp;
                     goto pcc1;
                  }

                goto fin;
pcc1:
                Flag_delay=4;
                goto struct_i;
               }
             else goto pr_bad;
           else
            {
              if((int) *p_pk[n_k] & msk_pk[n_k] ) i=1;
              else i=0;
              printf(" %d",i);
            }
           goto fin;
          }

    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if( (intrpr.wrd[0]=='P') && (intrpr.wrd[1]=='C')&& (intrpr.wrd[2]=='D') )
         /* --------  command 'PCD'  ------------ */
         /* ---  PCD - Put Chanel DAC through movement buffer,don't stop smoothly movement ---- */
          {
           kk_p = n_k_dac;
           i = *mas_dac[n_k_dac];

           if(intrpr.wdl==3)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
                printf(" %d %d",n_k_dac,i);
                goto fin;
               }
           else
              strg=&intrpr.wrd[3];

           if(f_innum(strg) )
            {
             kk_p=intrpr.num_var;
             if( (kk_p > NN_DAC) || (kk_p < 1.)) goto fin_e;
             n_k_dac = kk_p;
             i = *mas_dac[n_k_dac];
            }
           else goto pr_bad;

           if(f_inw())
             if(f_innum(intrpr.wrd) )
               {
                kk_i=intrpr.num_var;

                if(kk_i > 32767) kk_i=32767;
                else if(kk_i < -32767)  kk_i=-32767;

                sx_0 = kk_p;
                sy_0 = 2.;
                sz_0 = kk_i;
        //      Flag_delay=4;
                goto pca1;
               }
             else goto pr_bad;
           else
            {
              printf(" %d",i);
            }
           goto fin;
pca1:
           m_fin_m2=(m_fin - 2) & (sz_move-1);  /* previous segment */
           mm_fin_m2= &move_p[m_fin_m2];

           if(m_fin==m_beg) goto pcc1;
           if(mm_fin_m2->p_pcc != &fict_pp ) goto pcc1;

           mm_fin_m2->or_msk=kk_i;
           mm_fin_m2->and_msk=kk_i;
           mm_fin_m2->p_pcc=mas_dac[n_k_dac];

           if(m_fin==m_beg)
             if(mm_fin_m2->p_pcc == mas_dac[n_k_dac])
             {
                mm_fin_m2->p_pcc=&fict_pp;
                goto pcc1;
             }
           goto fin;
          }
    /*----------------------------------------------*/
#endif  // !defined(TR_SMOOTH)


    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='C')&& (intrpr.wrd[2]=='A') )
         /* --------  command 'PCA'  ------------ */
         /* ---  PCA - Put DAC chanel through movement buffer  ---- */
          {
           kk_p = n_k_dac;
           i = *mas_dac[n_k_dac];

           if(intrpr.wdl==3)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
                printf(" %d %d",n_k_dac,i);
                goto fin;
               }
           else
              strg=&intrpr.wrd[3];
           if(f_innum(strg) )
            {
             kk_p=intrpr.num_var;
             if( (kk_p > NN_DAC) || (kk_p < 1.)) goto fin_e;
             n_k_dac = kk_p;
             i = *mas_dac[n_k_dac];
            }
           else goto pr_bad;

           if(f_inw())
             if(f_innum(intrpr.wrd) )
               {
                kk_i=intrpr.num_var;

                if(kk_i > 32767) kk_i=32767;
                else if(kk_i < -32767)  kk_i=-32767;

                sx_0 = kk_p;
                sy_0 = 2.;
                sz_0 = kk_i;
                Flag_delay=4;
                goto struct_i;
               }
             else goto pr_bad;
           else
            {
              printf(" %d",i);
            }
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&& (intrpr.wrd[1]=='C') )
         /* --------  command 'PC'  ------------ */
         /* ---  PC - Put chanel through movement buffer  ---- */
          {
           kk_p= n_k;
           if( (int)*p_pk[n_k] & msk_pk[n_k]) i= 1;
           else i=0;

           if(intrpr.wdl==2)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
                printf(" %d %d",n_k,i);
                goto fin;
               }
           else
              strg=&intrpr.wrd[2];
           if(f_innum(strg) )
            {
             kk_p=intrpr.num_var;
             if( (kk_p > MAX_PK) || (kk_p < 1.)) goto fin_e;
             n_k = kk_p;
             if( (int)*p_pk[n_k] & msk_pk[n_k]) i= 1;
             else i=0;
            }
           else goto pr_bad;

           if(f_inw())
             if(f_innum(intrpr.wrd) )
               {
                kk_i=intrpr.num_var;

                if(kk_i==1.)  sx_0 = kk_p+100.;
                else if (kk_i==0.) sx_0=kk_p;
                else goto fin_e;

                sy_0 = 1.;
                Flag_delay=4;
                goto struct_i;
               }
             else goto pr_bad;
           else
            {
              if((int) *p_pk[n_k] & msk_pk[n_k] ) i=1;
              else i=0;
              printf(" %d",i);
            }
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F') && (intrpr.wrd[1]=='I') && (intrpr.wrd[2]=='N'))
         /* --------  команда 'FIN' - Flag independent movement ----- */
          {
           f_dis_set(&ds_fin,3,1);
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
#if(CALA_AI  != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A') && (intrpr.wrd[1]=='I') && (intrpr.wrd[2]=='N'))
         /* --------  команда 'AIN' - CALA_ADC chan number  -------- */
          {
           if( f_dis_set(&ds_ain,3,1) > 0  )
            {
               cala_nc=(int)(cala_ncf) << 4;     //  0x0  - AN1
                                                 //  0x10 - AN2
               cala_ncn=cala_nc>>4;
               CALA_ADC_Chn = cala_nc;

            }
           goto fin;
          }
    /*----------------------------------------------*/
#endif  // (CALA_AI  != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I') && (intrpr.wrd[1]=='O') )
    /* --------  command 'IO' ------------ */
    /*  set IO board 0,1 present ,noise time for IO boards ,ms */
          {
           if(io_p0==1) io_p0 = 16;
           if(io_p1==1) io_p1 = 16;
           if( (itmp1=f_dis_set(ds_io,2,4)) > 0  )
           {
            itmp1=0;
            if(io_p0==1) io_p0 = 16;
            if(io_p1==1) io_p1 = 16;

            if( (io_p0 == 0) || (io_p0 == 8) || (io_p0 == 16)) ;
            else {io_p0 = 0; itmp1=1; }

            if( (io_p1 == 0) || (io_p1 == 8) || (io_p0 == 16) ) ;
            else goto er_iop1;

       //   if( (io_p0 == 16) && (io_p1 == 8) ) goto er_iop1;

            if( (io_p0 == 8) && (io_p1 == 16) )
             {
   er_iop1:
               io_p1 = 0;
               itmp1=1;
             }

//          if( (io_p0 == 16) && (io_p2 == 8) )  goto er_iop2;
            if( (io_p2 == 0) || (io_p2 == 8) ) ;
            else
             {
   er_iop2:
               io_p2 = 0;
               itmp1=1;
             }

            if( (io_p0 == 16) && (io_p3 == 8) )  goto er_iop3;
            if( (io_p3 == 0) || (io_p3 == 8) ) ;
            else {
   er_iop3:        io_p3 = 0;
                   itmp1=1;
                 }

              f_cfg_count();
              if(itmp1) goto fin_e;
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P') && (intrpr.wrd[1]=='K') )
         /* --------  command 'PK'  ------------ */
         /* ---  PK - Put chanel directly  ---- */
          {
           kk_p= n_k;
           if( (int)*p_pk[n_k] & msk_pk[n_k]) i= 1;
           else i=0;

           if(intrpr.wdl==2)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
                printf(" %d %d",n_k,i);
                goto fin;
               }
           else
              strg=&intrpr.wrd[2];
           if(f_innum(strg) )
            {
             kk_p=intrpr.num_var;
             if( (kk_p > MAX_PK) || (kk_p < 1.)) goto fin_e;
             n_k = kk_p;
             }
           else goto pr_bad;

           if(f_inw())
             if(f_innum(intrpr.wrd) )
               {
                kk_i=intrpr.num_var;

                if(kk_i==1.)       *p_pk[n_k] = (int)*p_pk[n_k] |  msk_pk[n_k];
                else if (kk_i==0.) *p_pk[n_k] = (int)*p_pk[n_k] & ~msk_pk[n_k];
                else goto fin_e;

                goto fin;
               }
             else goto pr_bad;
           else
            {
              if( (int)*p_pk[n_k] & msk_pk[n_k] ) i=1;
              else i=0;
              printf(" %d",i);
            }
           goto fin;
          }
/*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='S'))
         /* --------  команда 'CS'  ------------ */
         /* ----  Coordinate system  -- */
          {
           if(  ( (m_fin-m_beg) & (sz_move-1) ) > 0 )  goto fin_e;
           itmp1=f_dis_set(&ds_cs,2,1);
           if(itmp1>0)
            {
             itmp1=(int)sc_f;
             mas_cr[0]=mas_sr_c[itmp1][0];
             mas_cr[1]=mas_sr_c[itmp1][1];
             mas_cr[2]=mas_sr_c[itmp1][2];
             s_uk0=&s_dem[mas_cr[0]];
             s_uk1=&s_dem[mas_cr[1]];
             s_uk2=&s_dem[mas_cr[2]];
             s_uk0d=&s_demd[mas_cr[0]];
             s_uk1d=&s_demd[mas_cr[1]];
             s_uk2d=&s_demd[mas_cr[2]];
            }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='X')&&(intrpr.wrd[1]=='X'))
         /* --------  команда 'XX'  ------------ */
          {
            for(i=0;i<n_ax;i++)
              f_stop_fx(i);

           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='X')&&(intrpr.wrd[1]=='Y'))
         /* --------  команда 'XY'  ------------ */
          {
           if(f_dis_set(ds_xx,2,4)>0)
            {
              Reg1_X=Reg_X;
              Step_X_1 = 1./ Step_X;
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='Y')&&(intrpr.wrd[1]=='Y'))
         /* --------  команда 'YY'  ------------ */
          {
           if((itmp1=f_dis_set(ds_yy,2,4)) >0)
            {
              Reg1_Y=Reg_Y;
              Step_Y_1 = 1./ Step_Y;
            }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&& (intrpr.wrd[1]=='F') )
         /* --------  command 'SF'  ------------ */
         /* --  SF - Sin wave garmonic Factors -- */
            {
             if( (i=f_0123456(3)) < 0 ) goto fin_e;
             gg1=&g1[0];
             gg2=&g2[0];
             gg3=&g3[0];
             gg4=&g4[0];
             gg5=&g5[0];
             gg6=&g6[0];
             gg7=&g7[0];
             goto m_sf;
            }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&& (intrpr.wrd[1]=='N') )
         /* --------  command 'SN'  ------------ */
         /* --  SN - Sin wave garmonic phases -- */
            {

             if( (i=f_0123456(3)) < 0 ) goto fin_e;
             gg1=&f1[0];
             gg2=&f2[0];
             gg3=&f3[0];
             gg4=&f4[0];
             gg5=&f5[0];
             gg6=&f6[0];
             gg7=&f7[0];
             goto m_sf;
            }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&& (intrpr.wrd[1]=='F') )
         /* --------  command 'CF'  ------------ */
         /* --  CF - Cos wave garmonic Factors -- */
            {

             if( (i=f_0123456(3)) < 0 ) goto fin_e;
             gg1=&cg1[0];
             gg2=&cg2[0];
             gg3=&cg3[0];
             gg4=&cg4[0];
             gg5=&cg5[0];
             gg6=&cg6[0];
             gg7=&cg7[0];
             goto m_sf;
            }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&& (intrpr.wrd[1]=='N') )
         /* --------  command 'CN'  ------------ */
         /* --  CN - Cos wave garmonic phases -- */
            {

           if( (i=f_0123456(3)) < 0 ) goto fin_e;
             gg1=&cf1[0];
             gg2=&cf2[0];
             gg3=&cf3[0];
             gg4=&cf4[0];
             gg5=&cf5[0];
             gg6=&cf6[0];
             gg7=&cf7[0];
  m_sf:
              kk_p= gg1[i] ;
              kk_i= gg2[i] ;
              kk_d= gg3[i] ;
              kkat= gg4[i] ;
              kk_f= gg5[i] ;
             tmpf1= gg6[i] ;
             tmpf2= gg7[i] ;
             if( (itmp1=f_dis_set(ds_kf,3,7)) == 0 ) goto fin;
             else if(itmp1<0) goto fin_e;

             gg1[i] =  kk_p;
             gg2[i] =  kk_i;
             gg3[i] =  kk_d;
             gg4[i] =  kkat;
             gg5[i] =  kk_f;
             gg6[i] = tmpf1;
             gg7[i] = tmpf2;

             if(i==0) f_tabl();
             else
                   f_arctg();
             goto fin;
            }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='D')&&( (intrpr.wrd[2]==' ')||(intrpr.wrd[2]== 0 )  ) )
         /*  -----   command  'CD'  */
          {
           kk_p= cos_err[0];
           kk_i= cos_err[1];
           kk_d= cos_err[2];
           kkat= cos_err[3];
           kk_f= cos_err[4];
           ftmp=cos_err[5];

           if( (itmp1=f_dis_set(ds_cd,2,6)) == 0 ) goto fin;
           else if(itmp1<0) goto fin_e;

           disable_int();
           f_arctg();
           enable_int();

           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='D'))
         /* --------  command 'CD'  ------------ */
          {
           if( (itmp0=f_123456(3)) < 0 ) goto fin_e;

           itmp1=f_dis_set(&ds_cd[itmp0],3,1);
           if(itmp1==1 )
              {
               disable_int();
               f_arctg();
               enable_int();
              }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='Z')&&(intrpr.wrd[2]=='E'))
         /* --------  command 'CZE'  ------------ */
          {

           kk_p= flag_cze;
           if(f_dis_set(ds_kf,3,1)<0) goto fin_e;
            flag_cze = kk_p;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='Z')&&(intrpr.wrd[2]=='F'))
         /* --------  command 'CZF'  ------------ */
          {
           if((itmp1=f_dis_set(ds_czf,3,6)) > 0)  f_cfg_cz();
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='Z')&&(intrpr.wrd[2]=='R'))
         /* --------  command 'CZR'  ------------ */
          {
           if(f_dis_set(&ds_czr,3,1) > 0)  f_cfg_cz();
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='Z')&&(intrpr.wrd[2]=='D'))
         /* --------  command 'CZD'  ------------ */
          {

           kk_p= flag_czd;
           if(f_dis_set(ds_kf,3,1)>0)
            flag_czd = kk_p;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
         /* --------  command 'CZC'  ------------ */
         /* --------   Test doubled.asm           */
/*
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='Z')&&(intrpr.wrd[2]=='C'))

          {

           kk_p = ddd[0];
           kk_i = ddd[1];
           kk_d = ddd[2];
           kkat = ddd[3];
           kk_f = ddd[4];
           tmpf1=ddd[5];
           tmpf2=ddd[6];

            i1=get_sp();
            printf("\n SP = %8x\n",i1);

            if(f_dis_set(ds_kf,3,7)>0)
             {
                  ddd[0]=kk_p ;
                  ddd[1]=kk_i ;
                  ddd[2]=kk_d ;
                  ddd[3]=kkat ;
                  ddd[4]=kk_f ;
                  ddd[5]=tmpf1;
                  ddd[6]=tmpf2;

                  if(tmpf2==1.)
                  {
                   printf("   add12() \n");
                   ddd[0] = kk_p;
                   ddd[1] = kk_i;

                   add12(ddd[0],ddd[1],&ddd[2]);

                   printf("\n %18.10f+%18.10f = %18.10f:%18.10f;  \n", ddd[0],ddd[1],ddd[2],ddd[3] );

                  }
                  else if(tmpf2==2.)
                  {
                   printf("   dbl_add() \n");


                    add12(ddd[0],ddd[1],&ddt[0]);
                    add12(ddd[2],ddd[3],&ddt[2]);

                    dbl_add(&ddt[0],&ddt[2],&ddd[4]);

                   printf("\n %18.10f:%18.10f + %18.10f:%18.10f\n = %18.10f:%18.10f \n", ddd[0],ddd[1],ddd[2],ddd[3],ddd[4],ddd[5] );

                  }
                  else if(tmpf2==4.)
                  {
                   printf("   mul12() \n");


                    mul12(ddd[0],ddd[1],&ddd[4]);
                   printf("\n %18.10f * %18.10f = %18.10f: %18.10f \n", ddd[0],ddd[1],ddd[4],ddd[5] );

              // в ddd[4,5] - результат

                    ddt[0]=-(int)ddd[4];
                    ddt[1]=0;
                    add12(ddt[0],0.,&ddt[0]);

                    dbl_add(&ddd[4],&ddt[0],&ddt[2]);

                    ddt[0]=-ddt[0];
           //       ddt[0] - целая часть
           //       ddt[2] - дробная часть

                   ddt[2]+=ddt[3];
                   if((ddt[2] < 0.)&& (ddt[0]>0) ) {ddt[2]+=1.;ddt[0]-=1.;}
                   else if((ddt[2] > 0.)&& (ddt[0]<0) ) {ddt[2]-=1.;ddt[0]+=1.;}

                   printf("\n = %7f.%8.7f \n",ddt[0],ddt[2]);

                  }
                  else if(tmpf2==5.)
                  {
                   printf("   dbldiv() \n");

                    add12(ddd[0],ddd[1],&ddt[0]);
                    add12(ddd[2],ddd[3],&ddt[2]);

                    dbl_div(&ddt[0],&ddt[2],&ddd[4]);
                   printf("\n %18.10f:%18.10f / %18.10f:%18.10f\n = %18.10f:%18.10f \n", ddt[0],ddt[1],ddt[2],ddt[3],ddd[4],ddd[5] );

              // в ddd[4,5] - результат

                    ddt[0]=-(int)ddd[4];
                    ddt[1]=0;
                    add12(ddt[0],0.,&ddt[0]);

                    dbl_add(&ddd[4],&ddt[0],&ddt[2]);

                    ddt[0]=-ddt[0];
           //       ddt[0] - целая часть
           //       ddt[2] - дробная часть

                   ddt[2]+=ddt[3];
                   if((ddt[2] < 0.)&& (ddt[0]>0) ) {ddt[2]+=1.;ddt[0]-=1.;}
                   else if((ddt[2] > 0.)&& (ddt[0]<0) ) {ddt[2]-=1.;ddt[0]+=1.;}

                   printf("\n %18.10f:%18.10f / %18.10f:%18.10f\n = %7f.%8.7f \n", ddd[0],ddd[1],ddd[2],ddd[3],ddt[0],ddt[2]);

                  }
                  else if(tmpf2==6.)
                  {
                   printf("   dbl mult C special \n");



                    dbl_one[0]=1.;
                    dbl_two[0]=1000000.;

         min_t2=0x7fffffff;max_t2=0;
         f_tim2();

                    dbl_div(dbl_one,dbl_two,dbl_ppm);
         f_tim3();
         printf("\n dbl_div()");
         print_t2();


                    dbl_one[0]=ddd[1];

         f_tim2();
                    dbl_mul(dbl_one,dbl_ppm,dbl_op1);
         f_tim3();
         printf("\n dbl_mul()");
         print_t2();

                    dbl_one[0]=ddd[0];

         f_tim2();
                    dbl_add(dbl_one,dbl_op1,dbl_op1);   // dbl_op1 = p1 + p2/1000000.
         f_tim3();
         printf("\n dbl_add()");
         print_t2();

                    dbl_one[0]=ddd[3];
                    dbl_mul(dbl_one,dbl_ppm,dbl_op2);
                    dbl_one[0]=ddd[2];
                    dbl_add(dbl_one,dbl_op2,dbl_op2);   // dbl_op2 = p3 + p4/1000000.



                    dbl_mul(dbl_op1,dbl_op2,&ddd[4]);

              // в ddd[4,5] - результат

                    ddt[0]=-(int)ddd[4];
                    ddt[1]=0;
                    add12(ddt[0],0.,&ddt[0]);

                    dbl_add(&ddd[4],&ddt[0],&ddt[2]);

                    ddt[0]=-ddt[0];
           //       ddt[0] - целая часть
           //       ddt[2] - дробная часть

                   if((ddt[2] < 0.)&& (ddt[0]>0) ) {ddt[2]+=1.;ddt[0]-=1.;}
                   else if((ddt[2] > 0.)&& (ddt[0]<0) ) {ddt[2]-=1.;ddt[0]+=1.;}

                   printf("\n %f:%6f * %f:%6f\n = %7f.%8.7f \n", ddd[0],ddd[1],ddd[2],ddd[3],ddt[0],ddt[2]);
                   printf(" ");
                   print_dbl(dbl_op1);
                   printf(" * ");
                   print_dbl(dbl_op2);
                   printf(" = ");
                   print_dbl(&ddd[4]);
                   printf("\n");

                  }
             }
             else
             {
               printf("\np7=\n 1 - add12(): p1=x,p2=y,p7=1\n");
               printf(" 2 - dbl_add(): p1=x,p2=xx,p3=y,p4=yy,p5=z,p6=zz,p7=2\n");
               printf(" 4 - mul12(): p1=x,p2=y,p5=z,p6=zz,p7=4\n");
               printf(" 5 - dbldiv(): p1=x,p2=y,p5=z,p6=zz,p7=5\n");
               printf(" 6 - dbl mult  : p1=x,p2=y,p5=z,p6=zz,p7=5\n");
             }
           goto fin;
          }
*/
    /*----------------------------------------------*/
    /*----------------------------------------------*/
         /* --------  command 'CZV'  ------------ */
         /* --------   Test doubled.asm           */

        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='Z')&&(intrpr.wrd[2]=='V'))

          {
            f_dis_setD(dsd_tst,3,2) ;


         f_tim2();
            add12(dd_par1[0],dd_par1[1],dbl_op1);
         f_tim3();
         printf("\n add12()");
         print_t2();

            printf("\n ");
            print_dbl(dd_par1);
            printf(" = ");
            print_dbl(dbl_op1);

         f_tim2();
            mul12(dd_par1[0],dd_par1[1],dbl_op1);
         f_tim3();
         printf("\n mul12()");
         print_t2();

            printf("\n ");
            printf("float  %f" ,dd_par1[0]);
            printf(" * ");
            printf("float %f",dd_par1[1]);
            printf(" = ");
            print_dbl(dbl_op1);


         f_tim2();
            dbl_add(dd_par1,dd_par2,dbl_op1);
         f_tim3();
         printf("\n dbl_add()");
         print_t2();

            printf("\n ");
            print_dbl(dd_par1);
            printf(" + ");
            print_dbl(dd_par2);
            printf(" = ");
            print_dbl(dbl_op1);

         f_tim2();
            dbl_sub(dd_par1,dd_par2,dbl_op1);
         f_tim3();
         printf("\n dbl_sub()");
         print_t2();

            printf("\n ");
            print_dbl(dd_par1);
            printf(" - ");
            print_dbl(dd_par2);
            printf(" = ");
            print_dbl(dbl_op1);


         f_tim2();
            dbl_mul(dd_par1,dd_par2,dbl_op1);
         f_tim3();
         printf("\n dbl_mul()");
         print_t2();

            printf("\n ");
            print_dbl(dd_par1);
            printf(" * ");
            print_dbl(dd_par2);
            printf(" = ");
            print_dbl(dbl_op1);

         f_tim2();
            dbl_div(dd_par1,dd_par2,dbl_op1);
         f_tim3();
         printf("\n dbl_div()");
         print_t2();

            printf("\n ");
            print_dbl(dd_par1);
            printf(" / ");
            print_dbl(dd_par2);
            printf(" = ");
            print_dbl(dbl_op1);



            goto fin;

          }

    /*----------------------------------------------*/
    /*----------------------------------------------*/
#define PR_eth 1
#if((PR_eth != 0)&& ( ETHER !=0))

         /* --------  command 'CZC'  ------------ */
         /* --------   Test of MAC value   */

        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='Z')&&(intrpr.wrd[2]=='C'))
          {
//---------------------------------------
   printf("\nMac  MACAddress.ByteD = %3d %3d %3d %3d %3d %3d",
   MAC.Byte.MAC0,
   MAC.Byte.MAC1,
   MAC.Byte.MAC2,
   MAC.Byte.MAC3,
   MAC.Byte.MAC4,
   MAC.Byte.MAC5);

  COPY_MAC(RemoteMAC,MAC);

   printf("\nRemote MACAddress.ByteX = %2x %2x %2x %2x %2x %2x",
   RemoteMAC.Byte.MAC0 & 0xff,
   RemoteMAC.Byte.MAC1 & 0xff,
   RemoteMAC.Byte.MAC2 & 0xff,
   RemoteMAC.Byte.MAC3 & 0xff,
   RemoteMAC.Byte.MAC4 & 0xff,
   RemoteMAC.Byte.MAC5 & 0xff);
/*
   printf("\n MACAddress.WordX = %4x %4x %4x",
   MAC.Word.MAC01,
   MAC.Word.MAC23,
   MAC.Word.MAC45);
*/

   printf("\n IP Address.ByteD = %3d %3d %3d %3d",
   IP.Byte.IP0 & 0xff,
   IP.Byte.IP1 & 0xff,
   IP.Byte.IP2 & 0xff,
   IP.Byte.IP3 & 0xff);

   printf("\n IP Address.ByteX = %2x %2x %2x %2x",
   IP.Byte.IP0 & 0xff,
   IP.Byte.IP1 & 0xff,
   IP.Byte.IP2 & 0xff,
   IP.Byte.IP3 & 0xff);

  COPY_IP(RemoteIP,IP);

   printf("\n Remote IP Address.ByteD = %3d %3d %3d %3d",
   RemoteIP.Byte.IP0 & 0xff,
   RemoteIP.Byte.IP1 & 0xff,
   RemoteIP.Byte.IP2 & 0xff,
   RemoteIP.Byte.IP3 & 0xff);


   printf("\nMyIP IP Address.WordX = %4x %4x",
   MyIP.Word.IP01,
   MyIP.Word.IP23);


  COPY_MAC(EF.MACH.Dest,RemoteMAC);
  COPY_MAC(EF.MACH.Source,MyMAC);
  EF.MACH.Type=PROTOCOL_IP;
  /* prepare ip header */
  EF.Data.IPH.TypeOfService=0;
  EF.Data.IPH.IHL=5;
  EF.Data.IPH.Version=4;
  EF.Data.IPH.Flags_FO=0;
  EF.Data.IPH.Protocol=PROTOCOL_TCP;
  EF.Data.IPH.TimeToLive=64;
  COPY_IP(EF.Data.IPH.Dest,RemoteIP);
  COPY_IP(EF.Data.IPH.Source,MyIP);
  EFI=(int *)&EF;
  for(i=0;i<128;i++)
  {
   i1=EFI[i];
   if((i&7)== 0) printf("\n %4x  ",i);
   printf(" %2x %2x",i1 & 0xff,(i1>>8)&0xff );
  }
          goto fin;

          }
#endif
    /*----------------------------------------------*/
#define PR_CZC 0
#if(PR_CZC != 0)
 #if (Cor_XY != 0)
    /*----------------------------------------------*/
         /* --------  command 'CZC'  ------------ */
         /* --------   Test time of performing   */

        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='Z')&&(intrpr.wrd[2]=='C'))
          {
//---------------------------------------
   printf("\n arr_cor_fl[][3]");
   for(i1=0;i1< NN_AX1; i1++)
     printf("\n %f %f %f ", arr_cor_fl[i1][0], arr_cor_fl[i1][1], arr_cor_fl[i1][2]);
//---------------------------------------
           i=0;
           i1=0;
//---------------------------------------
    printf( "\n Addr Flash %8x \n",(int)cor_xy[i1].BegCor_xy);

    mask1 = 0x0000ffff;
    mask0 = 0x00008000;

    printf( "\n cor_xy.TT_PER_1_x= %f \n", cor_xy[0].TT_PER_1_x);

    mask1 = 0x0000ffff;
    mask0 = 0x00008000;
    i=0;
/*
           disable_int();
           f_tim2();

   S_cur = *ss_dem[i1];
   if((TT_PER_1_x[i1]*cor_fct[i1]) != 0)
   {
           c_mlx = *ss_dem[0] * TT_PER_1_x[i1];
           c_ii = (int)(fabs(c_mlx));
           c_mlx -= (float)c_ii;

           c_mly = *ss_dem[1] * TT_PER_1_y[i1];
           c_jj = (int)(fabs(c_mly));
           c_mly -= (float)c_jj;



    corXY_scale=0.1;

    F_Addr= BegCor_xy[i1] + (lgth_y[i1]*c_ii + c_jj);
    v1= (int)((*((int*)(int)F_Addr) & mask1)-mask0) * corXY_scale;

    F_Addr +=1.;
    v2= (int)((*((int*)(int)F_Addr) & mask1)-mask0) * corXY_scale;

//    F_Addr +=i_lgth_y[i1];
    F_Addr += lgth_y[i1];
    v4= (int)((*((int*)(int)F_Addr ) & mask1)-mask0) * corXY_scale;

    F_Addr -=1.;
    v3= (int)((*((int*)(int)F_Addr) & mask1)-mask0) * corXY_scale;

    v2=v1 + (v2-v1) * c_mly;
    v4=v3 + (v4-v3) * c_mly;
    S_cur +=   (v2 + (v4-v2) * c_mlx );
   }
           f_tim3();
           enable_int();

             printf("\n16 bit  correction counting ");
           print_t2();
*/
//---------------------------------------------
           disable_int();
           f_tim2();
    v2=v1 + (v2-v1) * c_mlx;
           f_tim3();
           enable_int();
             printf("\nv2=v1 + (v2-v1) * c_mlx;  counting ");
           print_t2();
//-------------------------------
           disable_int();
           f_tim2();
    F_Addr= i1*i2;
           f_tim3();
           enable_int();
             printf("\nF_Addr= i1*i2;  counting ");
           print_t2();
//-------------------------------
           disable_int();
           f_tim2();
    F_Addr= c_ii*c_jj;
           f_tim3();
           enable_int();
             printf("\nF_Addr= c_ii*c_jj;  counting ");
           print_t2();
//-------------------------------
           disable_int();
           f_tim2();
    F_Addr= i1+i2;
           f_tim3();
           enable_int();
             printf("\nF_Addr= i1+i2;  counting ");
           print_t2();
//-------------------------------
           disable_int();
           f_tim2();
    F_Addr+=1;
           f_tim3();
           enable_int();
             printf("\nF_Addr+=1;  counting ");
           print_t2();
//-------------------------------
           i=0;
           disable_int();
           f_tim2();


    uu_cf2.fl=uu_cf1.fl + (uu_cf2.fl-uu_cf1.fl) * c_mlx;
    uu_cf4.fl=uu_cf3.fl + (uu_cf4.fl-uu_cf3.fl) * c_mlx;
    S_cur = *ss_dem[i1] + cor_fct[i1] * (uu_cf2.fl + (uu_cf4.fl-uu_cf2.fl) * c_mly);

           f_tim3();
           enable_int();
     printf("\nCorrection calculations counting ");
           print_t2();
//-------------------------------

    F_Addr=FLASH_BASE+Fl_Cor_XY+8;

           disable_int();
           f_tim2();
  //  F_Addr += lgth_y[i1]<<1;

    F_Addr -=2;

    i= *((int*)(int)F_Addr);
    F_Addr+=1;
//  uu_cf.it= (i & 0xffff) | ((*((int*)(int)F_Addr) ) <<16);
    uu_cf1.it= (i & mask1 ) | ((*((int*)(int)F_Addr) ) <<16);
//    v2=uu_cf1.fl;

           f_tim3();
           enable_int();

           printf("\nOne real variable read ");
           print_t2();
//------------------------------------
    F_Addr=FLASH_BASE+Fl_Cor_XY+8;
           disable_int();
           f_tim2();
    v4= (int)((*((int*)(int)F_Addr) & mask1)-mask0) * corXY_scale;
           f_tim3();
           enable_int();

    printf("\n16 bit correction value read (scale to float) ");
           print_t2();
//-------------------------------
           disable_int();
           f_tim2();
           i= *((int*)(int)F_Addr) ;
           f_tim3();
           enable_int();

    printf("\n16 bit value read only ");
           print_t2();
//-------------------------------
//---'CZC'------ Structure --------------
           mask1 = 0x0000ffff;
           i=0;
           disable_int();
           f_tim2();

#define fmask1 (mask1)

   uk_xy=&cor_xy[i];

   S_cur = *ss_dem[i];
   if((uk_xy->TT_PER_1_x) != 0)
   {

    c_mlx = *uk_xy->s_dem1 * uk_xy->TT_PER_1_x;
    c_ii = (int)(fabs(c_mlx));
    c_mlx -= (float)c_ii;

    c_mly = *uk_xy->s_dem2 * uk_xy->TT_PER_1_y;
    c_jj = (int)(fabs(c_mly));
    c_mly -= (float)c_jj;

    F_Addr1= uk_xy->BegCor_xy + ((uk_xy->lgth_y*c_ii + c_jj) *2.);

    uu_cf1.it= (*((int*)(int)F_Addr1) & fmask1) | ((*((int*)(int)(F_Addr1+1)) ) <<16);
    uu_cf2.it= (*((int*)(int)(F_Addr1+2 ) ) & fmask1) | ((*((int*)(int)(F_Addr1+3)) ) <<16);

    F_Addr1 += uk_xy->i_lgth_y2;
    uu_cf3.it= (*((int*)(int)F_Addr1) & fmask1) | ((*((int*)(int)(F_Addr1+1) ) ) <<16);
    uu_cf4.it= (*((int*)(int)(F_Addr1+2)) & fmask1) | ((*((int*)(int)(F_Addr1+3) ) ) <<16);
    uu_cf2.fl=uu_cf1.fl + (uu_cf2.fl-uu_cf1.fl) * c_mly;
  S_cur +=  (uu_cf2.fl + ( (uu_cf3.fl + (uu_cf4.fl-uu_cf3.fl) * c_mly) - uu_cf2.fl) * c_mlx)*uk_xy->cor_fct;

/*
      Y         v2    v4
      |
      |___X     v1    v3

    v2=v1 + (v2-v1) * c_mly;
    v4=v3 + (v4-v3) * c_mly;
    S_cur = *ss_dem[i1] + cor_fct[i1] * (v2 + (v4-v2) * c_mlx );
*/
   }
           f_tim3();
           enable_int();

    printf("\nfloat32 correction counting (struct) (cor = %f ) ", (S_cur-*ss_dem[i1]) );
           print_t2();


//---------------------------------------
           i=0;
           disable_int();
           f_tim2();

   S_cur = *ss_dem[i];
   if(cor_xy[i].TT_PER_1_x  != 0)
      S_cur+= f_cori_val_XY(i)*cor_xy[i].cor_fct;
           f_tim3();
           enable_int();
    printf("\nfloat32 correction counting (function) (cor = %f ) ", (S_cur-*ss_dem[i1]) );
           print_t2();
//---------------------------------------
           i=0;
           disable_int();
           f_tim2();

   uk_xy=&cor_xy[i];

   S_cur = *ss_dem[i];
   if((uk_xy->TT_PER_1_x  != 0)&&(uk_xy->TT_PER_1_y  != 0) )
   {

    c_mlx = *uk_xy->s_dem1 * uk_xy->TT_PER_1_x;
    c_ii = (int)(fabs(c_mlx));
    c_mlx -= (float)c_ii;

    c_mly = *uk_xy->s_dem2 * uk_xy->TT_PER_1_y;
    c_jj = (int)(fabs(c_mly));
    c_mly -= (float)c_jj;

    F_Addr1= uk_xy->BegCor_xy + ((uk_xy->lgth_y*c_ii + c_jj) *2.);

    uu_cf1.it= (*((int*)(int)F_Addr1) & fmask1) | ((*((int*)(int)(F_Addr1+1)) ) <<16);
    uu_cf2.it= (*((int*)(int)(F_Addr1+2 ) ) & fmask1) | ((*((int*)(int)(F_Addr1+3)) ) <<16);

    F_Addr1 += uk_xy->i_lgth_y2;
    uu_cf3.it= (*((int*)(int)F_Addr1) & fmask1) | ((*((int*)(int)(F_Addr1+1) ) ) <<16);
    uu_cf4.it= (*((int*)(int)(F_Addr1+2)) & fmask1) | ((*((int*)(int)(F_Addr1+3) ) ) <<16);
    uu_cf2.fl=uu_cf1.fl + (uu_cf2.fl-uu_cf1.fl) * c_mly;
  S_cur +=  (uu_cf2.fl + ( (uu_cf3.fl + (uu_cf4.fl-uu_cf3.fl) * c_mly) - uu_cf2.fl) * c_mlx)*uk_xy->cor_fct;

/*
      Y         v2    v4
      |
      |___X     v1    v3

    v2=v1 + (v2-v1) * c_mly;
    v4=v3 + (v4-v3) * c_mly;
    S_cur = *ss_dem[i1] + cor_fct[i1] * (v2 + (v4-v2) * c_mlx );
*/
   }
   else if (uk_xy->TT_PER_1_x  != 0)
   {

    c_mlx = *uk_xy->s_dem1 * uk_xy->TT_PER_1_x;
    c_ii = (int)(fabs(c_mlx));
    c_mlx -= (float)c_ii;

    F_Addr1= uk_xy->BegCor_xy + c_ii+c_ii;

    uu_cf1.it= (*((int*)(int)F_Addr1) & fmask1) | ((*((int*)(int)(F_Addr1+1)) ) <<16);
    uu_cf2.it= (*((int*)(int)(F_Addr1+2 ) ) & fmask1) | ((*((int*)(int)(F_Addr1+3)) ) <<16);
    S_cur +=  ( uu_cf1.fl + (uu_cf2.fl-uu_cf1.fl) * c_mlx)*uk_xy->cor_fct;
   }
           f_tim3();
           enable_int();
    printf("\nfloat32 correction counting (function if_else) (cor = %f ) ", (S_cur-*ss_dem[i1]) );
           print_t2();
//---------------------------------------
//-------------------------------
           goto fin;

          }
    /*----------------------------------------------*/
 #endif // (Cor_XY != 0)
#endif // PR_CZC
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='Z'))
         /* --------  command 'CZ'  ------------ */
          {
           if(f_dis_set(ds_cz,2,6)>0)  f_cfg_cz();
           goto fin;
          }
    /*----------------------------------------------*/

    /*----------------------------------------------*/
    /*               Z_Force                        */
    /*----------------------------------------------*/
    /*----------------------------------------------*/

#if(Z_Force != 0)
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='F')&&(intrpr.wrd[2]=='A'))
         /* --------  command 'MFA'== M50 -------- */
         /* --------  Xn - mkm , Freq Hz           */
          {
            kk_p = dff_xn;
            kk_i = vv_osc * 1000.* k_t_mks/(t_int*2.048) ;
           if(f_dis_set(ds_kf,3,2))
            vv_osc = 2.048 * f_mm_it(kk_i);
            dff_xn = kk_p;
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='F')&&(intrpr.wrd[2]=='B'))
         /* --------  command 'MFB'== M51 -------- */
         /* --------  Xn - mkm via velocity , Freq Hz  */
          {
            kk_i = vv_osc * 1000.* k_t_mks/(t_int*2.048) ;
            kk_p = dff_vn * 1000.* k_t_mks /( 0.002 * PI * kk_i * t_int);

           if(f_dis_set(ds_kf,3,2))
            vv_osc = 2.048 * f_mm_it(kk_i);
            dff_vn = f_mm_it( kk_p * 0.002 * PI * kk_i);
//          dff_vn =  kk_p * 0.002 * PI * kk_i * t_int / (1000. * k_t_mks) ;

            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='F')&&(intrpr.wrd[2]=='C'))
         /* --------  command 'MFC'== M52 -------- */
         /* --------  Force - 0...100(%), Freq Hz  */
          {
            kk_p = dff_yn/ 327.68;
            kk_i = vv_osc * 1000.* k_t_mks/(t_int*2.048) ;
            if(f_dis_set(ds_kf,3,2))
            vv_osc = fabs( 2.048 * f_mm_it(kk_i));
            dff_yn =  fabs(kk_p * 327.68);
            if(dff_yn > 32768 ) dff_yn=32768;
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='F')&&(intrpr.wrd[2]=='F'))
         /* --------  command 'MFF'== M55 -------- */
         /* --------  Stop oscillation  */
          {
            ss_osc=0;
            dff_xn=0;
            dff_vn=0;
            dff_yn=0;
            vv_osc=0;
            goto fin;
          }
    /*----------------------------------------------*/
#endif //(Z_Force != 0)

    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='R')&&(intrpr.wrd[1]=='F'))
         /* --------  command 'RF'  ------------ */
         /* ----  RF  - rotate factors -- */
          {
           if( (itmp1= f_dis_set(ds_rf,2,4)) >0 ) f_cfg_count() ;
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='V')&&(intrpr.wrd[1]=='E')&&(intrpr.wrd[2]=='R') )
         /* --------  команда 'VER' ----------- */
          {

           if(!f_dis_set(&ds_ver,3,1))
              if( (fl_ea_io <= 0) || (flag_ea==0) )
#if(PROC_TYPE == C33)
                 printf(".%d,DSP33 %s %8x %8x",(int)Ver_sub,VerNameDate,SW_code1,SW_code);
#else
                 printf(".%d,DSP32 %s %8x %8x",(int)Ver_sub,VerNameDate,SW_code1,SW_code);
#endif //(PROC_TYPE == C33)

           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='R')&&(intrpr.wrd[1]=='P')  )
         /* -----   command 'RP'        ---- */
           {
             if(f_dis_set(ds_rp,2,2)<0) goto fin_e;
             goto fin;
           }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G')&& (intrpr.wrd[1]=='D') )
         /* --------  команда 'GD' Передать наверх данные из АЦП---- */
          {
           f_Read_ADC_A();
           putch_ea(0);
           i=12;
           putch2(i);
           for(i=0;i<6;i++) putch2((int)ADC[i]);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G')&& (intrpr.wrd[1]=='L') )
         /* --------  команда 'GL' Передать наверх данные из АЦП в цикле---- */
          {
            putch_ea('\n');
            while(1)
            {
             f_Read_ADC_A();

             printf("1-8:  %5d %5d %5d %5d %5d %5d %5d %5d\r",(int)ADC[0],(int)ADC[1],(int)ADC[2],(int)ADC[3],(int)ADC[4],(int)ADC[5],(int)ADC[6],(int)ADC[7]);
             printf("9-16: %5d %5d %5d %5d %5d %5d %5d %5d\n",(int)ADC[8],(int)ADC[9],(int)ADC[10],(int)ADC[11],(int)ADC[12],(int)ADC[13],(int)ADC[14],(int)ADC[15]);

               f_delay_p(100000.);
             if(kbhit())
               if( getch() == CR )  break;
               else putch_ea('\n');
            }
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G')&& (intrpr.wrd[1]=='A') )
         /* --------  'GA' команда Передать наверх данные из АЦП---- */
          {
           if(f_inw()) goto fin_e;
           f_Read_ADC_A();
           printf(" %5d %5d %5d %5d %5d %5d %5d %5d\n",(int)ADC[0],(int)ADC[1],(int)ADC[2],(int)ADC[3],(int)ADC[4],(int)ADC[5],(int)ADC[6],(int)ADC[7]);
           printf("    %5d %5d %5d %5d %5d %5d %5d %5d\n",(int)ADC[8],(int)ADC[9],(int)ADC[10],(int)ADC[11],(int)ADC[12],(int)ADC[13],(int)ADC[14],(int)ADC[15]);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G')&& (intrpr.wrd[1]=='N') )
         /* --------  'GN' команда Передать наверх данные из CNT---- */
          {
           kk_p= *qcc_c[0] & 0xffff;
           kk_i= *qcc_c[1] & 0xffff;
           kk_d= *qcc_c[2] & 0xffff;
           kkat= *qcc_c[3] & 0xffff;
           kk_f= *qcc_c[4] & 0xffff;
           tmpf1=*qcc_c[5] & 0xffff;
           tmpf2=*qcc_c[6] & 0xffff;
           tmpf3=*qcc_c[7] & 0xffff;

           f_dis_set(ds_kf,2,8);

           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='M')  )
         /* --------  command 'DM' ---- */
           {
             if(f_dis_set(&ds_dm,2,1) <0) goto fin_e;
             goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='P')  )
         /* --------  command 'MP' ---- */
           {
             if(f_dis_set(&ds_mp,2,1) <0) goto fin_e;
             goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='J')  )
         /* --------  command 'MJ' ---- */
           {  // результат отработки блочной команды
             kk_p=J_com_c; // количество успешно введенных комвнд
             kk_i=J_com_n; // общее количество команд
     f_dis_set(ds_kf,2,2); // код и флаги последней обрабатываемой команды
                           // в случае ошибки -  команда вызвавшая ошибку
             printf(" %4x",J_com_cc);
             goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F')&&(intrpr.wrd[1]=='F')&&(intrpr.wdl==2) )
        {
         /*  -----   command  'FF' ------------ */
         /* --  Display period position inside of period  -- */
          /*--------------------*/
#if(PLANAR == 0)

          for(i=0;i<6;i++)
          {
           *ds_kf[i].op= FFFF[i] * ScaleCntMKM * pf[i];
           if(*ds_kf[i].op<0.) *ds_kf[i].op+=fabs(pf[i]);
          }

#else // (PLANAR != 0)
          for (i=0;i<3;i++)
           {
             *ds_kf[i].op = FFFF_C[i];
             if(*ds_kf[i].op<0.) *ds_kf[i].op+=fabs(pf[i]);
           }

          for(i=3;i<6;i++)
          {
           *ds_kf[i].op=(float) (CntDiskr[i] & 0xFFFF) * ScaleCntMKM * pf[i];
           if(*ds_kf[i].op<0.) *ds_kf[i].op+=fabs(pf[i]);
          }

#endif // (PLANAR == 0)
           itmp1=f_dis_set( ds_kf,2,6);

           goto fin;
        }
    /*----------------------------------------------*/
#if(PLANAR == 0)

    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L') && (intrpr.wrd[1]=='K') && ( (i=f_xyzcbu(3)) >= 0 ) )
         /* --------  команда 'LKX','LKY','LKZ','LKC', ------------ */
          {
           kf_par[0]= &BKp[i];
           kf_par[1]= &BKi[i];
           kf_par[2]= &BKd[i];
           kf_par[3]= &BKk[i];

m_bkk0:
           i1=4; // n param
           j=3;  // char in command name

m_bkk:
          for(tmp=0;tmp<i1;tmp++)
             *ds_kf[tmp].op=*kf_par[tmp];

           itmp1=f_dis_set( ds_kf,j,i1);
           if(itmp1 > 0)
           {

             for(tmp=0;tmp<itmp1;tmp++)
               *kf_par[tmp]=*ds_kf[tmp].op;


             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;

          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E') && (intrpr.wrd[1]=='K') && ( (i=f_xyzcbu(3)) >= 0 ) )
         /* --------  команда 'EKX','EKY','EKZ','EKC', ------------ */
          {
           kf_par[0]= &EKp[i];
           kf_par[1]= &EKi[i];
           kf_par[2]= &EKd[i];
           kf_par[3]= &EKk[i];

       //  i1 = 4;    // n param
       //  j = 3;     // char in command name
           goto m_bkk0;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L') && (intrpr.wrd[1]=='K') && (intrpr.wrd[2]=='V')&& ( (i=f_xyzcbu(4)) >= 0 ) )
         /* --------  команда 'LKVX','LKVY','LKVZ','LKVC', ------------ */
          {
           kf_par[0]= &BKVp[i];
           kf_par[1]= &BKVi[i];
           kf_par[2]= &BKVd[i];

           i1 = 3;    // n param
           j = 4;     // char in command name
           goto m_bkk;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E') && (intrpr.wrd[1]=='K') && (intrpr.wrd[2]=='V')&& ( (i=f_xyzcbu(4)) >= 0 ) )
         /* --------  команда 'EKVX','EKVY','EKVZ','EKVC', ------------ */
          {
           kf_par[0]= &EKVp[i];
           kf_par[1]= &EKVi[i];
           kf_par[2]= &EKVd[i];

           i1 = 3;    // n param
           j = 4;     // char in command name
           goto m_bkk;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M') && (intrpr.wrd[1]=='A') )
         /* --------  команда 'MA'- Mass Actual  ------------ */
          {
           for(tmp=0;tmp<6;tmp++)
              kf_par[tmp]= &Ma_a[tmp];

           i1 = 6;    // n param
           j = 2;     // char in command name
           goto m_bkk;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M') && (intrpr.wrd[1]=='L') )
         /* --------  команда 'ML'- Mass Begin (Low) ------------ */
          {
           for(tmp=0;tmp<6;tmp++)
              kf_par[tmp]= &Mb_a[tmp];

           i1 = 6;    // n param
           j = 2;     // char in command name
           goto m_bkk;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M') && (intrpr.wrd[1]=='E') )
         /* --------  команда 'ME'- Mass End    ------------ */
          {
           for(tmp=0;tmp<6;tmp++)
              kf_par[tmp]= &Me_a[tmp];

           i1 = 6;    // n param
           j = 2;     // char in command name
           goto m_bkk;
          }
    /*----------------------------------------------*/
#if(TWIN_X == 0)
        if ( intrpr.wrd[0]=='K'&&( (i=f_xyzcbu(2)) >= 0 ) )
#else
        if ( intrpr.wrd[0]=='K'&&( (i=f_xyzcbuw(2)) >= 0 ) )
#endif
         /* --------  команда 'KX','KY','KZ','KC', ------------ */
          {
           kk_p= k_p[i];
           kk_i= k_i[i];
           kk_d= k_d[i];
           kkat= k_v[i];
           kk_f= kat[i];
           tmpf1= frict[i]/0x7fff;
           tmpf2= k_fa[i];
           tmpf3= frict1[i];  //K_VN1[i]
/*
           tmpf3= k_f[i];
           tmpf4= frict1[i];
*/
           itmp1=f_dis_set( ds_kf,2,8);
           if(itmp1 > 0)
           {
             k_p[i]=kk_p;
             k_i[i]=kk_i;
             k_d[i]=kk_d;
             k_v[i]=kkat;
             kat[i]=kk_f;
             frict[i]=tmpf1*0x7fff;
             k_fa[i]=tmpf2;
             frict1[i]=tmpf3;
  /*
             k_f[i]=tmpf3;
             frict1[i]=tmpf4;
  */
             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
#if ( SURF != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='K')&&( intrpr.wrd[1]=='S' ) )
         /* --------  команда 'KS' ------------ */
          {
           kk_p= sk_p;
           kk_i= sk_i;
           kk_d= sk_d;
           kkat= sk_f;
           itmp1=f_dis_set( ds_kf,2,4);
           if(itmp1 > 0)
           {
             sk_p= kk_p;
             sk_i= kk_i;
             sk_d= kk_d;
             sk_f= kkat;
             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='Z')&&(intrpr.wrd[1]=='S'))
         /* --------  команда 'ZS'  ------------ */
         /*  ZS - Parameters Surface following system,
         */
          {
           itmp1=f_dis_set(ds_zs,2,6);
           if(itmp1>0)
            {
              f_v_az();
              disable_int();
              f_count_cf();
              enable_int();
            }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='S'))
         /* --------  команда 'DS' ------------ */
         /* Display Surface signal  */
          if(intrpr.wdl==2)
            {
              if(f_inw()) goto fin_e;
              printf(" %f ",Analog_1);
              goto fin;
            }
    /*----------------------------------------------*/
     if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='S')&& (intrpr.wrd[2]=='C'))
         {          //  'DSC'
            print_c(&Analog_1,NULL,NULL,NULL,NULL,NULL);
            goto fin;
         }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S') && (intrpr.wrd[1]=='S'))
         /* --------  команда 'SS' ------------ */
         /* Surface following State   */
        {
          if(intrpr.wdl==2)
            {
             if(f_inw()) strg=intrpr.wrd;
             else
             {
              if( (var[i_fl_surfe] == 0) )
                   printf(" %d",(int) -flag_surf);
              else
                   printf(" %d",(int) flag_surf);
              goto fin;
             }
            }
          else
              strg=&intrpr.wrd[2];

          if(f_innum(strg) )
            {
              if(intrpr.num_var==0.)
                { /* выключить слежение  */

                  var[i_fl_surfe]=0.;
                  if(flag_surf != 0 )
                     if( ((m_fin-m_beg) & (sz_move-1))== 0)
                     {
                       flag_surf=0.;
                       sXN_1 = 0.;
                       sXN_2 = 0.;
                       sYN=0.;

                       move_p[m_fin].z_b=s_surf;
                       *ss_dem[2]=s_surf;
                       var[Warning]=((int)var[Warning]) & ~(SURF_ERR);
                     }
                     else
                     {
                       if(flag_ea == 0) putch_ea('!');
                       goto fin;
                     }
                }
              else if(intrpr.num_var==1.)
                { /* включить слежение   */

                    if(Flag_sys[2] == 0)  goto fin_e;
                    var[i_fl_surfe]=1.;

                    if(flag_surf==0.)
                        {
                         sXN_1 = 0.;
                         sXN_2 = 0.;
                         sYN=0.;
                         s_surf=*ss_dem[2];
                        }
                    flag_surf=1.;
                }
              else if(intrpr.num_var== 2.)
                { /* включить режим слежения с отключенным флагом
                     разрешения и значением скорости  = 0 */

                    if( (Flag_sys[2] == 0) && (var[i_fl_surfe] != 0)   ) goto fin_e;

                    var[i_fl_surfe]=1.;

                    if(flag_surf==0.)
                //     if( ((m_fin-m_beg) & (sz_move-1))== 0)
                        {
                         sXN_1 = 0.;
                         sXN_2 = 0.;
                         sYN=0.;
                         s_surf=*ss_dem[2];
                        }
                     flag_surf=2.;
                }
              else if(intrpr.num_var== 3.)
                { /*  включить выход Z из режима слежения в
                      позицию var[i_z_abs_s]                   */

                    if( (Flag_sys[2] == 0) && (var[i_fl_surfe] != 0) ) goto fin_e;

                    var[i_fl_surfe]=1.;
                    if(flag_surf==0.)
                        {
                         sXN_1 = 0.;
                         sXN_2 = 0.;
                         sYN=0.;
                         s_surf=*ss_dem[2];
                        }
                    flag_surf=3.;
                }
              else goto pr_bad;
            }
          else goto pr_bad;

          goto fin;
        }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&&(intrpr.wrd[1]=='A'))
         /* --------  команда 'SA'  ------------ */
         /*  Surface Absolute (discrete)       */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

    //     if( (Flag_sys[2] != FS) ) goto fin_e;

           sx_0 = surf_a;
           if(f_dis_set(ds_pr,2,1) > 0) surf_a = sx_0;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&&(intrpr.wrd[1]=='R'))
         /* --------  команда 'SR'  ------------ */
         /*  Surface Relative (discrete)       */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;
    //     if( (Flag_sys[2] != FS) ) goto fin_e;
           sx_0=0;
           if(f_dis_set(ds_pr,2,1) > 0) surf_a += sx_0;

           goto fin;
          }
    /*----------------------------------------------*/
#endif // ( SURF != 0)
    /*----------------------------------------------*/

#if(TWIN_X == 0)
        if ( (intrpr.wrd[0]=='K')  && (intrpr.wrd[1]=='V')  &&  ( (i=f_xyzcbu(3)) >= 0 ) )
#else
        if ( (intrpr.wrd[0]=='K')  && (intrpr.wrd[1]=='V')  &&  ( (i=f_xyzcbuw(3)) >= 0 ) )
#endif

         /* --------  команда 'KVX','KVY','KVZ','KVC', ------------ */
          {
           kk_p= kv_p[i];
           kk_i= kv_i[i];
           kk_d= kv_d[i];
           kkat = k_fv[i];

#if(BTW == 0)
           itmp1=f_dis_set( ds_kf,3,4);
#else
           itmp1=f_dis_set( ds_kf,3,3);
#endif
           if(itmp1 > 0)
           {
             kv_p[i]=kk_p;
             kv_i[i]=kk_i;
             kv_d[i]=kk_d;
             k_fv[i]=kkat;

             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
#else  // (PLANAR != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L') && (intrpr.wrd[1]=='P')&& (intrpr.wrd[2]=='H') )
         /* --------  command 'LPH'  ------------ */
         /* --  LPH - Lining Phase parameters    -- */
          {
             if(f_dis_set(ds_lph,3,3)<0) goto fin_e;
             goto fin;

          }
        /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L') && (intrpr.wrd[1]=='P')&& (intrpr.wrd[2]=='O') )
         /* --------  command 'LPO'  ------------ */
         /* --  LPO - Lining offset parameters    -- */
          {
             if(f_dis_set(ds_lpo,3,6)<0) goto fin_e;
             goto fin;

          }
        /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L') && (intrpr.wrd[1]=='P')&& (intrpr.wrd[2]=='A') )
         /* --------  command 'LPA'  ------------ */
         /* --  LPA - Lining amplitude parameters    -- */
          {
             if(f_dis_set(ds_lpa,3,6)<0) goto fin_e;
             goto fin;

          }
        /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L') && (intrpr.wrd[1]=='P') )
         /* --------  command 'LP'  ------------ */
         /* --  LP - Lining parameters    -- */
          {
             if(f_dis_set(ds_lp,2,9)<0) goto fin_e;
             goto fin;
          }
        /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L')&& (intrpr.wrd[1]=='M')&& (intrpr.wrd[2]=='M') )
         /* --------  command 'LMM'  ------------ */
         /* --  LMM - moments for folowing systems -- */
            {
             if(f_dis_set(ds_mm,3,3) <0) goto fin_e;
             goto fin;
            }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='B'))
         /* --------  command 'NB'  ------------ */
          {
           if(f_dis_set(ds_nb,2,4) > 0)  f_tabl();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E')&& (intrpr.wrd[1]=='R') )
         /* --------  command 'ER'  ------------ */
         /* --  ER - errors for folowing systems -- */
            {
              if(f_dis_set(ds_er,2,7) > 0)
               for(i=0;i<5;i++)
                 if( (State_s[i] != 0) && (State_s[i] != REP_FND) )  ;
                 else  maxc_err[i] = max_err[i];

              goto fin;
            }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
#if(TWO_MS == 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L') && (intrpr.wrd[1]=='K') && ( (i=f_fxyzcb(3)) >= 0 ) )
         /* --------  команда 'LKF','LKX','LKY','LKZ','LKC', ------------ */
          {
           kf_par[0]= &BKp[i];
           kf_par[1]= &BKi[i];
           kf_par[2]= &BKd[i];
           kf_par[3]= &BKk[i];

m_bkk0:
           i1=4; // n param
           j=3;  // char in command name

m_bkk:
          for(tmp=0;tmp<i1;tmp++)
             *ds_kf[tmp].op=*kf_par[tmp];

           itmp1=f_dis_set( ds_kf,j,i1);
           if(itmp1 > 0)
           {

             for(tmp=0;tmp<itmp1;tmp++)
               *kf_par[tmp]=*ds_kf[tmp].op;


             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;

          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E') && (intrpr.wrd[1]=='K') && ( (i=f_fxyzcb(3)) >= 0 ) )
         /* --------  команда 'EKX','EKY','EKZ','EKC', ------------ */
          {
           kf_par[0]= &EKp[i];
           kf_par[1]= &EKi[i];
           kf_par[2]= &EKd[i];
           kf_par[3]= &EKk[i];

       //  i1 = 4;    // n param
       //  j = 3;     // char in command name
           goto m_bkk0;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M') && (intrpr.wrd[1]=='A') )
         /* --------  команда 'MA'- Mass Actual  ------------ */
          {
           kk_p= Ma_a[0];
           kk_i= Ma_a[3];
           kk_d= Ma_a[4];
           kkat= Ma_a[5];
           itmp1=f_dis_set( ds_kf,2,4);
           if(itmp1 > 0)
           {
             Ma_a[0] = kk_p;
             Ma_a[1] = kk_p;
             Ma_a[2] = kk_p;
             Ma_a[3] = kk_i;
             Ma_a[4] = kk_d;
             Ma_a[5] = kkat;

             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M') && (intrpr.wrd[1]=='L') )
         /* --------  команда 'ML'- Mass Begin (Low) ------------ */
          {
           kk_p= Mb_a[0];
           kk_i= Mb_a[3];
           kk_d= Mb_a[4];
           kkat= Mb_a[5];
           itmp1=f_dis_set( ds_kf,2,4);
           if(itmp1 > 0)
           {
             Mb_a[0] = kk_p;
             Mb_a[1] = kk_p;
             Mb_a[2] = kk_p;
             Mb_a[3] = kk_i;
             Mb_a[4] = kk_d;
             Mb_a[5] = kkat;

             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M') && (intrpr.wrd[1]=='E') )
         /* --------  команда 'ME'- Mass End    ------------ */
          {
           kk_p= Me_a[0];
           kk_i= Me_a[3];
           kk_d= Me_a[4];
           kkat= Me_a[5];
           itmp1=f_dis_set( ds_kf,2,4);
           if(itmp1 > 0)
           {
             Me_a[0] = kk_p;
             Me_a[1] = kk_p;
             Me_a[2] = kk_p;
             Me_a[3] = kk_i;
             Me_a[4] = kk_d;
             Me_a[5] = kkat;

             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/

        if ( intrpr.wrd[0]=='K'&&( (i=f_fxyzcb(2)) >= 0 ) )
         /* --------  команда 'KF','KX','KY','KZ','KC', ------------ */
          {
           kk_p= k_p[i];
           kk_i= k_i[i];
           kk_d= k_d[i];
           kkat= k_v[i];
           kk_f= kat[i];
           tmpf1= frict[i]/0x7fff;
           tmpf2= k_fa[i];
           tmpf3= frict1[i];
/*
           tmpf3= k_f[i];
           tmpf4= frict1[i];
*/

        if ( i == f_fxyzcb(3) )
           itmp1=f_dis_set( ds_kf,3,8);
        else
           itmp1=f_dis_set( ds_kf,2,8);

           if(itmp1 > 0)
           {
             k_p[i]=kk_p;
             k_i[i]=kk_i;
             k_d[i]=kk_d;
             k_v[i]=kkat;
             kat[i]=kk_f;
             frict[i]=tmpf1*0x7fff;
             k_fa[i]=tmpf2;
             frict1[i]=tmpf3;
  /*
             k_f[i]=tmpf3;
             frict1[i]=tmpf4;
  */
             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='K')  && (intrpr.wrd[1]=='V')  &&  ( (i=f_fxyzcb(3)) >= 0 ) )
         /* --------  команда 'KVX','KVY','KVZ','KVC', ------------ */
        {
           kk_p= kv_p[i];
           kk_i= kv_i[i];
           kk_d= kv_d[i];
           kkat = k_fv[i];

#if(BTW == 0)
           itmp1=f_dis_set( ds_kf,3,4);
#else
           itmp1=f_dis_set( ds_kf,3,3);
#endif
           if(itmp1 > 0)
           {
             kv_p[i]=kk_p;
             kv_i[i]=kk_i;
             kv_d[i]=kk_d;
             k_fv[i]=kkat;

             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
        }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
#else //(TWO_MS != 0)
        if ( intrpr.wrd[0]=='K'&&( (i=f_fxyuij(2)) >= 0 ) )
         /* --------  команда 'KX','KY','KZ','KC', ------------ */
          {
           kk_p= k_p[i];
           kk_i= k_i[i];
           kk_d= k_d[i];
           kkat= kat[i];
           kk_f= k_fa[i];

           itmp1=f_dis_set( ds_kf,2,5);
           if(itmp1 > 0)
           {
             k_p[i]=kk_p;
             k_i[i]=kk_i;
             k_d[i]=kk_d;
             kat[i]=kkat;
             k_fa[i]=kk_f;

             frict[i]=0;
             frict1[i]=0;
             k_v[i]=0;
             k_f[i]=1;

             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;

          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='K')  && (intrpr.wrd[1]=='V')  &&  ( (i=f_fxyuij(3)) >= 0 ) )
         /* --------  команда 'KVX','KVY','KVZ','KVC', ------------ */
          {
           kk_p= kv_p[i];
           kk_i= kv_i[i];
           kk_d= kv_d[i];
   //      kkat = k_fv[i];

           itmp1=f_dis_set( ds_kf,3,3);
           if(itmp1 > 0)
           {
             kv_p[i]=kk_p;
             kv_i[i]=kk_i;
             kv_d[i]=kk_d;
   //        k_fv[i]=kkat;

             disable_int();
             f_count_cf();
             enable_int();
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
/*
        if ( (intrpr.wrd[0]=='F')&&(intrpr.wrd[1]=='F')&&(intrpr.wdl==2) )
        {
         //-----   command  'FF' -----------
         //--  Display period position inside of period  --
         //--------------------
           ftmp= ScaleCntMKM * pf[3] *(float)FFFF[3];
           if(ftmp < 0) ftmp += fabs(pf[3]);

           ftmp1= ScaleCntMKM * pf[4] *(float)FFFF[4];
           if(ftmp1 < 0) ftmp1 += fabs(pf[4]);

           ftmp2= ScaleCntMKM * pf[5] *(float)FFFF[5];
           if(ftmp2 < 0) ftmp2 += fabs(pf[5]);

           printf(" %f %f %f %f %f %f\n",FFFF_C[0],FFFF_C[1],FFFF_C[2],ftmp,ftmp1,ftmp2);

           goto fin;
        }
*/
    /*----------------------------------------------*/

#endif  // (TWO_MS == 0)
    /*----------------------------------------------*/


#endif  // (PLANAR == 0)
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='V')&&(intrpr.wrd[1]=='P') )
         /* --------  команда 'VP' ------------ */
          {
           if(f_dis_set(ds_vp,2,10) <0 ) goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='V')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='P'))
          {    //'VSP'
           if( f_dis_set(ds_vsp,3,12) > 0 )
            {
               disable_int();
               f_v_az();
               enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='V')&&(intrpr.wrd[1]=='S'))
          {    //'VS'
           if( f_dis_set(ds_vs,2,12) > 0 )
            {
               disable_int();
               f_v_az();
               enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
#if(TWIN_X != 0)
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='W')&&(intrpr.wrd[2]=='X'))
          {    //'TWX'
           if( f_dis_set(ds_twx,3,8) > 0 )
            {
               disable_int();
               f_v_a_twx();
               enable_int();
            }
           goto fin;
          }

    /*----------------------------------------------*/

        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='W')&&(intrpr.wrd[2]=='S'))
          {    //'TWS'
           tws_tmp=Flag_tw;
           if( f_dis_set(ds_tws,3,2) > 0 )
            {
               disable_int();
               f_init_reg_tw(tws_tmp);
               enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
#endif // (TWIN_X != 0)
    /*----------------------------------------------*/
  #if( NOVS != 0)
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='W')&&(intrpr.wrd[2]=='L'))
          {    //'TWL'
           if(f_dis_set(ds_twl,3,4)>0)
             f_init_twl();
           goto fin;
          }

        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='W')&&(intrpr.wrd[2]=='F'))
          {    //'TWF'
           f_dis_set(ds_twf,3,3);
           goto fin;
          }

  #endif // ( NOVS != 0)
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='R')&& (intrpr.wrd[1]=='E')&& (intrpr.wrd[2]=='S') )
         /* --------  команда 'RES' ------------ */
          {
            if(f_wait_ready_flash() ) goto fin_e;
            disable_int();

//--------------------------------------------------

#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
                close_usb();
#endif

//--------------------------------------------------
            baud_cod=BAUD_9600;
            flag_DT_BR=0;
            Ini_RS232();

            begin();
            goto fin;
          }
/*----------------------------------------------*/
/*----------------------------------------------*/
 #if (Cor_XY != 0)   // команды двухкоординатной коррекции

     if ( (intrpr.wrd[0]=='$')&&(intrpr.wrd[1]=='E') && (intrpr.wrd[2]=='C')&& ( (i=f_xyzcb(4)) >= 0 ) && (intrpr.wdl==4))
         /* --------  команда '$ECX','$ECY','$ECZ' ---- */
          {  // очистить зону коррекции
              if(i>=n_cor_xy_ax) goto fin_e;


             for(j=0;j<n_cor_xy_ax ;j++)
              {
                f_cor_xy_NO_FS(j);
                cor_xy[j].TT_PER_1_x=0.; // запретить использование данных коррекции
              }
              i1 = cor_xy[i].lgth_flash; // размер зоны во флэш (по адресу флэш)
                                         // отведенный на данные коррекции одной оси

              FLASH_COM1 = 0xaa;
              FLASH_COM2 = 0x55;
              FLASH_COM1 = 0x80;
              FLASH_COM1 = 0xaa;
              FLASH_COM2 = 0x55;

              for(i2=cor_xy[i].BegCor_xy;i1>0;i1-=SECTOR_SIZE,i2+=SECTOR_SIZE)
                {
                  *(unsigned int *)(i2)=0x30;
                }
              putch(' ');
              if(f_wait_flash_rdy_nea(300000.) )
               {
                 puts(" - Erasing error");
               }

            f_delay_p(200000.);
            f_corXY_init();
            goto fin;
          }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='T'&&(intrpr.wrd[1]=='H')&&( (i=f_xyzcb(3)) >= 0 ) )
         /* -----   command 'THX','THY' ,'THZ'   ---- */
         /* -----  Receive plate Korrective Table --- */
           {

             if(i>=n_cor_xy_ax) goto fin_e;


             if(intrpr.wdl==3)
                 if(f_inw()) strg=intrpr.wrd;
                 else  {  goto fin_e; }
             else
                  strg=&intrpr.wrd[3];

             if(f_innum(strg) ) i1= intrpr.num_var;   /* begin index */
             else  { goto fin_e; }

//          adr_getch_fl = (FLASH_BASE_ALL + Fl_Cor_XY + Fl_Cor_XY_ax * i  - FLASH_BASE)<<1;
          adr_getch_fl = ((int)cor_xy[i].BegCor_xy - 12 - FLASH_BASE)<<1;
          adr_getch_fl += i1*4;
          if((getch_fl() != 0xff) || (getch_fl()!= 0xff) || (getch_fl() != 0xff) ||(getch_fl() != 0xff) ) i2=1;
          else i2=0;



             if(!f_inw()) { goto fin_e; }
             if(f_innum(intrpr.wrd) ) itmp1 = intrpr.num_var;   /* quantity values */
             else  { goto fin_e; }

          if ( (i1<0) || ( ((itmp1+i1)<<1) >  cor_xy[i].lgth_flash )) { goto fin_e; }

          for(j=0;j<n_cor_xy_ax ;j++)
          {
            f_cor_xy_NO_FS(j);
            cor_xy[j].TT_PER_1_x=0.; // запретить использование данных коррекции
          }

          adr_putch_fl = ((int)cor_xy[i].BegCor_xy - 12 - FLASH_BASE)<<1;
          adr_putch_fl += i1*4;


          adr_getch_fl = adr_putch_fl;
          if((getch_fl() != 0xff) || (getch_fl()!= 0xff) || (getch_fl() != 0xff) ||(getch_fl() != 0xff) ) i2=1;
          else i2=0;

             for(itmp0=0;itmp0<itmp1;i1++,itmp0++)
               {
                 while(!f_inw())
                 {
                   puts ("\n");
  mmth0:
      if(ffgets() )      // input from buf_c[] to command string buffer 'cb_RS232[]'
        {

         RS_n_bcom=0;
         strcpy(intrpr.bfcom,cb_RS232);
         goto mmth1;
        }

      else if(ffgets_USB() )      // input from buf_c_USB[] to command string buffer 'cb_USB[]'
        {

         USB_n_bcom=0;
         strcpy(intrpr.bfcom,cb_USB);
         goto mmth1;
        }
      else
       {
#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
   if( flag_usb == 1)    f_usb();
#endif
         goto mmth0;
       }
   mmth1:
                   intrpr.i3=0;

                 }
                 if(f_innum(intrpr.wrd) )
                  { // запись очередного значения
                    uu_cf1.fl=intrpr.num_var;

                    if(i2==0) i2=putch_fl( uu_cf1.it );
                    if(i2==0) i2=putch_fl( uu_cf1.it>>8 );
                    if(i2==0) i2=putch_fl( uu_cf1.it>>16 );
                    if(i2==0) i2=putch_fl( uu_cf1.it>>24 );
                  }
                 else {
                  //     puts(intrpr.bfcom);
                  //     printf( "5");
                         goto fin_e;
                      }
               }
//-----------------------------
           if(i2==0)
            {
         //  putch_fl(0);
         //  putch_fl(0);
             f_delay_p(20000.);
            }
            else
            {
             if(i2==1)
              {
              puts("\n Data don't store !!! Data zone must be erased previously !!! ");
              printf("\n Please erase data zone by '$EC%c' command before store new data.\n",letter[i]);
               goto fin_e;
              }
             else
              {
mth_err:       puts("\n Flash write error\n");
               goto fin_e;
              }
            }
//-----------------------------
  f_corXY_init();
//-----------------------------

            goto fin;
           }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='N'&&(intrpr.wrd[1]=='H')&&( (i=f_xyzcb(3)) >= 0 ) )
         /* -----  Display Corrective Table --- */
         /* -----   command 'NHX','NHY','NHZ'  ---- */
           {
             if(i>=n_cor_xy_ax) goto fin_e;
             if(intrpr.wdl==3)
                 if(f_inw()) strg=intrpr.wrd;
                 else
                  {
                    i1=0;
                    itmp1=20;
                    goto nhx1;
                  }

             else
                  strg=&intrpr.wrd[3];

             if(f_innum(strg) ) i1= intrpr.num_var;   /* begin index */
             else goto fin_e;

             itmp1=1;
             if(f_inw())
              if(f_innum(intrpr.wrd) ) itmp1= intrpr.num_var;   /* quantity values */
              else goto fin_e;
   nhx1:
          F_Addr = cor_xy[i].BegCor_xy - 12;
          F_Addr += i1*2;

             for(itmp0=0;itmp0<itmp1;i1++,itmp0++)
               {

                i= *((int*)(int)F_Addr);
                F_Addr+=1.;
                uu_cf1.it= (i & 0xffff) | ((*((int*)(int)F_Addr) ) <<16);
                F_Addr+=1.;

                printf(" %f",uu_cf1.fl);
       //       printf("(%8x)",uu_cf1.it);
#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
   if( flag_usb == 1)    f_usb();
#endif
               }
            goto fin;
           }
    /*----------------------------------------------*/

#endif // (Cor_XY != 0)
    /*----------------------------------------------*/
    /*----------------------------------------------*/
    /*----------------------------------------------*/
     if ( (intrpr.wrd[0]=='$')&&(intrpr.wrd[1]=='E') && (intrpr.wrd[2]=='P'))
         /* --------  команда '$EP' ------------ */
          {

 #if (Cor_XY != 0)
          for(j=0;j<n_cor_xy_ax ;j++)
           {
            f_cor_xy_NO_FS(j);
            cor_xy[j].TT_PER_1_x=0.; // запретить использование данных коррекции
           }
 #endif // (Cor_XY != 0)

            for(i=0;i<n_max;i++)
               if(intrr[i].sw) fcloss(i);

#if(AUTOM != 0)
                 for(i=0;i<N_aut;i++)
                    ff_autom[i] = NULL;
#endif // (AUTOM != 0)

              i1 = FLASH_PRG_SZ; // bytes x 2

              FLASH_COM1 = 0xaa;
              FLASH_COM2 = 0x55;
              FLASH_COM1 = 0x80;
              FLASH_COM1 = 0xaa;
              FLASH_COM2 = 0x55;

              for(i2=FLASH_BASE;i1>0;i1-=SECTOR_SIZE,i2+=SECTOR_SIZE)
                {
                  *(unsigned int *)(i2)=0x30;
                }
              puts(" ");
              if(f_wait_flash_ready(300000.) )
               {
                 puts(" - Erasing error");
               }

            f_delay_p(20000.);
            f_vect();
 #if (Cor_XY != 0)
  f_corXY_init();
 #endif // (Cor_XY != 0)
            goto fin;
          }

    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='@')&& (intrpr.wdl==1) )
         /* --------  command Receive program ---- */
           {     // '@'

 #if (Cor_XY != 0)
          for(j=0;j<n_cor_xy_ax ;j++)
           {
            f_cor_xy_NO_FS(j);
            cor_xy[j].TT_PER_1_x=0.; // запретить использование данных коррекции
           }
 #endif // (Cor_XY != 0)
            adr_getch_fl=0;
            if((getch_fl() != 0xff) || (getch_fl()!= 0xff) || (getch_fl() != 0xff) ||(getch_fl() != 0xff) ) i1=1;
            else i1=0;

            for(i=0;i<n_max;i++)
               if(intrr[i].sw) fcloss(i);

#if(AUTOM != 0)
                 for(i=0;i<N_aut;i++)
                    ff_autom[i] = NULL;
#endif // (AUTOM != 0)

            itmp1=0;   // error flag
            itmp0=0;
            adr_putch_fl = 0;    // set begin address in FLASH relatively FLASH_BASe

//printf("\n Write cycle begins");
            itmp0=getch();
//printf("\n next:");

            for(i=0;(i< ( FLASH_PRG_SZ + FLASH_PRG_SZ )  ) && ( (itmp0=getch()) != '@' );i++)
              {
               if(i1==0) putch_fl(itmp0);
              }
           if(i1==0)
           {
             putch_fl(0);
             putch_fl(0);

             f_delay_p(20000.);
             f_vect();

             f_delay_p(20000.);
            }
            else
            {
              puts("\n Program don't store !!! Program zone must be erased previously !!! ");
              puts("\n Please erase program zone by '$EP' command before store new program.\n");
            }

 #if (Cor_XY != 0)
  f_corXY_init();
 #endif // (Cor_XY != 0)
            if(kbhit() ) getch();
            if(kbhit() ) getch();
            if(kbhit() ) getch();
            if(kbhit() ) getch();
            if(kbhit() ) getch();
            if(kbhit() ) getch();
            if(kbhit() ) getch();

            goto fin;
           }
    /*---------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='P')&&(intrpr.wrd[2]=='1')  )
          if(intrpr.wdl==3)
         /* --------  command 'DP1'  ---- */
           {
            f_vect();
            f_prn_state();

            goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='P')&&(intrpr.wrd[2]=='3')  )
          if(intrpr.wdl==3)
         /* --------  command 'DP3'  ---- */
           {
            f_prn_prc();

            goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='P')&&(intrpr.wrd[2]=='4')  )
          if(intrpr.wdl==3)
         /* --------  command 'DP4'  ---- */
           {
            f_prn_prcs();

            goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='P')&&(intrpr.wrd[2]=='P')  )
          if(intrpr.wdl==3)
         /* --------  command 'DPP'  ---- */
           {
            f_more1();
            f_prn_proc();
            goto fin;
           }
    /*----------------------------------------------*/
#if(D_T != 0)
/*--------------------------------------------------*/
    if (intrpr.wrd[0]=='S')
      {
//------------- command  'SET'-----------------------
//------------- command  'SET'-----------------------
//------------- command  'SET'-----------------------

        if ( (intrpr.wrd[1]=='B')&& (intrpr.wrd[2]=='R') )
         /* --------  command 'SBR'  ---- */
         /* -----  Set Baud Rate --- */
           {
              if( f_in_val(3) > 0)
              {
                switch( ((int)intrpr.num_var) )
                {
                  case  1200:   baud_cod=BAUD_1200;   break;
                  case  2400:   baud_cod=BAUD_2400;   break;
                  case  4800:   baud_cod=BAUD_4800;   break;
                  case  9600:   baud_cod=BAUD_9600;   break;
                  case 19200:   baud_cod=BAUD_19200;  break;
                  case 38400:   baud_cod=BAUD_38400;  break;
                  default:  goto fin_e;
                }
              flag_DT_BR=1;
              Ini_RS232_DT();

              }
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='L')&& (intrpr.wrd[3]=='A') )
         /* --------  command 'SZLA'  ---- */
         /* -----  Set Z linear Acceleration --- */
           {
              if( f_in_val(4) > 0) var[Beg_A + Z_N] = intrpr.num_var;
              else goto fin_e;
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='I')&& (intrpr.wrd[2]=='D') )
         /* --------  command 'SID'  ---- */
         /* -----  Set ID --- */
           {
              if( f_in_val(3) > 0)
                if( (intrpr.num_var >= 0) && (intrpr.num_var<16) )
                  Contr_NN=intrpr.num_var;
                else goto fin_e;
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='L')&& (intrpr.wrd[3]=='D') )
         /* --------  command 'SZLD'  ---- */
         /* -----  Set Z linear Deceleration --- */
           {
              if( f_in_val(4) > 0) var[Beg_D + Z_N] = intrpr.num_var;
              else goto fin_e;
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='L')&& (intrpr.wrd[3]=='V') )
         /* --------  command 'SZLV'  ---- */
         /* -----  Set Z linear Velocity --- */
           {
              if( f_in_val(4) > 0) var[Beg_V + Z_N] = intrpr.num_var;
              else goto fin_e;
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='S')&& (intrpr.wrd[3]=='F') )
         /* --------  command 'SZSF'  ---- */
         /* -----  Set Z Smooth Factor --- */
           {
              if( f_in_val(4) > 0) var[Beg_SM + Z_N] = intrpr.num_var;
              else goto fin_e;
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='A')&& (intrpr.wrd[3]=='P') )
         /* --------  command 'SXAP'  ---- */
         /* -----  Set X Absolute Position --- */
           {
              if( f_in_val(4) > 0) var[Beg_AP + X_N] = intrpr.num_var;
              else goto fin_e;
              goto finx;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='T')&& (intrpr.wrd[2]=='A')&& (intrpr.wrd[3]=='P') )
         /* --------  command 'STAP'  ---- */
         /* -----  Set T(Y) Absolute Position --- */
           {
              if( f_in_val(4) > 0) var[Beg_AP + Y_N] = intrpr.num_var;
              else goto fin_e;
              goto finy;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='A')&& (intrpr.wrd[3]=='P') )
         /* --------  command 'SZAP'  ---- */
         /* -----  Set Z Absolute Position --- */
           {
              if( f_in_val(4) > 0) var[Beg_AP + Z_N] = intrpr.num_var;
              else goto fin_e;
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='V')&& (intrpr.wrd[2]=='A') )
         /* --------  command 'SVA'  ---- */
         /* -----  Set Vector Acceleration --- */
           {
              if( f_in_val(3) > 0) var[Beg_A + V_N] = intrpr.num_var;
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='V')&& (intrpr.wrd[2]=='V') )
         /* --------  command 'SVV'  ---- */
         /* -----  Set Vector Velocity --- */
           {
              if( f_in_val(3) > 0) var[Beg_V + V_N] = intrpr.num_var;
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='V')&& (intrpr.wrd[2]=='D') )
         /* --------  command 'SVD'  ---- */
         /* -----  Set Vector Deceleration --- */
           {
              if( f_in_val(3) > 0) var[Beg_D + V_N] = intrpr.num_var;
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='V')&& (intrpr.wrd[2]=='K') )
         /* --------  command 'SVK'  ---- */
         /* -----  Set Vector Kill deceleration --- */
           {
              if( f_in_val(3) > 0) var[Beg_K + V_N] = intrpr.num_var;
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='V')&& (intrpr.wrd[2]=='S') )
         /* --------  command 'SVS'  ---- */
         /* -----  Set Vector Smooth factor --- */
           {
              if( f_in_val(3) > 0) var[Beg_SM + V_N] = intrpr.num_var;
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='H')&& (intrpr.wrd[2]=='T') )
         /* --------  command 'SHT'  ---- */
         /* -----  Set HT parameter --- */
           {
              if( f_in_val(3) > 0) var[Beg_HT] = intrpr.num_var;
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='B')&& (intrpr.wrd[3]=='G') )
         /* --------  command 'SXBG'  ---- */
         /* -----  Set X BG parameter    --- */
           {
              if( f_in_val(4) > 0) var[Beg_BG + X_N] = intrpr.num_var;
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='H')&& (intrpr.wrd[2]=='I') )
         /* --------  command 'SHI'  ---- */
         /* -----  Set HIgh output --- */
           {
              if( f_in_val(3) > 0)
               {
                i1=intrpr.num_var;
                if( ( i1 > 16) || (i1 < 1.)) goto fin_e;

                *p_do[i1] = (int)*p_do[i1] |  msk_do[i1];
               }
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='L')&& (intrpr.wrd[2]=='O') )
         /* --------  command 'SLO'  ---- */
         /* -----  Set LOw output    --- */
           {
              if( f_in_val(3) > 0)
               {
                i1=intrpr.num_var;
                if( ( i1 > 16) || (i1 < 1.)) goto fin_e;

                *p_do[i1] = (int)*p_do[i1] & ~msk_do[i1];
               }
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='I')&& (intrpr.wrd[3]=='S') )
         /* --------  command 'SXIS'  ---- */
         /* -----  Set X input Source --- */
           {
              if( f_in_val(4) > 0)
               {
                if( (intrpr.num_var >0)&& (intrpr.num_var <17))
                 var[Beg_IS + X_N] = intrpr.num_var;
                else goto fin_e;
               }
              else goto fin_e;
              goto finx;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='T')&& (intrpr.wrd[2]=='I')&& (intrpr.wrd[3]=='S') )
         /* --------  command 'STIS'  ---- */
         /* -----  Set T input Source --- */
           {
              if( f_in_val(4) > 0)
               {
                if( (intrpr.num_var >0)&& (intrpr.num_var <17))
                 var[Beg_IS + Y_N] = intrpr.num_var;
                else goto fin_e;
               }
              else goto fin_e;
              goto finy;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='I')&& (intrpr.wrd[3]=='S') )
         /* --------  command 'SZIS'  ---- */
         /* -----  Set Z input Source --- */
           {
              if( f_in_val(4) > 0)
               {
                if( (intrpr.num_var >0)&& (intrpr.num_var <17))
                 var[Beg_IS + Z_N] = intrpr.num_var;
                else goto fin_e;
               }
              else goto fin_e;
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='I')&& (intrpr.wrd[2]=='N') )
         /* --------  command 'SIN'  ---- */
         /* -----  Set  INput logic --- */
           {
              if( f_in_val_i(3) > 0)
               {
                 msk_inv = intrpr.num_var_i;
               }
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/

//------------- command  'SET'-----------------------
//------------- command  'SET'-----------------------
//------------- command  'SET'-----------------------
      }
    /*----------------------------------------------*/

    if (intrpr.wrd[0]=='R')
      {
//------------- command  'REPORT' -----------------------
//------------- command  'REPORT' -----------------------
//------------- command  'REPORT' -----------------------

    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='B')&& (intrpr.wrd[2]=='R') )
         /* --------  command 'RBR'  ---- */
         /* -----  Report Baud Rate --- */
           {
             if(flag_DT_BR == 1)
             {
                    if(baud_cod == BAUD_1200 )  itmp=1200;
               else if(baud_cod == BAUD_2400 )  itmp=2400;
               else if(baud_cod == BAUD_4800 )  itmp=4800;
               else if(baud_cod == BAUD_9600 )  itmp=9600;
               else if(baud_cod == BAUD_19200)  itmp=19200;
               else if(baud_cod == BAUD_38400)  itmp=38400;
               else goto fin;
               printf("\n BR:%10d",itmp);
             }
             else printf(" - now in RUCH mode,apply 'SBRx' command first.");

             goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='I')&& (intrpr.wrd[2]=='D') )
         /* --------  command 'RID'  ---- */
         /* -----  Report ID --- */
           {
              printf("\n ID:%10d",(int)Contr_NN);
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='A') )
         /* --------  command 'RA'  ---- */
         /* -----  Report Analog input value    --- */
           {

              if( (var[20]<=4) && (var[20]>0)) i2=var[20]-1;
              else i2=0;
              itmp=ang[i2][8];

              if( f_in_val(2) > 0)
               {
                i1=intrpr.num_var;
                if( ( i1 > 8) || (i1 < 1.)) goto fin_e;

              if( (var[20]<=4) && (var[20]>0)) i2=var[20]-1;
              else i2=0;
              itmp=ang[i2][i1-1];
//           printf("\n  A%1d:%10d",i1,(int) (*p_da[i1]*scale_da[i1]) );
               printf("\n  A%1d:%10d",i1,itmp);
               }
              else goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='T')&& (intrpr.wrd[2]=='D')&& (intrpr.wrd[3]=='P') )
         /* --------  command 'RTDP'  ---- */
         /* -----  Report T (Y) Demand position --- */
           {

              ftmp= var[Ind_S_zad + X_N]*K_XDT1;
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;

              printf("\n TDP:%10d",(int)ftmp );
              goto finy;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='D')&& (intrpr.wrd[3]=='P') )
         /* --------  command 'RXDP'  ---- */
         /* -----  Report X Demand position --- */
           {
              ftmp= var[Ind_S_zad + Y_N]*K_XDT1;
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;

              printf("\n XDP:%10d",(int)ftmp );
              goto finx;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='D')&& (intrpr.wrd[3]=='P') )
         /* --------  command 'RZDP'  ---- */
         /* -----  Report Z Demand position --- */
           {

              ftmp= var[Ind_S_zad + Z_N]*K_ZDT1;
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;

              printf("\n ZDP:%10d",(int)ftmp );

              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='T')&& (intrpr.wrd[3]=='Z')&& (intrpr.wrd[4]=='D')&& (intrpr.wrd[5]=='P') )
         /* --------  command 'RXTZDP'  ---- */
         /* -----  Report XT(Y)Z Demand position --- */
           {
              ftmp= var[Ind_S_zad + Y_N]*K_XDT1;
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;
              printf("\n XDP:%10d",(int)ftmp);

              ftmp= var[Ind_S_zad + Z_N]*K_ZDT1;
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;
              printf(" ZDP:%10d",(int)ftmp );


              ftmp= var[Ind_S_zad + X_N]*K_XDT1;
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;
              printf(" TDP:%10d",(int)ftmp);

              goto finxyz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='T')&& (intrpr.wrd[3]=='Z')&& (intrpr.wrd[4]=='C')&& (intrpr.wrd[5]=='P') )
         /* --------  command 'RXTZCP'  ---- */
         /* -----  Report XT(Y)Z Current Position --- */
           {
              ftmp= var[Ind_ABS_LIN +1 + Y_N]*K_XDT1;
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;
              printf("\n XCP:%10d",(int)ftmp);

              ftmp= var[Ind_ABS_LIN +1 + Z_N]*K_ZDT1;
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;
              printf(" ZCP:%10d",(int)ftmp );


              ftmp= var[Ind_ABS_LIN +1 + X_N]*K_XDT1;
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;
              printf(" TCP:%10d",(int)ftmp);

              goto finxyz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='T')&& (intrpr.wrd[3]=='Z')&& (intrpr.wrd[4]=='A')&& (intrpr.wrd[5]=='P') )
         /* --------  command 'RXTZAP'  ---- */
         /* -----  Report XT(Y)Z Current Position --- */
           {
              ftmp= var[Beg_AP + X_N];
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;
              printf("\n XAP:%10d",(int)ftmp);

              ftmp= var[Beg_AP + Z_N];
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;
              printf(" ZAP:%10d",(int)ftmp );


              ftmp= var[Beg_AP + Y_N];
              if(ftmp > 0) ftmp+=0.5;
              else if (ftmp < 0) ftmp-=0.5;
              printf(" TAP:%10d",(int)ftmp);

              goto finxyz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='I')&& (intrpr.wrd[3]=='S') )
         /* --------  command 'RXIS'  ---- */
         /* -----  Report X input Source --- */
          {
              printf("\n XIS:%10d",(int)var[Beg_IS + X_N]);
              goto finx;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='T')&& (intrpr.wrd[2]=='I')&& (intrpr.wrd[3]=='S') )
         /* --------  command 'RTIS'  ---- */
         /* -----  Report T(Y) input Source --- */
          {
              printf("\n TIS:%10d",(int)var[Beg_IS + Y_N]);
              goto finy;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='I')&& (intrpr.wrd[3]=='S') )
         /* --------  command 'RZIS'  ---- */
         /* -----  Report Z input Source --- */
          {
              printf("\n ZIS:%10d",(int)var[Beg_IS + Z_N]);
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='I')&&(intrpr.wrd[2]=='N') )
         /* --------  command 'RIN'  ---- */
         /* -----  Report  INput logic --- */
          {
              printf("\n IN: 0X%8x",msk_inv);
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='A')&& (intrpr.wrd[3]=='P') )
         /* --------  command 'RXAP'  ---- */
         /* -----  Report X Absolute Position --- */
           {
              printf("\n XAP:%10d",(int)var[Beg_AP + X_N]);

finx:
              var[Beg_CA    ] = 1; // X
              var[Beg_CA+Y_N] = 0; // Y
              var[Beg_CA+Z_N] = 0; // Z
              goto fin;
finy:
              var[Beg_CA    ] = 0; // X
              var[Beg_CA+Y_N] = 1; // Y
              var[Beg_CA+Z_N] = 0; // Z
              goto fin;
finz:
              var[Beg_CA    ] = 0; // X
              var[Beg_CA+Y_N] = 0; // Y
              var[Beg_CA+Z_N] = 1; // Z
              goto fin;
finxyz:
              var[Beg_CA    ] = 1; // X
              var[Beg_CA+Y_N] = 1; // Y
              var[Beg_CA+Z_N] = 1; // Z
              goto fin;

          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='T')&& (intrpr.wrd[2]=='A')&& (intrpr.wrd[3]=='P') )
         /* --------  command 'RTAP'  ---- */
         /* -----  Report T Absolute Position --- */
           {
              printf("\n TAP:%10d",(int)var[Beg_AP + Y_N]);
              goto finy;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='A')&& (intrpr.wrd[3]=='P') )
         /* --------  command 'RZAP'  ---- */
         /* -----  Report Z Absolute Position --- */
           {
              printf("\n ZAP:%10d",(int)var[Beg_AP + Z_N]);
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='V')&& (intrpr.wrd[2]=='A') )
         /* --------  command 'RVA'  ---- */
         /* -----  Report Vector Acceleration --- */
           {
              printf("\n VA:%10d",(int)var[Beg_A + V_N]);
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='L')&& (intrpr.wrd[3]=='A') )
         /* --------  command 'RZLA'  ---- */
         /* -----  Report Z linear Acceleration --- */
           {
              printf("\n ZLA:%10d",(int)var[Beg_A + Z_N]);
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='V')&& (intrpr.wrd[2]=='V') )
         /* --------  command 'RVV'  ---- */
         /* -----  Report Vector Velocity --- */
           {
              printf("\n VV:%10d",(int)var[Beg_V + V_N]);
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='L')&& (intrpr.wrd[3]=='V') )
         /* --------  command 'RZLV'  ---- */
         /* -----  Report Z linear Velocity --- */
           {
              printf("\n ZLV:%10d",(int)var[Beg_V + Z_N]);
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='Z')&& (intrpr.wrd[2]=='L')&& (intrpr.wrd[3]=='D') )
         /* --------  command 'RZLD'  ---- */
         /* -----  Report Z linear Deceleration --- */
           {
              printf("\n ZLD:%10d",(int)var[Beg_D + Z_N]);
              goto finz;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='I')&& (intrpr.wrd[2]=='P') )
         /* --------  command 'RIP'  ---- */
         /* -----  Report Input Port --- */
           {
              puts("\n  IP:6543210987654321\n     ");
              itmp=var[BEG_IN];
              for(i=15;i>=0;i--)
               {
                if(itmp & msk_ip[i]) putch('1');
                else putch('0');
               }
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='O')&& (intrpr.wrd[2]=='P') )
         /* --------  command 'ROP'  ---- */
         /* -----  Report Output --- */
           {
              puts("\n  OP:6543210987654321\n     ");
              itmp=var[BEG_OUT];
              for(i=15;i>=0;i--)
               {
                if(itmp & msk_op[i]) putch('1');
                else putch('0');
               }
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[1]=='S')&& (intrpr.wrd[2]=='I') )
         /* --------  command 'RSI'  ---- */
         /* -----  Report System Inputs --- */
           {
              itmp=var[BEG_IN];
              puts("\n  SI:ES DRVs LR--TZYX\n      ");
              i=12;

              if(itmp & msk_si[i]) putch('1');
              else putch('0');
              puts(" ");
              i-=5;
              puts("1111");
              puts(" ");
              for(;i>=0;i--)
               {
                if(itmp & msk_si[i]) putch('1');
                else putch('0');
               }
              goto fin;
          }
    /*----------------------------------------------*/

//------------- command  'REPORT' -----------------------
//------------- command  'REPORT' -----------------------
//------------- command  'REPORT' -----------------------

      }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='0')&& (intrpr.wdl== 2) )
         /* --------  command 'T0'  ---- */
         /* -----  Tell motor and motion status --- */
           {

              Flag_ehl=-0.1;
              i_tmp = n_iter;
              while (i_tmp == n_iter);
              {
               itmp=0;
               if(Flag_ehl >= 0)
                 itmp |= 0x10;     // wait input

               if(Flag_sys[1] != 0)
                 itmp |= 0x08;     // fixed

               if( m_fin != m_beg )
                if( (move_p[m_beg].cod_c == 1) || (move_p[m_beg].cod_c == 3)  )
                 {
                   if( move_p[m_beg].k_x != 0)
                      itmp |= 0x01;      // move
                 }
                else if( move_p[m_beg].cod_c == 2 )
                      itmp |= 0x01;      // move

               if(var[Beg_HT] == 1)
                {  // long mode answer
                 puts("\nMotor X is ");
                 f_indt1(itmp);
                }
               else
                {
                 puts("\nX");
                 f_indt0(itmp);
                }
              }
      //---------------------------------------
       //     if(var[Beg_CA+3] !=0)
              {
               if(var[Beg_HT] == 1)
                {  // long mode answer
                 puts("\nMotor Y is disabled");
                }
               else
                {
                 puts("\nY dis MM 0");
                }
              }
      //---------------------------------------
           // if(var[Beg_CA+2] !=0)
              {
               itmp=0;
               if(Flag_ehl >= 0)
                 itmp |= 0x10;     // wait input

               if(Flag_sys[2] != 0)
                 itmp |= 0x08;     // fixed

               if( m_fin != m_beg )
                if( (move_p[m_beg].cod_c == 1) || (move_p[m_beg].cod_c == 3)  )
                 {
                   if( move_p[m_beg].k_z != 0)
                      itmp |= 0x01;      // move
                 }
                else if( move_p[m_beg].cod_c == 2 )
                      itmp |= 0x01;      // move

               if(var[Beg_HT] == 1)
                {  // long mode answer
                 puts("\nMotor Z is ");
                 f_indt1(itmp);
                }
               else
                {
                 puts("\nZ");
                 f_indt0(itmp);
                }
              }
      //---------------------------------------
              {
               itmp=0;
               if(Flag_ehl >= 0)
                 itmp |= 0x10;     // wait input

               if(Flag_sys[0] != 0)
                 itmp |= 0x08;     // fixed

               if( m_fin != m_beg )
                if( (move_p[m_beg].cod_c == 1) || (move_p[m_beg].cod_c == 3)  )
                 {
                   if( move_p[m_beg].k_y != 0)
                      itmp |= 0x01;      // move
                 }
                else if( move_p[m_beg].cod_c == 2 )
                      itmp |= 0x01;      // move

               if(var[Beg_HT] == 1)
                {  // long mode answer

                 puts("\nMotor T is ");
                 f_indt1(itmp);
                 puts("\nXT Motors are in common motion state");
                }
               else
                {
                 puts("\nT");
                 f_indt0(itmp);
                 puts("\nXT  common motion");
                }
              }
      //---------------------------------------
              goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='1')&& (intrpr.wdl== 2) )
         /* --------  command 'T1'  ---- */
         /* -----  Tell result of last 'begin motion' command --- */
           {
            if(var[Beg_HT] == 1)
             {  // long mode answer
               puts("\nResults of last BEGIN motion:");
               puts("\nX -  1");
               puts("\nY -  0");
               puts("\nZ -  1");
               puts("\nT -  1");
             }
            else
             {
               puts("\nBX 1   BY 1   ");
               puts("\nBZ 1   BT 1   ");
             }
             goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='2')&& (intrpr.wdl== 2) )
         /* --------  command 'T2'  ---- */
         /* -----  Tell the cause of motions end --- */
           {
            if(var[Beg_HT] == 1)
             {  // long mode answer
               puts("\nResults of last END motion:");
               puts("\nX -  1");
               puts("\nY -  0");
               puts("\nZ -  1");
               puts("\nT -  1");
             }
            else
             {
               puts("\nEX 1   EY 1   ");
               puts("\nEZ 1   ET 1   ");
             }
             goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='3')&& (intrpr.wdl== 2) )
         /* --------  command 'T3'  ---- */
         /* -----  Tell the program status --- */
           {
            if(var[Beg_HT] == 1)
             {  // long mode answer

              if(f_get_active())
puts("\nProgram is running");
              else
puts("\n\nProgram is NOT running");
puts("\nProgram is compiled.");
puts("\nProgram area is OK");
puts("\nProgram check sum is 0000");
             }
            else
             {
              if(f_get_active())
                 printf("\nprog run l# 366");
              else
                 printf("\nprog not running");
              printf("\nArea OK CS:0000");
             }
             goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='8')&& (intrpr.wdl== 2) )
         /* --------  command 'T8'  ---- */
         /* -----  Tell voltages  status --- */
           {
            if(var[Beg_HT] == 1)
             {  // long mode answer
              puts("\nVpp test is OK");
              puts("\n+12V test is OK");
              puts("\n-12V test is OK");
             }
            else
             {  //short mode answer
              puts("\nVpp.+12V-12V");
              puts("\n Ok  Ok  Ok");
             }
            goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E')&&(intrpr.wrd[1]=='C')&& (intrpr.wrd[2]=='H') && (intrpr.wrd[3]=='O'))
         /* --------  command 'ECHO'  ---- */
         /* -----  Echo ON-OFF --- */
           {
              if(f_dis_set(&ds_echo,4,1)<0 ) goto fin_e;
              goto fin;
          }
    /*----------------------------------------------*/
//      if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='X') && (intrpr.wdl ==2)  )
        if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='X') )
         /* --------  command 'PX'  ---- */
           {
            if(kol_err) goto fin_e;
            intrpr.i3=2;
px1:        if(f_inw())
             {

               if(intrpr.wdl >= n_fname)
                {
                  goto fin_e;
                }
//--------------------------------------

                if(strcmp(nm_rep,intrpr.wrd) == 0)
                {
                  if(f_run(intrpr.wrd) ) goto fin;
                  goto fin_e;

                }

//----------------------
#if(AUTOM != 0)
                 for(i=0;i<N_aut;i++)
                  {
                    if(ff_autom[i] != NULL)
                    {
                  //  ff_autom[i] = NULL;
                  //  au_sw[i]=0;
                      f_err_ea(NEST_ERR,0);
                      goto fin_e;
                    }
                  }
#endif // (AUTOM != 0)
//----------------------

                 // Stop all active process
                 for(i=0;i<n_max;i++)
                  {
                   if(intrr[i].sw)
                   {
                     if(strcmp(nm_error,intrr[i].name) == 0) ;
                     else if(strcmp(nm_warn,intrr[i].name) == 0) ;
                     else if(strcmp(nm_drive,intrr[i].name) == 0) ;
                     else if(strcmp(nm_init,intrr[i].name) == 0) ;
                     else fcloss(i);
                   }
                  }
//--------------------------------------

               if(f_innum(intrpr.wrd) )
                {
                  i2=intrpr.num_var;
                  if((i2==616)||(i2==615))
                   {
                     if(f_run(name_msc) ) goto fin;
                     else  goto fin_e;
                   }
                  goto fin_e;
                }

              if(f_run(intrpr.wrd) ) goto fin;
             }
            goto fin_e;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&&(intrpr.wrd[1]=='V')&& (intrpr.wrd[2]=='S')&& (intrpr.wrd[3]=='E') )
         /* --------  command 'AVSE'  ---- */
         /* -----  Receive ARRAY DATA  - var[4000 ... 4100] --- */
           {
             if(intrpr.wdl==4)
                 if(f_inw()) strg=intrpr.wrd;
                 else  {  goto fin_e; }
             else
                  strg=&intrpr.wrd[4];

             if(f_innum(strg) ) i1= intrpr.num_var;   /* begin index */
             else  { goto fin_e; }
             if(i1 > 100) goto fin_e;

             if(!f_inw()) { goto fin_e; }
             if(f_innum(intrpr.wrd) )
              {
               var[Beg_arrv +i1]  = intrpr.num_var;  // input first value
               i1++;
              }
             else  { goto fin_e; }

             while(1)
             {  // input value in cycle before string end

                if(!f_inw())  goto fin;
                if(i1 > 100) goto fin_e;
                if(f_innum(intrpr.wrd) )
                 {
                  var[Beg_arrv +i1]  = intrpr.num_var;
                  i1++;
                 }
                else  { goto fin_e; }
             }
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&&(intrpr.wrd[1]=='V')&& (intrpr.wrd[2]=='R')&& (intrpr.wrd[3]=='E') )
         /* --------  command 'AVRE'  ---- */
         /* -----  Display ARRAY DATA  - var[4000 ... 4100] --- */
           {
             if(intrpr.wdl==4)
                 if(f_inw()) strg=intrpr.wrd;
                 else  { i1=0;i2=99; goto a_dis; }
             else
                  strg=&intrpr.wrd[4];

             if(f_innum(strg) ) {i1= intrpr.num_var;}  /* begin index */
             else  { goto fin_e; }

             if(!f_inw())
               {
                i2 = i1;             /* end index */
                goto a_dis;
               }
             if(f_innum(intrpr.wrd) )
              {
               i2= intrpr.num_var;   /* end index */
              }
             else  { goto fin_e; }

             if( (i1>99)|| (i1<0) || (i2>99) || (i2<0) ) goto fin_e;

    a_dis:
             for(;i1<=i2;i1++)
             { // display array elements value

                printf(" %d",(int)var[Beg_arrv +i1]);
             }
             goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E')&&(intrpr.wrd[1]=='X')&&(intrpr.wrd[2]=='T')&&(intrpr.wrd[3]=='Z') )
    /* --------  команда 'EXTZ'   ----------------------- */
          {
           f_clr_bufe();
           goto fin;
          }
    /*--------------*/
        if ( (intrpr.wrd[0]=='E')&&(intrpr.wrd[1]=='X')&&(intrpr.wrd[2]=='T') )
    /* --------  команда 'EXT'   ----------------------- */
          {
           f_clr_bufe();
           goto fin;
          }
    /*--------------*/
#endif // (D_T != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='R')&&(intrpr.wrd[1]=='U')&& (intrpr.wrd[2]=='N') )
         /* --------  command 'RUN'  ---- */
           {
            if(kol_err) goto fin_e;
            if(f_inw())
             {
               if(intrpr.wdl >= n_fname)
                {
                  goto fin_e;
                }
 mcl1:
              if(f_run(intrpr.wrd) ) goto fin;
             }
             else
             {
              if(kol_par_sub)
               if(par_subr[0].name[0])
                {
                   strcpy(intrpr.wrd,par_subr[0].name);
                   putch_ea(' ');
                   puts(intrpr.wrd);
                   goto mcl1;
                }
             }
            goto fin_e;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&&(intrpr.wrd[1]=='T')&& (intrpr.wrd[2]=='O') && (intrpr.wrd[3]=='P'))
         /* --------  command 'STOP'  ---- */
           {
            if(f_inw())
             {
               if(intrpr.wdl >= n_fname)
                {
                  goto fin_e;
                }
               for(i=0;i<n_max;i++ )
                 if(strcmp(intrpr.wrd,intrr[i].name) == 0)
                  {
                   if(intrr[i].sw)
                   {
                     fcloss(i);
                     goto fin;
                   }
                  }
#if(AUTOM != 0)
             for(i=0;i<N_aut;i++ )
               if(strcmp(intrpr.wrd,autom_nm[i]) == 0)
                {
                   for(ii=0;ii<N_aut;ii++)
                   {
                    if(ff_autom[ii] == ff_automp[i])
                    {
                      ff_autom[ii]=NULL;
                      goto fin;
                    }
                   }
                }
#endif // (AUTOM != 0)
             // not founded

               goto fin_e;
             }
             else
             {
 ss0:
              // Stop all active process
               for(i=0;i<n_max;i++)
                   if(intrr[i].sw)
                   {

#if(D_T != 0)
                     if(strcmp(nm_drive,intrr[i].name) == 0) ;
                     else
#endif // (D_T != 0)

                     if(strcmp("&&LN",intrr[i].name) == 0) ;
                     else if(strcmp("&&FNP",intrr[i].name) == 0) ;
                     else
                       fcloss(i);
                   }

#if(AUTOM != 0)
                 for(i=0;i<N_aut;i++)
                    ff_autom[i] = NULL;
#endif // (AUTOM != 0)

               goto fin;
             }
            goto fin_e;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='P')  )
          if(intrpr.wdl==2)
         /* --------  command 'DP'  ---- */
           {
            puts("\n");
            put_text_flash(0);
            goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='B')  )
          if(intrpr.wdl==2)
         /* --------  command 'DB'  ---- */
           {
            puts("\n");
            put_version( ( (int) (&buf_c[0]) )<<2);
            goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='B')&&(intrpr.wrd[2]=='R')  )
          if(intrpr.wdl==3)
         /* --------  command 'DBR'  ---- */
           { // send in binary mode Input cyclic buffer RS232
            putch_ea(0);
            i1= n_buf;
            j= n_buf/4;

            putch2(i1);
           i2=uk_fin;
           for(i=0;i<n_buf;i++)
             {
               i1= f_ch_c(i2);
               putch(i1);
               i2++;
               i2 &= (n_buf-1);
             }
            goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='B')&&(intrpr.wrd[2]=='U')&&(intrpr.wrd[3]=='I')  )
          if(intrpr.wdl==4)
         /* --------  command 'DBUI'  ---- */
           { // send in binary mode Input cyclic buffer USB
            putch_ea(0);
            i1= n_buf_USB;
            j= n_buf_USB/4;
            putch2(i1);

            i2=uk_fin_USB;
            for(i=0;i<n_buf_USB;i++)
             {
               i1= f_ch_c_USB(i2);
               putch(i1);
               i2++;
               i2 &= (n_buf_USB-1);
             }

            goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='B')&&(intrpr.wrd[2]=='U')&&(intrpr.wrd[3]=='O')  )
          if(intrpr.wdl==4)
         /* --------  command 'DBUO'  ---- */
           { // send in binary mode Output cyclic buffer USB
            putch_ea(0);
            i1= n_buf_USB;
            putch2(i1);

            i2=uk_fin_USBw;
            for(i=0;i<n_buf_USB;i++)
             {
               i1= f_ch_c_USBw(i2);
               putch(i1);
               i2++;
               i2 &= (n_buf_USB-1);
             }

            goto fin;
           }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='R')&& (intrpr.wrd[1]=='R') )
         /* --------  команда 'RR' ------------ */
          {

//--------------------------------------------------
#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
                close_usb();
#endif

#define  MAGIC_MONITOR_KEY      0x5aa5a55a

            *( unsigned int *)(ID_ADDR) = MAGIC_MONITOR_KEY;
//--------------------------------------------------
            disable_int();
            disable_INT1();
            disable_INT2();
            disable_TINT0();
//--------------------------------------------------
            fl_get=1;
            boot_ld();
            goto fin;
          }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='I' )
         /* --------  команда 'IX','IY','IZ','IC' ------ */
          {
              if( (i=f_xyzcbu(2)) >= 0 )
               {
#if(PLANAR == 0)
                if(m_adc[i] > 0  )  itmp= f_get_ibd(m_adc[i]);    //=type_amp[((int)(m_adc[i]-1)) >> 2];
                else itmp = NO_BOARD;
                i1=m_adc[i];

#else  // (PLANAR != 0)

                if(m_adc[i+1] > 0  )  itmp =f_get_ibd(m_adc[i+1]); // = type_amp[((int)(m_adc[i+1]-1)) >> 2];
                else itmp = NO_BOARD;
                i1=m_adc[i+1];
#endif   // (PLANAR == 0)

//              printf ( " SevoBoard %d , channel %d\n" ,itmp, i1);
                if(intrpr.wdl==2)
                    if(f_inw()) strg=intrpr.wrd;
                    else
                        {
                           // display index mark flag
                           if(f_im_tst[i](i) != 0 )
                              puts(" 1");
                           else puts(" 0");

                           // display index mark directly
                           if( (itmp == UC612) || (itmp== DEP44_OLD) )
                           {
                                if(f_im_dix[i](i) != 0 )
                                   puts(" 1");
                                else puts(" 0");
                           }
                           goto fin;
                        }
                else
                     strg=&intrpr.wrd[2];
                if(sscanf(strg, "%d", &itmp0) )
                  {
                   if(itmp0 == 0)
  /* отключение схемы сброса по реперу */
                    {
                      f_im_res[i](i);
                    }
                   else
                    {
                      f_im_set[i](i);
                    }
                  }
                else  goto fin_e;
                goto fin;
               }
          }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='T'&&(intrpr.wrd[1]=='K')&&( (i=f_xy(3)) >= 0 ) )
         /* -----   command 'TKX','TKY'        ---- */
         /* -----  Receive plate Korrective Table --- */
           {
             if(intrpr.wdl==3)
                 if(f_inw()) strg=intrpr.wrd;
                 else  {  goto fin_e; }
             else
                  strg=&intrpr.wrd[3];

             if(f_innum(strg) ) i1= intrpr.num_var;   /* begin index */
             else  { goto fin_e; }

             if(!f_inw()) { goto fin_e; }
             if(f_innum(intrpr.wrd) ) itmp1 = intrpr.num_var;   /* quantity values */
             else  { goto fin_e; }

          if ( (i1<0) || ( (itmp1+i1) >  Cor_table_Length * (2-i) )) { goto fin_e; }

             for(itmp0=0;itmp0<itmp1;i1++,itmp0++)
               {
                 while(!f_inw())
                 {
                   puts ("\n");
  mmtk0:
      if(ffgets() )      // input from buf_c[] to command string buffer 'cb_RS232[]'
        {

         RS_n_bcom=0;
         strcpy(intrpr.bfcom,cb_RS232);
         goto mmtk1;
        }

      else if(ffgets_USB() )      // input from buf_c_USB[] to command string buffer 'cb_USB[]'
        {

         USB_n_bcom=0;
         strcpy(intrpr.bfcom,cb_USB);
         goto mmtk1;
        }
      else
       {
#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
   if( flag_usb == 1)    f_usb();
#endif
         goto mmtk0;
       }
   mmtk1:
                   intrpr.i3=0;

                 }
                 if(f_innum(intrpr.wrd) )
                   uk_t_kor[i+1][i1] = intrpr.num_var;
                 else {
                  //     puts(intrpr.bfcom);
                  //     printf( "5");
                         goto fin_e;
                      }
               }
//-----------------------------
#if (COR_INF != 0)
  f_cori_init();
#endif  // (COR_INF != 0)

/*
#if (Cor_XY != 0)
  f_corXY_init();
#endif // (Cor_XY != 0)
*/

#if( (COR_INF == 0)  && (Cor_XY == 0))
  f_cor_init();
#endif // ( (COR_INF == 0)  && (Cor_XY == 0))
//-----------------------------

            goto fin;
           }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='T'&&(intrpr.wrd[1]=='K')&&(intrpr.wrd[2]=='S'))
         /* -----   command 'TKS'        ---- */
         /* -----  Receive Surface signal correction Table --- */
           {
             if(intrpr.wdl==3)
                 if(f_inw()) strg=intrpr.wrd;
                 else  {  goto fin_e; }
             else
                  strg=&intrpr.wrd[3];

             if(f_innum(strg) ) i1= intrpr.num_var;   /* begin index */
             else  { goto fin_e; }

             if(!f_inw()) { goto fin_e; }
             if(f_innum(intrpr.wrd) ) itmp1 = intrpr.num_var;   /* quantity values */
             else  { goto fin_e; }

          if ( (i1<0) || ( (itmp1+i1) >  Cor_Surf_Lgth   )) { goto fin_e; }

             for(itmp0=0;itmp0<itmp1;i1++,itmp0++)
               {
                 while(!f_inw())
                 {
                   puts ("\n");
  mmtks0:
      if(ffgets() )      // input from buf_c[] to command string buffer 'cb_RS232[]'
        {

         RS_n_bcom=0;
         strcpy(intrpr.bfcom,cb_RS232);
         goto mmtks1;
        }

      else if(ffgets_USB() )      // input from buf_c_USB[] to command string buffer 'cb_USB[]'
        {

         USB_n_bcom=0;
         strcpy(intrpr.bfcom,cb_USB);
         goto mmtks1;
        }
      else
       {
#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
   if( flag_usb == 1)    f_usb();
#endif
         goto mmtks0;
       }
   mmtks1:
                   intrpr.i3=0;

                 }
                 if(f_innum(intrpr.wrd) )
                   Cor_surf[i1] = intrpr.num_var;
                 else {
                  //     puts(intrpr.bfcom);
                  //     printf( "5");
                         goto fin_e;
                      }
               }
//-----------------------------
          if(Cor_surf[0]<= 0)
          {
            Cor_surf[0]= 2048;
            Cor_surf[1]=-2048;
            Cor_surf[2]=0;
            Cor_surf[3]=2048;
          }
          TT_PER_1_Surf=1./Cor_surf[0];
//-----------------------------

            goto fin;
           }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='N'&&(intrpr.wrd[1]=='K')&&( (i=f_xy(3)) >= 0 ) )
         /* -----  Display Corrective Table --- */
         /* -----   command 'NKX','NKY'        ---- */
           {
             if(intrpr.wdl==3)
                 if(f_inw()) strg=intrpr.wrd;
                 else  goto fin_e;
             else
                  strg=&intrpr.wrd[3];

             if(f_innum(strg) ) i1= intrpr.num_var;   /* begin index */
             else goto fin_e;

             itmp1=1;
             if(f_inw())
              if(f_innum(intrpr.wrd) ) itmp1= intrpr.num_var;   /* quantity values */
              else goto fin_e;

             for(itmp0=0;itmp0<itmp1;i1++,itmp0++)
               {
                printf(" %f",(float)uk_t_kor[i+1][i1]);
                if((itmp0 % 8) == 0) printf("\n");
#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
   if( flag_usb == 1)    f_usb();
#endif
               }
            goto fin;
           }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='N'&&(intrpr.wrd[1]=='K')&&(intrpr.wrd[2]=='S') )
         /* -----  Display  Surface signal Corrective Table --- */
         /* -----   command 'NKS'        ---- */
           {
             if(intrpr.wdl==3)
                 if(f_inw()) strg=intrpr.wrd;
                 else  goto fin_e;
             else
                  strg=&intrpr.wrd[3];

             if(f_innum(strg) ) i1= intrpr.num_var;   /* begin index */
             else goto fin_e;

             itmp1=1;
             if(f_inw())
              if(f_innum(intrpr.wrd) ) itmp1= intrpr.num_var;   /* quantity values */
              else goto fin_e;

             for(itmp0=0;itmp0<itmp1;i1++,itmp0++)
               {
                printf(" %f",Cor_surf[i1]);
                if((itmp0 % 8) == 0) printf("\n");
#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
   if( flag_usb == 1)    f_usb();
#endif
               }
            goto fin;
           }
    /*----------------------------------------------*/

        if ( intrpr.wrd[0]=='D'&&(intrpr.wrd[1]=='K')&&(intrpr.wrd[2]=='S') )
         /* -----  Display  Surface signal Corrective Table --- */
         /* -----   command 'DKS'        ---- */
           {
              if(var[Ind_tbl_indx] >= Cor_Surf_Lgth)
                var[Ind_tbl_indx] = Cor_Surf_Lgth-1;
              else if(var[Ind_tbl_indx] < 0)
                 var[Ind_tbl_indx] = 0;

              i2=var[Ind_tbl_indx];// индекс в таблице
              i1=Cor_Surf_Lgth - var[Ind_tbl_indx];
              if(i1>8) i1=8;       // количество операндов

              kk_p= Cor_surf[i2] ;
              i2++; if(i2>=Cor_Surf_Lgth)  goto mdks1;
              kk_i= Cor_surf[i2] ;
              i2++; if(i2>=Cor_Surf_Lgth)  goto mdks1;
              kk_d= Cor_surf[i2] ;
              i2++; if(i2>=Cor_Surf_Lgth)  goto mdks1;
              kkat= Cor_surf[i2] ;
              i2++; if(i2>=Cor_Surf_Lgth)  goto mdks1;
              kk_f= Cor_surf[i2] ;
              i2++; if(i2>=Cor_Surf_Lgth)  goto mdks1;
             tmpf1= Cor_surf[i2] ;
              i2++; if(i2>=Cor_Surf_Lgth)  goto mdks1;
             tmpf2= Cor_surf[i2] ;
              i2++; if(i2>=Cor_Surf_Lgth)  goto mdks1;
             tmpf3= Cor_surf[i2] ;
mdks1:
             if( (itmp1=f_dis_set(ds_kf,3,i1)) == 0 ) goto fin;
             else if(itmp1<0) goto fin_e;

             i2=var[Ind_tbl_indx];// индекс в таблице
       Cor_surf[i2] =  kk_p;
       i2++;
       if(i2>=Cor_Surf_Lgth) goto fin;
       Cor_surf[i2] =  kk_i;
       i2++;
       if(i2>=Cor_Surf_Lgth) goto fin;
       Cor_surf[i2] =  kk_d;
       i2++;
       if(i2>=Cor_Surf_Lgth) goto fin;
       Cor_surf[i2] =  kkat;
       i2++;
       if(i2>=Cor_Surf_Lgth) goto fin;
       Cor_surf[i2] =  kk_f;
       i2++;
       if(i2>=Cor_Surf_Lgth) goto fin;
       Cor_surf[i2] = tmpf1;
       i2++;
       if(i2>=Cor_Surf_Lgth) goto fin;
       Cor_surf[i2] = tmpf2;
       i2++;
       if(i2>=Cor_Surf_Lgth) goto fin;
       Cor_surf[i2] = tmpf3;
            goto fin;
           }

    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='H')&&(intrpr.wrd[1]=='L')  )
          if(intrpr.wdl==2)
         /* -----   command 'HL'             ---- */
           {
            adr_putch_ram =(int)(&hlp[0])<<2;    // set begin address in EEPROM
            for(i=0;(i<L_HLP)&&(itmp0=getch());i++)
              {
               putch_ram(itmp0);
              }
            putch_ram(0);
            goto fin;
           }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
    // I2tP
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='W'))
         /* --------  command 'TW'  ------------ */
          {
                 kk_p= Tmot[0]+tCM[0];
                 kk_i= Tmot[1]+tCM[1];
                 kk_d= Tmot[2]+tCM[2];
                 kkat= Tmot[3]+tCM[3];
                 kk_f= Tmot[4]+tCM[4];
                tmpf1= Tmot[5]+tCM[5];
                tmpf2= Tmot[6]+tCM[6];

                 if(f_dis_set(ds_kf,2,7)>0)
                  {
                   disable_int();
                   tCM[0] = kk_p - Tmot[0];
                   tCM[1] = kk_i - Tmot[1];
                   tCM[2] = kk_d - Tmot[2];
                   tCM[3] = kkat - Tmot[3];
                   tCM[4] = kk_f - Tmot[4];
                   tCM[5] =tmpf1 - Tmot[5];
                   tCM[6] =tmpf2 - Tmot[6];
                   enable_int();
                  }
                 goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='1'))
         /* --------  command 'T1'  ------------ */
          {
           if(f_dis_set(ds_t1,2,7)>0)  f_cfg_count();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='T')&&(intrpr.wrd[2]=='1'))
         /* --------  command 'T1'  ------------ */
          {
           if(f_dis_set(ds_t1,3,7)>0)  f_cfg_count();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='C'))
         /* --------  command 'TC'  ------------ */
          {
           if(f_dis_set(ds_tc,2,7)>0)  f_cfg_count();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='O'))
         /* --------  command 'TO'  ------------ */
          {
           if(f_dis_set(ds_to,2,7)>0)  f_cfg_count();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='M'))
         /* --------  command 'NM'  ------------ */
          {
           if(f_dis_set(ds_nm,2,7)>0)  f_cfg_count();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='C') )
         /* --------  'SSCX''SSCY''SSCZ''SSCC'   ------------ */
          {
            if( (i = f_xyzcbu(4)) >= 0 )
            {
               tmpf1=v_nom_a[i];
               tmpf2=v_rot_a[i];
               tmpf3=I_a_nom[i];     // номинальный ток в масштабе усилителя
               tmpf4=I_a_max[i];     // максимальный ток в масштабе усилителя
               tmpf5=Flux[i];        // ток возбуждения асинхронного мотора в масштабе усилителя

               if(f_dis_set(ds_ssc,4,5 ) > 0 )
               {
                 v_nom_a[i] = tmpf1;
                 v_rot_a[i] = tmpf2;  // номинальный ток в масштабе усилителя
                 I_a_nom[i] = tmpf3;
                 I_a_max[i] = tmpf4;
                 Flux[i]    = tmpf5;      // ток возбуждения асинхронного мотора в масштабе усилителя

                 f_count_ssc();
                 Inom[i]= I_a_nom[i]/MAX_AMPL;
                 f_cfg_count();

               }
            }
            else goto fin_e;

            goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='?' )
         /* --------  command ? ------------ */
          {
            puts("\n");
            put_version( ( (int) (&hlp[0])  )<<2);
            goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='1' )
         /* --------  команда Время 1 ------------ */
          {    // '1 '
    /*
         f_tim0();
         f_tim1();
         min_max_t1();
    */
             print_t1();
             goto fin;
          }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='3' )
         /* --------  команда Время 3  '3 ' ------------ */
          {

  f_tim4();
         f_dd();
  f_tim5();
  min_max_t3();
             print_t3();
             goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&& (intrpr.wrd[1]=='B') )
         /* --------  'CB' - команда Очистить  буфер записи ---- */
          {
           if(fl_trace_loop == 0)
              uk_buf=0;
           min_t1=0x7fffffff;max_t1=0;
           min_t2=0x7fffffff;max_t2=0;
           min_t3=0x7fffffff;max_t3=0;
#if( (USB_BOARD != 0)|| (PROC_TYPE == C33))
           f_clear_buf();  // Clear USB debug buffers
#endif

           goto fin;
          }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='O' )
          if(intrpr.wdl==1)
         /* - 'O '-  команда Передать наверх буфер записи ---- */
          {
            if(f_inw()) goto fin_e;

            if(fl_trace_loop == 0)
            {
              putch_ea(0);
              j= uk_buf;
              i1=j * 4;
              putch2(i1);

              for(i2=0;i2<5;i2++)
              {
                if(f_ns[i2] == 0.) break;
              }

              for(i=0;i<j;i++)
                {
                 ffl.ff_f= buf_xyz[i];
                   if(i2 != 0)
                   {
                     if(ffl.ff_f==0.) ffl.ff_f=0.00000000001;
                     ffl.ff_i &= 0xfffffff0;
                     ffl.ff_i |= (unsigned long int) (i2 & 0xf);
                     i2=0;
                   }
                 putch4( ffl.ff_i);
                }
              goto fin;
            }
            else
            {

              fl_trace=1;
              putch_ea(0);

              for(i2=0;i2<5;i2++)
              {
                if(f_ns[i2] == 0) break;
              }

              j= (int)((dim_xyz-6)/i2) * i2;
              j+=i2;
              i1=j * 4;
              putch2(i1);
              for(i=uk_buf,i1=0;i1<j;i1++,i++)
                {
                   if(i>=j) i=0;
                   ffl.ff_f= buf_xyz[i];
                   if(i2 != 0)
                   {
                     if(ffl.ff_f==0.) ffl.ff_f=0.00000000001;
                     ffl.ff_i &= 0xfffffff0;
                     ffl.ff_i |= (unsigned long int) (i2 & 0xf);
                     i2=0;
                   }

                   putch4(ffl.ff_i);
                }

              fl_trace=0;
              goto fin;
            }

          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='O')&& (intrpr.wrd[1]=='A'))
         /* --------  команда Передать наверх таблицу арктангенсов ---- */
          {
           switch(intrpr.wrd[2])
           {
            case '1': itmp0=0; goto m_o2;
            case '2': itmp0=1; goto m_o2;
            case '3': itmp0=2; goto m_o2;
           }
            goto fin_e;

m_o2:
           disable_int();
           f_tim2();
           f_arctg();
           f_tim3();
           enable_int();
           min_max_t2();

           putch_ea(0);
           putch2(N_ARC+N_ARC+N_ARC+N_ARC );
           for(i=0;i<N_ARC;i++)
            {
              ffl.ff_f= UK_ATAN[itmp0][i];
              putch4(ffl.ff_i);
            }
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='O')&& (intrpr.wrd[1]=='M'))
         /* --------  команда Передать наверх данные глобального массива -- */
          {  // 'OM' , "OM"
          //   UV.SV.u_Arr_o[0] - начальный индекс в глобальном массиве
          //   UV.SV.u_Arr_o[1] - количество передаваемых байт


           f_dis_set(ds_om,2,2);

           i2=UV.SV.u_Arr_o[0];
           i1=UV.SV.u_Arr_o[1];

           ftmp1=UV.SV.u_Arr_o[0];
           ftmp2=UV.SV.u_Arr_o[1];

//         printf("\n i1= %d , i2 = %d , n_svar= %d",i1,i2,(int) n_svar);
//         printf("\n UV.SV.u_Arr_o[0]= %f , UV.SV.u_Arr_o[1] = %f ",ftmp1,ftmp2);

           if( (i1 > n_svar) || (i1 < 0) ) goto fin_e;
           if( ( (i2+i1) > n_svar) || (i2 <= 0) ) goto fin_e;

           putch_ea(0);
           putch2(i2<<2);
           for(i=0;i<i2;i++)
            {
              ffl.ff_f= var[i1+i];
              putch4(ffl.ff_i);
            }
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='V')&&(intrpr.wrd[1]=='T')  )
        // 'VT'
          if(intrpr.wdl==2)
         /* --------  команда Передать наверх сохраняемые переменные ---- */
          {
           if(f_inw()) goto fin_e;

           for(i=0;ee_mas[i] != NULL;i++) ;
           putch_ea(0);
           j=i+i+i+i+4+3;
           putch2(j);
           putch_ea('V');
           putch_ea('L');
           putch_ea(0x0d);
           putch4(i);
           for(j=0;j<i;j++)
            {
              uu1.fl=*(float *)ee_mas[j];
              putch4(uu1.it);
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='V')&&(intrpr.wrd[1]=='L')  )
        // 'VL'
          if(intrpr.wdl==2)
         /* --------  команда Принять сохраняемые переменные ---- */
           {
           if(f_inw()) goto fin_e;

           ftmp1=n_id;
           ftmp=k_t_mks_b+4;
           uk_get=getch;
           i=getch4();
           i=i-1;
           for(j=0;j<i;j++)
            {
              uu1.it=getch4();

              if( ((float *)ee_mas[j]) == &Contr_NN  ){ ftmp2 = uu1.fl;}
              else if( ((float *)ee_mas[j]) == &k_t_mks  )
               {
                ftmp = uu1.fl;
             // printf("k_t_mks= %f\n",ftmp);
               }
              else if( ((float *)ee_mas[j]) == &t_int  )
               {
                ftmp3 = uu1.fl;
             // printf("t_int= %f\n",ftmp3);
               }
              else *(float *)ee_mas[j]=uu1.fl;
            }
           uu1.it=getch4();
           n_id=ftmp1;
//         ftmp4=(ftmp3/ftmp); // new t_int
//         t_int=(ftmp3/ftmp)*(k_t_mks_b);

//           printf("\n TI %f;\n DSF %4f;",ftmp4,ftmp*4);
           if(ftmp > k_t_mks_b)
           {
     //     k_t_mks = k_t_mks_b;
     //     ds_ti.div  = k_t_mks_b;
            ftmp4=ftmp3/ftmp; // new t_int
            printf("\n !Have been loaded configuration for faster DSP:\n  Loaded data for DSP %4f MHz ; DSP board %4f Mhz.",ftmp*4,k_t_mks_b*4);
            printf("\n Set manually interrupt period :\n TI %f ; -  !Hang up is possible",ftmp4);
           }
           else
             t_int=(ftmp3/ftmp)*(k_t_mks);

           Contr_NN = ftmp2;
           if(kbhit() ) getch();
           if(kbhit() ) getch();
//----------------------------------------
           Baud_check();
//----------------------------------------
           disable_int();
           f_v_a();
           f_tim0_set();
           f_count_cf();
           f_ns_count();

  #if ( NOVS != 0)
        f_init_twl();
  #endif // ( NOVS != 0)

           enable_int();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&& (intrpr.wrd[1]=='C') )
         /* --------  команда 'CC' "CC"  ------------ */
         /*  'CC' - interpolation aCCuracy & Max ANgle between vectors */
/*CC - interpolation aCCuracy (mkm),max ANgle between vectors(deg)  */

          {
           itmp1=f_dis_set(ds_cc,2,2);
           if(itmp1>1)
                sin_max=sin(angle*PI/360.);
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&& (intrpr.wrd[1]=='O') )
         /* --------  команда 'CO' "CO"  ------------ */
         /*  'CO' - set flag last segment for conjugation */
          {
           kk_p= var[i_fl_cc];
           if(f_dis_set(ds_kf,2,1)>0)
           {
             var[i_fl_cc]=kk_p;
           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F')&& (intrpr.wrd[1]=='E')&& (intrpr.wrd[2]=='A') )
         /* --------  команда 'FEA'  ------------ */
          {
           itmp1=f_dis_set(ds_fea,3,2);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='R'))
         /* --------  команда 'RX','RY','RZ','RC'  ------------ */
          {
           if(intrpr.wdl==2)
            {
              if( (i=f_xyzcbu(2)) >= 0 )
                {

#if(PLANAR == 0)
                  if(f_inw()) goto fin_e;
                  disable_int();

                  var[I_p_max +i]=BIG_P;
                  var[I_p_min +i]=-BIG_P;

                  if(Flag_sys[i] == FS )
                   {
                      f_NO_FS(i);
                   }
                  Null_Period[i]=-1.;
                  Beg_angle[i]=-1.;

                  if( (State_s[i] != REP_FND) && (State_s[i] != 0))
                  {
                    if(Flag_sens > 0) Flag_sens--;
                  }
                  Statef_s[i]=0;
                  maxc_err[i]=max_err[i];

                  CntPerMKM[i]= -(float)CntDiskr[i] * ScaleCntMKM;
                  V_Diskr[i]=0;
                  ABS_LIN[i] = 0.;
                  s_dem2[i]=0.;
                  s_dem1[i]=0.;
                  *ss_dem[i] =0.;
                  *ss_demd[i] =0.;
                  f_clr_buf();
                  enable_int();
                  goto fin;
                }
//---------------------------------------
#else // (PLANAR != 0)
                  if(f_inw()) goto fin_e;
                  disable_int();

                  var[I_p_max +i]=BIG_P;
                  var[I_p_min +i]=-BIG_P;

                  if(Flag_sys[i] == FS )
                   {
                    if(i<=1) fPL_FS0_XY();
                    else fPL_FS0(i);
                   }
                  Null_Period[i+1]=-1.;
                  Beg_angle[i+1]=-1.;

                  if( (State_s[i] != REP_FND) && (State_s[i] != 0))
                  {
                    if(Flag_sens > 0) Flag_sens--;
                  }
                  Statef_s[i]=0;
                  maxc_err[i]=max_err[i];

                  if(i==0)
                  {
                   CntPerMKM[0]= -(float)CntDiskr[0] * ScaleCntMKM;
                   ABS_LINH[0] = 0.;
                   CntPerMKM[1]= -(float)CntDiskr[1] * ScaleCntMKM;
                   ABS_LINH[1] = 0.;
                   TT_PER_1[0] = 0;
                   TT_PER_1[1] = 0;
                  }
                  else  if(i==1)
                  {
                   CntPerMKM[2]= -(float)CntDiskr[2] * ScaleCntMKM;
                   ABS_LINH[2] = 0.;
                   TT_PER_1[2] = 0;
                  }
                  else
                  {
                   CntPerMKM[i+1]= -(float)CntDiskr[i+1] * ScaleCntMKM;
                   ABS_LIN[i+1] = 0.;
                   TT_PER_1[i+1] = 0;
                  }
                  s_dem2[i]=0.;
                  s_dem1[i]=0.;
                  *ss_dem[i] =0.;
                  *ss_demd[i] =0.;

                  f_clr_buf();
                  enable_int();
                  goto fin;
                }
#endif
            }
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L')&& (intrpr.wrd[1]=='T')&& (intrpr.wrd[2]=='V') )
         /* --------  command 'LTV'' -- */
         /* --  LTV - Input Limits for Vel. regulators-- */
            {
             if(f_dis_set(ds_ltv,3,6)>0)  f_count_cf();
             goto fin;
            }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L')&& (intrpr.wrd[1]=='T') )
         /* --------  command 'LT','LTX','LTY','LTZ','LTC' -- */
         /* --  LT - Limits for table DEG_V -- */
            {
           if( (i=f_xyzcbu(3)) >= 0 )  // 0 - X , 1 - Y ,2 - Z, 3 - C
           {
             if(f_dis_set(ds_lt[i],3,6)>0)
             {
            //  ftmp= (float)((int)var[i_k_ms_it]*(LIM_VM-1));
            //  if(V11_mm_sek[i] > ftmp) V11_mm_sek[i]=ftmp;
            //  if(V12_mm_sek[i] > ftmp) V12_mm_sek[i]=ftmp;
                f_tabl();
             }
           }
           else goto fin_e;

             goto fin;
            }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&&(intrpr.wrd[1]=='M')&&(intrpr.wrd[2]=='F')   )
           /*----------------------------------------------------*/
         /* ---- 'AMFX','AMFY','AMFZ','AMFC' searching start point parameters --- */
             if( (i=f_xyzcbu(4)) >= 0 )
             {

              if(f_dis_set( ds_amf[i],4,9) <0) goto fin_e;
              goto fin;
             }
    /*-----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G')&& (intrpr.wrd[1]=='S') )
           /* --------  command 'GS'  ------------ */
           /*----------------------------------------------------*/
         /* ---- 'GSX','GSY','GSZ','GSC' searching start point parameters --- */
             if( (i=f_xyzcbu(3)) >= 0 )
             {

              if(f_dis_set( ds_gs[i],3,6)>0)
                         f_cnt_dec();
              goto fin;
             }
    /*-----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&&(intrpr.wrd[1]=='M')&&(intrpr.wrd[2]=='M') )
         // --------  команда 'AMM'  ------------
         // максимальная амплитуда для асинхронного мотора и режимов с ослаблением поля
          {
           if(f_dis_set(ds_amm,3,6) <0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&&(intrpr.wrd[1]=='M')&&(intrpr.wrd[2]=='L') )
         // --------  команда 'AML'  ------------
          {
           if(f_dis_set(ds_aml,3,6) <0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&&(intrpr.wrd[1]=='M') )
         // --------  команда 'AM'  ------------
          {
           if(f_dis_set(ds_am,2,6) <0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='A')   )
         /* --------  команда 'TA'  ------------ */
          {
           if( f_dis_set(ds_ta,2,7)>0 )
           {
              f_cfg_count();
              f_cfg_ta();

#if(PROC_TYPE == C32)
               if(type_amp[5]==1)
                  IOSTRB=(AND_EXT_INT | WS_7 );
               else
                  IOSTRB=(INTERNAL_RDY | WS_7 );
#endif //(PROC_TYPE == C32)

           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='E')   )
         /* --------  команда 'TE'  ------------ */
          {
           if( f_dis_set(ds_te,2,6)>0 )
           {
              f_cfg_count();
              disable_int();
              f_arctg();
              enable_int();
           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='O')&&(intrpr.wrd[1]=='X')   )
         /* --------  команда 'OX' DACs offset ----------- */
          {
           if(f_dis_set(ds_ox,2,6) <0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A'))
         /* --------  команда 'A','AX','AY','AZ','AC'  ------------ */
          {
           if(intrpr.wdl==1)
             if(f_inw()) goto fin_e;
             else
              {
#if(PLANAR == 0)
               printf(" %6f %6f %6f %6f %6f %6f",AMPL[0],AMPL[1],AMPL[2],AMPL[3],AMPL[4],AMPL[5]);
#else
               printf(" %6f %6f %6f %6f %6f %6f %6f",AMPL[0],AMPL[1],AMPL[2],AMPL[3],AMPL[4],AMPL[5],AMPL[6]);
#endif
               goto fin;
              }

#if(PLANAR == 0)
           if( (i=f_xyzcbu(2)) >= 0 )
            {
#else
           if( (i=f_xyzcbu(2)) >= 0 )
            {
              if(i==0) ;
              else if (i==1) i=2;
              else i+=2;
#endif

              if(intrpr.wdl==2)
                 if(f_inw()) strg=intrpr.wrd;
                 else { printf(" %f",AMPL[i]);goto fin;}
              else
                  strg=&intrpr.wrd[2];
              if(f_innum(strg) )
               {
                Ynt=intrpr.num_var;
                if( fabs(Ynt) >  LIM_PVX[i])
                 if(Ynt >= 0.)   Ynt = LIM_PVX[i];
                 else            Ynt = -LIM_PVX[i];
                AMPL[i]= Ynt;
               }
              else  goto fin_e;
              goto fin;
            }
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='M')&&(intrpr.wrd[2]=='T')   )
         /* --------  'PMT'set         ------ */
          {
            if( (itmp1=f_dis_set(ds_pmt,3,6)) > 0  ) f_cfg_count();
            else if(itmp1<0) goto fin_e;
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='N') )
         /* --------  'PSN'set         ------ */
          {
            if(f_dis_set(ds_psn,3,6)>0)
             {
               f_cfg_count();
               f_count_lp();
             }
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='S')  )
         /* --------  command 'PSS' ---- */
           {

#if(PLANAR != 0)
             f_dis_set(ds_ps,3,4) ;
#else
             f_dis_set(ds_ps,3,3) ;
#endif
             goto fin;
           }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='S')  )
         /* --------  command 'PS' ---- */
           {

#if(PLANAR != 0)
             f_dis_set(ds_ps,2,4) ;
#else
             f_dis_set(ds_ps,2,3) ;
#endif
             goto fin;
           }
    /*----------------------------------------------*/

    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='X')   )
         /* --------  команда 'PX'  ------------ */
          {
           if(f_dis_set(ds_px,2,8)>0)
           {
             f_cfg_count();
             f_count_lp();
           }
           else goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='M')  )
         /* --------  'TM' -  Type of Motors --- */
          {
#if(PLANAR != 0)
           if(f_dis_set(ds_tm,2,7)>0)
           {
/*
  Motor_type[0]=2;
  Motor_type[1]=2;
  Motor_type[2]=2;
  Motor_type[3]=2;
*/
#else
           if(f_dis_set(ds_tm,2,6) > 0)
           {
#endif
             f_cfg_count();
           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&&(intrpr.wrd[1]=='D')&&(intrpr.wrd[2]=='C')   )
         /* --------  'ADC'set         ------ */
          {
            if(f_dis_set(ds_adc,3,6)>0)
             {
               f_cfg_count();
             }
            goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='G')&&(intrpr.wrd[2]=='N')   )
         /* --------  'TGN'set         ------ */
          {

#if(TWIN_X == 0)

  #if ( SURF == 0)
            if(f_dis_set(ds_tgn,3,6)>0)
  #else
            if(f_dis_set(ds_tgn,3,7)>0)
  #endif

#else

  #if ( SURF == 0)
            if(f_dis_set(ds_tgn,3,7)>0)
  #else
            if(f_dis_set(ds_tgn,3,8)>0)
  #endif

#endif


                        {
              f_cfg_count();
            }
            goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F')&&(intrpr.wrd[1]=='R')&&(intrpr.wrd[2]=='V')   )
         /* --------  'FRV'set         ------ */
          {
#if(TWIN_X == 0)
           if( f_dis_set(ds_frv,3,KOL_REG1) >0)
#else
           if( f_dis_set(ds_frv,3,KOL_REG1+1) >0)
#endif
            {
              f_cfg_count();
            }
            goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='G')&&(intrpr.wrd[2]=='O') )
         /* --------  'TGO'set         ------ */
          {
#if ( SURF == 0)
            f_dis_set(ds_tgo,3,6);
#else
            f_dis_set(ds_tgo,3,7);
#endif
            goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='G')&&(intrpr.wrd[2]=='S') )
         /* --------  'TGS'set         ------ */
          {
#if ( SURF == 0)
            f_dis_set(ds_tgs,3,6);
#else
            f_dis_set(ds_tgs,3,7);
#endif
            goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I')&&(intrpr.wrd[1]=='N')&&(intrpr.wrd[2]=='T') )
         /* --------  'INT'set         ------ */
          {
            if(f_dis_set(ds_int,3,6)>0) f_cfg_count();
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='G'))
         /* --------  command 'TG'  ------------ */
         /* ----  TG  - tangent of angles */
          {
             if( f_dis_set(ds_tg,2,2) > 0 )
                         f_dtg_cnt();
            goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
#if(PLANAR == 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='A')&&(intrpr.wrd[2]=='C') )
         /* --------  'DACX''DACY''DACZ''DACC'   ------------ */
          {
            if( (i = f_xyzcbu(4)) >= 0 )
            {
             j=mot_ph[(int)Motor_type[i]];
             i1=i * 3;
             if(f_dis_set(&ds_dac[i1],4,j)>0) f_cfg_count();
            }
            else goto fin_e;

            goto fin;
          }
    /*----------------------------------------------*/
#else  // (PLANAR != 0)
        if ( (intrpr.wrd[0]=='D')&&(intrpr.wrd[1]=='A')&&(intrpr.wrd[2]=='C') )
         /* --------  'DACX''DACY''DACZ''DACC'   ------------ */
          {
            if( (i = f_xiyjzcb(4)) >= 0 )
            {
             j=mot_ph[(int)Motor_type[i]];
             i1=i * 3;
             if(f_dis_set(&ds_dac[i1],4,j)>0) f_cfg_count();
            }
            else goto fin_e;

            goto fin;
          }
#endif
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='G')&&(intrpr.wrd[1]=='M') )
         /* --------  'GMX''GMY''GMZ''GMC'   ------------ */
          {
            if( (i = f_xyzcbu(3)) >= 0 )
            {
              if (
                  ((i == 0) && (Loop_x != 0)) ||
                  ((i == 3) && (Loop_c != 0))
                 )
              {
                 kk_p= Num[i] ;
                 kk_i= Denum[i] ;
                 if(f_dis_set(&ds_kf,3,2)>0)
                 {
                   Num[i]   = (int)kk_p;
                   Denum[i] = (int)kk_i;
               //  f_cfg_gear();
                   f_cfg_count();
             //    printf(" Rest = %f",Rest[i]);
                 }
                goto fin;
              }
            }
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='R')&&(intrpr.wrd[2]=='P')  )
         /* --------  'TRP' - type of reper ---- */
          {
            if(f_dis_set(ds_tr,3,6)>0 )
            {
#if(PLANAR != 0)
      Reper_type[0]=0;
      Reper_type[1]=0;
#endif
               f_cfg_count();
            }
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&&(intrpr.wrd[1]=='P'))
         /*  -----   command  'TP' ------------ */
          {
           if((itmp1=f_dis_set(&ds_tp,2,1)) >0 )
           {
            if(pult_type == PULT_24V)
            {
              scan_p=&scan_p_24v[0];
              msk_led=&msk_led_24v[0];
              MSK_KEY=MSK_KEY_24V;
              TL_KEY = TL_KEY_24V;
            }
            else
            {
              scan_p=&scan_p_5v[0];
              msk_led=&msk_led_5v[0];
              MSK_KEY=MSK_KEY_5V;
              TL_KEY = TL_KEY_5V;
            }
           }
           else if(itmp1<0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='P') &&(intrpr.wrd[2]=='S')   )
         /* --------  команда 'NPS'  ------------ */
          {
           if( f_dis_set(ds_np,3,6) > 0 )
           {
             for(i=0,i1=0;i<6;i++,i1++)
             {

#if(PLANAR != 0)
             if(i==1) i1=0;
#endif // (PLANAR != 0)

              if(Null_Period[i] < 0)  Beg_angle[i]=-1;
              else
              {
               Beg_angle[i]= (MotorPeriod[i1] * 0.25 ) - Null_Period[i];
               if(Beg_angle[i]< 0) Beg_angle[i]+= MotorPeriod[i1];
              }
             }
           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='P')&&(intrpr.wrd[2]=='R')   )
         /* --------  команда 'NPR'  ------------ */
          {
           for(i=0,i1=0;i<6;i++,i1++)
           {

#if(PLANAR != 0)
             if(i==1) i1=0;
#endif // (PLANAR != 0)
            if( Beg_angle_b[i] < 0) Beg_t[i]=-1;
            else
              {
               Beg_t[i]=  (MotorPeriod[i1] * 0.25 ) - Beg_angle_b[i];
               if(Beg_t[i]< 0) Beg_t[i]+= MotorPeriod[i1];
              }
           }

           if(f_dis_set(ds_nptt,3,12)>0)
           {
             if(Beg_t[0] == -7.)
              {

               for(i=0,i1=0;i<6;i++,i1++)
                {
#if(PLANAR != 0)
                   if(i==1) i1=0;
#endif // (PLANAR != 0)
                   if(Null_Period[i] < 0) Beg_angle_b[i]=-1;
                   else
                   {
                    Beg_angle_b[i] = (MotorPeriod[i1] * 0.25 ) - Null_Period[i];
                    if(Beg_angle_b[i] < 0.) Beg_angle_b[i]+=MotorPeriod[i1];
                   }
                }
                goto fin;
              }

             for(i=0,i1=0;i<6;i++,i1++)
              {
#if(PLANAR != 0)
                 if(i==1) i1=0;
#endif // (PLANAR != 0)
            if( Beg_t[i] < 0) Beg_angle_b[i]=-1;
            else
               {
                 Beg_angle_b[i]=  (MotorPeriod[i1] * 0.25 ) - Beg_t[i];
                 if(Beg_angle_b[i] < 0.) Beg_angle_b[i]+=MotorPeriod[i1];
               }
              }
           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='P')&&(intrpr.wrd[2]=='T') )
         /* --------  команда 'NPT'  ------------ */
          {
           for(i=0,i1=0;i<6;i++,i1++)
           {

#if(PLANAR != 0)
             if(i==1) i1=0;
#endif // (PLANAR != 0)
            if( Beg_angle_t[i] < 0) Beg_t[i]=-1;
            else
            {
             Beg_t[i]=   (MotorPeriod[i1] * 0.25 ) - Beg_angle_t[i];
             if(Beg_t[i]< 0) Beg_t[i]+=MotorPeriod[i1];
            }
           }

           if(f_dis_set(ds_nptt,3,12)>0)
           {
             if(Beg_t[0] == -7.)
              {
               for(i=0,i1=0;i<6;i++,i1++)
                {
#if(PLANAR != 0)
                  if(i==1) i1=0;
#endif // (PLANAR != 0)
                   if(Null_Period[i] < 0) Beg_angle_t[i]=-1;
                   else
                    {
                     Beg_angle_t[i] = (MotorPeriod[i1] * 0.25 ) - Null_Period[i];
                     if(Beg_angle_t[i] < 0.) Beg_angle_t[i]+=MotorPeriod[i1];
                    }
                }
                goto fin;
              }

             for(i=0,i1=0;i<6;i++,i1++)
              {
#if(PLANAR != 0)
                if(i==1) i1=0;
#endif // (PLANAR != 0)
                if( Beg_t[i] < 0) Beg_angle_t[i]=-1;
                else
                 {
                    Beg_angle_t[i] = (MotorPeriod[i1] * 0.25 ) - Beg_t[i];
                    if(Beg_angle_t[i] < 0.) Beg_angle_t[i]+=MotorPeriod[i1];
                 }
              }
           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F')&&(intrpr.wrd[1]=='N')&&(intrpr.wrd[2]=='P')   )
         /* --------  команда 'FNPX','FNPY','FNPZ','FNPC' -- */
          {
           if( (i=f_xyzcbu(4)) >= 0 )
           {
             i1= f_fnp1(i);
             if(i1 == 0) goto fin;
             if(flag_disp != 0) goto mnp0;
             goto fin;
           }
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='H')   )
         /* --------  команда 'CHX' 'CHY' 'CHZ' - */
          {
            if( (i=f_xyzcbu(3)) >= 0 )
            {
             f_NO_FS(i);
#if(PLANAR != 0)

  switch(i)
   {
    case 0:  // axis X

             k_dir[0]= -k_dir[0];
             k_dir[1]= -k_dir[1];
             pf[0] = -pf[0];
             pf[1] = -pf[1];
             RFY = -RFY;
             if(RFY <0) Ly=-fabs(Ly);
             else Ly  = fabs(Ly);

             if(Null_Period[0]>=0)
             {
               Null_Period[0] =  MotorPeriod[0] * 0.75  - Null_Period[0];
               if( Null_Period[0] < 0.) Null_Period[0] += MotorPeriod[0];
             }

             if(Null_P_L[0]>=0)
             {
               Null_P_L[0] =  MotorPeriod[0] * 0.75  - Null_P_L[0];
               if( Null_P_L[0] < 0.) Null_P_L[0] += MotorPeriod[0];
             }

             if(Null_Period[1]>=0)
             {
               Null_Period[1] =  MotorPeriod[0] * 0.75  - Null_Period[1];
               if( Null_Period[1] < 0.) Null_Period[1] += MotorPeriod[0];
             }

             if(Null_P_L[1] >= 0)
             {
               Null_P_L[1] =  MotorPeriod[0] * 0.75  - Null_P_L[1];
               if( Null_P_L[1] < 0.) Null_P_L[1] += MotorPeriod[0];
             }

             break;
 //---------------------------
    case 1:  // axis Y

             k_dir[2]= -k_dir[2];
             k_dir[3]= -k_dir[3];
             pf[2] = -pf[2];
             RFY = -RFY;
             if(RFY <0) Ly=-fabs(Ly);
             else Ly  = fabs(Ly);

             if(Null_Period[2]>=0)
             {
               Null_Period[2] =  MotorPeriod[1] * 0.75  - Null_Period[2];
               if( Null_Period[2] < 0.) Null_Period[2] += MotorPeriod[1];
             }

             if(Null_P_L[2]>=0)
             {
               Null_P_L[2] =  MotorPeriod[1] * 0.75  - Null_P_L[2];
               if( Null_P_L[2] < 0.) Null_P_L[2] += MotorPeriod[1];
             }

             break;
 //---------------------------
    default:
             i1=i+1;
             i2=i+2;

             pf[i1] = -pf[i1];
             k_dir[i2]= -k_dir[i2];

             if(Null_Period[i1]>=0)
             {
               if(Motor_type[i2]==2)  Null_Period[i1] =  MotorPeriod[i] * 0.75  - Null_Period[i1];
               else if(Motor_type[i2] == 3) Null_Period[i1] =  MotorPeriod[i] * 0.83333 - Null_Period[i1];
               if( Null_Period[i1] < 0.) Null_Period[i1] += MotorPeriod[i];
               Beg_angle[i1] = MotorPeriod[i] * 0.25  - Null_Period[i1];
               if(Beg_angle[i1] < 0.) Beg_angle[i1] += MotorPeriod[i];
             }
 //---------------------------
   }

#else  // (PLANAR == 0)

             pf[i] = -pf[i];
             k_dir[i]= -k_dir[i];

             if(Null_Period[i]>=0)
             {
               if(Motor_type[i]==2)  Null_Period[i] =  MotorPeriod[i] * 0.75  - Null_Period[i];
               else if(Motor_type[i] == 3) Null_Period[i] =  MotorPeriod[i] * 0.83333  - Null_Period[i];
               if( Null_Period[i] < 0.) Null_Period[i] += MotorPeriod[i];
               Beg_angle[i] = MotorPeriod[i] * 0.25  - Null_Period[i];
               if(Beg_angle[i] < 0.) Beg_angle[i] += MotorPeriod[i];
             }

#endif // (PLANAR != 0)

              disable_int();
              f_cfg_count();
              f_count_cf();
              enable_int();
              goto fin;
            }
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='P')   )
         /* --------  команда 'NPX' 'NPY' 'NPZ' - */
          {
            if( (i=f_xyzcbu(3)) >= 0 )
            {

              i1= f_np(i);
              if(i1 == 0) goto fin;
mnp0:
              np_err_print(i,i1);
              f_NO_FS(i);
              goto fin_e;
            }
          }
    /*----------------------------------------------*/
     if ((intrpr.wrd[0]=='I') && (intrpr.wrd[1]=='S')&& (intrpr.wrd[2]=='W'))
         {                  // 'ISW'

            if(f_dis_set(&ds_isw,3,1) > 0)
               f_init_sw(mskf_sw);
            goto fin;
         }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='K')&&(intrpr.wrd[2]=='R')   )
         /* --------  команда 'MKR'eady ------- */
          {
           if(f_dis_set(ds_mkr,3,6)>0 ) f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='K')&&(intrpr.wrd[2]=='S')   )
         /* --------  'MKS'sensor mask ------ */
          {
           if(f_dis_set(ds_mks,3,6)>0 ) f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='K')&&(intrpr.wrd[2]=='P')   )
         /* --------  команда 'MKP'lus -------- */
          {
           if(f_dis_set(ds_mkp,3,6)>0 ) f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='K')&&(intrpr.wrd[2]=='M')   )
         /* --------  команда 'MKM'inus -------- */
          {
           if(f_dis_set(ds_mkm,3,6)>0 ) f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='K')&&(intrpr.wrd[2]=='I')   )
         /* --------  команда 'MKI'ndex mark------- */
          {
           if(f_dis_set(ds_mki,3,6)>0 ) f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I')&&(intrpr.wrd[1]=='S')   )
         /* --------  команда 'IS' ------------ */
          {
           if(f_dis_set(ds_is,2,6) > 0)
                f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I')&&(intrpr.wrd[1]=='R')   )
         /* --------  команда 'IR' ------------ */
          {
           if(f_dis_set(ds_ir,2,6) > 0)
                f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I')&&(intrpr.wrd[1]=='M')   )
         /* --------  команда 'IM' ------------ */
          {
           if(f_dis_set(ds_im,2,6)>0 ) f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I')&&(intrpr.wrd[1]=='P')   )
         /* --------  команда 'IP' ------------ */
          {
           if(f_dis_set(ds_ip,2,6)>0 ) f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I')&&(intrpr.wrd[1]=='I')   )
         /* --------  команда 'II' ------------ */
          {
           if(f_dis_set(ds_ii,2,6) > 0)
                f_inv_msk();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='X')&&(intrpr.wrd[2]=='I')   )
    /* --------  команда 'MXI'   ----------------------- */
          {  // направление поиска индексных меток

           if(f_dis_set(&ds_m[6],3,6) > 0)
            {
            /*
                disable_int();
                f_cfg_count();
                f_count_cf();
                enable_int();
             */
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='X')   )
    /* --------  команда 'MX'   ----------------------- */
          {

#if(PLANAR != 0)
           if(f_dis_set(ds_m,2,13) > 0)
#else
           if(f_dis_set(ds_m,2,12) > 0)
#endif
            {
                disable_int();
                f_cfg_count();
                f_count_cf();
                enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
#if(BTW != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&& (intrpr.wrd[1]=='B')&&(intrpr.wrd[2]=='T')   )
    /* --------  команда 'MBT'   ----------------------- */
          {

           if(f_dis_set(ds_mbt,3,KOL_REG1+KOL_REG1) > 0)
            {
                disable_int();
                f_count_cf();
                enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='B')&&(intrpr.wrd[2]=='V') )
    /* --------  команда 'MBV'   ----------------------- */
          {

           if(f_dis_set(ds_mbv,3,KOL_REG1+KOL_REG1) > 0)
            {
                disable_int();
                f_count_cf();
                enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='B')&&(intrpr.wrd[2]=='A') )
    /* --------  команда 'MBA'   ----------------------- */
          {

           if(f_dis_set(ds_mba,3,KOL_REG1+KOL_REG1) > 0)
            {
                disable_int();
                f_count_cf();
                enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&&(intrpr.wrd[1]=='B')&&(intrpr.wrd[2]=='M') )
    /* --------  команда 'MBM'   ----------------------- */
          {

           if(f_dis_set(ds_mbm,3,KOL_REG1) > 0)
            {
                disable_int();
                f_count_cf();
                enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
#endif // (BTW != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E')&&(intrpr.wrd[1]=='X') && (intrpr.wdl== 2) )
         /* --------  команда 'EX'  ------------ */
          {
           if( (i1=f_dis_set(ds_e,2,7)) >0 )
           {
             if(i>6) i=6;
             for(i=0;i<i1;i++)
              if((State_s[i] == REP_FND) || (State_s[i] == 0) )
               maxc_err[i] = max_err[i];
           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L')&& (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='M') )
         /* --------  команда 'LXM' ------------ */
          {
           f_dis_set(ds_lxm,3,6);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L')&& (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='B') )
         /* --------  команда 'LXB' ------------ */
          {
           f_dis_set(ds_lxb,3,6);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&&(intrpr.wrd[1]=='G')   )
         /* --------  команда 'SGX''SGY''SGZ''SGC''SGB''SGU' ----- */
          {
           if( (i=f_xyzcbu(3)) >= 0 )
            {

             if(var[I_NP_c +i] < 0)
             {
              printf (" - successful searching 'G%c;' must be performed!",letter[i]);
              goto fin;
             }

#if(PLANAR == 0)
             if(Reper_type[i] == 2 )  // steper motor
              {
               if( (var[I_NP_c +i] >  ( MotorPeriod[i] * 0.75 )) ||    (var[I_NP_c +i] <  ( MotorPeriod[i] * 0.25 )) )
                   var[I_mode_gx +i]=1;
               else
                    var[I_mode_gx +i]=0;
              }
             else
              {
               if( (var[I_NP_c +i] >  ( fabs(pf[i]) * 0.75 )) ||    (var[I_NP_c +i] <  ( fabs(pf[i]) * 0.25 )) )
                   var[I_mode_gx +i]=1;
               else
                    var[I_mode_gx +i]=0;
              }
#else  // (PLANAR != 0)

             if(Reper_type[i] == 2 )  // steper motor
              {
               if( (var[I_NP_c +i] > ( MotorPeriod[i] * 0.75 )) || (var[I_NP_c +i] <  ( MotorPeriod[i] * 0.25 )) )
                   var[I_mode_gx +i]=1;
               else
                    var[I_mode_gx +i]=0;
              }
             else
              {
               if( (var[I_NP_c +i] >  (  fabs(pf[i+1]) * 0.75 )) ||  (var[I_NP_c +i] <  ( fabs(pf[i+1]) * 0.25 )) )
                   var[I_mode_gx +i]=1;
               else
                   var[I_mode_gx +i]=0;
              }
#endif // (PLANAR == 0)


            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L')&&(intrpr.wrd[1]=='X')   )
         /* --------  команда 'LX'  ------------ */
          {
           f_dis_set(ds_l,2,6);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='S' )
         /* --------  команда 'SX','SY','SZ','SC' ------------ */
  #if(TWO_MS == 0)
          if( (i=f_xyzcbu(2)) >= 0 )
  #else   //(TWO_MS != 0)
          if( (i=f_xyiju(2)) >= 0 )
  #endif //(TWO_MS == 0)
          {
           kk_p= var[I_max_s +i];       /* максимальный путь выхода на концевой датчик */
           kk_i= var[I_sr_s +i] ;       /* максимальный путь сползания с датчика */
           kk_d= var[I_sens_rep +i];    /* путь от датчика до репера */
           kkat= var[I_rep_s +i];       /* путь зоны поиска  репера */
           kk_f= var[I_abs_rep +i];     /* абсолютная координата репера */
           tmpf1= var[I_g_dem +i];      // home position absolute value
           tmpf2=var[I_mode_gx +i];  // mode of round
           tmpf3=var[I_NP_c +i];

           if(f_dis_set( ds_kf,2,8) <0) goto fin_e;

           var[I_max_s +i]    = kk_p;
           var[I_sr_s +i]     = kk_i;
           var[I_sens_rep +i] = kk_d;
           var[I_rep_s +i]    = kkat;
           var[I_abs_rep +i]  = kk_f;
           var[I_g_dem +i]=tmpf1;
           var[I_mode_gx +i]=tmpf2;

           goto fin;
          }
    /*-----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='R')  )
         /* --------  команда 'CR'- C Radius    ----------- */
          {
           if(f_dis_set(&ds_cr,2,1) <0) goto fin_e;
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E')&&(intrpr.wrd[1]=='W')  )
         /* -------- команда 'EW'- Enable Window control --- */
          {
           f_dis_set(&ds_ew,2,1);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='W')&&(intrpr.wrd[1]=='P')  )
         /* -------- команда 'WP'- Window plus     --- */
          {
           f_dis_set(ds_wp,2,5);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='W')&&(intrpr.wrd[1]=='M')  )
         /* -------- команда 'WM'- Window minus    --- */
          {
           f_dis_set(ds_wm,2,5);
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S') && (intrpr.wrd[1]=='P') )
         /* --------  команда 'SP'  ------------ */
         /* --SP - Serial port speed---------- */
          {
           Baud_check();
           if(intrpr.wdl==2)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
                 if(flag_DT_BR == 0)
                  {
                          if(baud_cod == BAUD_1200 )  itmp=1200;
                     else if(baud_cod == BAUD_2400 )  itmp=2400;
                     else if(baud_cod == BAUD_4800 )  itmp=4800;
                     else if(baud_cod == BAUD_9600 )  itmp=9600;
                     else if(baud_cod == BAUD_19200)  itmp=19200;
                     else if(baud_cod == BAUD_38400)  itmp=38400;
#if(PROC_TYPE == C33)
                     else if(baud_cod == BAUD_57600  )  itmp=57600   ;
                     else if(baud_cod == BAUD_115200 )  itmp=115200  ;
                     else if(baud_cod == BAUD_128000 )  itmp=128000  ;
#endif //(PROC_TYPE == C33)

                     else goto fin;

                   printf(" %d",itmp);
                  }
                 else printf(" - now in ACS mode , apply 'SP x' command first.");

                 goto fin;
               }
           else
               strg=&intrpr.wrd[2];
           if(f_innum(strg) )
             switch( ((int)intrpr.num_var) )
             {
               case  1200:   baud_cod=BAUD_1200;   break;
               case  2400:   baud_cod=BAUD_2400;   break;
               case  4800:   baud_cod=BAUD_4800;   break;
               case  9600:   baud_cod=BAUD_9600;   break;
               case 19200:   baud_cod=BAUD_19200;  break;
               case 38400:   baud_cod=BAUD_38400;  break;
#if(PROC_TYPE == C33)
               case 57600  :   baud_cod=BAUD_57600   ;  break;
               case 115200 :   baud_cod=BAUD_115200  ;  break;
               case 128000 :   baud_cod=BAUD_128000  ;  break;
#endif //(PROC_TYPE == C33)
               default:  goto fin_e;
             }
           else goto pr_bad;


           flag_DT_BR=0;
           disable_int();
           Ini_RS232();
           enable_int();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C')&&(intrpr.wrd[1]=='T'))
         /* --------  команда 'CT'  ------------ */
         /* ----  'CT'  - decrease/increase  Current Time  -- */
          {
           if(f_dis_set(ds_ct,2,3)>0) f_cnt_dec();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='S')&& (intrpr.wrd[2]=='F') )
        {         //     'DSF'
           if(f_dis_set(ds_ds,3,2)>0)
           {
/*
#if(PROC_TYPE == C32)
            *ds_ti.op *= (k_t_mks/ds_ti.div);
            ds_ti.div  = k_t_mks;
#endif //(PROC_TYPE == C32)
*/
//#if(PROC_TYPE == C33)
            k_t_mks = k_t_mks_b;
            ds_ti.div  = k_t_mks_b;
//#endif //(PROC_TYPE == C33)

            goto mm_ti;
           }
            k_t_mks = k_t_mks_b;
            ds_ti.div  = k_t_mks_b;
           goto fin;
        }
    /*----------------------------------------------*/

  #if(TR_JERK == 0)

        if ( (intrpr.wrd[0]=='T')&& (intrpr.wrd[1]=='D')&& (intrpr.wrd[2]=='F') )
        {
         /*  -----   command  'TDF'  - type move structure */
           if(f_dis_set(&ds_tdf,3,1));
           else  tdf_tmp=ssu_beg;
           disable_int();
           itmp=tdf_tmp;
           f_print_mvvs(itmp);
           enable_int();
           goto fin;
        }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&& (intrpr.wrd[1]=='D') )
        {
         /*  -----   command  'TD'  - type move structure */
           if(f_dis_set(&ds_td,2,1));
           else  td_tmp=m_beg;
           disable_int();
           itmp=td_tmp;
           f_print_mvs(itmp);
           enable_int();
           goto fin;
        }
  #endif //(TR_JERK  == 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='T')&& (intrpr.wrd[1]=='I') )
         /* --------  команда 'TI'  ------------ */
         /* ---TI - Timer Interrupt period---- */
          {
           if(f_dis_set(&ds_ti,2,1)>0)
            {
mm_ti:       disable_int();

             f_v_a();
             f_v_az();
             f_cnt_dec();
             f_tim0_set();
             f_count_cf();

             f_tabl();

             enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
       if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='R')&& (intrpr.wrd[2]=='P') )
           /* --------  command 'DRP'  ------------ */
           /*  DRP - direct rotation Parameters     */
          {
             n_dr_axt=n_dr_ax;

             if( (i=f_dis_set(ds_drp,3,3)) >0)
              {
               a_dem_dr = f_ms2_it(a_dr_mm);

               if(i >= 2)
                {
                   if( (n_dr_ax != 0.) && (n_dr_axt == 0.) )
                    {
                      // Direct Rotation Axis OFF

#if (TR_INDEP== 0)
                        if(m_fin != m_beg) goto fin_e;
                        disable_int();

                        n_dr_ax=n_dr_axt;
                        f_clr_buf();
#else   // (TR_INDEP == 1)
                         i1=n_dr_ax-1;
                         if(fl_ind_ax[i1] == 0)
                          { // буферированные перемещения
                           if(m_fin != m_beg) goto fin_e;
                           disable_int();
                           n_dr_ax=n_dr_axt;
                           f_clr_buf();
                          }
                         else
                          { //инициализация независимого генератора профиля
                            n_dr_ax=n_dr_axt;
                            f_clr_ind(i1);
                          }
#endif // (TR_INDEP== 0)

                    }
                   if( (n_dr_ax == 0.) && (n_dr_axt != 0.))
                    {
                      // Direct Rotation Axis ON

                        disable_int();
                        n_dr_ax  = n_dr_axt;
                        s_uk_dr  = s_uk_dem[(int)n_dr_ax];
                        s_dem_dr = *s_uk_dr;
                        enable_int();
                    }
                 s_uk_dr  = s_uk_dem[(int)n_dr_ax];
                }
               goto m_drt;

              }
             goto fin;
          }
    /*---------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='R')&& (intrpr.wrd[2]=='S') )
           /* --------  command 'DRS'  ------------ */
           /*  DRS - direct rotation State  */
          {
             f_dis_set(&ds_drs,3,1);
             goto fin;
          }
    /*---------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='R')&& (intrpr.wrd[2]=='T') )
          /* --------  command 'DRT'  ------------ */
          /*  DRT - direct rotation Turn ON/OFF -- */
          {
            if( f_dis_set(&ds_drt,3,1)>0)
            {
m_drt:
              if(drt_tmp == 0.)  v_dem_dr=0.;
              else
              {
               if( n_dr_ax >0)
                 if( (Flag_pult != 0)  && ( vp_tek[(int)(n_dr_ax-1)] != 0)) goto fin_e;

                disable_int();
                s_uk_dr  = s_uk_dem[(int)n_dr_ax];
                s_dem_dr = *s_uk_dr;
                enable_int();
                n_dr_ax_t2 = n_dr_ax ;
                if(drt_tmp > 0.) v_dem_dr = f_mm_it(v_dr_mm);
                else v_dem_dr = -f_mm_it(v_dr_mm);
              }
            }

            goto fin;
          }
    /*---------------------------------------------*/
    /*---------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&& (intrpr.wrd[1]=='D')  && (intrpr.wrd[3]=='I') && (intrpr.wdl>=4)  )
         /*  -----   command  'SDFI'  ------------ */
         /*  -----   command  'SDVI'  ------------ */
         /*  -----   command  'SDPI'  ------------ */
         /*  -----   command  'SDSI'  ------------ */
         /* ---SDFI - inversion               ---- */
          {
           switch(intrpr.wrd[2])
           {
            case 'F': sd_inv_op=&Frw_inv[0] ;  break;
            case 'V': sd_inv_op=&Rev_inv[0] ;  break;
            case 'P': sd_inv_op=&Pole_inv[0] ; break;
            case 'S': sd_inv_op=&Srv_On_inv[0];break;
            default: goto fin_e;
           }

           for(i=0;i<4;i++)
             if( sd_inv_op[i] == 0 ) sd_inv[i]=0;
             else sd_inv[i]=1.;

           if(f_dis_set(ds_sdi,4,4)>0)
           {
             for(i=0;i<4;i++)
               if( sd_inv[i] == 0 ) sd_inv_op[i]=0;
               else sd_inv_op[i]=0xffffffff;
           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&& (intrpr.wrd[1]=='D') )
         /*  -----   command  'SDF'  ------------ */
         /*  -----   command  'SDV'  ------------ */
         /*  -----   command  'SDP'  ------------ */
         /*  -----   command  'SDS'  ------------ */
         /*  -----   command  'SDE'  ------------ */
         /*  -----   command  'SDI'  ------------ */
         /* --- SDF - set signal numbers for step dir signals   ---- */
          {
             switch(intrpr.wrd[2])
             {
              case 'F': sd_par=&Frw_msk[0] ; break;
              case 'V': sd_par=&Rev_msk[0] ; break;
              case 'P': sd_par=&Pole_msk[0] ; break;
              case 'S': sd_par=&Srv_On_msk[0] ; break;
              case 'R': sd_par=&Ready_Sd_msk[0];break;
              case 'I': sd_par=&In_Pos_msk[0];break;
              case 'E': f_dis_set(ds_sde,3,4); goto fin;
              default: goto m_sd;
             }
             for(i=0;i<4;i++)
                 sd_inv[i]= sd_par[i];

             if(f_dis_set(ds_sdi,3,4))
               for(i=0;i<4;i++)
                 sd_par[i]=sd_inv[i];

             goto fin;
          }
    /*----------------------------------------------*/
#if(TWO_MS != 0)

        if ( (intrpr.wrd[0]=='M')&& (intrpr.wrd[1]=='D') )
         /*  -----   command  'MDI'  ------------ */
         /*  -----   command  'MDR'  ------------ */
         /*  -----   command  'MDS'  ------------ */
         /*  -----   command  'MDE'  ------------ */
         /*  -----   command  'MDP'  ------------ */
          {
             switch(intrpr.wrd[2])
             {
              case 'I': sd_par=&Md_In_Pos[0]; tmp=3; break;
              case 'R': sd_par=&Md_Rdy[0]; tmp=4;break;
              case 'S': sd_par=&Md_Sig[0]; tmp=3;break;
              case 'E': f_dis_set(ds_mde,3,6); goto fin;
              case 'P': if(f_dis_set(ds_mdp,3,2)>0) f_cfg_two();
                        goto fin;
              default: goto m_md;
             }
             for(i=0;i<tmp;i++)
                 sd_inv[i]= sd_par[i];

             if(f_dis_set(ds_sdi,3,tmp)>0)
               for(i=0;i<tmp;i++)
                 sd_par[i]=sd_inv[i];

             goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='M')&& (intrpr.wrd[1]=='D') )
         /*  -----   command  'MD' 'MD '-------- */
         /* --- MD - MoDe of meassurement system ---- */
          {
m_md:
             flag_md1 = flag_md;
             if(f_dis_set(&ds_md,2,1) >0)
             {
              if(flag_md1 == flag_md) goto fin;
              else if(flag_md1 == 1)
              {
            //   turn on encoder meassurement system
                if(Flag_sys[0] != FS) goto fin_e;
                if( (A_Sin[3] < MIN_AB[3]) || (A_Sin[4] < MIN_AB[4]) || (A_Sin[5] < MIN_AB[5]) ) goto fin_e;

                disable_int();

                XN_1[0] = 0;
                XN_2[0] = 0;

      uk_bt=&bt_xn[0];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;

                if(flag_rv[4] != 0)
                {
                   YNV[1] = YN[1];
                   YN[1] = 0;
                   XN_1[1] = 0.;
                   XN_2[1] = 0.;
                   XN_1V[1] = 0.;
                   XN_2V[1] = 0.;

                  uk_bt=&bt_xn[1];
                  uk_bt->TTn[0]=0;
                  uk_bt->TTn[1]=0;
                  uk_bt->TTn[2]=0;
                  uk_bt->TTn[3]=0;
                  uk_bt->TTn[4]=0;
                  uk_bt->TTn[5]=0;


                  uk_bt=&bt_v[1];
                  uk_bt->TTn[0]=0;
                  uk_bt->TTn[1]=0;
                  uk_bt->TTn[2]=0;
                  uk_bt->TTn[3]=0;
                  uk_bt->TTn[4]=0;
                  uk_bt->TTn[5]=0;
                }


                if(flag_rv[5] != 0)
                {
                   YNV[2] = YN[5];
                   YN[2] = 0;
                   XN_1[2] = 0.;
                   XN_2[2] = 0.;
                   XN_1V[2] = 0.;
                   XN_2V[2] = 0.;

                  uk_bt=&bt_xn[2];
                  uk_bt->TTn[0]=0;
                  uk_bt->TTn[1]=0;
                  uk_bt->TTn[2]=0;
                  uk_bt->TTn[3]=0;
                  uk_bt->TTn[4]=0;
                  uk_bt->TTn[5]=0;

                  uk_bt=&bt_v[2];
                  uk_bt->TTn[0]=0;
                  uk_bt->TTn[1]=0;
                  uk_bt->TTn[2]=0;
                  uk_bt->TTn[3]=0;
                  uk_bt->TTn[4]=0;
                  uk_bt->TTn[5]=0;
                }


                 MIN_ABR[3]=MIN_AB[3];
                 MIN_ABR[4]=MIN_AB[4];
                 MIN_ABR[5]=MIN_AB[5];
    //---------- -------------------------------
    //  init mea ssurement system N3

                 AA = (*P_Analog[6] - Null_ADC[6] ) * Norm_ampl[6];
                 BB = (*P_Analog[7] - Null_ADC[7] ) * Norm_ampl[7];

                 BR=fabs(BB);
                 AR=fabs(AA);

                 if( (AR*AR+BR*BR) < MIN_ABR[3] )  goto fin_emd;

                  if(AR > BR )
                     if(AA>0)  I0=TT1  + (BB*AMPS) /AR;
                     else      I0=TT2  + (BB*AMPS) /AR;
                  else
                     if(BB>0)  I0=TT3  + (AA*AMPS) /BR;
                     else      I0=TT4  + (AA*AMPS) /BR;

                  FFFF[3] = UK_ATAN[3][I0];
            /*-- -----*/
                 qc= *qcc_c[3];

                QuadNum = FFFF[3] >>14;

                NewCor=QuadNum-qc&3;
                OldCor[3]=NewCor;
                NewCnt= ((qc+NewCor)&0xfffc);
                OldCnt[3]=NewCnt;
                CntDiskr[3] = FFFF[3] | (NewCnt << 14);

                V_Diskr[3]=0;

             if(change_xy == 0)
              {
               ftmp= (ABS_LIN[0]+ABS_LIN[1]) * 0.5  ;
              }
             else ftmp = ABS_LIN[2];

                ftmp1= ftmp + (ABS_LIN[0] - ABS_LIN[1]) * enc_hall * 0.5;

                ABS_LIN[3] = ftmp1;
                CntPerMKM[3] =  ftmp1/pf[3] - (float) CntDiskr[3] * ScaleCntMKM ;
    //-----------------------------------------
    //  init meassurement system N4

                AA = (*P_Analog[8] - Null_ADC[8] ) * Norm_ampl[8];
                BB = (*P_Analog[9] - Null_ADC[9] ) * Norm_ampl[9];

                BR=fabs(BB);
                AR=fabs(AA);
                if( (AR*AR+BR*BR) < MIN_ABR[4] )  goto fin_emd;

                 if(AR > BR )
                    if(AA>0)  I0=TT1  + (BB*AMPS) /AR;
                    else      I0=TT2  + (BB*AMPS) /AR;
                 else
                    if(BB>0)  I0=TT3  + (AA*AMPS) /BR;
                    else      I0=TT4  + (AA*AMPS) /BR;

                 FFFF[4] = UK_ATAN[4][I0];
            /*--------*/
              qc= *qcc_c[4];

                QuadNum = FFFF[4] >>14;

                NewCor=QuadNum-qc&3;
                OldCor[4]=NewCor;
                NewCnt= ((qc+NewCor)&0xfffc);
                OldCnt[4]=NewCnt;
                CntDiskr[4] = FFFF[4] | (NewCnt << 14);
                V_Diskr[4]=0;

                ftmp1= ftmp + (ABS_LIN[1] - ABS_LIN[0]) * enc_hall * 0.5;

                ABS_LIN[4] = ftmp1;

                CntPerMKM[4] =  ftmp1/pf[4] - (float) CntDiskr[4] * ScaleCntMKM ;
    //-----------------------------------------
    //  init meassurement system N5

                AA = (*P_Analog[10] - Null_ADC[10] ) * Norm_ampl[10];
                BB = (*P_Analog[11] - Null_ADC[11] ) * Norm_ampl[11];

                BR=fabs(BB);
                AR=fabs(AA);
                if( (AR*AR+BR*BR) < MIN_ABR[5] )  goto fin_emd;

                 if(AR > BR )
                    if(AA>0)  I0=TT1  + (BB*AMPS) /AR;
                    else      I0=TT2  + (BB*AMPS) /AR;
                 else
                    if(BB>0)  I0=TT3  + (AA*AMPS) /BR;
                    else      I0=TT4  + (AA*AMPS) /BR;

                 FFFF[5] = UK_ATAN[5][I0];
            /*---------*/
              qc= *qcc_c[5];

                QuadNum = FFFF[5] >>14;

                NewCor=QuadNum-qc&3;
                OldCor[5]=NewCor;
                NewCnt= ((qc+NewCor)&0xfffc);
                OldCnt[5]=NewCnt;
                CntDiskr[5] = FFFF[5] | (NewCnt << 14);
                V_Diskr[5]=0;

             if(change_xy != 0)
              {
               ftmp= (ABS_LIN[0]+ABS_LIN[1]) * 0.5  ;
              }
             else ftmp = ABS_LIN[2];


                ABS_LIN[5] = ftmp;
                CntPerMKM[5] =  ftmp/pf[5] - (float) CntDiskr[5] * ScaleCntMKM ;

    //-----------------------------------------

                flag_md= 1;
                enable_int();

                goto fin;

    fin_emd:
                flag_md= 0;
                MIN_ABR[3]=-1.;
                MIN_ABR[4]=-1.;
                MIN_ABR[5]=-1.;
                enable_int();
                goto fin_e;
              }
              else if(flag_md1 == 0)
              {

            //   turn off encoder meassurement system

                disable_int();

            //  YN[0] = YN[3];
            /*
                XN_1[0] = XN_1[0]*hall_enc;
                XN_2[0] = XN_2[0]*hall_enc;
            */
                XN_1[0] = 0;
                XN_2[0] = 0;

                uk_bt=&bt_xn[0];
                uk_bt->TTn[0]=0;
                uk_bt->TTn[1]=0;
                uk_bt->TTn[2]=0;
                uk_bt->TTn[3]=0;
                uk_bt->TTn[4]=0;
                uk_bt->TTn[5]=0;

                if(flag_rv[4] != 0)
                {
                   YN[1] = YNV[1];
                   XN_1[1] = 0.;
                   XN_2[1] = 0.;

                  uk_bt=&bt_xn[1];
                  uk_bt->TTn[0]=0;
                  uk_bt->TTn[1]=0;
                  uk_bt->TTn[2]=0;
                  uk_bt->TTn[3]=0;
                  uk_bt->TTn[4]=0;
                  uk_bt->TTn[5]=0;

                }

                if(flag_rv[5] != 0)
                {
                   YN[2] = YNV[2];
                   XN_1[2] = 0.;
                   XN_2[2] = 0.;
                  uk_bt=&bt_xn[2];
                  uk_bt->TTn[0]=0;
                  uk_bt->TTn[1]=0;
                  uk_bt->TTn[2]=0;
                  uk_bt->TTn[3]=0;
                  uk_bt->TTn[4]=0;
                  uk_bt->TTn[5]=0;
                }

                s_dem[0]= (ABS_LIN[1]+ABS_LIN[0]) * 0.5;
                s_dem[1]= ABS_LIN[2];
                s_dem[3] =(ABS_LIN[1] - ABS_LIN[0])/scale_hall_d;

                s_dem1[0]=s_dem[0];
                s_dem2[0]=s_dem[0];
                a_zd_1[0]=0.;

                s_dem1[1]=s_dem[1];
                s_dem2[1]=s_dem[1];
                a_zd_1[1]=0.;

                s_dem1[3]=s_dem[3];
                s_dem2[3]=s_dem[3];
                a_zd_1[3]=0.;

                MIN_ABR[3]=-1.;
                MIN_ABR[4]=-1.;
                MIN_ABR[5]=-1.;

                flag_md= 0;

                Statef_s[2]=0;
                Statef_s[3]=0;
                Statef_s[4]=0;
                if ( Statef_s[0] == REP_FND)
                 {
                   var[I_p_max +0]=var[I_s_max +0];
                   var[I_p_min +0]=var[I_s_min +0];
                 }
                else
                 {
                   var[I_p_max +0]=BIG_P;
                   var[I_p_min +0]=-BIG_P;
                 }

                if ( Statef_s[1] == REP_FND)
                 {
                   var[I_p_max +1]=var[I_s_max +1];
                   var[I_p_min +1]=var[I_s_min +1];
                 }
                else
                 {
                   var[I_p_max +1]=BIG_P;
                   var[I_p_min +1]=-BIG_P;
                 }

                var[I_p_max +3]=var[I_s_max +4];
                var[I_p_min +3]=var[I_s_min +4];

                f_clr_buf();
          //    enable_int();

                goto fin;
              }
              goto fin_e;
             }
             goto fin;
          }
#endif
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='S')&& (intrpr.wrd[1]=='D') )
         /*  -----   command  'SD' 'SD '-------- */
         /* ---SD - Step_Dir                ---- */
          {
m_sd:
           ftmp=flag_st_d;
           if(f_dis_set(ds_sd,2,5)>0)
            {
             if(flag_st_d != ftmp)
               {
                disable_int();
                  if(flag_st_d==0.)
                   {
                    f_iter=f_otr_it;
                    Ready_Low(0);
                    Ready_Low(1);
                    Ready_Low(2);
                    Ready_Low(3);

                    f_clr_buf();
                    // enable_int() present in f_clr_buf();
                   }
                  else
                   {

//-------------------------------------
                       /* выключение пульта */
                       if(Flag_pult)
                       {
                        Flag_pult = 0;
                        f_delay_p(100000.);
                       }
//-------------------------------------

                   // -- turn off USB interpolator
                      flag_USB_int=0;
                      flag_USB_int0=0;
                      usbi_beg = 0;
                      usbi_fin = 0;
                      frame_n=0;
                   //-----------------------------
                    f_iter=f_step_dir ;
                    enable_int();
                   }
               }
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='U')&& (intrpr.wrd[1]=='V') )
         /*  -----   command  'UV' 'UV '-------- */
         /* ---UV - stream of output for interpreter ---- */
          {
           f_dis_set(ds_uv,2,1);
           goto fin;
          }
    /*----------------------------------------------*/

#if( (KR_PR != 0) )
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='A')&& (intrpr.wrd[1]=='I') )
         /*  -----   command  'AI' -------- */
         /* ---AI - type of axis information ---- */
          {
           if(f_dis_set(ds_ai,2,3)>0)
           {

           }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='U')&& (intrpr.wrd[1]=='D') )
         /*  -----   command  'UD' 'UD '-------- */
         /* ---UD - USB interpolator ON/OFF ---- */
          {
           flag_USB_int=flag_USB_int0;
           if( f_iter == f_USB_int  )  flag_USB_int= 0.5;

           ftmp=flag_USB_int;

           if(f_dis_set(ds_ud,2,1)>0)
            {
             if(flag_USB_int != ftmp)
               {
                disable_int();
                  if(flag_USB_int==0.)
                   {
                    f_iter=f_otr_it;
                    flag_USB_int0=0;

                    s_dem1[0]= *ss_dem[0];
                    s_dem2[0]= *ss_dem[0];

                    s_dem1[1]= *ss_dem[1];
                    s_dem2[1]= *ss_dem[1];

                    s_dem1[3]= *ss_dem[3];
                    s_dem2[3]= *ss_dem[3];

                    usbi_beg = 0;
                    usbi_fin = 0;
                    frame_n=0;
                    f_clr_buf();
                    // enable_int() present in f_clr_buf();
                   }
                  else
                   {
//-------------------------------------
                       /* выключение пульта */
                       if(Flag_pult)
                       {
                        Flag_pult = 0;
                        f_delay_p(100000.);
                       }
//-------------------------------------
                  // turn off Step-Dir interpolator
                    flag_st_d=0;
                    Ready_Low(0);
                    Ready_Low(1);
                    Ready_Low(2);
                    Ready_Low(3);
                  //-------------------------------
                   blc_fin=4;
                   for(i=0;i<4;i++)
                    for(i1=0;i1<blc_lgth;i1++)
                      Int_buf[3][blc_lgth]=0;
                  //-------------------------------

                    f_iter=f_USB_int ;

                    S_int0[0]= ( (int)(*ss_dem[0] * 1000.) << 4)  & 0xffffff;
                    S_int0[1]= ( (int)(*ss_dem[1] * 1000.) << 4)  & 0xffffff;
                    S_int0[3]= (int)(*ss_dem[3] * ScaleKR_ROT1 );

                    if(*ss_dem[0]>=0)
                      S_int1[0] =  *ss_dem[0] / Cost_INT1;
                    else
                      S_int1[0] = (int)( *ss_dem[0] / Cost_INT1)  - 1 ;

                    if(*ss_dem[1]>=0)
                      S_int1[1] =  *ss_dem[1] / Cost_INT1;
                    else
                      S_int1[1] = (int)( *ss_dem[1] / Cost_INT1)  - 1 ;

                    enable_int();
                   }
               }
            }
           goto fin;
          }
    /*----------------------------------------------*/
#endif


    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F')&& (intrpr.wrd[1]=='A') )
         /* --------  команда 'FA'  ------------ */
         /* ---  FA - Feed,Acceleration,Jerk  ---- */
          {

           if((fl_move_off !=0 ) && (flag_ea == 0))
           {
             kk_p=v_mm_s;
             kk_i=a_m_s2;
             f_dis_set(ds_kf,2,2);
           }
           else
            {
#if(TR_JERK != 0)
           itmp1=f_dis_set(ds_fa,2,3);
#else
           itmp1=f_dis_set(ds_fa,2,2);
#endif

              if(itmp1 > 0 )  f_v_a();
              else if(itmp1<0) goto fin_e;
            }
           goto fin;
          }
    /*----------------------------------------------*/
#if(PLANAR != 0)
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L')&& (intrpr.wrd[1]=='N') )
           /* --------  command 'LN'  ------------ */
           /*  LN - Lining motor                 */
          {
           if(intrpr.wrd[2]=='P')  // command 'LNP'
            {
             // ---- Display/input of parameters  ----

              f_dis_set(ds_ln,3,8);
              goto fin;
            }
           else if(intrpr.wrd[2]=='S')      // command 'LNS'
            {
              if( f_ln_s() ) goto fin_e;
              goto fin;
            }
           else if(intrpr.wrd[2]=='D')      // command 'LND'
            {
              if( Flag_ln == 10 ) { if(flag_ea  == 0)   puts(" 10");}
              else if( Flag_ln == 0 )
                {
                 if(flag_ea  == 0)  puts(" 0");
                 else goto ln1;
                }
              else
               {
ln1:              f_Read_ADC_A();                   // read ADC

                  f_fff( ADC,FFFL,Null_L,Norm_L );  // count new position

                   ftmp1=Lna_sc*LNAD_SC;

                   if( (fff_ampl[0] > ftmp1) || (fff_ampl[1] > ftmp1) || (fff_ampl[2] > ftmp1) )
                    {
                    // amplitudes too low,  no need check phases
                     itmp= -2;
                     goto ln11;
                    }

                // count differences
                  for(i=0;i<3;i++)
                   {
                    tmpfl[i] = ( FFFL[i] -  Null_P_L[i]);
                    if( fabs(tmpfl[i]) > (fabs(pf[i]) * 0.5) )
                      if( tmpfl[i] > 0.)  tmpfl[i]-=fabs(pf[i]);
                      else tmpfl[i] += fabs(pf[i]);
                   }

                  if( (fabs(tmpfl[0]) < LIN_MAX) && (fabs(tmpfl[1])<LIN_MAX) && (fabs(tmpfl[2])<LIN_MAX) )
                  {
                    tmpfl[0]= tmpfl[0]-tmpfl[1];
                    if( fabs(tmpfl[0]) > (fabs(pf[0]) * 0.5) )
                      if( tmpfl[0] > 0.)  tmpfl[0]-=fabs(pf[0]);
                      else tmpfl[0] += fabs(pf[0]);

                    if( fabs(tmpfl[0]) >= (LIN_MAX * Lvl_LNP) )
                    {
                     itmp=-3;
                     goto ln11;
                    }
                //  Lining is successfull
                    if(flag_ea  == 0) puts(" 1");
                    Flag_ln=1;
                  }
                  else
                   {
                    itmp=-1;
  ln11:             if(flag_ea  == 0) printf(" %d",itmp);
                    Flag_ln=-itmp;
                   }
               }
              goto fin;
            }
           else if(intrpr.wdl==2)
            { // ---- Lining motor
              /*------------------*/

              if(f_inw()) strg=intrpr.wrd;
              else
               {
                 if(f_lin_mot0()) goto fin_e;
                 goto fin;
               }
            }
            else
                strg=&intrpr.wrd[2];

            if(f_innum(strg) )
              {
               if(intrpr.num_var == 0.)
                 {
                   Flag_ln=0;
                   goto fin;
                 }
              }
            goto fin_e;
       /*------------------*/
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='R')&& (intrpr.wrd[1]=='T')&& ((intrpr.wdl==2) ||(intrpr.wrd[2]=='-')) )
           /* --------  команда 'RT','RT-'   ------------ */
          {
           if(Flag_sys[0] != FR) goto fin_e;

           kk_p=v_tek_m;
           v_tek_m=1.0;
           v_max =f_mm_it(v_mm_s_c);
           a_zad =f_ms2_it(a_m_s2_c);
           sz_0=0;
            /*--------------*/
           if(FYR == 0.)
                        ftmp=AMPL[2];
           else
                        ftmp=AMPL[0];
           //-----------------------------------
           // reducing current in not used phase to Ampl_y_lin
           // (FYR == 0  - Y) during the rotation

            ftmp2=Ampl_y_lin;

           if(FYR == 0.)
             while(fabs(ftmp2-AMPL[2]) > D_DAC)
              {
              if(ftmp2 > AMPL[2]) AMPL[2]+=D_DAC;
              else AMPL[2]-=D_DAC;
              if(Flag_Y_rot)
                 AMPL[3]=AMPL[2];
              f_delay(T_INCR);
              }
           else
             while(fabs(ftmp2-AMPL[0]) > D_DAC)
              {
              if(ftmp2 > AMPL[0]) AMPL[0]+=D_DAC;
              else AMPL[0]-=D_DAC;
              AMPL[1]=AMPL[0];
              f_delay(T_INCR);
              }
           //-----------------------------------
           if(intrpr.wrd[2]=='-')
               f_per_rot( -fabs(pf[0]) );
           else
               f_per_rot( fabs(pf[0]) );

         // set normal current in not used  phase (FYR == 0  - Y)

               ftmp2=ftmp;
               if(FYR == 0.)
               {
                 while(fabs(ftmp2-AMPL[2]) > D_DAC)
                   {
                    if(ftmp2 > AMPL[2]) AMPL[2]+=D_DAC;
                    else AMPL[2]-=D_DAC;
                    if(Flag_Y_rot)
                       AMPL[3]=AMPL[2];
                    f_delay(T_INCR);
                   }
                  AMPL[2]=ftmp;
                  if(Flag_Y_rot)
                     AMPL[3]=ftmp;
               }
               else
               {
                 while(fabs(ftmp2-AMPL[0]) > D_DAC)
                   {
                    if(ftmp2 > AMPL[0]) AMPL[0]+=D_DAC;
                    else AMPL[0]-=D_DAC;
                    AMPL[1]=AMPL[0];
                    f_delay(T_INCR);
                   }
                 AMPL[0]=ftmp;
                 AMPL[1]=ftmp;
               }
           f_delay(100000.);

           printf("  FF %f %f %f",FFFF_C[0],FFFF_C[1],FFFF_C[2]);

           v_tek_m=kk_p;
           f_v_a();

           goto fin;
          }
    /*----------------------------------------------*/


#endif    // (PLANAR != 0)
    /*----------------------------------------------*/
        if ( intrpr.wrd[0]=='V' )
         /* --------  команда 'VX','VY','VZ' ------------ */
  #if(TWO_MS == 0)
          if( (i=f_xyzcbu(2)) >= 0 )
  #else   //(TWO_MS != 0)
          if( (i=f_xyiju(2)) >= 0 )
  #endif
          {
           kk_p= v_max_s[i]   ;     /* скорость выхода на концевой датчик,mm/s */
           kk_i= v_maxl_s[i]  ;     /* скорость сползания с датчика */
           kk_d= v_maxll_s[i] ;     /* скорость от датчика до репера */
           kkat= v_maxlll_s[i];     /* скорость поиска репера */
           kk_f= a_max_s[i]   ;     /* ускорение поиска репера */

           if(f_dis_set( ds_kf,2,5) <0) goto fin_e;

           v_max_s[i]   = kk_p;
           v_maxl_s[i]  = kk_i;
           v_maxll_s[i] = kk_d;
           v_maxlll_s[i]= kkat;
           a_max_s[i]   = kk_f;

           goto fin;
          }
    /*-------------- --------------------------------*/
        if ( (intrpr.wrd[0]=='K')&&(intrpr.wrd[1]=='V'))
         /* --------  команда 'KV'  ------------ */
         /* ----  KV  - velocity factor ----- */
          {
           if((fl_move_off !=0 ) && (flag_ea == 0))
           {
             kk_p=v_tek_m;
             f_dis_set(ds_kf,2,1);
           }
           else
          {
           if(Flag_pult)
           {
            k_r_c =  2.* PI * r_c  /360.;
 v_tmp= (sqrt(vp_tek[0]*vp_tek[0] +vp_tek[1]*vp_tek[1] +vp_tek[2]*vp_tek[2] + vp_tek[3]*vp_tek[3]*k_r_c*k_r_c ) ) * k_it_mm;
           }
           else v_tmp=v_t * k_it_mm;

           if( (itmp1= f_dis_set(ds_kv,2,1)) >0 )
           {
           if(fl_trace_loop == 0)
                 uk_buf=0;
//  place next 2 string in all place
// where  v_tek_m  is changing

            if(v_tek_m > 0)
                    v_tek_m1=v_tek_m;
           }
//         else if(itmp1<0) goto fin_e;
          }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='S')&&(intrpr.wrd[2]=='S'))
         /* --------  команда 'NSS'  ------------ */
         /* ----  NSS  - Number of storsge ------ */
          {
           f_dis_set(&ds_ns[7],3,1);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='S'))
         /* --------  команда 'NS'  ------------ */
         /* ----  NS  - Number of storsge ------ */
          {
           if((itmp1=f_dis_set(ds_ns,2,8) ) >0 )
                          f_ns_count();
           else if(itmp1<0) goto fin_e;
//         fl_trace_loop=0;

           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N')&&(intrpr.wrd[1]=='T'))
         /* --------  команда 'NT'  ------------ */
         /* ----  NT  - Noise time --- */
          {
           if(f_dis_set(ds_nt,2,4)>0 ) {  f_cnt_dec(); f_count_cf();}
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='P')&&(intrpr.wrd[1]=='B'))
         /* --------  команда 'PB'  ------------ */
         /* ----  PB  - Pult bottom --- */
          {
           if(f_dis_set(ds_pb,2,14)>0) f_cfg_key();
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='L')&&(intrpr.wrd[1]=='S'))
         /* --------  команда 'LS'  ------------ */
         /* ----  LS  - Light switch --- */
          {
           if(f_dis_set(ds_ls,2,2)>0 )
            {
              f_led_arr();
              f_cnt_dec();
            }
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if  (intrpr.wrd[0]=='F')
         /* --------  команда 'FX','FY','FZ','FC' ------ */
          {
           if(intrpr.wdl==1)
             if(f_inw()) goto fin_e;
             else
              {
               printf(" %f %f %f %f %f %f",Flag_sys[0],Flag_sys[1],Flag_sys[2],Flag_sys[3],Flag_sys[4],Flag_sys[5]);
               goto fin;
              }
           if( (i=f_xyzcbu(2)) >= 0 )
            {
              if(intrpr.wdl==2)
                 if(f_inw()) strg=intrpr.wrd;
                 else { printf(" %f",Flag_sys[i]);goto fin;}
              else
                  strg=&intrpr.wrd[2];
              if(f_innum(strg) )
              {
                 itmp0=(int)intrpr.num_var;
                 switch(itmp0)
                 {
                  case 0:
                          {
                            f_NO_FS(i);
                            goto fin;
                          }
                  case 1:
                          {
                            if( (flag_fast[i] == 0) && (Null_Period[i] < 0) )
                             {
                              if(Flag_pult)
                               {
                                 Flag_pult = 0;
                                 f_delay_p(50000.);
                               }
                             }
                            f_FS(i);        /* разрешение и замыкание  системы */
                            goto fin;
                          }
                  case 2:
                          {
                            f_stop_fx(i);
                            f_FS2(i);
                            goto fin;
                          }
                 }
                  goto pr_bad;
              }
              else  goto fin_e;
              goto fin;
            }
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='W') && (intrpr.wrd[1]=='E') )
         /*  -----   command  'WE'  ------------ */
          if(intrpr.wdl==2)
          {
m_we_00:

 #if (Cor_XY != 0)

          for(j=0;j<n_cor_xy_ax ;j++)
           {
            f_cor_xy_NO_FS(j);
            cor_xy[j].TT_PER_1_x=0.; // запретить использование данных коррекции
           }
 #endif // (Cor_XY != 0)

            tmp1=0; // flag normal block used

            if(f_wait_ready_flash() ) goto fin_e;
            i1= CURR_BASE;

            CURR_BASE = f_get_new_base(CURR_BASE);
  m_we:
            adr_getch_fl = CURR_BASE + CURR_BASE;

            if( (getch_fl() != 0xff)||(getch_fl() != 0xff)||(getch_fl() != 0xff)||(getch_fl() != 0xff) )
            {
              f_sect_erase(CURR_BASE);
              if(f_wait_ready_flash() ) goto fin_e;
            }
            itmp1=0;
            base_cod+=1.;
            itmp1+=f_wrt_flash();
            fl_c_buf=0;
    /*
            adr_putch_fl= b_adr_prg_fl + CURR_BASE + CURR_BASE;
            adr_getch_ram=( (int) (&prg[0]) )<<2;
            itmp1+=f_wr_fl_prg(MAX_PRG);
   */
            adr_putch_fl= b_adr_Hlp_fl + CURR_BASE + CURR_BASE;
            adr_getch_ram=( (int) (&hlp[0]) )<<2;
            itmp1+=f_wr_fl_prg(L_HLP);

            itmp1+=f_wr_fl_cor();
            if(itmp1)
            {
               CURR_BASE=i1;
               goto fin_e;
            }

            itmp1+=f_wr_fl_cor_Surf();
            if(itmp1)
            {
               CURR_BASE=i1;
               goto fin_e;
            }

            if(tmp1 != 0)
             {
               CURR_BASE=i1;
               goto fin;
             }
            i1= f_get_new_base(CURR_BASE);
            if(f_wait_ready_flash() ) goto fin_e;
            f_sect_erase(i1);
            if(f_wait_ready_flash() ) goto fin_e;
            f_delay(200000);

 #if (Cor_XY != 0)
  f_corXY_init();
//  f_cor_xy_ON(0,0);

 #endif // (Cor_XY != 0)

            goto fin;
          }
          else if( (intrpr.wdl==3) &&( (i=f_1234(3)) >= 0 ) )
          {
           i1= CURR_BASE;
           CURR_BASE = C_BAS[i];

           tmp1=1; // flag extended block used
           goto m_we;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='R') && (intrpr.wrd[1]=='E'))
          if(intrpr.wdl==2)
         /*  -----   command  'RE' ------------ */
          {
            tmp1=0; // flag normal block used
      m_re:
           f_rd_ee();

           f_rd_char();

           f_rd_ee_cor();
           f_rd_ee_cor_Surf();
           f_cor_init();

           if(tmp1 != 0)
             {
               CURR_BASE=i1;
             }

// printf("\n base_cod = %f",base_cod);

              disable_int();
              f_arctg();
              f_v_a();
              f_v_az();
              f_cnt_dec();

              f_tabl();
              f_led_arr();
              f_inv_msk();
              f_ns_count();
              f_tim0_set();
              f_count_cf();

  #if ( NOVS != 0)
        f_init_twl();
  #endif // ( NOVS != 0)

              enable_int();

           goto fin;
          }
          else if( (intrpr.wdl==3) &&( (i=f_1234(3)) >= 0 ) )
          {
           i1= CURR_BASE;
           CURR_BASE = C_BAS[i];
           tmp1=1; // flag extended block used
           goto m_re;
          }
    /*----------------------------------------------*/
#if(LOTIS != 0)

        /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F') && (intrpr.wrd[1]=='D')&& (intrpr.wrd[2]=='T') )
         /*  -----   command  'FDT'  ------------ */
         /* ---FDT - Flash moDe  Time parameters          ---- */
          {
           if(f_dis_set(ds_fdt,3,3) > 0) goto m_setfd;
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F') && (intrpr.wrd[1]=='D')&& (intrpr.wrd[2]=='P') )
         /*  -----   command  'FDP'  ------------ */
         /* ---FDP - Flash moDe  Parameters          ---- */
          {
           f_dis_set(ds_fdp,3,3);
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F') && (intrpr.wrd[1]=='D') )
         /*  -----   command  'FD'  ------------ */
         /* ---FD - Flash moDe            ------ */
          {
           ftmp=delay_ff;
           if(f_dis_set(ds_fd,2,1)>0 )
            {
             if(ftmp == delay_ff) goto fin;
             disable_int();
               if(delay_ff==0.)
                {
                 fnn_tim1=0.5 + Per_flash * 1000000. /  ( t_int /k_t_mks);
                 fnn_tim2=  Length_pulse * 1000000.  /  ( t_int /k_t_mks);

                 f_iter=f_otr_it;
                 f_tim1_set0();
                 f_clr_buf();
                 flag_first_lt=1;
                }
               else
                {
      m_setfd:
                 fnn_tim1= 0.5 + Per_flash * 1000000. /  ( t_int /k_t_mks);
                 fnn_tim2= Length_pulse * 1000000.  /  ( t_int /k_t_mks);

                 flag_first_lt=1;
                 per_lt = t_int* 0.000001/k_t_mks;
                 f_tim1_set();
                 f_clr_buf_ltp();
                 f_iter=f_otr_lt;
                }
             enable_int();
            }
           goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F') && (intrpr.wrd[1]=='N') )
         /* --------  command 'FN'  ------------ */
         /* ---  FN - last Flash number ---- */
          {
           if(intrpr.wdl==2)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
                printf(" %d",flash_num);
                goto fin;
               }
           else
              strg=&intrpr.wrd[2];
           if(f_innum_i(strg) )
            {
             flash_num = intrpr.num_var_i;
             if ( flash_num < 0.) flash_num=0;

             var[ Lot_tim+1 ] = flash_num;
             var[ Lot_tim+2 ] = flash_num;
             var[ Lot_tim+3 ] = flash_num;
             var[ Lot_tim+4 ] = flash_num;

            }
           else goto fin_e;
           goto fin;
          }
/*----------------------------------------------*/


#endif
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I') && (intrpr.wrd[1]=='D')&& (intrpr.wrd[2]=='3')&& (intrpr.wrd[3]=='1')&& (intrpr.wrd[4]=='5'))
         /* --------  command 'ID315' ------------ */
         if(intrpr.wdl==5)
          {
           f_check_id(n_id);
           n_id=f_res_id;
           goto fin_e;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='I') && (intrpr.wrd[1]=='D') )
         /* --------  command 'ID' ------------ */
          {
           if(f_dis_set(&ds_id,2,1) == 0)
            {
             puts(" ");
             printf("%d %d",*( unsigned int *)(ID_ADDR),*( unsigned int *)(ID_ADDR+7) );
            }

#if(Id_check != 0)
           if( f_check_id(n_id) == 0) id_ok=1;
           else id_ok=0;
#endif

           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='W'))
         /* --------  команда 'DW' ------------ */
         /* Display Warnings */
          {
              if(intrpr.wdl==2)
                if(f_inw()) strg=intrpr.wrd;
                else
                 {
                  printf(" %8x",warn);
                  goto fin;
                 }
              else
                strg=&intrpr.wrd[2];

              if( !sscanf(strg, " %x ", &itmp0) ) goto fin_e;
              var[Warning] = itmp0;
              goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='E')&& (intrpr.wrd[2]=='E'))
         /* --------  команда 'DEE' ------------ */
         /* Display Extended Errors  */
          {
            printf(" %6x %6x %6x %6x %6x %6x",(int)State_xp[0],(int)State_xp[1],(int)State_xp[2],(int)State_xp[3],(int)State_xp[4],(int)State_xp[5]);
          /*
            for(i=0;i< 6;i++)
            {
              if( ((int)flag_err[i]) &  NP_ERR )
               {
                printf("\nPolus defining error :");
                  np_err_print(i,(int)State_xp[i]);
               }
            }
           */
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='E')&& (intrpr.wrd[2]=='Q'))
         /* --------  команда 'DEQ' ------------ */
         /* Display QLC Errors  */
          {
            puts( buf_deq);
            goto fin;
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='C') && (intrpr.wrd[1]=='E')&& (intrpr.wrd[2]=='Q'))
         /* --------  команда 'CEQ' ------------ */
         /* Clear QLC Errors  */
          {
            deq_pnt=0;
            deq_pnt+= sprintf(&buf_deq[deq_pnt]," - No QLC error\n");
            goto fin;
          }
    /*----------------------------------------------*/
#if (COR_INF != 0)
 #if (Cor_XY == 0)
        if ( (intrpr.wrd[0]=='C') && (intrpr.wrd[1]=='I'))
         /* --------  команда 'CI' ------------ */
         /* Correction of Infuence Y to X */
          {

           if( ((m_fin-m_beg) & (sz_move-1)) != 0) goto fin_e;
           kk_p=cor_fct[0];
           if(f_dis_set( ds_kf,2,1)> 0)
            if( (kk_p== 0) && (cor_fct[0] != 0))
            {
             // отключить коррекцию
              ftmp=f_cori_val();
              disable_int();
              cor_fct[0]=0;
              *ss_dem[0]+=f_cori_val();
     *ss_demF[0]=*ss_dem[0];
     s_dem1v[0]=*ss_dem[0];
     s_dem1[0]=*ss_dem[0];
     s_dem2[0]=*ss_dem[0];
              f_clr_buf();
            }
            else if( (kk_p != 0) && (cor_fct[0] == 0))
            {
             // включить коррекцию
               if( *uk_t_kor[0]>=1.0 )
                {
                  ftmp=f_cori_val();
                  disable_int();
                  cor_fct[0]=1;
                  *ss_dem[0]-=f_cori_val();
     *ss_demF[0]=*ss_dem[0];
     s_dem1v[0]=*ss_dem[0];
     s_dem1[0]=*ss_dem[0];
     s_dem2[0]=*ss_dem[0];
                  f_clr_buf();
                }
            }
            goto fin;
          }
//----------------------------------------------------

 #endif // (Cor_XY == 0)
#endif  // (COR_INF != 0)
    /*----------------------------------------------*/
#if(Cor_XY != 0)
//----------------------------------------------------
        if ( (intrpr.wrd[0]=='C') && (intrpr.wrd[1]=='I')&& (intrpr.wrd[2]=='V'))
         /* --------  команда 'CIV' ------------ */
         /* Correction value */
          {
           {  // no modified parameters


           for(i1=0;i1<n_cor_xy_ax;i1++)
              *ds_kf[i1].op=f_cori_val_XY(i1);

            f_dis_set(ds_kf,3,n_cor_xy_ax);
           }
           goto fin;
          }
//----------------------------------------------------
        if ( (intrpr.wrd[0]=='C') && (intrpr.wrd[1]=='I')&& (intrpr.wrd[2]=='I'))
         /* --------  команда 'CII' ------------ */
         /* Correction in plane  */
          {
           if(fl_trace_loop == 0)
              if(uk_buf >= (dim_xyz-11)) uk_buf=0;

           if( ((m_fin-m_beg) & (sz_move-1)) != 0) goto fin_e;

           for(i1=0;i1<n_cor_xy_ax;i1++)
              *ds_kf[i1].op=cor_xy[i1].cor_fct;

           if(f_dis_set( ds_kf,3,n_cor_xy_ax) > 0)
           {
              for(i1=0;i1<n_cor_xy_ax;i1++)
              {
                if((*ds_kf[i1].op==0) && (cor_xy[i1].cor_fct != 0))
                {
                 // отключить коррекцию

                   if(i1 < n_ax)
                   {
                    Statef_s_sv[i1]=Statef_s[i1];
                    Statef_s[i1]=COR_OFF;
                    Flag_sens++;
                   }
                   else
                      cor_xy[i1].cor_fct=0;
                }
                else if( (*ds_kf[i1].op != 0) && (cor_xy[i1].cor_fct == 0))
                {
                // включить коррекцию
                  if( cor_xy[i1].TT_PER_1_x > 0 )
                   {

                     if(i1 < n_ax)
                     {
                      for(i2=0;i2<n_cor_xy_ax;i2++)
                        arr_cor_fl[i1][i2]=0;

                      Statef_s_sv[i1]=Statef_s[i1];
                      Statef_s[i1]=COR_ON;
                      Flag_sens++;
                     }
                     else
                       cor_xy[i1].cor_fct=1;
                   }
                }
              }
           }
           goto fin;
          }
//----------------------------------------------------
        if ( (intrpr.wrd[0]=='C') && (intrpr.wrd[1]=='I'))
         /* --------  команда 'CI' ------------ */
         /* Correction in plane  */
          {
           if( ((m_fin-m_beg) & (sz_move-1)) != 0) goto fin_e;

           for(i1=0;i1<n_cor_xy_ax;i1++)
              *ds_kf[i1].op=cor_xy[i1].cor_fct;

           if(f_dis_set( ds_kf,2,n_cor_xy_ax) > 0)
           {

              for(i1=0;i1<n_cor_xy_ax;i1++)
              {
                if((*ds_kf[i1].op==0) && (cor_xy[i1].cor_fct != 0))
                {
                 // отключить коррекцию
                  f_cor_xy_NO_FS(i1);
                  f_clr_buf();
                }
                else if( (*ds_kf[i1].op != 0) && (cor_xy[i1].cor_fct == 0))
                {
                // включить коррекцию
               /*
                  if( cor_xy[i1].TT_PER_1_x > 0 )
                   {
                     ftmp=f_cori_val_XY(i1);
                     disable_int();
                     cor_xy[i1].cor_fct=1;
                     *ss_dem[i1]-=ftmp;
                     f_clr_buf();
                   }
               */

                     if(i1 < n_ax)
                     {
                      for(i2=0;i2<n_cor_xy_ax;i2++)
                        arr_cor_fl[i1][i2]=0;

                      Statef_s_sv[i1]=Statef_s[i1];
                      Statef_s[i1]=COR_ON;
                      Flag_sens++;
                     }
                     else
                       cor_xy[i1].cor_fct=1;

                }
              }
           }
           goto fin;
          }
//----------------------------------------------------
 #endif // (Cor_XY != 0)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='E'))
         /* --------  команда 'DE' ------------ */
         /* Display Errors  */
          {

              itmp0= flag_err[0];
              itmp1= flag_err[1];
              itmp2= flag_err[2];
              itmp3= flag_err[3];
              itmp4= flag_err[4];
              itmp5= flag_err[5];
              itmp6= flag_err[6];

              if(intrpr.wdl==2)
                 if(f_inw()) strg=intrpr.wrd;
                 else
                  {
                   printf(" %6x %6x %6x %6x %6x %6x %6x",itmp0,itmp1,itmp2,itmp3,itmp4,itmp5,itmp6);
                   goto f_nde1;
                  }
              else
                 strg=&intrpr.wrd[2];

              if( !sscanf(strg, " %x ", &itmp0) ) goto fin_e;
              if(!f_inw()) goto f_nde;
              if( !sscanf(intrpr.wrd, " %x ", &itmp1) ) goto fin_e;
              if(!f_inw()) goto f_nde;
              if( !sscanf(intrpr.wrd, " %x ", &itmp2) ) goto fin_e;
              if(!f_inw()) goto f_nde;
              if( !sscanf(intrpr.wrd, " %x ", &itmp3) ) goto fin_e;
              if(!f_inw()) goto f_nde;
              if( !sscanf(intrpr.wrd, " %x ", &itmp4) ) goto fin_e;
              if(!f_inw()) goto f_nde;
              if( !sscanf(intrpr.wrd, " %x ", &itmp5) ) goto fin_e;
              if(!f_inw()) goto f_nde;
              if( !sscanf(intrpr.wrd, " %x ", &itmp6) ) goto fin_e;
f_nde:
              if(itmp0)  f_error(0,itmp0);
              else flag_err[0] = itmp0;

              if(itmp1)  f_error(1,itmp1);
              else flag_err[1] = itmp1;

              if(itmp2)  f_error(2,itmp2);
              else flag_err[2] = itmp2;

              if(itmp3)  f_error(3,itmp3);
              else flag_err[3] = itmp3;

              if(itmp4)  f_error(4,itmp4);
              else flag_err[4] = 0;

              if(itmp5)  f_error(5,itmp5);
              else flag_err[5] = 0;

              if(itmp6)  f_error(6,itmp6);
              else flag_err[6] = 0;


f_nde1:       if(flag_err[0] + flag_err[1] + flag_err[2] + flag_err[3] + flag_err[4] + flag_err[5] + flag_err[6]);
              else
               {
                 fl_err=1.;
//               fl_errn=1.;
//               fl_errn1=1.;
               }
              goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='V')&& (intrpr.wrd[1]=='A') && (intrpr.wrd[2]=='R') )
         /* --------  команда 'VAR'  ------------ */
         /* --------  команда 'VARX'  ------------ */
         /* --------  команда 'VARC'  ------------ */
          {
           i1=0;
           if(intrpr.wrd[3]=='X')  i=4;
           else if(intrpr.wrd[3]=='C') { i=4;i1=1;}
           else i=3;


           kk_p= num_par;
           kk_i=var[(int) kk_p];

           if(intrpr.wdl==i)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
                if(i==3)
                   printf(" %d %f",(int)kk_p,kk_i);
                else if(i1==0)
                   printf(" %d %6x",(int)kk_p,((int)kk_i & HEX_MAX) );
                else
                  print_c(&var[(int)kk_p],&var[(int)kk_p+1],&var[(int)kk_p+2],&var[(int)kk_p+3],&var[(int)kk_p+4],&var[(int)kk_p+5]);

                goto fin;
               }
           else
              strg=&intrpr.wrd[i];
           if(f_innum(strg) )
            {
             kk_p=intrpr.num_var;
             if( (kk_p >= n_svar) || (kk_p < 0.)) goto fin_e;
            }
           else goto pr_bad;

           if(f_inw())
             if(f_innum(intrpr.wrd) )
               {
                kk_i=intrpr.num_var;
                var[(int)kk_p]=kk_i;
               }
             else goto pr_bad;
           else
            {
              kk_i=var[(int) kk_p];
             if(i==3)
              printf(" %f",kk_i);
             else if(i1==0)
                   printf(" %6x",((int)kk_i & HEX_MAX) );
             else
                  print_c(&var[(int)kk_p],&var[(int)kk_p+1],&var[(int)kk_p+2],&var[(int)kk_p+3],&var[(int)kk_p+4],&var[(int)kk_p+5]);
            }
           num_par = kk_p;
           goto fin;
          }
    /*----------------------------------------------*/
//-------------------
#if(TR_DOUBLE == 0)

        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='D'))
         /* --------  команда 'DD' ------------ */
         /* Display Demand position  */

          if(intrpr.wdl==2)
            {
              if(f_inw()) goto fin_e;
              printf(" %f %f %f %f %f %f",*ss_dem[0],*ss_dem[1],*ss_dem[2],*ss_dem[3],*ss_dem[4],*ss_dem[5]);
              goto fin;


            }
#else  //(TR_DOUBLE == 1)
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='D'))
         /* --------  команда 'DD' ------------ */
         /* Display Demand position  */

          if(intrpr.wdl==2)
            {
              if(f_inw()) goto fin_e;
              for(i=0;i<5;i++)
              {
               printf(" ");
               dbl_op1[0]=*ss_dem[i];
               dbl_op1[1]=*ss_demd[i];
               print_dbl( dbl_op1);
              }
              goto fin;

            }


#endif  //(TR_DOUBLE == 0)
    /*----------------------------------------------*/
     if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='D')&& (intrpr.wrd[2]=='C'))
         {                 // 'DDC'
//          print_c(&var[Ind_S_zad],&var[Ind_S_zad+1],&var[Ind_S_zad+2],&var[Ind_S_zad+3],&var[Ind_S_zad+4],&var[Ind_S_zad+5]);
            print_c(ss_dem[0],ss_dem[1],ss_dem[2],ss_dem[3],ss_dem[4],ss_dem[5]);
            goto fin;
         }
    /*----------------------------------------------*/
     if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='R')&& (intrpr.wrd[2]=='C'))
         {                  // 'DRC'
            print_c(&var[Ind_ABS_LIN],&var[Ind_ABS_LIN+1],&var[Ind_ABS_LIN+2],&var[Ind_ABS_LIN+3],&var[Ind_ABS_LIN+4],&var[Ind_ABS_LIN+5]);
            goto fin;
         }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
     if ( (intrpr.wrd[0]=='S') && (intrpr.wrd[1]=='W') )
         {                  // 'SW'
           sw_tmp=mskf_sw;
           if(f_dis_set(&ds_sw,2,1) > 0 )
            {
              if(sw_tmp==0)
               {
                mskf_sw=sw_tmp;
                fl_errn1=1.;
               }
              else
              {
                f_init_sw(sw_tmp);
              }
            }
           goto fin;
         }
    /*----------------------------------------------*/
     if ( (intrpr.wrd[0]=='F') && (intrpr.wrd[1]=='M'))
         {                  // 'FM'

          if(Flag_pult == 0 )

#if(LOTIS != 0)
           if(delay_ff == 0.)
            {
             if(v_t != 0.)  putch_ea('1');
             else putch_ea('0');
            }
           else
            {
              for(i=0;i<n_ax;i++)
               if(s_dem2[i] != s_dem1[i])
               {
                putch_ea('1');
                goto fin;
               }
              putch_ea('0');
              goto fin;
            }
#else
            {
             if(v_t != 0.)  putch_ea('1');
             else putch_ea('0');
            }
#endif   //(LOTIS != 0)

          else
            if((vp_tek[0]*vp_tek[0] +vp_tek[1]*vp_tek[1] +vp_tek[2]*vp_tek[2]+vp_tek[3]*vp_tek[3] +vp_tek[4]*vp_tek[4] ) != 0) putch_ea('1');
            else putch_ea('0');
            goto fin;
         }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='N') && (intrpr.wdl==1) )
         /* --------  команда 'N ' ------------ */
         /* Display Demand position  */
            {
              if(f_inw()) goto fin_e;
              tmp=0;

//            for(i=0;i<NN_INT_A;i++)
              for(i=0;i<n_ax;i++)
              {
               ftmp1=(s_dem[i]+0.5);

#if ( SURF != 0)
               if(i==2)
                 if(flag_surf != 0.)  ftmp1=s_surf+0.5;
#endif

               if(ftmp1 <= 0.) ftmp1-=1.;
               tmp1=ftmp1;
               putch4( tmp1);
               tmp+= ( tmp1 & 0xff);
               tmp+= ( (tmp1>>8)  & 0xff);
               tmp+= ( (tmp1>>16) & 0xff);
               tmp+= ( (tmp1>>24) & 0xff);
              }

              if(v_tek_m > 327.)  v_tek_m =327.;

              tmp1=v_tek_m * 100.;

              if(Flag_pult == 0 )
               {
                if(v_t != 0.)  tmp1 |= 0x8000;
               }
              else if((vp_tek[0]*vp_tek[0] +vp_tek[1]*vp_tek[1] +vp_tek[2]*vp_tek[2]+vp_tek[3]*vp_tek[3] +vp_tek[4]*vp_tek[4] ) != 0) tmp1 |= 0x8000;

              putch2( tmp1);
              tmp+= ( tmp1 & 0xff);
              tmp+= ( (tmp1>>8)  & 0xff);

              putch2( tmp);
              goto fin;
            }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='I') && (intrpr.wrd[2]=='O'))
         /* --------  команда 'DIO' ------------ */
          {
           if(intrpr.wdl==3)
             if(f_inw()) goto fin_e;
             else
              {
               printf(" I %4x%4x O %4x%4x, PT %8x ",(int)mas_in[1],(int)mas_in[0],(int)mas_out[1],(int)mas_out[0],PT_PORT_IN_s);
               goto fin;
              }
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='R'))
         /* --------  команда 'DR' ------------ */
         /* Display Real position  */

          if(intrpr.wdl==2)
            {
              if(f_inw()) goto fin_e;

#if ( ( MELT == 0) && ( BELSHINA == 0) )
              printf(" %f %f %f %f %f %f",ABS_LIN[0],ABS_LIN[1],ABS_LIN[2],ABS_LIN[3],ABS_LIN[4],ABS_LIN[5]);
#endif

#if ( BELSHINA != 0)
  printf(" %f %f %f %f %f %f",ABS_LIN[0],ABS_LIN[2]-ABS_LIN[1],ABS_LIN[2],ABS_LIN[3],ABS_LIN[4],ABS_LIN[5]);
#endif

#if ( MELT != 0)
  printf(" %f %f %f %f %f %f",( TG1 * ABS_LIN[2] - TG2* ABS_LIN[0])* DTG12,ABS_LIN[1],(ABS_LIN[0]-ABS_LIN[2]) * DTG12,ABS_LIN[3],ABS_LIN[4],ABS_LIN[5]);
#endif
              goto fin;
            }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='X'))
         /* --------  команда 'DX' ------------ */
         /* Display eror  */
          if(intrpr.wdl==2)
            {
              if(f_inw()) goto fin_e;
   printf(" %f %f %f %f %f %f",XN_1[0],XN_1[1],XN_1[2],XN_1[3],XN_1[4],XN_1[5]);
              goto fin;
            }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
     if ( (intrpr.wrd[0]=='D') && (intrpr.wrd[1]=='X')&& (intrpr.wrd[2]=='C'))
         {                  // 'DXC'
            print_c(&XN_1[0],&XN_1[1],&XN_1[2],&XN_1[3],&XN_1[4],&XN_1[5]);
            goto fin;
         }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
        if (  (intrpr.wrd[0]=='P' ) && (intrpr.wdl==1))
         /*  -----   command  'P' 'P ' "P"  ------------ */
        {
             if(f_inw()!=0) goto fin_e;
             flag_p=1;
             n_lin=1;
             goto fin;
        }
    /*----------------------------------------------*/
    /*-----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='F')&& (intrpr.wrd[2]=='1') )
           /* --------  command 'DF1'  ------------ */
           /*  DF1 - Constant Force flag 1 */
          {
             if( (i=f_xyzcb(4)) >= 0 )
             {
              f_dis_set( &ds_df1[i],4,1);
              goto fin;
             }
          }
    /*-----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='F')&& (intrpr.wrd[2]=='2') )
           /* --------  command 'DF2'  ------------ */
           /*  DF2 - Constant Force flag 2 */
          {
             if( (i=f_xyzcb(4)) >= 0 )
             {
              fst_tmp=Flag_fst2[i];
              if(f_dis_set( &ds_df2,4,1)>0)
               if( (fst_tmp == 0.) && (Flag_fst2[i] != 0) )
               {
                Flag_fst1[i]=0;
                disable_int();

                a_zd_1[i]=0.;


#if(PLANAR == 0)
                s_dem[i]=ABS_LIN[i];
                s_dem1[i]=s_dem[i];
                s_dem2[i]=s_dem[i];

    if(flag_rv[i] == 0)
     {
      XN_1[i] = 0.;
      XN_2[i] = 0.;
      YN[i] = Force_fst[i] * Flag_fst2[i];


      uk_bt=&bt_xn[i];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;

     }
    else
     {
       XN_1[i] = 0.;
       XN_2[i] = 0.;
       YN[i] = 0;

       XN_1V[i] = 0.;
       XN_2V[i] = 0.;
       YNV[i] = Force_fst[i] * Flag_fst2[i];

      uk_bt=&bt_xn[i];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;

      uk_bt=&bt_v[i];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;

     }

#else // (PLANAR != 0)
  if(i==0)
    s_dem[0]=0.5*(ABS_LIN[0] + ABS_LIN[1]);
  else
    s_dem[i]=ABS_LIN[i+1];
    s_dem1[i]=s_dem[i];
    s_dem2[i]=s_dem[i];
//---------------------
    if(flag_rv[i+1] == 0)
     {
      XN_1[i+1] = 0.;
      XN_2[i+1] = 0.;
      YN[i+1] = Force_fst[i] * Flag_fst2[i];

      uk_bt=&bt_xn[i+1];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;

      #if(TWO_MS != 0)

      uk_bt=&bt_xn[i+4];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;

      #endif
     }
    else
     {
       XN_1[i+1] = 0.;
       XN_2[i+1] = 0.;
       YN[i+1] = 0;

       XN_1V[i+1] = 0.;
       XN_2V[i+1] = 0.;
       YNV[i+1] = Force_fst[i] * Flag_fst2[i];

      uk_bt=&bt_xn[i+1];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;

      uk_bt=&bt_v[i+1];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;


      #if(TWO_MS != 0)
      uk_bt=&bt_xn[i+4];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;

      uk_bt=&bt_v[i+4];
      uk_bt->TTn[0]=0;
      uk_bt->TTn[1]=0;
      uk_bt->TTn[2]=0;
      uk_bt->TTn[3]=0;
      uk_bt->TTn[4]=0;
      uk_bt->TTn[5]=0;

      #endif

     }
//---------------------

#endif // (PLANAR == 0)

                Flag_fst2[i]=0;

#if ( TR_INDEP != 0)

if(fl_ind_ax[i] != 0)
  {
        f_clr_ind(i);
  }
else
    f_clr_buf();
/*--------------*/
#else
  /*--------------*/
    f_clr_buf();
  /*--------------*/
#endif   // ( TR_INDEP != 0)
                enable_int();
               }
               else Flag_fst2[i]=fst_tmp;
              goto fin;
             }
          }
    /*-----------------------------------------------*/
        if ( (intrpr.wrd[0]=='D')&& (intrpr.wrd[1]=='F') )
           /* --------  command 'DF'  ------------ */
           /*  DF - Direct  Force  */
          {
           /*----------------------------------------------------*/
           /* ---- 'DFX','DFY','DFZ','DFC' constant force parameters --- */
             if( (i=f_xyzcb(3)) >= 0 )
             {

              if(f_dis_set( ds_df[i],3,4)>0)
              {
                 ftmp = 1000. * k_t_mks/t_int ;
                 Max_Cnt_Fst_i[i]=Max_Cnt_Fst[i] * ftmp;
              }
              goto fin;
             }
          }
    /*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='F')&& (intrpr.wrd[1]=='E')&& (intrpr.wrd[2]=='A') )
         /* --------  команда 'FEA'  ------------ */
          {
           itmp1=f_dis_set(ds_fea,3,2);
           goto fin;
          }
    /*----------------------------------------------*/
#if (MODULATION != 0)
   //-------------------------
        if ( (intrpr.wrd[0]=='M')&& (intrpr.wrd[1]=='O')&& (intrpr.wrd[2]=='D')&& (intrpr.wrd[3]=='D') )
         /* --------  команда 'MODD'  ------------ */
          {

           f_dis_set(&ds_modd,4,1);
           goto fin;
          }
   //-------------------------
        if ( (intrpr.wrd[0]=='M')&& (intrpr.wrd[1]=='O')&& (intrpr.wrd[2]=='D') )
         /* --------  команда 'MOD'  ------------ */
          {
           if(f_dis_set(ds_mod,3,3)>0) f_init_mod();
           goto fin;
          }
   //-------------------------
        if ( (intrpr.wrd[0]=='M')&& (intrpr.wrd[1]=='O')&& (intrpr.wrd[2]=='N') )
         /* --------  команда 'MON'  ------------ */
          {
           f_dis_set(&ds_mon,3,1);
           goto fin;
          }
   //-------------------------
#endif  // (MODULATION != 0)
    /*----------------------------------------------*/
/*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E') && (intrpr.wrd[1]=='D') )
         /* --------  command 'ED'  ------------ */
         /* ---  ED - ESCI display  ---- */
          {
           if(intrpr.wdl==2)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
m_ed:
     disable_int();
     IOSTRB= (  AND_EXT_INT | WS_7  );
       uk_int=(unsigned int *) (BAS_ESCI +  (ED_addr>>1));

                i = *uk_int;
   //  IOSTRB=  ( OR_EXT_INT | WS_7   );
     IOSTRB=(INTERNAL_RDY | WS_7 );
     enable_int();

                if(ED_addr & 1) i= (i>>8) & 0xff;
                else i &= 0xff;
                printf(" *0x%4x = 0x%2x", ED_addr, i);
                goto fin;
               }
           else
              strg=&intrpr.wrd[2];

           if( sscanf(strg, "%x", &i1) )
           {
             if ((i1 < 0x4000) && (i1 >= 0) )
                  ED_addr = i1;
             else  goto fin_e;
           }
           else  goto fin_e;

           goto m_ed;
          }
/*----------------------------------------------*/
        if ( (intrpr.wrd[0]=='E') && (intrpr.wrd[1]=='S')&& (intrpr.wrd[2]=='W') )
         /* --------  command 'ESW'  ------------ */
         /* ---  EW - ESCI write  ---- */
          {
           if(intrpr.wdl==3)
              if(f_inw()) strg=intrpr.wrd;
              else
               {
m_ew:

   disable_int();
     IOSTRB= (  AND_EXT_INT | WS_7  );
       uk_int=(unsigned int *) (BAS_ESCI +  (EW_addr>>1));

                i = *uk_int;
//     IOSTRB=  ( OR_EXT_INT | WS_7   );
     IOSTRB=(INTERNAL_RDY | WS_7 );

    enable_int();


                if( (EW_addr & 1) != 0 ) i= (i>>8) & 0xff;
                else i &= 0xff;
                printf(" *0x%4x = 0x%2x", EW_addr, i);
                goto fin;
               }
           else
              strg=&intrpr.wrd[3];

           if( sscanf(strg, "%x", &i1) )
            {
             if ((i1 < 0x4000) && (i1 >= 0) )
                  EW_addr = i1;
             else   goto fin_e;
            }
           else   goto fin_e;


          if(f_inw()) strg=intrpr.wrd;
          else goto m_ew;

          if( sscanf(strg, "%x", &i1) )
          {
             if ((i1 < 0x100) && (i1 >= 0) );
             else   goto fin_e;
          }
          else   goto fin_e;

   disable_int();
     IOSTRB= (  AND_EXT_INT | WS_7  );
       uk_int=(unsigned int *) (BAS_ESCI +  (EW_addr>>1));

                i = *uk_int;

           if(EW_addr & 1) i= (i & 0xff) | ((i1 << 8) & 0xff00);
           else i = (i & 0xff00) | (i1 & 0x00ff);
           *uk_int=i;

       uk_int=(unsigned int *) (BAS_ESCI +  (EW_addr>>1) +1 );

//     IOSTRB=  ( OR_EXT_INT | WS_7   );
     IOSTRB=(INTERNAL_RDY | WS_7 );
    enable_int();
           goto fin;
          }
/*----------------------------------------------*/
    /*----------------------------------------------*/
         /* --------  команда 'D '  ------------ */
         /* ---- D  - Display values--------- */
        if ( intrpr.wrd[0]=='D' )
          if(intrpr.wdl==1)
          {
           if(f_inw()) goto fin_e;

           if(fl_err==1.)  puts("\nStart");
           else    puts("\nStop");

           puts("         X             Y             Z             C             B             U");

           puts("\nState ");
           for(i=0;i<6;i++)
            switch((int)Flag_sys[i])
            {
             case No_Sys :{puts("     No move  ");continue;}
             case FR     :{puts("     Open     ");continue;}
             case FS     :{puts("    Folowing  ");continue;}
            }
           puts("\nReper ");
           for(i=0;i<6;i++)
            {
              switch(State_s[i])
              {
               case  0     :{puts("   Not found  ");continue;}
               case REP_FND:{puts("   Was found  ");continue;}
              }
              printf("Searching %2d  ",State_s[i]);continue;
            }

           puts("\nError ");
           for(i=0;i<6;i++)
            printf("    %8x  ",(int)flag_err[i]);

           printf("\n Dem  %12f  %12f  %12f  %12f  %12f  %12f",*ss_dem[0],*ss_dem[1],*ss_dem[2],*ss_dem[3],*ss_dem[4],*ss_dem[5]);
           printf("\n Real %12f  %12f  %12f  %12f  %12f  %12f",ABS_LIN[0],ABS_LIN[1],ABS_LIN[2],ABS_LIN[3],ABS_LIN[4],ABS_LIN[5]);

           printf("\n Xn1  %12f  %12f  %12f  %12f  %12f  %12f",XN_1[0],XN_1[1],XN_1[2],XN_1[3],XN_1[4],XN_1[5]);

           printf("\n Yn   %12f  %12f  %12f  %12f  %12f  %12f",YN[0],YN[1],YN[2],YN[3],YN[4],YN[5]);
           printf("\n Ampl %12f  %12f  %12f  %12f  %12f  %12f",AMPL[0],AMPL[1],AMPL[2],AMPL[3],AMPL[4],AMPL[5]);

           printf("\n Port IN %4x%4x, OUT %4x%4x, PT %8x ",(int)mas_in[1],(int)mas_in[0],(int)mas_out[1],(int)mas_out[0],stab_pult);
           printf("\n V  = %4f  A = %4f VT = %4f CC= %4f, KORZ =%4f",v_max,a_zad,v_t,cc,dxn );
           printf("\n Tintr, mks = %4f Msk inv = %8x  BF= %d",  t_int/k_t_mks ,msk_inv, ( (m_fin-m_beg) & (sz_move-1)  )/2);
//         printf("\n vp_tek[0]= %4f,vp_tek[1]= %4f, vp_tek[2]= %4f", vp_tek[0],vp_tek[1],vp_tek[2] );
//         printf("\n A_A= %10.8f,B_B= %10.8f, C_C= %10.8f", A_A,B_B,C_C );

//         printf("\n term3 = %d ", term3);
//         printf("\n Position =  %8x,%8x,%8x,%8x ", (int)Position[0],(int)Position[1],(int)Position[2],(int)Position[3] );

//         printf("\n DF  %6f  %6f  %6f  %6f  %6f  %6f",DF[0],DF[1],DF[2],DF[3],DF[4],DF[5]);
//         printf("\n Flag_fnp1 = %8x",Flag_fnp1);
           printf("\n UART_type ");
           if(UART_type == COM81C17)  printf("COM81C17");
           else if( UART_type == TL16C55C ) printf("TL16C55C");
           else if( UART_type == TL16C752 ) printf("TL16C752");
           else printf("unknown");
//         printf(", baud_cod= %8x",baud_cod);
           printf( "\n SP=%8x , ST= %8x, IE=%8x, IF=%8x",get_sp(),get_st(),get_ie(),get_if() );
           printf("\n P_OUT0 = %8x, P_OUT0H = %8x, P_OUT1 = %8x, P_OUT1H = %8x",(int)P_OUT0, (int)P_OUT0H, (int)P_OUT1, (int)P_OUT1H);
/*
           printf( "\n int0= %8x, int1= %8x, tmr0= %8x, tmr1= %8x",
                  *(volatile unsigned int *)(0x809fc1),
                  *(volatile unsigned int *)(0x809fc2),
                  *(volatile unsigned int *)(0x809fc9),
                  *(volatile unsigned int *)(0x809fca));
*/
           goto fin;
          }
    /*----------------------------------------------*/
    /*----------------------------------------------*/
    /* неопознанная команда  */
fin_e:  if(flag_ea == 0)
        {
         if(flag_spr == 0)
          {
            if(flag_DT_BR== 0)
                 puts  ("?\n>");
            else
             {

                if(flag_p==0)
                  printf("\n%1d?0%1d ",(int)Contr_NN,cod_er_acs);
                else
                  puts("\nP?43\n");

             }
          }
         else   printf("?\n#%1d>",(int)Contr_NN);
        }
        else
        {
          f_err_ea(INTR_ERR,0);
        }
fin_e1:
        intr_bsy = 0;
        flag_ea=0;
        fl_ea_io=0;
//      flag_spr=0;
        return;
fin_ee: enable_int();
        goto fin_e;

fin:
         if(flag_ea == 0)
         if(flag_spr == 0)
          {
            if(flag_DT_BR== 0)
                   puts  ("\n>");
            else
              {
                if(flag_p==0)
                   printf("\n%1d> ",(int)Contr_NN);
                else
                 {
fin_pp0:            if((n_lin== 616) ||(n_lin== 615) )
                      printf("\n%d:MSC:",n_lin);
                    else
                      printf("\n%d:rem",n_lin);
fin_pp:            printf("\nP>%d\n",n_lin);n_lin++;
                 }
              }
          }
         else   printf("\n#%1d>",(int)Contr_NN);
fin0:
          intr_bsy = 0;
          flag_ea=0;
          fl_ea_io=0;
//        flag_spr=0;
          return;
}
/*----------------------------------------------------*/
