

extern float Temp_kor;
extern float Comp_kor;

extern float PropLiq[];
extern float ButLiq[];
extern float ButILiq[];
extern float EtanGas[];
extern float PropGas[];
extern float ButGas[];
extern float ButIGas[];
extern float EtanP[];
extern float PropanP[];
extern float ButanP[];
extern float ButanIP[];


float f_get_dens();
void f_get_dens_c();
float f_get_composition(float Dens,float Temp);
float f_get_Dns(float Compos,float Temp);
void  f_get_pressCT(float Compos,float Temp);
void f_get_pressDT(float Dens,float Temp);

