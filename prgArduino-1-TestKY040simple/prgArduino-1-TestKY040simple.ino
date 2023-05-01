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
  
  Description :   Programme permettant de lire les états des lignes CLK(A), DT(B), et SW
                  d'un encodeur rotatif EC11 monté en module KY-040, d'effectuer un comptage
                  des pas effectués, d'indiquer dans quel sens a été tourné l'encodeur (sens horaire,
                  ou anti-horaire), et d'afficher le tout sur le moniteur série de l'IDE Arduino

  Remarques :     - les signaux émanant du KY-040 seront scrutés en permanence, et le programme
                  agira en conséquence, en cas de changement de certains d'entre eux
                  - l'arduino utilisé ici sera un modèle Nano
                                    
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       01.05.2023

*/

// Constantes
#define pinArduinoRaccordementSignalSW          2           // La pin D2 de l'Arduino recevra la ligne SW du module KY-040
#define pinArduinoRaccordementSignalDTsoitB     3           // La pin D3 de l'Arduino recevra la ligne DT du module KY-040 (soit la ligne "B" de l'encodeur)
#define pinArduinoRaccordementSignalCLKsoitA    4           // La pin D4 de l'Arduino recevra la ligne CLK du module KY-040 (soit la ligne "A" de l'encodeur)

// Variables
int etatPrecedentLigneSW        = HIGH;    // Cette variable nous permettra de stocker le dernier état de la ligne SW lue, afin de le comparer à l'actuel
int etatPrecedentLigneCLKsoitA  = HIGH;    // Cette variable nous permettra de stocker le dernier état de la ligne CLK lue, afin de le comparer à l'actuel
                                                    // Nota : j'ai mis HIGH par défaut, car les lignaux du KY-040, telles que câblées ici avec ce module,
                                                    //        sont actives à l'état bas, donc, au repos à l'état haut (d'où le "HIGH")

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
    pinMode(pinArduinoRaccordementSignalSW, INPUT);
    pinMode(pinArduinoRaccordementSignalDTsoitB, INPUT);
    pinMode(pinArduinoRaccordementSignalCLKsoitA, INPUT);

    // Petite pause pour laisser se stabiliser les signaux, avant d'attaquer la boucle loop
    delay(200);

}


// =================
// Boucle principale
// =================
void loop() {

    // Lecture des signaux du KY-040 arrivant sur l'arduino
    int etatActuelDeLaLigneSW       = digitalRead(pinArduinoRaccordementSignalSW);
    int etatActuelDeLaLigneSCKsoitA = digitalRead(pinArduinoRaccordementSignalCLKsoitA);

    // On regarde si le bouton SW a changé d'état
    if(etatActuelDeLaLigneSW != etatPrecedentLigneSW) {
        
    }

    // On regarde si le bouton CLK (soit A) a changé d'état
    if(etatActuelDeLaLigneSCKsoitA != etatPrecedentLigneCLKsoitA) {
        
    }

}