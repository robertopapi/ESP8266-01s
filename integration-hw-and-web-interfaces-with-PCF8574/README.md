# Integration hw and web interfaces with PCF8574

## Obiettivi
L'obiettivo del progetto è quello di testare l'integrazione dei componenti hardware e software, rimanendo tuttavia fedele ad alcune scelte tecniche quali:
* l'uso del ESP8266-01s, nonostante il limitato numero di PIN/porte utili
* l'uso di un modulo con PCF8574 interafacciato grazie alla libreria dedicata (https://github.com/xreef/PCF8574_library)
* il collegamento al WiFi tramite la libreria WiFiManager (https://github.com/tzapu/WiFiManager)
* l'accesso web offerto tramite la libreria ESP8266WebServer (https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer)
* l'utilizzo di tecniche di comunicazione con il client web tramite AJAX e JSON, in particolare attraverso un flusso di tipo text/event-stream
* il funzionamento atteso è quello di poter gestire l'accensione dei LED tramite una piccola interfaccia web (flusso da client web verso l'ESP8266) con feedback da parte della scheda circa lo stato di accensione (flusso da ESP8266 verso client web). Allo stesso modo ma a parti invertite, risentire della pressione sui bottoni e riproduzione del loro stato sulla interfaccia web.

## Configurazione hardware
Lo schema di collegamento è quello rappresentato in figura e prevede sia delle porte del PCF8574 usate come input, collegate a dei bottoni, e altre usate come output, collegate a dei LED e al buzzer.

![schema elettrico di test](https://github.com/robertopapi/ESP8266-01s/blob/91a00783bc74dba958e12a1422c67fc99552a9cc/integration-hw-and-web-interfaces-with-PCF8574/test2.png)

Nel dettaglio, i bottoni sono collegati con una resistenza al positivo, per mantenere la porta del PCF8574 con il valore alto (1). Premendo il bottone si Chiude il circuito che lo collega alla massa, ponendo la porta corrispondente del PCF8574 al valore basso (0).
I LED invece sono collegati con l'anodo al positivo tramite una resistenza mentre il catodo è collegato con la porta del PCF8574.
Il buzzer attivo è collegato direttamente sul positivo mentre al negativo è collegato verso massa tramite un transistor NPN. La base del transistor è infine colllegato alla porta del PCF8574 tramite una resistenza.
Il numero limitato delle porte disponibili sulla scheda ESP8266 ci obbliga ad utilizzare il modulo PCF8574 per "demultiplexare" il dato seriale che viaggia attraverso il bus I2C gestito dalla scheda ESP8266 tramite due sole porte. Le porte corrispondenti sul ESP8266 sono:

* SDA = GPIO0 = 0
* SCL = GPIO2 = 2

Per intercettare il cambio di stato di una delle porte gestite dal PCF8574, utilizziamo l'apposita porta "INT" presente sul PCF8574 collegata alla terza porta GPIO disponibile sul ESP8266. Così facendo tuttavia si sacrifica la prestazione relativa alla porta RX della seriale che tuttavia in questo caso è trascurabile.

* INT = GPIO3 = RXD = 3

## Software
Lato software, ho distribuito il codice su due file, uno con il solo codice HTML e l'altro contenente lo sketch vero e proprio.
Il codice HTML è suddiviso su 4 variabili constant static char per comodità.
Il codice HTML è stato scritto in modo da sfruttare la modalità offerta dalla tecnica AJAX, perché i contenuti della pagina web siano aggiornati senza dover rieseguire il download dell'intera pagina. Allo stesso modo, sfruttando le nuove prestazioni date dalla possibilità di gestire il flusso dati tra scheda e client web tramite il flusso di tipo text/event-stream.
Per una maggiore interpretazione dei risultati della elaborazione, lato client web è stata predisposta più di una linea di DEBUG che è mostrata nel grande text box al centro.

![interfaccia web](https://github.com/robertopapi/ESP8266-01s/integration-hw-and-web-interfaces-with-PCF8574/interfaccia.png)

nel dettaglio, i simboli che rappresentano i LED sono cliccabili e diventano verdi se il LED hardware risulta effettivamente acceso. Stessa cosa per i bottoni che diventano grigio scuro se corrispondentemente il bottone fisico è premuto.

Lato sketch, la parte relativa alla inizializzazione dei servizi prevede in primo luogo la definizione della variabile "keyPressed" impostata inzialmente a false e che cambia di stato in true, quando una delle porte del PCF8574 cambia di stato.

A questo proposito deve essere pre dichiarata la funzione di interrupt che gestirà tale cambio di stato, attraverso l'istruzione

`void ICACHE_RAM_ATTR keyPressedOnPCF8574();`

la direttiva ICACHE_RAM_ATTR è indispensabile dato che la funzione deve essere richiamata dalla RAM. A quanto sembra tale impostazione non è opzionale.
Subito sotto si trova l'inizializzazione dell'oggetto pcf8574 tramite

`PCF8574 pcf8574(0x20, 0, 2, 3, keyPressedOnPCF8574);`

Con tale istruzione si danno indicazioni circa rispettivamente

* l'indirizzo della scheda PCF8574 (0x20),
* la porta SDA del BUS I2C (0),
* la porta SCL del BUS I2C (2),
* la porta INT (3),
* la funzione che deve gestire l'interrupt (keyPressedOnPCF8574)

di seguito l'impostazione di alcune variabili utili per la gestione sia locale che della pagina web.

Per la gestione dei diversi canali di interazione tra client web e scheda ESP8266, che tecnicamente fa da web server, sono state create tre URL diverse:
* una URL principale di root, attraverso la quale il client scarica la pagina web
* una URL per l'invio dei dati dal client (il browser web) verso il server (la nostra ESP8266), che corrisponde alla URL "/datasave"
* una URL per il flusso opposto cioè dal server verso il client e che corrisponde alla URL "/updatedata"

Per la gestione delle singole URL sono state definite delle funzioni specifiche, una per ciascuna URL attivata.

La principale è quella chiamata "handleRoot()" che di fatto contiene la pubblicazione del contenuto del file con il codice HTML.

La funzione "handleUpdateData()" è quella nella quale è definito il flusso text/event-stream che ha una sua precisa sintassi riassunta nei commenti descritti nel codice. Tale funzione, oltre che dichiarata come handler della URL corrispondente, è pure chiamata tutte le volte che avviene un aggiornamento dello stato delle porte del PCF8574, date ad esempio dalla pressione di uno dei bottoni.

La funzione "handleDataSave()" invece serve per gestire il flusso opposto cioè da client verso server, che avviene tramite lo scambio di una stringa JSON costituita da un campo per ciascun elemento cliccabile sulla interfaccia web e a cui corrispondono i singoli comandi di attivazione delle porte hardware.
Di seguito sono state definite anche due funzioni specifiche per la gestione dei bottoni e dei LED.

Nella funzione setup() ci sono le dichiarazioni relative al bus I2C, l'inizializzazione delle porte del PCF8574 e per finire le dichiarazioni relative al server WEB.

Nella funzione loop(), è definito il codice per gestire l'interrupt, dato dal cambio di stato della variabile "keyPressed". In tale parte di codice, oltre a richiamare il gestore dei bottoni (che al momento in effetti non fa nulla), è richiamata anche la funzione per la gestione del flusso verso il client web.

Come nota a margine, alla fine della funzione loop() è stato inserito un delay di 100 millisecondi che "sembra" necessario per permettere l'invio dei dati HTML verso il client: senza di esso personalmente ho avuto problemi con la pubblicazione della pagina web, tanto da mettere in dubbio (sbagliando) il buon funzionamento dell'hardware.

## Funzionamento
Collegati i componenti e alimentato i circuiti, una volta che il browser web client interrroga il server, oltre a visualizzare la corrispondente pagina, è possibile vedere che il flusso dati da server verso client si attiva automaticamente, con un invio ogni 5 secondi circa.

Se a questo punto si interagisce con l'interfaccia web, ad esempio cliccando su uno dei simboli che rappresentano i LED, praticamente all'istante si accende il corrispondente LED attestato sulla porta del PCF8574.

Purtroppo a questo punto si manifesta un difetto che non riesco a risolvere, cioè il feedback da server verso client relativo al cambio di stato della porta che in pratica rimane sincronizzato con il flusso aggiornato perodicamente cioè ogni 5 secondi.

L'effetto è che, cliccando sul simbolo del LED sull'interfaccia WEB, il LED fisico si accende immediatamente ma la notifica dell'avvenuta accensione avviene con un ritardo di circa 5 secondi.

Stessa cosa a parti invertite: se si preme uno dei bottoni, la notifica verso il client web avviene solo se il bottone è ancora premuto quando parte l'aggiornamento verso il client, cioè sempre ogni 5 secondi. In pratica la pressione sul bottone è rilevata sulla interfaccia web se e solo se il bottone resta premuto durante l'aggiornamento successivo o per un tempo maggiore a 5 secondi.

## Conclusioni
L'impressione è che la cadenza di ricezione dei dati ogni 5 secondi, dipenda dal browser web che richiede i dati al server con quella frequenza, piuttosto che dipendere da qualche iniziativa del server. Tuttavia non posso escludere che abbia perso qualche passaggio nella procedura.

In conclusione, è possibile interagire con le singole porte del PCF8574 via web purché ci si accontenti (per ora) di non avere feedback in tempo reale.
 


