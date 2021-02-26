int sortie[]={4,5,3,2};
void setup() {
  Serial.begin(9600);
  pinMode(sortie[1],OUTPUT);
  pinMode(sortie[2],OUTPUT);
  pinMode(sortie[3],OUTPUT);
////////////////////////////////////////////////////////
////////////////ECLAIRAGE///////////////////////////////
  Serial.println("test eclairage");
  digitalWrite(sortie[3],HIGH);
  Serial.println("on");
  delay(500);
  digitalWrite(sortie[3],LOW);
  Serial.println("off");
  delay(500);
    digitalWrite(sortie[3],HIGH);
  Serial.println("on");
  delay(500);
  digitalWrite(sortie[3],LOW);
  Serial.println("off");
  delay(500);
    digitalWrite(sortie[3],HIGH);
  Serial.println("on");
  delay(500);
  digitalWrite(sortie[3],LOW);
  Serial.println("off");
  delay(500);

/////////////////////////////////////////////////////
/////////////////AROSAGE////////////////////////////
  Serial.println("test arosage");
  digitalWrite(sortie[2],HIGH);
  Serial.println("on");
  delay(10000);
  digitalWrite(sortie[2],LOW);
  Serial.println("off");
  delay(1000);

 //////////////////////////////////////////////
 ///////////CHAUFFAGE/////////////////////////
  Serial.println("test chauffage");
  digitalWrite(sortie[1],HIGH);
  Serial.println("on");
  delay(50000);
  digitalWrite(sortie[1],LOW);
  Serial.println("off");
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:

}
