
float Step_prn=10.; // mm
float LVL_max= 1400; //mm
float    Volume=0;
float Level=0;

#define max_src 4000
float MM[max_src];
float VV[max_src];
float L1[max_src];
float L2[max_src];
int n_src=0;

// вводит текстовый файл - TCT
// формирует таблицу вместимости резервуара в двоичном виде
// формирует таблицу вместимости резервуара с заданным шагом
// в файле TCT_PRN.txt
// данные по умолчанию в файле TCT.txt .

char fname_prn[]="TCT_PRN.txt";
        char fname[50]={"tct.txt"};
        char fnm_shrc[6]=".tct";
        char fnm_shrb[6]=".tcb";
        char ch_tct='x';

int flag_prn=1;

/*
 Input :  ASCII file with  Cyl data ,
 Output:  ASCII file with TCT data

 Last letter of file name must be '1' or '2' - defines TCT number

 Example usage :
 TCT.exe  1.txt  2.tct
 TCT.exe  1.txt
 TCT.exe

 First parameter :   input .txt  file name
 Second parameter:   output .pgr file name
 If second parameter not present  output name = input name with extension .pgr.
*/

#define PI  3.1415926535

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <time.h>
#include <bios.h>

#include <sys\timeb.h>
#include <sys\types.h>
#include <string.h>

#include "vol.h"
#include "vol.c"
/*============================================*/

        FILE *fdata,*stream_wrk,*stream_wrb;
        char fname_cd[50]={"*.tct"};
        char fname_cd1[50]={"*.tct"};
        char inp_str[200];
        char fname_tmp[100];
        double par0,par1,par2,par3,par4;
        double par10;

        char bf_q[8];

        int kv,i,i1,j,k,l,m,n;
        long int itmp;
/* ========================================================= */
char fstin(char *string,int n);
/* ========== ============================================== */
char xx,cp;
float xx_f[6];
double xx_d;

unsigned long int ee_tmp;
unsigned long int ee_nn1;
unsigned long int ii2;

unsigned int l_bytes,l_bytes1;
char *ch_ptr;
char ch_tmp;


int i1;

int n_param=5;
int n_need[40]={1,1,0,0};
char ch1,ch2;
 long int ii;
 int tmp;
 main(argc, argv, envp)
     int argc;
     char **argv;
     char **envp;
{
      float ftmp;
  struct s_TCT *tct_uk;

  unsigned long int count=0xffff;

    count=max_TCT* sizeof(TCT_sz);
    if( ( TCT_T1 = (struct s_TCT *)malloc( (size_t)count )) == NULL )
    {
      printf("\n\r!TCT1 not allocated\n");
      exit (0);
    }
    TCT_ARR[0]=&TCT_T1[0];

      if(argc>=2) strcpy(fname,argv[1]);                  /* HEX file name */
      else
         {
           goto mm_m1;
         }

      if(argc>=3) strcpy(fname_cd,argv[2]);               /* COD file name */
      if(!(strcmp(fname,"q") & strcmp(fname,"Q") )) exit(0);
mm_m1:
      if ((fdata = fopen (fname,"rt")) == NULL)
          {
           printf("\nFile %s not opened \n",fname);
begin:
           printf("\n  Perform *.TXT file with TCT");
           printf("\n  to *.TCT , *.TCB  files ");
           printf("\n  Last letter of file name is number for TCn command ");
           printf("\n  usage :   TCT.exe tct1.txt <tbl1.tct>");
           exit (0);
          }


/*--------------------------------------------------*/
      if(argc<5)
      {
         strcpy(fname_cd,fname);

            /* поиск точки или 0 */
         for(i=0;i<8;i++) if( (fname[i]=='.')||(fname[i]==0) ) break;
         if(i > 0)
          if ((fname_cd[i-1] >= '1') && (fname_cd[i-1] <= '9')  ) ch_tct= fname_cd[i-1];

            fname_cd[i]=0;
            i1=0;
            strcpy(fname_cd1,fname_cd);
            strcat(fname_cd,fnm_shrc);
            strcat(fname_cd1,fnm_shrb);
      }
/*--------------------------------------------------*/
      if(!(strcmp(fname_cd,"q") & strcmp(fname_cd,"Q") )) exit(0);

        if ((stream_wrk = fopen (fname_cd,"wt")) == NULL)
           {    printf("\nFile %s not opened \n",fname_cd);
                argc=1;
                goto begin;
           }

        if ((stream_wrb = fopen (fname_cd1,"wb")) == NULL)
           {    printf("\nFile %s not opened \n",fname_cd1);
                argc=1;
                goto begin;
           }

/* -------------------------------------------------------------- */
           printf("\nReading from the file  %s \n",fname);
           fprintf(stream_wrk,"--  TCT (%f mm) , source file %s :\n",(float)D_R,fname );


             for(ii=0;ii<max_src;ii++)
             {
   m_s1:
               par1=0;par2=0;par3=0;par4=0;
               fgets( inp_str, 199, fdata );
                if (feof(fdata))
                {
       m11:
           printf ("  %ld  storages  have been read from %s\n",ii,fname);
                   goto m1;
                }

             if( sscanf( inp_str,"%lf %lf",&par1,&par2) <= 1)
               goto m_s1;
             if(ii==0) par0=par2;
            // максимальный объем
             par10=par2;


                VV[ii] = par2;
                L1[ii] = par1;

           fprintf(stream_wrk,"-- %3ld  %10lf %10lf\n",ii+1,par1,par2);


                if(kbhit()) break;

             }
m1:
           printf ("  %ld  lines have been read from %s\n",ii,fname);
           fclose (fdata);
           n_src=ii;

           fprintf(stream_wrk,"--  %ld lines read, V0=%lf ,VM=%lf \n",ii,par0,par10);

           f_prep_vol_tcb();

           fprintf(stream_wrk,"TCT%c  %d   \n",ch_tct,n_TCT[0]);

           for(ii=0;ii< n_TCT[0];ii++)
           {

             fprintf(stream_wrk,"%10lf %10lf\n",TCT_T1[ii].level, TCT_T1[ii].volume);

           }
           fprintf(stream_wrk,"\n");
           printf ("\n  %ld  commands have been written to %s\n",ii,fname_cd);
           fclose (stream_wrk);



        if ((stream_wrk = fopen (fname_prn,"wt")) == NULL)
           {    printf("\nFile %s not opened \n",fname_prn);
                exit(0);
           }

           fprintf(stream_wrk,"   Tank%c Capacity Table, step= %.1f mm\n",ch_tct,Step_prn);
           fprintf(stream_wrk,"   N       Level,mm  Volume ,l  \n");

           for(i=1,Level=0;Level < (LVL_max+Step_prn);Level+=Step_prn,i++)
           {
             Volume=f_get_vol (0,Level);
             fprintf(stream_wrk,"%4d  %10.1f %10.1f\n",i,Level,Volume);
           }

           fclose (stream_wrk);


           l_bytes  = n_TCT[0] * 8;
           l_bytes1 = l_bytes + 4;

           ch_ptr= (char *) &TCT_T1[0];
           ee_nn1=0;

           ch_tmp='T';
           fwrite (&ch_tmp, sizeof(char),1,stream_wrb);
           ch_tmp='C';
           fwrite (&ch_tmp, sizeof(char),1,stream_wrb);
           ch_tmp=ch_tct;
           fwrite (&ch_tmp, sizeof(char),1,stream_wrb);
           ch_tmp=0xd;
           fwrite (&ch_tmp, sizeof(char),1,stream_wrb);

           fwrite (&l_bytes1, sizeof(l_bytes1),1,stream_wrb);
           ii2=sizeof(l_bytes1);

           for(ii=0;ii< n_TCT[0];ii++)
           {
             fwrite (&TCT_T1[ii], sizeof(TCT_T1[0]),1,stream_wrb);

             for(i1=0;i1< sizeof(TCT_T1[0]);i1++)
             {
               ch_tmp=*ch_ptr++;
               ee_nn1 +=((unsigned long)(ch_tmp)) & 0xff;
               ii2++;
             }

           }
           fwrite (&ee_nn1, sizeof(ee_nn1),1,stream_wrb);
           ii2+=sizeof(ee_nn1);
           fclose (stream_wrb);

           printf ("  %ld  bytes of data command  TC%c  wrote to %s\n",ii2+4,ch_tct,fname_cd1);
           printf ("\n CRC = %8lx\n",ee_nn1);

/* ============================================================ */
}
/* ========================================================= */
