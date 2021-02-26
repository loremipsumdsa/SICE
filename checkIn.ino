#include <DS3231.h>
DS3231 rtc(SDA, SCL);
int capteur[]={6,20,20,20,20,20};
int aLum=1;
int aTemp=2;
int aHum=0;
int x;
int luminMin;
void setup() {
  Serial.begin(9600);
    rtc.begin(); // Initialize the rtc object
  ////////////////////////////////////////////////////////
////////////////ECLAIRAGE///////////////////////////////
  Serial.println("test luminosite");
  x=0;
  x=analogRead(aLum);
  Serial.println("valeur au capteur: "+x);
  if (x<luminMin)//si il faut nuit
  {
    capteur[5]=0;  
  }
  else//si il fait jour
  {
  capteur[5]=1;  
  }
  Serial.println("valeur traitee: "+capteur[5]);
  x=0;

/////////////////////////////////////////////////////
/////////////////AROSAGE////////////////////////////
  Serial.println("test hydrometrie");
  x=0;
  x=analogRead(aHum);
  Serial.println("valeur au capteur: "+x);
  capteur[2]=map(x,0,950,0,100);
  x=0;

 //////////////////////////////////////////////
 ///////////CHAUFFAGE/////////////////////////
  Serial.println("test temperature");
  x=0;
  x=analogRead(aTemp);
  Serial.println("valeur au capteur: "+x);
  int t=map(x,0,1023,0,5000);//on transforme la valeur lue en valeur de tension entre 0 et 5000 mV (car pas de virgule)
  x=map(t,0,1023,0,5000);//on transforme la valeur lue en valeur de tension entre 0 et 5000 mV (car pas de virgule)
  capteur[1]=map(x,0,1750,-50,125); //on transformela tension (de 0 à 1750mV en température (de -50°C à 125°C);
  Serial.println("valeur traitee: "+capteur[1]);
  x=0;

  ///////////////////////////////////////////////////////////
 ///////////////Heure et minute////////////////////////////
 String heureStr;
  Serial.println(F("Repcuperation de l'heure"));
  heureStr=rtc.getTimeStr()[0];
  heureStr+=rtc.getTimeStr()[1];
  capteur[3]=heureStr.toInt();

  heureStr=rtc.getTimeStr()[0];
  heureStr+=rtc.getTimeStr()[1];
  capteur[4]=heureStr.toInt();
}

void loop() {
  // put your main code here, to run repeatedly:

}
