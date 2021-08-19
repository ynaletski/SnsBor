
/*--------------------------------------------------*/
#include "0set_ver.h"
/*--------------------------------------------------*/
//  these parameters define the size of velocity tables see enc_lin.c
//  float LIM_AVX[NN_AX][LIM_VM*2] ; //-- Amplitude limit in function of velocity

//  float LIM_AVX1[LIM_VM*2];        //-- Amplitude limit in function of velocity
                                     //--  during return from big errors (X_N > MAX_ERR)

//  float DEG_VX[NN_AX][LIM_VM*2]  ; //--  Dependence of angle between
                                     //--  mechanical and electrical vectors
                                     //--      in  function of velocity

  #define  DL_Time_M      250  /* mks,maximal  interrupt period   */
  #define  LIM_V_mm_sek  1500           // mm/s
//#define  LIM_V_mm_sek   400           // mm/s
/*--------------------------------------------------*/
#if(TWIN_X == 0)

#define KOL_REG   6
#define KOL_REG1  6

#else

#define KOL_REG   7
#define KOL_REG1  7

#endif

#define N_PULT    5  // quantity of axes for pult control
#define NN_AX     6

//#define   NN_INT_A  4     //  quantity of axes for 'N' command

#define NN_AX1    7
#define NN_AX2    8
#define NN_DAC    24  //  quantity of DAC channel
/*--------------------------------------------------*/
/*--------------------------------------------------*/

#if(TR_JERK == 0)
 #if(LOTIS == 0)
   #define   sz_move  512   // must be order of 2  - size of moving
                            // structure array 'move_p'
 #else
   #define   sz_move  32    // must be order of 2  - size of moving

 #endif  // (LOTIS == 0)
#endif // (TR_JERK == 0)
//-----------------
#if(TR_JERK != 0)

 #define   sz_move  256    // must be order of 2  - size of moving
                           // structure array 'move_p'
#endif // (TR_JERK != 0)
//-----------------

#define  N_int_ax 5  // 5 axis interpolator

/*--------------------------------------------------*/
#define PI  3.1415926535
#define PI_H  3.14159
#define PI_L  0.0000026535


#define k_rad (PI/180000.)
/*--------------------------------------------------*/

#define     D_DAC     16             /* DAC discrete  */
#define     MAX_DAC  32767.          /* maximum DAC   */

#define MAX_BIN 0xffffff   // maximal int value for 24 mantissa order
                           // also defined in 'traekt.c'

#define MAX_FNP 0.6        // max amplitude for fnp

#define MUL_COM_B (0.0078125) // ( 1./128.)
#define MUL_COM_Q (10.) //

#define MAX_IK 104
#define MAX_PK 80

#define   K_INC    100.0

#if(PROC_TYPE == C32)

/*---------------------------------------------*/

#define PT_PORT_IN    (*(int *)0xA10000)  // DSPf board TTL  input
#define PT_PORT_OUT   (*(int *)0xA10000)  // DSPf board TTL output

  #define BAS_DAC   0x810ff0
//#define BAS_DAC_DS 0x810fe0
  #define BAS_DAC1   0x810fd0
  #define BAS_DAC2   0x810fb0

/*

         регистры  память
                   процессов
  ET1100  0x1000 + 0x2000       0...0x2fff (байт    )   A0-A13
           0x800 + 0x1000       0...0x17ff (2 б.слов)   A0-A12
                                0x2000 ... 0x37ff
     EN=  /A15 * /A14 * A13;
*/
  #define BAS_ESCI  0x812000      /// 0x812000...0x8137ff */
//----------------------------------------

#define Analog1 *(volatile int*)0x810ff0 /* ввод готовых данных  0...3(AX1,BX1,AX2,BX2 */
#define Analog2 *(volatile int*)0x810ff1 /* ввод готовых данных  4...7(AY1,BY1   ) */

#define QCL_YWP *(volatile int*)0x810ff2 /* QYL QWL */
#define QCH_YWP *(volatile int*)0x810ff3 /* QYH QWH */
#define QCL_XZP *(volatile int*)0x810ff4 /* QXL QZL */
#define QCH_XZP *(volatile int*)0x810ff5 /* QXH QZH */

#define QCL_YWP1 *(volatile int*)0x810fd2 /* QYL QWL */
#define QCH_YWP1 *(volatile int*)0x810fd3 /* QYH QWH */
#define QCL_XZP1 *(volatile int*)0x810fd4 /* QXL QZL */
#define QCH_XZP1 *(volatile int*)0x810fd5 /* QXH QZH */

//----------------------------------------
// IN-OUT  24V

//  IO8  IO16

// IO16 board 1
#define IN_POR0_A    (0x810180)  //  S1    S2    S3
#define OUT_P0_A     (0x8101c0)  //  ON    ON    ON

// IO16 board 2
#define IN_POR1_A    (0x810182)  //  S1    S2    S3
#define OUT_P1_A     (0x8101c2)  //  OFF   ON    ON

// IO8 board 1
#define IN_POR0_A8   (0x810180)  //  S1    S2    S3
#define OUT_P0_A8    (0x8101c0)  //  ON    ON    ON

// IO8 board 2
#define IN_POR0_B8   (0x810182)  //  S1    S2    S3
#define OUT_P0_B8    (0x8101c2)  //  OFF   ON    ON

// IO8 board 3
#define IN_POR0_C8   (0x810184)  //  S1    S2    S3
#define OUT_P0_C8    (0x8101c4)  //  ON    OFF   ON

// IO8 board 4
#define IN_POR0_D8   (0x810186)  //  S1    S2    S3
#define OUT_P0_D8    (0x8101c6)  //  OFF   OFF   ON


/*---------------------------------------------*/
#define USB_ADDR      0x8107fc
#define IN_UT        (0x8107fa)  //  S1    S2
#define OUT_UT       (0x8107fa)  //  OFF   OFF
//----------------------------------------
//       UC612_b1
  #define DAC1_612   0x810810
  #define LDAC1_612   0x81081f // загрузка DAC
  #define CNT1_612   0x810800
  #define ADC1_612   0x810820
  #define RDY1_612   0x810831  // ввод D0-D5   Ready  amplif
  #define EN1_612    0x810832  // вывод D0-D5  Enable amplif
  #define V24IN1_612  0x810833  // Ввод D0-D7   24V in


  #define IM1_612    0x810834  // D0-D5 ввод индексных меток directly
  #define IMFR1_612  0x810835  // D0-D5 чтение - флаги сброса счетчиков
  #define IMFW1_612  0x810836  // D0-D5 запись - флаги сброса счетчиков

  #define VER1_612   0x81083f  // D0-D7 FPGA vers, D8-D15 PCB vers

//       UC612_b2
  #define DAC2_612   0x810910
  #define LDAC2_612   0x81091f // загрузка DAC
  #define CNT2_612   0x810900
  #define ADC2_612   0x810920
  #define RDY2_612   0x810931  // ввод D0-D5   Ready  amplif
  #define EN2_612    0x810932  // вывод D0-D5  Enable amplif
  #define V24IN2_612  0x810933  // Ввод D0-D7   24V in

  #define IM2_612    0x810934  // D0-D5 ввод индексных меток directly
  #define IMFR2_612  0x810935  // D0-D5 чтение - флаги сброса счетчиков
  #define IMFW2_612  0x810936  // D0-D5 запись - флаги сброса счетчиков

  #define VER2_612   0x81093f  // D0-D7 FPGA vers, D8-D15 PCB vers

#endif //(PROC_TYPE == C32)
/*---------------------------------------------*/

#if(PROC_TYPE == C33)

 // Адреса 0xc0xxxx - доступ с фиксированной длительностью обращения к шине
 // Адреса 0xc4xxxx - доступ с длительностью определяемой сигналом
 //                   готовности адресуемого устройства

/* TTL Input-output registers */
#define PT_PORT_IN    (*(volatile unsigned int *)0xE00000)
#define PT_PORT_OUT   (*(volatile unsigned int *)0xE80000)

  #define BAS_DAC   0xc00ff0
//#define BAS_DAC_DS 0xc00fe0
  #define BAS_DAC1   0xc00fd0
  #define BAS_DAC2   0xc00fb0

/*

         регистры  память
                   процессов
  ET1100  0x1000 + 0x2000       0...0x2fff (байт    )   A0-A13
           0x800 + 0x1000       0...0x17ff (2 б.слов)   A0-A12
                                0x2000 ... 0x37ff
     EN=  /A15 * /A14 * A13;
*/
  #define BAS_ESCI  0xc02000      /// 0xc02000...0xc037ff */
//----------------------------------------

#define Analog1 *(volatile int*)0xc00ff0 /* ввод готовых данных  0...3(AX1,BX1,AX2,BX2 */
#define Analog2 *(volatile int*)0xc00ff1 /* ввод готовых данных  4...7(AY1,BY1   ) */

#define QCL_YWP *(volatile int*)0xc00ff2 /* QYL QWL */
#define QCH_YWP *(volatile int*)0xc00ff3 /* QYH QWH */
#define QCL_XZP *(volatile int*)0xc00ff4 /* QXL QZL */
#define QCH_XZP *(volatile int*)0xc00ff5 /* QXH QZH */

#define QCL_YWP1 *(volatile int*)0xc00fd2 /* QYL QWL */
#define QCH_YWP1 *(volatile int*)0xc00fd3 /* QYH QWH */
#define QCL_XZP1 *(volatile int*)0xc00fd4 /* QXL QZL */
#define QCH_XZP1 *(volatile int*)0xc00fd5 /* QXH QZH */

//----------------------------------------
// IN-OUT  24V

//  IO8  IO16

// IO16 board 1
#define IN_POR0_A    (0xc00180)  //  S1    S2    S3
#define OUT_P0_A     (0xc001c0)  //  ON    ON    ON

// IO16 board 2
#define IN_POR1_A    (0xc00182)  //  S1    S2    S3
#define OUT_P1_A     (0xc001c2)  //  OFF   ON    ON

// IO8 board 1
#define IN_POR0_A8   (0xc00180)  //  S1    S2    S3
#define OUT_P0_A8    (0xc001c0)  //  ON    ON    ON

// IO8 board 2
#define IN_POR0_B8   (0xc00182)  //  S1    S2    S3
#define OUT_P0_B8    (0xc001c2)  //  OFF   ON    ON

// IO8 board 3
#define IN_POR0_C8   (0xc00184)  //  S1    S2    S3
#define OUT_P0_C8    (0xc001c4)  //  ON    OFF   ON

// IO8 board 4
#define IN_POR0_D8   (0xc00186)  //  S1    S2    S3
#define OUT_P0_D8    (0xc001c6)  //  OFF   OFF   ON




//----------------------------------------
//       UC612_b1
  #define DAC1_612   0xc00810
  #define LDAC1_612   0xc0081f // загрузка DAC
  #define CNT1_612   0xc00800
  #define ADC1_612   0xc00820
  #define RDY1_612   0xc00831  // ввод D0-D5   Ready  amplif
  #define EN1_612    0xc00832  // вывод D0-D5  Enable amplif
  #define V24IN1_612  0xc00833  // Ввод D0-D7   24V in


  #define IM1_612    0xc00834  // D0-D5 ввод индексных меток directly
  #define IMFR1_612  0xc00835  // D0-D5 чтение - флаги сброса счетчиков
  #define IMFW1_612  0xc00836  // D0-D5 запись - флаги сброса счетчиков

  #define VER1_612   0xc0083f  // D0-D7 FPGA vers, D8-D15 PCB vers

//       UC612_b2
  #define DAC2_612   0xc00910
  #define LDAC2_612   0xc0091f // загрузка DAC
  #define CNT2_612   0xc00900
  #define ADC2_612   0xc00920
  #define RDY2_612   0xc00931  // ввод D0-D5   Ready  amplif
  #define EN2_612    0xc00932  // вывод D0-D5  Enable amplif
  #define V24IN2_612  0xc00933  // Ввод D0-D7   24V in

  #define IM2_612    0xc00934  // D0-D5 ввод индексных меток directly
  #define IMFR2_612  0xc00935  // D0-D5 чтение - флаги сброса счетчиков
  #define IMFW2_612  0xc00936  // D0-D5 запись - флаги сброса счетчиков

  #define VER2_612   0xc0093f  // D0-D7 FPGA vers, D8-D15 PCB vers

#endif //(PROC_TYPE == C33)
/*---------------------------------------------*/
#define   BASE_D_L  BAS_DAC
#define   BASE_D_L1  BAS_DAC1
#define   e_m_s3_c  50  // jerk for calibration,lining,NPx
//----------------------------------------

/*--------------------------------------*/
#define UC48_OLD  0
#define DEP44_OLD 1
#define UC48_NEW  2
#define UC612     3
#define UC_FICT  10
#define NO_BOARD -1
/*--------------------------------------*/
//  (SER_EN)

#define SERIAL_PORT     0x808040
#define SERIAL_PORT0    *(int *)SERIAL_PORT
#define G_CTRL0         *(int *)SERIAL_PORT     // Global control
#define R_DATA0         *(int *)(SERIAL_PORT+12) // Port receive data
#define T_PORT_CTR      *(int *)(SERIAL_PORT+2) // Transmit Port Control
#define R_PORT_CTR      *(int *)(SERIAL_PORT+3) // Receive  Port Control
#define RX_TIM_CTR      *(int *)(SERIAL_PORT+4) // R/X Timer control
#define RX_TIM_PER      *(int *)(SERIAL_PORT+6) // R/X Timer period

#define D_CL 0x4
#define D_DX 0x40
#define D_FS 0x400
/*--------------------------------------*/
extern float Per_loop, Scale_LP_1, Scale_LP;
extern float Per_loop_px;
extern float Per_loop_pc;
/*--------------------------------------*/
extern int id_ok;

#define State_s  (int)Statef_s

void f_count_lp();
void f_count_mbe();
void f_psch_sd();
void f_step_d();
void f_step_d0();
void f_step_de();
void f_step_de1();
void f_psch_sd();

void f_scom_coil(int i2);
void f_scom_2ph(int i_c);
void f_scpl_2ph(int i_c);
void f_scom_3ph(int i_c);
void f_scom_3ph3(int i_c);
void f_scom_3pha(int i_c);

int f_sensXU2();
int f_sensXU2R();
extern int (*f_sensXUP)();

extern float Scale_mnm;

#define round_lp(Val)    (float) (  (int)((Val)*Scale_LP_1) & (MAX_BIN ) ) * Scale_LP
#define round_lps(Val)   (float) (( ( (( (int)( ( Val )*Scale_LP_1) & (MAX_BIN )) +mask0) & mask1) - mask0)   ) * Scale_LP
#define round_krs(Val)   (float) (( ( (( (int)( Val ) & (MAX_BIN )) +mask0) & mask1) - mask0) )

// переводит мкм в 1/16 нм
#define S_dem_ftoi(Val)   (int) ( (Val) * Scale_mnm )

#define  FICT_ER     (50000.)      // permissible error during searching
#define  FICT_ERSC   (3.)          // permissible error SCALE to max_err[]  during searching
/*---------------------------------------------*/
#define  I2TA_ER     0x0001        /* I2T   error first  group         */
#define  I2TA1_ER    0x0002        /* I2T   error second group         */
#define  NP_ERR      0x0004        /* не определен полюс */
#define  ER_ENC      0x0008        /* encoder error in TWO meass system */
#define  ER_LIN_AI   0x0008        /* превышение ускорением предельного значения  для энкодера типа 5 (E_ANAI) */
#define  SENS_ON     0x10          /* не появился датчик      */
#define  SENS_OFF    0x20          /* не исчез датчик         */
#define  REP_ON      0x40          /* нет репера              */
#define  LIM_SENS    0x100         /* наезд на концевой датчик */
#define  READY_ER    0x400         /* нет готовности привода при следящей системе  */
#define  FATAL_ER    0x0800        /* fatal error code  planar motor   */
#define  POSIT_ER    0x1000        /* ошибка позиционирования   */
#define  LIM_DIST    0x2000        /* выход за пределы допустимой зоны */
#define  ER_LIN      0x4000        /* плохие сигналы с линейки */
#define  POSIT_ER_F  0x8000        /* position error rotating close loop system planar motor */
#define  ERR_CRC_BL  0x10000       /* CRC error for command J */
#define  ERR_N_BL    0x20000       // количество команд в блоке (команда 'J') превышает допустимое значение
#define  ERR_UNKN_BL 0x40000       // неизвестная команда в блоке (команда 'J')
#define  ERR_TOUT_BL 0x80000       // таймаут при приеме команды 'J'

#define  SURF_ERR    0x0010        /* предупреждение : ошибка слежения за поверхностью > допустимой */
#define  USB_INT_ERR (0x0040)        /* ошибка интерполятора USB */
#define  SP_STATE_ER 0x800         /* ошибка состояния шпинделя */

#define  SYNTAX_ERR   0x800000  /* код синтаксической ошибки электроавтоматики */
/*--------------------------------------------------*/
// automatic errors

#define  INTR_ERR     0x400000  /* код ошибки команды "INTR" */
#define  L_DIST       0x010000  /* ошибка : перемещение за пределы зоны   */
/*--------------------------------------------------*/
// Ошибки D_T , C - электроавтоматика

extern char name_msc[];

#define NEST_ERR   0x010
#define V4_ERR     0x020
#define DE_DW_ERR  0x040
/*--------------------------------------------------*/

#define Period   2048  // SIN table period
#define COD_BACK  10000000
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#define b_adr_Hlp_fl  0x2000  /* begin address Help in Flash ,bytes         */
#define b_adr_prg_fl  0x6000  /* begin address dem. program in Flash ,bytes */
#define b_adr_cor_Surf  0x6000 /* begin address table of correction surface signal in Flash, bytes */
#define b_adr_cor     0x8000  /* begin address table of correction in Flash,bytes */
#define MAX_HLP       0xffff

#define Cor_table_Length   1000

#define Cor_Surf_Lgth   130

#pragma DATA_SECTION (Cor_surf,".ram_hl")
extern float Cor_surf[Cor_Surf_Lgth];
extern float TT_PER_1_Surf;

extern float Reper_type[];  // 0 - Not index (to nearest begin of period) ,1 - Index
extern int (*ff_sens[])(int i_c);
extern float Flag_mode_gx[];
extern float NP_c[];

struct Cor_table
{   float x_kor[Cor_table_Length];
    float y_kor[Cor_table_Length];
};

 #pragma DATA_SECTION (korr_table,".ram_hl")
extern struct Cor_table  korr_table;

//#define N_CMT     (3)  /* quantity of selfcommutation for one regulator counting */
extern int N_CMT;
extern float N_CMT_f;
#define L_HLP  (b_adr_prg_fl-b_adr_Hlp_fl - 2)    // maximal length  of  Help

extern int adr_getch_fl;
extern int adr_putch_fl;
extern int adr_getch_ram;
extern int adr_putch_ram;
extern int fl_putch_bsy;

extern int CURR_BASE;
extern float base_cod;

#define  C_BASE1  0x20000
#define  C_BASE2  0x28000
#define  C_BASE3  0x30000
#define  C_BASE4  0x38000
//I2tP
extern float tCM[]; // current temperature of motor windings (I2t protection)
extern float tC[];  // tmp variables for I2t protection
extern float C1[];  // C1  variables for I2t protection
extern float C2[];  // C2  variables for I2t protection
extern float dt_max[]; // overheating of for I2t protection  ( Tmax - Tmot )

extern float Tmax[]; // grad, motor winding maximal permissible temperarure
extern float Tmot[]; // grad,motor case temperature
extern float T1[];  // s, time for heating of winding from Tmot to Tmax with current 1.0
extern float Inom[]; // nominal value of current 0...1.0


//extern int flag_err[];

 int  getch_fl(void);
 int  putch_fl(int ii);
 int  getch_ram(void);
 void putch_ram(int ii);
 void f_rd_char();
 int  f_check_id(float n_id);
 void  f_cnt_I2T_cfg();
 void f_NO_FS(int i);
/*----------------------------------------------*/
struct PDIUSBD12_struct
{
 volatile int DAT;
 volatile int COM;
};

/*--------------------------------------------------*/
struct USB_int_str
{
  int x;
  int y;
  int c;
  int frame;
};
/*--------------------------------------------------*/
struct ADC_struct
{
 volatile int s1;
 volatile int s2;
};

extern struct dis_set ds_ti;
extern struct ADC_struct  *ADCs;
extern int MAX_V;
extern float f_res_id;

extern int n_interupt;
extern int uk_fbuf;

extern float v_fnp;
extern float fast_s_dem;
extern float *fast_s_real;

extern float *fast_al;
extern float fast_am;
extern float flag_fpt;
extern float fast_scale_m;

extern float buf_xyz[];

#define dim_xyz_c  16000  /* длина массива буфера записи */
#define MSK_RI     0x200  /* маска сброса по реперу      */

extern int n_interupt;
extern int uk_fbuf;

extern float flag_disp;
extern float k_fast;
extern float v_fnp;
extern float fast_s_dem;
extern float *fast_s_real;
extern float buf_xyz[];
extern int uk_buf;

extern float Low_X,High_X,Reg_X,Reg1_X,Step_X,Step_X_1;
extern float Low_Y,High_Y,Reg_Y,Reg1_Y,Step_Y,Step_Y_1;

#define  dim_xyz_c 16000  /* длина массива буфера записи */
#define MAX_AMPL    0x7fff

extern float max_err[];
extern float maxc_err[];

extern float cz_x[];
extern float cz_y[];
extern float cz_z[];

extern float czp_y[];
extern float czp_z[];
void f_cfg_cz();
/*------------------------------*/
extern float cz_x[];    // значение угла барабана в точке максимума биения 0...360000
extern float cz_a[];    // абсцисса вектора после пересчета из полярной системы
extern float cz_o[];    // ордината вектора после пересчета из полярной системы
extern float cz_y[];    // значение Y в точке замера биения 0...700000
extern float cz_z[];    // величина биения 0...1000 мкм
extern float k_ca,k_co; // масштабные коэффициенты по смещению центра оси (см. 'f_cfg_cz() ')
           // параметры коррекции смещения оси барабана
extern float czp_y[];      // позиция по Y
extern float czp_z[];      // величина смещения
extern float A_A,B_B,C_C;  // коэффициенты корректирующей параболы
extern float flag_czd,flag_cze,cz_rr,cz_rr_1;
extern float czx_rr_1;   // переводит линейное смещение (мкм) в 0.001 градусов барабана

extern float dxn;
extern int flag_print;
extern float fl_pr;
/*------------------------------*/
extern float pf_ds[];
extern float OldCnt_DS[];
extern float CntPer_DS[];
extern float flag_st_d;
extern void (*f_iter)();
void f_step_dir();
void f_psch_sd();
void f_otr_it();
/*------------------------------*/
extern float *p_pk[];
extern float *p_da[];
extern float *p_do[];
extern float *mas_dac[];
extern float scale_da[];
extern unsigned int msk_pk[];
extern unsigned int msk_do[];
extern unsigned int *p_ik[];
extern unsigned int msk_ik[];

extern float *pf_ik[];
extern unsigned int mskf_ik[];

extern float Beg_angle[];
extern float MotorPeriod[];
extern float Null_Period[];
extern float  Scale_m_per[];
extern float  Scale_m_per_1[];

//extern int  Flag_sys[];
extern float *s_dem;
extern float fast_phase4;

extern int Frw_inv[];
extern int Rev_inv[];
extern int Srv_On_inv[];
extern int Pole_inv[];

/*------------------------------*/

extern float *uk_zadp2[],fict_dr,*state_dr,*s_uk_dr,v_t_dr;

extern float *s_uk_dem[];
extern float s_dem_dr;
extern float n_dr_ax_t2;
void f_direct_rot();

/*------------------------------*/

extern int iv_s[];
extern int iv_rdy[];
extern int iv_m[];
extern int iv_p[];
extern int iv_q[];

/*------------------------------*/
extern float Motor_type[];
extern int mot_ph[];
extern float m_dac[3][];
extern float *DAC_cc[];
/*------------------------------*/

extern int m_beg,m_fin;    /* pointers begin/end in moving structure            */
//-------------------
extern float  sx_0,sy_0,sz_0,sc_0,sb_0; /* input values for counting moving     */
extern float  sx_0d,sy_0d,sz_0d,sc_0d,sb_0d; /* input values for counting moving     */
//-------------------
extern int Flag_arc;
extern int Flag_delay;
#pragma DATA_SECTION (move_p,".sysm")
extern struct move_tt move_p[];

#pragma DATA_SECTION (UV,".sysm")
//extern float var[]; /* глобальные  переменные */

extern int msk_pl[];
extern int msk_mn[];
extern int msk_q[];

extern int intr_bsy;
extern int flag_ea;
void interpr();
/*------------------------------*/
/*-------------------------------------------------------------------------*/

extern int uk_beg,uk_fin;
extern int uk_beg_USB,uk_fin_USB;
extern int uk_beg_USBw,uk_fin_USBw;


#define n_bufcom   260
#define Max_com_bl ((n_bufcom-3)/7)

#define wrd_w       32   /* длина команды + 1-й операнд в байтах   */


struct intr_r    /* состояние интерпретатора */
{
    int sw;                 /* переключатель состояния */
    int n_bcom;             /* указывает на  последний введенный символ в bfcom */
                            /* если intrpr.bfcom[intrpr.n_bcom]==0 - команда введена */
    int i1;                 /* индекс к bfcom начала слова      */
    int i2;                 /* индекс к bfcom конца слова       */
    int i3;                 /* индекс к bfcom начала след-го слова */
    int wdl;                /* длина введенного слова           */
    float num_var;          /* текущий номер символьной переменной */
    float num_varD;          /* текущий номер символьной переменной */
    int  num_var_i;

    char bfcom[n_bufcom];   /* буфер команды интерпретатора     */
    char wrd[wrd_w];           /* буфер слова                      */
    char com[15];           /* выполняемая команда              */

};
/*------------------------------*/
void f_pos_dep();
void f_pos_UC48();
void f_pos_UC48_h();
void f_pos_UC48_o();
void f_pos_DEP_B21();
void f_pos_UC48_B21();

void f_empty();
int fi_empty(int i);
void f_pos_DEP_B1();
void f_pos_DEP_B2();

void f_pos_UC48_B1();
void f_pos_UC48_B2();
void f_pos_UC48_B3();

void f_pos_UC612_B1();
void f_pos_UC612_B2();

void ff_empty( int i1);

void f_pos_enc(int i1);
void f_pos_enc1(int i1);
void f_pos_enc_NA(int i1);
void f_pos_enc_A(int i1);
void f_pos_enc_AI(int i1);
void f_pos_enc_AI1(int i1);
void f_pos_enc_3f(int i1);
void f_pos_enc_3f1(int i1);

void f_io8();
void f_io16();
void f_io16_8();


/*------------------------------*/
extern float vp_tek[];
/*------------------------------*/
extern struct intr_r intrpr;
extern int fl_dec_c;
extern float del_dec_c; /* время задержки до начала снижения тока , мс */
extern float t_inc_c;   /* время нарастания тока, мс  */
extern float t_dec_c;   /* время спадания тока , мс   */
extern float da_incr;  /* приращение амплитуды  за прерывание при увеличении тока */
extern float da_decr;  /* уменьшение амплитуды  за прерывание при уменьшении тока */
extern float n_intr_dm;

extern float LIMOP_PVX[];
extern float n_ax;
/*------------------------------*/
extern unsigned int *p_pl[];
extern unsigned int *p_mn[];
extern unsigned int *p_rdy[];
extern unsigned int *p_sens[];
extern float *p_q[];
/*------------------------------*/
extern int *uk_dac0[];
extern unsigned int *uk_dac4[];
extern int *uk_Analog2[];
extern float *uk_dac_c_4[];
extern float *uk_dac_c_41[];
extern int *uk_CalFlags[];
extern int *uk_CNT_STT[];

extern int msk_res[];

extern int *uk_612im[];
extern int *uk_612imr[];
extern int *uk_612imw[];
extern int msk612[];

 /*------------------------------*/
extern unsigned int *p_sw;
extern int msk_sw;
extern int iv_sw;
/*------------------------------*/
extern unsigned int UC48_TTL;
extern unsigned int UC48_TTL_t,old_UC48,cnt_uc;
extern unsigned int DSP_TTL;

void f_i2t_prot();

extern float *ss_dem[];
extern float *ss_demF[];
extern float ss_demF_1[];
extern float *ss_demd[];
extern float *ssb_dem[];
extern float fict_s[];
extern float k_it_mm;

extern float RFX,RFY;
extern float  ABS_LINH[];
extern float  V_REALX[];

extern float TG1,TG2,DTG12;
extern unsigned int int_fict;

extern float flag_rv[];
extern float tg_num[];
extern float tg_offs[];
extern float tg_norm[];
extern float *tg_val[];
extern float  Analog_1;

extern float LIM_PVV_mm[];
extern float abs_fnp;

extern float s_g_fct[];
extern float s_g_dem[];

//extern float flag_surf;
extern float s_surf;
//extern float surf_a;

extern int usb_buf0[];
extern int usb_buf1[];
extern int usb_buf1w[];
extern int usb_buf2[];

extern int usbw_buf0[];
extern int usbw_buf1[];
extern int usbw_buf2[];

extern float flag_usb,usb_mode;
extern float f_usb_setup;
extern float flag_USB_int;
extern float flag_USB_int0;

extern float Scale_mnm; // переводит мкм в 1/16 нм
extern float Scale_nm_m; // переводит 1/16 нм в мкм

extern int str_desc0[];
extern int str_desc1[];
extern int str_desc2[];
extern int usb_buf_n;

extern int i_usb;
extern int string_manuf[];
extern int string_prod[];
extern int string_serialN[];

extern volatile int fict_dd;

/*------------------------------*/
struct dis_set   /* структура для отображения-ввода значений */
{
 float *op;
 float low_l;
 float high_l;
 float div;
};
/*-------------------------------------------*/
struct dis_setD  /* структура для отображения-ввода значений удвоенной точности */
{
 float *op;
 float *opD;
 float low_l;
 float high_l;
};
/*-------------------------------------------*/

#define ScaleKR_ROT  (1./36.) // (360000./12960000.)
      // переводит 0.1 '' в  1/1000 градуса

#define ScaleKR_ROT1  36    // (12960000./360000.)
      // переводит 1/1000 градуса в 0.1 ''

#define USB_bufi_l   64
#define USB_bufi_li  63       //   USB_bufi_l - 1
#define N_usb_i      32


extern int S_int0[]; // младшее слово - 3 младших байта , вес 1/16 нм
extern int S_int1[]; // старшее слово -вес 1/16 * 2**24
extern int  frame_n;
extern float usb_p1,usb_p2,usb_p3;

extern int flag_whit0;
extern int flag_whit1;
extern int flag_whit2;
/*-------------------------------------------*/
extern char cb_RS232[];
extern char cb_USB[]  ;
extern char cb_EA[]   ;
extern int RS_n_bcom  ;
extern int USB_n_bcom ;
extern int EA_n_bcom  ;
extern float ax_inf[];

#define blc_lgth  16
extern int Int_buf[3][blc_lgth];
extern int int_frame[blc_lgth];

extern int blc_fin;
extern int blc_n;

extern int  usbi_beg; // begin circlet interpolator buffer
extern int  usbi_fin; // end circlet interpolator buffer
extern int  frame_n;

extern struct USB_int_str USB_int[];

extern float s_dem1[];
extern float s_dem2[];

extern int SW_code,SW_code1;
/*-------------------------------------------*/

int f_sns_index(int i);
int f_sns_not_ind(int i);
int f_sns_dep(int i);
int f_sns_st_mot(int i);
int f_sensXU2();
void f_dtg_cnt();
void f_init_sw(float f_sw);

int close_usb();
int usbhit0();
int usbhit1();
int usbhit2();
int usb_read0 ( int *uk_buf,int n_byte);
int usb_read1 ( int *uk_buf,int n_byte);
int usb_read2 ( int *uk_buf,int n_byte);
void f_usb_state();
void f_USB_int();   // First pass USB-int .
void f_USB_int_m(); // Main  pass USB-int .

void init_usb();
int present_usb();
void f_usb();

int f_hall_loop();
int f_42();  // Direct force - torque performing
int f_44();  // Direct force 4 module groups performing

void f_clear_buf();
/*------------------------------*/
extern float  V1_mm_sek;
extern float  V2_mm_sek;
extern float  MAX_A1;
extern float  LIM1;

extern float  MAX_ERRF;
extern float  MAX_ERRFF;
extern float  MAX_MOM;
extern float  MAX_MOM1;
extern float  max_momt;
extern float  MOM_OTP;

extern float axis_dir[];    /* direction of axes  */

extern float FYR ; /* 0 - X modules rotation,1 - Y modules rotation for lining */
extern float Ly;   /* 0 - Y modules don't make selfcommutation in function of angle */
extern float fct_ysc;
extern float Flag_Y_rot;
extern float RFY_a;

extern int flag_fctr;
extern int flag_ft_ctr;
extern int flag_ff_ctr;

extern float m_adc_df;
extern float Null_df[];
extern float Scale_df[];

extern float kf_fctr[];
extern float KF1_fctr[];

extern float *P_Analog_df[];
//------------------

extern float Per_dft,Per_dff;

extern float m_adc_dft[];
extern float Null_dft[];
extern float Scale_dft[];

extern float kf_ft_ctr[];

extern float KF1_ft_ctr[];

extern float *P_Analog_dft[];
//------------------

extern float m_adc_dff[];
extern float Null_dff[];
extern float Scale_dff[];

extern float kf_ff_ctr[];
extern float KF1_ff_ctr[];

extern float *P_Analog_dff[];

/*------------------------------*/

extern int N_CMT;
extern float N_CMT_f;
int f_warn(int i);
extern float flag_rest; //-- restart flag , 1 - restart---

extern float  g1[],g2[],g3[],g4[],g5[],g6[],g7[];
extern float  f1[],f2[],f3[],f4[],f5[],f6[],f7[];
extern float  cg1[],cg2[],cg3[],cg4[],cg5[],cg6[],cg7[];
extern float  cf1[],cf2[],cf3[],cf4[],cf5[],cf6[],cf7[];
extern float  *Statef_s;
extern float  FFFF_C[];

extern float Null_L[6];
extern float Norm_L[6];

extern float *a_zd_1;

extern float LIN_MAX;
extern float N_LIN;
extern float N_LIN1;
extern float Ampl_y_lin;

extern unsigned int n_iter;         // текущая для хранения количества прерываний

#define REP_FND   10  /* репер найден успешно */
#define COR_ON    20  /* включить коррекцию  */
#define COR_OFF   22  /* выключить коррекцию */
#define WAIT_COR  24  /* ожидание завершения включения коррекций других осей */
/*--------------------------------------------------*/
#define  TIME_FX0   100   // ms, LED blinking period for 'No move' mode
#define  TIME_FX1   300   // ms, LED blinking period for 'Close loop' mode
#define  TIME_FX2   600   // ms, LED blinking period for 'Open loop ' mode
/*--------------------------------------------------*/
void f_sd_end();
void f_sd_beg();
int f_per_rot(float angle);
int f_move( float ssx , float ssy , float ssz );
int f_move_a(int i,float ssx);
void np_err_print(int i,int i1);
int  f_prnt_NP(int i1);

/*------------------------------*/
#if(LOTIS != 0)

extern float per_lt;
extern float NN_SC;

#define  PARAM_ERR   0x400000  /* код ошибки при расчете  f_inp_lt() */
#define  PARAM1_ERR   0x200000  /* код ошибки при расчете  f_inp_lt() */

void f_tim1_set();
void f_tim1_set0();

int f_lt_count();

//----------------------------------

#define sz_move_lt 256
#define sz_move_ltp 512
/* ---------------------------------------  */
// структура предварительного накопления точек
struct move_ltsp    // parameters of movement for flash tracking
{
  float x_b;
  float y_b;
  float z_b;

  float dx;
  float dy;
  float dz;

  float AA;
  float TTT;        // предварительное время перемещения
};
/* ---------------------------------------  */
struct move_lts     // parameters of movement for flash tracking
{
    float tim_lot; // источник значения длительности отработки сегмента
    float tr3  ;   // конец перемещения

    float x_b;     // coordinates of movement begining
    float k_x;     // 1000000. или -1000000.
    float v_begx;     // begin velocities
    float tr1_x;   // начало полки скорости
    float tr2_x;   // конец полки скорости
    float A01_x;
    float A02_x;
    float Sap_x;
    float Svp_x;
    float Vm_x ;

    float v_begx1;     // begin velocities
    float v_begy1;     // begin velocities
    float v_begz1;     // begin velocities

    float y_b;
    float k_y;
    float v_begy;     //
    float tr1_y;   // начало полки скорости
    float tr2_y;   // конец полки скорости
    float A01_y;
    float A02_y;
    float Sap_y;
    float Svp_y;
    float Vm_y;

    float z_b;
    float k_z;
    float v_begz;     //
    float tr1_z;   // начало полки скорости
    float tr2_z;   // конец полки скорости
    float A01_z;
    float A02_z;
    float Sap_z;
    float Svp_z;
    float Vm_z;

    float tr_flash; // выдать разрешение на электрооптику
};
/* ---------------------------------------  */

extern struct move_ltsp  move_ltp[];
extern int ltp_beg,ltp_beg_p1,ltp_beg_p2,ltp_fin,ltp_fin_p1;    /* pointers begin/end in moving_lt structure  */
extern struct move_ltsp *mltp_beg,*mltp_beg_p1,*mltp_beg_p2,*mltp_fin,*mltp_fin_p1;

extern struct move_lts move_lt[sz_move_lt];
extern struct move_lts *mlt_beg,*mlt_fin,*mlt_fin_p1;

extern int lt_beg,lt_fin,lt_fin_p1; /* pointers begin/end in moving_lt structure  */
extern int flag_first_lt;

extern int flag_stop_lt,flag_ost_lt;
void f_otr_lt();
extern float flag_cmp;

extern int fnn_tim1;
extern int fnn_tim2;
extern int nn_tim1;
extern float fdp_par1,fdp_par2,fdp_par3;
extern float delay_ff;

extern struct dis_set  ds_fd[];
extern struct dis_set  ds_fdt[];
extern struct dis_set  ds_fdp[];

extern float   Per_flash ;   // сек, период вспышек лазера
extern float   Length_pulse; // сек, длительность импульса разрешения
extern float   Pulse_pre;    // сек, опережение  импульса разрешения относительно накачки

extern int flash_num;

int f_inp_q();
int f_tst_q();

#endif
/*------------------------------*/
int kbhit_RS(void);
int kbhit_USB(void);
int getch_RS(void);
int getch_USB(void);
extern float n_line;
extern float pult_type;
/*------------------------------*/
int f_move_a1(int i,float ssx);
extern int flag_xy;   // flag make searching Y after X
extern int  flag_usbH;

extern float scale_hall_d;
extern float scale_enc_d;
extern float hall_enc,enc_hall;
extern float flag_md,flag_md1;

extern float  dist_hall, dist_enc,change_xy;

extern float wp_xm[],wp_xp[],wp_rm[],wp_rp[];
//-------------------------------------------------

#define N_BT   6   /* для TTn до 6 порядка включительно */
#define N_BTX  9   /*   (N_BT/2) *3   для TTn до 6 порядка включительно */

#define LowPass  0
#define HighPass 1
#define Notch    3

struct bt_filtr
{
  int bt_n;          /* порядок фильтра   */
  float yy;          /* выходное значение */
  float TTn[N_BT ];  /* хранение предыдущих состояний фильтра */
  float kxn[N_BTX];  /* коэффициенты уравнений в конечных разностях*/
  float kyn[N_BTX];  /* коэффициенты уравнений в конечных разностях*/
};

extern struct bt_filtr bt_xn[], bt_v[], bt_a[];
extern float bt_order[];
extern float bt_freq[];
extern float btv_order[];
extern float btv_freq[],flag_btv_md[];
extern float bta_order[];
extern float bta_freq[];
extern float v_t;

extern float Contr_NN; // Controller number (in multicontroller system )

int count_bt( int BT_type ,float bt_n,float bt_freq, struct bt_filtr *BT  );
int f_bt_count(float input, struct bt_filtr *bt);
//-------------------------------------------------
extern  int Max_Cnt_Fst_i[NN_AX];      // то же , в количестве прерываний
extern  int cnt_p_fst[NN_AX];          // текущий счетчик антидребезга
extern float per; // for tr_j.c
//-------------------------------------------------
void f_ns_count();
void trace();


float f_vel();

extern float fsc_v;
extern float k_dacv;
//-------------------------------------------------
extern float Pos_s_type;
extern float type_amp[];
extern float type_enc[];
//-------------------------------------------------

extern float *Position[];
extern float XN_2V[];

extern float k_p[];
extern float k_i[];
extern float k_d[];
extern float kat[];
extern float kat_[];
extern float k_f[];

extern float k_fa[];
extern float kv_p[];
extern float kv_i[];
extern float kv_d[];
extern float k_fv[];

extern float sp_dem;
extern float dacv_mul;
extern float dac_ext[];

extern unsigned int TTL_UT_IN;

//-------------------------------------------------
// for SPR LINK
extern int uk_beg_SPR,uk_fin_SPR;
extern int uk_beg_SPRw,uk_fin_SPRw;

extern int SPR_n_bcom;
extern int flag_spr;
extern int flag_spuc;
extern int flag_sprc;

extern int n_dst_a;
extern int flag_wt_e;

extern int sp_com_n;

int putch_SPR(int ch);
int f_ch_c_SPRw(int uk);
int f_ch_c_SPR(int uk);
//-------------------------------------------------
void f_count_ssc();

extern   float *SBX_210;
extern   float scale_slip[];
extern   float v_nom_a[];    // синхронная частота асинхронного мотора
extern   float v_rot_a[];    // частота вращения ротора

/*
extern   float v_slip[];
extern   float I_a_nom[];
extern   float Flux[];
extern   float Flux_c[];
extern   float slip[];
*/

//-------------------------------------------------
//#define   EHL_ALL   10000

//-------------------------------------------------
extern float T_INCR;
extern float v_mm_s_c;   /* скорость калибровки ADC , мм/с */
extern float a_m_s2_c;  /* Ускорение калибровки датчиков Холла,м/с2 */
extern float ADC[];  /* Усредненные значения по 10 измерениям     */
extern float pf[];   /* position factors for position sensors */

extern float *u_tmp1,*u_tmp2,ii_tmp;
extern float kff_ln,val_rot_ln,ftmp_ln,ftmp2_ln,kk_p_ln;

float f_mm_it (float v_mm);
float f_ms2_it (float a_ms2);
void f_rest_dacp();
//-------------------------------------------------
extern int fp_ip1,fp_ip2;
extern int fp_i_ret;
extern float fp_ftmp,fp_ftmp1,fp_ftmp4;
extern int Flag_axp[];
extern int Flag_axp_f[]; // Queue of Fx1 processes X,Y ...
extern int Flag_axp_g[]; // Queue of Gx  processes X,Y ...
extern int fp_sw_ret;

extern int letter[];

extern int fp_i;

extern int i_sign;

extern float  MAX_A11[];

//-------------------------------------------------
extern float CntPerMKM[], PerF, ScaleCntMKM;
extern float MotorPeriod[];
extern int CntDiskr[],OldCnt[];
extern float n_f_dir[];
extern float m_adc[];
extern float DF[];
extern float MIN_AB[];
extern float MIN_ABR[];
extern int fp_flag_sign;
extern float n_f_div[];
extern float n_f_per[];
extern float n_f_per_b[];

extern float flag_fast[];
extern float fast_ampl[];
extern float n_f_offs[];
extern float f_ampl_min[];
extern float f_ampl_def[];
extern float flag_ampl;
extern float fast_amp;
//extern int State_xp[];
extern int Flag_axp_p[];

extern float v_mms_t,a_ms2_t,e_ms3_t;
//-------------------------------------------------
extern float stg_num;
extern float *stg_val;
//-------------------------------------------------
// Polus defining

#define  D_FRICT (0.15)
#define Min_fnp_ampl 0.15  // mkm , минимальная амплитуда отклика при
                           // быстром определении полюса , см f_fnp().
//-------------------------------------------------

void f_stop_fx( int nn_ax);
int f_run_fnp( int nn_ax );
void f_err_ea(int num_err,int ii);
void empt_a();

int f_wih(int ii,float delay);
int f_wil(int ii,float delay);
int f_wv(float vvar,float val, float msk,float delay );
void f_eh();
int f_pch(int ii,float delay);
int f_pcl(int ii,float delay);
int f_pv(float vvar,float val, float msk,float delay );
int f_pcv(float vvar,float val, float msk,float delay );
int f_pr(float xx,float yy,float zz,float cc,float bb);


#if(D_T != 0)
#define N_aut 6   // number of fast  f_autom
#endif

#if(TREATM != 0)
#define N_aut 6   // number of fast  f_autom
#endif

#if((PLANAR != 0) && (D_T == 0))
#define N_aut 6   // number of fast  f_autom
#endif

void f_autom();
extern char autom_nm[][15];
extern int au_sw[];
extern void (*ff_automp[])();
extern void (*ff_autom[])(int i_c);

#if(D_T != 0)
#include  "dt.h"
#endif // (D_T != 0)

extern unsigned int tt0,tt1,tt2,tt3;
extern float fl_trace_loop,fl_trace;

//-------------------------
extern  void (*f_im_set[6])(int i_r);
extern  int  (*f_im_tst[6])(int i_r);
extern  int  (*f_im_dix[6])(int i_r);
extern  void (*f_im_res[6])(int i_r);
//-------------------------
 void f_48_im_set(int i_r);
 int  f_48_im_tst(int i_r);
 void f_48_im_res(int i_r);
//-------------------------
 void f_481_im_set(int i);
 int  f_481_im_tst(int i);
 void f_481_im_res(int i);
//-------------------------
 void f_dep_im_set(int i_r);
 int  f_dep_im_tst(int i_r);
 int  f_dep_dix(int i);
 void f_dep_im_res(int i_r);
//-------------------------
 void f_dep1_im_set(int i);
 int  f_dep1_im_tst(int i);
 int  f_dep1_dix(int i);
 void f_dep1_im_res(int i);
//-------------------------
 void f_nul_im_set(int i_r);
 int  f_nul_im_tst(int i_r);
 int  f_nul_dix(int i_r);
 void f_nul_im_res(int i_r);
//-------------------------
void f_612_im_set(int i);
int  f_612_im_tst(int i);
int  f_612_dix(int i)   ;
void f_612_im_res(int i);
//-------------------------
 int  f_sns_im_tst(int i_r);
//-------------------------
void  f_ibd_count();
int f_get_ibd(float n_k);
void f_out1_8();
void f_out9_16();
void f_out13_20();
void f_out1_12();
void f_out17_24();
void f_out13_24();
//-------------------------
void f_cfg_ta();

 #pragma DATA_SECTION (deq_pnt,".ram_hl")
 #pragma DATA_SECTION (buf_deq,".ram_hl")
extern int deq_pnt;
extern char buf_deq[];
//-------------------------
extern void (*ff_scom_pl)();
void f_pl_md1();
void f_pl_md2();
//-------------------------
extern float  cor_fct[];
extern float  cori_val[];  // correction of axes influencies

void  f_cori_init(void);
float f_cori_val(void);
//-------------------------
extern float  io_p0,io_p1,io_p2,io_p3;
extern float cala_ncf;
extern int cala_nc;
extern int cala_ncn;

extern unsigned int TTL_UC612;
extern unsigned int V24_UC612;
extern int II0;
extern float n_ver;


#define PL_SP   12.
#define ASYN_M  13.


extern int scan_p_in[];
extern int in_pt_big;
extern int *scan_p;
extern unsigned int PT_PORT_OUT_co;
extern int in_pt_big;
extern int stab_pult,old_pult ,  cnt_pult;
extern int l_cnt_p_i,l_cnt_io_i;

extern float arr_VX[];
extern float arr_VY[];

extern int fl_ea_io;
extern float dd_zz;
extern int dd_ii;

extern float *scv_i;
extern float *sca_i;
extern float *ind_start ;
extern float *ind_start1;
extern float *s_end_i   ;
extern float *s_end     ;
extern float *ss_dem_i  ;
extern float *fl_ind_ax;
extern float *vi_tek;
extern float inv_q[];

//-------------------------
int f_init_ind();
void f_ind_it();
int f_prep_ind( float dpos, int n_ax );
int f_ind_start(int n_ax, int mode);
//-------------------------
 void add12(float ddd1, float ddd2,float *dd_res);
 void dbl_add(float *x_dbl, float *y_dbl,float *res_dbl);
 void dbl_mul(float *x_dbl, float *y_dbl,float *res_dbl);
 void dbl_div(float *x_dbl, float *y_dbl,float *res_dbl);
 int get_sp(void);
 int get_ie(void);
 int get_st(void);
 int get_if(void);
 void print_dbl(float *x_dbl);
//-------------------------
extern  char r_buf[],BufferPrintf[];
extern volatile float sssi;
extern volatile float dlt_ssi[];
extern float *uk_zadp1[];
extern float *uk_zadp1d[];

extern float fl_err;
extern float fl_errn;
extern float fl_errn1;
//-------------------------
extern float *s_uk0;
extern float *s_uk1;
extern float *s_uk2;

extern float ftmp_fin[];
extern float *s_dem_buf;
extern int  fl_new_m;
extern struct move_tt *mm_beg,*mm_fin,*mm_fin_m1,*mm_fin_m2,*mm_fin_p1,*mm_fin_p2,*mm_beg_m1;
extern float v_tek_m_i;

extern float scvi[];
extern float scai[];
extern float fl_ind_axt[];

void f_v_az_i();
void f_clr_ind_v(int i1,float sss);

extern float *FNP_OFFS;
extern int USB_SW;
extern int flag_SC;
extern unsigned int USB_tmp1;
//-------------------------
/*------------------------------*/
// Z_Force

#if(Z_Force != 0)

extern float *vpv_osc;
extern float *dpff_xn;
extern float *dpff_vn;
extern float *dpff_yn;

extern float ss_osc;
extern float val_osc;

#define vv_osc (*vpv_osc )
#define dff_xn (*dpff_xn )
#define dff_vn (*dpff_vn )
#define dff_yn (*dpff_yn )

#endif //(Z_Force != 0)
/*------------------------------*/
void f_corXY_init(void);
float f_cori_val_XY(int i);

void f_cor_xy_NO_FS(int i);
void f_cor_xy_ON(int mode_on,int n_ax);

//-----------------------
struct s_cor_xy
{
float BegCor_xy;
float TT_PER_1_x;
float TT_PER_1_y;
float lgth_x;
int   i_lgth_x;
int   lgth_xi;
float lgth_y;
int   lgth_yi;
float cor_fct;
int   i_lgth_y2;
int n_ax1;
int n_ax2;
float *s_dem1;
float *s_dem2;
float *s_min1;
float *s_min2;
int lgth_flash;
};
//-----------------------
extern struct s_cor_xy  cor_xy[];
extern float *s_dem_cor;
extern float Statef_s_sv[];
extern float arr_cor_fl[NN_AX1][n_cor_xy_ax];

#define LNAD_SC    (1.6666666)  // permissible scale for analog signal decrease
                          // for alignment of planar motor
extern float fff_ampl[],ADC_[];
extern float Nul[],N_a[],t_int,v_max,a_zad,t_cb,k_t_mks;
extern unsigned int tmp_ln,tmp1_ln,tmp2_ln;
extern int Fl_get_data;
extern float *P_ADC[];
void begin(void);
void f_init_mod( void);
void f_scom_2phg(int i_c);
void f_cfg_gear();

void f_clr_bt(struct bt_filtr *uk_bt);

extern float Old_pos_g[];
extern float Add_gear[];
extern float Num[];
extern float Denum[];
extern float Rest[];
extern float Denum_1[];

extern float sm_cos;
extern float cos_err[];
float sin_x( float xx,int i_k );
float cos_x( float xx,int i_k );

extern unsigned int compare;

extern float kss_ms;
extern float ep2_pol;
void f_usb_sh();
void putch_usb1(int ch);
//----------------------------------------
struct com_jj     // parameters of movement
{
    int com;     // command code and flags
    unsigned int par[6];
};

struct com_jjf    // parameters of movement
{
    int com;     // command code and flags
    float par[6];
};
//----------------------------------------
extern struct com_jj *J_COM_RS;
extern struct com_jj *J_COM_USB;
extern struct com_jjf *J_COM;

extern int *com_num_RS;
extern int *com_num_USB;
extern int *com_num;

extern int *cc_RS;
extern int *cc_USB;
extern int com_l_RS;
extern int C_Sum_RS;
extern int switch_RS;
extern int msk[];
extern int  fl_new_m;    // flag to interrupt -> new movement have counted and
extern float acs_cmp1;
//----------------------------------------
extern int flag_b_USB;
extern int nn_b_USB;
extern int switch_USB;
extern int flag_btout_USB;
extern int f_dd11();
extern int J_com_n,J_com_c, J_com_cc;

void f_Chg_laser_mode();

extern float Flag_tw;
extern float Flag_tws;
extern float Y_lgth;
extern float dLY;
extern float LY_2;
extern float flag_X1p;

extern float Y_lgth_Lx_1;

void  f_init_reg_tw(float fl_tw );
void f_init_twl();

float f_twin_xp();
float f_twin_xm();

void f_sens_pal_xyzc(int iii);
void f_sens_pal_x();

extern void (*ff_automl)();
extern float delay_time;
extern float dim_xyz;


extern float press_num;
extern float *press_val;
extern float press_offs;
extern float press_norm;

extern float Flag_dx1,Flag_dxtw;


/* ---------------------------------------  */
#if (Loop_a == 0)

struct move_tt     // parameters of movement
{
    int cod_c;     // command code  1 - line; 2 - arc; 3 - conjugation zone
                   // 4 - DELAY, 5 - Put Number, 6 - Wait Number ,7 Put Chanell

    float x_b;     // coordinates of movement begining
    float y_b;
    float z_b;
    float c_b;
    float b_b;






    float k_x;     // coordinates of unit vector          for cod_c==1
    float k_y;     // coordinates of unit vector changing for cod_c==3
    float k_z;     // coordinates of circle centre  X,Y, value of
    float k_c;     // movement by Z for  cod_c==2
    float k_b;     // movement by Z for  cod_c==2






    float s_p;     // length of movement for cod_c == 1 , 3
                   // 2L for cod_c==3

    float s_p_c;

    float v_max ;   // trajectory velocity  (nominal)
    float v_maxl;  // phisycal maximal velocity
    float v_lim ;   // maximal limit of velocity in the end of segment
    float fn_nal;  // functional

    float a_zad;   // trajectory acceleration
    float *p_pcc;    // pointer to discrete output
    int and_msk;   // and mask for output to discrete output
    int or_msk ;    // or mask for output to discrete output
};
#else    // (Loop_a == 0)
struct move_tt     // parameters of movement
{
    int cod_c;     // command code  1 - line; 2 - arc; 3 - conjugation zone
                   // 4 - DELAY, 5 - Put Number, 6 - Wait Number ,7 Put Chanell

    float x_b;     // coordinates of movement begining
    float y_b;
    float z_b;
    float c_b;
    float b_b;
    float a_b;






    float k_x;     // coordinates of unit vector          for cod_c==1
    float k_y;     // coordinates of unit vector changing for cod_c==3
    float k_z;     // coordinates of circle centre  X,Y, value of
    float k_c;     // movement by Z for  cod_c==2
    float k_b;     // movement by Z for  cod_c==2
    float k_a;     // movement by Z for  cod_c==2


    float s_p;     // length of movement for cod_c == 1 , 3
                   // 2L for cod_c==3

    float s_p_c;

    float v_max ;   // trajectory velocity  (nominal)
    float v_maxl;  // phisycal maximal velocity
    float v_lim ;   // maximal limit of velocity in the end of segment
    float fn_nal;  // functional

    float a_zad;   // trajectory acceleration
    float *p_pcc;    // pointer to discrete output
    int and_msk;   // and mask for output to discrete output
    int or_msk ;    // or mask for output to discrete output
};

#endif   // (Loop_a == 0)
/* ---------------------------------------  */

#define  v_smema  var[UT_OUT]

