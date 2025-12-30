#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <LiquidCrystal.h>

// Initialisation du LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Définition des broches et seuils
const int PinPTC = 7; // Broche pour le relais
const float SEUIL_TEMP = 29.0; // Seuil de température en °C pour le mois d'avril
const float SEUIL_HUMIDITE = 100.0; // Seuil d'humidité en % valur au pif  sans recherche a modif 

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

// Variables globales
int buttonState;
int prevButtonState = -1;
int selectedOption = 0; 
const int numOptions = 4;
String options[] = {"Grosse pluie", "Rapide", "Normal", "Calcul"};
bool arreter = false; // Indique si le programme doit s'arrêter
int tempsSechage = 30; // Temps par défaut pour ProgrammeCalcul
bool tempsSelection = false; // Indique si on est en train de choisir le temps

void setup() {
  pinMode(PinPTC, OUTPUT);
  digitalWrite(PinPTC, LOW); // Désactiver le chauffage au démarrage
  lcd.begin(16, 2);

  if (!htu.begin()) {
    lcd.clear();
    lcd.print("Capteur Erreur!");
    while (1); // Bloquer si le capteur ne fonctionne pas
  }

  lcd.clear();
  lcd.print("Systeme Pret");
  delay(2000);
  displayOption(); // Afficher le menu initial
}

void loop() {
  buttonState = read_LCD_buttons();

  if (buttonState != prevButtonState) {
    if (!tempsSelection) { // Si on n'est pas en mode sélection du temps
      if (buttonState == 4) { // UP pour naviguer dans le menu
        selectedOption = (selectedOption - 1 + numOptions) % numOptions;
        displayOption();
      } else if (buttonState == 3) { // DOWN pour naviguer dans le menu
        selectedOption = (selectedOption + 1) % numOptions;
        displayOption();
      } else if (buttonState == 5) { // RIGHT pour valider le programme
        if (selectedOption == 3) { // Si "Calcul" est sélectionné
          tempsSelection = true; // Passer en mode sélection du temps
          displayTempsSelection();
        } else {
          executeOption();
        }
      }
    } else { // Si on est en mode sélection du temps
      if (buttonState == 4) { // UP pour augmenter le temps
        tempsSechage++;
        displayTempsSelection();
      } else if (buttonState == 3) { // DOWN pour diminuer le temps
        if (tempsSechage > 1) tempsSechage--;
        displayTempsSelection();
      } else if (buttonState == 5) { // RIGHT pour valider le temps
        tempsSelection = false; // Quitter le mode sélection du temps
        executeOption();
      }
    }
    prevButtonState = buttonState;
  }
}

// Fonction pour afficher l'option actuellement sélectionnée
void displayOption() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("> " + options[selectedOption]);
}

// Fonction pour afficher le mode sélection du temps
void displayTempsSelection() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Choisir temps:");
  lcd.setCursor(0, 1);
  lcd.print(tempsSechage);
  lcd.print(" min");
}

// Fonction pour exécuter l'option sélectionnée
void executeOption() {
  lcd.clear();
  lcd.print("Lancement...");
  lcd.setCursor(0, 1);
  lcd.print(options[selectedOption]);
  delay(2000);

  if (selectedOption == 0) {
    GrossePluie();
  } else if (selectedOption == 1) {
    ProgrammeRapide();
  } else if (selectedOption == 2) {
    ProgrammeNormal();
  } else if (selectedOption == 3) {
    ProgrammeCalcul(tempsSechage);
  }

  displayOption(); // Retour au menu
}

// Programme Grosse Pluie
void GrossePluie() {
  lcd.clear();
  lcd.print("Pluie: ON");
  delay(2000);

  while (htu.readHumidity() > SEUIL_HUMIDITE) {
    digitalWrite(PinPTC, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(htu.readHumidity(), 1);
    delay(1000);
  }
  digitalWrite(PinPTC, LOW);
  lcd.clear();
  lcd.print("Pluie termine");
  delay(2000);
}

// Programme Rapide
void ProgrammeRapide() {
  lcd.clear();
  lcd.print("Rapide: ON");
  digitalWrite(PinPTC, HIGH);
  delay(1800000); // 30 minutes
  digitalWrite(PinPTC, LOW);
  lcd.clear();
  lcd.print("Rapide termine");
  delay(2000);
}

// Programme Normal
void ProgrammeNormal() {
  lcd.clear();
  lcd.print("Normal: ON");
  delay(2000);

  while (htu.readTemperature() < SEUIL_TEMP) {
    digitalWrite(PinPTC, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(htu.readTemperature(), 1);
    delay(1000);
  }
  digitalWrite(PinPTC, LOW);
  lcd.clear();
  lcd.print("Normal termine");
  delay(2000);
}

// Programme Calculé
void ProgrammeCalcul(int tempsSechage) {
  lcd.clear();
  lcd.print("Temps: ");
  lcd.print(tempsSechage);
  lcd.print(" min");
  delay(2000);

  for (int i = 0; i < tempsSechage && !arreter; i++) {
    verifierStop(); // Vérifie si le programme doit s'arrêter
    if (arreter) break;

    digitalWrite(PinPTC, HIGH);
    delay(60000); // 1 minute
    lcd.setCursor(0, 0);
    lcd.print("Sechage en cours");
    lcd.setCursor(0, 1);
    lcd.print("Reste: ");
    lcd.print(tempsSechage - i - 1);
    lcd.print(" min");
  }

  digitalWrite(PinPTC, LOW);
  lcd.clear();
  if (arreter) {
    lcd.print("Programme stop");
  } else {
    lcd.print("Sechage termine");
  }
  delay(2000);
}

// Fonction pour vérifier si le programme doit être arrêté
void verifierStop() {
  buttonState = read_LCD_buttons();
  if (buttonState == 2) { // Bouton LEFT pour arrêter
    arreter = true;
  }
}

// Lecture des boutons du LCD Keypad Shield
int read_LCD_buttons() {
  int adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return 0; // Aucun bouton
  if (adc_key_in < 50)   return 1; // SELECT
  if (adc_key_in < 195)  return 2; // LEFT
  if (adc_key_in < 380)  return 3; // DOWN
  if (adc_key_in < 555)  return 4; // UP
  if (adc_key_in < 790)  return 5; // RIGHT
  return 0;
}