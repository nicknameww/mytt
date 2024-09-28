#include "common.h"
#define _ServerCMD_
 
#include "ServerCMD.h"  
#include "math.h"

#include "tr_testor.h"
 
 extern   uint16_t SendUSBbytes (uint8_t* Buf, uint32_t Len);
 extern  CMD cmdlist;
 unsigned char sndbuf[1024];
 const char str[]="DO NOT DONE";
void SendPacket(unsigned char AFN, unsigned short addr,u8 parm,u8 I2C_TYPE, u8 *pLoad, u16 len)
{ 
	Server_Hex_Down_Comm_Header *pCmd; 
	u16 i;
	u8 sum=0;
	
  pCmd=(Server_Hex_Down_Comm_Header*)sndbuf;
	pCmd->FRA_SFLAG=0x7ee7;
	pCmd->PacketLen=len+sizeof(Server_Hex_Down_Comm_Header);//load 
	pCmd->I2CAddr=addr;
	pCmd->P.u8Parm=parm;
	pCmd->I2C_Type=I2C_TYPE;
	pCmd->AFN=AFN;//
	if(pLoad)memcpy(&pCmd->Load,pLoad,len);
	
	
	for(i=0;i<len+sizeof(Server_Hex_Down_Comm_Header)-1;i++)
				sum+=sndbuf[i];
	
	sndbuf[len+sizeof(Server_Hex_Down_Comm_Header)-1]=sum;
	SendUSBbytes(sndbuf,len+sizeof(Server_Hex_Down_Comm_Header));
	
	
	
}
void ServerCmdDispatch(uint8_t* revbuf, uint32_t Len)
{
	Server_Hex_Down_Comm_Header *Ser_Cmd; 
   
	 u16 tmp;
   u16 Packetlen,i; 
  	u8 sum; 
 
 
  if(Len>4)
  {
   tmp=*(u16*)revbuf;   
   if(tmp!=0x7ee7)return;	
      
 } 
	
  Ser_Cmd=(Server_Hex_Down_Comm_Header*)revbuf;
	Packetlen=Ser_Cmd->PacketLen; 
	if(Packetlen>64)return;
 
	sum=0;
	for(i=0;i<Ser_Cmd->PacketLen-1;i++)
	{  
		sum+=revbuf[i];
	}
	
	if(sum==revbuf[Ser_Cmd->PacketLen-1])
	{	
		switch(Ser_Cmd->AFN)
		{
			case 0x01:
			 //SendPacket(0x01,0,0,0,0,0);
			//GPIOA->ODR^=GPIO_Pin_15;
			break;
			case 0x02: 
			 if(!cmdlist.isDone) 
					SendPacket(0x88,0,0,0,(u8*)str,sizeof(str));
			 else{
				  tmp=Ser_Cmd->PacketLen;
				  memcpy(cmdlist.cmdbuf,(void*)revbuf,tmp);
				  cmdlist.isDone=0;
			 }
			break;
		}
	 
}
	 
}


 
 


 
 
 
