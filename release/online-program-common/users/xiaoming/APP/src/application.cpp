#if 0
#include "application.h"		//»ù´¡Í·ÎÄ¼þ


#include "my_led_test.h"		//ÓÃ»§Í·ÎÄ¼þ²âÊÔ


#include "DHT22_lib_usr_test1/DHT22_lib_usr_test1.h"		//ÓÃ»§¿â1Í·ÎÄ¼þ²âÊÔ
#include "DHT33_lib_usr_test2/DHT33_lib_usr_test2.h"		//ÓÃ»§¿â2Í·ÎÄ¼þ²âÊÔ


#include "DHT00_lib_public_test1/DHT00_lib_public_test1.h"		//¹«¹²¿â1Í·ÎÄ¼þ²âÊÔ
#include "DHT11_lib_public_test2/DHT11_lib_public_test2.h"		 //¹«¹²¿â2Í·ÎÄ¼þ²âÊÔ

SYSTEM_MODE(MODE_AUTOMATIC)



//=========================================================================

#define CMD_LIGHTSWITCH  "channel/smartLight_0/cmd/switch"
#define DATA_LIGHTSTATE  "channel/smartLight_0/data/status"

const int led = D7;


void LightSwitchCb(uint8_t * payload, uint32_t len)
{
    SerialUSB.println("LightSwitchCb");
    if(!memcmp(payload,"1",1))
    {
        IntoRobot.printf("TIME_Compile:%s \n",__TIME__);
        digitalWrite(led, HIGH);
        IntoRobot.publish(DATA_LIGHTSTATE, (uint8_t *)"1", 1);

    }
    else if(!memcmp(payload,"0",1))
    {
         IntoRobot.printf("TIME_Compile:%s \n",__TIME__);
        digitalWrite(led, LOW);
        IntoRobot.publish(DATA_LIGHTSTATE, (uint8_t *)"0", 1);
    }
}
//=========================================================================




void setup()
{


//===========================
    pinMode(led, OUTPUT);
    IntoRobot.subscribe(CMD_LIGHTSWITCH, NULL, LightSwitchCb);
//===========================






	int test=VAR_TEST;		//ÓÃ»§Í·ÎÄ¼þ²âÊÔ
	test++;
	setup_DHT22_lib_usr_test1();
	setup_DHT33_lib_usr_test2();

	setup_DHT00_lib_public_test1();
	setup_DHT00_lib_public_test1();
}

void loop()
{
	loop_DHT22_lib_usr_test1();
	loop_DHT33_lib_usr_test2();

	loop_DHT00_lib_public_test1();
	loop_DHT00_lib_public_test1();
}
#endif


#if 0

#include "application.h"

//light
//cmd
#define CMD_LIGHTSWITCH "channel/lightSwitch_0/cmd/switch"           //light switch
//data
#define DATA_LIGHTSTATE  "channel/lightSwitch_0/data/light"          //light feedback

//core
//cmd
#define CMD_GRAPH_TOTALSETPINMODE "channel/coreCtrl/cmd/mode_bak"    //get the total pin mode
#define CMD_GRAPH_SETPINMODE "channel/coreCtrl/cmd/mode"             //get the total pin mode

#define CMD_GRAPH_TOTALSETPINDATA "channel/coreCtrl/cmd/write_bak"   //get the total pin set value
#define CMD_GRAPH_SETPINDATA "channel/coreCtrl/cmd/write"            //get the total pin mode

#define CMD_GRAPH_TOTALPINFBDATA "channel/coreCtrl/cmd/getread"      //get the total pin feedback value
//data
#define DATA_GRAPH_PINFBDATA "channel/coreCtrl/data/read"            //realtime changed value

#define DIGITMAXNUM      16
#define ANALOGMAXNUM     25

#define ANALOGDIFF       20


//digit pin mode
u8 DMode[DIGITMAXNUM]={0};
//digit pin data last
s32 DDataLast[DIGITMAXNUM]={0};

//analog pin mode
u8 AMode[ANALOGMAXNUM]={0};
//analog pin data last
s32 ADataLast[ANALOGMAXNUM]={0};

aJsonClass aJson;

void LightSwitchCb(uint8_t * payload, uint32_t len);
void CmdGraphTotalSetPinModeCb(uint8_t * payload, uint32_t len);
void CmdGraphSetPinModeCb(uint8_t * payload, uint32_t len);
void CmdGraphTotalSetPinDataCb(uint8_t * payload, uint32_t len);
void CmdGraphSetPinDataCb(uint8_t * payload, uint32_t len);
void CmdGraphTotalPinFbDataCb(uint8_t * payload, uint32_t len);
void GetPinFeedbackData(bool total);

void LightSwitchCb(uint8_t * payload, uint32_t len)
{
    SerialUSB.println("LightSwitchCb");
    if(!memcmp(payload,"1",1))
    {
        digitalWrite(13, HIGH);// Turn ON the LED pins
        IntoRobot.publish(DATA_LIGHTSTATE, (uint8_t *)"1", 1);

    }
    else if(!memcmp(payload,"0",1))
    {
        digitalWrite(13, LOW);// Turn ON the LED pins
        IntoRobot.publish(DATA_LIGHTSTATE, (uint8_t *)"0", 1);
    }
}

void CmdGraphTotalSetPinModeCb(uint8_t * payload, uint32_t len)
{
    CmdGraphSetPinModeCb(payload,len);
    IntoRobot.unsubscribe(CMD_GRAPH_TOTALSETPINMODE, NULL);
    IntoRobot.subscribe(CMD_GRAPH_TOTALSETPINDATA, NULL, CmdGraphTotalSetPinDataCb);
}

void CmdGraphSetPinModeCb(uint8_t * payload, uint32_t len)
{
    u32 i,num,pinnum;
    String tmp;

    for(i=0; i<len; i++)
    {tmp+=(char)payload[i];}

    aJsonObject *root = aJson.parse((char *)tmp.c_str());
    if (root == NULL)
    {return;}

    aJsonObject* Item_Object;
    num=aJson.getArraySize(root);
    for(i=0; i<num; i++)
    {
        Item_Object=aJson.getArrayItem(root, i);
        if (Item_Object == NULL)
        {return;}
        pinnum=atoi(&Item_Object->name[1]);

        if(Item_Object->valueint>4)
        {continue;}
        switch(Item_Object->name[0])
        {
            case 'D':
                if(pinnum>=DIGITMAXNUM)
                {continue;}
                DMode[pinnum]=Item_Object->valueint;
                if((Item_Object->valueint!=1)&&(Item_Object->valueint!=3))
                {
                    DDataLast[pinnum]=-1;
                }
                break;
            case 'A':
                if(pinnum>=ANALOGMAXNUM)
                {continue;}
                AMode[pinnum]=Item_Object->valueint;
                if((Item_Object->valueint!=1)&&(Item_Object->valueint!=3))
                {
                    ADataLast[pinnum]=-1;
                }
                pinnum+=30;
                break;
            default:
                continue;
                break;
        }

        switch(Item_Object->valueint)
        {
            case 1: //digit read
                pinMode(pinnum,INPUT_PULLDOWN);
                break;
            case 3: //analog read
                pinMode(pinnum,AN_INPUT);
                break;
            case 2: //digit write
            case 4: //analog write
            default:
                pinMode(pinnum,OUTPUT);
                break;
        }
    }
    aJson.deleteItem(root);
}

void CmdGraphTotalSetPinDataCb(uint8_t * payload, uint32_t len)
{
    CmdGraphSetPinDataCb(payload, len);
    IntoRobot.unsubscribe(CMD_GRAPH_TOTALSETPINDATA, NULL);
    IntoRobot.subscribe(CMD_GRAPH_SETPINMODE, NULL, CmdGraphSetPinModeCb);
    IntoRobot.subscribe(CMD_GRAPH_SETPINDATA, NULL, CmdGraphSetPinDataCb);
}

void CmdGraphSetPinDataCb(uint8_t * payload, uint32_t len)
{
    u32 i,num,pinnum;
    String tmp;

    for(i=0; i<len; i++)
    {tmp+=(char)payload[i];}

    aJsonObject *root = aJson.parse((char *)tmp.c_str());
    if (root == NULL)
    {return;}

    aJsonObject* Item_Object;
    num=aJson.getArraySize(root);
    for(i=0; i<num; i++)
    {
        Item_Object=aJson.getArrayItem(root, i);
        if (Item_Object == NULL)
        {return;}
        pinnum=atoi(&Item_Object->name[1]);
        switch(Item_Object->name[0])
        {
            case 'D':
                if(pinnum>=DIGITMAXNUM)
                {continue;}
                if(Item_Object->valueint>1)
                {continue;}
                if(DMode[pinnum]==2)       //digit write
                {
                    digitalWrite(pinnum, Item_Object->valueint);
                }
                else if(DMode[pinnum]==4)  //analog write
                {
                    analogWrite(pinnum, Item_Object->valueint);
                }
                break;
            case 'A':
                if(pinnum>=ANALOGMAXNUM)
                {continue;}
                if(Item_Object->valueint>255)
                {continue;}
                if(AMode[pinnum]==2)       //digit write
                {
                    analogWrite(pinnum+30, Item_Object->valueint);
                }
                else if(AMode[pinnum]==4)  //analog write
                {
                    analogWrite(pinnum+30, Item_Object->valueint);
                }
                break;
            default:
                continue;
                break;
        }
    }
    aJson.deleteItem(root);
}

void CmdGraphTotalPinFbDataCb(uint8_t * payload, uint32_t len)
{
    GetPinFeedbackData(true);
}

void GetPinFeedbackData(bool total)
{
    char temp[16];
    int i;
    s32 value,diff;

    aJsonObject* root = aJson.createObject();
    if (root == NULL)
    {return;}

    //digit
    for(i=0; i<DIGITMAXNUM; i++)
    {
        switch(DMode[i])
        {
            case 1://digit read
                value=digitalRead(i);
                if((total==true)||(value!=DDataLast[i]))
                {
                    DDataLast[i]=value;
                    memset(temp,0,sizeof(temp));
                    sprintf(temp,"D%d",i);
                    aJson.addNumberToObject(root, temp, (int)value);
                }
                break;
            default:
                break;
        }
    }
    //analog
    for(i=0; i<ANALOGMAXNUM; i++)
    {
        switch(AMode[i])
        {
            case 1: //digit read
                value=digitalRead(i+30);
                if((total==true)||(value!=ADataLast[i]))
                {
                    ADataLast[i]=value;
                    memset(temp,0,sizeof(temp));
                    sprintf(temp,"A%d",i);
                    aJson.addNumberToObject(root, temp, (int)value);
                }
                break;

            case 3: //analog read
                value=analogRead(i+30);
                if(value>ADataLast[i])
                {diff=value-ADataLast[i];}
                else
                {diff=ADataLast[i]-value;}
                if((total==true)||(diff>ANALOGDIFF))
                {
                    ADataLast[i]=value;
                    memset(temp,0,sizeof(temp));
                    sprintf(temp,"A%d",i);
                    aJson.addNumberToObject(root, temp, (int)value);
                }
                break;
            default:
                break;
        }
    }

    if(aJson.getArraySize(root))
    {
        char* string = aJson.print(root);
        IntoRobot.publish(DATA_GRAPH_PINFBDATA, (uint8_t *)string,strlen(string));
        free(string);
    }
    aJson.deleteItem(root);
}


void setup()
{
    s32 i=0;

    //-1 initial status
    for(i=0; i<DIGITMAXNUM; i++)
    {DDataLast[i]=-1;}

    for(i=0; i<ANALOGMAXNUM; i++)
    {ADataLast[i]=-1;}

    IntoRobot.subscribe(CMD_GRAPH_TOTALSETPINMODE, NULL, CmdGraphTotalSetPinModeCb);
    IntoRobot.subscribe(CMD_LIGHTSWITCH, NULL, LightSwitchCb);
}


void loop()
{
    GetPinFeedbackData(false);
    delay(1000);
}

#endif

#if 0
#include "IntoRobot_DHT.h"

//¿¿¿¿¿¿topic¿¿
#define HUMITURE_DATA_TEMPERATURE    "channel/humiture_0/data/temperature"   //¿¿¿¿
#define HUMITURE_DATA_HUMIDITY       "channel/humiture_0/data/humidity"      //¿¿¿¿
#define DHT11_PIN   D0
#define DHT_TYPE    DHT11

IntoRobot_DHT dht11(DHT11_PIN,DHT_TYPE);

void setup()
{
    //¿¿¿
   	dht11.begin();
}

void loop()
{
	IntoRobot.publish(HUMITURE_DATA_TEMPERATURE, dht11.getTempCelcius()); //¿¿¿¿¿
	IntoRobot.publish(HUMITURE_DATA_HUMIDITY, dht11.getHumidity());        //¿¿¿¿¿
	delay(1000);
}
#endif



#if 1
#include "application.h"
void setup()
{
    pinMode(D7, OUTPUT);
}
void loop()
{
    digitalWrite(D7, HIGH);
    delay(1000);
    digitalWrite(D7, LOW);
    delay(1000);
}
#endif

