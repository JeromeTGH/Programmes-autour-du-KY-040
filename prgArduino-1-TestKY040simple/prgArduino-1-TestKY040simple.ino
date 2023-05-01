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



// ========================
// Initialisation programme
// ========================
void setup() {

 
}


// =================
// Boucle principale
// =================
void loop() {


}