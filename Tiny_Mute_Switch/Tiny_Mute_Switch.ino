#include <Keyboard.h>

#define LED_PIN 17
#define BUTTON_PIN 2
#define UNPRESS_TIMER_MAX 500
#define DEBOUNCE 10

volatile uint8_t CURRENT_STATE;
volatile uint8_t PREVIOUS_STATE;
volatile uint32_t LAST_MUTE_TIMESTAMP;
volatile uint32_t LAST_STATE_TIMESTAMP;

void led_debug(uint8_t state);
void isr_interrupt();
void down_routine();
void up_routine();
void state_change();
void send_mute();

void setup() {
  Keyboard.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initial values, all states to whatever is present, last state timestamp to current time and first mute timestamp to 0
  CURRENT_STATE = digitalRead(BUTTON_PIN);
  PREVIOUS_STATE = CURRENT_STATE;
  LAST_STATE_TIMESTAMP = millis();
  LAST_MUTE_TIMESTAMP = 0;

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isr_interrupt, CHANGE);
}

void isr_interrupt(){
  uint8_t percieved_state = digitalRead(BUTTON_PIN);

  if(percieved_state == LOW){
    down_routine();
  }else{
    up_routine();
  }

  state_change();

  led_debug(percieved_state);
}

void down_routine(){
  if(CURRENT_STATE == HIGH){
    uint32_t current_timestamp = millis();
    if(current_timestamp - LAST_STATE_TIMESTAMP >= DEBOUNCE){
      CURRENT_STATE = LOW;
      LAST_STATE_TIMESTAMP = current_timestamp;
    }
  }
}

void up_routine(){
  if(CURRENT_STATE == LOW){
    uint32_t current_timestamp = millis();
    if(current_timestamp - LAST_STATE_TIMESTAMP >= DEBOUNCE){
      CURRENT_STATE = HIGH;
      LAST_STATE_TIMESTAMP = current_timestamp;
    }
  }
}

void state_change(){
  if(PREVIOUS_STATE != CURRENT_STATE){
    // detected a state change, now lets perform the adecuate logic depending on the change

    uint32_t current_mute_timestamp = millis();

    if(CURRENT_STATE == LOW){
      // this means the key has been pressed down, so in a down operation we allways mute
      send_mute();
      // And we update the last mute timestamp, as its relevant for the oposito operation relative to this one
      LAST_MUTE_TIMESTAMP = current_mute_timestamp;
    }else{
      // if is pulled high, we will perform a mute ONLY if enough time has elapsed since the last mute
      if(current_mute_timestamp - LAST_MUTE_TIMESTAMP > UNPRESS_TIMER_MAX){
        send_mute();
      }
    }

    // Lastly, we update the state to keep record
    PREVIOUS_STATE = CURRENT_STATE;
  }
}

void send_mute(){
  Keyboard.press(KEY_F24);
  Keyboard.release(KEY_F24);
}

void led_debug(uint8_t state){
  digitalWrite(LED_PIN, state);
}

void loop() {
  // Devoid of emotion
}
