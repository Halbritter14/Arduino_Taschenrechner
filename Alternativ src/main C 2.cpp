#include <Arduino.h>

String inputString = "";    // Ein String zum Speichern der eingehenden Daten
bool stringComplete = false;  // Wird wahr, wenn der gesamte String empfangen wurde

void setup() {
    Serial.begin(9600); // Starte die serielle Kommunikation mit 9600 Baud
    inputString.reserve(200); // Reserviert Speicher für den Eingabestring
}

void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read(); // Lese das nächste Zeichen
        inputString += inChar;
        if (inChar == '\n') { // Überprüfe auf das Ende der Zeile
            stringComplete = true;
        }
    }
}

int calculate(String data) {
    int firstNumber = 0, secondNumber = 0;
    char operation = 0;

    int spaceIndex = data.indexOf(' ');
    if (spaceIndex == -1) return 0; // Kein Operator gefunden

    firstNumber = data.substring(0, spaceIndex).toInt();
    int secondSpaceIndex = data.indexOf(' ', spaceIndex + 1);
    if (secondSpaceIndex == -1) return 0; // Kein zweiter Operator gefunden

    operation = data.charAt(spaceIndex + 1);
    secondNumber = data.substring(secondSpaceIndex + 1).toInt();

    switch (operation) {
        case '+': return firstNumber + secondNumber;
        case '-': return firstNumber - secondNumber;
        case '*': return firstNumber * secondNumber;
        case '/':
            if (secondNumber == 0) {
                Serial.println("Error: Division by zero");
                return 0;
            }
            return firstNumber / secondNumber;
        default:
            Serial.println("Error: Invalid operation");
            return 0;
    }
}

void loop() {
    if (stringComplete) {
        int result = calculate(inputString); // Führe die Berechnung aus
        Serial.println(result); // Sende das Ergebnis zurück an den PC
        inputString = ""; // Bereite den String für die nächste Eingabe vor
        stringComplete = false; // Setze den Zustand zurück, dass der String noch nicht komplett ist
    }
}
