
Radar Class provides an interface for controlling 2 cannons by  a radar system. 

The Radar class contains several public methods for interacting with the system, as well as several private helper methods. The public methods include:

Radar(): the constructor method for creating an instance of the Radar class, which takes several input parameters, including pins for the ultrasonic sensor, servo motor, and lasers, as well as values for the radar speed and cannon distance.

begin(): a method for initializing the Serial communication at a specified baud rate.

getDistance(): a method for obtaining the current distance measured by the ultrasonic sensor.

getAngle(): a method for obtaining the current angle of the radar system.

calculatePID(): a method for calculating the PID value based on the input and set point.

setStripLEDColor(): a method for setting the color of an individual LED in a strip of LEDs.

setNeoPixelsColor(): a method for setting the color of a series of NeoPixels.

scan(): a method for scanning the radar system.

shoot(): a method for shooting the Laser cannon at a specified 

calculateDist(): a method for calculating the distance based on the angle.

The private methods include:

printInfo(): a method for printing internal information to the serial monitor.

ShowInfoLcd(): a method for showing error messages on an LCD display.

showEncoderReadingOnLCD(): a method for displaying encoder readings on an LCD display.

degtoradian(): a helper method for converting degrees to radians.

radiantodeg(): a helper method for converting radians to degrees.