
// шаг приращения по радиусу
//#define D_R (0.1)
#define D_R (1.0)

#define LVL_MAX (1450.0)
//#define LVL_MAX (1200.0)

#define proc10  (0.100)
#define proc90  (0.900)

extern int num_out1,num_out2;
#define OUT1 msk_int[num_out1]
#define OUT2 msk_int[num_out2]

#define max_TCT  3000
#define max_ord  4096

#define seg_TCT (0xb000)

#define max_nn   2

#define MM_LVL (50000)
#define cyl_nn   10

struct s_TCT
{
float level;
float volume;
};

struct s_Cyl
{
 float Radius;
 float Radius1;
 float Lgth;
 float Offs;    // расстояние от базы TCT (Level=0) до оси цилиндра
};

extern struct s_Cyl Vol1[];
extern struct s_Cyl Vol2[];

extern struct s_Cyl *Cyl_ARR[];
extern struct s_TCT *TCT_T1;
extern struct s_TCT *TCT_T2;
extern struct s_TCT *TCT_ARR[];
extern int n_TCT[];

extern struct s_TCT TCT_sz;

float f_get_vol(int ii);
double f_vol_cyl(double h, double Rr, double Ll);
float f_get_volD(int ii);

int  f_wr_tct();
void  f_rd_tct();
void set_ff_get();

extern float (*ff_get_vol[max_nn])(int ii);

extern int flag_vol[];
extern int flag_maxV[];
extern float Vol_max[];

extern float *val_Temp[];
extern float *val_Press[];
extern float D_Etan100,D_Etan,P_Etan;
extern int n_Cyl[];

void f_count_tank(int ii);

extern float Vol_10[];
extern float Vol_90[];

extern float H_Cyl_M[];

void f_prep_VM(int ii);

double f_vol_cyl(double h, double Rr, double Ll);
double f_vol_cyl12 (double Level,struct s_Cyl *Cyl_str_ptr);

