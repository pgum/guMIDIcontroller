/*
Gu::HWApi holds class that manages physical button inputs using AceButton library.
*/
#ifndef guHWapi_H
#define guHWapi_H
#include <Arduino.h>
#include <AceButton.h>
#include "guhelpers.h"

namespace Gu::HWApi{
using namespace ace_button;

struct guButtonCfg{
  const Id id;
  const Pin pin;
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
  String signature() const {
    return String(numberOfUserButtons) + "us " + String(numberOfControlButtons) + "ct";
  }
  private:
    void initUserPins() {
      for(const auto &b : cfg.userBtns){
        pinMode(b.pin, pinConnectionTypeInputPullup);
        userButtons[b.id].setButtonConfig(&userButtonConfig);
        userButtons[b.id].init(b.pin, pinConnectionTypeEventOnStateHigh, b.id);
      }
    }
    void initCtrlPins() {
      for(const auto &b : cfg.ctrlBtns){
        pinMode(b.pin, pinConnectionTypeInputPullup);
        ctrlButtons[b.id].setButtonConfig(&ctrlButtonConfig);
        ctrlButtons[b.id].init(b.pin, pinConnectionTypeEventOnStateHigh, b.id);
      }
    }
};

} //namespace Gu::HWApi
#endif
