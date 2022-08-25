
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

 
  int d;
  
  
  // Check frame length
  if (Serial.available() > 18)
  {
    // Check Start Byte of the API Frame
    if (Serial.read() == 0x7E)
    {
      // Read and discard 2 bytes
      for (int i=0; i<2; i++)
      d = Serial.read();
      
      // Check Frame Type
      if(Serial.read() != 0x83) return;
      
      // Read and discard 8 bytes
      for (int i=0; i<9; i++)
      d= Serial.read();
      
      
      int xp1=Serial.read();
      int xp2=Serial.read();
      int yp1=Serial.read();
      int yp2=Serial.read();    
      if(xp1==0)
      Serial.print("left-");
      else if(xp1==2||xp1==1)
      Serial.print("right-");
      else
      Serial.print("center-");
      if(yp1==0)
      Serial.print("forward-");
      else if(yp1==2||yp1==1)
      Serial.print("backward-");
      else
      Serial.print("center-");
      int ts1=Serial.read();
      if(ts1==0)
      Serial.print("off");
      else if(ts1==3)
      Serial.print("on");
     Serial.println(); 
    }
    
  }
  }
