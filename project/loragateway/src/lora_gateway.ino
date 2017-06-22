/**
 ******************************************************************************
 Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#include "lora_gateway_config.h"
#include "lora_gateway_protocol.h"
#include "lora_gateway_log.h"
#include "lora_gateway_os.h"
#include "lora_gateway_params.h"
#include "lora_gateway_ui.h"
#include "sx1278-board.h"
#include "sx1278.h"


SerialDebugOutput debugOutput(115200, ALL_LEVEL);

#ifndef CONFIG_GATEWAY_COMMUNICATION_MQTT
SYSTEM_MODE(MANUAL)
#endif
PRODUCT_TYPE(PRODUCT_TYPE_GATEWAY)
PRODUCT_ID(RfOaH3qogjjud1df)
PRODUCT_SECRET(b05b732bfb4d1917433e7849c495a8d9)
PRODUCT_SOFTWARE_VERSION(1.0.0)
PRODUCT_HARDWARE_VERSION(1.0.0)


uint8_t gatewayIDHex[8];
static osjob_t initJob, statReportJob, pullDataJob, dealUdpPackageJob;
static RadioEvents_t RadioEvents;

void init_before_setup(void)
{
    System.disableFeature(SYSTEM_FEATURE_CONFIG_SAVE_ENABLED);          //不保存配置模式
    System.disableFeature(SYSTEM_FEATURE_DATAPOINT_ENABLED);            //关闭数据点处理
}

STARTUP( init_before_setup() );

static void OnTxDone( void ) {
    log_v("OnTxDone\r\n");
    loraRx();
}

static void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr ) {
    uint8_t bufferUp[512];
    uint16_t bufferLen;

    // Take the timestamp as soon as possible, to have accurate reception timestamp
    GatewayParams.Status.lastTmst = (uint32_t)micros();
    log_v("OnRxDone rssi = %d, snr = %d, data< %d >:", rssi, snr, size);
    log_v_dump(payload, size);

    GatewayParams.Statistics.cp_nb_rx_rcv++;           // Receive statistics counter
    GatewayParams.Statistics.cp_nb_rx_ok++;

    bufferLen = gatewayBuildUpstreamPacket(GatewayParams.Status.lastTmst, bufferUp, payload, size);
    if (bufferLen) {
        sendPacketToServer(bufferUp, bufferLen);
    } else {
        log_v("eH:: WARNING S_RX empty message, sf=%d\r\n", SX1278.Settings.LoRa.Datarate);
    }
    loraRx();
}

static void OnTxTimeout( void ) {
    log_v("OnTxTimeout\r\n");
    loraRx();
}

static void OnRxTimeout( void ) {
    log_v("OnRxTimeout\r\n");
    loraRx();
}

static void OnRxError( void ) {
    log_v("OnRxError\r\n");
    GatewayParams.Statistics.cp_nb_rx_rcv++;           // Receive statistics counter
    GatewayParams.Statistics.cp_nb_rx_bad++;
    loraRx();
}

static void OnFhssChangeChannel( uint8_t currentChannel ) {
    log_v("OnFhssChangeChannel\r\n");
}

static void OnCadDone( bool channelActivityDetected ) {
    log_v("OnCadDone\r\n");
}

static void gatewayInit(void) {
    SX1278BoardInit();
    // Radio initialization
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    RadioEvents.RxError = OnRxError;
    RadioEvents.FhssChangeChannel = OnFhssChangeChannel;
    RadioEvents.CadDone = OnCadDone;
    Radio.Init( &RadioEvents );

    loraParamsInit();
    loraRx();
}

// deal Udp receive
static void dealUdpRecPackageFunc(osjob_t *job) {
    SX1278IoCheck();
    manageServerConnect();
    manageServerCommunication();
    os_setCallback(job, dealUdpRecPackageFunc);
}

// gateway stat report
static void gatewayStatReportFunc(osjob_t *job) {
    gatewaySendStat();
    os_setTimedCallback(job, os_getTime()+CONFIG_STAT_INTERVAL*1000000, gatewayStatReportFunc);
}

// gateway pull data
static void gatewayPullDataFunc(osjob_t *job) {
    gatewayPullData();
    os_setTimedCallback(job, os_getTime()+CONFIG_PULL_INTERVAL*1000000, gatewayPullDataFunc);
}

// initial job
static void initFunc(osjob_t *job) {
    gatewayInit();

    os_setCallback(&dealUdpPackageJob, dealUdpRecPackageFunc);
    os_setCallback(&statReportJob, gatewayStatReportFunc);

#ifndef CONFIG_GATEWAY_COMMUNICATION_MQTT
    os_setCallback(&pullDataJob, gatewayPullDataFunc);
#endif
}

void system_event_callback(system_event_t event, int param, uint8_t *data, uint16_t datalen) {
    switch(event) {
        case event_mode_changed:
            switch(param) {
                case ep_mode_normal:
                    gatewayUiIndicatorControl(false);
                    break;
                case ep_mode_imlink_serial_config:
                    gatewayUiIndicatorBlink(500);
                    break;
                case ep_mode_ap_serial_config:
                    gatewayUiIndicatorBlink(1000);
                    break;
                default:
                    break;
            }
            break;
        case event_cloud_status:
            switch(param) {
                case ep_cloud_status_disconnected:
                    gatewayUiIndicatorControl(false);
                    break;
                case ep_cloud_status_connected:
                    gatewayUiIndicatorControl(true);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void setup() {
    log_gateway_int();
    log_v("lora gateway start ...\r\n");
    String gatewayID = System.deviceID();

    log_v("--------------------------------------\r\n");
    log_v("gateway ID: %s\r\n", gatewayID.c_str());
    log_v("frequency : %d\r\n", CONFIG_FREQUENCY);
    log_v("sf:         SF%d\r\n", CONFIG_SPREADING_FACTOR);
    log_v("udp server: %s\r\n", CONFIG_LORAWAN_UDP_SERVER);
    log_v("udp port:   %d\r\n", CONFIG_LORAWAN_UDP_PORT);
    log_v("lat:        %f\r\n", CONFIG_GATEWAY_LAT);
    log_v("lon:        %f\r\n", CONFIG_GATEWAY_LON);
    log_v("alt:        %d\r\n", CONFIG_GATEWAY_ALT);
    log_v("--------------------------------------\r\n");

    memset(gatewayIDHex, 0, sizeof(gatewayIDHex));
    string2hex(gatewayID.c_str(), gatewayIDHex, sizeof(gatewayIDHex), false);

    gatewayUiInitial();
    System.on(event_mode_changed|event_cloud_status, &system_event_callback);
    System.set(SYSTEM_PARAMS_PRODUCT_BOARD_ID, "188001");
    System.set(SYSTEM_PARAMS_PRODUCT_BOARD_NAME, "gl1000");
    initCommunication();
    waitServerConnect();
    os_init();
    os_setCallback(&initJob, initFunc);
}

void loop() {
    os_runloop();
}

