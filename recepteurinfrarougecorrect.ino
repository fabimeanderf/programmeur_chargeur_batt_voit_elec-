/*inclusion des bibliotèques utiles à ce projet*/
#include <IRremote.h> // include the IRremote library
#define RECEIVER_PIN 2 /* définir la broche du récepteur IR*/

/*#define outputCommand 8*/
/*byte sortie[outputCommand]; /*déclaration d'un tableau commandant 8 sorties*/
const byte multi= 4;
const byte defaut=5;
const byte circuitOrange = 6;
const byte circuitViolet = 7;
const byte circuitJaune = 8;
const byte circuitVert = 9;
const byte circuitBleu = 10;  
unsigned long previousMillis = 0; /*on utilise la fonction millis en donnant uen valeur 0 au départ du microcontrôleur */
unsigned long currentMillis;/*récupère la valeur de millis lorsqu'elle est demandée*/
const int interval500mS = 500;/*cela représente 500ms */
const byte speakerPin= 12;

/*pin 12 commande buzzer, pin 4 commande multiplexage  */
/*pin5 led défaut code non reconnu,pin 6 commande circuit orange*/
/*pin 7 commande circuit rose, pin 8 commande circuit jaune */
/*pin9 commande circuit vert, pin 10 commande circuit bleu*/

IRrecv receiver(RECEIVER_PIN); // crée un objet receiver de la classe  IRrecv  et la lie à la broche d'entrée du récepteur (broche2 )
decode_results results; // crée une instance appelé results de la classe decode_results ,Cet objet stocke les valeurs décodées de l’objet récepteur. Nous accèderons à ces valeurs dans le reste du code.
unsigned long key_value = 0; // variable stockant la valeur de key value, la valeur du code HEXADECIMALE reçu"

void setup() {
receiver.enableIRIn(); //  initialision du récepteur avec la fonction  enableIRIn().

/*boucle d'initiation des sorties et mise à l'état haut pour la pin 4 */
   for(int i=4 ; i<=10 ; i++){// on définit les différentes commandes relais en sortie 
     pinMode(i,OUTPUT);
     if(i==4){/*allumage multiplexage ,seule la sortie 4 est placée à l'état haut à l'initialisation*/
      pinMode(4,OUTPUT);
       digitalWrite(4,HIGH);
      }
     else{ 
   digitalWrite(i,LOW);//on les met en position 0 absence de commande des relais
     }
   }/*fermeture de la boucle for */
    pinMode(speakerPin,OUTPUT);
 }/*fermeture de void setup*/


void loop() {
    if (receiver.decode(&results)) { /* decode the received signal and store it in results*/
      if  (results.value == 0xFFFFFFFF)  { /* si la valeur est égale à 0xFFFFFFFF*/
      results.value = key_value; /* définit la valeur sur la valeur clé*/
       }     /*lorsque l'on appuie  continuellement sur une touche, on commence  à recevoir 0xFFFFFFFF de la télécommande. Cela signifie une répétition de la clé précédente. */


       /*un switch pour rechercher les différents codes reçus et commander les relais*/
      
       switch(results.value){ /* on compare le code de réception avec les codes proposés */
        case 0xFFA25D: toggleLED(4);buzSound(200,1200); break;// exécuter la fonction de multiplexage des leds
        delay(500);
        case 0xFF629D: toggleLED(6);buzSound(200,1200); break;// exécuter la fonction de bascule de la sortie sur la sortie orange
         delay(500);
        case 0xFFE21D: toggleLED(7);buzSound(200,1200);break;// exécuter la fonction de bascule de la sortie sur la sortie violette
         delay(500);
        case 0xFF22DD: toggleLED(8);buzSound(200,1200);break;// exécuter la fonction de bascule de la sortie sur la sortie jaune pale
          delay(500);
        case 0xFF02FD: toggleLED(9);buzSound(200,1200);break;// exécuter la fonction de bascule de la sortie sur la sortie verte
         delay(500);
        case 0xFFC23D: toggleLED(10);buzSound(200,1200);break;// exécuter la fonction de bascule de la sortie sur la sortie bleue
         delay(500);
        case 0xFFE01F: arretTotal();buzSound(200,1200);break;// exécuter la fonction de bascule de la sortie sur la sortie bleue
        delay(500);
        default:defaultCircuit();buzSoundDefault(300,600);break;
        delay(500);
     }
  /*à la fin de la boucle. Lorsque le code de répétition 0xFFFFFFFF est reçu, nous le remplaçons par la valeur de clé précédente que nous avons stockée comme  
   key_value.*/
   key_value = results.value;/* stocke la valeur HEXADECIMALE reçue sous la forme key_value avant une nouvelle boucle */
   receiver.resume();// réinitialise le récepteur pour le code suivant
  /*la fonction  resume() est appelée, ce qui réinitialise le récepteur et le prépare à recevoir le code suivant.*/
   }
}

     void defaultCircuit(){
       digitalWrite(5,HIGH);
       delay(500);
       digitalWrite(5,LOW);
       
    }
    
   void  toggleLED (int pin)  { /*/ fonction pour mettre en service ou arrêter les relais */
           currentMillis = millis();
           if (currentMillis - previousMillis >= interval500mS) {
             previousMillis = currentMillis;
              if  ( digitalRead ( pin ) == HIGH )  { /* si le relais est excité*/
                 digitalWrite ( pin, LOW ) ; /* on le desexcite*/
              }
              else {
                  digitalWrite ( pin, HIGH ) ; /*  on excite le relais */
              }/*fermeture du else*/
           } /*fermeture du if de lecture du pin */    
      }/*fermeture du void toggleLED*/
  
  void buzSound(int period,int duration){
    /*periode est un cycle de note  
     duree est la durée de l'émission d'impulsions en milliseconde 
     */
     int pulse= period/2;
     for(long i = 0; i < duration*1000;i += period){
      digitalWrite(speakerPin,HIGH);
      delayMicroseconds(pulse);
      digitalWrite(speakerPin,LOW);
      delayMicroseconds(pulse);
     }
  }

  void buzSoundDefault(int period,int duration){
      int pulse= period/2;
     for(long i = 0; i < duration*1000;i += period){
      digitalWrite(speakerPin,HIGH);
      delayMicroseconds(pulse);
      digitalWrite(speakerPin,LOW);
      delayMicroseconds(pulse);
     }
     delay(200);
      for(long i = 0; i < duration*1000;i += period){
      digitalWrite(speakerPin,HIGH);
      delayMicroseconds(pulse);
      digitalWrite(speakerPin,LOW);
      delayMicroseconds(pulse);
      }
  }

  void arretTotal(){
    for (int k=6;k<=10;k++){
      digitalWrite(k,LOW);
       }
  }
 

 



   

  
