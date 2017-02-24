# esp8266-deep-sleep-examples

This repository is a example code base demonstrating some
aspects of ESP8266' `ESP.deepSleep` function for the Arduino platform. It results from my experiments when hacking on a DIY project.

When you tried deep sleep on an ESP8266 and wondered,
* where is my memory gone?
* why is the wifi not working as expected?
* are there some patterns for this stuff?

then please read on.

The examples do not come with a build system, but can be run using [PlatformIO](www.platformio.org).

4 flavors:

* [The bare minimum of understanding deep sleep](01_default_nowifi/README.md)
* [How to retain variables](02_default_nowifi_retainvars/README.md)
* [Wifi](03_default_wifi/README.md)
* [Wifi, extra power save](04_disablerf_wifi_retainstate/README.md)
