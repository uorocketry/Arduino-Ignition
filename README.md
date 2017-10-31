# Arduino-Ignition
Arduino code for wireless hybrid engine ignition test rig.

The igniter software is a state machine that is controlled by input from the serial port.  
In our case, the serial port is used by an XBEE pro 900HP radio, giving us wireless control of the igniter.

## State Machine

The state machine has 6 states it can be in, each corresponding to a number:

State|Number|Green LED|Red LED|Action
:---:|:---:|:---:|:---:|---
`STOP`|1|ON|OFF|Close the valve, turn the igniter relay off.
`IGNITE`|2|OFF|ON|Turn the igniter relay on, starting the igniter.
`OPEN`|3|ON|ON|Keep the igniter relay on, open the valve, keep it open for 10 seconds
`CLOSE`|4|ON|OFF|Close the valve
`WAIT`|5|ON|OFF|Do nothing
`PING`|6|---|---|Send ``"I AM ALIVE!"`` to the base station over the radio

The state machine is written such that the user can go to  the `STOP` state from any of the other states at any time. This is a safety feature, so that the engine test can be aborted and stopped at any time.

## State Transitions
The igniter starts in the `CLOSE` state.  It waits to receive serial input.

The following are two typical state flows:
##### Complete Run
1. Power on -> `CLOSE`
2. User presses `ping` button at base station -> `PING`
3. User presses `start` button at base station -> `IGNITE`
4. Automatic transition: `IGNITE`->`OPEN`
5. Valve opens and stays open for 10 seconds
6. Automatic transition: `OPEN`->`CLOSE`
7. Automatic transition: `CLOSE`->`WAIT`

##### Aborted Run
1. Power on -> `CLOSE`
2. User presses `ping` button at base station -> `PING`
3. User presses `start` button at base station -> `IGNITE`
4. Automatic transition: `IGNITE`->`OPEN`
5. **User presses `stop` button at any time during any state -> `STOP`**
6. Automatic transition: `STOP`->`CLOSE`
7. Automatic transition: `CLOSE`->`WAIT`


## Radio information:
An Xbee Pro 900HP radio is used both for the igniter and the base station.

The XBee packet includes a checksum, which ensures that the data received by one XBee is the same as the data sent by the other XBee.

Parameter|Value|
---|---  
Specified Outdoor Range|	10 Kbps: up to 9 miles (14 km) ( w/ 2.1 dB dipole antennas)
Tested Range | 100m
Range to be used | 50m
Transmit Power|	Up to 24 dBm (250 mW)  
Receiver Sensitivity	| -110 dBm @ 10 Kbps
