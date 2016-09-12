#ifndef WIDGET_SMART_LIGHT_H_
#define WIDGET_SMART_LIGHT_H_

#include "application.h"

class WidgetSmartLight : public WidgetBaseClass
{
    public:
        WidgetSmartLight(uint8_t ucItem = 0);
        ~WidgetSmartLight();
        void begin(void (*UserCallBack)(void) = NULL);
        char getLightSwitch(void);
        void displayLightStatus(uint8_t ucVal);
        void control(uint8_t ucPin, uint8_t ucVal);

    private:
        char pCmdSwitchTopic[64];
        char pDataStatusTopic[64];
        char _switchKey = NONE;
        uint8_t _Item = 0;

        void (*_EventCb)(void);
        void widgetBaseCallBack(uint8_t *payload, uint32_t len);
};

#endif
