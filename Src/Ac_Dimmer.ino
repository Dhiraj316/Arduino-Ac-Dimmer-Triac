#define triac_gate   8
#define pot         A0
 
volatile bool ZC = 0;
uint16_t alpha;
 
void setup(void) {
  pinMode(triac_gate, OUTPUT);
  digitalWrite(triac_gate, LOW);
  attachInterrupt(0, ZC_detect, CHANGE);       
}
 
void ZC_detect() {
  ZC = 1;
}
 
void loop() {
 
  if( ZC){
    if(alpha < 9500) {
      delayMicroseconds(alpha);
      digitalWrite(triac_gate, HIGH);
      delayMicroseconds(200);
      digitalWrite(triac_gate, LOW);
    }
    ZC = 0;
 
    alpha = ( 1023 - analogRead(pot) ) * 10;
    if(alpha > 9500)
      alpha = 9500;
  }
}
