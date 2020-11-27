/*
Action is a wrapper for callback function and name tag.
By creating own actions in namepsace Gu::Actions::<module>, they can be used to connect with physical inputs ie with Gu::Programs
*/
#pragma once
#ifndef guAction_H
#define guAction_H
#include "functional-avr/nonstd.h"
#include <Arduino.h>

namespace Gu::Actions {
using Callback = nonstd::function<void()>;

struct Action { 
  Callback callback;
  const char* name;
};

static Action None() { return { [](){ NULL; }, "---" }; };

} //namespace Gu::Actions
#endif