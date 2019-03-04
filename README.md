# Thermostat

A servo motor controller for my dorm room's thermostat, connected to the Internet of Things through the 
[Particle Cloud](https://www.particle.io).

## How it Works

I've connected a small analog servo motor to a Particle Photon board, and duct taped it to the thermostat so the gear on the
servo spins the temperature dial's gear. The code running on the Photon uses the lovely Particle API to set the angle of the
servo:

```c++
int controlValue = percentToAngle(newAngle);
this->servo->write(controlValue);
```

A set of functions is made available to be called using the Particle REST API, like so:

```c++
Particle.function("setPercent", &Thermometer::_setAngleAsync, this);
Particle.function("sysDelay", &Thermometer::_sysDelay, this);
Particle.function("getAngle", &Thermometer::_getAngle, this);
...
```

Then, I created a simple [IFTTT](https://ifttt.com) applet triggered by my Google Assistant that calls the appropriate
function through the Particle Cloud, and sets the temperature remotely.

## Building

Prerequisites:
- Particle CLI or Particle Desktop IDE or Particle Workbench

Make sure you have the correct Particle device target selected and run `particle compile <platform>`.

This program is intended to be run on the Particle [Photon](https://store.particle.io/products/photon) device, so the 
build and flash commands would be:

```bash
particle compile photon --saveTo Thermometer.bin
particle flash <device-name> Thermometer.bin
```

To get your device name, connect via USB and run

```bash
particle serial list
```

Or go to the [Particle Console](https://console.particle.io/devices).

## Contributing

Pull requests are welcome. 

To Do:

- [ ] Add a persistant "Favorite temperature" setting
- [ ] Create an Android/iOS mobile app to interface with the device
- [ ] Create a nice web interface
- [ ] 3D print a proper way to attach it to the thermostat
- [ ] User authentication for the API

## License

This code is available under the MIT License.
