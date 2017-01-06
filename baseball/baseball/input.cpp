#include "input.h"
#include "keyboard.h"
#include "leapmotion.h"

Input &Input::s_input = LeapMotion();
