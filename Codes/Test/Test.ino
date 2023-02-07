#include "TinyGPS.h"
TinyGPS gps

void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available())
  {
    int c = Serial.read();
    if (gps.encode(c))
    {
      // process new gps info here
      long lat, lon;
      unsigned long fix_age, time, date, speed, course;
      unsigned long chars;
      unsigned short sentences, failed_checksum;
      // retrieves +/- lat/long in 100000ths of a degree
      gps.get_position(&lat, &lon, &fix_age);
      // time in hhmmsscc, date in ddmmyy
      gps.get_datetime(&date, &time, &fix_age);
      // returns speed in 100ths of a knot
      speed = gps.speed();
      // course in 100ths of a degree
      course = gps.course();
    }
  }
}
