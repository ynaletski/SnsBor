

//#define proc10  (0.100)
//#define proc90  (0.900)

#define max_nn   9

extern float proc10;
extern float proc90;

extern float prc10[max_nn];
extern float prc90[max_nn];


extern int num_out1,num_out2;
#define OUT1 msk_int[num_out1]
#define OUT2 msk_int[num_out2]

#define max_TCT  1000 //2000
#define max_ord  2048

#define seg_TCT (0xb000)
#define seg_TCT2 (0xC000)


#define MM_LVL (50000)
#define cyl_nn   10

struct s_TCT
{
float level;
float volume;
};

extern float H_Cyl_M[];
extern struct s_TCT *TCT_T1;
extern struct s_TCT *TCT_T2;
extern struct s_TCT *TCT_T3;
extern struct s_TCT *TCT_T4;
extern struct s_TCT *TCT_T5;
extern struct s_TCT *TCT_T6;
extern struct s_TCT *TCT_T7;
extern struct s_TCT *TCT_T8;
extern struct s_TCT *TCT_T9;


extern struct s_TCT *TCT_ARR[];
extern int n_TCT[];

extern struct s_TCT TCT_sz;

float f_get_vol(int ii,float Level);
double f_vol_cyl(double h, double Rr, double Ll);
float f_get_volD(int ii);

int  f_wr_tct();
void  f_rd_tct();

extern int flag_vol[];
extern int flag_maxV[];
extern float Vol_max[];

extern float *val_Temp[];
extern float *val_Press[];
extern float D_Etan100,D_Etan,P_Etan;
extern int n_Cyl[];

void f_count_tank(int ii);

void  f_zer_tct();

extern float Vol_10[];
extern float Vol_90[];


void f_prep_VM(int ii);

double f_vol_cyl(double h, double Rr, double Ll);

extern float alpha_LVL[]  ;
extern float alpha_Tank[] ;
extern float T0_LVL[] ;
extern float T0_Tank[];

extern float lvl_rab;

extern int Fl_ch_mm[max_nn];

extern unsigned int ctr_msk[];

