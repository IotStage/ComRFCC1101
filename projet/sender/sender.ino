/******************************************
 * Radio informations sender with cc1101  *
 * create by BNG                          *
 * 06/02/2017                             *
 * version 1                              *
 ******************************************/

#include "EEPROM.h"
#include "cc1101.h"

//creation d'un objet cc1101
CC1101 cc1101;

byte syncWord[2] = {199, 0}; // mot de synchronisation

CCPACKET paquet; // le paquet envoye (debut de trame | syncword | donnees utiles | FCS)

// a flag that a wireless packet has been received
boolean packetAvailable = false;

// declaration des prototypes de fonction
void initialisation(void);
void cc1101signalsInterrupt(void);
void envoiPaquet(void);
void formatPaquet(String message);
void verificationPaquetAEnvoyer();


void setup()
{
  Serial.begin(9600);
  initialisation();
}


void loop(){
  //verificationPaquetAEnvoyer();
  //verificationPaquetRecu();
  //delay(2000);
  formatPaquet("test");
  formatPaquet("test Emetteur");
  
}

void initialisation(){
  Serial.println("Debut");
  
  // initialisation de l'antenne RF
  cc1101.init();
  
 //cc1101.setSyncWord(syncWord, false); //activation de la synchronisation
 //cc1101.setCarrierFreq(CFREQ_868); //changement de frequence
 //cc1101.set_868_GFSK_500_K();       //changement du type de modulation et du debit (modulation GFSK, debit 500 kbauds)
 //cc1101.set_868_GFSK_1_2_K();       //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds)
  cc1101.set_433_GFSK_1_2_K();        //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds avec frequence 433 Mhz)
  cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
  
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  Serial.println("Equipement initialise");
}

/* Handle interrupt from CC1101 (INT0) gdo0 on pin2 */
void cc1101signalsInterrupt(void){
  // set the flag that a package is available
  packetAvailable = true;
}


void envoiPaquet() {
  if(cc1101.sendData(paquet)){
    Serial.println("Envoi termine");
   }else{
    Serial.println("Echec de l'envoi");
  }
}

/***
 * format segmentatoin des donnees a envoyer en paquet, 
 * ajout d'une entete specifique (~@]#) pour limiter les paquets
 */
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


