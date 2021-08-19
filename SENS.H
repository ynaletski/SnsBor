
struct s_PMP_dev
{
float Prod_lvl; // 0.1 mm
float offset;
float scale;
float Wat_lvl;  // 0.1mm
float Temp;     // 0.1 C
float VolLiq;
float VolWat;
float VolDif;
float MassLiq;
float Mass;
float DensLiq;
float DensGas;
float DensReal;
float TempReal;
};

#define max_PMP  9 //16 moe
#define PMP_max_inp_err  5

extern struct s_icp_dev PMP[];
extern struct COM_rqst PMP_Rqst[];
void f_PMP(int ii);
void f_PMP_rd(int ii);
void f_PMP_Flt(int ii);

extern struct s_PMP_dev s_PMP[];

extern long int PMP_ADDR[];

extern int State_PMP[];

extern int fl_inv_out;

int f_check_crc_PMP(char *buf,int ii);

int f_prepare_PMP(char *Cmd,int *length);

int ffgets_com_PMP( int ii);

float f_get_float_PMP(unsigned char *buf);

long int f_get_long_PMP(unsigned char *buf);

extern int Tank_nn;

extern long int time_bot;

extern long int Bot_time;

extern float Lvl_offs[max_PMP];

extern unsigned int PMP_inp_err[];

