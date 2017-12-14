#include <msp430.h>
#include <stdlib.h>

int arrayOfInts[] = {1,1,0,2, 2,2,0,1, 0,1,0,2, 2,0,1,1 ,1,0,2,2,2,0, 1,1,0,1,0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 2, 2, 0, 1, 0, 2, 0, 1, 1, 0, 2};
int pLeft = 6;
int pRight = 15;

int ledL4 = 7;
int ledL3 = 8;
int ledL2 = 9;
int ledL1 = 10;

int ledR4 = 14;
int ledR3 = 13;
int ledR2 = 12;
int ledR1 = 11;

int pin = 0;

bool justPressed = false, hasLost = false;
bool L4, L3, L2, L1, R4, R3, R2, R1; 

int buttonPin = PUSH2;
int stateOfButton = 0;

bool playerLeft = true;

int index = 0;

void randGeneration(){

  switch (arrayOfInts[index])
  {  
    case 1:
    L4 = true;
    R4 = false;
    break;
   
    case 2:
    R4 = true;
    L4 = false;
    break;

    case 0:
    default:
    L4 = R4 = false;
    
  }
  index++;
}

void lose(){
  delay(1000000);
  exit(0);
}

void checkLoss(){
  if(playerLeft == true){
    if(L1 == true){
      lose();
    }
    
  } else if(R1 == true){
    lose();
  }
}

void stepDown(){
  L1 = L2;
  L2 = L3;
  L3 = L4;
  
  R1 = R2;
  R2 = R3;
  R3 = R4;
}

void render(){
  if(L4){
    digitalWrite(ledL4, HIGH);
  } else {
    digitalWrite(ledL4, LOW);
  }
  
  if(L3){
    digitalWrite(ledL3, HIGH);
  } else {
    digitalWrite(ledL3, LOW);
  }
  
  if(L2){
    digitalWrite(ledL2, HIGH);
  } else {
    digitalWrite(ledL2, LOW);
  }
  
  if(L1){
    digitalWrite(ledL1, HIGH);
  } else {
    digitalWrite(ledL1, LOW);
  }
  
  if(R4){
    digitalWrite(ledR4, HIGH);
  } else {
    digitalWrite(ledR4, LOW);
  }
  
  if(R3){
    digitalWrite(ledR3, HIGH);
  } else {
    digitalWrite(ledR3, LOW);
  }
  
  if(R2){
    digitalWrite(ledR2, HIGH);
  } else {
    digitalWrite(ledR2, LOW);
  }
  
  if(R1){
    digitalWrite(ledR1, HIGH);
  } else {
    digitalWrite(ledR1, LOW);
  }
  
  if(playerLeft){
    digitalWrite(pRight, LOW);
    digitalWrite(pLeft, HIGH);
  } else {
    digitalWrite(pRight, HIGH);
    digitalWrite(pLeft, LOW);
  }

}

void setup(){
  // put your setup code here, to run once:
  //Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  
  L4 = L3 = L2 = L1 = R4 = R3 = R2 = R1 = false;
   // make the LED pins outputs
  pinMode(P1_0,OUTPUT);
  pinMode(ledL4, OUTPUT);
  pinMode(ledL3, OUTPUT);
  pinMode(ledL2, OUTPUT);
  pinMode(ledL1, OUTPUT);

  
  pinMode(ledR4, OUTPUT);
  pinMode(ledR3, OUTPUT);
  pinMode(ledR2, OUTPUT);
  pinMode(ledR1, OUTPUT);

  pinMode(pRight, OUTPUT);
  pinMode(pLeft, OUTPUT);

  pinMode(3, OUTPUT);
  // Configuration word
  // Bits 15-10: Unused
  // Bits 9-8: Clock source select: set to SMCLK (16MHz)
  // Bits 7-6: Input divider: set to 8
  // Bits 5-4: Mode control: Count up to TACCRO and reset
  // Bit 3: Unused
  // Bits 2: TACLR : set to initially clear timer system
  // Bit 1: Enable interrupts from TA0
  // Bit 0: Interrupt (pending) flag : set to zero (initially)
  TA0CTL=0b0000001011010010; 
  TACCR0=2000; // Set TACCR0 = 2000 to generate a 1ms timebase @ 16MHz with a divisor of 8
  TACCTL0=BIT4; // Enable interrupts when TAR = TACCR0
}
void loop()
{
  // put your main code here, to run repeatedly:

}
// The address function that follows this vector statement is placed in the specified location Interrupt Vector table 
#pragma vector=TIMER0_A0_VECTOR
__interrupt  void timerA0ISR(void)
{
// Timer A0 Interrupt service routine
  static int msCount=0;// Count milliseconds to allow a 1 second pulse
  static int state=0;  // Remember LED state for toggling purposes

  if(!hasLost)
  {
    msCount++;
  }
  
  if (msCount >= 1000)
  {
    msCount = 0;
    stepDown();
    randGeneration();
    stateOfButton = digitalRead(buttonPin);
    if (stateOfButton == LOW && !justPressed) 
    {     
          justPressed = true;
          digitalWrite(3, LOW);
          if(playerLeft == true)
          {
            playerLeft = false;
          }
          else
          {
            playerLeft = true;
          }
    } 
    else
    {
      digitalWrite(3, HIGH);
      justPressed = false;
    }
    render();
    //checkLoss();
    if(playerLeft == true){
    if(L1 == true){
      hasLost = true;
      delay(0);
      exit(0);
    }
    
    } else if(R1 == true){
      hasLost = true;
      delay(10000);
      exit(0);
    }
}
    state=~state;             // toggle state
}
