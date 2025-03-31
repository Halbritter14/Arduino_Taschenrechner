#include <Arduino.h>

// Variablen zur Speicherung der eingehenden Daten
String inputString = "";    // Ein String, um die eingehenden Daten zu speichern
bool stringComplete = false;  // Wahr, wenn der komplette String empfangen wurde

void setup() {
    Serial.begin(9600); // Starte die serielle Kommunikation mit 9600 Baud
    inputString.reserve(200); // Reserviert Speicher für den Eingabestring
}

// Diese Funktion wird aufgerufen, wenn neue Daten verfügbar sind
void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read(); // Liest das nächste verfügbare Zeichen
        inputString += inChar;
        if (inChar == '\n') { // Prüft auf das Ende der Zeile (Eingabeende)
            stringComplete = true;
        }
    }
}

// Funktion zur Berechnung der Rechenaufgabe
int calculate(String data) {
    int firstNumber = 0, secondNumber = 0; // Variablen für die Zahlen
    char operation = 0; // Variable für den Operator

    // Einfache Parsing-Logik für "number1 operation number2"
    int spaceIndex = data.indexOf(' '); // Findet das Leerzeichen (erster Operator)
    if (spaceIndex == -1) return 0; // Kein Operator gefunden

    // Extrahiert die erste Zahl
    firstNumber = data.substring(0, spaceIndex).toInt();

    // Findet das zweite Leerzeichen (nach dem Operator)
    int secondSpaceIndex = data.indexOf(' ', spaceIndex + 1);
    if (secondSpaceIndex == -1) return 0; // Kein zweiter Operator

    // Extrahiert den Operator
    operation = data.charAt(spaceIndex + 1);

    // Extrahiert die zweite Zahl
    secondNumber = data.substring(secondSpaceIndex + 1).toInt();

    // Berechnung durchführen basierend auf dem Operator
    switch (operation) {
        case '+': return firstNumber + secondNumber;
        case '-': return firstNumber - secondNumber;
        case '*': return firstNumber * secondNumber;
        case '/': 
            if (secondNumber == 0) {
                Serial.println("Error: Division by zero");
                return 0; // Fehlerbehandlung für Division durch Null
            }
            return firstNumber / secondNumber;
        default:
            Serial.println("Error: Invalid operation");
            return 0; // Fehlerbehandlung für ungültige Operation
    }
}

void loop() {
    if (stringComplete) {
        int result = calculate(inputString); // Führt die Berechnung durch
        Serial.println(result); // Sendet das Ergebnis zurück an den PC
        inputString = ""; // Bereitet den String für die nächste Eingabe vor
        stringComplete = false; // Setzt den Zustand zurück, dass der String noch nicht komplett ist
    }
}
