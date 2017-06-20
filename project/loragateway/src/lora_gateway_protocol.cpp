
#include "lora_gateway_config.h"
#include "lora_gateway_protocol.h"
#include "lora_gateway_mgt.h"
#include "lora_gateway_log.h"
#include "lora_gateway_os.h"
#include "lora_gateway_params.h"
#include "lora_gateway_ui.h"
#include "gBase64.h"
#include "sx1278-board.h"
#include "sx1278.h"


extern uint8_t gatewayIDHex[8];
#define LORA_JOB_NUM    10
osjob_t loraJob[LORA_JOB_NUM] = {0};

static int parseServerPacket(uint8_t *buffer, int bufferLen);

#ifdef CONFIG_GATEWAY_COMMUNICATION_MQTT
static void communicationCb(uint8_t *payload, uint32_t len)
{
    //log_v_dump(payload, len);
    parseServerPacket(payload, len);
}

static void mqttInitCommunication()
{
    String topic = "";
    topic += "v2/gateway/";
    topic += System.deviceID();
    topic += "/tx";

    IntoRobot.subscribe(topic.c_str(), NULL, communicationCb, 0, TOPIC_VERSION_CUSTOM);
}

static void waitMqttConnect(void)
{
    while(!IntoRobot.connected()) {
        IntoRobot.process();
        delay(1000);
    }
}

static void manageMqttConnect(void)
{
}

static bool mqttSend(uint8_t * msg, int length)
{
    String topic = "";

    topic += "v2/gateway/";
    topic += System.deviceID();
    topic += "/rx";

    //log_v_dump(msg, length);
    log_v("mqttSend = %d\r\n", length);
    if(IntoRobot.publish(topic.c_str(), msg, length, 0, 0, TOPIC_VERSION_CUSTOM)) {
        return true;
    }
    return false;
}

#else

UDP Udp;

static bool udpConnected = false;
static bool checkNetworkConnect(void)
{
#ifdef configWIRING_WIFI_ENABLE
    if (!WiFi.ready()) {
#else
    if (!Cellular.ready()) {
#endif
        return false;
    }
    return true;
}

static bool checkUdpConnect(void)
{
    return udpConnected;
}

static bool udpConnect(void)
{
    if(udpConnected) {
        return;
    }

    bool ret = false;

#ifdef configWIRING_WIFI_ENABLE
    IPAddress udpServer = WiFi.resolve(CONFIG_LORAWAN_UDP_SERVER);
#else
    IPAddress udpServer = Cellular.resolve(CONFIG_LORAWAN_UDP_SERVER);
#endif

    log_v("UDP connect: romoteIp:%s, remotePort:%d, localPort:%d\r\n", CONFIG_LORAWAN_UDP_SERVER, (uint16_t)CONFIG_LORAWAN_UDP_PORT, (uint16_t)CONFIG_LOCAL_UDP_PORT);
    if (Udp.begin(udpServer, (uint16_t)CONFIG_LORAWAN_UDP_PORT, (uint16_t)CONFIG_LOCAL_UDP_PORT)) {
        Udp.beginPacket();
        log_v("Connection successful\r\n");
        udpConnected = true;
        ret = true;
    } else {
        udpConnected = false;
        log_v("Connection failed\r\n");
    }
    return(ret);
}

static bool udpDisConnect(void)
{
    if(udpConnected)
    {
        Udp.stop();
        udpConnected = false;
    }
    return true;
}

static void waitUdpConnect(void)
{
    while(!checkUdpConnect()) {
        if(checkNetworkConnect()) {
            udpConnect();
        } else {
            udpDisConnect();
        }
        IntoRobot.process();
        delay(1000);
    }
}

static void manageUdpConnect(void)
{
    if(checkNetworkConnect()) {
        if(!checkUdpConnect()) {
            udpConnect();
        }
    } else {
        udpDisConnect();
    }
}

static bool udpSend(uint8_t * msg, int length)
{
    if(!udpConnected) {
        return;
    }

    int l;
    GatewayParams.Status.lastToken = msg[2]*256+msg[1];

    //send the update
    if ((l = Udp.write((char *)msg, length)) != length) {
        log_v("udpSend: Error write\r\n");
    } else {
        log_v("udpSend: sent %d bytes\r\n", l);
    }
    Udp.endPacket();
    return true;
}

static void dealUdpRecPackage(void)
{
    int packetSize = 0;
    uint8_t bufferDown[512] = {0};

    while((packetSize = Udp.parsePacket()) > 0)
    {
        if (packetSize > (sizeof(bufferDown) -1)) {
            Udp.flush();
            return;
        }
        Udp.read(bufferDown, packetSize);
        IPAddress remoteIpNo = Udp.remoteIP();
        uint16_t remotePortNo = Udp.remotePort();
        log_v("From %d:%d:%d:%d, port %d\r\n", remoteIpNo[0], remoteIpNo[1], remoteIpNo[2], remoteIpNo[3], remotePortNo);
        if (parseServerPacket(bufferDown, packetSize) < 0) {
            log_e("readUDP error\r\n");
        }
    }
}

#endif

void initCommunication(void)
{
#ifdef CONFIG_GATEWAY_COMMUNICATION_MQTT
    mqttInitCommunication();
#endif
}

void waitServerConnect(void)
{
#ifdef CONFIG_GATEWAY_COMMUNICATION_MQTT
    waitMqttConnect();
#else
    waitUdpConnect();
#endif
}

void manageServerConnect(void)
{
#ifdef CONFIG_GATEWAY_COMMUNICATION_MQTT
    manageMqttConnect();
#else
    manageUdpConnect();
#endif
}

void manageServerCommunication(void)
{
#ifdef CONFIG_GATEWAY_COMMUNICATION_MQTT
#else
    dealUdpRecPackage();
#endif
}

void sendPacketToServer(uint8_t * msg, int length)
{
#ifdef CONFIG_GATEWAY_COMMUNICATION_MQTT
    mqttSend(msg, length);
#else
    udpSend(msg, length);
#endif
}

// gateway lora down stream func
static void loraDownStreamFunc(osjob_t *job)
{
    if((NULL != job->data) && (NULL != job->reserved) &&job->datalen) {
        loraParamsTxConfig((GatewayLoraTxParams_t *)job->reserved);
        Radio.Send(job->data, job->datalen);
        free(job->data);
        free(job->reserved);
        job->data = NULL;
        job->reserved = NULL;
    }
    job->datalen = 0;
}

static int parseServerPacket(uint8_t *buffer, int bufferLen)
{
    uint8_t protocol;
    uint16_t token;
    uint8_t ident;
    uint8_t LoraBuffer[64];
    uint16_t LoraBufferlen;
    uint8_t buff_up[5];
    int num;

    uint8_t * data = buffer + 4;
    protocol = buffer[0];
    token = buffer[2]*256 + buffer[1];
    ident = buffer[3];

    // now parse the message type from the server (if any)
    switch (ident) {
        case PKT_PUSH_DATA: // 0x00 UP
            log_v("PKT_PUSH_DATA <server->gateway %d bytes> data:", bufferLen);
            log_v_dump(buffer, bufferLen);
            break;

        case PKT_PUSH_ACK: // 0x01 DOWN
            log_v("PKT_PUSH_ACK  <server->gateway %d bytes> token: %x\r\n", bufferLen, token);
            break;

        case PKT_PULL_DATA: // 0x02 UP
            log_v("Pull Data\r\n");
            break;

        case PKT_PULL_RESP: // 0x03 DOWN
            log_v("PKT_PULL_RESP <server->gateway %d bytes> data:", bufferLen);
            //log_v_dump(data, bufferLen);

            GatewayParams.Status.lastTmst = micros();     // Store the tmst this package was received
            // Send to the LoRa Node first (timing) and then do messaging
            LoraBufferlen = gatewayParseDownStreamPacket(LoraBuffer, data, bufferLen-4);
            if(LoraBufferlen){
                for(num = 0; num < LORA_JOB_NUM; num++) {
                    if(NULL == loraJob[num].data) {
                        loraJob[num].data = (uint8_t *)malloc(LoraBufferlen);
                        loraJob[num].reserved = (void *)malloc(sizeof(GatewayLoraTxParams_t));
                        if((NULL != loraJob[num].data)&&(NULL != loraJob[num].reserved)) {
                            loraJob[num].datalen = LoraBufferlen;
                            memcpy(loraJob[num].data, LoraBuffer, LoraBufferlen);
                            memcpy(loraJob[num].reserved, &GatewayParams.TxParams, sizeof(GatewayLoraTxParams_t));
                            os_setTimedCallback(&loraJob[num], GatewayParams.Status.sendTmst-CONFIG_TX_CORRECT_TIME, loraDownStreamFunc);
                        } else {
                            if(NULL != loraJob[num].data) {
                                free(loraJob[num].data);
                            }

                            if(NULL != loraJob[num].reserved) {
                                free(loraJob[num].reserved);
                            }
                        }
                        break;
                    }
                }
            }

            // Now respond with an PKT_PULL_ACK; 0x04 UP
            buff_up[0]=buffer[0];
            buff_up[1]=buffer[1];
            buff_up[2]=buffer[2];
            buff_up[3]=PKT_PULL_ACK;
            buff_up[4]=0;

            sendPacketToServer(buff_up, 4);

            data = buffer + 4;
            data[bufferLen] = 0;
            break;

        case PKT_PULL_ACK: // 0x04 DOWN; the server sends a PULL_ACK to confirm PULL_DATA receipt
            log_v("PKT_PULL_ACK  <server->gateway %d bytes> data:", bufferLen);
            log_v_dump(buffer, bufferLen);
            break;

        default:
            // For simplicity, we send the first 4 bytes too
            gateway_mgt(bufferLen, buffer);
            break;

    }
    return bufferLen;
}

// ----------------------------------------------------------------------------
// Send UP periodic Pull_DATA message to server to keepalive the connection
// and to invite the server to send downstream messages when these are available
// *2, par. 5.2
// - Protocol Version (1 byte)
// - Random Token (2 bytes)
// - PULL_DATA identifier (1 byte) = 0x02
// - Gateway unique identifier (8 bytes) = MAC address
// ----------------------------------------------------------------------------
int gatewayPullData(void)
{
    uint8_t pullDataReq[13];       // status report as a JSON object
    int pullIndex=0;

    // 12-byte binary (!) header
    pullDataReq[0]  = PROTOCOL_VERSION;   // 0x01
    uint8_t token_h = (uint8_t)rand();    // random token
    uint8_t token_l = (uint8_t)rand();    // random token
    pullDataReq[1]  = token_h;
    pullDataReq[2]  = token_l;
    pullDataReq[3]  = PKT_PULL_DATA;      // 0x02
    pullDataReq[4]  = gatewayIDHex[0];
    pullDataReq[5]  = gatewayIDHex[1];
    pullDataReq[6]  = gatewayIDHex[2];
    pullDataReq[7]  = gatewayIDHex[3];
    pullDataReq[8]  = gatewayIDHex[4];
    pullDataReq[9]  = gatewayIDHex[5];
    pullDataReq[10] = gatewayIDHex[6];
    pullDataReq[11] = gatewayIDHex[7];
    pullIndex = 12;                       // 12-byte header

    pullDataReq[pullIndex] = 0;           // add string terminator, for safety

    log_v("gateway pullData < %d > : ", pullIndex);
    log_v_dump(pullDataReq, pullIndex);

    //send the update
    sendPacketToServer(pullDataReq, pullIndex);
    return 0;
}

// ----------------------------------------------------------------------------
// Send UP periodic status message to server even when we do not receive any
// data.
// Parameter is socketr to TX to
// ----------------------------------------------------------------------------
int gatewaySendStat(void)
{
    uint8_t Buffer[512];
    int BufferIndex=0;
    aJsonClass aJson;

    // 12-byte binary (!) header
    Buffer[0]  = PROTOCOL_VERSION;   // 0x01
    uint8_t token_h   = (uint8_t)rand();    // random token
    uint8_t token_l   = (uint8_t)rand();    // random token
    Buffer[1]  = token_h;
    Buffer[2]  = token_l;
    Buffer[3]  = PKT_PUSH_DATA;      // 0x00
    Buffer[4]  = gatewayIDHex[0];
    Buffer[5]  = gatewayIDHex[1];
    Buffer[6]  = gatewayIDHex[2];
    Buffer[7]  = gatewayIDHex[3];
    Buffer[8]  = gatewayIDHex[4];
    Buffer[9]  = gatewayIDHex[5];
    Buffer[10] = gatewayIDHex[6];
    Buffer[11] = gatewayIDHex[7];
    BufferIndex = 12;                        // 12-byte header

    // start of JSON structure that will make payload
    aJsonObject* root = aJson.createObject();
    if(root == NULL) {
        return -1;
    }

    aJsonObject* statObject = aJson.createObject();
    if (statObject == NULL) {
        aJson.deleteItem(root);
        return -1;
    }

    aJson.addItemToObject(root, "stat", statObject);
    char timestamp[64] = {0};
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d CET", Time.year(),Time.month(),Time.day(),Time.hour(),Time.minute(),Time.second());
    aJson.addStringToObject(statObject, "time", timestamp);
    aJson.addNumberToObject(statObject, "lati", CONFIG_GATEWAY_LAT);
    aJson.addNumberToObject(statObject, "long", CONFIG_GATEWAY_LON);
    aJson.addNumberToObject(statObject, "alti", (int)CONFIG_GATEWAY_ALT);
    aJson.addNumberToObject(statObject, "rxnb", (int)GatewayParams.Statistics.cp_nb_rx_rcv);
    aJson.addNumberToObject(statObject, "rxok", (int)GatewayParams.Statistics.cp_nb_rx_ok);
    aJson.addNumberToObject(statObject, "rxfw", (int)GatewayParams.Statistics.cp_up_pkt_fwd);
    aJson.addNumberToObject(statObject, "ackr", 0);
    aJson.addNumberToObject(statObject, "dwnb", 0);
    aJson.addNumberToObject(statObject, "txnb", 0);
    aJson.addStringToObject(statObject, "pfrm", CONFIG_GATEWAY_PLATFORM);
    aJson.addStringToObject(statObject, "mail", CONFIG_GATEWAY_EMAIL);
    aJson.addStringToObject(statObject, "desc", CONFIG_GATEWAY_DESCRIPTION);

    //send udp data
    char* string = aJson.print(root);
    memcpy(&Buffer[12], string, strlen(string));
    BufferIndex += strlen(string);
    Buffer[BufferIndex] = 0;

    log_v("gateway stat: < %d > %s \r\n", BufferIndex, (char *)(Buffer+12));
    sendPacketToServer(Buffer, BufferIndex);

    free(string);
    aJson.deleteItem(root);
    return 0;
}

int gatewayBuildUpstreamPacket(uint32_t tmst, uint8_t *bufferUp, uint8_t *message, uint16_t messageLength)
{
    aJsonClass aJson;

    // {"rxpk":[{
    // "time":"2013-03-31T16:21:17.528002Z",
    // "tmst":3512348611,
    // "chan":2,
    // "rfch":0,
    // "freq":866.349812,
    // "stat":1,
    // "modu":"LORA",
    // "datr":"SF7BW125",
    // "codr":"4/6",
    // "rssi":-35,
    // "lsnr":5.1,
    // "size":32, "data":"-DS4CGaDCdG+48eJNM3Vai-zDpsR71Pn9CPA9uCON84"
    // }]}

    // 12-byte binary (!) header
    bufferUp[0] = PROTOCOL_VERSION;    // 0x01 still
    uint8_t token_h = (uint8_t)rand();      // random token
    uint8_t token_l = (uint8_t)rand();      // random token
    bufferUp[1] = token_h;
    bufferUp[2] = token_l;
    bufferUp[3] = PKT_PUSH_DATA;       // 0x00
    bufferUp[4] = gatewayIDHex[0];
    bufferUp[5] = gatewayIDHex[1];
    bufferUp[6] = gatewayIDHex[2];
    bufferUp[7] = gatewayIDHex[3];
    bufferUp[8] = gatewayIDHex[4];
    bufferUp[9] = gatewayIDHex[5];
    bufferUp[10] = gatewayIDHex[6];
    bufferUp[11] = gatewayIDHex[7];

    int bufferIndex = 12;

    // start of JSON structure that will make payload
    aJsonObject* root = aJson.createObject();
    if(root == NULL) {
        return -1;
    }

    aJsonObject* rxpkArray = aJson.createArray();
    if (rxpkArray == NULL) {
        aJson.deleteItem(root);
        return -1;
    }
    aJson.addItemToObject(root, "rxpk", rxpkArray);

    aJsonObject* frameObject = aJson.createObject();
    if (frameObject == NULL) {
        aJson.deleteItem(root);
        return;
    }
    aJson.addItemToArray(rxpkArray, frameObject);
    aJson.addNumberToObject(frameObject, "tmst", tmst);
    aJson.addNumberToObject(frameObject, "freq", (double)SX1278.Settings.Channel/1000000);
    aJson.addNumberToObject(frameObject, "chan", 0);
    aJson.addNumberToObject(frameObject, "rfch", 0);
    aJson.addNumberToObject(frameObject, "stat", 1);
    aJson.addStringToObject(frameObject, "modu", "LORA");
    String datrString = "SF";
    datrString += SX1278.Settings.LoRa.Datarate;
    datrString += "BW125";
    aJson.addStringToObject(frameObject, "datr", datrString.c_str());
    aJson.addStringToObject(frameObject, "codr", "4/5");
    aJson.addNumberToObject(frameObject, "lsnr", SX1278.Settings.LoRaPacketHandler.SnrValue);
    aJson.addNumberToObject(frameObject, "rssi", SX1278.Settings.LoRaPacketHandler.RssiValue);
    aJson.addNumberToObject(frameObject, "size", messageLength);

    char b64[256]={0};
    int encodedLen = base64_enc_len(messageLength);
    base64_encode(b64, (char *)message, messageLength);
    aJson.addStringToObject(frameObject, "data", b64);

    char* string = aJson.print(root);

    memcpy(&bufferUp[12], string, strlen(string));
    bufferIndex += strlen(string);
    bufferUp[bufferIndex] = 0;

    free(string);
    aJson.deleteItem(root);

    log_v("rxpk: %s \r\n", bufferUp+12);
    return bufferIndex;
}

/*---------------------------------------------------------------------------- */
/*decode the server message and prepare the payload, This function is used for */
/*regular downstream messages and for JOIN_ACCEPT messages.                    */
/*---------------------------------------------------------------------------- */
int gatewayParseDownStreamPacket(uint8_t *message, uint8_t *bufferDown, uint8_t length)
{
    //Meta Data sent by server (example)
    //{"txpk":{"codr":"4/5","data":"Kuc5CSwJ7/a5JgPHrP29X9K6kf/Vs5kU6g==","freq":434,"ipol":true,"modu":"LORA","powe":14,"rfch":0,"size":18,"tmst":1890991792,"datr":"SF7BW125"}}
    //{"txpk":{"codr":"4/5","data":"Kuc5CSwJ7/a5JgPHrP29X9K6kf/Vs5kU6g==","freq":434,"ipol":true,"modu":"LORA","powe":14,"rfch":0,"size":18,"imme":true,"datr":"SF7BW125"}}
    //12-byte header;
    //HDR (1 byte)

    //Data Reply for JOIN_ACCEPT as sent by server:
    //AppNonce (3 byte)
    //NetID (3 byte)
    //DevAddr (4 byte) [ 31..25]:NwkID , [24..0]:NwkAddr
    //DLSettings (1 byte)
    //RxDelay (1 byte)
    //CFList (fill to 16 bytes)
    aJsonClass aJson;

    log_v("server packet: %s\r\n", (char *)bufferDown);
    aJsonObject *root = aJson.parse((char *)bufferDown);
    if (root == NULL) {
        return -1;
    }

    aJsonObject* txpkObject = aJson.getObjectItem(root, "txpk");
    if (txpkObject == NULL) {
        aJson.deleteItem(root);
        return -1;
    }

    // imme
    aJsonObject* immeObject = aJson.getObjectItem(txpkObject, "imme");
    if (immeObject != NULL) {
        if(immeObject->valuebool) {
            GatewayParams.Status.sendTmst = micros();
        }
    }

    // tmst
    aJsonObject* tmstObject = aJson.getObjectItem(txpkObject, "tmst");
    if (tmstObject != NULL) {
        if( tmstObject->type == aJson_Int ) {
            GatewayParams.Status.sendTmst = (uint32_t)tmstObject->valueint;
        } else if( tmstObject->type == aJson_Uint ) {
            GatewayParams.Status.sendTmst = (uint32_t)tmstObject->valueuint;
        }
        log_v("joint tmst1 = %d\r\n", GatewayParams.Status.sendTmst);
    }

    // time
    char *time = NULL;
    aJsonObject* timeObject = aJson.getObjectItem(txpkObject, "time");
    if (timeObject != NULL) {
        time = timeObject->valuestring;
    }

    // freq
    aJsonObject* freqObject = aJson.getObjectItem(txpkObject, "freq");
    if (freqObject != NULL) {
        if( freqObject->type == aJson_Float ) {
            GatewayParams.TxParams.channel = (uint32_t)(freqObject->valuefloat*1000000);
        } else if( freqObject->type == aJson_Int ) {
            GatewayParams.TxParams.channel = (uint32_t)(freqObject->valueint*1000000);
        }
    }

    // rfch
    aJsonObject* rfchObject = aJson.getObjectItem(txpkObject, "rfch");
    if (rfchObject == NULL) {
        aJson.deleteItem(root);
        return -1;
    }

    // ncrc
    aJsonObject* ncrcObject = aJson.getObjectItem(txpkObject, "ncrc");
    if (ncrcObject != NULL) {
        GatewayParams.TxParams.crcOn = !ncrcObject->valuebool;
    }

    // powe
    aJsonObject* poweObject = aJson.getObjectItem(txpkObject, "powe");
    if (poweObject != NULL) {
        GatewayParams.TxParams.power = poweObject->valueint;
    }

    // modu
    aJsonObject* moduObject = aJson.getObjectItem(txpkObject, "modu");
    if (moduObject != NULL) {
        if(!strcmp(moduObject->valuestring, "FSK")) {
            GatewayParams.TxParams.modem = MODEM_FSK;
        } else {
            GatewayParams.TxParams.modem = MODEM_LORA;
        }
    }

    // datr
    int x0, x1;
    if (GatewayParams.TxParams.modem == MODEM_LORA) {
        aJsonObject* datrObject = aJson.getObjectItem(txpkObject, "datr");
        if (datrObject != NULL) {
            if(sscanf(datrObject->valuestring, "SF%dBW%d", &x0, &x1) == 2) {
                switch (x0) {
                    case  7: GatewayParams.TxParams.datarate = 7;  break;
                    case  8: GatewayParams.TxParams.datarate = 8;  break;
                    case  9: GatewayParams.TxParams.datarate = 9;  break;
                    case 10: GatewayParams.TxParams.datarate = 10; break;
                    case 11: GatewayParams.TxParams.datarate = 11; break;
                    case 12: GatewayParams.TxParams.datarate = 12; break;
                    default: GatewayParams.TxParams.datarate = 7;  break;
                }
                switch (x1) {
                    case 125: GatewayParams.TxParams.bandwidth = 0; break;
                    case 250: GatewayParams.TxParams.bandwidth = 1; break;
                    case 500: GatewayParams.TxParams.bandwidth = 2; break;
                    default: GatewayParams.TxParams.bandwidth = 0; break;
                }
            }
        }

        // codr
        aJsonObject* codrObject = aJson.getObjectItem(txpkObject, "codr");
        if (codrObject != NULL) {
            if      (strcmp(codrObject->valuestring, "4/5") == 0) GatewayParams.TxParams.coderate = 1;
            else if (strcmp(codrObject->valuestring, "4/6") == 0) GatewayParams.TxParams.coderate = 2;
            else if (strcmp(codrObject->valuestring, "2/3") == 0) GatewayParams.TxParams.coderate = 2;
            else if (strcmp(codrObject->valuestring, "4/7") == 0) GatewayParams.TxParams.coderate = 3;
            else if (strcmp(codrObject->valuestring, "4/8") == 0) GatewayParams.TxParams.coderate = 4;
            else if (strcmp(codrObject->valuestring, "1/2") == 0) GatewayParams.TxParams.coderate = 4;
            else {
                GatewayParams.TxParams.coderate = 1;
            }
        }

        // ipol
        aJsonObject* ipolObject = aJson.getObjectItem(txpkObject, "ipol");
        if (ipolObject != NULL) {
            GatewayParams.TxParams.iqInverted = ipolObject->valuebool;
        } else {
            aJson.deleteItem(root);
            return -1;
        }
    }

    // size
    uint16_t psize = 0;
    aJsonObject* sizeObject = aJson.getObjectItem(txpkObject, "size");
    if (sizeObject != NULL) {
        psize = sizeObject->valueint;
    }

    // data
    char * data = NULL;
    aJsonObject* dataObject = aJson.getObjectItem(txpkObject, "data");
    if (dataObject != NULL) {
        data = dataObject->valuestring;
    }

    if (data != NULL) {
        log_v("data: %s \r\n", (char *)data);
    } else {
        log_v("ERROR: data is NULL \r\n");
        return -1;
    }

    uint16_t payLength = base64_dec_len((char *)data, strlen(data));
    base64_decode((char *)message, (char *)data, strlen(data));

    if (payLength != psize) {
        log_v("WARNING: payLength:%d, psize=%d ", payLength, psize);
    }
    log_v_dump(message, payLength);

    GatewayParams.Statistics.cp_up_pkt_fwd++;
    aJson.deleteItem(root);
    return payLength;
}

