

float f_get_float( char *buf)
{
 union  { float f; unsigned long l; } o;

    //      byte    1  0  3  2
    //      buf    [0][1][2][3]
     o.l=
         (  (unsigned long)buf[1]  &  0x000000ff) |        // 3
         ( ((unsigned long)buf[0] << 8) &  0x0000ff00) |   // 2
         ( ((unsigned long)buf[3] << 16) & 0x00ff0000)  |  // 1
         ( ((unsigned long)buf[2] << 24) & 0xff000000);    // 0
         break;

  return o.f;

}
//-----------------------------------------
void f_set_float( char *buf,float value)
{
 union  { float f; unsigned long l; } o;

    //        byte  1  0  3  2
    //        buf  [0][1][2][3]

         buf[1] = o.l & 0xff;
         buf[0] = (o.l >> 8) & 0xff;
         buf[3] = (o.l >> 16) & 0xff;
         buf[2] = (o.l >> 24) & 0xff;

}
//-----------------------------------------

