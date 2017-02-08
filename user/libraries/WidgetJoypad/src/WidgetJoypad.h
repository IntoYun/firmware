#ifndef WIDGET_JOYPAD_H_
#define WIDGET_JOYPAD_H_

#include "application.h"

class WidgetJoypad : public WidgetBaseClass
{
    public:
        WidgetJoypad(uint8_t ucItem = 0);
        ~WidgetJoypad();
        void begin(void (*UserCallBack)(void) = NULL);
        uint8_t getUpkey(void);
        uint8_t getDownkey(void);
        uint8_t getLeftkey(void);
        uint8_t getRightkey(void);
        uint8_t getFnkey1(void);
        uint8_t getFnkey2(void);
        uint8_t getFnkey3(void);
        uint8_t getFnkey4(void);
        uint8_t getFnkey5(void);
        uint8_t getFnkey6(void);
        int getLeftJoystickX(void);
        int getLeftJoystickY(void);
        int getRightJoystickX(void);
        int getRightJoystickY(void);

    private:
        char pCmdSwitchTopic[64];
        uint8_t _Item = 0;
        //方向键
        uint8_t _upKey;
        uint8_t _downKey;
        uint8_t _leftKey;
        uint8_t _rightKey;
        //功能键
        uint8_t _fnKey1;
        uint8_t _fnKey2;
        uint8_t _fnKey3;
        uint8_t _fnKey4;
        uint8_t _fnKey5;
        uint8_t _fnKey6;
        //遥杆坐标
        int _joystickX_7;
        int _joystickY_7;
        int _joystickX_8;
        int _joystickY_8;
        
        char *_switchKey;

        void (*_EventCb)(void);
        void widgetBaseCallBack(uint8_t *payload, uint32_t len);
};

#endif
