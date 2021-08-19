
#define max_MVD 4

#define F_Vol       1
#define F_Vol_Dens  2
#define F_Mass      3

float f_get_float( char *buf, int cod);


#define ToutAnsMVD_rtu 100

#define MVD_max_out_err 5
#define MVD_max_inp_err 5
#define MVD_max_t_err 3

#define MVD_RD_I   1
#define MVD_RD_L   2
#define MVD_RD_F   3
#define MVD_RD_S   4
#define MVD_RD_C   5
#define MVD_RD_P   8
#define MVD_WR_I   16
#define MVD_WR_L   17
#define MVD_WR_F   18
#define MVD_WR_C   19


#define n_coil_b 8
#define n_char_b 50

struct s_MVD_dev
{
unsigned int r_addr; // address of MODBUS register
int n;            // number of data - for read coils and ascii
int type;         // data type
int i;            // int,coil value for write
int exc;          // exception
long int  l;      // long
float f;          // float

float FlowM;          // float
float FlowV;          // float
float MassI;          // float
float MassT;          // float
float VolI;          // float
float VolT;          // float
float VolTd;          // float
float Temp;          // float
float TempR;         // float
float Dens;          // float
float Press;         // float
float Compos;       // float
float PressG;       // float
float PressB;       // float
float PressD;       // float
float PressGN;      // float
float Volt_A;        // float
float Volt_DC;       // float
float DensG;       // float

int reg_err;
};

extern struct s_MVD_dev s_MVD[];

float f_get_composition(float Dens,float Temp);
float f_get_Dns(float Compos,float Temp);
float f_get_dens();
void f_get_dens_c();
unsigned int f_get_int( char *buf);

extern float propan[];
extern float butan[];

extern float PropanP[];
extern float ButanP[];


extern float PermPlus;
extern float PermMinus;
extern float Temperature;
extern float Temperature1;
extern float Density;
extern float Composition;

extern float t20c;
extern float v_Proc;
extern float HTemp,HDens,HCompos;

extern int ModeFill;
extern int igun_1;

extern int flag_flch;

extern long int beg_flch;

extern int   time_flch;
extern float val_flch2;
extern float val_flch;
extern float val0_flch;

extern long int time_beg_fill;
extern int TimeBegFlch;

extern int MVD_t_rslt[];
void f_get_pressDT(float Dens,float Temp);

extern int ModeReduce;

extern int   ModeReduce   ;

long int f_get_long( char *buf);
int f_get_intS( char *buf);
void f_set_float( char *buf,float value, int cod);
void f_set_long( char *buf,long int value);
void f_set_int( char *buf, int value);


extern float K_mul_F;
extern float T_Ref;
extern float D_ref;
extern float V_ref;
extern int flag_AD;

extern float Dens_AD,Temp_AD;
extern float Dens_a,Temp_a; // result

