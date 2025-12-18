//Project Title: Embedded systems Robotdog
//Author: Daan Smit
//Date: 11-12-2025
//Version: 4

 /* Description:
 *   This code is functional for using a 5 bar linkage with given L1 and L2 to calculate the angels using inverse kinematics
 *   The effector point C can be moved anywhere to specific coordinates within the 5 bar linkage operating limits
 *
 * Hardware:
 *   - Arduino nano
 *   - 2 x mg50 servo's
 *
 * Software/Libraries:
 *   - Servo.h
  *  - Math.h
 *
 * Credits/References:
 *   - Ilja Rukin
 *   - Tazar Technical
*    - circuiTician
 *
 * License:
 *   - non
 */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//REQUIRED LIBARIES
#include <Servo.h>
#include <math.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Servo servo1; 
Servo servo2; 

int servopin1 = 9;   //Servo op pin 9 arduino nano PWM pin
int servopin2 = 10;  //Servo op pin 10 arduino nano PWM pin

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Global variables
const float L1 = 65.0; 
const float L2 = 85.0;
const float d = 15;

float alpha1, alpha2, alpha;
float beta1, beta2, beta;

float xC = 15;
float yC = 125; 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Function
void inverseKinematics(float x, float y);


//========================================================================================================================//
//                                                 MAIN SETUP                                                             //                                                                 
//========================================================================================================================//

void setup() {
  Serial.begin(9600);  //9600 for more stability on nano
  
  servo1.attach(servopin1);
  servo2.attach(servopin2);
  
  Serial.println("Arduino Nano Servo Control Started");
  Serial.print("Servo 1 on pin "); Serial.println(servopin1);
  Serial.print("Servo 2 on pin "); Serial.println(servopin2);
}

//========================================================================================================================//
//                                                 MAIN LOOP                                                              //                                                                 
//========================================================================================================================//

void loop() { 

  //Configured as a triangle movement for walking
  //The code is without proper interpolation movement will be hard and sharp

  //Position 1
  xC =  15 + 25;  //25 mm to the right from start position
  yC =  155 - 25; //25 mm movement down the y start position
  inverseKinematics(xC, yC);  //Write the xC and xY to the IK function
  Serial.print("Pos1: alpha=");
  Serial.print(alpha);  
  Serial.print(" beta="); 
  Serial.println(beta);
  servo1.write(alpha);  //Write the angle alpha to servo1
  servo2.write(beta); //Write the angle beta to servo2
  delay(500);

  //Position 2
  xC =  15 - 25;  //25 mm to the left from start position
  yC =  155 - 25; //25 mm down the y start position 
  inverseKinematics(xC, yC);
  Serial.print("Pos1: alpha="); 
  Serial.print(alpha);
  Serial.print(" beta="); 
  Serial.println(beta);
  servo1.write(alpha);
  servo2.write(beta);
  delay(500); 


  //Position 3
  xC =  15 + 0; //The start position
  yC = 155 - 45;  //45 mm down the y start position
  inverseKinematics(xC, yC);
  Serial.print("Pos1: alpha="); 
  Serial.print(alpha);
  Serial.print(" beta="); 
  Serial.println(beta);
  servo1.write(alpha);
  servo2.write(beta);
  delay(500);
}

//========================================================================================================================//
//                                                 Inverse Kinematics functie                                             //                                                                 
//========================================================================================================================//

void inverseKinematics(float x, float y){
  //Calculations for lines c and e
  float c = sqrt(x*x+y*y);  
  float e = sqrt((d - x)*(d - x) + y*y);  
  
  //Using atan2 for calculating angla alpha1
  alpha1 = atan2(y, x);
  
  //Using constrain function so it limits it between -1 and 1 so that no NaN will come through
  float cosAlpha2 = (L2*L2 - c*c - L1*L1) / (-2*L1*c);
  cosAlpha2 = constrain(cosAlpha2, -1.0, 1.0);
  alpha2 = acos(cosAlpha2);

  beta1 = atan2(y, d - x);
  float cosBeta2 = (L2*L2-e*e-L1*L1)/(-2*L1*e);
  cosBeta2 = constrain(cosBeta2, -1.0, 1.0);
  beta2 = acos(cosBeta2);

   //From Radians to degrees using the 180/PI
  alpha1 = alpha1 * 180.0 / PI;
  alpha2 = alpha2 * 180.0 / PI;
  beta1  = beta1  * 180.0 / PI;
  beta2  = beta2  * 180.0 / PI;

  //Calculating the final alpha and beta angles used for controlling the servo's
  alpha = alpha1+alpha2;
  beta = (180-(beta1+beta2));
}