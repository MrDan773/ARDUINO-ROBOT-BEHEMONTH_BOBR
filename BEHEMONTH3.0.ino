//BEHEMONTH = Blitzkrieg Engine Heavily Enhanced Massively Nuclear Tank of Havoc
//Bentley Original with Bluetooth Robot
#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor Mt(4);

const int trigPin = A2; // Trigger pin
const int echoPin = A3; // Echo pin
NewPing sonar(trigPin, echoPin);
SoftwareSerial bluetooth(0,1); //RX, TX pins

unsigned int speedTun = 250; const unsigned int defaultSpeedTun = 250;
const int defaultSpeed = 140; unsigned int motorSpeed = 140; //initital speed

unsigned int distance;
char command;

bool OK = 0;
bool OK2 = 0;

void setup() {
   // put your setup code here, to run once:
   Serial.begin(9600);
   bluetooth.begin(9600);
   Serial.println("BEHEMONTH got lifeforce.");
   M1.setSpeed(220);
   M2.setSpeed(220);
   Mt.setSpeed(speedTun);
   delay(10);
   M1.run(RELEASE);
   M2.run(RELEASE);
   Mt.run(RELEASE);
}

void loop() {
  if(OK == 0){
  Serial.println("Behemonth has awaken.");
  OK+=1;
  }
  if (bluetooth.available() && OK == 1) {
  //if (OK == 1) {
    //command = Serial.read();
    command = bluetooth.read();
    if(OK2 == 0){
      bluetooth.println("Behemonth will take commands from bluetooth.");
      Serial.println("Behemoth is listening.");
      OK2 = 1;
      }
    //w - forward;// s - back;// d - right;// a - left;
    //t - shoot; //y - stop shoot;// x - stop wheels;
    //f - gear wheels;//v - gear turret;
    //o - tester intoarcere (ex: os750 intoarce stanga 750ms);
    //p - functia ocolire si tragere aka combat mode
    switch (command) {
      case 'e':
        carefulwalk(1000,10);
        break;
      case 'w':
        forward();
        break;
       case 's':
        backward();
        break;
       case 'a':
        left();
        break;
       case 'd':
        right();
        break;
       ///
       case 't':
        shoot();
        break;
       case 'y':
        xshoot();
        break;
       ///
       case 'f':
        gearWheel();
        break;
        ///
       case 'o':
        tester();
        break;
       case 'p':
        do{
          Serial.print("BEHEMONTH HAS ENTERED COMBAT.");
          delay(200);
          MainFunction();
          Serial.println("##############################");
          delay(200);
          command = bluetooth.read();
          delay(200);
          if(command=='x')
            break;
          Serial.print("check: ");
          Serial.println(command);
        }while(command!='x');
        break;
        ///////
       case 'x':
        stopw();
        break;
      case 'q':
       for(unsigned i=1;i<=3;i++)
       {
        powpow(70,300,1000);
       }
        break;
       default:
        noAction();      
        break;
    }
  }
}

void forward() {
   delay(10);
   M1.run(FORWARD);
   M2.run(FORWARD); 
}

void backward() {
   delay(10);
   M1.run(BACKWARD);
   M2.run(BACKWARD);
}

void left() {
   M1.run(BACKWARD);
   M2.run(FORWARD);
}

void right() {
   M2.run(BACKWARD);
   M1.run(FORWARD);
}

void noAction(){
}

void shoot(){
  delay(10);
  Serial.println("Motor Turret starts.");
  Mt.run(FORWARD);
}

void xshoot(){
  delay(10);
  Mt.run(RELEASE);
  Serial.println("Motor Turret has stopped.");
}

void stopw(){
   M1.run(RELEASE);
   M2.run(RELEASE);
   delay(10);
}

void resetWheelSpeedToDefault(){
  setWheelSpeed(defaultSpeed);
  motorSpeed = defaultSpeed;
}

void setTurretSpeed(int motorS){
   Mt.setSpeed(motorS);
   Serial.print("Turret's speed set to ");Serial.print(motorS);Serial.println(".");
}

void setWheelSpeed(int motorS) {
   M1.setSpeed(motorS);
   M2.setSpeed(motorS);
}

byte gearNr = 1;
unsigned int lowspeed = 140;
unsigned int mediumspeed = 180;
unsigned int maximspeed = 220;
void gearWheel(){
  if(gearNr==1)
    {Serial.print("Gear 1: ");Serial.println(lowspeed);setWheelSpeed(lowspeed);gearNr+=1;motorSpeed=lowspeed;}
    else if(gearNr==2)
      {Serial.print("Gear 2: ");Serial.println(mediumspeed);setWheelSpeed(mediumspeed);gearNr+=1;motorSpeed=mediumspeed;}
      else
        {Serial.print("Gear 3: ");Serial.println(maximspeed);setWheelSpeed(maximspeed);gearNr=1;motorSpeed=maximspeed;}
  delay(10);
}

void tester(){
    delay(10);
    char directie = Serial.read();
    delay(100);
    unsigned int timp = Serial.parseInt();
    Serial.println("OBIECTIV ATINS.");
    switch (directie) {
      case 'd':
        Serial.println("Timp dreapta:");
        rightTurn(timp);
        break;
       case 's':
        leftTurn(timp);
        Serial.println("Timp stanga:");
        break;
    }
}

void leftTurn(int timp){
   M1.run(BACKWARD);
   M2.run(FORWARD);
   Serial.print("<<<"); 
   delay(timp);
   M2.run(RELEASE);
   M1.run(RELEASE);
   Serial.println(timp);
}

void rightTurn(int timp){
   M2.run(BACKWARD);
   M1.run(FORWARD);
   Serial.print(">>>"); 
   delay(timp);
   M2.run(RELEASE);
   M1.run(RELEASE);
   Serial.println(timp);
}

void slowmovingback(unsigned int speed){
   delay(10);
   setWheelSpeed(speed);
   backward();
}

void stopslowmovingback(){
    delay(10);
    stopw();
    setWheelSpeed(motorSpeed);
}

void powpow(unsigned int timp,unsigned recoil,unsigned int dilei){
  delay(dilei);
  Mt.run(FORWARD);
  
  slowmovingback(135);
  delay(recoil);
  stopslowmovingback();

  Serial.print("****");
  delay(timp);
  Mt.run(RELEASE);
}

void walk(unsigned timp){
   M1.run(FORWARD);
   M2.run(FORWARD);
   Serial.print("/\/\/\/"); 
   delay(timp);
   M2.run(RELEASE);
   M1.run(RELEASE);
   Serial.println(timp);
}

bool obiectGasit;
void cautareStanga(unsigned int cautari){
  byte i=1;
  obiectGasit=0;
  while(i<=cautari&&obiectGasit==0)
  {
    distance = sonar.ping_cm();
    if(distance>0 && distance<=80)
      {
        Serial.println("Target identified.");
        obiectGasit=1;
        break;
      
      }
    else
      {
        i+=1;
        leftTurn(550);
      }
  }
  //daca nu ajunge sa gaseasca ceva prin rotire merge drept inainte pentru o perioada
  if(obiectGasit==0)
    carefulwalk(1000,10);
}

void cautareDreapta(unsigned int cautari){
  byte i=1;
  obiectGasit=0;
  while(i<=cautari&&obiectGasit==0)
  {
    distance = sonar.ping_cm();
    if(distance>0 && distance<=80)
      {
        Serial.println("Target identified.");
        obiectGasit=1;
        break;
      
      }
    else
      {
        i+=1;
        rightTurn(550);
      }
  }
  //daca nu ajunge sa gaseasca ceva prin rotire merge drept inainte pentru o perioada
  if(obiectGasit==0)
    carefulwalk(1000,10);
}

bool Ready_for_engagement;
void apropiereObiect(unsigned int distanta){
  Ready_for_engagement=0;
  do{
    distance = sonar.ping_cm();
    if(distance>distanta)
      forward();
    else{
      stopw();
      Ready_for_engagement=1;
      Serial.println("Ready for engagement.");
      break;
    }
  }while(Ready_for_engagement==0);
}

bool tinta;
bool pereteFull;
bool pereteSpreDreapta;
bool pereteSpreStanga;
int distances[4];
void identifyTarget(){
  tinta=0;
  pereteFull=0;
  pereteSpreDreapta=0;
  pereteSpreStanga=0;
  distances[0] = 0;distances[1] = 0;distances[2] = 0;distances[3] = 0;
  //0-stanga//1-dreapta//2-prima fata//3-a doua fata
  distances[2]=sonar.ping_cm();
  Serial.print( "prima distanta:");
  Serial.println( distances[2]);

  rightTurn(350);
  delay(200);
  distances[1]=sonar.ping_cm();
  Serial.print( "DISTANTA DREAPTA:");
  Serial.println( distances[1]);

  delay(200);
  leftTurn(400);

  delay(200);
  leftTurn(420);
  distances[0]=sonar.ping_cm();
  Serial.print( "DISTANTA STANGA:");
  Serial.println( distances[0]);

  delay(200);
  rightTurn(440);
  distances[3]=sonar.ping_cm();
  Serial.print( "a doua distanta:");
  Serial.println( distances[3]);
  
  
  delay(500);
  if(distances[2]>20){
  if(distances[2]>distances[3])
    {
      do{
        distances[3]=sonar.ping_cm();
        backward();
        delay(50);
      }while(distances[2]>=distances[3]);
      stopw();
    }
  }
  else{
    if(30>distances[3])
    {
      do{
        distances[3]=sonar.ping_cm();
        backward();
        delay(50);
      }while(30>=distances[3]);
      stopw();
    }
  }

  delay(500);//se gandeste
  if((distances[0]>37||distances[0]<=0)&&(distances[1]>37||distances[1]<=0))
      {
        Serial.println("Target aquired.");
        tinta=1;
      }  
    else
    {
      Serial.println("Non-target encountered.");
       if((distances[0]<35||distances[0]<=0)&&(distances[1]<35||distances[1]<=0))
        {
          Serial.println("Perete FULL");
          pereteFull=1;
        }
        else if((distances[0]<35||distances[0]<=0)&&(distances[1]>35||distances[1]<=0))
          {
            Serial.println("Perete spre stanga.");
          pereteSpreStanga=1;
          }
          else
          {
            Serial.println("Perete spre dreapta.");
          pereteSpreDreapta=1;
          }
    }
}

bool obstacle;
unsigned int cevainfata;
void carefulwalk(unsigned timp,byte n){
  unsigned k = timp/n;
  obstacle=0;
  do{ 
    cevainfata=sonar.ping_cm();
    forward();
    delay(timp/n);
    k=k+(timp)/n; 
  }while((cevainfata<=0||cevainfata>35)&&k<=timp);
  if(cevainfata<35&&cevainfata>0)
    obstacle=1;
  Serial.println("BEHEMONTH A MERS CA UN CADILAC.");
  stopw();
}

bool cazExtra;
void ocolireDreapta(){
  cazExtra=0;
  apropiereObiect(32);
  delay(200);
  rightTurn(750);
  delay(200);
  cevainfata=sonar.ping_cm();
  if(cevainfata>0 && cevainfata<30){
     cazExtra=1;
     Serial.println("BEHEMONTH HAS ENTERED AN UNIC STUATION.");
  }
  else{
    delay(200);
    carefulwalk(800,8);
    if(obstacle==1){
      return;
    }
    leftTurn(750);
  }
  if(cazExtra==0){
  carefulwalk(800,4);
    if(obstacle==1){
          return;
        }
  delay(200);
  carefulwalk(500,5);
      if(obstacle==1){
          return;
        }
  }
  Serial.println("...");
}

void ocolirePerete(){
  delay(100);
  apropiereObiect(30);
  delay(500);
  leftTurn(770);
  cevainfata=sonar.ping_cm();
  if(cevainfata>0&&cevainfata<40)
  {
  delay(500);
  Serial.println("BEHEMONTH HAS FOUND HIMSELF INTO A CORNER.");
  leftTurn(770);
  }
  else{
    delay(500);
    carefulwalk(800,4);
    Serial.println("BEHEMONTH SIDE BY SIDE WITH THE WALL.");
  }
}

//////////
//////////////////////////////
void ocolireStanga(){
  cazExtra=0;
  apropiereObiect(30);
  delay(200);
  leftTurn(750);
  delay(200);
  cevainfata=sonar.ping_cm();
  if(cevainfata>0 && cevainfata<30){
     cazExtra=1;
     Serial.println("BEHEMONTH HAS ENTERED AN UNIC STUATION.");
  }
  else{
    delay(200);
    carefulwalk(800,4);
    if(obstacle==1){
      return;
    }
    rightTurn(750);
  }
  if(cazExtra==0){
  carefulwalk(800,4);
  delay(200);
  carefulwalk(500,5);
      if(obstacle==1){
          return;
        }
  }
  Serial.println("...");
}
///////////////////////////////////////////////
//////////

bool primaincercare=0;
void MainFunction(){
  stopw();
  Serial.println("BEWARE SOLDIERS, WAR IS NO JOKE.");
  if(primaincercare==0)
    {cautareStanga(8);primaincercare+=1;}
  else if(primaincercare==1);{
    if(distances[0]>distances[1])
      {
        cautareStanga(4);
      }
      else{
        cautareDreapta(4);
      }
  }  
 if(obiectGasit==1)
  {
    apropiereObiect(32);
    if(Ready_for_engagement==1)
    {
      identifyTarget();
      if(tinta==1){
        Serial.print("FIRE IN THE HOLE");
        for(unsigned i=1;i<=3;i++)
            powpow(70,300,950);
        Serial.println("WE'VE HIT THE TARGET.");
        ocolireDreapta();
      }
      else if(pereteFull==1)
      {
        ocolirePerete();
        Serial.println("PERETE EVITAT");

      }
      else if(pereteSpreStanga==1)
        {
        ocolireDreapta();

        }
      else if(pereteSpreDreapta==1)
      {
        ocolireStanga();
      }
    }
  }
}
