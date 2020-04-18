#define VBATPIN A6
#define BA 4
#define B1 2
#define B2 3
#define T1 0x00001388 //5000 in hex
#define T2 0x00002710 //10000 in hex
#define MASK 0b11000111

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(BA, INPUT_PULLUP);
  Serial.begin(9600);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  unsigned long sound_alarm_at = 0;
  uint8_t b1_history = 0;
  uint8_t b2_history = 0;
  uint8_t ba_history = 0;

  while(1) {
    update_button(&b1_history, B1);
    update_button(&b2_history, B2);
    update_button(&ba_history, BA);

    if(is_button_pressed(&b1_history) || is_button_pressed(&b2_history)){
      Serial.print((unsigned long) 0 - is_button_down(&b1_history));
      sound_alarm_at = millis() + 
                       (T1 & ((unsigned long) 0 - is_button_down(&b1_history))) +
                       (T2 & ((unsigned long) 0 - is_button_down(&b2_history)));
    }

    if((millis() > sound_alarm_at) && (sound_alarm_at != 0)){
      digitalWrite(LED_BUILTIN, HIGH);
      sound_alarm_at = 0;
    }

    if(is_button_pressed(&ba_history)){
      digitalWrite(LED_BUILTIN, LOW);
      sound_alarm_at = 0;
    }

    Serial.print(sound_alarm_at);
    Serial.print(", ");
    float mesuredVBat = analogRead(VBATPIN);
    mesuredVBat *= 2;
    mesuredVBat *= 3.3;
    mesuredVBat /= 1024;
    Serial.print("VBat: "); 
    Serial.println(mesuredVBat);
  }
}

void update_button(uint8_t *button_history, const uint8_t pin_number){
  *button_history = *button_history << 1;
  *button_history |= (digitalRead(pin_number) == 0);
}

uint8_t is_button_pressed(uint8_t *button_history){
  uint8_t pressed = 0; 
  if((*button_history & MASK) == 0b00000111){
    pressed = 1;
    *button_history = 0b11111111;
  }
  return pressed;
}

uint8_t is_button_released(uint8_t *button_history){
  uint8_t released = 0; 
  if((*button_history & MASK) == 0b11000000){
    released = 1;
    *button_history = 0b00000000;
  }
  return released;
}

uint8_t is_button_down(uint8_t *button_history){
  return (*button_history == 0b11111111);
}

uint8_t is_button_up(uint8_t *button_history){
  return (*button_history == 0b00000000);
}
