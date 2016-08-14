// Code for Hybrid Bot - Written for Arduino Mega. 
// Team Robocon BITS Pilani - Goa 
// Last update - 26 February 2016

#include <Herkulex.h>
#include <Servo.h>
#include <Wire.h>

#define RX   15       // Connected with HerkuleX TX Pin
#define TX   14        // Connected with HerkuleX RX Pin
#define MOTORID1   219 // This is for ARM    HerkuleX Servo Motor ID
#define MOTORID2   221  // This is for BLDC Servo - used as a fan

#define FAN 12
#define m1in1 52   
#define m1in2 50
#define m2in1 48
#define m2in2 44
#define m3in1 42 
#define m3in2 38 
#define pwm1 2
#define pwm2 3
#define pwm3 4 

byte a[18]; // Array for PS2 controller values

/********************List of Buttons**************************/
/*    a[0] = ps2.readButton(PS2_SQUARE);
 a[1] = ps2.readButton(PS2_CIRCLE);
 a[2] = ps2.readButton(PS2_TRIANGLE);
 a[3] = ps2.readButton(PS2_CROSS);
 a[4] = ps2.readButton(PS2_LEFT_1);
 a[5] = ps2.readButton(PS2_RIGHT_1);
 a[6] = ps2.readButton(PS2_LEFT_2);
 a[7] = ps2.readButton(PS2_RIGHT_2);
 a[8] = ps2.readButton(PS2_UP);
 a[9] = ps2.readButton(PS2_DOWN);
 a[10] = ps2.readButton(PS2_LEFT);
 a[11] = ps2.readButton(PS2_RIGHT);
 a[12] = ps2.readButton(PS2_SELECT);
 a[13] = ps2.readButton(PS2_START);
 a[14] = ps2.readButton(PS2_JOYSTICK_LEFT_X_AXIS);
 a[15] = ps2.readButton(PS2_JOYSTICK_LEFT_Y_AXIS);
 a[16] = ps2.readButton(PS2_JOYSTICK_RIGHT_X_AXIS);
 a[17] = ps2.readButton(PS2_JOYSTICK_RIGHT_Y_AXIS);
 */

int m1=0,m2=0,m3=0; // Movement speed of driving motors
int deadz=15; // deadzone for PS2 analogs

int angle = 0;
int bldcangle = 0;
int rot = 0;

Servo ms; // BLDC at pin 8
int velo = 66;
int blgr = 0;
int big=170;
int small=120;
int gear = 0;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Herkulex.beginSerial3(115200);

  Herkulex.reboot(MOTORID2); //reboot first motor
  delay(500); 
  Herkulex.initialize(); //initialize motors
  delay(200);  


  // HerkuleX.moveAngle(MOTORID1, 0, 100, HERKULEX_LED_RED);
  delay(500);
  Herkulex.moveOneAngle(MOTORID2, 50, 100,LED_BLUE);

  ms.attach(12);
  delay(500);
  ms.write(179);
  delay(2000); //  D - D - D – D ( Sound)
  ms.write(68); // MIN value
  delay(2000);

  pinMode(m1in1,OUTPUT);
  pinMode(m1in2,OUTPUT);
  pinMode(m2in1,OUTPUT);
  pinMode(m2in2,OUTPUT);
  pinMode(m3in1,OUTPUT);
  pinMode(m3in2,OUTPUT);
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(pwm3,OUTPUT);
}


void loop() 
{
  Wire.requestFrom(5,18);
  for(int i=0;i<18;i++)
  {
    a[i]=Wire.read();
//    Serial.print(a[i]);
//    Serial.print(" ");
  }
  if(a[10] == 0)
  {
    gear = gear+1;
  }
  
  if(a[11] == 0)
  {
    blgr = blgr+1;
  }
  
   
/*Serial.print(a[16]);
Serial.print("  ");
Serial.print(a[17]);
Serial.print("  ");
Serial.println(a[15]);*/

  Drive_Base();
  m1 = m2 = m3 = 0;
  /*if(a[8] == 0)
  {
    angle = angle + 30;
    //  HerkuleX.moveAngle(MOTORID1, angle, 100, HERKULEX_LED_RED);
  }
  if(a[9] == 0)
  {
    angle = angle - 30;
    //    HerkuleX.moveAngle(MOTORID1, angle, 100, HERKULEX_LED_RED);
  } */
  if(a[6] == 0)
  {
    angle = angle + 30;
    Herkulex.moveOneAngle(MOTORID2, (angle), 100, LED_BLUE);
  } 
  if(a[4] == 0)
  {
    angle = angle - 30;
    Herkulex.moveOneAngle(MOTORID2, (angle), 100, LED_BLUE);
  } 

  if(angle> 50 )
    angle = 50;

  if(angle <-160)
    angle = -160;
  
  Drive_BLDC();



}

void Drive_BLDC()
{
  if((blgr%2)==0)
  {
  if(a[5]==0 )
  {
    velo = velo + 10;
    ms.write(velo);
  }
  else
    if(a[7]==0 )
    {
      velo = velo - 15;
      ms.write(velo);
    }      

 if(velo < 70)
 {
   velo = 66;
   ms.write(66);
 }
 if(velo > 175)
 {
   velo = 170;
   ms.write(170);
 }
  delay(200);
  }
  
  else
  {
  if(a[5]==0 )
  {
    ms.write(175); 
  }
  else
    if(a[7]==0 )
    {
      ms.write(110);
    }      

    else 
      ms.write(66); 
  delay(200);
}
}

void Drive_Base()
{
//  if(a[16] == 0) // When LEFT ANALOG stick moved to LEFT move left ie. X-axis -ve 
 // {
 //   m1 = m2 = m3 = 60;
//    digitalWrite(m1in1,HIGH);
//    digitalWrite(m1in2,LOW);
//    digitalWrite(m2in1,HIGH);
//    digitalWrite(m2in2,LOW);
//    digitalWrite(m3in1,HIGH);
//    digitalWrite(m3in2,LOW);
//    analogWrite(pwm1,abs(m1));
//    analogWrite(pwm2,abs(m2));
//    analogWrite(pwm3,abs(m3));

//  }
//  if (a[16] == 255) // When LEFT ANALOG stick moved to RIGHT move right ie. X-axis +ve
//  {
//    m1 = m2 = m3 = -60;

//    digitalWrite(m1in1,LOW);
//    digitalWrite(m1in2,HIGH);
//    digitalWrite(m2in1,LOW);
//    digitalWrite(m2in2,HIGH);
//    digitalWrite(m3in1,LOW);
//    digitalWrite(m3in2,HIGH);
//    analogWrite(pwm1,abs(m1));
//    analogWrite(pwm2,abs(m2));
//    analogWrite(pwm3,abs(m3));
  
  /********************MOTION CALCULATON*********************************************/
    if((gear%2)==0)
    {
    rot = map((a[16]-123), -123, 132, 60, -60);    
    }
    else
    {
    rot = map((a[16]-123), -123, 132, 45, -45);
    }
    
    int x = a[14] -132;// map to -128 to 128
    int y = a[15] - 123;  
    m1 = rot + (x + 1.732*y)/2; // Calculation for angle to move straight.
    m2 = (rot + (x - 1.732*y)/2);
    m3 = rot + -x;

    if(m1>10)
    {
      digitalWrite(m1in1,LOW);
      digitalWrite(m1in2,HIGH);

    }
    else if(m1<-10)
    {
      digitalWrite(m1in1,HIGH);
      digitalWrite(m1in2,LOW);

    }
    else
    {
      digitalWrite(m1in1,LOW);
      digitalWrite(m1in2,LOW);
    }

    if(m2>10)
    {
      digitalWrite(m2in1,LOW);
      digitalWrite(m2in2,HIGH);

    }
    else if(m2<-10)
    {
      digitalWrite(m2in1,HIGH);
      digitalWrite(m2in2,LOW);

    }
    else
    {
      digitalWrite(m2in1,LOW);
      digitalWrite(m2in2,LOW);
    } 

    if(m3>10)
    {
      digitalWrite(m3in1,LOW);
      digitalWrite(m3in2,HIGH);

    }
    else if(m3<-10)

    {  
      digitalWrite(m3in1,HIGH);
      digitalWrite(m3in2,LOW);

    }
    else
    {
      digitalWrite(m3in1,LOW);
      digitalWrite(m3in2,LOW);
    } 

    m1 = abs(m1);
    m2 = abs(m2);
    m3 = abs(m3);
    
    if((gear%2)==0)
    {
      m1 = map(m1, 0, 127, 0, 255);
      m2 = map(m2, 0, 127, 0, 255);
      m3 = map(m3, 0, 127, 0, 255);
    }
    else
    {
      m1 = map(m1, 0, 127, 0, 127);
      m2 = map(m2, 0, 127, 0, 127);
      m3 = map(m3, 0, 127, 0, 127);
    }

    if((m1>-deadz)&&(m1<deadz)) // application of deadzone
      m1=0;
    if((m2>-deadz)&&(m2<deadz))
      m2=0;
    if((m3>-deadz)&&(m3<deadz))
      m3=0;
    if(m1>207) // If speed more than 255 then make it 255 as 255 max for Herkules
      m1=255;
    if(m3>207)
      m3=255;
    if(m1<5)
      m1=0;
    if(m3<5)
      m3=0;
    if(m2<5)
      m2=0;
    if(m2>207)
      m2=255; 


    /********************MOVEMENT EXECUTION****************************************/
  
  
  Serial.print(m1);
  Serial.print(" ");
  Serial.print(m2);
  Serial.print(" ");
  Serial.println(m3);

  analogWrite(pwm1,abs(m1));
  analogWrite(pwm2,abs(m2));
  analogWrite(pwm3,abs(m3));

}










