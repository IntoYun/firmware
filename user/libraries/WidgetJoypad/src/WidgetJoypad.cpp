#include "WidgetJoypad.h"


WidgetJoypad::WidgetJoypad(uint8_t ucItem)
{
    _Item=ucItem;
    memset(pCmdSwitchTopic,0,sizeof(pCmdSwitchTopic));
    sprintf(pCmdSwitchTopic,"channel/joypad_%d/cmd/control",_Item);
}

WidgetJoypad::~WidgetJoypad()
{

}

void WidgetJoypad::begin(void (*UserCallBack)(void))
{
    _EventCb = UserCallBack;
    IntoRobot.subscribe(pCmdSwitchTopic,NULL,this);
}


uint8_t WidgetJoypad::getUpkey(void)
{
    return _upKey;
}

uint8_t WidgetJoypad::getDownkey(void)
{
    return _downKey;
}

uint8_t WidgetJoypad::getLeftkey(void)
{
    return _leftKey;
}

uint8_t WidgetJoypad::getRightkey(void)
{
    return _rightKey;
}

uint8_t WidgetJoypad::getFnkey1(void)
{
    return _fnKey1;
}

uint8_t WidgetJoypad::getFnkey2(void)
{
    return _fnKey2;
}

uint8_t WidgetJoypad::getFnkey3(void)
{
    return _fnKey3;
}

uint8_t WidgetJoypad::getFnkey4(void)
{
    return _fnKey4;
}

uint8_t WidgetJoypad::getFnkey5(void)
{
    return _fnKey5;
}

uint8_t WidgetJoypad::getFnkey6(void)
{
    return _fnKey6;
}

int WidgetJoypad::getLeftJoystickX(void)
{
    return _joystickX_7;
}

int WidgetJoypad::getLeftJoystickY(void)
{
    return _joystickY_7;
}

int WidgetJoypad::getRightJoystickX(void)
{
    return _joystickX_8;
}

int WidgetJoypad::getRightJoystickY(void)
{
    return _joystickY_8;
}


void WidgetJoypad::widgetBaseCallBack(uint8_t *payload, uint32_t len)
{
    aJsonClass aJson;

  aJsonObject *root = aJson.parse((char *)payload);

  if(root == NULL)
  {
      aJson.deleteItem(root);
    return;
  }

  aJsonObject *key = aJson.getObjectItem(root, "key");

  if(key == NULL)
  {
      aJson.deleteItem(root);
      return;
  }

  aJsonObject *switchKey = aJson.getObjectItem(key, "switch");

  if(switchKey == NULL)
  {
      aJson.deleteItem(root);
      return;
  }

  aJsonObject *state = aJson.getObjectItem(key, "state");

  if(state == NULL)
  {
      aJson.deleteItem(root);
      return;
  }

  _switchKey = switchKey->valuestring;

  if(strcmp(_switchKey,"up") == 0)
  {
      _upKey = state->valueint;
  }

    if(strcmp(_switchKey,"down") == 0)
  {
      _downKey = state->valueint;
  }

    if(strcmp(_switchKey,"left") == 0)
  {
      _leftKey = state->valueint;
  }

  if(strcmp(_switchKey,"right") == 0)
  {
      _rightKey = state->valueint;
  }

  if(strcmp(_switchKey,"1") == 0)
  {
      _fnKey1 = state->valueint;
  }

  if(strcmp(_switchKey,"2") == 0)
  {
      _fnKey2 = state->valueint;
  }

  if(strcmp(_switchKey,"3") == 0)
  {
      _fnKey3 = state->valueint;
  }

  if(strcmp(_switchKey,"4") == 0)
  {
      _fnKey4 = state->valueint;
  }

  if(strcmp(_switchKey,"5") == 0)
  {
      _fnKey5 = state->valueint;
  }

  if(strcmp(_switchKey,"6") == 0)
  {
      _fnKey6 = state->valueint;
  }

  if(strcmp(_switchKey,"7") == 0)
  {
      aJsonObject *joystickX_7 = aJson.getObjectItem(state,"x");
      if(joystickX_7 == NULL)
      {
          aJson.deleteItem(root);
          return;
      }
      _joystickX_7 = joystickX_7->valueint;

      aJsonObject *joystickY_7 = aJson.getObjectItem(state,"y");
      if(joystickY_7 == NULL)
      {
          aJson.deleteItem(root);
          return;
      }

      _joystickY_7 = joystickY_7->valueint;
  }

  if(strcmp(_switchKey,"8") == 0)
  {
      aJsonObject *joystickX_8 = aJson.getObjectItem(state,"x");
      if(joystickX_8 == NULL)
      {
          aJson.deleteItem(root);
          return;
      }
      _joystickX_8 = joystickX_8->valueint;

      aJsonObject *joystickY_8 = aJson.getObjectItem(state,"y");
      if(joystickY_8 == NULL)
      {
          aJson.deleteItem(root);
          return;
      }
      _joystickY_8 = joystickY_8->valueint;
  }

  aJson.deleteItem(root);
  _EventCb();

}
