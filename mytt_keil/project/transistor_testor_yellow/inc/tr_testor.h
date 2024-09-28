 
 
   struct PARM{
	unsigned char Parm1:1;
	unsigned char Parm2:1;
	unsigned char Parm3:1;
	unsigned char Parm4:1;
	unsigned char Parm5:1;
	unsigned char Parm6:1;
	unsigned char NO_RES:1;
	unsigned char tested_shutdown:1;
};
    typedef struct _TEST_PARM_{ 
		unsigned char serial_type;
		signed char gate_v_start;
		signed char gate_v_end;//?????? 10 mv 
		unsigned char collection_v_start;//????????? 0.1V
		unsigned char collection_v_end;		
		unsigned char  base_seg;//????(ua) ??(mv) ??
		unsigned char  E_Resistor;
		unsigned char gain;
		unsigned char teststatus;
		union {
			struct  PARM		Parm;
			  unsigned char		u8Parm;
		  }P;
		unsigned short  base_serial_max_value;//????(ua) ??(mv)
  }TEST_PARM;

typedef struct _SerialData_{
		unsigned char serial_type;
	  unsigned char collection_v_end; 
		unsigned char collection_v_start; 
		unsigned char  serial_cnt; 
	  u8 scale;
		u8 tmp;
		u8 gainIndex;
		u8 EresistorIndex;
		float  base_cureent; 
		u16 load[512];// 
  }SERIALDATA;

typedef enum _TEST_TYPE_{
TEST_TYPE_NPN,
TEST_TYPE_PNP,
TEST_TYPE_JFETN,
TEST_TYPE_JFETP,
TEST_TYPE_DIODE
}TEST_TYPE;

 typedef struct _cmd_{
   u8 isDone;
	 char cmdbuf[31];
}CMD;
 
#ifdef _TR_TEST_
	void dowork(void);
#else		
	extern  void owork(void);
#endif
