# WiFi_IR

## Obiettivi
L'obiettivo del progetto è quello di sostituire un normale telecomando TV a infrarossi con un piccolo dispositivo che gestisca l'emissione dei treni di impulsi IR, grazie a comandi ricevuti attraverso una semplice interfaccia web. Pre-requisiti sono:
* l'uso di un piccolo controllore ESP8266, nello specifico un ESP-12F che offre 4 MB di memoria totale e alcune porte in più rispetto al ESP-01S, che pure dispone di un solo MB di memoria.
* l'accesso web offerto tramite la libreria ESP8266WebServer (https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
* l'utilizzo di tecniche di comunicazione con il client web tramite delle semplici chiamate di tipo webservice, delegando alla interfaccia sviluppata in HTML, tutta la complessità di interpretazione del comportamento utente
* la possibilità di poter gestire tale prestazione oltre che attraverso la selezione di bottoni, anche tramite semplici comandi vocali.

## Configurazione hardware
Data la presenza di possibili interferenze in media frequenza (dell'ordine dei MHz) e a valle di numerose prove di compatibilità, si è ritenuto necessario separare fisicamente il modulo di gestione del LED a InfraRossi con quello di governo e interfaccia del microcontrollore, in modo eventualmente da poter utilizzare diversi modelli di ESP8266.
Gli schemi di collegamento quindi sono almeno due: uno propriamente di "potenza", per la gestione diretta del LED IR, e uno di alimentazione e interfacciamento del microcontrollore e in particolare quello specifico per il ESP-12F.

![schema elettrico cricuito di potenza](https://github.com/robertopapi/ESP8266-01s/blob/fcc14a7b672705b6838940ba3d4204f068edf33b/WiFi_IR/20260930%20IR%20POWER%20RED.png)


## Software
Lato software, ho distribuito il codice su più file e separando inoltre i file destinati al browser web che sono caricati direttamente tramite il web server.
Il codice HTML è suddiviso su 4 variabili constant static char per comodità.

# Funzionamento
Collegati i componenti e alimentato i circuiti, una volta che il browser web client interrroga il server.

## Conclusioni

 



