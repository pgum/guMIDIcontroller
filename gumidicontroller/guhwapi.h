#ifndef guHWapi_H
#define guHWapi_H
#include <AceButton.h>
#include "guhelpers.h"
using namespace ace_button;

using Pin = byte;

constexpr Pin Pin06 = 6;
constexpr Pin Pin07 = 7;
constexpr Pin Pin08 = 8;
constexpr Pin Pin09 = 9;
constexpr Pin Pin10 = 10;
constexpr Pin Pin14 = 14;
constexpr Pin Pin15 = 15;
constexpr Pin Pin16 = 16;
constexpr Pin Pin18 = 18;
constexpr Pin Pin19 = 19;
constexpr Pin Pin20 = 20;
constexpr Pin Pin21 = 21;

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
#endif
