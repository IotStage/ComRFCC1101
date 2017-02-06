// ce programme permet d'envoyer des message de taille 60 caractères
// si le message depasse 60 caractères, on le decoupe en message de 
//55 caractères puis on y ajoute la séquence de séparation ~@]#`
#include "EEPROM.h"
#include "cc1101.h"

CC1101 cc1101;

byte syncWord[2] = {199, 0};
CCPACKET paquet;

void setup()
{
  Serial.begin(9600);
  Serial.println("Debut");
  
 
  Serial.println("initializing...");
  // Initialisation de l'antenne RF
  cc1101.init();
  

//cc1101.setCarrierFreq(CFREQ_433);

cc1101.set_868_GFSK_500_K();
  
  cc1101.disableAddressCheck();
  
  delay(1000);
  
  Serial.println("Initialisation terminee");
  Serial.println("Entrez le message a envoyer");
}

void envoiPaquet() {
  if(cc1101.sendData(paquet)){
    Serial.println("Envoi termine");
   }else{
    Serial.println("Echec de l'envoi");
  }
}

void formatPaquet(String message){
  if(message.length()<61){
    Serial.print("Taille ");
    Serial.println(message.length());
    paquet.length=message.length();
    message.getBytes(paquet.data, message.length()+1);
    envoiPaquet();
  }
  else{
    String partie1 = message.substring(0, 55);
    String partie2 = message.substring(55);
    partie1+="~@]#`";
    formatPaquet(partie1);
    //delay(500);
    formatPaquet(partie2);
  }
}



void loop()
{
  if(Serial.available() > 0) {
    String message = Serial.readString();
    formatPaquet(message);
  }
}
