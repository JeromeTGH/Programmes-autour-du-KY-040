/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier :       prgArduino-2-TestKY040avecInterruptions.ino
  
  Description :   Programme permettant de compter le nombre de crans tournés sur un encodeur EC11 (monté
                  sur module KY-040), et d'indiquer le sens de rotation suivi. La détection des signaux
                  de l'encodeur se fera via les entrées d'interruption arduino INT0 (rattaché à la
                  broche D2 de l'arduino) et INT1 (rattaché à la broche D3 de l'arduino)
  
  Remarques :     - l'arduino utilisé ici sera un modèle Nano
                  - le choix des broches D2 et D3 pour les interruption n'est pas anodin ; en effet,
                    cela peut différer ou être plus complexe, si vous utilisez d'autres broches de l'arduino
                                    
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       02.05.2023

*/

// Constantes
#define pinArduinoRaccordementSignalSW          2       // La pin D2 de l'Arduino recevra la ligne SW du module KY-040
#define pinArduinoRaccordementSignalCLKsoitA    3       // La pin D3 de l'Arduino recevra la ligne CLK du module KY-040 (soit la ligne "A" de l'encodeur)
#define pinArduinoRaccordementSignalDTsoitB     4       // La pin D4 de l'Arduino recevra la ligne DT du module KY-040 (soit la ligne "B" de l'encodeur)

// Variables
int compteur = 0;                   // Cette variable nous permettra de compter combien de crans ont été parcourus, sur l'encodeur
                                    // (sachant que nous compterons dans le sens horaire, et décompterons dans le sens antihoraire)

int etatPrecedentLigneSW;           // Cette variable nous permettra de stocker le dernier état de la ligne SW, afin de le comparer à l'actuel
int etatPrecedentLigneCLKsoitA;     // Cette variable nous permettra de stocker le dernier état de la ligne CLK, afin de le comparer à l'actuel

// ========================
// Initialisation programme
// ========================
void setup() {

    // Initialisation de la liaison série (arduino nano <-> PC)
    Serial.begin(9600);
    Serial.println(F("=========================================================================="));
    Serial.println(F("Exemple 2 (programme de test Arduino Nano <-> module KY-040, utilisant les"));
    Serial.println(F("           interruptions Arduino INT0 et INT1, avec affichage du nombre de"));
    Serial.println(F("           crans parcourus sur l'encodeur, ainsi que le sens de rotation)"));
    Serial.println(F("========================================================================="));
    Serial.println("");

    // Configuration de certaines pins de notre Arduino Nano en "entrées" (celles qui recevront les signaux du KY-040)
    pinMode(pinArduinoRaccordementSignalSW, INPUT);
    pinMode(pinArduinoRaccordementSignalDTsoitB, INPUT);
    pinMode(pinArduinoRaccordementSignalCLKsoitA, INPUT);

    // Petite pause pour laisser le temps aux signaux de se stabiliser
    delay(200);

    // Mémorisation des valeurs par défaut, avant tout
    etatPrecedentLigneSW = digitalRead(pinArduinoRaccordementSignalSW);
    etatPrecedentLigneCLKsoitA = digitalRead(pinArduinoRaccordementSignalCLKsoitA);

    // Affichage de la valeur initiale du compteur, sur le moniteur série
    Serial.print(F("Valeur initiale du compteur = "));
    Serial.println(compteur);

    // Activation d'interruption
    attachInterrupt(digitalPinToInterrupt(pinArduinoRaccordementSignalCLKsoitA), changementDetecteSurLigneCLKsoitA, CHANGE);

}


// =================
// Boucle principale
// =================
void loop() {

    // Boucle infinie ici, qui sera interrompue à chaque nouvelle interruption, et reprise juste après la fin d'exécution de la routine d'interruption

}


// ==========================================================
// Routine d'interruption : changementDetecteSurLigneCLKsoitA
// ==========================================================
void changementDetecteSurLigneCLKsoitA() {

    // Lecture des signaux CLK(A) et DT(B) du KY-040, arrivant sur l'arduino
    int etatActuelDeLaLigneSCKsoitA = digitalRead(pinArduinoRaccordementSignalCLKsoitA);
    int etatActuelDeLaLigneDTsoitB  = digitalRead(pinArduinoRaccordementSignalDTsoitB);

    // Mémorisation du nouveau état de CLK (A), puisqu'il vient de changer (sans quoi, cette interruption n'aurait pas été appelée)
    etatPrecedentLigneCLKsoitA = etatActuelDeLaLigneSCKsoitA;

    // On compare ensuite l'état des lignes CLK (A) et DT (B)
    // ------------------------------------------------------
    // Nota : - si A différent de B, alors cela signifie que nous avons tourné l'encodeur dans le sens horaire
    //        - si A égal à B, alors cela signifie que nous avons tourné l'encodeur dans le sens antihoraire

    if(etatActuelDeLaLigneSCKsoitA != etatActuelDeLaLigneDTsoitB) {
        // A différent de B => cela veut dire que nous tournons dans le sens horaire
        // Alors on incrémente le compteur
        compteur++;

        // Et on affiche ces infos sur le moniteur série
        Serial.print(F("Sens = horaire | Valeur du compteur = "));
        Serial.println(compteur);
    }
    else {
        // A est identique à B => cela veut dire que nous tournons dans le sens antihoraire
        // Alors on décrémente le compteur
        compteur--;

        // Et on affiche ces infos sur le moniteur série
        Serial.print(F("Sens = antihoraire | Valeur du compteur = "));
        Serial.println(compteur);
    }

}
