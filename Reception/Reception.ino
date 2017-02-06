#include "EEPROM.h"
#include "cc1101.h"

// The connection to the hardware chip CC1101 the RF Chip
CC1101 cc1101;

byte syncWord[2] = {199, 0};

// a flag that a wireless packet has been received
boolean packetAvailable = false;


/* Handle interrupt from CC1101 (INT0) gdo0 on pin2 */
void cc1101signalsInterrupt(void){
  // set the flag that a package is available
  packetAvailable = true;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Debut");
  
  // initialisation de l'antenne RF
  cc1101.init();
  
//  cc1101.setSyncWord(syncWord, false);
//  cc1101.setCarrierFreq(CFREQ_433);
  //cc1101.setTxPowerAmp(PA_LowPower);

  cc1101.set_868_GFSK_500_K();
  cc1101.disableAddressCheck();
  
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  
  
  Serial.println("Equipement initialise");
}

void receptionPaquet(){
  CCPACKET paquet;
    
    if(cc1101.receiveData(&paquet) > 0){
      if(!paquet.crc_ok) {
        Serial.println("Erreur CRC");
      }
      
      if(paquet.length > 0){
        if(paquet.length == 60 && paquet.data[55]=='~' && paquet.data[56]=='@' && paquet.data[57]==']' && paquet.data[58]=='#' && paquet.data[59]=='`' ){
          for(int j=0; j<55; j++){
            Serial.write(paquet.data[j]);
          }
        }
        else{
          for(int j=0; j<paquet.length; j++){
            Serial.write(paquet.data[j]);
          }
          Serial.println("");
        }
      }
    }
}

void verificationPaquetRecu(){
  if(packetAvailable){
    //Serial.println("Paquet recu");
    
    // Desactivation de la reception
    detachInterrupt(0);

    packetAvailable = false;
    
    receptionPaquet();
    
    // Activation de la reception
    attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  }
}


void loop()
{
  verificationPaquetRecu();
}
