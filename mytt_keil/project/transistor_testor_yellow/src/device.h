 

#define AD_SEL1 PA3
#define AD_SEL0 PA2
#define AD_N_CHANNEL ADC_Channel_0 
#define AD_P_CHANNEL ADC_Channel_1

 

#define GAIN_CTRL_C GPIO_Pin_8
#define GAIN_CTRL_A GPIO_Pin_6
#define V_I_CTRL GPIO_Pin_7

//#define R_E_1K PA6
//#define R_E_100R PB2
//#define R_E_10R PB10
//#define R_E_1R PB11

#define BI0 PB3
#define BI1 PB4
#define BI2 PB5
#define RELAYS_ON() GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define RELAYS_OFF() GPIO_ResetBits(GPIOA,GPIO_Pin_15)	  

 enum{
	AD_Postive,
	AD_Nagetive
	
	
};

enum{
	GAIN_0DB,
	GAIN_20DB,
	GAIN_30DB
};

typedef enum {
	R_E_R36=0,
	R_E_3R6=1,
	R_E_33R=2,
	R_E_330R=3,
	R_E_SHUTDOWN=4
}R_E;
typedef enum {
R_B_390K,
R_B_39K,
R_B_3K9,
R_B_390R,
R_B_180R	
}R_B;
enum{
AD_CHANNEL_E,
AD_CHANNEL_C,
AD_CHANNEL_B
};
typedef enum{
I_OUT,
V_OUT	
}IV;


 
#ifdef _DEVICE_TRANSISTOR_ 

 

void DAC_configuretion(void);
u16 getchannlvolt(u32 channel);
float GetVoltByChannle(u32 channel);
void GPIO_Configuration(void);
void SET_AD_PN(int PN);
void Set_B_Resistor(int i);
void Sel_AD_CHANNEL(int i);
	void Set_E_Resistor(int x);
 void delay_ms( u16 nCount);
void SET_GAIN(int g);
void DAC_SetBaseRVolt(float v);
void DAC_SetCollectVolt(float v);
void Set_Vi_out(int x);
  float GetADCData(u8 TType,u8 cnt);
#else
extern void DAC_configuretion(void);
extern  void ADC_Config(void);
extern u16 getchannlvolt(u32 channel);
extern float GetVoltByChannle(u32 channel);
extern void GPIO_Configuration(void);
extern void SET_AD_PN(int PN);
extern void Set_B_Resistor(int i);
extern void Sel_AD_CHANNEL(int i);
extern	void Set_E_Resistor(int x);
extern void delay_ms( u16 nCount);
extern void SET_GAIN(int g);
extern void DAC_SetBaseRVolt(float v);
extern void DAC_SetCollectVolt(float v);
extern void Set_Vi_out(int x);
extern float GetADCData(u8 TType,u8 cnt);
extern const float BaseMax[];
extern const u32 R_B_MAP[];
#endif
