# Default RF, Wifi

This example enables WiFi in a way that it's turned on in setup(), every time the device wakes up from sleep.

This can be used to i.e. sleep for a larger amount of time (say 5 minutes),
measure something, connect to WiFi, send values off to some backend and go
to sleep again. For testing purposes the sleep amount is set to 5 seconds.

We're using `RF_DEFAULT` as the parameter to deep sleep, so the RF unit is kept active during sleep:

```
  ESP.deepSleep(num_seconds_to_sleep * 1000000, RF_DEFAULT);
```

That works, but does consume power during sleep.

```
--- Miniterm on /dev/cu.SLAB_USBtoUART  9600,8,N,1 ---
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
..........................ip:172.20.10.12,mask:255.255.255.240,gw:172.20.10.1
in loop()
We're connected, do something with wifi..
deep sleep, num_seconds=5
state: 5 -> 0 (0)
rm 0
del if0
usl
 �N?�$�¶␖��␔␡saQ��4�����)
f r0, ..scandone
state: 0 -> 2 (b0)
state: 2 -> 3 (0)
state: 3 -> 5 (10)
add 0
aid 1
cnt
.
connected with ThingforwardWiFi, channel 1
dhcp client start...
..................................................................ip:172.20.10.12,mask:255.255.255.240,gw:172.20.10.1
in loop()
We're connected, do something with wifi..
```

ESP8266 supports RF_DISABLED which effectively turns of all RF functionality, and consumes much less power.
Unfortunately, changing `RF_DEFAULT` to `RF_DISABLED` breaks the sketch:

```
We're connected, do something with wifi..
deep sleep, num_seconds=5
state: 5 -> 0 (0)
rm 0
del if0
usl
 �N?�$�¦␖��␔OC␌A��4�����)
f r0, ..scandone
no ThingforwardWiFi found, reconnect after 1s
.reconnect
f 0, .error: pll_cal exceeds 2ms!!!
.error: pll_cal exceeds 2ms!!!
.error: pll_cal exceeds 2ms!!!
.error: pll_cal exceeds 2ms!!!
error: pll_cal exceeds 2ms!!!
..error: pll_cal exceeds 2ms!!!
error: pll_cal exceeds 2ms!!!
.error: pll_cal exceeds 2ms!!!
```

Device wakes up after deep sleep, but in `setup()` our calls to `WiFi.begin()` etc. fail because RF is still disabled.
Blog posts state that after a `ESP.deepSleep(... RF_DISABLED)` another call to `ESP.deepSleep(..., RF_DEFAULT) is necessary.

This sounds easy, but the second call to ESP.deepSleep with `RF_DEFAULT` again puts the device to sleep. So when waking up,
we need to know wether the last call to ESP.deepSleep has been made with RF DISABLED or RF DEFAULT. [We're going to use the EEPROM stuff for that](../04_disablerf_wifi_retainstate/README.md).
