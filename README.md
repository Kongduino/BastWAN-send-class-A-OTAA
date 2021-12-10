# BastWAN-send-class-A-OTAA

A sample sketch showing how to send LoRaWAN packets using a BastWAN and Keyboard Featherwing. See [this Twitter thread](https://twitter.com/Kongduino/status/1468828260238721026).

You need to add a `secret.h` file with your creds:

```c
const char *devEui = "0000000000000000";
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";
```

This code is configured for 915 MHz, SF10BW125. Your mileage may vary. `SF10BW125` may be changed in the code, line 60:

`lora.setDataRate(SF10BW125);`

The frequency range is set in the library's Config.h file.