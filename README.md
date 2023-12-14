# TATransmitter
lianisier Analog Digitalwandlerwert und die die Werte über einen
ServerSocket heraus.
# Inbetriebname
Beim Ersten einschalten wird ein W-Lan mit der ssid TATransmitter erzeugt.
Verbindet man sich mit dem W Lan TATransmitter exclusive wird im Browswer
ein KonfigurationDialg geöffnet.
Siehe Doku WiFiSettings. 
In dem Dialog wird der Zugriff auf das W-Lan 
, Serverport des TATransmitter und der Servername(default dose1) konfigurit.
# C++ Klassen
class ADValToTA:
Übersetzt den Wert des AD Wandler in einen TA mit Hilfe einer Tabelle.

class: ADHandler
Ist das Interface zum AD1115 und gibt den AD Wert als Integer Wert raus.
# Client
Der Client muss nach den Verbinden "Start" in den Socket schreiben.
Dann beginnt der Server (TATransmitter) die Übertragung.
# ServerSocket
Der Server unterstüzt nur einen Clienten. Der Messwert wird als String 
übermittelt, Format "x.xx".
Es werde ca. 30 Werte pro Sekunde ausgegeben.

# Verwendete Libs
WiFi
WiFiSettings
Adafruit_ADS1115


Have fun ;-)

HtH