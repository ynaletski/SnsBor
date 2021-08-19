
#define Slv_deb   0
#define Slv_deb_rtu  0

#define MSK_OK    6
#define Reg_Cmd   7
#define CMD_BAD   (-1)

#define i_cfm    (19)
#define Strt_cmd  (0x55)

#define CMD_ERR (0x8000)
#define CMD_DIS_PMP (0x100)
#define CMD_DIS_RCV (0x1000)
#define Pump_act     1
#define CMD_CONT     0x4000

#define RCV_act     0x10
#define RCV_flch    0x20
#define RCV_prep    0x40
#define RCV_fin     0x80

#define DEN_act     0x100
#define DEN_fin     0x800

#define Cmd_brk    0
#define Cmd_err   -1
#define Cmd_rcv    1
#define Beg_cnt    2
#define Stp_cnt    5
#define End_DLV    10
#define n_buf_RTU_s 200

 // функция не распознана
#define  RTU_err_fn  1

 // недопустимый адрес
#define  RTU_Addr  2

// недопустимые данные
#define  RTU_Val   3

// ошибка в Slave
#define  RTU_Slv_err  4


#define Slv_b_lgth  2048
#define Slv_b_lgth_1 2047

#define M_RD   0
#define M_WR   0x80
#define mess_lgth 26

#define i_fl_cd  9   // индекс в Slv_int ячейки с кодом кодировки float
                     // аналог регистра 521 в MVD

#define  Dens_min  400
#define  Dens_max  700
#define  Temp_min  (-40)
#define  Temp_max  (50)

//#define cod_float (Slv_int[i_fl_cd] )
//#define n_sl_int  20
//#define n_sl_flt  20

#define AddrLowInt 0
#define AddrHighInt (AddrLowInt+n_sl_int-1)

#define AddrLowFloat  1000
#define AddrHighFloat (AddrLowFloat + n_sl_flt0 + n_sl_flt0 + n_sl_lng + n_sl_lng -  2 )

#define AddrLowLong (AddrLowFloat + n_sl_flt0 + n_sl_flt0 )


struct stor_buf
{
long int time;
char type_n;
char messg[mess_lgth];
};

extern int flag_Slv;
extern int ComPortSlv;
extern int AddrSlv;

extern unsigned long ComBaudSlv;
extern int Com_lgthSlv;
extern int Com_paritySlv;
extern int Com_stopSlv;
extern struct stor_buf *bf_Slv;
extern int uk_beg_Slv;
extern int uk_fin_Slv;
extern int Slv_int[];
extern float Slv_float[];

extern int flag_Slv3;
extern int ComPortSlv3;
extern int AddrSlv2;
extern unsigned long ComBaudSlv3;
extern int Com_lgthSlv3 ;
extern int Com_paritySlv3;
extern int Com_stopSlv3  ;


extern int cod_float;

extern void *list_Slv_int[];
extern float *list_Slv_flt[];
extern int n_sl_int ;
extern int n_sl_int1;
extern int n_sl_flt;
extern int n_sl_flt1;

extern int State_SLV;
extern int sw_mmi_slv;
extern int sw_dlv_slv;
extern int Last_evt;

extern int  WD_PMP_flag;
extern int  WD_PMP_Fl;
extern long int WD_PMP_time;
extern long int WD_PMP_Per;
extern long int Com_Baud_Slave;


void f_SlaveRTU();
void f_get_SlvBuf();
void f_free_SlvBuf();
void f_stor_Slv( char type,char *buf,int buf_n);
int ffgets_SlvRTU(int ComPort);
void  f_intr_SlvRTU();
int f_get_numRTU2(int ComPort);
int f_get_numRTU7(int ComPort);

int f_is_int(int addr);
int f_is_float(int addr);

void f_prn_slv1();

int  f_int_fnc(int Addr);
int f_stop_dlv();
int f_alm_stop();
int f_rst_slv();
int f_CE_slv();
int f_start_dlv();
int f_cmd_prn();
int f_WE_slv();
void ff_we_slv();

extern int CMD_REG;

extern int n_res_a;
extern int n_GRK_a;


extern int flag_slvrtu;
extern int nn_comb;

void fun_tim_u(void);

extern long int byte_timout_slv ;
extern long int out_delay_slv   ;

extern int fl_slv_out;

extern int n_sl_flt0;

