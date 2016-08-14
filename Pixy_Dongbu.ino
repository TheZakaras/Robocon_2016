/* Code for moving a Dongbu servo motor using values obtained from Pixy
 *  Team Robocon BITS Pilani - Goa
 *  Last update: 25 Januray 2016
 */

#include <SPI.h>  
#include <Pixy.h>
#include <Servo.h>
#include <HerkuleX.h>

#define RX   8        // Connected with HerkuleX TX Pin
#define TX   9        // Connected with HerkuleX RX Pin
#define MOTORID  219  // HerkuleX Servo Motor ID


Pixy pixy;
Servo servo ;
int x_mem[5];

int prop;
int last_prop;
int derivative;
int final;
int integral;
//unsigned long tim;
int err_angle= 0;
void setup()
{ Serial.begin(9600);
  
  HerkuleX.begin(115200, RX, TX);   
  delay(10);
  // Torque ON
  HerkuleX.torqueOn(MOTORID);
  pixy.init();
  for ( int k=0;k<5;k++)
    x_mem[k] = 0;
  prop=0;
  last_prop=0;
}
void loop() 
{ 
  
   uint32_t i=0;
   
   int current_ang = 0;
   int servo_input = 0;
   int ideal_x = 160;
   int err_x = 0;
   int ideal_ang = -15;
   int prod,maxProd,maxJ;
   maxProd  = prod = maxJ = 0;
   
   
     uint16_t blocks; //size of array pixy sends
      for (int k = 0;k<5;k++)
      {
         blocks = pixy.getBlocks();

          if (blocks) // if there were any recognized objects
          {     
            
            //finds the largest object that fits the signature

            for ( int j = 0; j < blocks; j++)
            {

                 prod = pixy.blocks[j].width * pixy.blocks[j].height;

                if (prod > maxProd) //save the new largest obj
                {   maxProd = prod;
                    maxJ = j;
                 }

             }

           }
           else 
            err_angle = 1;
           
           x_mem[k] = -ideal_x + pixy.blocks[maxJ].x;
           delay(2);
           err_x = err_x + x_mem[k]/5;
           
      }

      //PID implemented on the image
      
prop = err_x;
prop = prop/2;
// integral = integral + prop;
derivative = prop - last_prop;
final =  prop/3 + derivative*3 ;
final = ideal_ang + err_x;
if(prop >60)
  prop =160;
  if(prop<-60)
    prop = -160;
  if((prop <20)&&(prop>-20))
    prop =0;
  if((prop>40)&&(prop<60))
    prop = 50;
//  if((final>40)&&(final<50))
    // final = 40;
  if((prop>20)&&(prop<40))
     prop = 30;
//       if((prop>20)&&(prop<30))
//      prop = 20;
  if((prop<-40)&&(prop>-60))
     prop = -50;
//    if((prop<-40)&&(prop>-50))
//   prop = -40;
  if((prop<-20)&&(prop>-40))
      prop = -30;
  if((prop<-20)&&(prop>-30))
      prop = -20;
          
          
    if(err_angle == 0)
       HerkuleX.moveAngle(MOTORID, prop, 1, HERKULEX_LED_BLUE);
    else 
       HerkuleX.moveAngle(MOTORID, prop, 1, HERKULEX_LED_BLUE);
       Serial.print(prop);
       Serial.print("        ");
       Serial.print(err_x);
   //    servo.write(100);
       

       last_prop = err_x;
     //  delay(100);
   Serial.println();

}
        

       
