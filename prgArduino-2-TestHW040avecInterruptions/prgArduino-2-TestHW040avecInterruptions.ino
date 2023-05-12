/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier :       prgArduino-2-TestHW040avecInterruptions.ino
  
  Description :   Programme permettant de compter le nombre de crans tournés sur un encodeur HW-040,
                  de déterminer le sens de rotation, et d'afficher le tout sur le moniteur série, de
                  l'IDE Arduino.
                  de l'encodeur se fera via les entrées d'interruption arduino INT0 (rattaché à la
                  broche D2 de l'arduino) et INT1 (rattaché à la broche D3 de l'arduino)
  
  Remarques :     - l'arduino utilisé ici sera un modèle Nano
                  - le choix des broches D2 et D3 pour les interruption n'est pas anodin ; cela correspond
                  aux entrées d'interruptions externes INT0 (D2) et INT1 (D3), natives sur arduino
                                    
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       02.05.2023

*/

// Constantes
#define pinArduinoRaccordementSignalSW  2       // La pin D2 de l'Arduino recevra la ligne SW du module HW-040
#define pinArduinoRaccordementSignalCLK 3       // La pin D3 de l'Arduino recevra la ligne CLK du module HW-040
#define pinArduinoRaccordementSignalDT  4       // La pin D4 de l'Arduino recevra la ligne DT du module HW-040

// Variables
int compteur = 0;                   // Cette variable nous permettra de compter combien de crans ont été parcourus, sur l'encodeur
                                    // (sachant que nous compterons dans le sens horaire, et décompterons dans le sens antihoraire)

int etatPrecedentLigneSW;           // Cette variable nous permettra de stocker le dernier état de la ligne SW, afin de le comparer à l'actuel
int etatPrecedentLigneCLK;          // Cette variable nous permettra de stocker le dernier état de la ligne CLK, afin de le comparer à l'actuel

// ========================
// Initialisation programme
// ========================
void setup() {

    // Initialisation de la liaison série (arduino nano <-> PC)
    Serial.begin(9600);
    Serial.println(F("=========================================================================="));
    Serial.println(F("Exemple 2 (programme de test Arduino Nano <-> module HW-040, utilisant les"));
    Serial.println(F("           interruptions Arduino INT0 et INT1, avec affichage du nombre de"));
    Serial.println(F("           crans parcourus sur l'encodeur, ainsi que le sens de rotation)"));
    Serial.println(F("========================================================================="));
    Serial.println("");

    // Configuration de certaines pins de notre Arduino Nano en "entrées" (celles qui recevront les signaux du HW-040)
    pinMode(pinArduinoRaccordementSignalSW, INPUT);         // à remplacer par : pinMode(pinArduinoRaccordementSignalSW, INPUT_PULLUP);
                                                            // si jamais votre module HW-040 n'est pas doté de résistance pull-up, au niveau de SW
    pinMode(pinArduinoRaccordementSignalDT, INPUT);
    pinMode(pinArduinoRaccordementSignalCLK, INPUT);

    // Petite pause pour laisser le temps aux signaux de se stabiliser
    delay(200);

    // Mémorisation des valeurs par défaut, avant tout
    etatPrecedentLigneSW = digitalRead(pinArduinoRaccordementSignalSW);
    etatPrecedentLigneCLK = digitalRead(pinArduinoRaccordementSignalCLK);

    // Affichage de la valeur initiale du compteur, sur le moniteur série
    Serial.print(F("Valeur initiale du compteur = "));
    Serial.println(compteur);

    // Activation d'interruptions sur les lignes CLK et SW
    attachInterrupt(digitalPinToInterrupt(pinArduinoRaccordementSignalCLK), changementDetecteSurLigneCLK, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinArduinoRaccordementSignalSW), changementDetecteSurLigneSW, CHANGE);

}


// =================
// Boucle principale
// =================
void loop() {

    // Boucle infinie ici, qui sera interrompue à chaque nouvelle interruption, et reprise juste après la fin d'exécution de la routine d'interruption

}


// =====================================================
// Routine d'interruption : changementDetecteSurLigneCLK
// =====================================================
void changementDetecteSurLigneCLK() {

    // Lecture des signaux CLK et DT du HW-040, arrivant sur l'arduino
    int etatActuelDeLaLigneCLK = digitalRead(pinArduinoRaccordementSignalCLK);
    int etatActuelDeLaLigneDT  = digitalRead(pinArduinoRaccordementSignalDT);

    // Mémorisation du nouveau état de CLK, puisqu'il vient de changer (sans quoi, cette interruption n'aurait pas été appelée)
    etatPrecedentLigneCLK = etatActuelDeLaLigneCLK;

    // On compare ensuite l'état des lignes CLK et DT
    // ----------------------------------------------
    // Nota : - si CLK différent de DT, alors cela signifie que nous avons tourné l'encodeur dans le sens horaire
    //        - si CLK égal à DT, alors cela signifie que nous avons tourné l'encodeur dans le sens antihoraire

    if(etatActuelDeLaLigneCLK != etatActuelDeLaLigneDT) {
        // CLK différent de DT => cela veut dire que nous tournons dans le sens horaire
        // Alors on incrémente le compteur
        compteur++;

        // Et on affiche ces infos sur le moniteur série
        Serial.print(F("Sens = horaire | Valeur du compteur = "));
        Serial.println(compteur);
    }
    else {
        // CLK est identique à DT => cela veut dire que nous tournons dans le sens antihoraire
        // Alors on décrémente le compteur
        compteur--;

        // Et on affiche ces infos sur le moniteur série
        Serial.print(F("Sens = antihoraire | Valeur du compteur = "));
        Serial.println(compteur);
    }

}


// ====================================================
// Routine d'interruption : changementDetecteSurLigneSW
// ====================================================
void changementDetecteSurLigneSW() {

    // On lit le nouvel état de la ligne SW
    int etatActuelDeLaLigneSW = digitalRead(pinArduinoRaccordementSignalSW);

    // On mémorise le nouvel état de la ligne SW, puisqu'il vient de changer (sans quoi nous ne serions pas dans cette routine d'interruption)
    etatPrecedentLigneSW = etatActuelDeLaLigneSW;

    // Puis on affiche le nouvel état de SW sur le moniteur série de l'IDE Arduino
    if(etatActuelDeLaLigneSW == LOW)
        Serial.println(F("Bouton SW appuyé"));
    else
        Serial.println(F("Bouton SW relâché"));

}
