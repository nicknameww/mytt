typedef struct {
	unsigned char Parm1:1;
	unsigned char Parm2:1;
	unsigned char Parm3:1;
	unsigned char Parm4:1;
	unsigned char Parm5:1;
	unsigned char Parm6:1;
	unsigned char NOT_RES:1;
	unsigned char WriteReadTest:1;
}PARM;
 typedef struct _PROTOCOL_ 
{
  unsigned short	FRA_SFLAG; //;  
  unsigned short 	PacketLen; 
  unsigned short 	I2CAddr; 	
  unsigned char		AFN;		    
  unsigned char		I2C_Type;
  union {
	  PARM		Parm;
	  unsigned char		u8Parm;
  }P;
  unsigned char		Load;
  
 }Server_Hex_Down_Comm_Header;
#ifdef _SERVERCMD_ 
 void SendPacket(unsigned char AFN, unsigned short addr,u8 parm,u8 I2C_TYPE, u8 *pLoad, u16 len);
#else 
 extern void SendPacket(unsigned char AFN, unsigned short addr,u8 parm,u8 I2C_TYPE, u8 *pLoad, u16 len);
#endif
