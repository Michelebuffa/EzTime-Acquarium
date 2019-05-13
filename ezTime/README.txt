<sup> Vai a: & nbsp; & nbsp; & nbsp; [Sommario] (# tabella-dei-contenuti) & nbsp; & nbsp; & mdash; & nbsp; & nbsp; [Riferimento funzione] (# riferimento-funzione) & nbsp; & nbsp; & mdash ; & nbsp; & nbsp; [dataTime function] (# datetime) </ sup>

# ezTime, una libreria di Arduino per tutto il tempo & nbsp; <sup> * </ sup>

& Nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; <Sup> scritto & nbsp; & nbsp; & nbsp Rop; Gonggrijp </ sup>

** ezTime & mdash; pronunciato "Easy Time" & mdash; è una libreria di data e ora Arduino molto semplice da usare che fornisce ricerche temporali in rete NTP, ampio supporto fuso orario, stringhe di data e ora formattate, eventi utente, precisione millisecondo e altro. **

& Nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; <Sup> * & nbsp; limitazioni & nbsp; May & nbsp; applicare, & nbsp; vedere & nbsp; "[2036 & nbsp; e & nbsp; 2038] (# 2036-e-2038)" & nbsp; capitolo </ sup>

! [] (Immagini / movimento-clock.gif)

& Nbsp;

> ** Newsflash **: * Per trovare le informazioni sul fuso orario, ezTime utilizzava inizialmente timezoneapi.io, che poteva essere utilizzato gratuitamente per un massimo di 50 query al giorno. Hanno cambiato questa politica e hanno forzato l'uso di https, rompendo entrambi ezTime. (Per maggiori informazioni vedi [qui] (https://github.com/ropg/ezTime/issues/2).) A partire dalla versione 0.7.4, ezTime fa uso del proprio daemon di ricerca timezone online, rimuovendo una dipendenza da alcune terze parti che potrebbero cambiare la loro politica proprio come ha fatto timezoneapi. Si prega di vedere i dettagli per [* `setLocation` *] (# setlocation) perché l'interfaccia è cambiata leggermente. Ora puoi anche effettuare ricerche GeoIP per l'impostazione dell'ora locale automatica (solo in paesi che non si estendono su più fusi orari). *

## Una breve storia di ezTime

Stavo lavorando su [M5ez] (https://github.com/ropg/M5ez), una libreria di interfaccia per creare facilmente programmi dall'aspetto interessante per l'hardware "[M5Stack] (http://m5stack.com/)" ESP32 . La barra di stato di M5ez aveva bisogno di visualizzare l'ora. Questo era tutto, lo giuro. Ho pensato che avrei usato [Time] (https://github.com/PaulStoffregen/Time), la libreria di Michael Margolis 'e Paul Stoffregen per fare cose del tempo su Arduino. Quindi ho dovuto sincronizzarlo con un server NTP, quindi ho pensato che avrei usato [NTPclient] (https://github.com/arduino-libraries/NTPClient), una delle librerie client NTP esistenti. E poi volevo che mostrasse l'ora locale, quindi avrei bisogno di un modo per l'utente di impostare un offset tra l'ora UTC e l'ora locale.

Fin qui tutto bene.

Poi mi sono ricordato di quanto sono irritato quando arriverà o verrà l'ora legale, dato che devo impostare manualmente alcuni dei miei orologi come il forno a microonde, l'orologio nel cruscotto dell'auto, ecc. È il 2018, il mio orologio dovrebbe sapere regole del fuso orario. Quindi potrei ottenere la libreria [Timezone library] di Jack Christensen (https://github.com/JChristensen/Timezone). Ma ha bisogno delle regole del fuso orario, come * "L'ora legale entra in vigore l'ultima domenica di marzo alle 02:00 ora locale" * raccontata. Ho pensato di ottenere semplicemente questi dati da internet e analizzarli.

Poi ho voluto visualizzare l'ora a 12 o 24 ore e ho pensato a vari formati per data e ora. Non sarebbe bello avere qualche funzione per stampare il tempo formattato come offrono molti linguaggi di programmazione?

Trascurando il campo di battaglia dopo aver implementato una parte di questo, sembrava che ci dovesse essere un modo migliore. Un modo in cui tutto questo lavoro andrebbe a vantaggio di più persone. Ecco come ezTime & mdash; il progetto che avrebbe richiesto solo pochi giorni Venuto per essere.

## ezTime è ...

** self-contained **: Dipende solo da altre librerie per andare online, ma poi non ha bisogno di altre librerie per la ricerca di dati NTP e timezone. (E anche il networking può essere disabilitato completamente se hai un'altra fonte per volta.)

** preciso **: a differenza di altre librerie, ezTime non getta via o manipola le seconde informazioni frazionarie dal server NTP. Una richiesta NTP a pool.ntp.org richiede solo 40ms di andata e ritorno su DSL di casa al giorno d'oggi, quindi è consigliabile aggiungere una precisione inferiore al secondo a una libreria di orari. ezTime legge i secondi frazionari e cerca di tenere conto della latenza della rete per darti un tempo preciso.

** compatibile all'indietro **: Qualsiasi cosa scritta per la libreria di tempo Arduino esistente funzionerà ancora. È possibile impostare il fuso orario in cui deve essere inserito lo schizzo o essere in UTC, che è l'impostazione predefinita. Ma puoi anche impostare ed esprimere il tempo facendo riferimento a più fusi orari, tutto molto facile e intuitivo.

** eventful **: è possibile impostare eventi in modo che ezTime esegua le proprie funzioni in un dato momento ed elimina di nuovo gli eventi se si cambia idea.

** robust **: non fallisce se l'ape del fuso orario scompare: può utilizzare i dati memorizzati nella cache, che ezTime può memorizzare in EEPROM (AVR Arduinos) o NVS (ad esempio ESP32 tramite la libreria Preferenze).

** informativo **: non è necessario indovinare mentre stai lavorando su qualcosa, ezTime può stampare messaggi sulla porta seriale al livello di dettaglio desiderato, indicando le informazioni sull'ora legale che riceve o quando riceve un aggiornamento NTP e da quanto il tuo orologio interno era spento, per esempio.

** risparmio di tempo **: non più tempo speso per scrivere codice per stampare la data o l'ora in un modo più carino. Stampa con facilità le "20:20" o "Sabato 23 agosto 2018". Previene lo sfarfallio dello schermo con funzioni `minuteChanged ()` e `secondChanged ()` senza memorizzare alcun valore da confrontare.

** abbastanza piccolo **: funziona con tutte le funzionalità e tutte le informazioni di debug su un vecchio Arduino Uno con uno scudo Ethernet, lasciando 2/3 di RAM e persino alcuni dei flash con cui lavorare. Varie opzioni di # define consentono di escludere parti della libreria se si desidera ridurla: si può anche escludere del tutto il networking se si dispone di una sorgente temporale differente.

** facile da usare **: non ci credo finché non lo vedi. Dai uno sguardo ad alcuni di questi esempi per vedere quanto è facile da usare.

& Nbsp;

### Fusi orari

(uno schizzo completo per mostrare quanto sia semplice)

`` `
#include <ezTime.h>
#include <WiFi.h>

void setup () {
Serial.begin (115200);
WiFi.begin ("your-ssid", "your-password");

waitForSync ();

Serial.println ("UTC:" + UTC.dateTime ());

Fuso orario Nuova Zelanda;
NewZealand.setLocation ( "Pacifico / Auckland");
Serial.println ("Ora della Nuova Zelanda:" + NewZealand.dateTime ());
}

void loop () {}
`` `

`` `
UTC: venerdì, 7-set-2018 11:25:10 UTC
Ora della Nuova Zelanda: venerdì, 7-set-2018 23:25:11 NZST
`` `

& Nbsp;

### Data e ora formattati

`` `
Serial.println ("COOKIE:" + UTC.dateTime (COOKIE));
Serial.println ("IS8601:" + UTC.dateTime (ISO8601));
Serial.println ("RFC822:" + UTC.dateTime (RFC822));
Serial.println ("RFC850:" + UTC.dateTime (RFC850));
Serial.println ("RFC3339:" + UTC.dateTime (RFC3339));
Serial.println ("RFC3339_EXT:" + UTC.dateTime (RFC3339_EXT));
Serial.println ("RSS:" + UTC.dateTime (RSS));
Serial.println ();
Serial.println ("o simile" + UTC.dateTime ("l ~ t ~ h ~ e jS ~ o ~ f F Y, g: i A"));
`` `

`` `
COOKIE: sabato 25 agosto - 2018 14:23:45 UTC
IS8601: 2018-08-25T14: 23: 45 + 0000
RFC822: sab, 25 ago 18 14:23:45 +0000
RFC850: sabato, 25 agosto-18 14:23:45 UTC
RFC3339: 2018-08-25T14: 23: 45 + 00: 00
RFC3339_EXT: 2018-08-25T14: 23: 45.846 + 00: 00
RSS: sab, 25 ago 2018 14:23:45 +0000

o come sabato 25 agosto 2018, 14:23
`` `

& Nbsp;

### millisecondi

`` `
for (int n = 0; n <10; n ++) {
Serial.println (UTC.dateTime ("l, d-M-y H: i: s.v T"));
}
`` `

`` `
Sabato, 25 agosto-18 14: 32: 53.282 UTC
Sabato, 25 agosto-18 14: 32: 53.283 UTC
Sabato, 25 agosto-18 14: 32: 53.284 UTC
Sabato, 25 agosto-18 14: 32: 53.285 UTC
Sabato, 25 agosto-18 14: 32: 53.287 UTC
Sabato, 25 agosto-18 14: 32: 53.290 UTC
Sabato, 25 agosto-18 14: 32: 53.293 UTC
Sabato, 25 agosto-18 14: 32: 53,297 UTC
Sabato, 25 agosto-18 14: 32: 53.300 UTC
Sabato, 25 agosto-18 14: 32: 53.303 UTC
`` `

> * Questo è sul mio ESP32. Guarda come riempie il buffer seriale in un primo momento, quindi devi aspettare che i caratteri vengano inviati prima che possa tornare? *

& Nbsp;

### Informazioni complete e * ... oh, guarda solo questi aggiornamenti NTP *

`` `
[...]
setInterval (60);
setDebug (INFO);
}

void loop () {
eventi ();
}
`` `

`` `
livello di debug ezTime impostato su INFO
Interrogare pool.ntp.org ... successo (andata e ritorno 42 ms)
Ora ricevuta: sabato 25-ago-18 14: 34: 53.410 UTC (l'orologio interno era veloce 1 ms)
Interrogare pool.ntp.org ... successo (andata e ritorno 43 ms)
Ora ricevuta: sabato, 25-ago-18 14: 35: 53.480 UTC (l'orologio interno era lento di 1 ms)
Interrogare pool.ntp.org ... successo (andata e ritorno 43 ms)
Ora ricevuta: sabato 25-ago-18 14: 36: 53,525 UTC (l'orologio interno era lento di 1 ms)
Interrogare pool.ntp.org ... successo (andata e ritorno 36 ms)
Ora ricevuta: sabato 25-ago-18 14: 37: 53.573 UTC (l'orologio interno era 4 ms lento)
Interrogazione su pool.ntp.org ... successo (andata e ritorno 35 ms)
Ora ricevuta: sabato 25-ago-18 14: 38: 53,636 UTC (l'orologio interno era a posto)
Interrogare pool.ntp.org ... successo (andata e ritorno 32 ms)
Ora ricevuta: sabato 25-ago-18 14: 39: 53,674 UTC (l'orologio interno era lento di 1 ms)
`` `

& Nbsp;

## Iniziare

ezTime è una libreria di Arduino. Per iniziare a usarlo con l'IDE Arduino:

* Scegli Schizzo -> Includi libreria -> Gestisci librerie ...
* Digita `ezTime` nella casella di ricerca.
* Fare clic sulla riga per selezionare la libreria.
* Fare clic sul pulsante Installa per installare la libreria.

in File -> Esempi vedrai ora un ezTime in "Esempi da librerie personalizzate". Puoi provare a eseguire alcuni di questi esempi per vedere se tutto funziona. ezTime è fatto per essere, come suggerisce il nome, abbastanza facile da usare. Quindi probabilmente capirai molto di come funzionano le cose guardando solo gli esempi. Ora puoi solo giocare con quelli e usare il resto di questa documentazione solo quando ti blocchi, o continua a leggere per vedere come funzionano le cose in ezTime.

# & nbsp;

# ezTime Manuale dell'utente

& Nbsp;

## Di questo manuale

### Funzioni semi-interne

Alcune funzioni non sono necessariamente utili per gli utenti quotidiani di questa libreria, ma potrebbero essere utili a qualcuno un giorno. Ad esempio, questa libreria controlla automaticamente i server NTP, quindi non dovrebbe essere necessario "manualmente" ottenere una risposta NTP. Ma la funzione per farlo è ancora esposta all'utente. Anche alcune funzioni che non hanno nulla a che fare con il tempo, come `zeropad` sono lì per te da usare, semplicemente perché * potrebbero * essere utili a qualcuno, e la biblioteca ha bisogno di loro internamente in modo che non abbiano alcun costo aggiuntivo in termini di dimensioni . In questo manuale, i nomi di queste funzioni sono stampati in * corsivo * nelle intestazioni dei capitoli, solo per rendere più facile per voi vedere quali funzioni sono funzionalità di base e che non sono realmente necessarie nell'uso quotidiano.

### Specifica del tempo

Odio la documentazione che mi fa comunque raggiungere il codice sorgente, quindi questo manuale fornisce il prototipo di funzione con ciascuna funzione in modo da poter vedere quali tipi o argomenti ciascuna funzione prende e che tipo è il valore restituito. Ho preso una scorciatoia però. Molte funzioni consentono di specificare un orario. Nel prototipo di funzione questo sembra:

`time_t t = TIME_NOW, const ezLocalOrUTC_t local_or_utc = LOCAL`

In questo manuale, sostituiamo questi due argomenti opzionali nelle definizioni di funzione con:

`time`

Questo perché il priore è solo un po 'troppo lungo per essere ripetuto migliaia di volte, e inoltre rende le cose più complicate di quanto debbano essere. Nella maggior parte dei posti in cui specifichi un'ora in ezTime, è più probabile che tu intenda "adesso". Questo può essere fatto non fornendo alcun argomento, o `TIME_NOW`. Potresti fare una serie di richieste di fila e volere assicurarti che l'ora non cambierà tra loro. Non è necessario attenersi al valore temporale in una variabile. Dopo aver effettuato una chiamata che non specifica il tempo (che significa `TIME_NOW`), puoi specificare` LAST_READ` per usare l'ora dal momento esatto in cui hai effettuato quella prima chiamata.

Altrimenti, è possibile specificare un valore `time_t`, un noto intero con segno a 32 bit per specificare il tempo in secondi trascorsi dalle 00:00 del 1 ° gennaio 1970. Se si specifica un valore diverso da` TIME_NOW` o `LAST_READ`, puoi quindi specificare se intendi in UTC o in ora locale, seguendolo con un secondo argomento che è "UTC_TIME" o "LOCAL_TIME".

Ad esempio, se hai impostato un fuso orario chiamato Berlino, `Berlin.isDST (1536314299, UTC_TIME)` ti dice se l'ora legale è attiva in quel momento, come secondi da 00:00 1 gen 1970 UTC, in contrapposizione a la stessa quantità di secondi da quel momento a Berlino (che sarebbe l'impostazione predefinita). Ci saranno alcuni esempi in seguito, che mostrano come creare ed elaborare tali timestamp. Per lo più, non è necessario specificare nulla perché si desidera solo qualcosa legato al tempo "in questo momento".

> * Sidenote time-geek: ezTime non ha informazioni storiche sulle regole dell'ora legale del passato o del futuro, applica solo le regole che ha ora come se fossero applicate anche nel passato o nel futuro. Controlla [qui] (https://www.timeanddate.com/) per i record storici per i fusi orari. *

& Nbsp;

## Come funziona tutto

### Cosa succede quando includi la libreria

Tutto inizia quando includi la libreria con `#include <ezTime.h>`. Da quel momento in avanti è possibile utilizzare le funzioni in questo manuale per controllare il comportamento di ezTime. Ci sarà poi un oggetto fuso orario chiamato "UTC", che verrà impostato come fuso orario predefinito per tutti i comandi che hanno un prefisso temporale opzionale.

### Nessun demone qui

È importante capire cosa fa ezTime. In qualche modo non crea un processo in background che mantiene il tempo, i server dei contatti o qualsiasi altra cosa. L'Arduino fa il cronometraggio per noi con il suo contatore `millis ()`, che mantiene il tempo in millisecondi dall'inizio dell'Arduino. Tutto ciò che ezTime fa quando sincronizza il tempo è quello di memorizzare un tempo (in secondi dal 1970) e la posizione del contatore millis quando era. Vedendo quanto il contatore dei millis è avanzato e aggiungendo quel punto di partenza dal 1970, ezTime dice al tempo. Ma quel clock interno non è perfetto, potrebbe & mdash; molto lentamente & mdash; allontanarsi dal tempo reale. Ecco perché c'è un evento periodico impostato per sincronizzare l'orologio con il server NTP.

Se vuoi che succedano degli eventi & mdash; se il tuo o l'NTP aggiorna periodicamente ezTime) & mdash; dovresti avere `events ()` nel ciclo principale del tuo programma.

### Ma mi sono appena svegliato!

Il tuo codice potrebbe chiamare `Serial.println (UTC.dateTime ());` per stampare una rappresentazione testuale completa di data e ora nel formato predefinito della porta seriale. La libreria avrebbe scoperto che il tempo non era ancora stato sincronizzato e invierebbe una richiesta NTP a uno dei server NTP a cui `pool.ntp.org` risolve. Se il tuo Arduino si è appena svegliato, probabilmente non ha ancora ricevuto le informazioni DHCP, o non è ancora connesso alla rete WiFi. E così la ricerca del tempo fallirebbe e la chiamata a `.dateTime` restituirebbe una stringa con la data e l'ora subito dopo la mezzanotte del 1 ° gennaio 1970: il punto zero per il contatore del tempo in stile Unix usato da ezTime. In seguito sarebbe corretto in tempo reale, ma non è carino.

Peggio è quando si imposta un fuso orario per il quale si desidera recuperare le regole dell'ora legale dal server: non può farlo se la connessione non è ancora attiva. Ecco perché esiste una funzione chiamata `waitForSync` che chiama semplicemente` events () `fino a quando non viene sincronizzata (o finché non passa un numero prestabilito di secondi, vedi sotto).

& Nbsp;

## Impostazione e sincronizzazione del tempo

La richiesta NTP dallo scenario precedente non è riuscita perché la rete non era ancora attiva, quindi l'orologio non sarebbe ancora sincronizzato. Una nuova richiesta verrà pianificata per 1801 secondi e inviata quando il tuo codice (o `waitForSync`) chiama` events`.

& Nbsp;

### timeStatus

`timeStatus_t timeStatus ();`

Restituisce lo stato in cui si trova l'orologio. `TimeStatus ()` restituirà uno di:

| timeStatus | significato |
| ---- | ---- |
| `timeNotSet` | Nessun aggiornamento NTP o impostazione manuale dell'orologio (chiamando il metodo `.setTime` di un fuso orario) ha avuto luogo |
| `timeSet` | L'orologio dovrebbe avere l'ora corrente |
| `timeNeedsSync` | Una richiesta di NTP programmata è scaduta da più di un'ora. (Il tempo necessario per un aggiornamento prima che 'timeNeedsSync` sia impostato è configurato dalla definizione `NTP_STALE_AFTER` nel file` ezTime.h`.) |

& Nbsp;

### waitForSync

`bool waitForSync (uint16_t timeout = 0);`

Se il tuo codice utilizza fusi orari diversi da UTC, potrebbe essere necessario attendere per inizializzarli finché non c'è un tempo valido per vedere se le definizioni del fuso orario memorizzate nella cache sono ancora attuali. E se stai visualizzando un calendario o un orologio, potrebbe sembrare sciocco se prima dice mezzanotte del 1 ° gennaio 1970 prima di mostrare il tempo reale. `waitForSync` attenderà il collegamento della rete, quindi il tempo per essere sincronizzato prima di restituire` true`. Se si specifica un timeout (in secondi), esso ritorna dopo molti secondi anche se l'orologio non è ancora sincronizzato, restituendo `false`. (ezTime error `TIMEOUT`, vedi il [capitolo sui messaggi di errore e di debug] (# errors-and-debug-information) più avanti)

& Nbsp;

### * setServer e setInterval *

`void setServer (String ntp_server = NTP_SERVER);`

`void setInterval (uint16_t seconds = 0);`

Per impostazione predefinita, ezTime è impostato per il polling `pool.ntp.org` circa ogni 30 minuti. Queste impostazioni predefinite dovrebbero funzionare per la maggior parte delle persone, ma è possibile cambiarle specificando un nuovo server con `setServer` o un nuovo intervallo (in secondi) con setInterval. Se chiami setInterval con un intervallo di 0 secondi o lo chiami come "setInterval ()", non verranno più effettuate query NTP.

& Nbsp;

### * updateNTP *

`void updateNTP ();`

Aggiorna immediatamente l'ora dal server NTP. Continuerà a riprovare ogni 30 minuti (definito da `NTP_RETRY` in` ezTime.h`), pianificherà il prossimo aggiornamento dopo l'intervallo normale.

& Nbsp;

### * lastNtpUpdateTime *

`time_t lastNtpUpdateTime ();`

Restituirà l'ultima volta in cui l'ora è stata sincronizzata con successo con il server NTP.

& Nbsp;

### * queryNTP *

`bool queryNTP (server delle stringhe, time_t & t, unsigned long e measured_at);`

Ciò invierà una singola query al server NTP specificato dall'utente. Inserirà, nelle variabili `t` e` measured_at` passate per riferimento, il tempo di UTIX unix e il contatore `millis ()` nel momento in cui è accaduto il secondo esatto. Lo fa sottraendo da `millis ()` i secondi frazionari ricevuti nella risposta, così come la metà del tempo necessario per ottenere una risposta. Ciò significa che presuppone che il ritardo della rete sia simmetrico, il che significa che la richiesta di ottenere il server ha richiesto il tempo richiesto per la risposta.

Se il time server risponde, `queryNTP` restituisce` true`. Se viene restituito `false`,` error () `restituirà 'NO_NETWORK` (se WiFi non è connesso) o` TIMEOUT` se una risposta ha richiesto più di 1500 millisecondi (definiti da `NTP_TIMEOUT` in` ezTime.h`. ).

Nota che questa funzione è usata internamente da ezTime, ma di per sé non mantiene il tempo che tiene ezTime. Probabilmente non dovrai mai chiamare questo dal tuo codice.

& Nbsp;

## Fusi orari

> * Se solo fosse così semplice come suggerisce questa mappa. Ogni band è in realtà composta da paesi che cambiano tutti in base all'ora legale in date diverse e spesso cambiano anche le regole per quando ciò accade. *

! [] (Immagini / timezones.gif)

I fusi orari in ezTime sono oggetti. Possono essere creati con `Timezone yourTZ`, dove` yourTZ` è il nome che scegli di fare riferimento al fuso orario. In questo manuale, questo nome sarà utilizzato da ora in poi. Ma puoi scegliere il nome che preferisci.

Internamente, ezTime memorizza tutto ciò che sa su un fuso orario come due stringhe. Uno è il nome ufficiale del fuso orario nel formato "Olson" (come "Europa / Berlino"). Quel nome è usato per aggiornare poi quando necessario tutte le altre informazioni necessarie per rappresentare il tempo in quel fuso orario. Questo è in un'altra stringa, nel cosiddetto formato "posix". Spesso è un po 'più lungo e per Berlino è `CET-1CEST, M3.4.0 / 2, M10.4.0 / 3`. Gli elementi di questa stringa hanno i seguenti significati:

| Elemento | significato |
| ---- | ---- |
| `CET` | Nome del fuso orario in orario standard (CET = Central European Time in questo caso).
| `-1` | Ore compensate da UTC, che significa sottrarre un'ora da questo momento per arrivare a UTC. (Nota offset è spesso scritto altrove al contrario (quindi +1 in questo caso), solo per confondere le cose.) Potrebbe anche specificare minuti, come `-05: 30` per l'India. |
| `CEST` | Nome del fuso orario in Ora legale (DST), CEST sta per Ora legale dell'Europa centrale |
| `, M3` | L'ora legale inizia a marzo |
| `.4` | Alla quarta occorrenza di
| `.0` | una domenica |
| `/ 2` | alle 02:00 ora locale |
| `, M10` | L'ora legale finisce a ottobre |
| `.4` | alla quarta occorrenza di |
| `.0` | una domenica |
| `/ 3` | alle 03:00 ora locale |

& Nbsp;

### imposta default

`void setDefault ()` & nbsp; & nbsp; & nbsp; & nbsp; & mdash; & nbsp; ** MUST ** deve essere prefisso con il nome di un fuso orario, come `yourTz.setDefault ()`

`#include <ezTime.h>` include la libreria, crea l'oggetto `ezTime` e l'istanza` UTC` della classe `Timezone`, oltre a` defaultTZ`, che è un riferimento a UTC a meno che non lo si imposti su un altro fuso orario da chiamando `yourTZ.setDefault ()`. ezTime è compatibile con la classica libreria di tempo Arduino, e quindi puoi chiamare varie funzioni nello spazio dei nomi di root come `hour ()` e `minute ()` & mdash; senza un fuso orario di fronte. Sono interpretati come se fossero passati al fuso orario predefinito. Quindi, se hai un codice esistente, basta impostare un fuso orario e impostarlo come predefinito dovrebbe far funzionare quel codice come se l'ora fosse stata impostata nell'ora locale. Il nuovo codice che dipende da ezTime dovrebbe probabilmente menzionare esplicitamente il fuso orario.

& Nbsp;

### setPosix

`bool setPosix (String posix)` & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; ** MUST ** deve essere prefisso con il nome di un fuso orario, ad esempio `India.setPosix (" IST-5: 30 ")`

Ti permette di inserire direttamente le informazioni sulla posix per un fuso orario. Per semplici fusi orari, è possibile impostare le cose manualmente. Ad esempio per l'India, un semplice

`` `
Timezone India;
India.setPosix ( "IST-5: 30");
Serial.println (India.dateTime ());
`` `

è abbastanza, perché il tempo trascorso in India non va avanti e indietro con l'andirivieni dell'ora legale (anche se l'offset di mezz'ora verso l'UTC è piuttosto strano).

& Nbsp;

### getPosix

`String getPosix ()` & nbsp; & nbsp; & nbsp; & nbsp; & mdash; & nbsp; ** MUST ** deve essere preceduto dal nome di un fuso orario, ad esempio `India.getPosix ()`

`getPosix` fa quello che ti aspetteresti e restituisce semplicemente la stringa posix memorizzata in ezTime per un determinato fuso orario.

& Nbsp;

### isDST

`bool isDST (TIME);` & nbsp; & nbsp; & nbsp; & nbsp; & mdash; & nbsp; Presuppone il fuso orario predefinito se non viene prefissato un fuso orario

Ti dice se l'ora legale è in vigore in un determinato momento in questo fuso orario. Se non fornisci argomenti, è interpretato come "adesso". Puoi anche specificare un tempo (in secondi dal 1970, ritorneremo a quello) nel primo argomento. Se vuoi sapere una determinata ora in UTC all'interno del windown dell'ora legale in un determinato fuso orario, puoi impostare il secondo argomento su `false`, altrimenti si presume che tu stia chiedendo un orario espresso come ora locale.

& Nbsp;

### getTimezoneName

`String getTimezoneName (TIME);` & nbsp; & nbsp; & nbsp; & nbsp; & mdash; & nbsp; Presuppone il fuso orario predefinito se non viene prefissato un fuso orario

Fornisce il codice breve corrente per il fuso orario, come "IST" per l'India, o "CET" (durante l'orario standard) o "CEST" (durante l'ora legale) per la maggior parte dell'Europa.

& Nbsp;

### getOffset

`int16_t getOffset (TIME)` & nbsp; & nbsp; & nbsp; & nbsp; & mdash; & nbsp; Presuppone il fuso orario predefinito se non viene prefissato un fuso orario

Fornire l'offset da UTC in minuti all'ora indicata (o ora se non si specifica nulla). L'offset qui è nella stessa direzione delle informazioni di posix, quindi -120 significa 2 ore a est di UTC.

& Nbsp;

### setLocation

`boolsetLocation (String location =" ")` & nbsp; & nbsp; & nbsp; & nbsp; & mdash; & nbsp; ** MUST ** deve essere preceduto dal nome di un fuso orario

Con `setLocation` puoi fornire una stringa per effettuare una ricerca su Internet per un fuso orario. La stringa può essere sia un nome di fuso orario Olson, come `Europa / Berlino` (o qualche parte univoca di tale nome). ([Qui] (https://en.wikipedia.org/wiki/List_of_tz_database_time_zones) è un elenco completo di questi nomi.) Oppure può essere un codice paese di due lettere per qualsiasi paese che non si estende su più fusi orari, come ` NL` o `DE` (ma non` US`). Dopo aver recuperato le informazioni, viene caricato nel fuso orario corrente e memorizzato nella cache se è impostata una cache (vedere sotto). `setLocation` restituirà` false` (Impostazione `NO_NETWORK`,` DATA_NOT_FOUND` o `SERVER_ERROR`) se non è possibile ottenere informazioni sul fuso orario.

Se non fornisci alcuna posizione (`YourTZ.setLocation ()`), ezTime tenterà di effettuare una ricerca GeoIP per trovare il Paese associato al tuo indirizzo IP. Se quello è un paese che ha un singolo fuso orario, verrà caricato quel fuso orario, altrimenti verrà visualizzato un `SERVER_ERROR` (" Paese estende più fusi orari ").

Nel caso di `SERVER_ERROR`,` errorString () `restituisce l'errore dal server, che potrebbe essere" Paese comprende più fusi orari "," Paese non trovato "," Ricerca GeoIP non riuscita "o" Fuso orario non trovato ".

Se esegui più chiamate a `setLocation`, assicurati che siano a più di 3 secondi di distanza, perché il server non risponderà se le chiamate dallo stesso IP arrivano entro 3 secondi l'una dall'altra (vedi sotto).

& Nbsp;

### timezoned.rop.nl

`timezoned.rop.nl` è il servizio di fuso orario di ezTime a cui si connette. È un semplice servizio UDP che riceve un pacchetto sulla porta UDP 2342 con la richiesta e risponde con un pacchetto che contiene le informazioni POSIX per quel fuso orario (dopo "OK") o l'errore (dopo "ERR"). Risponderà allo stesso numero IP una volta ogni tre secondi per evitare di essere utilizzato negli attacchi DDoS.

Il servizio ha il potenziale di vedere quali numeri IP usano ezTime e quali dati del fuso orario richiedono. Qualsiasi ricerca GeoIP viene eseguita su un database locale, non sono coinvolte terze parti. Il servizio non mantiene i file di registro a meno che qualcosa non funzioni e sia necessario eseguire il debug. In tal caso, tutti i file di registro verranno cancellati al termine del lavoro, ma entro 48 ore al massimo.

I dati non sono mai stati utilizzati per scopi diversi dal debug, né sono previsti altri usi futuri.

& Nbsp;

### Memorizzazione nella cache del fuso orario, EEPROM o NVS

Se si utilizza setLocation, le informazioni sul fuso orario provengono da timezoned.rop.nl. Cercherò di farlo funzionare nel modo più stabile possibile, ma se quel servizio ha un problema, il tuo Arduino non saprebbe più a che ora si trova.

Ecco perché è possibile creare un posto per ezTime per memorizzare i dati relativi al fuso orario. In questo modo, non è necessario ottenere nuovamente le informazioni ogni volta che si avvia l'Arduino. È possibile memorizzare la cache per un fuso orario in EEPROM (impostazione predefinita) o NVS.

Se il tuo codice dice `tz.setCache (0)` proverà a leggere e interpretare i dati da EEPROM posizione 0 immediatamente, e memorizzerà tutti i dati aggiornati futuri che riceve per quel fuso orario lì. Alcuni programmi potrebbero voler semplicemente avviarsi in qualsiasi fuso orario impostato dall'utente, quindi chiamano "yourTZ.setCache (0)" quando iniziano e "yourTZ.setLocation" quando l'utente accede alle impostazioni per selezionare un fuso orario diverso. I programmi semplici potrebbero fare:

`` `
if (! someTZ.setCache (0)) someTZ.setLocation ("Europe / Berlin");
`` `

Per ottenere solo i dati del fuso orario da Internet quando la cache è vuota o obsoleta e utilizzare le informazioni memorizzate nella cache tutte le altre volte. (Si noti che se si modifica la città nell'esempio sopra, si otterranno comunque le informazioni di Berlino dalla cache e non si eseguirà `setLocation` finché non si esegue` someTZ.clearCache () `.

& Nbsp;

### setCache

`bool setCache (int16_t address)` & nbsp; & nbsp; & nbsp; & nbsp; & mdash; & nbsp; ** MUST ** deve essere preceduto dal nome di un fuso orario

Se ezTime è compilato con `#define EZTIME_CACHE_EEPROM` (che è l'impostazione predefinita), è possibile fornire una posizione EEPROM. Un singolo fuso orario richiede 50 byte per la cache. I dati sono scritti in forma compressa in modo che le stringhe Olson e Posix si adattino in 3/4 dello spazio che normalmente occupano, e insieme a esso viene memorizzato un checksum, un campo lunghezza e un singolo byte per il mese in cui il la cache è stata recuperata, nei mesi successivi al gennaio 2018.

`bool setCache (Nome stringa, Chiave stringa)` & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; & nbsp; ** DEVE ** essere preceduto dal nome di un fuso orario

Su ESP32 e forse su altre piattaforme, c'è un'emulazione per EEPROM in flash, ma c'è anche un meccanismo più carino che memorizza chiavi e valori in flash. Puoi usare questo comando abilitando `#define EZTIME_CACHE_NVS` in` ezTime.h`. Puoi quindi fornire un nome di sezione e una chiave per fungere da posizione di memorizzazione della cache per un determinato fuso orario.

& Nbsp;

### clearCache

`void clearCache (bool delete_section = false)` & nbsp; & nbsp; & nbsp; & nbsp; & mdash; & nbsp; ** MUST ** deve essere preceduto dal nome di un fuso orario

Cancella la cache per un fuso orario. Se si utilizza EEPROM, i byte vengono sovrascritti con zeri, se si utilizza NVS, la chiave viene cancellata. Se fornisci l'argomento `true` usando NVS, l'intera sezione viene cancellata. Fatelo solo se quella sezione non contiene altro che volete mantenere.

& Nbsp;

### Crazy timezones

#### Isole Chatham e Nepal

Le isole Chatham si trovano nel Pacifico a circa 800 chilometri a est della Nuova Zelanda. Circa 600 persone vivono lì, ma hanno il loro fuso orario (UTC + 12: 45). È uno dei soli tre fusi orari con uno scostamento di 45 minuti da UTC, gli altri sono Nepal Time (UTC + 05: 45) e il non ufficiale Australian Central Western Time (UTC + 08: 45). Questi fusi orari funzionano bene in ezTime.

#### Marocco

Il Marocco va avanti e indietro all'ora legale due volte all'anno. Questo al momento interrompe ezTime in quanto il nostro parser può gestire solo un periodo di DST all'anno. Per fortuna smetteranno di farlo nel 2020: i marocchini probabilmente si sono stancati di tutti gli orologi che non si adattano correttamente.

& Nbsp;

## Ottenere data e ora

### appuntamento

`String dateTime (TIME, String format = DEFAULT_TIMEFORMAT);` <br>
& nbsp; & nbsp; & nbsp; & nbsp; & mdash; & nbsp; Presuppone il fuso orario predefinito se non viene prefissato un fuso orario

Inizieremo con una delle funzioni più potenti di ezTime. Con `dateTime` puoi rappresentare una data e / o un'ora nel modo desiderato. Lo fai nello stesso modo in molti linguaggi di programmazione: fornendo una speciale stringa di formattazione. Molti personaggi in questa stringa hanno significati speciali e saranno sostituiti. Ciò significa che `UTC.dateTime (" l, d-M-y H: i: s.v T ")` potrebbe restituire `Saturday, 25-Aug-18 14: 32: 53.282 UTC`. Di seguito è riportato l'elenco dei caratteri e di cosa sono sostituiti da. Tutti i personaggi non presenti in questa lista non sono semplicemente sostituiti e rimangono come sono. Guarda le ultime due voci per un modo di usare i caratteri su questo elenco nella tua stringa.

| char | sostituito da
| ----- | : -----
| `d` | Giorno del mese, 2 cifre con zero iniziali
| `D` | Prime tre lettere del giorno in inglese, come `Tue`
| `j` | Giorno del mese senza zeri iniziali
| `l` | (L minuscolo) Il giorno della settimana in inglese, come `Tuesday`
| `N` | // Rappresentazione numerica ISO-8601 del giorno della settimana. (1 = Monday, 7 = Sunday)
| `S` | English ordinal suffix for the day of the month, 2 characters (st, nd, rd, th)
| `w` | Numeric representation of the day of the week (0 = Sunday)
| `F` | A month's name, such as `January`
| `m` | Numeric representation of a month, with leading zeros
| `M` | Three first letters of a month in English, like `Apr`
| `n` | Numeric representation of a month, without leading zeros
| `t` | Number of days in the given month
| `Y` | A full numeric representation of the year, 4 digits
| `y` | Last two digits of the year
| `a` | antimeridiane o pomeridiane
| `A` | AM or PM
| `g` | 12-hour format of an hour without leading zeros
| `G` | 24-hour format of an hour without leading zeros
| `h` | 12-hour format of an hour with leading zeros
| `H` | 24-hour format of an hour with leading zeros
| `i` | Minutes with leading zeros
| `s` | Seconds with leading zero
| `T` | abbreviation for timezone, like `CEST`
| `v` | milliseconds as three digits
| `e` | Timezone identifier (Olson name), like `Europe/Berlin`
| `O` | Difference to Greenwich time (GMT) in hours and minutes written together, like `+0200`. Here a positive offset means east of UTC.
| `P` | Same as O but with a colon between hours and minutes, like `+02:00`
| `Z` | Timezone offset in seconds. West of UTC is negative, east of UTC is positive.
| `z` | The day of the year (starting from 0)
| `W` | ISO-8601 week number. See right below for explanation link.
| `X` | ISO-8601 year for year-week notation as four digit year. Warning: Not guaranteed to be same as current year, may be off by one at start or end of year. See [here](https://en.wikipedia.org/wiki/ISO_week_date)
| `B` | One-letter military code for the timezone, or `?` if the offset is not a whole number of hours.
| `\` | Not printed, but escapes the following character, meaning it will not be replaced. But inserting a backslash in the string means you have to supply two backslashes `\\` to be interpreted as one.
| `~` | (tilde) Same as backslash above, except easier to insert in the string. Example: `~t~h~e` will print the word `the` in the string. Letters should be escaped even if they are not on the list because they may be replaced in future versions.

So as an example: `UTC.dateTime("l ~t~h~e jS ~o~f F Y, g:i A")` yields date and time in this format: `Saturday the 25th of August 2018, 2:23 PM`.

&nbsp;

### Built-in date and time formats

There are built-in values to specify some standard date and time formats. For example: `UTC.dateTIme(RSS)` (without quotes around RSS) returns something like `Sat, 25 Aug 2018 14:23:45 +0000`. Here's a list of all these built in format abbreviations.

| name        | formatted date and time
|:------|:------|
| ATOM        | 2018-08-25T14:23:45+00:00
| COOKIE      | Saturday, 25-Aug-2018 14:23:45 UTC
| IS8601      | 2018-08-25T14:23:45+0000
| RFC822      | Sat, 25 Aug 18 14:23:45 +0000
| RFC850      | Saturday, 25-Aug-18 14:23:45 UTC
| RFC1036     | Sat, 25 Aug 18 14:23:45 +0000
| RFC1123     | Sat, 25 Aug 2018 14:23:45 +0000
| RFC2822     | Sat, 25 Aug 2018 14:23:45 +0000
| RFC3339     | 2018-08-25T14:23:45+00:00
| RFC3339_EXT | 2018-08-25T14:23:45.846+00:00
| RSS         | Sat, 25 Aug 2018 14:23:45 +0000
| W3C         | 2018-08-25T14:23:45+00:00
| ISO8601_YWD | 2018-W34-5

&nbsp;

### Time and date as numbers

`time_t now()`&nbsp;&nbsp;&nbsp;&nbsp;&mdash;&nbsp;Assumes default timezone if no timezone is prefixed

Returns the current time in seconds since midnight Jan 1st 1970 in the timezone specified.

`uint8_t hour(TIME)`<br>
`uint8_t hourFormat12(TIME)`<br>
`uint8_t minute(TIME)`<br>
`uint8_t second(TIME)`<br>
`uint16_t ms(TIME)`<br>
`uint8_t day(TIME)`<br>
`uint8_t weekday(TIME)`<br>
`uint8_t month(TIME)`<br>
`uint16_t year(TIME);`

These functions return the various elements of date or time for right now (no arguments) or for a given time in seconds sinds 1970. `weekday` returns a number starting with 1 for Sunday. `hourFormat12` does hours from 1 to 12.

If you want to compare you can use compiler defines in all capital letters for names of days and months, like:

```
if (weekday() == TUESDAY) Serial.print("Tuesday!!");
```

```
if (month() == FEBRUARY && day() == 14) Serial.print("Valentine's day!");
```

&nbsp;

`bool isAM(TIME)`&nbsp;&nbsp;&nbsp;&nbsp;&mdash;&nbsp;Both assume default timezone if no timezone is prefixed<br>
`bool isPM(TIME)`

These will tell if it is before or after noon for a given `TIME`, return `true` or `false`.

&nbsp;

`uint16_t dayOfYear(TIME)`&nbsp;&nbsp;&nbsp;&nbsp;&mdash;&nbsp;Assumes default timezone if no timezone is prefixed

Returns how many days have passed in the year. January 1st returns 0,

&nbsp;

### *weekISO and yearISO*

`uint8_t weekISO(TIME)`<br>`uint16_t yearISO(TIME)`&nbsp;&nbsp;&nbsp;&nbsp;&mdash;&nbsp;Both assume default timezone if no timezone is prefixed

These functions return the ISO-8601 Year-week notation year and week number. Note that the year returned here can differ one from the current year at the first or last days or the year. ISO-8601 defines the first year of the week as the first week that has a Thursday in it. Meaning the start of the ISO-year can be a few days earlier (in December) or a few days later (in January).

&nbsp;

### *militaryTZ*

`String militaryTZ(TIME)`&nbsp;&nbsp;&nbsp;&nbsp;&mdash;&nbsp;Assumes default timezone if no timezone is prefixed

Returns the one-letter military code for the timezone. See [here](https://www.timeanddate.com/time/zones/military) for details. If the offset for the current timezone is not a whole number of hours, "?" is returned.

&nbsp;

### secondChanged and minuteChanged

`bool secondChanged()`

`bool minuteChanged()`

You might have code that put the time on a display in some really nice-looking format, using `dateTime`. The main loop wants to keep the time updated, but not every time the main loop runs, because it would cause the display to flicker. The classic solution for this is to store the time, recreate the string every time and compare to see if it changed. With `secondChanged` and `minuteChanged` you can just write something like:

```
if (minuteChanged()) WriteToSomeDisplay(UTC.dateTime("H:i"));
```

&nbsp;

### names of days and months

`String dayStr(const uint8_t day)`

`String dayShortStr(const uint8_t day)`

`String MonthStr(const uint8_t month)`

`String MonthShortStr(const uint8_t month)`

These functions will take a numeric argument and convert it to the name of the day or the name of the months. These functions do not tell you the current day or month, they just convert the number `1` to `Sunday`, `Sun`, `January` or `Jan` respectively. They are here to be compatible with the classic Time library. The [`dateTime`](#datetime) function can provide all sorts of strings and is much more flexible.

&nbsp;

## Events

### events

`void events()`

This is what your loop functions should call if they want events executed. This includes user-set events (see below) and the NTP updates that ezTime does periodically. `events()` also calls the Arduino function `yield()`, so you do not need to call that anymore (but once more doesn't hurt).

&nbsp;

### setEvent

`uint8_t setEvent(void (*function)(), TIME)`&nbsp;&nbsp;&nbsp;&nbsp;&mdash;&nbsp;Both assume default timezone if no timezone is prefixed

`uint8_t setEvent(void (*function)(), uint8_t hr, uint8_t min, uint8_t sec,`<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`uint8_t day, uint8_t mnth, uint16_t yr)`


With ezTime, you can set your own events to run at a specified time. Simply run `setEvent` specifying the name of the function you would like to call (without the brackets) and a time you would like to call it. The first time `events` runs and notices that it is at or after the time you specified it will run and then delete the event. If you want an event to recur, simply set a new event in the function that gets called. You can have a maximum of 8 events by default (easily changed by changing `MAX_EVENTS` in `ezTime.h`). ezTime uses one event internally to trigger the next NTP update.

`setevent` returns an 8-bit event handle between 1 and MAX_EVENTS which you can store in a variable and use to delete the event with `deleteEvent` should your program need to. Zero is returned and the error `TOO_MANY_EVENTS` set if there are no more free slots for your new event.

&nbsp;

### deleteEvent

`void deleteEvent(uint8_t event_handle)`

Deletes the event with the handle as returned by `setEvent`.

`void deleteEvent(void (*function)())`

Buy you can also call `deleteEvent` with the name of the function (again without the brackets) to delete all events that would have executed that function.

&nbsp;


## Setting date and time manually

![](images/setting-clock.jpg)

### setTime

`void setTime(time_t t, uint16_t ms = 0)`&nbsp;&nbsp;&nbsp;&nbsp;&mdash;&nbsp;Both assume default timezone if no timezone is prefixed

`void setTime(uint8_t hr, uint8_t min, uint8_t sec,`<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`uint8_t day, uint8_t mnth, uint16_t yr)`

`setTime` pretty much does what it says on the package: it sets the time to the time specified, either as separate elements or as a time_t value in seconds since Jan 1st 1970. If you have another source of time  &mdash; say, a GPS receiver &mdash; you can use `setTime` to set the time in the UTC timezone. Or you can set the local time in any other timezone you have set up and ezTime will set its internal offset to the corresponding time in UTC so all timezones stay at the correct time.

It's important to realise however that NTP updates will still become due and when they do time will be set to the time returned by the NTP server. If you do not want that, you can turn off NTP updates with `setInterval()`. If you do not use NTP updates at all and do not use the network lookups for timezone information either, you can compile ezTime with no network support by commenting out `#define EZTIME_NETWORK_ENABLE` in the `ezTime.h` file, creating a smaller library.

### Alternate sources of time

If your time source is not NTP, the way to update time is to create a user function that gets the time from somewhere and then sets the clock with `setTime` and then schedules the next time it synchronises the clock with `setEvent`. This way you have full flexibility: you can schedule the next update sooner if this update fails, for instance. Remember to turn off NTP updates if you want your new time to stick.

## Working with time values

### *breakTime*

`void breakTime(time_t time, tmElements_t &tm)`

If you create a `tmElements_t` structure and pass it to `breakTime`, it will be filled with the various numeric elements of the time value specified. tmElements_t looks as follows:

```
typedef struct  {
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t Wday;   // day of week, sunday is day 1
	uint8_t Day;
	uint8_t Month;
	uint8_t Year;   // offset from 1970;
} tmElements_t;
```

Meaning this code would print the hour:

```
tmElements_t tm;
breakTime(UTC.now(), tm);
Serial.print(tm.Hour);
```

But `Serial.println(UTC.hour())` also works and is much simpler. `breakTime` is used internally and is a part of the original Time library, so it is available for you to use. Mind that the year is a single byte value, years since 1970.

&nbsp;

### makeTime

`time_t makeTime(tmElements_t &tm);`

This does the opposite of `breakTime`: it takes a `tmElements_t` structure and turns it into a `time_t` value in seconds since Jan 1st 1970.

`time_t makeTime(uint8_t hour, uint8_t minute, uint8_t second,`<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`uint8_t day, uint8_t month, int16_t year);`

This version takes the various numeric elements as arguments. Note that you can pass the year both as years since 1970 and as full four digit years.

&nbsp;

### *makeOrdinalTime*

`time_t makeOrdinalTime(uint8_t hour, uint8_t minute, uint8_t second,`<br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`uint8_t ordinal, uint8_t wday, uint8_t month, int16_t year);`

With `makeOrdinalTime` you can get the `time_t` value for a date written as "the second Tuesday in March". The `ordinal` value is 1 for first, 2 for second, 3 for third, 4 for fourth and either 5 or 0 for the last of that weekday in the month. `wday` is weekdays starting with Sunday as 1. You can use the names of ordinals, months and weekdays in all caps as they are compiler defines. So the following would find the `time_t` value for midnight at the start of the first Thursday of the year in variable `year`.

```
makeOrdinalTime(0, 0, 0, FIRST, THURSDAY, JANUARY, year)
```

> *This is actually a fragment of ezTime's own code, as it can print ISO week numbers and the first ISO week in a year is defined as the week that has the first Thursday in it.*

&nbsp;

### *compileTime*

`time_t compileTime(String compile_date = __DATE__, String compile_time = __TIME__);`

You can ignore the arguments above and just say `compileTime()`. Returns the time value for when you compiled your sketch. You can check out the "NoNetwork" example with this library to see it in use: it makes your Arduino pretend to know what time it is.

&nbsp;

### *tzTime*

`time_t tzTime(TIME)`&nbsp;&nbsp;&nbsp;&nbsp;&mdash;&nbsp;Both forms **MUST** be prefixed with name of a timezone

This is the internal workhorse function that converts `time_t` in UTC to `time_t` in a timezone or vice versa. It is used by almost all the functions that apply to a timezone, and it takes `TIME` &mdash; meaning nothing for "right now", or a `time_t` value and an optional argument to specify whether that is `LOCAL_TIME` or `UTC_TIME`, and then it will convert to the opposite. `TIME_NOW` and `LAST_READ` are always output as `time_t` in that timezone.

`time_t tzTime(time_t t, ezLocalOrUTC_t local_or_utc, String &tzname, bool &is_dst, int16_t &offset)`

In this second form you have to supply all arguments, and it will fill your `tzname`, `is_dst` and `offset` variables with the appropriate values, the offset is in minutes west of UTC. Note that there are easier functions for you to get this information: `getTimezoneName`, `isDST` and `getOffset` respectively. If your code calls all three in a tight loop you might consider using `tzTime` instead as the other functions each do the whole parsing using `tzTime`, so you would be calling it three times and it does quite a bit.

&nbsp;

## Various functions

These functions are available for you to use because ezTime needed them internally, so they come at no extra cost, so to speak.

### *zeropad*

`String zeropad(uint32_t number, uint8_t length);`

Pads `number` with zeroes to the left until the resulting string is `length` places long.

&nbsp;

## Errors and debug information

### *setDebug*

`void setDebug(ezDebugLevel_t level)`<br>`void setDebug(ezDebugLevel_t level, Print &device)`

`level` sets the level of detail at which ezTime outputs messages on the serial port. Can be set to one of:

| level  | effect  |
|---|---|
| `NONE` | ezTime does not output anything on the serial port |
| `ERROR` | ezTime will show when errors occur. Note that these may be transient errors that ezTime recovers from, such as NTP timeouts. |
| `INFO`  | Essentially shows you what ezTime is doing in the background. Includes messages about NTP updates, initialising timezones, etc etc. |
| `DEBUG`  | Detailed debugging information unlikely to be of much use unless you are trying to get to the bottom of certain internal behaviour of ezTime. |

*Note:* you can specify which level of debug information would be compiled into the library. This is especially significant for AVR Arduino users that need to limit the flash and RAM footprint of ezTtime. See the "Smaller footprint, AVR Arduinos" chapter further down.
`device` is optional and can specify a device to receive the debug messages. This defaults to the Hardwareserial object named `Serial` but can be any device that has inherited from the `Print` class. Don't worry if you don't understand that: it means you can specify not only serial ports, but also a handle to a file you have opened on the SD card as well as a lot of LCD screen devices. For instance, on my M5Stack device I can &mdash; after `#include <M5Stack.h>` and `m5.begin()` &mdash; do: `setDebug(INFO, m5.lcd)`

![](images/M5Stack-debug.jpg)

You cannot send debug information to multiple devices at the same time.

&nbsp;

### *error*

`ezError_t error(bool reset = false);`

A number of functions in ezTime are booleans, meaning they return `true` or `false` as their return value, where `false` means some error occurred. `error` will return an `ezError_t` enumeration, something like `NO_NETWORK` (obvious) or `LOCKED_TO_UTC` (when you try to load some new timezone info to the UTC object). You can test for these specific errors and this document will mention which errors might happen in what functions.

When you call `error(true)`, it will also reset the error to `OK`, so you can make sure no new errors happened after a certain point.

&nbsp;

### *errorString*

`String errorString(ezError_t err = LAST_ERROR);`

This will give you a string representation of the error specified. The pseudo-error `LAST_ERROR`, which is the default, will give you the textual representation of the last error. This will not reset the last error stored.

&nbsp;

## Compatibility with Arduino Time library

The classic Arduino time library has a lot of functions and defines that end up in the root namespace, meaning you can just run `hour()` instead of `someTZ.hour()`. These functions are supported by ezTime and will act as if they are prefixed with the defaultTZ. This is UTC by default, but you can make any timezone the default by writing `someTZ.setDefault();`

If you do not wish to have these functions in your namespace, you can comment out `#define ARDUINO_TIMELIB_COMPATIBILITY` in `ezTime.h`. New code depending on ezTime should probably explicitly state the timezone, especially in code with multiple timezones.

&nbsp;

## Smaller footprint, AVR Arduinos

This library compiles on an Arduino Uno with an Ethernet shield. However, it will use up almost all of the flash on that, which is fine if you were making a date and time display anyway. But if your code is bigger than that, you will want to make it smaller. By uncommenting `#define EZTIME_MAX_DEBUGLEVEL_NONE` in `ezTime.h` you get no debugging information and no textual errors, which saves a couple of kilobytes. If you do not use networking, you should also comment out `#define EZTIME_NETWORK_ENABLE`, that will save a *ton* of space: not just in ezTime but also because the networking library does not get loaded.

&nbsp;

## 2036 and 2038

The NTP timestamps used here run until the 7th of February 2036. NTP itself has 128 bits of time precision, I haven't looked into it much. Didn't have to, because just a little later, on the 19th of January 2038, the time_t 32 bit signed integer overflows. This is 20 years from today, in 2018. The Arduino world, if it still exists around then, will have come together around some solution that probably involves 64-bit time like in many operating systems of 2018. If you use this library in your nuclear generating station (**NOOOOO!**), make sure you're not around when these timers wrap around.

Should you be the one doing maintenance on this in some far-ish future: For ezTime I created another overflowing counter: the cache age for the timezone information is written as a single unsigned byte in months after January 2018, so that could theoretically cause problems in 2039, but I think everything will just roll over and use 2039 as the new anchor date.

&nbsp;

## Inspiration

Please do tell me if you made something cool and I'll include it here.

### Arduino clock

Github user [BugerDread](https://github.com/BugerDread) made an [Arduino clock](https://github.com/BugerDread/esp8266-ezTIME-wifi-clock) using ESP8266, ezTIME and MAX7219 LED display modules. It's pretty:

![](images/ticker-and-ntpclock.jpg)

![](images/esp-ntp-clock.jpg)

&nbsp;

## ezTime on various Arduino platforms

If your Arduino has anything like normal Arduino networking, we can make it work. In some cases it might take an exception in the code if it needs a special header file or so, but no big deal. And if it has `EEPROM.h` or `Preferences.h` to store things in flash, we can make the cache work too. Please open an issue on [github](htttps://github.com/ropg/ezTime) to tell me if something doesn't work. Here's a list of boards that ezTime has been tested on.

### DSD Tech ESP8266

![](images/ESP8266.jpg)

ezTime 0.7.2 ran fine. Board: Generic ESP8266, Reset Method: nodemcu. Don't forget to replace `#include <WiFi.h>` with  `#include <ESP8266WiFi.h>` in your sketch.

&nbsp;

### Teensy 3.2

![](images/Teensy-3.2.jpg)

ezTime 0.7.2 ran fine. Did not test networking, so compiled with `#define EZTIME_NETWORK_ENABLE` commented out, used NoNetwork example.

&nbsp;

### Arduino Uno R3 (clone) with Ethernet Shield W5100

![](images/Uno-with-Ethernet.jpg)

ezTime 0.7.4 ran, the EthernetShield example leaves some 5k of flash:

```
Sketch uses 26536 bytes (82%) of program storage space. Maximum is 32256 bytes.
Global variables use 733 bytes (35%) of dynamic memory, leaving 1315 bytes for local variables. Maximum is 2048 bytes.
```

By setting `#define EZTIME_MAX_DEBUGLEVEL_NONE` in `eztime.h` we can free up some more flash:

```
Sketch uses 23870 bytes (74%) of program storage space. Maximum is 32256 bytes.
Global variables use 729 bytes (35%) of dynamic memory, leaving 1319 bytes for local variables. Maximum is 2048 bytes.
```

ezTime and NoNetwork example without `#define EZTIME_NETWORK_ENABLE` (if you have another time source and are willing to put in the Posix information for timezones yourself.):

```
Sketch uses 11558 bytes (35%) of program storage space. Maximum is 32256 bytes.
Global variables use 354 bytes (17%) of dynamic memory, leaving 1694 bytes for local variables. Maximum is 2048 bytes.
```

&nbsp;

### M5Stack (ESP32)

![](images/M5Stack.jpg)

ezTime 0.7.2 ran fine.

&nbsp;

### Arduino Micro

![](images/Arduino-Micro.jpg)

USB took a while to be recognized on my Mac, and then I took a while to discover that this is one that needs the

```
while (!Serial) { ; }		// wait for serial port to connect. Needed for native USB port only
```

line that you see in many sketches. But then ezTime 0.7.2 ran fine using NoNetwork example.

&nbsp;

### Arduino Due

![](images/Arduino-Due.jpg)

ezTime 0.7.2 runs fine (No networking on board, so tested with NoNetwork example). If you use the native USB port it also needs the `while (!Serial) { ; }` and you need to change all the `Serial.` to `SerialUSB.` in your sketch. Note that if you want debugging info you can pass the SerialUSB port as the second argument to `setDebug`.

&nbsp;

### Arduino MKR1000

![](images/Arduino-MKR-1000.jpg)

ezTime 0.7.2 worked, eventually. But I didn't like this one. Getting online is difficult. Install Wifi101 library from the library manager and make sure to start your sketch with:

```
#include <SPI.h>
#include <WiFi101.h>
```

* Test sketch complained about WiFi firmware / driver mismatch. Couldn't get the firmware update tool to work, but WiFi worked anyway.
* The WiFi object does not have the `isConnected` method so I wrote some detection for ezTime to skip the NO_NETWORK checks. This means that if you have debug level at ERROR or higher, waitForSync will throw some NTP TIMEOUT errors (and then continue just fine after wifi is online).
* It doesn't have `EEPROM.h` or `Preferences.h` but some proprietary `FlashStorage.h`. So no cache for the moment. (Turn off both cache defines at the beginning of `ezTime.h`. I'll write it if the third person wants it.

&nbsp;

### Arduino Nano

![](images/Arduino-Nano.jpg)

ezTime 0.7.2 runs fine (No networking on board, so tested with NoNetwork example)


&nbsp;

## Table of Contents

   * [ezTime, an Arduino library for all of time <sup>*</sup>](#eztime-an-arduino-library-for-all-of-time-)
      * [A brief history of ezTime](#a-brief-history-of-eztime)
      * [ezTime is ...](#eztime-is-)
         * [Timezones](#timezones)
         * [Formatted date and time](#formatted-date-and-time)
         * [milliseconds](#milliseconds)
         * [Rich information and <em>... oh my just look at these NTP updates</em>](#rich-information-and--oh-my-just-look-at-these-ntp-updates)
      * [Getting started](#getting-started)
   * [ezTime User Manual](#eztime-user-manual)
      * [About this manual](#about-this-manual)
         * [Semi-internal functions](#semi-internal-functions)
         * [Specifying time](#specifying-time)
      * [How it all works](#how-it-all-works)
         * [What happens when you include the library](#what-happens-when-you-include-the-library)
         * [No daemons here](#no-daemons-here)
         * [But I only just woke up !](#but-i-only-just-woke-up-)
      * [Setting and synchronising time](#setting-and-synchronising-time)
         * [timeStatus](#timestatus)
         * [waitForSync](#waitforsync)
         * [<em>setServer and setInterval</em>](#setserver-and-setinterval)
         * [<em>updateNTP</em>](#updatentp)
         * [<em>lastNtpUpdateTime</em>](#lastNtpUpdateTime)
         * [<em>queryNTP</em>](#queryntp)
      * [Timezones](#timezones-1)
         * [setDefault](#setdefault)
         * [setPosix](#setposix)
         * [getPosix](#getposix)
         * [isDST](#isdst)
         * [getTimezoneName](#gettimezonename)
         * [getOffset](#getoffset)
         * [setLocation](#setlocation)
         * [timezoned.rop.nl](#timezoned-rop-nl)
         * [Timezone caching, EEPROM or NVS](#timezone-caching-eeprom-or-nvs)
         * [setCache](#setcache)
         * [clearCache](#clearcache)
         * [Crazy timezones](#crazy-timezones)
            * [Chatham Islands and Nepal](#chatham-islands-and-nepal)
            * [Morocco](#morocco)
      * [Getting date and time](#getting-date-and-time)
         * [dateTime](#datetime)
         * [Built-in date and time formats](#built-in-date-and-time-formats)
         * [Time and date as numbers](#time-and-date-as-numbers)
         * [<em>weekISO and yearISO</em>](#weekiso-and-yeariso)
         * [<em>militaryTZ</em>](#militarytz)
         * [secondChanged and minuteChanged](#secondchanged-and-minutechanged)
         * [names of days and months](#names-of-days-and-months)
      * [Events](#events)
         * [events](#events-1)
         * [setEvent](#setevent)
         * [deleteEvent](#deleteevent)
      * [Setting date and time manually](#setting-date-and-time-manually)
         * [setTime](#settime)
         * [Alternate sources of time](#alternate-sources-of-time)
      * [Working with time values](#working-with-time-values)
         * [<em>breakTime</em>](#breaktime)
         * [makeTime](#maketime)
         * [<em>makeOrdinalTime</em>](#makeordinaltime)
         * [<em>compileTime</em>](#compiletime)
         * [<em>tzTime</em>](#tztime)
      * [Various functions](#various-functions)
         * [<em>zeropad</em>](#zeropad)
      * [Errors and debug information](#errors-and-debug-information)
         * [<em>setDebug</em>](#setdebug)
         * [<em>error</em>](#error)
         * [<em>errorString</em>](#errorstring)
      * [Compatibility with Arduino Time library](#compatibility-with-arduino-time-library)
      * [Smaller footprint, AVR Arduinos](#smaller-footprint-avr-arduinos)
      * [2036 and 2038](#2036-and-2038)
      * [Inspiration](#Inspiration)
         * [Arduino clock](#Arduino-clock)
      * [ezTime on various Arduino platforms](#eztime-on-various-arduino-platforms)
         * [DSD Tech ESP8266](#dsd-tech-esp8266)
         * [Teensy 3.2](#teensy-32)
         * [Arduino Uno R3 (clone) with Ethernet Shield W5100](#arduino-uno-r3-clone-with-ethernet-shield-w5100)
         * [M5Stack (ESP32)](#m5stack-esp32)
         * [Arduino Micro](#arduino-micro)
         * [Arduino Due](#arduino-due)
         * [Arduino MKR1000](#arduino-mkr1000)
         * [Arduino Nano](#arduino-nano)
      * [Table of Contents](#table-of-contents)
         * [Function reference](#function-reference)

&nbsp;

### Function reference

| function | returns | arguments | TZ prefix | network | cache |
|:---------|:--------|:----------|:----------|:--------|:------|
| [**`breakTime`**](#breaktime) | `void` | `time_t time`, `tmElements_t &tm` | no | no | no
| [**`clearCache`**](#clearcache) | `void` | `bool delete_section = false` | yes | yes | NVS
| [**`clearCache`**](#clearcache) | `void` | | yes | yes | EEPROM
| [**`compileTime`**](#compiletime) | `time_t` | `String compile_date = __DATE__`, `String compile_time = __TIME__` | no | no | no
| [**`dateTime`**](#datetime) | `String` | `TIME`, `String format = DEFAULT_TIMEFORMAT` | optional | no | no
| [**`day`**](#time-and-date-as-numbers) | `uint8_t` | `TIME` | optional | no | no
| [**`dayOfYear`**](#time-and-date-as-numbers) | `uint16_t` | `TIME` | optional | no | no
| [**`dayShortStr`**](#names-of-days-and-months) | `String` | `uint8_t day` | no | no | no
| [**`dayStr`**](#names-of-days-and-months) | `String` | `uint8_t day` | no | no | no
| [**`deleteEvent`**](#deleteevent) | `void` | `uint8_t event_handle` | no | no | no
| [**`deleteEvent`**](#deleteevent) | `void` | `void (`*function`)(``)` | no | no | no
| [**`error`**](#error) | `ezError_t` | `bool reset = false` | no | no | no
| [**`errorString`**](#errorstring) | `String` | `ezError_t err = LAST_ERROR` | no | no | no
| [**`events`**](#events) | `void` | | no | no | no
| [**`getOffset`**](#getoffset) | `int16_t` | `TIME` | optional | no | no
| **function** | **returns** | **arguments** | **TZ prefix** | **network** | **cache** |
| [**`getOlson`**](#getolson) | `String` | | optional | yes | yes |
| [**`getPosix`**](#getposix) | `String` | | yes | no | no
| [**`getTimezoneName`**](#gettimezonename) | `String` | `TIME` | optional | no | no
| [**`hour`**](#time-and-date-as-numbers) | `uint8_t` | `TIME` | optional | no | no
| [**`hourFormat12`**](#time-and-date-as-numbers) | `uint8_t` | `TIME` | optional | no | no
| [**`isAM`**](#time-and-date-as-numbers) | `bool` | `TIME` | optional | no | no
| [**`isDST`**](#isdst) | `bool` | `TIME` | optional | no | no
| [**`isPM`**](#time-and-date-as-numbers) | `bool` | `TIME` | optional | no | no
| [**`lastNtpUpdateTime`](#lastNtpUpdateTime) | `time_t` | | no | yes | no
| [**`makeOrdinalTime`**](#makeordinaltime) | `time_t` | `uint8_t hour`, `uint8_t minute`, `uint8_t second`, `uint8_t ordinal`, `uint8_t wday`, `uint8_t month`, `uint16_t year` | no | no | no
| [**`makeTime`**](#maketime) | `time_t` | `tmElements_t &tm` | no | no | no
| [**`makeTime`**](#maketime) | `time_t` | `uint8_t hour`, `uint8_t minute`, `uint8_t second`, `uint8_t day`, `uint8_t month`, `uint16_t year` | no | no | no
| [**`militaryTZ`**](#militarytz) | `String` | `TIME` | optional | no | no
| [**`minute`**](#time-and-date-as-numbers) | `uint8_t` | `TIME` | optional | no | no
| [**`minuteChanged`**](#secondchanged-and-minutechanged) | `bool` | | no | no | no
| [**`month`**](#time-and-date-as-numbers) | `uint8_t` | `TIME` | optional | no | no
| **function** | **returns** | **arguments** | **TZ prefix** | **network** | **cache** |
| [**`monthShortStr`**](#names-of-days-and-months) | `String` | `uint8_t month` | no | no | no
| [**`monthStr`**](#names-of-days-and-months) | `String` | `uint8_t month` | no | no | no
| [**`ms`**](#time-and-date-as-numbers) | `uint16_t` | `TIME_NOW` or `LAST_READ` | optional | no | no
| [**`now`**](#time-and-date-as-numbers) | `time_t` | | optional | no | no
| [**`queryNTP`**](#queryntp) | `bool` | `String server`, `time_t &t`, `unsigned long &measured_at` | no | yes | no
| [**`second`**](#time-and-date-as-numbers) | `uint8_t` | `TIME` | optional | no | no
| [**`secondChanged`**](#secondchanged-and-minutechanged) | `bool` | | no | no | no
| [**`setCache`**](#setcache) | `bool` | `String name`, `String key` | yes | yes | NVS
| [**`setCache`**](#setcache) | `bool` | `int16_t address` | yes | yes | EEPROM
| [**`setDebug`**](#setdebug) | `void` | `ezDebugLevel_t level` | no | no | no
| [**`setDebug`**](#setdebug) | `void` | `ezDebugLevel_t level`, `Print &device` | no | no | no
| [**`setDefault`**](#setdefault) | `void` | | yes | no | no
| [**`setEvent`**](#setevent) | `uint8_t` | `void (*function)()`, `TIME` | optional | no | no
| [**`setEvent`**](#setevent) | `uint8_t` | `void (*function)()`, `uint8_t hr`, `uint8_t min`, `uint8_t sec`, `uint8_t day`, `uint8_t mnth`, `uint16_t yr` | optional | no | no
| [**`setInterval`**](#setserver-and-setinterval) | `void` | `uint16_t seconds = 0` | | yes | no
| **function** | **returns** | **arguments** | **TZ prefix** | **network** | **cache** |
| [**`setLocation`**](#setlocation) | `bool` | `String location = ""` | yes | yes | no
| [**`setPosix`**](#setposix) | `bool` | `String posix` | yes | yes | no
| [**`setServer`**](#setserver-and-setinterval) | `void` | `String ntp_server = NTP_SERVER` | no | yes | no
| [**`setTime`**](#settime) | `void` | `time_t t`, `uint16_t ms = 0` | optional | no | no
| [**`setTime`**](#settime) | `void` | `uint8_t hr`, `uint8_t min`, `uint8_t sec`, `uint8_t day`, `uint8_t mnth`, `uint16_t yr` | optional | no | no
| [**`timeStatus`**](#timestatus) | `timeStatus_t` | | no | no | no
| [**`tzTime`**](#tztime) | `time_t` | `TIME` | yes | no | no
| [**`tzTime`**](#tztime) | `time_t` | `TIME`, `String &tzname`, `bool &is_dst`, `int16_t &offset` | yes | no | no
| [**`updateNTP`**](#updatentp) | `void` | | no | yes | no
| [**`waitForSync`**](#waitforsync) | `bool` | `uint16_t timeout = 0` | no | yes | no
| [**`weekISO`**](#weekiso-and-yeariso) | `uint8_t` | `TIME` | optional | no | no
| [**`weekday`**](#time-and-date-as-numbers) | `uint8_t` | `TIME` | optional | no | no
| [**`year`**](#time-and-date-as-numbers) | `uint16_t` | `TIME` | optional | no | no
| [**`yearISO`**](#weekiso-and-yeariso) | `uint16_t` | `TIME` | optional | no | no
| [**`zeropad`**](#zeropad) | `String` | `uint32_t number`, `uint8_t length` | no | no | no

