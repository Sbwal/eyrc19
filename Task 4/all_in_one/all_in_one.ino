#define InL1            13                      // motor pin
#define PWML            10                      // PWM motor pin  
#define InL2            9                       // motor pin  

#define InR1            7                       // motor pin
#define PWMR            6                       // PWM motor pin
#define InR2            4                       // motor pin 
#define MagF            51                      // electromagnet pin
 
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

void MAG_init(){
    pinMode(MagF, OUTPUT);
    
    digitalWrite(MagF, LOW);
}

void MagPick(void)  {
  digitalWrite(MagF, HIGH);
}

void MagDrop(void)  {
  digitalWrite(MagF, LOW);
}


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  motor_init();
   MAG_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  

  int d;
  
  
  // Check frame length
  if (Serial.available() > 18)
  {
    
    if (Serial.read() == 0x7E)
    {

      for (int i=0; i<2; i++)
      d = Serial.read();
      
     
      if(Serial.read() != 0x83) return;
      
      
      for (int i=0; i<9; i++)
      d= Serial.read();
      int c=0;
      
      int xp1=Serial.read();
      int xp2=Serial.read();
      int yp1=Serial.read();
      int yp2=Serial.read();    
      if(xp1==0)
      {
        motorForwardL(250);
        motorBackwardR(250);
      }
      else if(xp1==2||xp1==1)
      {
        motorBackwardL(250);
        motorForwardR(250);
      }
      else
      c++;
      if(yp1==0)
     {
        motorForwardR(250);
        motorForwardL(250);
      }
      else if(yp1==2||yp1==1)
      {
        motorBackwardR(250);
        motorBackwardL(250);
      }
      else
      c++;
     if(c==2)
     {
        motorForwardR(0);
        motorForwardL(0);
      }
     int ts1=Serial.read();
     if(ts1==0)
       MagDrop();    
      else if(ts1==3)
      MagPick();
    }
    
  }
  }

 
