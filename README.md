
# iot

Some Arduino projects to make life easier and a DIY way to make a home smart

### Car_in_garage_detector1

This is a **arduino** based data transmitter using the **NRF24** radio module to be installed in a garage to detect and transmit following values

1. Door Open (Garage lights turns on automaticaly)
2. Door Close (Garage lights turns off automaticaly)
3. Vehicle availability (using a **HC-SR04 ultrasonic sensor**)
4. Air quality level
5. Temperature
6. Humidity level


### iot_wireless_data_receiver

This is the receiving end for the above "Car_in_garage_detector1" transmitter. Should be hosted on another board connected to a **NRF24** radio module
This board should be connected over USB to another computer (eg: a Raspberry Pi) . That computer will host the **Server**.

### Server

This server is written using the **ExpressJS** framework based on **NodeJS**.
It continuesly reads the USB serial connection of the arduino receiver board to read the transmissions being sent from the **Car_in_garage_detector1** board to the **iot_wireless_data_receiver**.
If any required values were received, the **Server* will call some webhooks of a **Home Assistant** instance to update it's sensors and trigger alerts



