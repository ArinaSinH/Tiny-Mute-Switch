# Mute switch
This tiny project implements a dedicated USB HID button designed for a usefull microphone mute switch. Any kind of switch can be used: a keyboard switch, a foot pedal or even a paperclip and a piece of foil.

## What can it do?
It can toggle the microphone or and off with a short press, but if the button in hold down long enough, it will mute/unmute on button release too. It also implements a configurable debounce to senure dubious quality buttons will work kinda consistently.

Both how long the button needs to be pressed down for a "short press" and the debounce time can be configured in the project before flashing the device (Exmplanation in how it works).

## How it works
An USB HID capable microcontroller is the brain of the operation. I've used an Arduino pro micro (any variant using the Atmega32u4 chip will work). There are other micros capable of native HID that will be able to be flashed after installing the apropiate board in arduino IDE, like the esp32 s2, s3 and p4 variants, but bare in mind these examples are way more power hungry than the pro micro.

The project contains 4 global variables you can configure before flashing the device:

```c++
// The debug led pin
#define LED_PIN 17
// the pin to read the button state
#define BUTTON_PIN 2
// How long in ms a press will still be considered short
#define UNPRESS_TIMER_MAX 500
// How long in ms the debunce is
#define DEBOUNCE 10
```

Some clarification with these values:
 - **LED_PIN**: If you want, you can define a pin to just power on and off when the button itself is pressed, no matter the current mute/unpute state. Not necesary in any way, shape or form, but really usefull to debug the button conection. It defaults to pin 17, which is the builtin LED of the arduino pro micro
 - **BUTTON_PIN**: What pin will be reading the buttons state. The pin we usse here is really important. For efficiency, this program is powered by interrupts, meaning the relevant code only executes if certain conditions are met. The pro micro only support interrupts in certain pins, specifically pins 0, 1, 2, 3 and 7 are interrupt capable, you can use any of them.
 - **UNPRESS_TIMER_MAX**: The point in ms where a short press becomes a long press. Whenever the button is pressed down, the mute key is pressed. When the button is released, if the press lasted less than UNPRESS_TIMER_MAX, nothing happens, but if its longer, then the mute key is pressed once again. This allows you to both toggle the mute and hold/release to mute/unmute quickly. It defaults to 500ms.
 - **DEBOUNCE**: How long in ms the debounce is. With betetr quality buttons you can decrease the debounce time and still get consistent results. With the crappy button I tested 10ms was mostly ok.

 The button pin is condigured as a pullup, so we only need to properly conect the button, one end to the pin the other to gorund:  
![Wiring_diagram](circuit.svg "Wiring diagram")