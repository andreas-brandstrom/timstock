#define VBATPIN A6
#define B1 2
#define B2 3
#define B3 4
bool ten_sec_alarm;
bool twenty_sec_alarm;
unsigned long sound_alarm_at; 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  Serial.begin(9600);
  sound_alarm_at = 0; 
  ten_sec_alarm = 0;
  twenty_sec_alarm = 0;
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  bool b1 = b1_is_pressed();
  ten_sec_alarm |= b1;
  bool b2 = b2_is_pressed();
  twenty_sec_alarm  |= b2;
  bool b3 = b3_is_pressed();

  if(ten_sec_alarm && (sound_alarm_at == 0)){
    sound_alarm_at = millis() + 10000;
    ten_sec_alarm = 0;  
  }
   
  if(twenty_sec_alarm && (sound_alarm_at == 0)){
    sound_alarm_at = millis() + 20000;
    twenty_sec_alarm = 0;
  }

  if((millis() > sound_alarm_at) && (sound_alarm_at != 0)){
    digitalWrite(LED_BUILTIN, HIGH);
    sound_alarm_at = 0;
  }

  if(digitalRead(LED_BUILTIN) && b3){
    digitalWrite(LED_BUILTIN, LOW);
  }

  Serial.print(sound_alarm_at);
  Serial.print("(");
  Serial.print(b1);
  Serial.print(",");
  Serial.print(b2);
  Serial.print(",");
  Serial.print(b3);
  Serial.print(") ");
  float mesuredVBat = analogRead(VBATPIN);
  mesuredVBat *= 2;
  mesuredVBat *= 3.3;
  mesuredVBat /= 1024;
  Serial.print("VBat: "); 
  Serial.println(mesuredVBat);
}

uint8_t b1_is_pressed(void){
  static uint8_t button_history = 0;
  return is_pressed(&button_history, B1);
}

uint8_t b2_is_pressed(void){
  static uint8_t button_history = 0;
  return is_pressed(&button_history, B2);
}

int8_t b3_is_pressed(void){
  static uint8_t button_history = 0;
  return is_pressed(&button_history, B3);
}

uint8_t is_pressed(uint8_t *button_history, uint8_t pin_number){
  uint8_t pressed = 0;
 
  *button_history = *button_history << 1;
  *button_history |= (digitalRead(pin_number) == 0);
  if((*button_history & 0b11000111) == 0b00000111){
    pressed = 1;
    *button_history = 0b11111111;
  }
  return pressed;
}
