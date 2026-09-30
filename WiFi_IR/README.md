# WiFi_IR

## Obiettivi
L'obiettivo del progetto è quello di sostituire un normale telecomando TV a infrarossi con un piccolo dispositivo che gestisca l'emissione dei treni di impulsi IR, grazie a comandi ricevuti attraverso una semplice interfaccia web. Pre-requisiti sono:
* l'uso di un piccolo controllore ESP8266, nello specifico un ESP-12F che offre 4 MB di memoria totale e alcune porte in più rispetto al ESP-01S, che pure dispone di un solo MB di memoria.
* l'accesso web offerto tramite la libreria ESP8266WebServer (https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
* l'utilizzo di tecniche di comunicazione con il client web tramite delle semplici chiamate di tipo webservice, delegando alla interfaccia sviluppata in HTML, tutta la complessità di interpretazione del comportamento utente
* la possibilità di poter gestire tale prestazione oltre che attraverso la selezione di bottoni, anche tramite semplici comandi vocali.

## Configurazione hardware
Data la presenza di possibili interferenze in media frequenza (dell'ordine dei MHz) e a valle di numerose prove di compatibilità, si è ritenuto necessario separare fisicamente il modulo di gestione del LED a InfraRossi con quello di governo e interfaccia del microcontrollore, in modo eventualmente di poter utilizzare diversi modelli di ESP8266.
Gli schemi di collegamento quindi sono almeno due: uno propriamente di "potenza", per la gestione diretta del LED IR, e uno di alimentazione e interfacciamento del microcontrollore e in particolare quello specifico per il ESP-12F.

![schema elettrico di test](https://github.com/robertopapi/ESP8266-01s/blob/91a00783bc74dba958e12a1422c67fc99552a9cc/integration-hw-and-web-interfaces-with-PCF8574/test2.png)


## Software
Lato software, ho distribuito il codice su due file, uno con il solo codice HTML e l'altro contenente lo sketch vero e proprio.
Il codice HTML è suddiviso su 4 variabili constant static char per comodità.
Il codice HTML è stato scritto in modo da sfruttare la modalità offerta dalla tecnica AJAX, perché i contenuti della pagina web siano aggiornati senza dover rieseguire il download dell'intera pagina. Allo stesso modo, sfruttando le nuove prestazioni date dalla possibilità di gestire il flusso dati tra scheda e client web tramite il flusso di tipo text/event-stream.
Per una maggiore interpretazione dei risultati della elaborazione, lato client web è stata predisposta più di una linea di DEBUG che è mostrata nel grande text box al centro.

# Funzionamento
Collegati i componenti e alimentato i circuiti, una volta che il browser web client interrroga il server, oltre a visualizzare la corrispondente pagina, è possibile vedere che il flusso dati da server verso client si attiva automaticamente, con un invio ogni 5 secondi circa.

## Conclusioni
L'impressione è che la cadenza di ricezione dei dati ogni 5 secondi, dipenda dal browser web che richiede i dati al server con quella frequenza, piuttosto che dipendere da qualche iniziativa del server. Tuttavia non posso escludere che abbia perso qualche passaggio nella procedura.

In conclusione, è possibile interagire con le singole porte del PCF8574 via web purché ci si accontenti (per ora) di non avere feedback in tempo reale.
 



