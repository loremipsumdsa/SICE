#include <DS3231.h>

//Infos de branchements rtc:
//VCC==>5V
//GND==>GND
//SCL==>A5
//SDA==>A4


#include <SD.h>
#include <SPI.h> //bibliothèqe pour SPI
#include <Ethernet.h> //bibliothèque pour Ethernet
EthernetServer serveur(80); // déclare l'objet serveur au port d'écoute 80
File monFichier;
EthernetClient client;
DS3231 rtc(SDA, SCL);
byte ip[] = {192, 168, 0, 26}; //adresse IP
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; //adresse mac de la carte
int param[] = {7,20,30,8,10,20,10};
int capteur[]={6,20,20,20,20,20};
int sortie[]={4,5,3,2};
int aLum=1;
int aTemp=2;
int aHum=0;
int luminMin=200;
String varDonn; 
char ca;
int x;
bool online=true;


void initRtc()
{
  rtc.begin(); // Initialize the rtc object
}

void getForm()
{
  Serial.println(F("execution de la fonction getForm"));
  x=1;
  ca=0;
  while (client.available())
  {
    ca=client.read();
    if (ca=='&'||x>1)
    {
      Serial.println(F("valeurs trouvees"));
      while (ca!='=')
      {
        Serial.println(F("lecture..."));
        ca=client.read();
      }
      ca=client.read();
      while (ca!='&' && client.available())
      {
        Serial.println(F("lecture d'une variable..."));
        varDonn+=ca;
        ca=client.read();
      }
      Serial.print(F("ajout de la valeur"));
      Serial.print(x);
      Serial.println(F(" au tableau"));
      param[x]=varDonn.toInt();
      x++;
      varDonn="";
      param[0]=x;
    }
  }
  Serial.println(F("Fin de construction du tableau"));
  x=1;
  ca=0;
  for (int i=1;i<param[0];i++)
  {
    Serial.println(param[i]);
  }
  x=1;
  ca=0;
  Serial.println(F("scan des fichiers de sauvegarde preexistant"));
  if (SD.exists("config.txt"))//destruction des fichiers preexistants
  {
    Serial.println(F("fichier preexistant detecte: destruction"));
    if (!(SD.remove("config.txt")))//verification de la destruction
    {
      Serial.println(F("Erreur lors de la destruction"));
      return;
    }
    Serial.println(F("fichier detruit"));  
  }
    //////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
  Serial.println(F("creation et ouverture d'un nouveau fichier"));
  if (!(monFichier = SD.open("config.txt",FILE_WRITE)))//verification de la creation d'un nouveau fichier
  {
    Serial.println(F("Erreur lors de la creation"));
    return; //stoppe le programme
  }
  Serial.println(F("Fichier cree et ouvert"));
  monFichier.write('&');
  for (int c=0;c<param[0];c++)
  {
    monFichier.print(param[c]);
    monFichier.write('&');    
  }
    monFichier.write(';');
    monFichier.close();
    x=1;
    ca=0;
}



void gestServeur()
{
/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////Connection du client/////////////////////////////////////////////
  client = serveur.available(); //on écoute le port
  if (client) 
  { //si client connecté
    Serial.println(F("Client en ligne")); //on le dit...
    
    if (client.connected()) 
     {
      
///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////Recuperation des données//////////////////////////////////////

        //getInfo(client);
        Serial.println(F("Variable transmise par le client: "));
        getForm();
////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////Transmission des données/////////////////////////////////////
        Serial.println(F("\nTransmission des informations vers le clients..."));
       // client.println("HTTP/1.1 200 OK"); // type du HTML
      // client.println("Content-Type: text/html; charset=ascii");
       if(!( monFichier = SD.open("index.htm", FILE_READ)))
       {
       Serial.println(F("erreur ouverture fichier"));
       return; 
       }
       ca=0;
        while (monFichier.available()) 
        {
          ca= monFichier.read(); //on lit un caractère
          if (ca=='?')
          {
            ca=monFichier.read();
            if (ca=='c')
            {
            ca = monFichier.read();
            client.print(capteur[int(ca)-48]);
            Serial.print(F("var trans: t"));
            Serial.println(ca);
            ca = monFichier.read();
            }
            else if (ca=='p')
            {
            ca = monFichier.read();
            client.print(param[int(ca)-48]);
            Serial.print(F("var trans: t"));
            Serial.println(ca);
            ca= monFichier.read(); 
            }
            else
            {
              client.write('?');
            }
          }
        client.write(ca);
        }
      }
      monFichier.close();
      Serial.println(F("fin de transmision"));
/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////déconnection du client/////////////////////////////////////      
    client.stop();
    Serial.println(F("\nclient deconnecte\n"));
    delay(20);
    
    }
  else
  {
    Serial.println("Aucun client connecté");
  }
}


void initServeur()
{
   ///////////////////////////////////////////////////////////////////////////
  //////////////////////INITIALISATION DU SERVER////////////////////////////
  Serial.println(F("Initialisation du serveur..."));
 if (Ethernet.begin (mac)==0)//initialisation de la communication Ethernet
 {
    online=false;
    Serial.println("connection impossible. mise en mode offline");
 }
  Serial.print(F("-Le serveur est sur l'adresse : "));
  Serial.println(Ethernet.localIP()); //on affiche l'adresse IP de la connexion
  serveur.begin();//demarage du serveur
  Serial.println(F("-Le serveur a demarre\n"));
  Serial.println(F("Serveur fonctionnel, en attente de clients...\n\n"));
}

void initSd()
{
  ////////////////////////////////////////////////////////////////
  ////////////////INITIALISATION DE LA CARTE SD//////////////////
  Serial.println(F("Initialisation de la carte SD..."));
  if (!SD.begin(4))//teste la communication avec la carte(pin 
  {
    Serial.println(F("-Communication avec la carte sd impossible"));
    return; //stoppe le programme
  }
  Serial.println(F("-Communication avec la carte SD operationnelle"));
          
  Serial.println(F("Lecture du fichier..."));
  if (!(monFichier = SD.open("index.htm", FILE_READ))) 
  { //verification de l'ouverture en lecture
    Serial.print(F("-Erreur de fichier"));
    return; //stoppe le programme
  }
  monFichier.close();
  Serial.println(F("-Fichier ouvert\n"));
  Serial.println(F("Lecture du fichier"));
  if (!(monFichier = SD.open("config.txt", FILE_READ))) //verification de l'ouverture en lecture
  {                                                                    
    Serial.println(F("Erreur de fichier"));
    return; //stoppe le programme
  }
  Serial.println(F("Fichier ouvert"));
  Serial.println(F("information recuperees:\n"));
  ca = 0; //caractere lu
  x = 0; //position dans le tableau  
  ca=monFichier.read();
  while (ca!=';')
  {
    ca=monFichier.read();
    if (ca=='&')
    {
      param[x]=varDonn.toInt();
      x++;
      varDonn="";
    }
    else
    {
      varDonn+=ca;  
    }
  }
  monFichier.close();
  Serial.println(F("-fin de la recuperation"));
  Serial.println(F("-parametre recuperes:"));
  for (int i=0;i<param[0];i++)
  {
    Serial.println(param[i]);
  }
  ca = 0;
  x = 0; 
}


void gestIn()
{
  Serial.println(F("Verification des entres")); 
  //////////////////////////////////////////////////////
  ///////////////Temperature///////////////////////////
  Serial.println(F("recuperation de la temperature"));
  x=0;
  x=analogRead(aTemp);
  int t=map(x,0,1023,0,5000);//on transforme la valeur lue en valeur de tension entre 0 et 5000 mV (car pas de virgule)
  x=map(t,0,1023,0,5000);//on transforme la valeur lue en valeur de tension entre 0 et 5000 mV (car pas de virgule)
  capteur[1]=map(x,0,1750,-50,125); //on transformela tension (de 0 à 1750mV en température (de -50°C à 125°C);
  x=0; 
  /////////////////////////////////////////////////
  ///////////////Humidité//////////////////////////
  Serial.println(F("recuperation de l'Hydrometrie"));
  x=0;
  x=analogRead(aHum);
  capteur[2]=map(x,0,950,0,100);
  x=0;
  /////////////////////////////////////////////////////////
  /////////////////Luminosité/////////////////////////////
    Serial.println(F("recuperation de la luminosité"));
  x=0;
  x=analogRead(aLum);
  if (x<luminMin)//si il faut nuit
  {
    capteur[5]=0;  
  }
  else//si il fait jour
  {
  capteur[5]=1;  
  }
  x=0;
  ///////////////////////////////////////////////////////////
  ///////////////Heure et minute////////////////////////////
  Serial.println(F("Repcuperation de l'heure"));
  String heureStr;
  heureStr=rtc.getTimeStr()[0];
  heureStr+=rtc.getTimeStr()[1];
  capteur[3]=heureStr.toInt();

  heureStr=rtc.getTimeStr()[0];
  heureStr+=rtc.getTimeStr()[1];
  capteur[4]=heureStr.toInt();
}

void gestOut ()
{
  Serial.println(F("execution des sorties"));
  ////////////////////////////////////////////////
  ////////////////Temperature/////////////////////
  Serial.print(F("Chauffage: "));
  if (capteur[1]<param[1]-3)
  {
   digitalWrite(sortie[1],LOW);
   Serial.println(F("on")); 
  }
  else
  {
   digitalWrite(sortie[1],HIGH);
   Serial.println(F("off"));    
  }

  ////////////////////////////////////////////
  ////////////Humidité////////////////////////

  //////////////////////////////////////////////
  //////////////Luminosité//////////////////////
    Serial.println(F("Eclairage: "));
  if (capteur[5]=0)
  {
    if(capteur[3]>param[3]||capteur[3]<param[5])
    {
      if(capteur[4]>param[4]||capteur[3]<param[6])
      {
        digitalWrite(sortie[1],LOW);
        Serial.println(F("on")); 
      }
    } 
  }
  else
  {
   digitalWrite(sortie[1],HIGH);
   Serial.println(F("off")); 
  }  
}









void setup() 
{
  Serial.begin(9600); //débute la communication avec le moniteur série
//  monRtc.begin(); // Initialize the rtc object
  Serial.println(F("***********Programme Server SD************* \n"));
  pinMode(sortie[1],OUTPUT);
  pinMode(sortie[2],OUTPUT);
  pinMode(sortie[3],OUTPUT);
  initSd();
  initServeur();
  initRtc();
}

void loop() {
  if (online)
  {
  gestServeur();
  }
  Serial.println("suite");

}


