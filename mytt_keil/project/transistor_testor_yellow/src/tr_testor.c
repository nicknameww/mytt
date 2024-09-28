 #include "common.h"
 #include "device.h"
 #include "ServerCMD.h"
#define _TR_TEST_
 #include "tr_testor.h"
 #include "math.h"
 const short base_current_map[]={10,100,1000,10000,20000,1,1,50};//ua
 const int base_resistor_map[]={385000,39000,3900,310,1,1,1,75000};//这里的电阻值 为cd4051的导通电阻(大概40R@VCC-VEE 16V)+定值电阻，大电流时cd4051影响较大，需要根据实际情况测试调整
 const float base_maxcureent_volt[]={3.85,3.9,3.9,3.1,3,3,3,3.75};//
 typedef struct _GEGB_{
	float B_value; //三极管B值
	float E_voltage;//集电极 源极，放大后的电压 
	u8 GainIndex;//放大器增益表索引
	u8 E_resistor;//采样电阻表索引
}GEGB;
signed char c_flag,b_flag;//基极，集电极符号

const float R_E_list[]={0.33+0.09,3.6+0.09,33+0.168,327.5};//发射极采样电阻 +2*MOS电阻
const float G_list[]={1,10.901,33.25};//放大器增益列表
#define RCNT 3//(sizeof(R_E_list)/sizeof(float))
#define GCNT (sizeof(G_list)/sizeof(float))
#define ECNT RCNT*GCNT
float R_G_MAP[RCNT][GCNT];
float R_G_list[ECNT];
float diff_error[ECNT];
//const float expect_v=2.8;
 CMD cmdlist;
 u8 TestType;
void insertion_sort(float arr[], int len){
        int i,j;
					float key;
        for (i=1;i<len;i++){
                key = arr[i];
                j=i-1;
                while((j>=0) && (arr[j]>key)) {
                        arr[j+1] = arr[j];
                        j--;
                }
                arr[j+1] = key;
        }
}

 void GetRGIndex(u8 index,u8 *r_index,u8 *g_index){
	 int i;
	 float *pdata;
	 pdata=(float*)R_G_MAP;
	  
	 for(i=0;i<ECNT;i++){
		if(R_G_list[index]==*pdata)
		{
			*r_index=i/(GCNT);
			*g_index=i%(GCNT);
			break;
		}
		pdata++; 
	}
	 
 }
  void set_test_type(char type)
 {
	switch(type)
	{
		case TEST_TYPE_NPN:
			c_flag=1;
			b_flag=1;
			Set_Vi_out(I_OUT);
			break;
		case TEST_TYPE_PNP:
			c_flag=-1;
			b_flag=-1;
			Set_Vi_out(I_OUT);
			break;
		case TEST_TYPE_JFETN:
				c_flag=1;
				b_flag=-1;
			Set_Vi_out(V_OUT);
			break;
		case TEST_TYPE_JFETP:
				c_flag=-1;
				b_flag=1;
			Set_Vi_out(V_OUT);
			break;
		case TEST_TYPE_DIODE:
				c_flag=1;
				b_flag=1;
			break;
	}
	
 }
  //设置发射极电阻，及采样放大增益。
 u8 Get_EResistor_and_GAIN(TEST_PARM *ptp,GEGB *gegb)
 {
	 float min;//估计放大量
	 u8 cur,ri,gi; 
	 
   //Set_E_Resistor(R_E_SHUTDOWN);	 	 
	 //RELAYS_ON();
			 ////////////////
    
		DAC_SetCollectVolt(ptp->collection_v_end*c_flag);
	 ///////////////
	 min=3.3;
	 if(ptp->E_Resistor==0)//发射极电阻自动
	 {   
			// 
		
		   delay_ms(1);

			 memset(diff_error,0,sizeof(diff_error));
			 for(cur=0;cur<ECNT;cur++){
			 GetRGIndex(cur,&ri,&gi);
			 SET_GAIN(gi);
			 Set_E_Resistor(ri);
			 delay_ms(4);
			 diff_error[cur]=3.1-GetADCData(TestType,3);
		   if(diff_error[cur]<0)break;
			 if(diff_error[cur]>0&&diff_error[cur]<min){
					 gegb->E_resistor=ri;gegb->GainIndex=gi;min=diff_error[cur];
					 gegb->B_value=((3.1-min)/R_G_list[cur])/(base_current_map[ptp->base_serial_max_value]*1e-6)-1; 
					 gegb->E_voltage=c_flag*(3.1-min);
					 
				 }
			 }
			 
	 }
	  
  DAC_SetCollectVolt(0);
  DAC_SetBaseRVolt(0);
	//	RELAYS_OFF();	
  return 0;
 }
  void ShutDownBase(void)
 {	
	Set_E_Resistor(R_E_SHUTDOWN); 
	Set_Vi_out(V_OUT);	
	DAC_SetBaseRVolt(0);

//		switch(TestType)
//		{
//			 
//			case TEST_TYPE_NPN:
//			 
//			case TEST_TYPE_PNP:
//			  DAC_SetBaseRVolt(0);
//				Set_Vi_out(V_OUT);				
//				break;			
//			case  TEST_TYPE_JFETN:
//			  //DAC_SetBaseRVolt(-4);
//				Set_Vi_out(V_OUT);				
//				break;			
//			case  TEST_TYPE_JFETP:
//			 // DAC_SetBaseRVolt(4); 
//				Set_Vi_out(V_OUT);				
//				break;			
//		}
    
 }
 void cmdlistinit(void)
 {
	 
	 u8 i,j;
	cmdlist.isDone=1;
  TestType=0;
	Sel_AD_CHANNEL(AD_CHANNEL_E);	 
	DAC_SetCollectVolt(0);
	ShutDownBase();
	for(i=0;i<RCNT;i++)
	{
		for(j=0;j<GCNT;j++)
		{
			R_G_MAP[i][j]=R_E_list[i]*G_list[j];		
			R_G_list[3*i+j]=R_E_list[i]*G_list[j];
		}
	}
		insertion_sort(R_G_list,ECNT);
		//Set_EResistor_and_GAIN(2,0);
	 // GetRGIndex(5,&i,&j);
 }
 float GetIe(u8 gi,u8 ri,float *Ue)
 {
	 float Ie;
	 Ie=GetADCData(TestType,3);
	 *Ue=Ie/G_list[gi];
	 Ie/=(R_E_list[ri]*G_list[gi]);
  
	 return Ie; 
 }
 u8 getUce(void)
 {
		return 11;
 }
 u8 do_on_series_test(SERIALDATA *pSdata)
 {
 
	  float c_v,ue,stepv,tmpIe;
	   
	  
	  u16 cnt; 
	  u32 p;	 
    u16 *pData;
		c_v=pSdata->collection_v_start; 
		ue=fabs(pSdata->base_cureent);
	  if(TestType<TEST_TYPE_JFETN)
		{
			 
			if(1e-5>=ue)
			{
				pSdata->scale=0;//缩放因子 
				p= 10000000;
			}
			else if(ue>1e-5&&100e-6>ue)
			{
				pSdata->scale=1;//缩放因子 				
				p=100000;
			}
			else
			{
				p=10000;
				pSdata->scale=2;//缩放因子 
			}
		}
		else
		{
			p=100000;
			pSdata->scale=1;//缩放因子 
		}
			cnt=0;
		stepv=0.05;
		pData=(u16*)&pSdata->load;	
		while(cnt<(sizeof(pSdata->load)/sizeof(u16)))
		{			
			
			if(c_v>pSdata->collection_v_end)break;
			DAC_SetCollectVolt(c_v*c_flag);
			delay_ms(2);			
			tmpIe=GetIe(pSdata->gainIndex,pSdata->EresistorIndex,&ue);//	
			if(TestType<TEST_TYPE_JFETN)//  三极管不减Ib
				tmpIe-=pSdata->base_cureent; //得到ic
	    *pData=(u16)(tmpIe*p); 
		  pData++;		
		  *pData=((c_v-ue)*100.0);
			pData++;
			if(c_v>=1)stepv=0.1;
			c_v+=stepv;
			 
			cnt++;
		}
		// DAC_SetCollectVolt(0);	 
		return cnt;
 }

 SERIALDATA sdheader;
 
 void dowork(void)
 {
	 GEGB gegb;
	 float baisv,dv,dest_v;
		u8 i;
	Server_Hex_Down_Comm_Header *pcmd;
	TEST_PARM *ptp;
	if(!cmdlist.isDone)
	{
		
		  pcmd=(Server_Hex_Down_Comm_Header*)cmdlist.cmdbuf;
		  if(pcmd->AFN==0x02)
			{
				ptp=(TEST_PARM*)(&pcmd->Load);
				TestType=ptp->serial_type;
				set_test_type(TestType);
			if(ptp->serial_type==TEST_TYPE_JFETN||ptp->serial_type==TEST_TYPE_JFETP)
				DAC_SetBaseRVolt((float)ptp->gate_v_start/10.0);
			else					
				DAC_SetBaseRVolt(base_maxcureent_volt[ptp->base_serial_max_value]*b_flag);
			 Set_B_Resistor(ptp->base_serial_max_value);			
			  if(ptp->E_Resistor==0){//自动档时得到最佳射极电阻与放大增益组合。
					Get_EResistor_and_GAIN(ptp,&gegb);
				}
				else
				{					
					gegb.GainIndex=ptp->gain;			
					gegb.E_resistor=ptp->E_Resistor-1;
					gegb.B_value=-1;
					gegb.E_voltage=-1;				   
				}
				
				sdheader.gainIndex=gegb.GainIndex;			
				sdheader.EresistorIndex=gegb.E_resistor;	
				SET_GAIN(sdheader.gainIndex);
			  Set_E_Resistor(sdheader.EresistorIndex);
			 
				SendPacket(0x02,0,0,0,(void*)&gegb,sizeof(GEGB));//通知pc

				sdheader.collection_v_end=ptp->collection_v_end;
				sdheader.collection_v_start=ptp->collection_v_start;
			 
				sdheader.serial_type=ptp->serial_type;
				//sdheader.serial_cnt=100;
				if(ptp->serial_type<TEST_TYPE_JFETN)
				{
					dest_v=b_flag*base_maxcureent_volt[ptp->base_serial_max_value] ;
					dv=dest_v/(float)ptp->base_seg;
					baisv=dv;
					
				}
				else
				{
					//dest_v=(float)ptp->gate_v_end/10.0;
					dv=(float)(ptp->gate_v_end-ptp->gate_v_start)/(10*(ptp->base_seg));
					baisv=(float)ptp->gate_v_start/10.0;
				}
				 i=0;
				 while(i<ptp->base_seg)
				 { 
				  DAC_SetBaseRVolt(baisv);
				  delay_ms(1);
				 if(ptp->serial_type<TEST_TYPE_JFETN)
					sdheader.base_cureent=(float)(baisv/base_resistor_map[ptp->base_serial_max_value]);	//三级管为基极电流	
				 else
					 sdheader.base_cureent=baisv;//jfet就是门级电压 
				 
				 sdheader.serial_cnt=do_on_series_test(&sdheader);
					SendPacket(0x04,0,0,0,(void*)&sdheader, (sizeof(SERIALDATA)-1024)+sdheader.serial_cnt*4);
					 baisv+=dv;
				 i++;
				}
				
				__disable_irq();
				cmdlist.isDone=1;
				__enable_irq();		
					if(ptp->P.Parm.tested_shutdown)
					{
						 	
					 
						 DAC_SetCollectVolt(0);	
						 ShutDownBase();
						
					
					}
					
			}
	}
		delay_ms(5);
	}
