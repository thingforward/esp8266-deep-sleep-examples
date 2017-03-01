# RF_DEFAULT mode, no wifi

This is the easiest example to play around with `ESP.deepSleep`.
The sketch in `src/main.cpp` is simple: It defines an int variable
called `value`, initialized to 0.

`setup()` starts the Serial object and dumps out `value`.

`loop()` dumps out `value` and sends the device to deep sleep.

For this to work, two things are necessary. First, GPIO16 has to be connected to RST. On my NodeMCU, GPIO16 is D0, and i connect it
directly. When it is connected, flashing does not work, at least not when the device is actually in deep sleep (obvious reason). So make sure to unplug this when flashing firmware.

Second, D0 has to be configured, which is done in `setup()` using:

```
  pinMode(D0,WAKEUP_PULLUP);
```

Running this yields the following on serial monitor:

```
$ pio run -t upload && pio device monitor
(...)
--- Miniterm on /dev/cu.SLAB_USBtoUART  9600,8,N,1 ---
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
 �n?�4�Ҧ␆��␔_SAQ��(�␋������)
value=0
in loop()
value=3
0�~?�4�¦␆��␔OCAA��(�␋������)
value=0
in loop()
value=3
0�~?�$�¶␖��␔_SAQ��(�␋������)
value=0
in loop()
value=3
```

So what's happening there? The garbage output appears whenever the device wakes up from deep sleep. Then, `setup()` is executed, dumping `value=0`. After that comes `loop()`, setting and dumping `value=3`.

Finally, `ESP.deepSleep(num_seconds_to_sleep * 1000000, RF_DEFAULT);` sends the device to deep sleep, then it goes "reset & rewind".

Looking at a current meter shows that during sleep, very little power is consumed (my USB charger doctor has 3 digits, so anything below 0.01 amps is displayed as 0). When resetting, the current meter goes up to 0.07A.

From the output of `value` it's clear that we lost our SRAM contents every time the device goes to sleep. [How can we save variables for later usage?](../02_default_nowifi_retainvars/README.md)
