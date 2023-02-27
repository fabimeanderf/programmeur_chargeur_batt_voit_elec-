/***********************************************************************programmateur_de_prise_green_up_citroen-----------------------------------------------------------
utilisation d'un DS1307 RTC , d'un afficheur LCD 2*16, de deux boutons poussoirs NO et d'une led de sortie  symbolisant le relais de puissance 
Le croquis utilise 6948 octets (21%) de l'espace de stockage de programmes. Le maximum est de 32256 octets.
Les variables globales utilisent 377 octets (18%) de mémoire dynamique, ce qui laisse 1671 octets pour les variables locales. Le maximum est de 2048 octets.
une RTC DS1307 mise à l'heure d'hiver ,
SDA relié sur A4
SCL relié sur A5

le relais de puissance commandé par la pin 2
l'interrupteur CM commandé par la pin 3
l'interupteur PR command"é par la pin 4
l'interrupteur été commandé par la pin 6 
le lcd commandé par la pin 7 pour RS 
le lcd commabndé par la pin 8 pour E
les pins 1,10,11,12 commandent les données sur 4 bits 
ADC0 récupére la conversion analogique numérique du divideur de tension R2,R3,R4,R5 pour attribuer les plages horaires 


 le montage comporte un arrêt général 220 afin de ne pas toujours le mettre en service , La RTC gardant l'information de l'heure par une pile de sauvegarde 

*/
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;

// initialise les différentes commandes LCD 
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int heures;
int minutes;
int secondes;
char messageClock[8] ="";
int positionRotacteur;/*on recherche la valeur analogique de Ao en fonction de la position du rotacteur*/
int intMarche = 3;
int intProg = 4;
int sortieRelais = 2;
int heureEte = 6;

void setup()
{  
   // Initialize DS3231
  clock.begin();/*démarrage de la RTC*/
  lcd.begin(16, 2); /*définit le type d'écran lcd 16 x 2*/
  pinMode(intMarche, INPUT_PULLUP); /*l'interrupteur connecté à la pin 3 qui mettra en servive ou non en mode manuel*/ 
  pinMode(intProg, INPUT_PULLUP); /*l'interrupteur connecté à la pin 4  qui mettra en servive ou non en mode manuel*/ 
  pinMode(sortieRelais,OUTPUT);/*la sortie de commande du relais */
  pinMode(heureEte,INPUT_PULLUP);/*l'interrupteur connécté à la pin 6 qui ajoutera une heure d'avance pour l'heure d'été*/
  } 

void loop()
{
  dt = clock.getDateTime();/*on récupère les données totales de la RTC avec la variable dt*/
 
 heures=dt.hour;/*on récupére les heures minutes et secondes grace à la variable dt*/
 if (digitalRead(heureEte) == LOW){
     if(heures == 23){
    heures = 0;
     }
     else{
     heures = heures+1;
     }
  }
 minutes=dt.minute;
 secondes=dt.second;
  /* on imprime les valeurs d'heure et temps sur l'écran LCD grace à trois variables qui vont être formatées par sprintf*/
   /*formatage de messageClock avec sprintf*/
  /*%2d affiche le chiffre sur 2 caractères */
  /*%02d affiche le chiffrsur 2 caractères et lorsque les nombres sont inférieurs à 10, il affiche un 0 à gauche  */
 sprintf(messageClock,"%02d:%02d:%02d",heures,minutes,secondes);
lcd.setCursor(1,0);/*curseur placé sur la première ligne et première case pour afficher l'heure*/ 
lcd.clear();/*on nettoie avant d'afficher*/
lcd.print("heure :");/*on imprime heure : puis le message Clock formaté à l'emplacement 7 de la première ligne*/
lcd.setCursor(7,0);
lcd.print(messageClock);

lcd.setCursor(0,1 );/*on se place sur la deuxième ligne*/
/*réglage au alentour de 916*/
positionRotacteur=analogRead(A0);
if (positionRotacteur>=780 && positionRotacteur<=880){
   lcd.print("comm.manuelle ");/* par défaut la commande manuelle est à l'arrêt*/
   digitalWrite(sortieRelais,LOW);
   lcd.setCursor(15,1);
   lcd.print("A");
   if (digitalRead(intMarche) == LOW){/*si interrupteur fermé la commande manuelle est en marche*/
     digitalWrite(sortieRelais, HIGH);
     lcd.setCursor(15,1);
     lcd.print("M");
   }
}

 /*position plage 7h 13h en fonction marche que sur ce créneau*/
 /*réglage au alentour de 613  3/5 de 1023*/ 
else if(positionRotacteur>=580 && positionRotacteur<=680){
  lcd.setCursor(0,1);
  lcd.print("plage 7h 13h");
  digitalWrite(sortieRelais, LOW);
  lcd.setCursor(15,1);
  heures=dt.hour;/*on rappelle la variable heure  de la RTC  */
  if (digitalRead(heureEte) == LOW){
     if(heures == 23){
    heures = 0;
     }
     else{
     heures = heures+1;
     }
  }
  lcd.print("A");
   if((heures>=7 && heures <13) && digitalRead(intProg) == LOW){//inférieur à 13 pour que la plage se termine à 12h59mn59s
     digitalWrite(sortieRelais, HIGH);
     lcd.setCursor(15,1);
     lcd.print("M");
   }
}
 

 /*position plage 14h00 20h00 en fonction marche que sur ce créneau*/ 
 /*réglage au alentour de 409   2/5 de 1023*/ 
else if(positionRotacteur>=380 && positionRotacteur<=480){
  lcd.setCursor(1,1);
  lcd.print("plage 14h 20h");
  digitalWrite(sortieRelais, LOW);
  lcd.setCursor(15,1);
  heures=dt.hour;/*on rappelle la variable heure  de la RTC  */
  if (digitalRead(heureEte) == LOW){
     if(heures == 23){
    heures = 0;
     }
     else{
     heures = heures+1;
     }
  }
  lcd.print("A");
     if((heures>=14 && heures<20) && digitalRead(intProg) == LOW ){//inférieur à 20 pour que la plage se termine à 19h59mn59s
     digitalWrite(sortieRelais, HIGH);
     lcd.setCursor(15,1);
     lcd.print("M");
  }
}
 

/*position plage 0h00 5h00 en fonction marche que sur ce créneau*/ 
/*réglage au alentour de 204 1/5 de 1023*/ 
else if(positionRotacteur>=180 && positionRotacteur <=280){
    lcd.setCursor(2,1);
    lcd.print("plage 1h 7h");
    digitalWrite(sortieRelais, LOW);
    lcd.setCursor(15,1);
    heures=dt.hour;/*on rappelle la variable heure  de la RTC  */
   if (digitalRead(heureEte) == LOW){
     if(heures == 23){
    heures = 0;
     }
     else{
     heures = heures+1;
     }
  }
    lcd.print("A");
    if((heures>=1 && heures <8) && digitalRead(intProg) == LOW){//inférieur à 8 pour que la plage se termine à 07h59mn59s
      digitalWrite(sortieRelais,HIGH); 
      lcd.setCursor(15,1);
      lcd.print("M");
    } 
}

/*par défaut en dehors des créneaux horaires , le relais est au repos*/
else{
      digitalWrite(sortieRelais,LOW); 
}

/*on boucle tous les 10ms*/
  delay(300);
}
