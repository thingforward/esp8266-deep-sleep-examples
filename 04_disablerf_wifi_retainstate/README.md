# Disabled RF, WiFi, retain wakeup state

This is the last part where we're putting all the stuff together. We'll use
the DISABLE_RF flag when going to deep sleep, which will consume the least amount
of energy when in sleep, we'll activate WiFi the correct way and we're using
the EEPROM to remember the wake up state after a call to ESP.deepSleep.

The correct order is first to call `ESP.deepSleep(<sleep amount>, RF_DISABLED);`, followed by a
`ESP.deepSleep(1, WAKE_RF_DEFAULT);` which brings the RF module into a functing state.

We create a struct that contains a state variable.
* CONNECT_STATE_NONE means "not connected, no nothing"
* CONNECT_STATE_PREPARE means "woke up with RF_DISABLED, going to deepSleep with WAKE_RF_DEFAULT"
* CONNECT_STATE_CONNECTING means "woke from WAKE_RF_DEFAULT, starting to connect to WiFi"
* CONNECT_STATE_CONNECTED means, yeah you guessed it.

```c
#define CONNECT_STATE_NONE				0
#define CONNECT_STATE_PREPARE			1
#define CONNECT_STATE_CONNECTING	2
#define CONNECT_STATE_CONNECTED		3

struct wifi_state_retain_s {
	int initializer = 0b10101010;
	int connect_state = CONNECT_STATE_NONE;
};
```

Three functions to deal with this struct, read/write from/to EEPROM and initialize properly:

```c
void get_wifi_state(struct wifi_state_retain_s *s);
void put_wifi_state(struct wifi_state_retain_s *s);
void init_wifi_state();
```

In `setup()` wifi is turned off:

```
// turn off Wifi and disable features we do not want.
WiFi.mode(WIFI_OFF);
WiFi.disconnect(true);
WiFi.setAutoConnect(false);
WiFi.stopSmartConfig();
```

Main `loop()` works its way through a mini state machine. I connected a button
to D1. When in CONNECT_STATE_NONE it checks the button. If it's not pressed, it
goes to deep sleep with RF_DISABLED. If it's pressed, it continues with CONNECT_STATE_PREPARE.

From CONNECT_STATE_PREPARE it goes to CONNECT_STATE_CONNECTING and CONNECT_STATE_CONNECTED.
When in CONNECT_STATE_CONNECTED, we can do something with the Wifi, and going to sleep afterwards.

Looking a the serial output:

```
setup() done.
in loop()
connect_state=none
deep sleep, num_seconds5
0�~?�4�Ҷ␖��4␡sAQ��h�����)
setup() done.
in loop()
connect_state=none
in loop()
connect_state=prepare
 �n?�4�҆␆��␔OCAA��h�����)
setup() done.
in loop()
connect_state=connecting
mode : sta(5c:cf:7f:80:8b:ce)
add if0
f r0, ......................scandone
state: 0 -> 2 (b0)
state: 2 -> 3 (0)
state: 3 -> 5 (10)
add 0
aid 1
cnt
.
connected with ThingForwardWiFi, channel 1
dhcp client start...
..............................ip:172.20.10.12,mask:255.255.255.240,gw:172.20.10.1
in loop()
connect_state=connected
We're connected, do something with wifi..
going to deep sleep, num_seconds=5
state: 5 -> 0 (0)
rm 0
del if0
usl

```

During the first loop, the button was not pressed. In the second loop, it was,
and it continued by preparing and connecting to WiFi. After that it goes to deepSleep
and one can see WiFi turned off (rm 0, del if0, usl).

The ChargerDoctor shows <0.01 Amps when in deep Sleep mit RF_DISABLE, and that's
the best i could reach. If somebody has more hints on saving energy, please let me know :)

Best,
Andreas
