/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier :       prgArduino-1-TestKY040simple.ino
  
  Description :   Programme permettant de lire les états des lignes CLK, DT, et SW d'un encodeur rotatif KY-040,
                  d'effectuer un comptage du nombre de crans parcourus, de déterminer dans quel sens a été tourné
                  l'encodeur (sens horaire, ou anti-horaire), et d'afficher le tout sur le moniteur série de l'IDE Arduino

  Remarques :     - l'arduino utilisé ici sera un modèle Nano
                  - les signaux CLK et SW, émanant du KY-040, seront scrutés en permanence ; et le programme réagira
                  à chaque changement de l'un d'entre eux
                                    
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       12.05.2023

*/

// Constantes
#define pinArduinoRaccordementSignalSW  2       // La pin D2 de l'Arduino recevra la ligne SW du module KY-040
#define pinArduinoRaccordementSignalCLK 3       // La pin D3 de l'Arduino recevra la ligne CLK du module KY-040
#define pinArduinoRaccordementSignalDT  4       // La pin D4 de l'Arduino recevra la ligne DT du module KY-040

// Variables
int compteur = 0;                   // Cette variable nous permettra de savoir combien de crans nous avons parcourus sur l'encodeur
                                    // (sachant qu'on comptera dans le sens horaire, et décomptera dans le sens anti-horaire)

int etatPrecedentLigneSW;           // Cette variable nous permettra de stocker le dernier état de la ligne SW lu, afin de le comparer à l'actuel
int etatPrecedentLigneCLK;          // Cette variable nous permettra de stocker le dernier état de la ligne CLK lu, afin de le comparer à l'actuel

// ========================
// Initialisation programme
// ========================
void setup() {

    // Initialisation de la liaison série (arduino nano <-> PC)
    Serial.begin(9600);
    Serial.println(F("========================================================================="));
    Serial.println(F("Exemple 1 (programme de test Arduino Nano <-> module KY-040, scrutant en "));
    Serial.println(F("           permanence certains changement d'états de lignes de l'encodeur"));
    Serial.println(F("           KY-040, avec affichage des informations sur le moniteur série)"));
    Serial.println(F("========================================================================="));
    Serial.println("");

    // Configuration des pins de notre Arduino Nano en "entrées", car elles recevront les signaux du KY-040
    pinMode(pinArduinoRaccordementSignalSW, INPUT);         // à remplacer par : pinMode(pinArduinoRaccordementSignalSW, INPUT_PULLUP);
                                                            // si jamais votre module KY-040 n'est pas doté de résistance pull-up, au niveau de SW
    pinMode(pinArduinoRaccordementSignalDT, INPUT);
    pinMode(pinArduinoRaccordementSignalCLK, INPUT);

    // Mémorisation des valeurs initiales, au démarrage du programme
    etatPrecedentLigneSW  = digitalRead(pinArduinoRaccordementSignalSW);
    etatPrecedentLigneCLK = digitalRead(pinArduinoRaccordementSignalCLK);

    // Affichage de la valeur initiale du compteur, sur le moniteur série
    Serial.print(F("Valeur initiale du compteur = "));
    Serial.println(compteur);

    // Petite pause pour laisser se stabiliser les signaux, avant d'attaquer la boucle loop
    delay(200);

}


// =================
// Boucle principale
// =================
void loop() {

    // Lecture des signaux du KY-040 arrivant sur l'arduino
    int etatActuelDeLaLigneCLK = digitalRead(pinArduinoRaccordementSignalCLK);
    int etatActuelDeLaLigneSW  = digitalRead(pinArduinoRaccordementSignalSW);
    int etatActuelDeLaLigneDT  = digitalRead(pinArduinoRaccordementSignalDT);

    // *****************************************
    // On regarde si la ligne SW a changé d'état
    // *****************************************
    if(etatActuelDeLaLigneSW != etatPrecedentLigneSW) {

        // Si l'état de SW a changé, alors on mémorise son nouvel état
        etatPrecedentLigneSW = etatActuelDeLaLigneSW;

        // Puis on affiche le nouvel état de SW sur le moniteur série de l'IDE Arduino
        if(etatActuelDeLaLigneSW == LOW)
            Serial.println(F("Bouton SW appuyé"));
        else
            Serial.println(F("Bouton SW relâché"));

        // Petit délai de 10 ms, pour filtrer les éventuels rebonds sur SW
        delay(10);
    }

    // ******************************************
    // On regarde si la ligne CLK a changé d'état
    // ******************************************
    if(etatActuelDeLaLigneCLK != etatPrecedentLigneCLK) {

      // On mémorise cet état, pour éviter les doublons
      etatPrecedentLigneCLK = etatActuelDeLaLigneCLK;

      if(etatActuelDeLaLigneCLK == LOW) {
        
        // On compare le niveau de la ligne CLK avec celui de la ligne DT
        // --------------------------------------------------------------
        // Nota : - si CLK est différent de DT, alors cela veut dire que nous avons tourné l'encodeur dans le sens horaire
        //        - si CLK est égal à DT, alors cela veut dire que nous avons tourné l'encodeur dans le sens anti-horaire

        if(etatActuelDeLaLigneCLK != etatActuelDeLaLigneDT) {
            // CLK différent de DT => cela veut dire que nous comptons dans le sens horaire
            // Alors on incrémente le compteur
            compteur++;

            // Et on affiche ces infos sur le moniteur série
            Serial.print(F("Sens = horaire | Valeur du compteur = "));
            Serial.println(compteur);
        }
        else {
            // CLK est identique à DT => cela veut dire que nous comptons dans le sens antihoraire
            // Alors on décrémente le compteur
            compteur--;

            // Et on affiche ces infos sur le moniteur série
            Serial.print(F("Sens = antihoraire | Valeur du compteur = "));
            Serial.println(compteur);
        }

        // Petit délai de 1 ms, pour filtrer les éventuels rebonds sur CLK
        delay(1);
        
      }
    }

    // ********************************
    // Puis on reboucle … à l'infini !
    // ********************************
   
}
