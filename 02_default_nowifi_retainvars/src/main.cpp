#include <Arduino.h>
#include <EEPROM.h>

int num_seconds_to_sleep = 5;

// structure for some sample variables
struct to_be_retained_s {
  int initializer = 0b10101010;
  int value = 0;
  bool flag = false;
};

// dump to Serial
void dump_retained_struct(struct to_be_retained_s *s) {
  Serial.print("value="); Serial.print(s->value);
  Serial.print(",flag="); Serial.println(s->flag?"true":"false");
}

// read struct content from eeprom
void get_retained_struct(struct to_be_retained_s *s) {
  EEPROM.get(0,*s);
}

// write struct content to eeprom
void put_retained_struct(struct to_be_retained_s *s) {
  EEPROM.put(0,*s);
  EEPROM.commit();
}

// ensure  struct is initialized with defaults.
void init_retained_struct() {
  struct to_be_retained_s s;
  get_retained_struct(&s);
  if (s.initializer != 0b10101010) {
    s.initializer = 0b10101010;
    s.flag = false;
    s.value = 0;
    put_retained_struct(&s);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("in setup()");

  // initialize EEPROM with given size
  EEPROM.begin(64);
  init_retained_struct();

  pinMode(D0,WAKEUP_PULLUP);
}

void loop() {
  Serial.println("in loop()");

  // read struct from eeprom, modify values,
  // save back and dump to Serial.
  struct to_be_retained_s s;
  get_retained_struct(&s);
  s.flag = !s.flag;
  s.value += 1;
  put_retained_struct(&s);

  Serial.print("Updated struct to: ");
  dump_retained_struct(&s);

  ESP.deepSleep(num_seconds_to_sleep * 1000000, RF_DEFAULT);
}
