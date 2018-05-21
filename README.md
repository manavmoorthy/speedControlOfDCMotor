# speedControlOfDCMotor

Designed a closed loop speed control system for a DC motor which takes in a reference speed in RPM, using an Arduino microcontroller. The tuning of the control loop is done using a mathematical model utilizing a dynamic proportional gain parameter that minimizes the settling time for the motor (when a new desired RPM is entered) and the overshoot to achieve stability in motor performance.

The three stages involved in the design of this project included:
- Designing a tachometer.
- Achieving the motor to run at desired speed with minimal overshoot and settling time.
- Load stabilization i.e., make the motor run at desired speed even under the action of load.

The process flow for speed control of the DC Motor under both stable and load conditions are listed below:
- Enter the desired speed on the serial monitor.
- Parse the input and find the error between the actual (current) speed and the entered speed of the DC motor.
- Using the proportional gain parameter information priors, dynamically change the analog value to be written to the motor until a stable condition is observed.

The hardware components necessary for the implementation include:
- Arduino Microcontroller
- 12 V DC Motor
- Digital IR Sensor
USE: To measure the number of revolutions, by using interrupt when the flap is right in front of the IR sensor. It gives an output 1 when the motor flap is in front, covering it, else gives output 0.
- KA 7812 Voltage Regulator
USE: To regulate flow of 12 volts across the DC Motor.
- Diode (IN4007)
USE: Used to prevent back EMF in the circuit.
- Transistor
USE: Acts as a control switch
- Resistor 