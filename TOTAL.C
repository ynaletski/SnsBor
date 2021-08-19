
float MassTotal=0,VolTotal=0,DenTotal=-1,TempTotal=-100;
float MassGRK=0., VolGRK=0.;
float Compos_rcv=0;
void f_cnt_totals()
{
  float Dens,M1,M2,V1;
  if(Reg_State & 0x90) // (RCV_act | RCV_fin)
  {
     if((Dens=s_MVD[0].Dens) < 450.) Dens=540.;
     if((M1=s_MVD[0].MassT) == vBIG_P) M1=0.;
     if((M2=s_MVD[1].MassT) == vBIG_P) M2=0.;
     if((V1=s_MVD[0].VolT) == vBIG_P) V1=0.;

     MassTotal = M1+M2 ;
     VolTotal=V1+ M2 * 1000./Dens;
     if(VolTotal != 0)
        DenTotal=MassTotal*1000./VolTotal;
     else
        DenTotal= s_MVD[0].Dens;
     TempTotal=s_MVD[0].Temp;

 //  Compos_rcv=f_get_composition(s_MVD[0].Dens,s_MVD[0].Temp); ...
 // !!! Недоделано
 //  TempTotal =f_get_temp(DenTotal,Compos_rcv);

  }

