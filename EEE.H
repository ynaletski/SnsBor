
#define max_buf_eep 2050

void f_ee_num_init();
int f_wrt_eee();
int f_chk_EEE_CRC();
int f_rd_eee();

extern struct eee_dat EE_Dat[];
extern unsigned long int Eeprom_summ;
extern char buf_eep[2050];
extern unsigned int ee_lgth_byte;
extern int eee_num;
extern int ee_members;

extern int flag_Hst;

extern int flag_en_er;
