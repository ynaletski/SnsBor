
int flag_en_er=PMP_max_inp_err;
char buf_eep[max_buf_eep];
int eee_num=0;
unsigned long int Eeprom_summ;
unsigned int ee_lgth_byte=0;
float fict_ftmp=0;
int fict_itmp=0;
int fict_sv=0;
int fict_int1=1;
int flag_Hst=1;

// Список параметров сохраняемых в EEPROM
struct eee_dat EE_Dat[]=
{
&ComBaud[0],sizeof(ComBaud[0]),
&ComBaud[1],sizeof(ComBaud[1]),
&ComBaud[2],sizeof(ComBaud[2]),
&ComBaud[3],sizeof(ComBaud[3]),
&ComBaud[4],sizeof(ComBaud[4]),

&eee_num,sizeof(eee_num),

#if defined(ICP_7017C)
//#if (0)    //defined(ICP_7017C)

&I7017C[0].status         ,sizeof(I7017C[0].status        ),
&I7017C[0].port           ,sizeof(I7017C[0].port          ),
&I7017C[0].addr           ,sizeof(I7017C[0].addr          ),
&I7017C[0].type           ,sizeof(I7017C[0].type          ),
&I7017C[0].format         ,sizeof(I7017C[0].format        ),
&I7017C[0].channels       ,sizeof(I7017C[0].channels      ),
&I7017C[0].CRC_flag       ,sizeof(I7017C[0].CRC_flag      ),
&I7017C[0].pool_time      ,sizeof(I7017C[0].pool_time     ),
&I7017_Rqst[0].timeout    ,sizeof(I7017_Rqst[0].timeout   ),

&flag_en_er               ,sizeof(flag_en_er              ),
&I7017C[1].port           ,sizeof(I7017C[1].port          ),
&I7017C[1].addr           ,sizeof(I7017C[1].addr          ),
&I7017C[1].type           ,sizeof(I7017C[1].type          ),
&I7017C[1].format         ,sizeof(I7017C[1].format        ),
&I7017C[1].channels       ,sizeof(I7017C[1].channels      ),
&I7017C[1].CRC_flag       ,sizeof(I7017C[1].CRC_flag      ),
&I7017C[1].pool_time      ,sizeof(I7017C[1].pool_time     ),
&I7017_Rqst[1].timeout    ,sizeof(I7017_Rqst[1].timeout   ),
#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout    ),

&flag_en_er               ,sizeof(flag_en_er              ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

#endif

&ComPortEcho          ,sizeof(ComPortEcho        ),
&ToutLicpScan         ,sizeof(ToutLicpScan       ),
&licp_amax            ,sizeof(licp_amax          ),
&LICP_Rqst.CRC_flag   ,sizeof(LICP_Rqst.CRC_flag ),

#if defined(ICP_7060)

&I7060[0].status         ,sizeof(I7060[0].status        ),
&I7060[0].port           ,sizeof(I7060[0].port          ),
&I7060[0].addr           ,sizeof(I7060[0].addr          ),
&I7060[0].type           ,sizeof(I7060[0].type          ),
&I7060[0].format         ,sizeof(I7060[0].format        ),
&I7060[0].channels       ,sizeof(I7060[0].channels      ),
&I7060[0].CRC_flag       ,sizeof(I7060[0].CRC_flag      ),
&I7060[0].pool_time      ,sizeof(I7060[0].pool_time     ),
&I7060_Rqst[0].timeout    ,sizeof(I7060_Rqst[0].timeout   ),

&I7060[1].status         ,sizeof(I7060[1].status        ),
&I7060[1].port           ,sizeof(I7060[1].port          ),
&I7060[1].addr           ,sizeof(I7060[1].addr          ),
&I7060[1].type           ,sizeof(I7060[1].type          ),
&I7060[1].format         ,sizeof(I7060[1].format        ),
&I7060[1].channels       ,sizeof(I7060[1].channels      ),
&I7060[1].CRC_flag       ,sizeof(I7060[1].CRC_flag      ),
&I7060[1].pool_time      ,sizeof(I7060[1].pool_time     ),
&I7060_Rqst[1].timeout    ,sizeof(I7060_Rqst[1].timeout   ),
#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout       ,sizeof(Rqst_fict.timeout    ),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.type           ,sizeof(ICP_fict.type          ),
&ICP_fict.format         ,sizeof(ICP_fict.format        ),
&ICP_fict.channels       ,sizeof(ICP_fict.channels      ),
&ICP_fict.CRC_flag       ,sizeof(ICP_fict.CRC_flag      ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout       ,sizeof(Rqst_fict.timeout    ),

#endif

&WDOG[0].status         ,sizeof( WDOG[0].status        ),
&WDOG[0].port           ,sizeof( WDOG[0].port          ),
&WDOG[0].CRC_flag       ,sizeof( WDOG[0].CRC_flag      ),
&WDOG[0].pool_time      ,sizeof( WDOG[0].pool_time     ),
&WDOG_Rqst[0].timeout   ,sizeof( WDOG_Rqst[0].timeout   ),

&WDOG[1].status         ,sizeof( WDOG[0].status        ),
&WDOG[1].port           ,sizeof( WDOG[0].port          ),
&WDOG[1].CRC_flag       ,sizeof( WDOG[0].CRC_flag      ),
&WDOG[1].pool_time      ,sizeof( WDOG[0].pool_time     ),
&WDOG_Rqst[1].timeout   ,sizeof( WDOG_Rqst[0].timeout   ),

&ComProt[0]     ,sizeof( ComProt[0] ),
&ComProt[1]     ,sizeof( ComProt[1] ),
&ComProt[2]     ,sizeof( ComProt[2] ),
&ComProt[3]     ,sizeof( ComProt[3] ),
&ComProt[4]     ,sizeof( ComProt[4] ),


#if defined(RTU_ComSK)

&ComSK[0].status         ,sizeof(ComSK[0].status        ),
&ComSK[0].port           ,sizeof(ComSK[0].port          ),
&ComSK[0].addr           ,sizeof(ComSK[0].addr          ),
&ComSK[0].pool_time      ,sizeof(ComSK[0].pool_time     ),
&ComSK_Rqst[0].timeout   ,sizeof(ComSK_Rqst[0].timeout   ),
&ComSK_WD_Rqst[0].timeout, sizeof(ComSK_WD_Rqst[0].timeout),
&sk_pool_period[0] ,sizeof(sk_pool_period[0]),

&ComSK[1].status         ,sizeof(ComSK[1].status        ),
&ComSK[1].port           ,sizeof(ComSK[1].port          ),
&ComSK[1].addr           ,sizeof(ComSK[1].addr          ),
&ComSK[1].pool_time      ,sizeof(ComSK[1].pool_time     ),
&ComSK_Rqst[1].timeout   ,sizeof(ComSK_Rqst[1].timeout   ),
&ComSK_WD_Rqst[1].timeout, sizeof(ComSK_WD_Rqst[1].timeout),
&sk_pool_period[1] ,sizeof(sk_pool_period[1]),

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long            ,sizeof(fict_long),
&fict_long            ,sizeof(fict_long),
#endif

#if defined(RTU_Delta)

&Delta[0].status         ,sizeof(Delta[0].status        ),
&Delta[0].port           ,sizeof(Delta[0].port          ),
&Delta[0].addr           ,sizeof(Delta[0].addr          ),
&Delta[0].pool_time      ,sizeof(Delta[0].pool_time     ),
&Delta_Rqst[0].timeout   ,sizeof(Delta_Rqst[0].timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),
//&Delta_WD_Rqst[0].timeout, sizeof(Delta_WD_Rqst[0].timeout),
//&Dlt_pool_period[0] ,sizeof(sk_pool_period[0]),

&Delta[1].status         ,sizeof(Delta[1].status        ),
&Delta[1].port           ,sizeof(Delta[1].port          ),
&Delta[1].addr           ,sizeof(Delta[1].addr          ),
&Delta[1].pool_time      ,sizeof(Delta[1].pool_time     ),
&Delta_Rqst[1].timeout   ,sizeof(Delta_Rqst[1].timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),
//&Delta_WD_Rqst[1].timeout, sizeof(Delta_WD_Rqst[1].timeout),
//&Dlt_pool_period[1] ,sizeof(sk_pool_period[1]),

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long                ,sizeof(fict_long),
&fict_long                ,sizeof(fict_long),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),
&fict_long            ,sizeof(fict_long),
&fict_long            ,sizeof(fict_long),

#endif
//-----------------------

&Com_lgth[0]   ,sizeof(Com_lgth[0]),
&Com_lgth[1]   ,sizeof(Com_lgth[0]),
&Com_lgth[2]   ,sizeof(Com_lgth[0]),
&Com_lgth[3]   ,sizeof(Com_lgth[0]),
&Com_lgth[4]   ,sizeof(Com_lgth[0]),
&Com_parity[0],sizeof(Com_parity[0]),
&Com_parity[1],sizeof(Com_parity[0]),
&Com_parity[2],sizeof(Com_parity[0]),
&Com_parity[3],sizeof(Com_parity[0]),
&Com_parity[4],sizeof(Com_parity[0]),
&Com_stop[0]  ,sizeof(Com_stop[0]),
&Com_stop[1]  ,sizeof(Com_stop[0]),
&Com_stop[2]  ,sizeof(Com_stop[0]),
&Com_stop[3]  ,sizeof(Com_stop[0]),
&Com_stop[4]  ,sizeof(Com_stop[0]),

//-----------------------
#if defined(RTU_MVD)

&MVD[0].status         ,sizeof(MVD[0].status        ),
&MVD[0].port           ,sizeof(MVD[0].port          ),
&MVD[0].addr           ,sizeof(MVD[0].addr          ),
&MVD[0].pool_time      ,sizeof(MVD[0].pool_time     ),
&MVD_Rqst[0].timeout   ,sizeof(MVD_Rqst[0].timeout   ),

&MVD[1].status         ,sizeof(MVD[1].status        ),
&MVD[1].port           ,sizeof(MVD[1].port          ),
&MVD[1].addr           ,sizeof(MVD[1].addr          ),
&MVD[1].pool_time      ,sizeof(MVD[1].pool_time     ),
&MVD_Rqst[1].timeout   ,sizeof(MVD_Rqst[1].timeout   ),

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

#endif
//-----------------------
#if defined(ICP_TSD)

&TSD[0].status         ,sizeof(TSD[0].status        ),
&TSD[0].port           ,sizeof(TSD[0].port          ),
&TSD[0].addr           ,sizeof(TSD[0].addr          ),
&TSD[0].pool_time      ,sizeof(TSD[0].pool_time     ),
&TSD_Rqst[0].timeout   ,sizeof(TSD_Rqst[0].timeout   ),

&TSD[1].status         ,sizeof(TSD[1].status        ),
&TSD[1].port           ,sizeof(TSD[1].port          ),
&TSD[1].addr           ,sizeof(TSD[1].addr          ),
&TSD[1].pool_time      ,sizeof(TSD[1].pool_time     ),
&TSD_Rqst[1].timeout   ,sizeof(TSD_Rqst[1].timeout   ),

#else

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

&ICP_fict.status         ,sizeof(ICP_fict.status        ),
&ICP_fict.port           ,sizeof(ICP_fict.port          ),
&ICP_fict.addr           ,sizeof(ICP_fict.addr          ),
&ICP_fict.pool_time      ,sizeof(ICP_fict.pool_time     ),
&Rqst_fict.timeout    ,sizeof(Rqst_fict.timeout   ),

#endif

//-----------------------

&byte_timout_rtu[0]   ,sizeof(byte_timout_rtu),
&scom_tim[0]   ,sizeof(scom_tim),

&ahour00.hour ,sizeof(ahour00.hour),
&ahour00.min  ,sizeof(ahour00.min),
&ahour00.sec  ,sizeof(ahour00.sec),

//-----------------------
&ComBaudSlv     ,sizeof(ComBaudSlv    ),
&Com_lgthSlv    ,sizeof(Com_lgthSlv   ),
&Com_paritySlv  ,sizeof(Com_paritySlv ),
&Com_stopSlv    ,sizeof(Com_stopSlv   ),
//-----------------------
&ComPortSlv ,sizeof(ComPortSlv ),
&AddrSlv    ,sizeof(AddrSlv  ),
&fict_itmp   ,sizeof(fict_itmp  ),
//-----------------------
&flag_Slv  ,sizeof(flag_Slv ),

&FlagWinSum ,sizeof(FlagWinSum ),

&f_ns[0]        ,sizeof(f_ns[0]        ),
&f_ns[1]        ,sizeof(f_ns[1]        ),
&f_ns[2]        ,sizeof(f_ns[2]        ),
&f_ns[3]        ,sizeof(f_ns[3]        ),
&f_ns[4]        ,sizeof(f_ns[4]        ),
&i_nn_d         ,sizeof(i_nn_d         ),
&dim_xyz        ,sizeof(dim_xyz        ),
&fl_trace_loop  ,sizeof(fl_trace_loop  ),

//-----------------------
&fict_ftmp      ,sizeof(fict_ftmp      ),
&fict_ftmp      ,sizeof(fict_ftmp      ),
&fict_ftmp      ,sizeof(fict_ftmp      ),
&fict_ftmp      ,sizeof(fict_ftmp      ),
&fict_itmp      ,sizeof(fict_itmp      ),
&fict_ftmp      ,sizeof(fict_ftmp      ),


&fict_long ,sizeof(fict_long  ),

&v_num_var                ,sizeof( v_num_var                ),

&fl_id1  ,sizeof( fl_id1),
&chr_id[0] ,sizeof( chr_id),
&fl_id  ,sizeof( fl_id),
//--------------
&fict_int1  ,sizeof(fict_int1 ),

&WD_PMP_Per  ,sizeof(WD_PMP_Per ),
&n_id        ,sizeof(n_id ),


#if defined(DIGIMAG)

&DGM[0].status         ,sizeof(DGM[0].status      ),
&DGM[0].port           ,sizeof(DGM[0].port        ),
&DGM[0].pool_time      ,sizeof(DGM[0].pool_time   ),
&DGM_Rqst[0].timeout   ,sizeof(DGM_Rqst[0].timeout),

&DGM[1].status         ,sizeof(DGM[1].status      ),
&DGM[1].port           ,sizeof(DGM[1].port        ),
&DGM[1].pool_time      ,sizeof(DGM[1].pool_time   ),
&DGM_Rqst[1].timeout   ,sizeof(DGM_Rqst[1].timeout),

&DGM[2].status         ,sizeof(DGM[1].status      ),
&DGM[2].port           ,sizeof(DGM[1].port        ),
&DGM[2].pool_time      ,sizeof(DGM[1].pool_time   ),
&DGM_Rqst[2].timeout   ,sizeof(DGM_Rqst[1].timeout),


&DGM_ADDR[0],      sizeof( DGM_ADDR[0]   ),
&DGM_ADDR[1],      sizeof( DGM_ADDR[1]   ),
&DGM_ADDR[2],      sizeof( DGM_ADDR[0]   ),


&s_DGM[0].offset    ,  sizeof(s_DGM[0].offset       ),
&s_DGM[1].offset    ,  sizeof(s_DGM[1].offset       ),
&s_DGM[2].offset    ,  sizeof(s_DGM[0].offset       ),
&s_DGM[0].scale   ,  sizeof(s_DGM[0].scale   ),
&s_DGM[1].scale   ,  sizeof(s_DGM[1].scale   ),
&s_DGM[2].scale   ,  sizeof(s_DGM[0].scale   ),


#endif
//-----------------------
#if defined(PMP201)

&PMP[0].status         ,sizeof(PMP[0].status      ),
&PMP[0].port           ,sizeof(PMP[0].port        ),
&PMP[0].addr           ,sizeof(PMP[0].addr          ),
&PMP[0].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[0].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[1].status         ,sizeof(PMP[1].status      ),
&PMP[1].port           ,sizeof(PMP[1].port        ),
&PMP[1].addr           ,sizeof(PMP[1].addr          ),
&PMP[1].pool_time      ,sizeof(PMP[1].pool_time   ),
&PMP_Rqst[1].timeout   ,sizeof(PMP_Rqst[1].timeout),

&PMP[2].status         ,sizeof(PMP[1].status      ),
&PMP[2].port           ,sizeof(PMP[1].port        ),
&PMP[2].addr           ,sizeof(PMP[1].addr          ),
&PMP[2].pool_time      ,sizeof(PMP[1].pool_time   ),
&PMP_Rqst[2].timeout   ,sizeof(PMP_Rqst[1].timeout),

&PMP[3].status         ,sizeof(PMP[0].status      ),
&PMP[3].port           ,sizeof(PMP[0].port        ),
&PMP[3].addr           ,sizeof(PMP[0].addr          ),
&PMP[3].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[3].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[4].status         ,sizeof(PMP[0].status      ),
&PMP[4].port           ,sizeof(PMP[0].port        ),
&PMP[4].addr           ,sizeof(PMP[0].addr          ),
&PMP[4].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[4].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[5].status         ,sizeof(PMP[0].status      ),
&PMP[5].port           ,sizeof(PMP[0].port        ),
&PMP[5].addr           ,sizeof(PMP[0].addr          ),
&PMP[5].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[5].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[6].status         ,sizeof(PMP[0].status      ),
&PMP[6].port           ,sizeof(PMP[0].port        ),
&PMP[6].addr           ,sizeof(PMP[0].addr          ),
&PMP[6].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[6].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[7].status         ,sizeof(PMP[0].status      ),
&PMP[7].port           ,sizeof(PMP[0].port        ),
&PMP[7].addr           ,sizeof(PMP[0].addr          ),
&PMP[7].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[7].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[8].status         ,sizeof(PMP[0].status      ),
&PMP[8].port           ,sizeof(PMP[0].port        ),
&PMP[8].addr           ,sizeof(PMP[0].addr          ),
&PMP[8].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[8].timeout   ,sizeof(PMP_Rqst[0].timeout),

//&s_PMP[0].offset    ,  sizeof(s_PMP[0].offset       ),
//&s_PMP[0].scale   ,  sizeof(s_PMP[0].scale   ),

//-ADD 7 REZERV-------------------------------
&PMP[9].status         ,sizeof(PMP[0].status      ),
&PMP[9].port           ,sizeof(PMP[0].port        ),
&PMP[9].addr           ,sizeof(PMP[0].addr          ),
&PMP[9].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[9].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[10].status         ,sizeof(PMP[0].status      ),
&PMP[10].port           ,sizeof(PMP[0].port        ),
&PMP[10].addr           ,sizeof(PMP[0].addr          ),
&PMP[10].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[10].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[11].status         ,sizeof(PMP[0].status      ),
&PMP[11].port           ,sizeof(PMP[0].port        ),
&PMP[11].addr           ,sizeof(PMP[0].addr          ),
&PMP[11].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[11].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[12].status         ,sizeof(PMP[0].status      ),
&PMP[12].port           ,sizeof(PMP[0].port        ),
&PMP[12].addr           ,sizeof(PMP[0].addr          ),
&PMP[12].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[12].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[13].status         ,sizeof(PMP[0].status      ),
&PMP[13].port           ,sizeof(PMP[0].port        ),
&PMP[13].addr           ,sizeof(PMP[0].addr          ),
&PMP[13].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[13].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[14].status         ,sizeof(PMP[0].status      ),
&PMP[14].port           ,sizeof(PMP[0].port        ),
&PMP[14].addr           ,sizeof(PMP[0].addr          ),
&PMP[14].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[14].timeout   ,sizeof(PMP_Rqst[0].timeout),

&PMP[15].status         ,sizeof(PMP[0].status      ),
&PMP[15].port           ,sizeof(PMP[0].port        ),
&PMP[15].addr           ,sizeof(PMP[0].addr          ),
&PMP[15].pool_time      ,sizeof(PMP[0].pool_time   ),
&PMP_Rqst[15].timeout   ,sizeof(PMP_Rqst[0].timeout),

#endif
//-----------------------

//-----------------------
&ComBaudSlv2    ,sizeof(ComBaudSlv2   ),
&Com_lgthSlv2   ,sizeof(Com_lgthSlv2  ),
&Com_paritySlv2 ,sizeof(Com_paritySlv2),
&Com_stopSlv2   ,sizeof(Com_stopSlv2  ),
//-----------------------
&ComPortSlv2,sizeof(ComPortSlv2),
&AddrSlv2   ,sizeof(AddrSlv2 ),
&flag_Slv2 ,sizeof(flag_Slv2),
//-----------------------


&num_out1, sizeof( num_out1 ),
&num_out2, sizeof( num_out2 ),

&alpha_LVL[0] , sizeof( alpha_LVL[0]  ),
&alpha_LVL[1] , sizeof( alpha_LVL[1]  ),
&T0_LVL[0]    , sizeof( T0_LVL[0]     ),
&T0_LVL[1]    , sizeof( T0_LVL[1]     ),
&alpha_Tank[0], sizeof( alpha_Tank[0] ),
&alpha_Tank[1], sizeof( alpha_Tank[1] ),
&T0_Tank[0]   , sizeof( T0_Tank[0]    ),
&T0_Tank[1]   , sizeof( T0_Tank[1]    ),

//---------------------------
//-----------------------
&ComBaudSlv3    ,sizeof(ComBaudSlv2   ),
&Com_lgthSlv3   ,sizeof(Com_lgthSlv2  ),
&Com_paritySlv3 ,sizeof(Com_paritySlv2),
&Com_stopSlv3   ,sizeof(Com_stopSlv2  ),
//-----------------------
&ComPortSlv3,sizeof(ComPortSlv2),
&AddrSlv3   ,sizeof(AddrSlv2 ),
&flag_Slv3 ,sizeof(flag_Slv2),
//-----------------------


&alpha_LVL[2] , sizeof( alpha_LVL[0]  ),
&T0_LVL[2]    , sizeof( T0_LVL[0]     ),
&alpha_Tank[2], sizeof( alpha_Tank[0] ),
&T0_Tank[2]   , sizeof( T0_Tank[0]    ),

&H_Cyl_M[0], sizeof(H_Cyl_M[0]  ),
&H_Cyl_M[1], sizeof(H_Cyl_M[1]  ),
&H_Cyl_M[2], sizeof(H_Cyl_M[2]  ),
&H_Cyl_M[3], sizeof(H_Cyl_M[0]  ),
&H_Cyl_M[4], sizeof(H_Cyl_M[1]  ),
&H_Cyl_M[5], sizeof(H_Cyl_M[2]  ),
&H_Cyl_M[6], sizeof(H_Cyl_M[0]  ),
&H_Cyl_M[7], sizeof(H_Cyl_M[1]  ),
&H_Cyl_M[8], sizeof(H_Cyl_M[2]  ),

&byte_timout_slv    ,sizeof( byte_timout_slv  ),
&out_delay_slv      ,sizeof( out_delay_slv    ),

&proc10             ,sizeof( proc10           ),
&proc90             ,sizeof( proc90           ),

&ZeroPage           ,sizeof( ZeroPage         ),
&EmptPage           ,sizeof( EmptPage         ),

&Password           ,sizeof( Password         ),

&fl_inv_out          ,sizeof( fl_inv_out      ),
&Tank_nn             ,sizeof( Tank_nn         ),

&Sl3_time1         ,sizeof( Sl3_time1     ),
&Sl3_time2         ,sizeof( Sl3_time2     ),
&Sl3_time3         ,sizeof( Sl3_time3     ),

&flag_Hst          ,sizeof( flag_Hst     ),
&num_in1           ,sizeof( num_in1      ),
&num_in2           ,sizeof( num_in2      ),

&inv_msk[0]        ,sizeof( inv_msk      ),

&Bot_time          ,sizeof( Bot_time     ),

&Lvl_offs          ,sizeof( Lvl_offs     ),

&Fl_ch_mm          ,sizeof( Fl_ch_mm     ),
&prc10[0]          ,sizeof( prc10        ),
&prc90[0]          ,sizeof( prc90        ),

&Eeprom_summ,sizeof(Eeprom_summ),

NULL,0,
};
//---------------------------
void f_ee_num_init()
{
int i;
   for( i=0;EE_Dat[i].ptr != 0;i++) {}
   eee_num=i;
}
//---------------------------
int ee_members=0;
int f_wrt_eee()
/* Store all variables in EEE  */
{
int i,i1,i2,j;
int val;
char *ch_ptr;

 Eeprom_summ=0;

        for(i=0,i1=0;EE_Dat[i].ptr!=0;i++)
        {
          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;
          for(j=0;j<i2;j++)
          {
            buf_eep[i1]=*ch_ptr++;
            if(EE_Dat[i].ptr!= &Eeprom_summ)
//              Eeprom_summ +=buf_eep[i1];
              Eeprom_summ +=((unsigned long)(buf_eep[i1])) & 0xff;
            i1++;
          }
        }

          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;

 EE_WriteEnable();
   i2= EE_MultiWrite_L(0,i1,buf_eep);
 EE_WriteProtect();

    ee_members=i;

// printf (" %d",i);
 return i2;
}
//--------------------------------------
int f_chk_EEE_CRC()
{
int i,i1,i2,j;
int val;
char *ch_ptr;
unsigned long int Ee_summ;

 Ee_summ=0;

 if((i1=EE_MultiRead_L(0,2048,buf_eep))<0 )
 {
   return i1;
 }
//-----------------------------
   // Check CRC
        for(i=0,i1=0;EE_Dat[i].ptr!=0;i++)
        {
          i2=EE_Dat[i].l;
          ch_ptr=(char *)EE_Dat[i].ptr;
          for(j=0;j<i2;j++)
          {
            if(EE_Dat[i].ptr==&Eeprom_summ)
              *ch_ptr++=buf_eep[i1++];
            else
              Ee_summ +=((unsigned long)(buf_eep[i1++])) & 0xff;
          }
        }

     ee_lgth_byte=i1;
//  printf("\n\rf_chk_EEE_CRC: %d записей,%dбайт,CRC_count=%08lx,CRC_rd%08lx\n\r",i,i1,Ee_summ,Eeprom_summ);
   if(Ee_summ != Eeprom_summ) return -100;
   return 0;
}
//--------------------------------------
int f_rd_eee()
/* Read all variables from  buf_eep  */
{
int i,i1,i2,j;
int val;
char *ch_ptr;

unsigned long int Ee_summ;
 Ee_summ=0;
//-----------------------------

   for(i=0,i1=0;(EE_Dat[i].ptr!=0)&&(i<(eee_num-1)) ;i++)
   {
     i2=EE_Dat[i].l;
     ch_ptr=(char *)EE_Dat[i].ptr;
     for(j=0;j<i2;j++)
     {
       *ch_ptr++=buf_eep[i1];
       Ee_summ +=buf_eep[i1++];
     }
   }
   f_ee_num_init();


#if defined(DIGIMAG)
   DGM[0].addr=(int)DGM_ADDR[0];
   DGM[1].addr=(int)DGM_ADDR[1];
   DGM[2].addr=(int)DGM_ADDR[2];
#endif

   Sl3_time1_l=Sl3_time1 * 1000;
   Sl3_time2_l=Sl3_time2 * 1000;
   Sl3_time3_l=Sl3_time3 * 1000;

   return 0;
}
//--------------------------------------

