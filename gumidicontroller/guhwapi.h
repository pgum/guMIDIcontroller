#ifndef guHWapi_H
#define guHWapi_H
#include <AceButton.h>

using namespace ace_button;

using inputPin = byte;

struct guButtonCfg{
  const CfgId id;
  const byte pin;
};

template <byte numberOfUserButtons, byte numberOfControlButtons>
struct guHwConfig{
  const guButtonCfg userBtns[numberOfUserButtons];
  const guButtonCfg ctrlBtns[numberOfControlButtons];
};

template <byte numberOfUserButtons, byte numberOfControlButtons>
class guHwApi {
  public:
  constexpr static auto pinConnectionTypeInputPullup = INPUT_PULLUP;
  constexpr static auto pinConnectionTypeEventOnStateHigh = HIGH;
  AceButton userButtons[numberOfUserButtons];
  AceButton ctrlButtons[numberOfControlButtons];
  
  ButtonConfig::EventHandler userButtonsEventHandler;
  ButtonConfig::EventHandler ctrlButtonsEventHandler;
  
  ButtonConfig userButtonConfig;
  ButtonConfig ctrlButtonConfig;
  
  const guHwConfig<numberOfUserButtons, numberOfControlButtons> cfg;
  
  guHwApi(guHwConfig<numberOfUserButtons, numberOfControlButtons> hwConfig): cfg(hwConfig){}
  void init() {
      initUserPins(); 
      initCtrlPins();
      userButtonConfig.setEventHandler(userButtonsEventHandler);
      ctrlButtonConfig.setEventHandler(ctrlButtonsEventHandler);
      userButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);
      ctrlButtonConfig.setFeature(ButtonConfig::kFeatureSuppressAll);
  }
  void update(){
    for( auto &b : userButtons) b.check();
    for( auto &b : ctrlButtons) b.check();

  }
  private:
    void initUserPins() {
      for(const auto &b : cfg.userBtns){
        pinMode(b.pin, pinConnectionTypeInputPullup);
        auto id = Id2Byte(b.id);
        userButtons[id].setButtonConfig(&userButtonConfig);
        userButtons[id].init(b.pin, pinConnectionTypeEventOnStateHigh, id);
      }
    }
    void initCtrlPins() {
      for(const auto &b : cfg.ctrlBtns){
        pinMode(b.pin, pinConnectionTypeInputPullup);
        auto id = Id2Byte(b.id);
        ctrlButtons[id].setButtonConfig(&ctrlButtonConfig);
        ctrlButtons[id].init(b.pin, pinConnectionTypeEventOnStateHigh, id);
      }
    }
};
#endif
