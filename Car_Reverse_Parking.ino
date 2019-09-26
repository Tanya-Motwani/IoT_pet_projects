int trigpin = D0;
int echopin = D2;
int ledpin = D4;
long duration,cm,inches;

int Delay=100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode (trigpin, OUTPUT);
  pinMode (echopin, INPUT);
  pinMode (ledpin, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(trigpin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);

  pinMode(echopin, INPUT);
  duration = pulseIn(echopin, HIGH);

  cm = (duration/2)/29.1;
  inches = (duration/2)/74;

  Serial.print(duration);
  Serial.print(" ,");
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  digitalWrite(ledpin,HIGH);
  
  if(cm<5)
    Delay = 100;
  else if(cm<10)
    Delay = 200;
  else if(cm<15)
    Delay = 300;
  else if(cm<20)
    Delay = 400;
  else if(cm<25)
    Delay = 500;
  else if(cm<30)
    Delay = 600;
  else if(cm<35)
    Delay = 700;
  else
    Delay = 800;

  delay(Delay);

  digitalWrite(ledpin,LOW);
   
  delay(Delay);

  

  
}