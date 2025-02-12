IoT Projekt: Pametni klima uređaj

 📌 Opis projekta
Ovaj projekt omogućava praćenje trenutne temperature prostorije
pomoću senzora TMP36 na osnovu kojeg se ukoliko je temperatura ispod optimalne pali buzzer
i žuta dioda. Nakon što temperatura dostigne optimalno stanje gase se ove dvije komponente
i pali se zelena dioda. Ukoliko temperatura poraste iznad optimalne, pali se crvena lampica
i motor koji rashlađuje.

🛠 Korištene tehnologije
- **Arduino** ( ESP8266)
- **Senzori** (TMP36)
- **HTML, CSS, JavaScript** (za web interfejs)

 📂 Struktura projekta

IoT-Project/
│── firmware/           # Arduino kod (.ino fajl i biblioteke)
│── web/                # HTML, CSS, JavaScript fajlovi
│── images/             # Slike projekta
│── README.md           # Opis projekta


 🔧 Instalacija i pokretanje
1️. Hardverska podešavanja
a) Poveži senzor, buzzer,motor,diode i otpornike te mikrokontroler sa Arduino pločom.
b) Uvjeriti se da je Arduino IDE instaliran.

2️. Softverska podešavanja
a) Instalirati potrebne biblioteke u Arduino IDE (Arduino.h, ESP8266WiFi.h, Firebase_ESP_Client.h).
b) Otvori `firmware/main.ino` i učitaj kod na Arduino ploču.Napomena: Dodati svoj WiFi.
c) Pokreni web aplikaciju za očitavanje podataka temperature na istu.

 📩 Kontakt
Ako imate bilo kakvih pitanja, slobodno me kontaktirajte putem mog GitHub profila.

