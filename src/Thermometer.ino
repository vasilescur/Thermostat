/*
 * Project: Thermometer
 * Author:  Radu Vasilescu
 * Date:    2019-03-02
 */

//* Settings

// Servo parameters
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

// Connection pin to servo control wire
const int servoPin = D0;

//* Classes

class Thermometer {
    private:
        int currentAngle;

        int minAngle;
        int maxAngle;

        Servo* servo;

    public:
        //* Constructor and function visibilities
        Thermometer(Servo* servo, int minAngle, int maxAngle) {
            this->minAngle = minAngle;
            this->maxAngle = maxAngle;

            this->servo = servo;
        }

        /**
         * Make functions visible to the Particle cloud
         */
        void exposeFunctions() {
            Particle.function("setPercent", &Thermometer::_setAngleAsync, this);
            Particle.function("sysDelay", &Thermometer::_sysDelay, this);
            Particle.function("getAngle", &Thermometer::_getAngle, this);
            Particle.function("getMaxAngle", &Thermometer::_getAngleRange, this);
            Particle.function("getPercent", &Thermometer::getPercent, this);
            Particle.function("writeDirect", &Thermometer::_writeDirect, this);
            
            Particle.function("upByPercent", &Thermometer::upByPercent, this);
            Particle.function("downByPercent", &Thermometer::downByPercent, this);
        }

        //* Bullshit functions for web-based API

        int _setAngleAsync(String newAngle) {
            this->setAngleAsync(newAngle.toInt());
            return 1;
        }

        int _sysDelay(String ms) {
            this->sysDelay(ms.toInt());
            return 1;
        }

        int _getAngle(String args) {
            return this->getAngle();
        }

        int _getAngleRange(String args) {
            return this->getAngleRange();
        }

        int _writeDirect(String value) {
            return this->writeDirect(value.toInt());
        }

        //* Real getters

        int getAngle() {
            return this->currentAngle;
        }
        
        int getPercent(String args) {
            return (int) map(this->currentAngle, SERVO_MIN_ANGLE, SERVO_MAX_ANGLE, 0, 100);
        }

        int getAngleRange() {
            return SERVO_MAX_ANGLE;
        }

        //* Utility functions

        /**
         * Convert a desired percent to an angle
         */
        int percentToAngle(int percent) {
            return map(percent, 0, 100, this->minAngle, this->maxAngle);
        }

        int writeDirect(int value) {
            this->servo->write(value);
            return 1;
        }
        
        int upByPercent(String value) {
            int newPercent = this->getPercent("") + value.toInt();
            
            if (newPercent < 1) {
                newPercent = 1;
            }
            
            if (newPercent > 100) {
                newPercent = 100;
            }
            
            return this->setAngleAsync(percentToAngle(newPercent));
        }
        
        int downByPercent(String value) {
            int newPercent = this->getPercent("") - value.toInt();
            
            if (newPercent < 1) {
                newPercent = 1;
            }
            
            if (newPercent > 100) {
                newPercent = 100;
            }
            
            return this->setAngleAsync(percentToAngle(newPercent));
        }

        /**
         * Function wrapper for system delays to allow async calls from Particle
         * cloud without having to parse multiple arguments from a string.
         */
        void sysDelay(int ms) {
            delay(ms);
        }

        //* Setters

        /**
         * Asynchronously sets the angle of the servo. Begins instructing the servo
         * to move, and then returns true if successful.
         */
        bool setAngleAsync(int newAngle) {
            if (this->servo->attached()) {
                int controlValue = percentToAngle(newAngle);
                this->servo->write(controlValue);
                this->currentAngle = newAngle;
                return true;
            } else {
                return false;
            }
        }

        /**
         * Synchronously sets the angle of the servo. If an error occurs, the delay
         * time will still be executed.
         */
        bool setAngleSync(int newAngle, int delayMs) {
            bool success = this->setAngleAsync(newAngle);
            delay(delayMs);
            return success;
        }
};

//* Main Code

Servo* thermoServo;
Thermometer* thermo;

void setup() {
    // Set up servo
    thermoServo = new Servo();
    thermoServo->attach(servoPin);

    // Set up thermometer
    thermo = new Thermometer(
        thermoServo, 
        SERVO_MIN_ANGLE, 
        SERVO_MAX_ANGLE
    );

    // Publish functions to the Particle cloud
    thermo->exposeFunctions();

    // Initialize with an angle of half the angle range
    // (Delay of 1 second)
    bool success = thermo->setAngleSync(thermo->getAngleRange() / 2, 1000);
}

void loop() {

}