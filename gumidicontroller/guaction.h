/*
Action is a wrapper for callback function and name tag.
By creating own actions in namepsace Gu::Actions::<module>, they can be used to connect with physical inputs ie with Gu::Programs
*/
#ifndef guAction_H
#define guAction_H
//#include <functional>
//#include <utility>
#include <Arduino.h>

namespace Gu::Actions {
//a może da sie variable template na Callback zrobić?
using Callback = void *();
//using Callback = std::function<void()>;

struct Action { 
  Callback callback;
  const char* name;
  //TODO: jakbym to chciał przerobić na operator String(...){}? to jak?
  //std::initializer_list<Q> takie coś gdzieś? https://en.cppreference.com/w/cpp/language/list_initialization
  //bo struct nie może mieć metod w środku, tak?
  String toString(){ return String(name); }
};

//error: could not convert '{<lambda closure object>Gu::Actions::None()::<lambda()>{}, "---"}' from '<brace-enclosed initializer list>' to 'Gu::Actions::Action'
//error: body of constexpr function 'constexpr Gu::Actions::Action Gu::Actions::None()' not a return-statement
constexpr Action None() { return { [](){ NULL; }, "---" }; };

}
#endif