#include <REGtenxTM52F5284.H>
#include <intrins.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>





#define BR9600 6	//Fsysclk=3.6864MHz
//#define BR9600 3	//Fsysclk=1.8432MHz
#define LED_G P1_6
#define TRIG_ULTRASONIC P1_7
#define ECHO_ULTRASONIC	P2_7
#define WIFI_INDI P3_2
#define TCP_INDI P3_3
#define IP_INDI P3_4
#define WORK_INDI P3_5
#define PERIOD 25500	//	25500
#define PWM_UNIT 100	//	25500/255=100
//#define AP "IMGoCSEEIT"
//#define AP_pwd "aaaa0816"
#define AP "HTC123"
#define AP_pwd "12345678"
//#define AP "My ASUS"
//#define AP_pwd "88887777"
#define DMg "A:AM:A2M:A2M"	//
#define PM "A2M"	//車格編號
//#define IP_add "192.168.43.51"
//#define IP_add "192.168.0.102"
#define IP_add "192.168.0.103"
//#define IP_add "192.168.137.63"
#define PORT "8087"
#define AID "Z1"
#define DET_HIGH 20
//#define DET_HIGH_DEF 20
#define CHECK_TIME_O 5
//#define CHECK_TIME_O_DEF 10
#define CHECK_TIME_UO 5
//#define CHECK_TIME_UO_DEF 10





bit wifi_flag=0;
bit TX_flag=0;
bit RX_flag=0;
bit park_flag=0;	//park status flag
bit modify_flag=0;	//park status change flag
bit TCP_flag=0;	//TCP 連線成功旗標
bit addDev_flag=0;	//新增裝置旗標 首次連線登錄裝置資訊
bit STATUS_H=0;	//首次連線時偵測車格狀態用旗標 高位元
bit STATUS_L=0;	//首次連線時偵測車格狀態用旗標 低位元	狀態機:三種狀態
bit Remv_flag=0;	//裝置移除旗標
bit IP_flag=0;
bit Roff_flag=1;
bit search_flag=0;
bit Send_flag=0;
bit mac_flag=0;
//bit set_flag=0;

unsigned char R=0;
unsigned char G=0;
unsigned char Blue=0;
idata unsigned char SEC=20;
idata unsigned char ESPt=0;
idata unsigned char Echo_timeout=0;
idata unsigned char addD_timeout=0;
//idata unsigned char time_o=CHECK_TIME_O_DEF;
//idata unsigned char time_uo=CHECK_TIME_UO_DEF;
//idata int high=DET_HIGH_DEF;
unsigned int dis=0;	//判斷距離用於偵測車位是否有停車

idata char Pip[20]={0};	//PPlc IP
idata char Mac[25]={0};

char RX_temp[45]={0};
//char color[3]={0};	
char timeout=0;
char RXi=0;
unsigned char O=0;
unsigned char UO=0;
char ip_key=0;
char mac_key=0;
//char i=0;
//char j=0;
char i,j;
char color[3]={0};	
char* temp=NULL;
char* str=NULL;



int T_on=0;

/*
char* code Add_NO="{\"Data\":{\"MT\":\"Add\",\"DT\":\"PPlc\",\"DMg\":\"A:AM:A1:A1M\",\"AID\":\"Z1\",\"PS\":\"NOc\"}}\n";
char* code Add_OCC="{\"Data\":{\"MT\":\"Add\",\"DT\":\"PPlc\",\"DMg\":\"A:AM:A1:A1M\",\"AID\":\"Z1\",\"PS\":\"Oc\"}}\n";
char* code State_NO="{\"Data\":{\"MT\":\"SU\",\"PM\":\"A1M\",\"PS\":\"NOc\",\"AID\":\"Z1\"}}\n";
char* code State_OCC="{\"Data\":{\"MT\":\"SU\",\"PM\":\"A1M\",\"PS\":\"Oc\",\"AID\":\"Z1\"}}\n";
*/
//char code CIPSTART[]="AT+CIPSTART=1,\"TCP\",\"192.168.43.51\",8087\r\n";
char code CIPSTART_Gen[]="AT+CIPSTART=1,\"TCP\",\"";
char code CIPCLOSE[]="AT+CIPCLOSE=1\r\n";
char code CWMODE_DEF[]="AT+CWMODE_DEF=1\r\n";
char code CIPMUX[]="AT+CIPMUX=1\r\n";
char code CIPSERVER[]="AT+CIPSERVER=1,3333\r\n";
char code CIPSENDEX[]="AT+CIPSENDEX=1,120\r\n";
char code CIPSTA_CUR[]="AT+CIPSTA_CUR?\r\n";
char code CIFSR[]="AT+CIFSR\r\n";
char code CIPSTAMAC_CUR[]="AT+CIPSTAMAC_CUR?\r\n";

char code ATE[]="ATE0\r\n";

//char code Add_NO[]="{\"Data\":{\"MT\":\"Add\",\"DT\":\"PPlc\",\"DMg\":\"A:AM:A1:A1M\",\"AID\":\"Z1\",\"PS\":\"NOc\"}}\n";
char code Add_NO_Gen[]="{\"Data\":{\"MT\":\"Add\",\"DT\":\"PPlc\",\"DMg\":\"";
char code Add_NO_Gen2[]="\",\"AID\":\"";
char code Add_NO_Gen3[]="\",\"PS\":\"NOc\"}}\n";

//char code Add_OCC[]="{\"Data\":{\"MT\":\"Add\",\"DT\":\"PPlc\",\"DMg\":\"A:AM:A1:A1M\",\"AID\":\"Z1\",\"PS\":\"Oc\"}}\n";
char code Add_OCC_Gen[]="{\"Data\":{\"MT\":\"Add\",\"DT\":\"PPlc\",\"DMg\":\"";
char code Add_OCC_Gen2[]="\",\"AID\":\"";
char code Add_OCC_Gen3[]="\",\"PS\":\"Oc\"}}\n";

//char code State_OCC[]="{\"Data\":{\"MT\":\"SU\",\"PM\":\"A1M\",\"PS\":\"Oc\",\"AID\":\"Z1\"}}\n";
char code State_OCC_Gen[]="{\"Data\":{\"MT\":\"SU\",\"PM\":\"";
char code State_OCC_Gen2[]="\",\"PS\":\"Oc\",\"AID\":\"";
char code State_OCC_Gen3[]="\"}}\n";

//char code State_NO[]="{\"Data\":{\"MT\":\"SU\",\"PM\":\"A1M\",\"PS\":\"NOc\",\"AID\":\"Z1\"}}\n";
char code State_NO_Gen[]="{\"Data\":{\"MT\":\"SU\",\"PM\":\"";
char code State_NO_Gen2[]="\",\"PS\":\"NOc\",\"AID\":\"";
char code State_NO_Gen3[]="\"}}\n";



void UART_INT() interrupt 4 using 2
{
//WIFI_INDI=0;
	if(TI)
	{
		TI=0;
		TX_flag=0;
	}

	if(RI)
	{
		RI=0;
		RX_temp[RXi++]=SBUF;

		if(SBUF=='\n')
		{
			RX_temp[RXi]='\0';
			RXi=0;
			if(RX_temp[0]!=0)
			{
//	            if(strstr(RX_temp,"busy")!=NULL);

	            if(strstr(RX_temp,"ERR")!=NULL);

	            else if(strstr(RX_temp,"1,CO")!=NULL)	//與Arduino TCP連線成功豎立登錄旗標
	            {
	            	TCP_flag=1;
	            	P3_3=0;
//	            	P1_7=0;
//	                RX_temp[0]=0;
	            }

	            else if(strstr(RX_temp,"1,CL")!=NULL)	//TCP連線中止
	            {
	                TCP_flag=0;
	                P3_3=1;
//	                P1_7=1;
//	                RX_temp[0]=0;
	            }

				else if(strstr(RX_temp,"LON")!=NULL)	//中控開燈命令	<font color="#RRGGBB">
	            {
	                temp=strchr(RX_temp,'#');
	                /*
	                R=strtol(temp,NULL,16)/256/256;
	                G=strtol(temp,NULL,16)/256;
	                Blue=strtol(temp,NULL,16)%256;
					*/
					search_flag=1;
//					INTE1&=0xFE;
					for(i=0;i<=2;i++)
					{
						for(j=0;j<=1;j++)
						{
							temp=temp+1;
							color[j]=*temp;
							if(j==1)
							{
								color[2]='\0';
							}
						}
						if(!i)
						{
							R=strtol(color,NULL,16);
						}
						else if(i==1)
						{
							G=strtol(color,NULL,16);
						}
						else if(i==2)
						{
							Blue=strtol(color,NULL,16);
						}
						color[0]=0;
						color[1]=0;
					}
					
					if(!G)
					{
						LED_G=1;
						TR0=0;
//						ET0=0;
						Roff_flag=1;
					}
					else
					{
						if(G==255)
						{
							Roff_flag=1;
							LED_G=0;
							TR0=0;
//							ET0=0;
						}
						else
						{
							T_on=G*PWM_UNIT;
							Roff_flag=0;
							TH0=(65536-T_on)/256;
							TL0=(65536-T_on)%256;
//							ET0=1;
							TR0=1;
							LED_G=0;	
						}	
					}
					if(!R)
					{
						PINMOD&=0xBF;
						P1_4=1;
					}
					else
					{
						PWM0DH=255-R;
						PINMOD|=0x40;
					}
					if(!Blue)
					{
						PINMOD&=0x7F;
						P1_5=1;
					}
					else
					{
						PWM1DH=255-Blue;
						PINMOD|=0x80;
					}				
	//				RX_temp[0]=0;
	            }

	            else if(strstr(RX_temp,"LOFF")!=NULL)	//中控關燈命令
	            {
					Roff_flag=1;
					search_flag=0;
//					ET0=0;
					TR0=0;
					LED_G=1;
					PINMOD&=0x1F;
//					INTE1|=0x01;				
				
	//				RX_temp[0]=0;
	            }
	            else if(strstr(RX_temp,"AIP,")!=NULL)
	            {
	            	if(strstr(RX_temp,"0.0.0.0")==NULL)
	            	{
		            	strcpy(Pip,strchr(RX_temp,'\"')+1);	//抓取IP checked 4/5/2017
		            	while(Pip[ip_key]!='\"')
		            		ip_key++;
		    			*(Pip+ip_key)='\0';
		    			IP_flag=1;
		    			IP_INDI=0;
		    			ip_key=0;
		    		}
		    		else
		    		{
		    			IP_flag=0;
		    			IP_INDI=1;
		    		}
	//	    		RX_temp[0]=0;
	            }

	           	else if(strstr(RX_temp,"GOT")!=NULL)	//wifi 連線
	            {
	                wifi_flag=1;
	                WIFI_INDI=0;
	//                RX_temp[0]=0;
	            }
	     
	            else if(strstr(RX_temp,"DIS")!=NULL)	//wifi斷線
	            {
	                wifi_flag=0;
	                IP_flag=0;
	                WIFI_INDI=1;
	//                RX_temp[0]=0;
	            }

	            else if(strstr(RX_temp,"ADDS")!=NULL)
	            {
	            	STATUS_H=1;
				    STATUS_L=0;
				    addDev_flag=1;
//				    P3_3=0;

	//			    RX_temp[0]=0;
	            }

				else if(strstr(RX_temp,"ADDF")!=NULL)
				{
					STATUS_H=0;
	    			STATUS_L=0;

	//    			RX_temp[0]=0;
				}

				else if(strstr(RX_temp,str)!=NULL)
				{
					RX_flag=0;
	//				RX_temp[0]=0;
				}

				else if(strstr(RX_temp,"C_CU")!=NULL)
				{
					if(strstr(RX_temp,"ff:ff:ff:ff:ff:ff")==NULL)
					{
						strcpy(Mac,strchr(RX_temp,'\"')+1);
						while(Mac[mac_key]!='\"')
							mac_key++;
						*(Mac+mac_key)='\0';
						mac_key=0;
						mac_flag=1;
					}
				}

	            else if(strstr(RX_temp,"REMV")!=NULL)	//裝置移除
	            {
					LED_G=1;
					PINMOD&=0x1F;						
					Remv_flag=1;
					STATUS_H=0;	
					STATUS_L=0;

	//				RX_temp[0]=0;
	            }


/*
	            else if(strstr(RX_temp,"Set")!=NULL)	//Set:H5C5
	            {
	            	for(i=0;i<=1;i++)
					{
						atoi()
					}
	            }

*/	            
	/*
	            else if(strstr(RX_temp,"ADDD")!=NULL)
	            {
					Remv_flag=0;
					RX_temp[0]=0;
	            }
	*/
		       	RX_temp[0]=0;	//刪除資料   
//		       	RI=0;
			}			
		}
	}
}

void PWM_R() interrupt 1 using 1
{
//	P3_6=~P3_6;
	if(Roff_flag)
	{
		if(G==255)
			LED_G=0;

		else
			LED_G=1;

		TR0=0;
		TH0=0;
		TL0=0;
//		TR0=1;	
	}
	else
	{
		if(LED_G)
		{
			TR0=0;
			TH0=(65536-T_on)/256;
			TL0=(65536-T_on)%256;
			TR0=1;
			LED_G=0;
		}
		else if(!LED_G)
		{	
			TR0=0;
			TH0=(65536-(PERIOD-T_on))/256;
			TL0=(65536-(PERIOD-T_on))%256;
			TR0=1;
			LED_G=1;
		}
	}
}

void InitNOP()   //10us
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}	

void Send_STR(char* TX_data, char* ptr)	//UART 傳輸函式
{
	char i=0;

	str=ptr;

	if(str[0]!='\0')		//設定接收完成旗標
		RX_flag=1;	

	while(TX_data[i]!='\0')		//TX until EOF
	{
		TX_flag=1;
		SBUF=TX_data[i];
		while(TX_flag);
		i++;
	}
	
	ESPt=0;
	while(RX_flag)	//等待完成timeout
	{
		if(ESPt>=1)		
			RX_flag=0;
	}
}

void TM3_INT() interrupt 7 using 3	//TIMER3中斷 1秒一次 用於超音波sensor判斷車格狀態
{
	
	if(ESPt<=255)	//timer 1s for check return str
		ESPt++;
	else
		ESPt=0;

	
	if(SEC<=255)	//1s for waiting re-addDev timout 20s
		SEC++;	
	else
		SEC=0;

	WORK_INDI=~WORK_INDI; //working indicator

	if(!Remv_flag)
	{
		dis=0;	// 距離
		Echo_timeout=0;

		TRIG_ULTRASONIC=1;
	 	InitNOP();	//10us trigger ultrasonic
		TRIG_ULTRASONIC=0;
		
		while((!ECHO_ULTRASONIC)&&(Echo_timeout<=200))	//waiting echo timeout, preventing  when interrupt is occured
		{
			_nop_();
			Echo_timeout++;
		}	//等待ECHO
		
		while(ECHO_ULTRASONIC&&(dis<=400))
		{	
			//58.2us per cm
			InitNOP();
			InitNOP();
			InitNOP();
			InitNOP();
			InitNOP();
			dis++;		
		}

		if(dis<DET_HIGH)	//distance<DET_HIGH cm 
		{
			if(O>=255)
			{
				O=CHECK_TIME_O;
			}
			else
				O++;	//check Oc	
//			P3_6=1;	//parking indicator 
//			P3_7=0;
			if(!search_flag)
				P1_6=1;
			UO=0;
		}
		else 
		{
			if(UO>=255)
			{
				UO=CHECK_TIME_UO;
			}
			else
				UO++;	//unOc
//			P3_6=0;
//			P3_7=1;	//parking indicator
			if((!search_flag)&&(!Remv_flag))
				P1_6=0;
			O=0; 
		}

		if(IP_flag&&(!Send_flag))
		{
			if(!addDev_flag)
				STATUS_L=0;
			if((!STATUS_H)&&(!STATUS_L))	//首次執行需要向中控登錄車格資訊及車格狀態,先抓取第一次車格狀態 first_flag=00(binary)
			{
				if(O>=CHECK_TIME_O)
				{
					park_flag=1;
					STATUS_L=1;
					STATUS_H=0;	//登錄車格之後進到下一個狀態,此狀態用於傳送給arduino車格資訊 first_flag=01(binary)
				}
				if(UO>=CHECK_TIME_UO)
				{
					park_flag=0;
					STATUS_L=1;
					STATUS_H=0;	
				}
			}
			else if(STATUS_H&&(!modify_flag))	//進入一般執行狀態 first_flag=10(binary) Detecion flag guarantee that status is handled
			{	
				if(O>=CHECK_TIME_O)	//check 10 times(SEC)
				{
					if(!park_flag)
					{
						modify_flag=1;	//狀態變化旗標,與上次狀態相同 modify_flag=0
					}

					park_flag=1;	

					if(O>=255)	//狀態保持時間重置
					{
						O=CHECK_TIME_O;
					}
				}
				else if(UO>=CHECK_TIME_UO)
				{
					if(search_flag)
					{
						PINMOD&=0x1F;
						TR0=0;
						LED_G=1;
						search_flag=0;
					}
					
					if(park_flag)
					{
						modify_flag=1;
					}

					park_flag=0;

					if(UO>=255)	//狀態保持時間重置
					{
						UO=CHECK_TIME_UO;
					}
				}
			}
		}
	}
}

void Init_PWM_GB()	//PWM initial GB
{
	PWMCON=0x00;	
	PWM0PRD=0xFF;
	PWM1PRD=0xFF;
}

void Init_PWM_R()	//PWM initial B
{
	TMOD|=0x01;
	IPH|=0x02;
	IP|=0x02;
	TH0=0/256;
	TL0=0%256;
	TR0=0;
	LED_G=1;
	ET0=1;	
//	TR0=1;
}

void InitiIO()
{
	P1MODH|=P17MOD_2;	//CMOS output 
	TRIG_ULTRASONIC=0;	
}

void InitTM3()	//timer 1s initial
{
    OPTION&=0xFC;		//32768 clock cycle
	INTE1|=0x01;		//Enable TM3 INT
}

void InitUART()	//UART initial
{
	P3MODL&=0xF0;			//P3.0(RX), P3.1(TX) as mode 0

	TMOD|=T1_Mode2;			//Set TM1 for UART	
	TL1=256-BR9600;
	TH1=256-BR9600;
	TR1=1;

	SM0=0;				//Mode 1: 8 bit UART, Baud Rate is variable 
	SM1=1;
	REN=1;	//UART Reception Enable
	IPH|=0x10;
	IP&=0xEF;

	ES=1;				//Serial Port interrupt enable
}
/*
char* TCP_con() //Building TCP connection
{
	idata char str[50]={0};

	strcpy(str,"AT+CIPSTART=3,\"TCP\",\"");
	strcat(str,IP_add);
	strcat(str,"\",");
	strcat(str,PORT);
	strcat(str,"\r\n");
	
	return str;
}
*/
/*
void TCP_close()	//Closing TCP connection
{
	Send_STR("AT+CIPCLOSE=0\r\n","OK");
}

/*
void Init_ESP8266()	//STA Mode, single connection 
{	
	ESPt=0;
	Send_STR("AT+CWMODE_DEF=1\r\n","OK");	//STA mode 
	while(ESPt<=3);
	Send_STR("AT+CIPMUX=1\r\n","OK");
	while(ESPt<=6);
	Send_STR("AT+CIPSERVER=1,3333\r\n","OK");

	strcpy(str,"AT+CWJAP_DEF=");	//setting WiFi using predefine data
	strcat(str,"\"");
	strcat(str,AP);
	strcat(str,"\",");
	strcat(str,"\"");
	strcat(str,AP_pwd);
	strcat(str,"\"");
	strcat(str,"\r\n");
	Send_STR(str,OK);
	InitNOP();

}
*/
/*
char* AddDev(char* Parkstatus) large	//adding Device checked 4/5/2017
{
	char str[120]={0};
//	char code AddD[]="{\"Data\":{\"MT\":\"Add\",\"DT\":\"PPlc\",\"DMg\":\"";
//	strcpy(str,"IP:");
//	strcat(str,Pip);
	strcpy(str,"{\"Data\":{\"MT\":\"Add\",\"DT\":\"PPlc\",\"DMg\":\"");
	strcat(str,DMg);
	strcat(str,"\",\"AID\":\"");
	strcat(str,AID);
	strcat(str,"\",\"PS\":\"");
	strcat(str,Parkstatus);
	strcat(str,"\"}}\n");
	
	return str;
}

*/
/*
char* Update_Stat(char* Parkstatus)	large//checked 4/5/2017
{	
	char str[100]={0};
//	char code UpSt[]="{\"Data\":{\"MT\":\"SU\",\"PM\":\"";
	strcpy(str,"{\"Data\":{\"MT\":\"SU\",\"PM\":\"");
	strcat(str,PM);
	strcat(str,"\",\"PS\":\"");
	strcat(str,Parkstatus);
	strcat(str,"\",\"AID\":\"Z1\"}}\n");
	
	return str;	
}
*/
/*
char* Init_WiFi() large	//checked 4/5/2017
{
	char str[50]={0};

	strcpy(str,"AT+CWJAP_DEF=");	//Setting WiFi
	strcat(str,"\"");
	strcat(str,AP);
	strcat(str,"\",");
	strcat(str,"\"");
	strcat(str,AP_pwd);
	strcat(str,"\"");
	strcat(str,"\r\n");
	Send_STR(str,"OK");

	return str;
}
/*
void TCP_cnn()
{
	Send_STR(CIPSTART_Gen,"");
	Send_STR(IP_add,"");
	Send_STR("\",","");
	Send_STR(PORT,"");
	Send_STR("\r\n","SUCCESS");
}
*/
/*
void Add_NO()
{
    Send_STR(Add_NO_Gen,"");
	Send_STR(DMg,"");
	Send_STR(Add_NO_Gen2,"");
	Send_STR(AID,"");
	Send_STR(Add_NO_Gen3,"");
}
*/
/*
void Add_OCC()
{
	Send_STR(Add_OCC_Gen,"");
	Send_STR(DMg,"");
	Send_STR(Add_OCC_Gen2,"");
	Send_STR(AID,"");
	Send_STR(Add_OCC_Gen3,"");
}
*/
/*
void State_OCC()
{
	Send_STR(State_OCC_Gen,"");
	Send_STR(PM,"");
	Send_STR(State_OCC_Gen2,"");
	Send_STR(AID,"");
	Send_STR(State_OCC_Gen3,"");
}
*/
/*
void State_NO()
{
	Send_STR(State_NO_Gen,"");
	Send_STR(PM,"");
	Send_STR(State_NO_Gen2,"");
	Send_STR(AID,"");
	Send_STR(State_NO_Gen3,"");
}
*/

void main()
{
	char i=0;

	CLKCON&=0xFE;	//CPUCLK Prescaler /2
//	CLKCON&=0xFD;	//CPUCLK Prescaler /4
	InitNOP();
	SELFCK=1;		//Select FRC as System clock source	
	SCKTYPE=1;
//	SP=0x1E;
//	IPH=0x12;
//	IP|=IPH_PSH;
	
	EA=1;
	InitiIO();	//CMOS output
	InitUART();
	InitTM3();
	Init_PWM_R();
 	Init_PWM_GB();
 	
// 	P3_3=0;

	ESPt=0;
	while(!wifi_flag&&ESPt<=15);

	if(!wifi_flag)
	{
		Send_STR("AT+CWJAP_DEF=","");	//Setting WiFi
		Send_STR("\"","");
		Send_STR(AP,"");
		Send_STR("\",","");
		Send_STR("\"","");
		Send_STR(AP_pwd,"");
		Send_STR("\"","");
		Send_STR("\r\n","OK");
		ESPt=0;
		while(!wifi_flag&&ESPt<=15);
	}

	while(!wifi_flag);

	if(wifi_flag)
	{
		ESPt=0;
		while(ESPt<=10);
		Send_STR(ATE,"OK");
		ESPt=0;
		while(ESPt<=5);
		Send_STR(CWMODE_DEF,"OK");	//STA mode 
		ESPt=0;
		while(ESPt<=5);
		Send_STR(CIPMUX,"OK");
		ESPt=0;
		while(ESPt<=5);
		Send_STR(CIPSERVER,"OK");
		ESPt=0;
		while(ESPt<=5);
		Send_STR(CIFSR,"OK");
		ESPt=0;
		while(ESPt<=5);	
		Send_STR(CIPSTAMAC_CUR,"OK");
		ESPt=0;
		while(ESPt<=5);
	}


//	Send_STR("AT+CIPSTA_CUR?\r\n","OK");
//	while(SEC<=3)
//	Send_STR("AT+CWMODE_DEF=1\r\n","OK");	//STA mode 
//	while(SEC<=6);
//	Send_STR("AT+CIPMUX=1\r\n","OK");
//	while(SEC<=9);
//	Send_STR("AT+CIPSERVER=1,3333\r\n","OK");	
//	Init_ESP8266();
	
    while(1)
    {    	
    	/*
    	if(!wifi_flag)
    	{
    		Send_STR("AT+CWJAP_DEF=","");	//Setting WiFi
			Send_STR("\"","");
			Send_STR(AP,"");
			Send_STR("\",","");
			Send_STR("\"","");
			Send_STR(AP_pwd,"");
			Send_STR("\"","");
			Send_STR("\r\n","OK");
    	}
    	*/
    	while(!wifi_flag);

    	if(!mac_flag)
	    {
	    	Send_STR(CIPSTAMAC_CUR,"OK");
	    }

	    if(!IP_flag&&wifi_flag)
	    {
	    	Send_STR(CIFSR,"OK");
//	    	ESPt=0;
//			while(ESPt<=3);	
	    }


    

	    if((!Remv_flag)&&IP_flag&&(!search_flag))
	    {

			if((!STATUS_H)&&STATUS_L&&SEC>=20)	//first_flag=01(binary)
			{  
			    SEC=0;
			    Send_flag=1;
			    if(park_flag)
			    {	        
			        Send_STR(CIPSTART_Gen,"");
					Send_STR(IP_add,"");
					Send_STR("\",","");
					Send_STR(PORT,"");
					Send_STR("\r\n","SUCCESS");
					/*
			        InitNOP();
			        InitNOP();
			        InitNOP();
			        InitNOP();
			        InitNOP();
			        */
					if(TCP_flag)
			        {
//				        P3_3=0;
				        Send_STR(CIPSENDEX,">");
				        InitNOP();
				        Send_STR("IP:","");
				        Send_STR(Pip,"");				        
				        Send_STR(",","");
				        Send_STR("MAC:","");
				        Send_STR(Mac,"");
						Send_STR(Add_OCC_Gen,"");
						Send_STR(DMg,"");
						Send_STR(Add_OCC_Gen2,"");
						Send_STR(AID,"");
						Send_STR(Add_OCC_Gen3,"");
				        InitNOP();				        
				        Send_STR("\\0","OK");
				        /*
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        */
				        Send_STR(CIPCLOSE,"OK");
				        
			        }
			        else
			        {
			        	STATUS_H=0;
			        	STATUS_L=0;
			        }
//			        Send_STR(CIPCLOSE,"OK");
			    }
			    else 
			    {   
			        Send_STR(CIPSTART_Gen,"");
					Send_STR(IP_add,"");
					Send_STR("\",","");
					Send_STR(PORT,"");
					Send_STR("\r\n","SUCCESS");
					/*
			        InitNOP();			        
			        InitNOP();
			        InitNOP();
			        InitNOP();
			        InitNOP();
			        */
			        if(TCP_flag)
			        {
//				        P3_3=0;
				        Send_STR(CIPSENDEX,">");
				        InitNOP();
				        Send_STR("IP:","");
				        Send_STR(Pip,"");
				        Send_STR(",","");
				        Send_STR("MAC:","");
				        Send_STR(Mac,"");
				        Send_STR(Add_NO_Gen,"");
						Send_STR(DMg,"");
						Send_STR(Add_NO_Gen2,"");
						Send_STR(AID,"");
						Send_STR(Add_NO_Gen3,"");
				        InitNOP();
				        
				     	Send_STR("\\0","OK");
				     	/*
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        */
				        Send_STR(CIPCLOSE,"OK");		        
			        }
			        else
			        {
			        	STATUS_H=0;
			        	STATUS_L=0;
			        }        
			    }
			    Send_flag=0;
//			    SEC=0;
			}			
			else if(STATUS_H&&modify_flag)	//first_flag=10(binary)
			{		
				Send_flag=1;
				if(park_flag)	//if parking status changed inform arduino server
			    {
			        Send_STR(CIPSTART_Gen,"");
					Send_STR(IP_add,"");
					Send_STR("\",","");
					Send_STR(PORT,"");
					Send_STR("\r\n","SUCCESS");
					/*
			    	InitNOP();
			        InitNOP();
			        InitNOP();
			        InitNOP();
			        InitNOP();
			        */
			    	if(TCP_flag)
			    	{
				    	Send_STR(CIPSENDEX,">");
				    	InitNOP();
				    	Send_STR(State_OCC_Gen,"");
						Send_STR(PM,"");
						Send_STR(State_OCC_Gen2,"");
						Send_STR(AID,"");
						Send_STR(State_OCC_Gen3,"");
				        InitNOP();
				        Send_STR("\\0","OK");
				        /*
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        */
				        Send_STR(CIPCLOSE,"OK");
				        modify_flag=0;
				        Send_flag=0;
			    	}
//			    	Send_STR(CIPCLOSE,"OK");
			    }
			    else
			    {
			        Send_STR(CIPSTART_Gen,"");
					Send_STR(IP_add,"");
					Send_STR("\",","");
					Send_STR(PORT,"");
					Send_STR("\r\n","SUCCESS");
					/*
			    	InitNOP();
			        InitNOP();
			        InitNOP();
			        InitNOP();
			        InitNOP();
			        */
			    	if(TCP_flag)
			    	{
				    	Send_STR(CIPSENDEX,">");
		    			InitNOP();
						Send_STR(State_NO_Gen,"");
						Send_STR(PM,"");
						Send_STR(State_NO_Gen2,"");
						Send_STR(AID,"");
						Send_STR(State_NO_Gen3,"");
				        InitNOP();
				        Send_STR("\\0","OK");
				        /*
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        InitNOP();
				        */
				        Send_STR(CIPCLOSE,"OK");
				        modify_flag=0; 
				        Send_flag=0;       
				    }
//				    Send_STR(CIPCLOSE,"OK");
			    }
			    
			}
		}
	
	}
		
}



