//this sketch can count 4 pulses of a single pulse train from an encoder,increasing the precision 2 fold.

#define a_m1 2
#define b_m1 3

#define a_m2 18
#define b_m2 19
  
int C1 = 0; 
int C2 = 0;
void setup()
{
  pinMode (a_m1,INPUT_PULLUP);
  pinMode (b_m1,INPUT_PULLUP); 
  pinMode (a_m2,INPUT_PULLUP);
  pinMode (b_m2,INPUT_PULLUP);   
  Serial.begin (9600);
  attachInterrupt(digitalPinToInterrupt(a_m1), a_m1Change, CHANGE);
 attachInterrupt(digitalPinToInterrupt(b_m1), b_m1Change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(a_m2), a_m2Change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(b_m2), b_m2Change, CHANGE);
} 
void loop()
{
  Serial.print(C1/540*360*3.5/3.14);
  Serial.print(" ");
  Serial.print(C2/540*360*3.5/3.14);
  Serial.println();
}

void a_m1Change()
{
  if(digitalRead(a_m1))
  { 
    if(digitalRead(b_m1))
    { C1++; }
    else
    { C1--; }
  }
  else
  {
    if(digitalRead(b_m1))
    { C1--; }
    else
    { C1++; }
  }
}

void b_m1Change()
{
  if(digitalRead(b_m1))
  { 
    if(digitalRead(a_m1))
    { C1--; }
    else
    { C1++; }
  }
  else
  {
    if(digitalRead(a_m1))
    { C1++; }
    else
    { C1--; }
  }
}

void a_m2Change()
{
  if(digitalRead(a_m2))
  { 
    if(digitalRead(b_m2))
    { C2++; }
    else
    { C2--; }
  }
  else
  {
    if(digitalRead(b_m2))
    { C2--; }
    else
    { C2++; }
  }
}

void b_m2Change()
{
  if(digitalRead(b_m2))
  { 
    if(digitalRead(a_m2))
    { C2--; }
    else
    { C2++; }
  }
  else
  {
    if(digitalRead(a_m2))
    { C2++; }
    else
    { C2--; }
  }
}
