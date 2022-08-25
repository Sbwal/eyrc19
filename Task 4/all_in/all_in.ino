#define InL1            13                      // motor pin
#define PWML            10                      // PWM motor pin  
#define InL2            9                       // motor pin  

#define InR1            7                       // motor pin
#define PWMR            6                       // PWM motor pin
#define InR2            4                       // motor pin 
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include <math.h>
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;
double ax_f=0,ay_f=0,az_f=0;
double gx_f=0,gy_f=0,gz_f=0;
int gx_l=0,gy_l=0,gz_l=0;
int angle=0;
int angd=0;
double alpha=0.76;
// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO


#define LED_PIN 13
bool blinkState = false;

void motor_init(){
    pinMode(InL1, OUTPUT);
    pinMode(InL2, OUTPUT);
    pinMode(PWML, OUTPUT);
    
    pinMode(InR1, OUTPUT);
    pinMode(InR2, OUTPUT);
    pinMode(PWMR, OUTPUT);
}
void motorForwardL(int PWM_val)  {
    analogWrite(PWML, PWM_val);
    digitalWrite(InL1, LOW);
    digitalWrite(InL2, HIGH);
}

void motorForwardR(int PWM_val)  {
    analogWrite(PWMR, PWM_val);
    digitalWrite(InR1, LOW);
    digitalWrite(InR2, HIGH);
}
void motorBackwardL(int PWM_val)  {
    analogWrite(PWML, PWM_val);
    digitalWrite(InL1, HIGH);
    digitalWrite(InL2, LOW);
}

void motorBackwardR(int PWM_val)  {
    analogWrite(PWMR, PWM_val);
    digitalWrite(InR1, HIGH);
    digitalWrite(InR2, LOW);
}

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(9600);
      motor_init();
    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");


    // configure Arduino LED pin for output
    pinMode(LED_PIN, OUTPUT);
}



void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
        ax_f=(1-alpha)*ax+alpha*ax_f;
        ay_f=(1-alpha)*ay+alpha*ay_f;
        az_f=(1-alpha)*az+alpha*az_f; 
        gx_f= (1-alpha)*gx_f + (1-alpha)*(gx-gx_l);
        gy_f= (1-alpha)*gy_f + (1-alpha)*(gy-gy_l);
        gz_f= (1-alpha)*gz_f + (1-alpha)*(gz-gz_l);
        gx_l = gx;
        gy_l = gy; 
        gz_l = gz; 
        double dt=1/100;
        double acc=atan(ax_f/abs(az_f))*180/3.14;
        angd=angle;
        angle=(1-alpha)*(angle+-1*gy_f*dt)+(alpha)*(acc);
        Serial.print(angle);
        
    #endif

    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
        if(gx>32767)
        gx=gx-65536;
        if(gy>32767)
        gy=gy-65536;
        if(gz>32767)
        gz=gz-65536;
        if(ax>32767)
        ax=ax-65536;
        if(ay>32767)
        ay=ay-65536;
        if(az>32767)
        az=az-65536;
        ax=ax/16384;
        ay=ay/16384;
        az=az/16384;
        gx=gx/131;
        gy=gy/131;
        gz=gz/131;
    #endif

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);

    double ang=(angle-angd)/10;
  int pwm=-1*(-0.100000*0-0.528836*0+24.718145*angle+3.403287*ang);
  Serial.print(" ");
  Serial.println(pwm);
  
 motorForwardL(100);
 delay(1000);
}
