//robot_scan
//by Stephen M. Brown
//for DelDotSci

//This is the beginning of the scan and follow program for both the Dracuum and the FollowBot
//This uses a neck servo mounted ultrasonic sensor that hunts for the closes object and tries to 
//repeatedly scan the object for movement.  If no movement found after a couple of seconds, it 
//returns to scanning.  At this point no body movement will be coded, but eventually the whole body
//will turn to place the object at 90degrees of the servo.

//declare variables
//declare functions
//void loop
//scan from 0degrees to 180 degrees. Keeping track of closest readings of angle and distance and angular size
//scan back from 180 degress to zero keeping track of closest readings of angle and distance and angular size.
//go to center of contact and scan from +25 degrees to -25 degrees (this is updated contact info)
//if no changes for 5 seconds return to scanning...

                                             //INCLUDE
#include <Servo.h>

                                             //PIN ASSIGNMENTS
int inputPin=A0;  //input for the ultrasonic rangefinder
int outputPin=A1;  //output for the ultrasonic rangefinder
int ledpin=13;
                                             //GLOBAL VARIABLES
int angle=0;    //initial angle for the servo
Servo servo_neck;  //initialize servo_neck.  Not sure if this is the only servo of the bot
int closest_distance = 1000;
                                             //FUNCTIONS
                                                 //function:range_find()
int range_find()
  {
/*Main range finding function for ultrasonic emitter/detector
 * Nothing is sent to it, just returns 'distance'
 * not sure--still--of necessary timing  between shutting off the emiitter and turning on the detector
 *
 * .3m/millisecond
 */
  digitalWrite(outputPin, LOW);  //turn off emiitter  2 microseconds enough?  Seems to work...
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);//turn on emitter for a pulse
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);//turn off emitter

  int distance = pulseIn(inputPin, HIGH); //determine the length of the pulse(physically this make more sense to me if
  distance = distance/58;                 //this would be measuring the time LOW.  but this is from the 
  return distance;                        //example code for the ultrasonic detector...
  delay(50);
  if (distance >= 50)
  {
    digitalWrite(ledpin, HIGH);
  }
  else
  {
    digitalWrite(ledpin, LOW);
  }
}

                                                   //function:tight_scan()
int tight_scan(int target_angle, int target_distance)
{
  /*initializes a tighter scan of a sensed target
   * will zero on the closest object (sent to function)
   * and do scanning sweeps of the object
   * 
   */

  int orig_target_distance = target_distance;//sets the base line for the detected object(called target here)
  int delta_angle = 0;
  int target_angular_size = 5; //degrees  initial guess for target angular size
  //find angular size
    servo_neck.write(target_angle);  //goes to the target angle
          //maybe a smaller move for servo.write?
    for(delta_angle = 0 ; delta_angle < 15; delta_angle+=1)
    {
        delay(100);
      int current_distance = range_find();
      int count = 0;
      //while target_distance is within 5percent of original target_distance
      //keep tight_scan going....
      while (target_distance < 1.05*orig_target_distance)
      {
        target_distance = range_find();
        delay(100);
        servo_neck.write(target_angle+delta_angle);
        delta_angle+=1;
      }
     
     }
}
                                                  //function:scan()
int scan()
{
  /*Main scanning function for the FollowBot
   * 
   */
  int target_angle = 90;

  for(angle = 0; angle < 180; angle += 1)
  {
    servo_neck.write(angle);
    int current_distance = range_find();
    Serial.println(current_distance);       //added to use serial plotter.
    Serial.println(" ");
    if (current_distance < closest_distance)
      {
        closest_distance = current_distance;
        tight_scan(angle, current_distance);
      }
    else
      delay(30);  //tested to find something that doesnt task the servo
      continue;
   }
   
    for(angle = 180; angle>0; angle-=1)
    {
      servo_neck.write(angle);
      int current_distance = range_find();
      Serial.println(current_distance);
      if (current_distance < closest_distance)
        {
          closest_distance = current_distance;
          tight_scan(angle, current_distance);
        }
      else
        delay(30);
        continue;
   }
}
                                                  //ONE-TIME RUN
void setup() 
  {
  servo_neck.attach(3);
  Serial.begin(9600);   //for serial board communications
  }
                                                  //MAIN-LOOP
void loop() 
{
 scan();
}
