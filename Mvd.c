
float K_mul_F=1000.;
float T_Ref=15.0;
float D_ref=540.;
float V_ref=1000.;
int flag_AD=0;

float Dens_AD=0,Temp_AD=0;
float Dens_a=0,Temp_a=0; // result

// Массовый расходомер Micro Motion MVD

float FctBern=0;
float R_MVD =12;  // мм

void (*ff_serv_N)()=NULL;


struct s_MVD_dev s_MVD[max_MVD]=
{
0 , //int r_addr;       // address of MODBUS register
0,  //int n;            // number of data - for read coils and ascii
0,  //int type;         // data type
0,  //int i;            // int,coil value for write
0,  //int exc;          // exception
0,  //long int  l;      // long
0,    //float f;          // float

5,    //-1.0, //float FlowM;         // float
-1.0, //float FlowV;         // float
1  ,  //-1.0, //float MassI;        // float
2,    //-1.0, //float MassT;         // float
3,    //-1.0,  //float VolI;          // float
4,    //-1.0, //float VolT;          // float
-1.0, //float VolTd;         // float
7,    //-1.0, //float Temp;          // float
-1.0, //float TempR;          // float
6.0, //float Dens;          // float
8.0,    //-1.0, //float Press          // float
-1.0, //float Compos;        // float
 0.0, //float PressG;        // float
-1.0, //float PressB;        // float
-1.0, //float PressD;        // float
-1.0, //float PressGN;       // float
-1.0, //float Volt_A         // float
-1.0, //float Volt_DC        // float
-1.0, //float DensG;         // float

0,    //int reg_err;

//================================
0 , //int r_addr;       // address of MODBUS register
0,  //int n;            // number of data - for read coils and ascii
0,  //int type;         // data type
0,  //int i;            // int,coil value for write
0,  //int exc;          // exception
0,  //long int  l;      // long
0,    //float f;          // float

15,    //-1.0, //float FlowM;         // float
-1.0, //float FlowV;         // float
11  ,  //-1.0, //float MassI;        // float
12,    //-1.0, //float MassT;         // float
13,    //-1.0,  //float VolI;          // float
14,    //-1.0, //float VolT;          // float
-1.0,  //float VolTd;         // float
17,    //-1.0, //float Temp;          // float
-1.0,  //float TempR;          // float
16,    //float Dens;          // float
-1,    //-1.0, //  float Press  // float
-1.0,  //float Compos;        // float
 0.0,  //float PressG;        // float
-1.0,  //float PressB;        // float
-1.0,  //float PressD;        // float
-1.0,  //float PressGN;       // float
-1.0,  //float Volt_A         // float
-1.0,  //float Volt_DC        // float
-1.0, //float DensG;         // float

0,    //int reg_err;

};
//--------------------------------
//-----------------------------------------
float f_get_float( char *buf, int cod)
{
 union  { float f; unsigned long l; } o;

     switch(cod)  // ( Reg 0521 )
     {
     case 0:
    //  code == 0 : 0  1  2  3
    //             [3][4][5][6]
     o.l=
         ( (unsigned long)buf[6]  &0xff) |                // 3
         ( ((unsigned long)buf[5] << 8) &0xff00) |        // 2
         ( ((unsigned long)buf[4] << 16) &0xff0000)  |    // 1
         ( ((unsigned long)buf[3] << 24) &0xff000000);    // 0
        break;
     case 1:
    //  code == 1 : 2  3  0  1
    //             [3][4][5][6]
     o.l=
         (  (unsigned long)buf[4]  &  0x000000ff) |        // 3
         ( ((unsigned long)buf[3] << 8) &  0x0000ff00) |   // 2
         ( ((unsigned long)buf[6] << 16) & 0x00ff0000)  |  // 1
         ( ((unsigned long)buf[5] << 24) & 0xff000000);    // 0
         break;
      case 2:
    //  code == 2 : 1  0  3  2
    //             [3][4][5][6]

     o.l=
         ( (unsigned long)buf[5]  &0xff) |              // 3
         ( ((unsigned long)buf[6] << 8) &0xff00) |      // 2
         ( ((unsigned long)buf[3] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)buf[4] << 24) &0xff000000);  // 0
         break;
      case 3:
    //  code == 3 : 3  2  1  0
    //             [3][4][5][6]
     o.l=( (unsigned long)(buf[3]) & 0x000000ff) |      // 3
         ( ((unsigned long)buf[4] << 8) &0xff00) |      // 2
         ( ((unsigned long)buf[5] << 16) &0xff0000)  |  // 1
         ( ((unsigned long)buf[6] << 24) &0xff000000);  // 0
       break;
       default : return 0.11;
     }

  if(( o.l == 0x7fa00000) || ( o.l == 0x00007fa0))
     o.f= BIG_P;
  return o.f;

}
//-----------------------------------------
void f_set_float( char *buf,float value, int cod)
{
 union  { float f; unsigned long l; } o;

    o.f=value;
    if(( o.l == 0x7fa00000) || ( o.l == 0x00007fa0))
       o.f=BIG_P;

     switch(cod)  // ( Reg 0521 )
     {
     case 0:  //  msb        lsb
    //  code == 0 : 3  2  1  0
    //             [0][1][2][3]
         buf[3] = o.l & 0xff;
         buf[2] = (o.l >> 8) & 0xff;
         buf[1] = (o.l >> 16) & 0xff;
         buf[0] = (o.l >> 24) & 0xff;

        break;
     case 1:
   default :
    //  code == 1 : 2  0  3  2
    //        buf  [0][1][2][3]
         buf[1] = o.l & 0xff;
         buf[0] = (o.l >> 8) & 0xff;
         buf[3] = (o.l >> 16) & 0xff;
         buf[2] = (o.l >> 24) & 0xff;

         break;
      case 2:
    //  code == 2 : 2  3  0  1
    //             [0][1][2][3]

         buf[2] = o.l & 0xff;
         buf[3] = (o.l >> 8) & 0xff;
         buf[0] = (o.l >> 16) & 0xff;
         buf[1] = (o.l >> 24) & 0xff;
         break;
      case 3:
    //  code == 3 : 0  1  2  3
    //             [0][1][2][3]

         buf[0] = o.l & 0xff;
         buf[1] = (o.l >> 8) & 0xff;
         buf[2] = (o.l >> 16) & 0xff;
         buf[3] = (o.l >> 24) & 0xff;

       break;
     }
}
//-----------------------------------------
void f_set_int( char *buf, int value)
{
         buf[1] = value & 0xff;
         buf[0] = (value >> 8) & 0xff;
}
//-----------------------------------------
void f_set_long( char *buf,long int value)
{
         buf[3] = value & 0xff;
         buf[2] = (value >> 8) & 0xff;
         buf[1] = (value >> 16) & 0xff;
         buf[0] = (value >> 24) & 0xff;
}
//-----------------------------------------
long int f_get_long( char *buf)
{
long int l_tmp;

    l_tmp =
          ( (unsigned long)(buf[3]) & 0x000000ff) |
          ( ((unsigned long)buf[2] << 8) &0xff00) |
          ( ((unsigned long)buf[1] << 16) &0xff0000)  |
          ( ((unsigned long)buf[0] << 24) &0xff000000);
  return l_tmp;
}
//-----------------------------------------
unsigned int f_get_int( char *buf)
{
unsigned int itmp;
        itmp=(buf[4] & 0xff) | ( ((unsigned int)buf[3] << 8) &0xff00)  ;
        return itmp;
}
//-----------------------------------------
int f_get_intS( char *buf)
{
 int i_tmp;

    i_tmp =
          ( (unsigned int)(buf[1]) & 0x00ff) |
          ( ((unsigned int)buf[0] << 8) &0xff00) ;

  return i_tmp;
}
//-----------------------------------------

