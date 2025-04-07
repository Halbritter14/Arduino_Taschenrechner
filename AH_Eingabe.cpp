// AH_Eingabe.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <windows.h>
#include <string>
#include <cctype>  // Für std::isdigit und std::isspace

HANDLE serialHandle;

void openSerialPort() {
    serialHandle = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (serialHandle == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port\n";
        return;
    }

    // Konfiguriere den seriellen Port
    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);
    GetCommState(serialHandle, &serialParams);
    serialParams.BaudRate = CBR_9600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;
    SetCommState(serialHandle, &serialParams);

    // Setze Timeouts
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(serialHandle, &timeouts);
}

void sendData(const std::string& data) {
    DWORD bytesWritten;
    WriteFile(serialHandle, data.c_str(), static_cast<DWORD>(data.size()), &bytesWritten, NULL);
}

std::string readData() {
    DWORD bytesRead;
    char buffer[1024];
    ReadFile(serialHandle, buffer, sizeof(buffer), &bytesRead, NULL);
    return std::string(buffer, bytesRead);
}

bool isValidCalculation(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c) && !std::isspace(c) && c != '+' && c != '-' && c != '*' && c != '/') {
            return false;
        }
    }
    return true;
}

bool containsSpaces(const std::string& str) {
    return str.find(' ') != std::string::npos;
}

int main() {
    openSerialPort();
    std::string input;
    while (true) {
        std::cout << "Geben Sie eine Berechnung ein: ";
        std::getline(std::cin, input);
        if (!isValidCalculation(input)) {
            std::cout << "Falsche Eingabe. Nur Zahlen und +, -, *, / sind erlaubt.\n";
            continue;
        }
        if (!containsSpaces(input)) {
            std::cout << "Bitte fuegen Sie ein Leerzeichen zwischen den Zahlen und Operatoren ein.\n";
            continue;
        }
        sendData(input + "\n");  // Sendet die Eingabe an den Arduino
        std::string result = readData();  // Liest das Ergebnis vom Arduino
        std::cout << "Ergebnis: " << result << std::endl;
    }
}


// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
