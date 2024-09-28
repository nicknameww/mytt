#define _DEVICE_TRANSISTOR_
#include "common.h"
#include "device.h"

void DAC_configuretion(void)
{
	 DAC_InitTypeDef DAC_InitStructure;
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_InitStructure.DAC_Trigger=DAC_Trigger_Software;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bits1_0;
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Enable;
	
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);
	DAC_Cmd(DAC_Channel_1,ENABLE);
	
	DAC_Init(DAC_Channel_2,&DAC_InitStructure);
	DAC_Cmd(DAC_Channel_2,ENABLE);
}	
 void ADC_Config(void)
{
  ADC_InitTypeDef     ADC_InitStructure;
 
  
 
  
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
 
  
  /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
  ADC_Init(ADC1, &ADC_InitStructure); 
  
 
  ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_239_5Cycles);
 

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* Enable the ADC peripheral */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADRDY flag */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
   
  /* ADC1 regular Software Start Conv */ 
  //ADC_StartOfConversion(ADC1);
  
}
u16 getchannlvolt(u32 channel)
{
	ADC1->CR|=ADC_CR_ADSTP;
	ADC_ChannelConfig(ADC1, channel , ADC_SampleTime_71_5Cycles); 
	ADC1->CHSELR=channel;
	while( ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)==RESET); 
	ADC_StartOfConversion(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
	return ADC_GetConversionValue(ADC1);
}
float GetVoltByChannle(u32 channel)
{
	float v;
	v=VCC*getchannlvolt(channel);
	v=v/4096;
	return v;
}
 
float GetADCData(u8 TType,u8 cnt)
{
	float v;
	u8 i,channel;
	v=0;
	i=cnt;
	
	if(TType==0||TType==2||TType==4)
		channel=AD_N_CHANNEL;
	else
		channel=AD_P_CHANNEL;
		while(i){
				__disable_irq();
				v+=GetVoltByChannle(channel);
				__enable_irq();		
				i--;
				
			}
		return v/cnt;
 
	
}
	void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE); 
	 /*
  
	PA4	--BASE_I_CTRL (DA_C1),
	PA5	--C2 --BASE_VOLT_CTRL(DA_C2)
	*/
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
  /*
	#define BI0 PB3
	#define BI1 PB4
	#define BI2 PB5
	*/	
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
 
	 #define PIN_AD_SEL1 PB11
	 #define PIN_AD_SEL0 PB10
	 
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;// 
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	/*
 
	#define GAIN_CTRL_C PB8
	#define GAIN_CTRL_A PB6
	#define V_I_CTRL PB7
	
	*/
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;//
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB,&GPIO_InitStruct); 
	
 /*
	#define PIN_R_E_330R PA6
	#define PIN_R_E_33R PB0
	#define PIN_R_E_3R6 PB2
	#define PIN_R_E_0R36 PB1
 */
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;//
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;	
	GPIO_Init(GPIOB,&GPIO_InitStruct); 
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
	//PA15 relay 
		GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_15;//
 
	GPIO_Init(GPIOA,&GPIO_InitStruct); 
	
 
}
 
void Set_B_Resistor(int i)
{
  
	GPIOB->ODR&=(~(0x38));
	GPIOB->ODR|=((i<<3)&0x38);							
}
void Sel_AD_CHANNEL(int i)
{ 
  GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
	GPIOB->BRR|=((~(i<<10))&0xc00);
}
	void Set_E_Resistor(int x)
	{
		 
		switch(x)
		{
			case R_E_330R:
				GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);				
				GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				break;
			case R_E_33R:	
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOB, GPIO_Pin_1|GPIO_Pin_2);		
				GPIO_ResetBits(GPIOB,GPIO_Pin_0);			
				break;
			case R_E_3R6:	
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1);		
				GPIO_ResetBits(GPIOB,GPIO_Pin_2);			
				break;
			case R_E_R36:	
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_2);		
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);			
				break;

			case R_E_SHUTDOWN:
				GPIO_SetBits(GPIOA,GPIO_Pin_6);
				GPIO_SetBits(GPIOB, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2);		
				 
				break;
		}
	
	}
 void delay_ms( u16 nCount)
{
	u16 i;
	if(nCount==0)return;
	while(nCount--)
	{
		
		i=2300;
		while(i--);
	}
}
void SET_GAIN(int g)
{
	switch(g){
		case GAIN_0DB:
			GPIO_SetBits(GPIOB,GAIN_CTRL_C);
	  	GPIO_SetBits(GPIOB,GAIN_CTRL_A);
			break;
		case GAIN_30DB:
			GPIO_SetBits(GPIOB,GAIN_CTRL_C);
			GPIO_ResetBits(GPIOB,GAIN_CTRL_A);
		break;
		case GAIN_20DB: 
			GPIO_SetBits(GPIOB,GAIN_CTRL_A);
			GPIO_ResetBits(GPIOB,GAIN_CTRL_C);
		break;
		default:
			break;
	}
}
void DAC_SetBaseRVolt(float v)
{
	u16 data;
	v=(v+4)*3/8;
	data=(v/VCC)*4095;	
  if(data>=4095)data=4095;
DAC_SetChannel2Data(DAC_Align_12b_R,data);
DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE);	
}
void DAC_SetCollectVolt(float v)
{
	u16 data;
	v/=2;
	v=(v+5.25)/3.5;
	if(v>VCC)v=VCC;
	data=(v/VCC)*4095;	
  
DAC_SetChannel1Data(DAC_Align_12b_R,data);
DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);	
}
void Set_Vi_out(int x)
{
	if(x==I_OUT)
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
	if(x==V_OUT)
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
		
} 
