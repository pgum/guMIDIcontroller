#ifndef guAction_H
#define guAction_H
//#include <functional>
//#include <utility>
#include <Arduino.h>

namespace Gu::Actions {
using Callback = void *();
//using Callback = std::function<void()>;

struct Action { 
  Callback callback;
  const char* name;
  //TODO: jakbym to chciał przerobić na operator String(...){}? to jak?
  String toString(){ return String(name); }
};

//error: could not convert '{<lambda closure object>Gu::Actions::None()::<lambda()>{}, "---"}' from '<brace-enclosed initializer list>' to 'Gu::Actions::Action'
//error: body of constexpr function 'constexpr Gu::Actions::Action Gu::Actions::None()' not a return-statement
constexpr Action None() { return { [](){ NULL; }, "---" }; };

}
#endif