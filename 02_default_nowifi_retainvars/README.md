# RF_DEFAULT mode, no wifi, retain variables

In this example we're going to store variables in a way that we can
reactivate them after a deep sleep, by using the EEPROM. Arduino libraries
provide a class `EEPROM` with a bunch of methods to read data from the EEPROM
and write data to it.

I need to include it in the sketch:

```
#include <EEPROM.h>
```

I defined a struct that contains some variables, and convenience methods
to access it (see `src/main.cpp`):

```
struct to_be_retained_s {
  int initializer;
  int value;
  bool flag;
};

void dump_retained_struct(struct to_be_retained_s *s);
void get_retained_struct(struct to_be_retained_s *s);
void put_retained_struct(struct to_be_retained_s *s);
void init_retained_struct();
```

`dump_retained_struct` prints out the contents of the struct to Serial,
`get_retained_struct` uses `EEPROM.get` to read it, `put_retained_struct` uses
`EEPROM.put` and `EEPROM.commit` to write to EEPROM.
`init_retained_struct` puts default values.

In `setup()`, we initialize the EEPROM and set default values:

```
void setup() {
  // ...
  EEPROM.begin(64);
  init_retained_struct();
}
```

When executing `setup()`, how do we know wether we're starting fresh or waking
up from deep sleep? The struct contains an initializer field which is checked in
`init_retained_struct`. If it does not match a given signature, we guess that we
started fresh and need to set values to zero. If it matches a given signature,
we woke up from sleep (and had sane values from before). This is a quick work-around,
a better implementation could additionally store and calculate a hash value over all struct fields to see if the structure is valid or not.

In `loop()` we always query our data structure via the functions above, into a stack-local variable, to always get the correct values from EEPROM:

```
void loop() {
  // (...)
  struct to_be_retained_s s;
  get_retained_struct(&s);
  s.flag = !s.flag;
  s.value += 1;
  put_retained_struct(&s);
  // (...)
}
```

Good, [but still no WiFi](../03_default_wifi/README.md)!
