/*
   ______               _                  _///_ _           _                   _
  /   _  \             (_)                |  ___| |         | |                 (_)
  |  [_|  |__  ___  ___ _  ___  _ __      | |__ | | ___  ___| |_ _ __ ___  _ __  _  ___  _   _  ___
  |   ___/ _ \| __|| __| |/ _ \| '_ \_____|  __|| |/ _ \/  _|  _| '__/   \| '_ \| |/   \| | | |/ _ \
  |  |  | ( ) |__ ||__ | | ( ) | | | |____| |__ | |  __/| (_| |_| | | (_) | | | | | (_) | |_| |  __/
  \__|   \__,_|___||___|_|\___/|_| [_|    \____/|_|\___|\____\__\_|  \___/|_| |_|_|\__  |\__,_|\___|
                                                                                      | |
                                                                                      \_|
  Fichier :       prgArduino-3-TestKY040avecAntiRebond.ino
  
  Description :   Programme permettant de compter le nombre de crans tournés sur un encodeur KY-040, de
                  déterminer le sens de rotation, et d'afficher le tout sur le moniteur série, de l'IDE
                  Arduino. Inclus : système d'antirebond logiciel, pour le couple de signaux CLK/DT.
  
  Remarques :     - l'arduino utilisé ici sera un modèle Nano
                  - un système "d'anti-rebond logiciel" sera mis en oeuvre ici, pour filtrer au maximum
                    les signaux indésirables
                                    
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       13.05.2023

*/

// Constantes
#define pinArduinoRaccordementSignalSW  2       // La pin D2 de l'Arduino recevra la ligne SW du module KY-040
#define pinArduinoRaccordementSignalCLK 3       // La pin D3 de l'Arduino recevra la ligne CLK du module KY-040
#define pinArduinoRaccordementSignalDT  4       // La pin D4 de l'Arduino recevra la ligne DT du module KY-040

// Variables
int etatPrecedentLigneSW;           // Cette variable nous permettra de stocker le dernier état de la ligne SW, afin de le comparer à l'actuel
int etatPrecedentLigneCLK;          // Cette variable nous permettra de stocker le dernier état de la ligne CLK, afin de le comparer à l'actuel
int etatPrecedentLigneDT;           // Cette variable nous permettra de stocker le dernier état de la ligne DT, afin de le comparer à l'actuel

int compteur = 0;                   // Cette variable nous permettra de compter combien de crans ont été parcourus, sur l'encodeur
                                    // (sachant que nous compterons dans le sens horaire, et décompterons dans le sens antihoraire)

// ========================
// Initialisation programme
// ========================
void setup() {

    // Initialisation de la liaison série (arduino nano <-> PC)
    Serial.begin(9600);
    Serial.println(F("=========================================================================="));
    Serial.println(F("Exemple 3  (programme de test Arduino Nano <-> module KY-040, utilisant les"));
    Serial.println(F("           interruptions matérielles arduino, avec système antirebond logiciel"));
    Serial.println(F("           permettant de filtrer au maximum les signaux non désirés, puis"));
    Serial.println(F("           affichage du nombre de crans parcourus et le sens de rotation)"));
    Serial.println(F("========================================================================="));
    Serial.println("");

    // Configuration de certaines pins de notre Arduino Nano en "entrées" (celles qui recevront les signaux du KY-040)
    pinMode(pinArduinoRaccordementSignalSW, INPUT);
    pinMode(pinArduinoRaccordementSignalCLK, INPUT);
    pinMode(pinArduinoRaccordementSignalDT, INPUT);

    // Petite pause pour laisser le temps aux signaux de se stabiliser
    delay(200);

    // Mémorisation des valeurs initiales des lignes SW/CLK/DT, au démarrage du programme
    etatPrecedentLigneSW  = digitalRead(pinArduinoRaccordementSignalSW);
    etatPrecedentLigneCLK = digitalRead(pinArduinoRaccordementSignalCLK);
    etatPrecedentLigneDT  = digitalRead(pinArduinoRaccordementSignalDT);

    // Affichage de la valeur initiale du compteur, sur le moniteur série
    Serial.print(F("Valeur initiale du compteur = "));
    Serial.println(compteur);

    // Activation d'interruptions sur les lignes CLK et DT
    attachInterrupt(digitalPinToInterrupt(pinArduinoRaccordementSignalCLK), changementSurLigneCLK, CHANGE);   // CHANGE => détecte tout changement d'état
    attachInterrupt(digitalPinToInterrupt(pinArduinoRaccordementSignalSW), changementSurLigneSW, CHANGE);     // CHANGE => détecte tout changement d'état

}


// =================
// Boucle principale
// =================
void loop() {

    // Boucle infinie ici, qui sera interrompue à chaque nouvelle interruption, et reprise juste après la fin d'exécution de la routine d'interruption

}


// ==============================================
// Routine d'interruption : changementSurLigneCLK
// ==============================================
void changementSurLigneCLK() {

    // Lecture des lignes CLK et DT, issue du KY-040, arrivant sur l'arduino
    int etatActuelDeLaLigneCLK = digitalRead(pinArduinoRaccordementSignalCLK);
    int etatActuelDeLaLigneDT  = digitalRead(pinArduinoRaccordementSignalDT);

    // ************************
    // * CAS : Incrémentation *
    // ************************
    // Si CLK = 1 et DT = 0, et que l'ancienCLK = 0 et ancienDT = 1, alors le bouton a été tourné d'un cran vers la droite (sens horaire, donc incrémentation)
    if((etatActuelDeLaLigneCLK == HIGH) && (etatActuelDeLaLigneDT == LOW) && (etatPrecedentLigneCLK == LOW) && (etatPrecedentLigneDT == HIGH)) {

        // Alors on incrémente le compteur
        compteur++;

        // Et on affiche ces infos sur le moniteur série
        Serial.print(F("Sens = horaire | Valeur du compteur = "));
        Serial.println(compteur);
        
    }

    // ************************
    // * CAS : Décrémentation *
    // ************************
    // Si CLK = 1 et DT = 1, et que l'ancienCLK = 0 et ancienDT = 0, alors le bouton a été tourné d'un cran vers la gauche (sens antihoraire, donc décrémentation)
    if((etatActuelDeLaLigneCLK == HIGH) && (etatActuelDeLaLigneDT == HIGH) && (etatPrecedentLigneCLK == LOW) && (etatPrecedentLigneDT == LOW)) {

        // Alors on décrémente le compteur
        compteur--;

        // Et on affiche ces infos sur le moniteur série
        Serial.print(F("Sens = antihoraire | Valeur du compteur = "));
        Serial.println(compteur);
        
    }

    // Et on mémorise ces états actuels dans étant "les nouveaux anciens", pour le "tour suivant" !
    etatPrecedentLigneCLK = etatActuelDeLaLigneCLK;
    etatPrecedentLigneDT = etatActuelDeLaLigneDT;
    
}


// =============================================
// Routine d'interruption : changementSurLigneSW
// =============================================
void changementSurLigneSW() {
  
    // On lit le nouvel état de la ligne SW
    int etatActuelDeLaLigneSW = digitalRead(pinArduinoRaccordementSignalSW);

    // On affiche le nouvel état de SW sur le moniteur série de l'IDE Arduino
    if(etatActuelDeLaLigneSW == LOW)
        Serial.println(F("Bouton SW appuyé"));
    else
        Serial.println(F("Bouton SW relâché"));

    // Puis on mémorise le nouvel état de la ligne SW
    etatPrecedentLigneSW = etatActuelDeLaLigneSW;

}
