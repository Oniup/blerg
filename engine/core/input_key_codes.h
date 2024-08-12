// Copyright (c) 2024 Oniup (https://github.com/Oniup)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CORE_INPUT_KEYCODES_H
#define CORE_INPUT_KEYCODES_H

// Same key codes from GLFW

enum KeyCode {
  KeyCode_Unknown = -1,

  // PrintAble keys
  // ============================================================================================

  KeyCode_Space        = 32,
  KeyCode_Apostrophe   = 39,  // '
  KeyCode_Comma        = 44,  // ,
  KeyCode_Minus        = 45,  // -
  KeyCode_Period       = 46,  // .
  KeyCode_Slash        = 47,  // /
  KeyCode_0            = 48,
  KeyCode_1            = 49,
  KeyCode_2            = 50,
  KeyCode_3            = 51,
  KeyCode_4            = 52,
  KeyCode_5            = 53,
  KeyCode_6            = 54,
  KeyCode_7            = 55,
  KeyCode_8            = 56,
  KeyCode_9            = 57,
  KeyCode_Semicolon    = 59,  // ;
  KeyCode_Equal        = 61,  // =
  KeyCode_A            = 65,
  KeyCode_B            = 66,
  KeyCode_C            = 67,
  KeyCode_D            = 68,
  KeyCode_E            = 69,
  KeyCode_F            = 70,
  KeyCode_G            = 71,
  KeyCode_H            = 72,
  KeyCode_I            = 73,
  KeyCode_J            = 74,
  KeyCode_K            = 75,
  KeyCode_L            = 76,
  KeyCode_M            = 77,
  KeyCode_N            = 78,
  KeyCode_O            = 79,
  KeyCode_P            = 80,
  KeyCode_Q            = 81,
  KeyCode_R            = 82,
  KeyCode_S            = 83,
  KeyCode_T            = 84,
  KeyCode_U            = 85,
  KeyCode_V            = 86,
  KeyCode_W            = 87,
  KeyCode_X            = 88,
  KeyCode_Y            = 89,
  KeyCode_Z            = 90,
  KeyCode_LeftBracket  = 91,
  KeyCode_Backslash    = 92,
  KeyCode_RightBracket = 93,
  KeyCode_GraveAccent  = 96,   // `
  KeyCode_World1       = 161,  // non-us #1
  KeyCode_World2       = 162,  // non-us #2

  // Function Keys
  // ============================================================================================

  KeyCode_Escape       = 256,
  KeyCode_Enter        = 257,
  KeyCode_Tab          = 258,
  KeyCode_Backspace    = 259,
  KeyCode_Insert       = 260,
  KeyCode_Delete       = 261,
  KeyCode_Right        = 262,
  KeyCode_Left         = 263,
  KeyCode_Down         = 264,
  KeyCode_Up           = 265,
  KeyCode_PageUp       = 266,
  KeyCode_PageDown     = 267,
  KeyCode_Home         = 268,
  KeyCode_End          = 269,
  KeyCode_CapsLock     = 280,
  KeyCode_ScrollLock   = 281,
  KeyCode_NumLock      = 282,
  KeyCode_PrintScreen  = 283,
  KeyCode_Pause        = 284,
  KeyCode_F1           = 290,
  KeyCode_F2           = 291,
  KeyCode_F3           = 292,
  KeyCode_F4           = 293,
  KeyCode_F5           = 294,
  KeyCode_F6           = 295,
  KeyCode_F7           = 296,
  KeyCode_F8           = 297,
  KeyCode_F9           = 298,
  KeyCode_F10          = 299,
  KeyCode_F11          = 300,
  KeyCode_F12          = 301,
  KeyCode_F13          = 302,
  KeyCode_F14          = 303,
  KeyCode_F15          = 304,
  KeyCode_F16          = 305,
  KeyCode_F17          = 306,
  KeyCode_F18          = 307,
  KeyCode_F19          = 308,
  KeyCode_F20          = 309,
  KeyCode_F21          = 310,
  KeyCode_F22          = 311,
  KeyCode_F23          = 312,
  KeyCode_F24          = 313,
  KeyCode_F25          = 314,
  KeyCode_Kp0          = 320,
  KeyCode_Kp1          = 321,
  KeyCode_Kp2          = 322,
  KeyCode_Kp3          = 323,
  KeyCode_Kp4          = 324,
  KeyCode_Kp5          = 325,
  KeyCode_Kp6          = 326,
  KeyCode_Kp7          = 327,
  KeyCode_Kp8          = 328,
  KeyCode_Kp9          = 329,
  KeyCode_KpDecimal    = 330,
  KeyCode_KpDivide     = 331,
  KeyCode_KpMultiply   = 332,
  KeyCode_KpSubtract   = 333,
  KeyCode_KpAdd        = 334,
  KeyCode_KpEnter      = 335,
  KeyCode_KpEqual      = 336,
  KeyCode_LeftShift    = 340,
  KeyCode_LeftControl  = 341,
  KeyCode_LeftAlt      = 342,
  KeyCode_LeftSuper    = 343,
  KeyCode_RightShift   = 344,
  KeyCode_RightControl = 345,
  KeyCode_RightAlt     = 346,
  KeyCode_RightSuper   = 347,
  KeyCode_Menu         = 348,

  KeyCode_Last = KeyCode_Menu,
};

enum KeyMod {
  // If this bit is set one or more Shift keys were held down.
  KeyMod_Shift = 0x0001,
  // If this bit is set one or more control keys were held down.
  KeyMod_Control = 0x0002,
  //  If this bit is set one or more alt keys were held down.
  KeyMod_Alt = 0x0004,
  //  If this bit is set one or more super keys were held down.
  KeyMod_Super = 0x0008,
  // If this bit is set the caps lock key is enabled and the @ref
  // GLFW_LOCK_KEY_MODS input mode is set.
  KeyMod_CapsLock = 0x0010,
  // If this bit is set the num lock key is enabled and the @ref
  // GLFW_LOCK_KEY_MODS input mode is set.
  KeyMod_NumLock = 0x0020,
};

/*! @} */

enum MouseButton {
  MouseButton_1      = 0,
  MouseButton_2      = 1,
  MouseButton_3      = 2,
  MouseButton_4      = 3,
  MouseButton_5      = 4,
  MouseButton_6      = 5,
  MouseButton_7      = 6,
  MouseButton_8      = 7,
  MouseButton_Last   = MouseButton_8,
  MouseButton_Left   = MouseButton_1,
  MouseButton_Right  = MouseButton_2,
  MouseButton_Middle = MouseButton_3,
};

enum GamepadJoystick {
  GamepadJoystick_1    = 0,
  GamepadJoystick_2    = 1,
  GamepadJoystick_3    = 2,
  GamepadJoystick_4    = 3,
  GamepadJoystick_5    = 4,
  GamepadJoystick_6    = 5,
  GamepadJoystick_7    = 6,
  GamepadJoystick_8    = 7,
  GamepadJoystick_9    = 8,
  GamepadJoystick_10   = 9,
  GamepadJoystick_11   = 10,
  GamepadJoystick_12   = 11,
  GamepadJoystick_13   = 12,
  GamepadJoystick_14   = 13,
  GamepadJoystick_15   = 14,
  GamepadJoystick_16   = 15,
  GamepadJoystick_LAST = GamepadJoystick_16,
};

enum GamepadButton {
  GamepadButton_A           = 0,
  GamepadButton_B           = 1,
  GamepadButton_X           = 2,
  GamepadButton_Y           = 3,
  GamepadButton_LeftBumper  = 4,
  GamepadButton_RightBumper = 5,
  GamepadButton_Back        = 6,
  GamepadButton_Start       = 7,
  GamepadButton_Guide       = 8,
  GamepadButton_LeftThumb   = 9,
  GamepadButton_RightThumb  = 10,
  GamepadButton_DpadUp      = 11,
  GamepadButton_DpadRight   = 12,
  GamepadButton_DpadDown    = 13,
  GamepadButton_DpadLeft    = 14,
  GamepadButton_Last        = GamepadButton_DpadLeft,

  GamepadButton_Cross    = GamepadButton_A,
  GamepadButton_Circle   = GamepadButton_B,
  GamepadButton_Square   = GamepadButton_X,
  GamepadButton_Triangle = GamepadButton_Y,
};

enum GamepadAxis {
  GamepadAxis_LeftX        = 0,
  GamepadAxis_LeftY        = 1,
  GamepadAxis_RightX       = 2,
  GamepadAxis_RightY       = 3,
  GamepadAxis_LeftTrigger  = 4,
  GamepadAxis_RightTrigger = 5,
  GamepadAxis_Last         = GamepadAxis_RightTrigger,
};

#endif
