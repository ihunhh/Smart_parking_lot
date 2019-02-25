#include <Bridge.h>
#include <BridgeClient.h>
#include <BridgeServer.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 8087
#define PORT_PC 5987
//#define PORT_PC 8086
#define NUM_CLIENTS 6
//#define LIST 5
#define PORT_PARK 3333
#define PM "BM"
#define AID "Z1"
#define PiPID "B"

#define PARK_P1 12
#define PARK_P2 13
#define PARK_P3 11
#define PARK_P4 10
#define PARK_P5 7

#define a_seg 2
#define b_seg 3
#define c_seg 4
#define d_seg 5
#define e_seg 6
#define f_seg 9
#define g_seg 8


typedef struct Park_info{ 
  byte ip[4];
  byte mac[6];
  char ID[6];
  struct Park_info* link;
}Pinfo;

typedef struct linklist_par{
  Pinfo* END;
}LINKLIST_PAR;

/*
  a-2
  b-3
  c-4
  d-5
  e-6
  g-8
  f-9
*/

void process(byte );
void catch_Info(char* );
void pri_info(LINKLIST_PAR );
char* Pillar_Info();


BridgeServer server(PORT);

BridgeClient clients[NUM_CLIENTS];

BridgeClient client_connect;

boolean pillar_reg_flag=0;
boolean remv_flag=0;
//char serial_ptr[3]={0};
//char count=0;
//byte PID=0;
//IPAddress server_PC(172, 20, 10, 14);
//IPAddress server_PC(192, 168, 43, 14);
//IPAddress server_PC(192, 168, 43, 94);
IPAddress server_PC(192, 168, 0, 100);
LINKLIST_PAR PARK;
//Process wifiCheck;
/*
  int PARK_P1=12;
  int PARK_P2=13;
  int a_seg=2;
  int b_seg=3;
  int c_seg=4;
  int d_seg=5;
  int e_seg=6;
  int f_seg=9;
  int g_seg=8;
*/
//Pinfo clients_info[LIST];

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Starting bridge...\n");
  Bridge.begin();
  server.begin();
//  delay(3000);
  client_connect.setTimeout(500);
//  client_connect.setTimeout(200);



 
  Serial.print("MULTIPLE TCP CONNECTION SERVER FOR ");
  Serial.print(NUM_CLIENTS);
  Serial.print(" CLIENTS");
  Serial.println(", PORT:8087");
  Serial.println("");




  pinMode(PARK_P1, OUTPUT);
  pinMode(PARK_P2, OUTPUT);
  pinMode(PARK_P3, OUTPUT); 
  pinMode(PARK_P4, OUTPUT); 
  pinMode(PARK_P5, OUTPUT);

  pinMode(a_seg, OUTPUT);
  pinMode(b_seg, OUTPUT);
  pinMode(c_seg, OUTPUT);
  pinMode(d_seg, OUTPUT);
  pinMode(e_seg, OUTPUT);
  pinMode(f_seg, OUTPUT);
  pinMode(g_seg, OUTPUT);

  digitalWrite(a_seg,HIGH);
  digitalWrite(b_seg,HIGH);
  digitalWrite(c_seg,LOW);
  digitalWrite(d_seg,LOW);
  digitalWrite(e_seg,LOW);
  digitalWrite(f_seg,LOW);
  digitalWrite(g_seg,LOW);



/* 
  for(byte i=0;i<LIST;i++)
  {
    strstr(clients_info[i].ID,"NULL");
  }
*/
  while(!pillar_reg_flag)
  {
    if(client_connect.connect(server_PC, PORT_PC))
    { 
      client_connect.print("{\"Data\":{\"MT\":\"Add\",\"DT\":\"Pillar\",\"PM\":\"");
      client_connect.print(PM);
      client_connect.print("\",\"AID\":\"");
      client_connect.print(AID); 
      client_connect.print("\",\"PPID\":\"");
      client_connect.print(PiPID);
      client_connect.print("\"}}");
      client_connect.stop();
      Serial.println("REGISTING PILLAR FINISH");
      Serial.println("");
      pillar_reg_flag=1;
    }

    else
    {
      Serial.println("CONNECTING PC SERVER FAIL!!!");
      Serial.println("");
    }
  
  }


  PARK.END=NULL;

}

void loop()
{
//  count++;
  if(!remv_flag)
  {    
    while(!pillar_reg_flag)
    {
      if(client_connect.connect(server_PC, PORT_PC))
      {
        client_connect.print("{\"Data\":{\"MT\":\"Add\",\"DT\":\"Pillar\",\"PM\":\"");
        client_connect.print(PM);
        client_connect.print("\",\"AID\":\"");
        client_connect.print(AID); 
        client_connect.print("\",\"PPID\":\"");
        client_connect.print(PiPID);
        client_connect.print("\"}}");
        client_connect.stop();
        Serial.println("REGISTING PILLAR FINISH");
        Serial.println("");
        pillar_reg_flag=1;
      }    
  //    count=0;
    }

    for(byte i=0;i<NUM_CLIENTS;i++)
    {
      if(clients[i].connected())
        process(i);
      else
      {
        if((bool)clients[i])
        {
          clients[i].stop();
          Serial.print("LOST CONNECTION TO ");
          Serial.print("CLIENT");
          Serial.print("[");
          Serial.print(i);
          Serial.println("]");
          Serial.println("");
        }
        clients[i]=server.accept();
        if(clients[i].connected())
        {
          Serial.print("TCP CONNECTION SUCCESS OF ");
          Serial.print("CLIENT");
          Serial.print("[");
          Serial.print(i);
          Serial.println("]");
          Serial.println("");
          clients[i].print("SUCCESS\n");   
        }
/*              
        if(Serial.available())
        {
          (Serial.readStringUntil('\n')).toCharArray(serial_ptr,3);
          if(strstr(serial_ptr,"Wc")!=NULL)
          {
            wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");
            while (wifiCheck.available() > 0) 
            {
              char c = wifiCheck.read();
              Serial.print(c);
            }
            Serial.println();
          }
        }
*/             
      }
    }
  /*  
    for(byte i=0;i<PID;i++)
    {
      Serial.println(clients_info[i].ID);
      Serial.println(clients_info[i].ID);
    }
  */
  }
}

void process(byte id)
{
  char str[120]={0};
  //  char* temp=NULL;
  //  char ID_temp[5]={0};
  char LON[15]="LON";
  boolean sendToserver_flag=0;
  //  byte ip_temp[4]={0};
  Pinfo* cur;

  if(clients[id].available())
  { 
    (clients[id].readStringUntil('\n')).toCharArray(str,120);
  
    Serial.println(str);
  
    if(strstr(str,"Add")!=NULL)
    {
      Serial.println("MISSIONT:Add");
      Serial.println("");
      catch_Info(str);
  //      for(byte i=0;i<PID;i++)
    
        /*
        Serial.print("CONNECTING TO ");
        Serial.print(cur->ID);
        Serial.println("...");
        Serial.println("");
        */
      if(strstr(str,"NOc")!=NULL)
      { 
        if(strstr(str,"1M")!=NULL)
        {
          digitalWrite(PARK_P1,HIGH);
        }
        else if(strstr(str,"2M")!=NULL)
        {
          digitalWrite(PARK_P2,HIGH);
        }
        else if(strstr(str,"3M")!=NULL)
        {
          digitalWrite(PARK_P3,HIGH);
        }
        else if(strstr(str,"4M")!=NULL)
        {
          digitalWrite(PARK_P4,HIGH);
        }
        /*
        else if(strstr(str,"5M")!=NULL)
        {
          digitalWrite(PARK_P5,HIGH);
        }
        */
      }
      else if(strstr(str,"Oc")!=NULL)
      {  
        if(strstr(str,"1M")!=NULL)
        {
          digitalWrite(PARK_P1,LOW);
        }
        else if(strstr(str,"2M")!=NULL)
        {
          digitalWrite(PARK_P2,LOW);
        }
        else if(strstr(str,"3M")!=NULL)
        {
          digitalWrite(PARK_P3,LOW);
        }
        else if(strstr(str,"4M")!=NULL)
        {
          digitalWrite(PARK_P4,LOW);
        }
        /*
        else if(strstr(str,"5M")!=NULL)
        {
          digitalWrite(PARK_P5,LOW);
        }
        */             
      }

      cur=PARK.END; 
      if(client_connect.connect(server_PC, PORT_PC))
      {
        client_connect.print(strchr(str,'{'));
        client_connect.stop();
        Serial.println(strchr(str,'{'));
        Serial.print("REGISTING ");
        Serial.print(cur->ID);
        Serial.println(" TO PC SERVER FINISHED!!!");
        Serial.println("");
        while(cur)
        {
          if(strstr(str,cur->ID)!=NULL)
          {   
            if(client_connect.connect(cur->ip,PORT_PARK))
            {
              client_connect.print("ADDS\n");
              client_connect.stop();
              Serial.print("FEEDBACK ");
              Serial.print(cur->ID);
              Serial.println(" FINISHED!!!");
              Serial.println("");
              break;
            }
          }
          cur=cur->link;  
        }
      }
      else
      {
        while(cur)
        {
          if(strstr(str,cur->ID)!=NULL)
          {   
            if(client_connect.connect(cur->ip,PORT_PARK))
            {
              client_connect.print("ADDF\n");
              client_connect.stop();
              Serial.print("FEEDBACK ");
              Serial.print(cur->ID);
              Serial.println(" FAIL!!!");
              Serial.println("");
              break;
            }
          }
          cur=cur->link;  
        }     
      } 
     
    }  
    else if(strstr(str,"\"SU\"")!=NULL)
    {
      Serial.println("MISSION:StateUpdate");
      Serial.println("");
      /*
      Serial.println("CONNECTING TO PC SERVER...");
      Serial.println("");
      */
      /*
      sendToserver_flag=1;
      while(sendToserver_flag)
      {
        if(client_connect.connect(server_PC, PORT_PC))
        {
          client_connect.print(str);
          client_connect.stop();
          sendToserver_flag=0;
          Serial.println(str);
          Serial.println("UPDATE STATUS FINISHED!!!");
          Serial.println("");
        }
      }
      */

      if(strstr(str,"NOc")!=NULL)
      {
        if(strstr(str,"1M")!=NULL)
        {
          digitalWrite(PARK_P1,HIGH);
        }
        else if(strstr(str,"2M")!=NULL)
        {
          digitalWrite(PARK_P2,HIGH);
        }
        else if(strstr(str,"3M")!=NULL)
        {
          digitalWrite(PARK_P3,HIGH);
        }
        else if(strstr(str,"4M")!=NULL)
        {
          digitalWrite(PARK_P4,HIGH);
        }
        /*
        else if(strstr(str,"5M")!=NULL)
        {
          digitalWrite(PARK_P5,HIGH);
        }
        */
      }
      else if(strstr(str,"Oc")!=NULL)
      {
        if(strstr(str,"1M")!=NULL)
        {
          digitalWrite(PARK_P1,LOW);
        }
        else if(strstr(str,"2M")!=NULL)
        {
          digitalWrite(PARK_P2,LOW);
        }
        else if(strstr(str,"3M")!=NULL)
        {
          digitalWrite(PARK_P3,LOW);
        }
        else if(strstr(str,"4M")!=NULL)
        {
          digitalWrite(PARK_P4,LOW);
        }
        /*
        else if(strstr(str,"5M")!=NULL)
        {
          digitalWrite(PARK_P5,LOW);
        }
        */          
      }
      sendToserver_flag=1;
      while(sendToserver_flag)
      {
        if(client_connect.connect(server_PC, PORT_PC))
        {
          client_connect.print(str);
          client_connect.stop();
          sendToserver_flag=0;
          Serial.println(str);
          Serial.println("UPDATE STATUS FINISHED!!!");
          Serial.println("");
        }
      }
    }  
    else if(strstr(str,"search")!=NULL)
    {
      Serial.println("MISSION:search");
      Serial.println("");
  //      for(byte i=0;i<PID;i++)

      cur=PARK.END;
      while(cur)
      {
        if(strstr(str,cur->ID)!=NULL)
        {
          if(strstr(str,"ON")!=NULL)
          { 
            Serial.print("SEARCHING CAR INDICATOR TURN ON ID:");
            Serial.print(cur->ID);
            Serial.println("...");
            Serial.println("");
            /*
            Serial.print("CONNECTING TO CAR PARK ID:");
            Serial.print(cur->ID);
            Serial.println("...");
            Serial.println("");
            */
            if(client_connect.connect(cur->ip,PORT_PARK))
            {
              /*
              Serial.print(cur->ID);
              Serial.println(" CONNECTED");
              Serial.println("");
              */
              strcat(LON,strchr(str,'#'));
              strcat(LON,"\n");
              client_connect.print(LON);
              client_connect.stop();
              /*
              Serial.print("SENDING COMMAND:");
              Serial.println(strcat(strcat(LON,strchr(str,'#')),"\n"));
              Serial.println("");
              */
              Serial.print(cur->ID);
              Serial.println(" TURNED ON FINISHED!!!");
              Serial.println("");
  //              Serial.print(strcat(strcat("LON",strchr(str,'#')),'\n'));
            }
            break;
          }
          else if(strstr(str,"OFF")!=NULL)
          {
            Serial.print("SEARCHING CAR INDICATOR OFF ID:");
            Serial.print(cur->ID);
            Serial.println("...");
            Serial.println("");
            /*
            Serial.print("CONNECTING TO CAR PARK ID:");
            Serial.print(cur->ID);
            Serial.println("...");
            Serial.println("");
            */
            if(client_connect.connect(cur->ip,PORT_PARK))
            {
              /*
              Serial.print(cur->ID);
              Serial.println(" CONNECTED");
              Serial.println("");
              */
              client_connect.print("LOFF\n");
              client_connect.stop();
              Serial.print(cur->ID);
              Serial.println(" TURNED OFF FINISHED!!!");
              Serial.println("");
            }
            break;
          }
        }
        cur=cur->link;
      }
    }
    else if(strstr(str,"Delete")!=NULL)
    {
      Serial.println("MISSION:Delete");
      Serial.println("");

      cur=PARK.END;
      if(strstr(str,PM)!=NULL)
      {
        remv_flag=1;
        digitalWrite(PARK_P1,LOW);
		digitalWrite(PARK_P2,LOW);
		digitalWrite(PARK_P3,LOW);
		digitalWrite(PARK_P4,LOW);

		digitalWrite(a_seg,HIGH);
		digitalWrite(b_seg,HIGH);
		digitalWrite(c_seg,HIGH);
		digitalWrite(d_seg,HIGH);
		digitalWrite(e_seg,HIGH);
		digitalWrite(f_seg,HIGH);
		digitalWrite(g_seg,HIGH);
        while(cur)
        {
          if(client_connect.connect(cur->ip,PORT_PARK))
          {
            /*
            Serial.print(cur->ID);
            Serial.println(" CONNECTED");
            Serial.println("");
            */
            client_connect.print("REMV\n");
            client_connect.stop();
            Serial.print(cur->ID);
            Serial.println(" REMOVED FINISHED!!!");
            Serial.println("");
            cur=cur->link;
          }
        }
      }
      else
      {
        while(cur)
        {
          if(strstr(str,cur->ID)!=NULL)
          {
            /*
            Serial.print("CONNECTING TO CAR PARK ID:");
            Serial.println(cur->ID);
            Serial.println("...");
            Serial.println("");
            */
            if(client_connect.connect(cur->ip,PORT_PARK))
            {
              /*
              Serial.print(cur->ID);
              Serial.println(" CONNECTED");
              Serial.println("");
              */
              client_connect.print("REMV\n");
              client_connect.stop();
              Serial.print(cur->ID);
              Serial.println(" REMOVED FINISHED!!!");
              Serial.println("");
            }
            break;
          }
          cur=cur->link;
        }
      }

    }
    else if(strstr(str,"message")!=NULL)
    {
      if (strstr(str,"AddSuccess")!=NULL)
      {
        Serial.println("Add SUCCESS!!!");
      }
      else if(strstr(str,"Usuccess")!=NULL)
      {
        Serial.println("StateUpdate SUCCESS!!!");
      }
      else if(strstr(str,"Fail")!=NULL)
      {
        Serial.println("Mission FAIL!!!");
      }
    }
  }
}

/*
byte search(char* ptr)
{
  for(byte i=0;i<NUM_CLIENTS;i++)
  {
    if(ID[i]==ptr)
    {
      return i;
      break;
    }
  }
}
*/
/*
void catch_Info(char* str)
{
    char* temp;
    char* temp_ip;
    char ID_temp[50]={0};
    byte ip_temp[4]={0};
    byte j=0;
    temp=strstr(str,"DMg");
    for(byte i=0;i<=3;i++)
    {
      temp=strchr(temp,':')+1;
    }
    strcpy(ID_temp,temp);
    while(ID_temp[j]!='\"')
      j++;
    ID_temp[j]='\0';

    if(strstr(str,"IP:")!=NULL)
    {
      temp_ip=strstr(str,"IP:");
      temp_ip=strchr(str,':')+1;
      ip_temp[0]=atoi(temp_ip);
      for(byte m=1;m<=3;m++)
      {
          temp_ip=strchr(temp_ip,'.')+1;
          ip_temp[m]=atoi(temp_ip);
      }
    }
    if(!PID)
    {
      strcpy(clients_info[PID].ID,ID_temp);
      for(byte l=0;l<4;l++)
      {
        clients_info[PID].ip[l]=ip_temp[l];
      }
      PID++;
    }
    else
    {
      for(byte k=0;k<PID;k++)
      {
        if(strstr(ID_temp,clients_info[k].ID)!=NULL)
        {
          for(byte l=0;l<4;l++)
          {
            clients_info[k].ip[l]=ip_temp[l];
          }
        }
        else if(k==(PID-1))
        {
          if(strstr(ID_temp,clients_info[k].ID)!=NULL)
          {
            for(byte l=0;l<4;l++)
            {
              clients_info[k].ip[l]=ip_temp[l];
            }
          }
          else
          {
            strcpy(clients_info[PID].ID,ID_temp);
            for(byte l=0;l<4;l++)
            {
              clients_info[PID].ip[l]=ip_temp[l];
            }
            PID++;
          }
        }
      }
    }

}
*/
void catch_Info(char* str)
{
  char* temp;
  char* temp_ip;
  char* temp_mac;
  char ID_temp[7]={0};
  char ip_temp[4]={0};
  char mac_temp[6]={0};
  byte i; 
  Pinfo* pin_temp;
  Pinfo* now;

  temp=strstr(str,"DMg");
  for(i=0;i<=3;i++)
  {
    temp=strchr(temp,':')+1;
  }
  strncpy(ID_temp,temp,6);
  i=0;
  while(ID_temp[i]!='\"')
    i++;
  ID_temp[i]='\0';


  if(strstr(str,"IP:")!=NULL)
  {
    temp_ip=strchr(str,':')+1;
    ip_temp[0]=atoi(temp_ip);
    for(i=1;i<=3;i++)
    {
      temp_ip=strchr(temp_ip,'.')+1;
      ip_temp[i]=atoi(temp_ip);
    }
  }
  if(strstr(str,"MAC:")!=NULL)
  {
    temp_mac=strstr(str,"MAC:")+4;
    mac_temp[0]=strtol(temp_mac,NULL,16);
    for(i=1;i<=5;i++)
    {
      temp_mac=strchr(temp_mac,':')+1;
      mac_temp[i]=strtol(temp_mac,NULL,16);
    }
  }
  if(!(PARK.END))
  {
    pin_temp=(Pinfo*)malloc(sizeof(Pinfo));
    PARK.END=pin_temp;
    (PARK.END)->link=NULL;
    strcpy((PARK.END)->ID,ID_temp);
    for(i=0;i<4;i++)
    {
      (PARK.END)->ip[i]=ip_temp[i];
    }
    for(i=0;i<6;i++)
    {
      (PARK.END)->mac[i]=mac_temp[i];
    }
  }
  else
  {
    
    now=PARK.END;

    while(now)
    {
      if(strstr(now->ID,ID_temp)!=NULL)
      {
        now->ip[0]=0;
        for(i=0;i<4;i++)
        {
          now->ip[i]=ip_temp[i];
        }
        for(i=0;i<6;i++)
        {
          (PARK.END)->mac[i]=mac_temp[i];
        }
        break;
      }
      now=now->link;
    }
    if(!now)
    {
      pin_temp=(Pinfo*)malloc(sizeof(Pinfo));
      pin_temp->link=PARK.END;
      PARK.END=pin_temp;
      strcpy((PARK.END)->ID,ID_temp);
      for(i=0;i<4;i++)
      {
        (PARK.END)->ip[i]=ip_temp[i];
      }
      for(i=0;i<6;i++)
      {
        (PARK.END)->mac[i]=mac_temp[i];
      }
    }
  }    
}
/*
void pri_info(LINKLIST_PAR U)
{
  byte i;
  byte j=0;

  Pinfo* temp=U.END;
  while(temp)
  {
    Serial.print("ID[");
    Serial.print(j);
    Serial.print("]:");
    Serial.println(temp->ID);

    Serial.print("IP[");
    Serial.print(j);
    Serial.print("]:");
    for(i=0;i<=3;i++)
    {
      if(i==3)
      {
        Serial.println(temp->ip[i]);
      }
      else
      {
        Serial.print(temp->ip[i]);
        Serial.print(".");
      }           
    }
    j++;
    temp=temp->link;
  }
}
*/
/*
char* Pillar_Info()
{
  char ptr[200]="{\"Data\":{\"MT\":\"Add\",\"DT\":\"Pillar\",\"PM\":\"";
  strcat(ptr,PM);
  strcat(ptr,"\",\"AID\":\"");
  strcat(ptr,AID);
  strcat(ptr,"\",\"PPID\":\"");
  strcat(ptr,PiPID);
  strcat(ptr,"\"}}");

  return ptr;
}

*/

/*
void got_IP(char* str)
{ 
  char* temp_ip;
  byte ip_temp[4]={0};
  if(strstr(str,"IP:")!=NULL)
  {
    temp_ip=strstr(str,"IP:");
    temp_ip=strchr(str,':');
    ip_temp[0]=atoi(temp_ip);
    for(byte m=1;m<=3;m++)
    {
        temp_ip=strchr(temp_ip,'.')+1;
        ip_temp[m]=atoi(temp_ip);
    }
  }
}
*/