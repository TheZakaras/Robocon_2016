/* Code for a 2-DOF arm scanning using a laser sensor
 *  Team Robocon BITS Pilani - Goa
 *  Last update - 14 January 2016
 */

#include <SPI.h>  
#include <Servo.h>

int x=0;
int y=0;
int l1=30,l2=26; // length of the arms in cm
float c2;
float s2;
float k1;
float k2;
float thetarad;//major angle

float thetadeg;
float psirad ;
float psideg;

float final_angle;
int final_dist;
int first_angle,second_angle;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servogrip;
int ifm = A0;
int ifm_value = 0;
int min_ifm_value[3];
int min_ifm_value_1[3];
int min_ifm_angle[3];
int history;

void setup() {
  // put your setup code here, to run once:
  pinMode(11,OUTPUT);
  first_angle =90;
second_angle = 90;
  servo1.attach(11);//peeche waali
  servo2.attach(10);//aage waali
  servo3.attach(6);
  servogrip.attach(12);
  delay(1000);
  Serial.begin(9600);
  history = 900;
  min_ifm_value[0] = 5000;
  min_ifm_angle[1] = 0;
  min_ifm_value_1[0] = 5000;
  servo1.write(180);
  delay(1000);
  servo2.write(180);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
   int prod,maxProd,maxJ,oren,def,sign;
    maxProd  = prod = maxJ = oren = sign= 0;
    def = 30;
    servo1.write(0);
    delay(100);
  //  int allmaxprod,maxtheta,maxpsi;
    //allmaxprod=maxtheta=maxpsi=0;
//     uint16_t blocks; //size of array pixy sends

     for( int i=55;i<=125;i=i+1)
     {  servo3.write(i);
         delay(500);
         
            ifm_value=analogRead(ifm);
       
          if(((history - ifm_value) > 200))
          {  min_ifm_value[0] = ifm_value;
             min_ifm_value[1] = i;
             sign = -1;
//              min_ifm_value[2] = j;
          }
      //  Serial.println(i);
       // Serial.println(ifm_value);  
      if(((ifm_value - history) > 100))
       {   min_ifm_value_1[0] = ifm_value;
           min_ifm_value_1[1] = i;
           sign = 1;
       }
       history = ifm_value;
       Serial.println(ifm_value);
       Serial.println(i);
     }
       int final = min_ifm_value[1]/2 +min_ifm_value_1[1]/2;
      oren = min_ifm_value[1]/2 -min_ifm_value_1[1]/2;
        servo3.write(final);
        servogrip.write(final + (oren - def)*sign);
        final_angle = final * 3.14/180;
        final_dist = analogRead(ifm);
        final_dist = final_dist/10;
         x=30 + final_dist*cos(final_angle);
        y=26 - final_dist*sin(final_angle);
        c2=(pow(x,2)+pow(y,2)-pow(l1,2)-pow(l2,2))/(2*l1*l2);   //Inverse Kinematics
        s2=sqrt(1-pow(c2,2));
        k1=l1+(l2*c2);
        k2=l2*s2;
        thetarad=atan2(y,x)-atan2(k2,k1);//major angle

        thetadeg=180 - (thetarad*180)/3.14;
        psirad = atan2(s2,c2);
        psideg=180 - (psirad*180)/3.14;
        
        servo1.write(thetadeg);
        delay(1000);
        servo2.write(psideg);
        delay(1000);

        delay(10000);
        
//     for( int i=80;i>=20;i=i-3)
//     {  servo1.write(i);
//         delay(100);
////        for(int j=20;j<=80;j=j+1)
////         {  servo2.write(j);
////           delay(20);}
////           for(int j=80;j>=20;j=j-1n)
////         {  servo2.write(j);
////           delay(20);
//           ifm_value=analogRead(ifm);
//          if(ifm_value<min_ifm_value[3])
//          {  min_ifm_value[1] = ifm_value;
//             min_ifm_angle[1] = i;
////              min_ifm_value[2] = j;
//          }
// //        }
//   
// }
//     min_ifm_angle[2] = (min_ifm_angle[1] + min_ifm_angle[0])/2;
//     servo1.write(min_ifm_angle[2]);
//     delay(1000);
//     servo2.write(min_ifm_value[2]);
//     delay(1000);
Serial.print("final");
Serial.println(final);
     //   Serial.println(min_ifm_value[0]); 
     delay(3000);
digitalWrite(12,HIGH);
}
         
      

     

    

            


