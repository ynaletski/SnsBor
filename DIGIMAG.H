
struct s_DGM_dev
{
float Prod_lvl; // 0.1 mm
float offset;
float scale;
float Wat_lvl;  // 0.1mm
float Temp;     // 0.1 C
float VolLiq;
float VolGas;
float MassLiq;
float MassGas;
float Mass;
float DensLiq;
float DensGas;
float DensReal;
float TempReal;
};

#define max_DGM  3
#define DGM_max_inp_err  3

extern struct s_icp_dev DGM[];
extern struct COM_rqst DGM_Rqst[];
void f_DGM(int ii);
void f_DGM_rd(int ii);
void f_DGM_Flt(int ii);
int f_check_crc_DGM(char *buf,int ii);

extern struct s_DGM_dev s_DGM[];

extern long int DGM_ADDR[];

extern int State_dgm[];

extern int fl_inv_out;
