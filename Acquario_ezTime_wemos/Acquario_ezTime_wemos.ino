// Funzione "long" map per numeri 32 bit corretta come da Github
#include "long_Map.h"
// Funzione "float" map per numeri in virgola corretta come da Github
#include "mapFloat.h"

// NTP information
#include <ezTime.h>
uint32_t t;
uint32_t t1;
Timezone myTZ;
#define LOCALTZ_POSIX  "CET-1CEST,M3.4.0,M10.4.0/3"    // Time in Rome with DST indication

// Remote site information
#include <ESP8266WiFi.h>
const char* ssid     = "Michele";
const char* password = "bffmhl77b22b885a";

// Global connection variables
WiFiClient client;

// Fasi lunari
#include "MoonPhase.h"
/* include a MoonPhase instance */
MoonPhase MoonPhase;

/* Ephemeris */
#include <Ephemeris.h>
#define altitude 20 // Altitudine sul livello del mare
#define timezone -2 // Italia
#define twilight 0 // Italia
//float latitude  = 45.099325;
//float longitude = 8.551742;
//const char* latitude     = "45,05,50";
//const char* longitude = "8,33,10";

int hh, mn, ss, dd, mm, yy;

//Scala cromatica Kelvin
#include <Kelvin2RGB.h> // Conversione Tabella colori Kelvin in RGB

/* Libreria RTC DS1302 */
//#include "MyRealTimeClock.h"
/* CLK -> D4, DAT -> D3, RST -> D2 */
//MyRealTimeClock myRTC(16, 7, 6); // Assign Digital Pins
//MyRealTimeClock myRTC(15, 13, 12); // Assign Digital Pins

/* Definisci i pin utilizzati comando led rgb */

#define RED_SOLE D1 //Pin Sole Rosso
#define BLU_SOLE D2 //Pin Sole Verde
#define GRN_SOLE D3 //Pin Sole Blu
#define RED_LUNA D8 //Pin Luna Rosso
#define BLU_LUNA D7 //Pin Luna Verde
#define GRN_LUNA D6 //Pin Luna Blu

/* Segnalazione Connessione */
#define WIFI D4 //Pin Luna Blu

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for Serial port to connect. Needed for native USB port only
  }

  /* Imposta i pin di Arduino da utilizzare */

  /* Pin Sole */
  pinMode(RED_SOLE, OUTPUT);
  pinMode(BLU_SOLE, OUTPUT);
  pinMode(GRN_SOLE, OUTPUT);

  analogWrite(RED_SOLE, 0);
  analogWrite(GRN_SOLE, 0);
  analogWrite(BLU_SOLE, 0);

  /* Pin Luna */
  pinMode(RED_LUNA, OUTPUT);
  pinMode(BLU_LUNA, OUTPUT);
  pinMode(GRN_LUNA, OUTPUT);

  analogWrite(RED_LUNA, 0);
  analogWrite(GRN_LUNA, 0);
  analogWrite(BLU_LUNA, 0);

  /* Segnalazione Connessione */
  pinMode(WIFI, OUTPUT);

  digitalWrite(WIFI, LOW);

  // Initiate connection with SSID and PSK
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    Serial.print(F("Wifi? "));
    digitalWrite(WIFI, HIGH);
    delay(1000);
    digitalWrite(WIFI, LOW);
    delay(1000);
  }

  /*  ezTime option */
  waitForSync();
  myTZ.setPosix(LOCALTZ_POSIX);
  myTZ.setDefault();

  /*! Flipe longitude coordinate. Default: West is negative and East is positive. */
  Ephemeris::flipLongitude(true);
  /*! Set altitude in meters for location on earth (improve precision for rise and set). */
  Ephemeris::setAltitude(altitude);
  /*! Set location on earth (used for horizontal coordinates conversion). */
  Ephemeris::setLocationOnEarth(8, 33, 10,
                                45, 05, 50);

}

void luna()
{
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //Variabili locali
  int mrhours, mrminutes;
  float mrseconds;
  int mshours, msminutes;
  float msseconds;
  float tmoon;
  t = now();
  t = t - 3600UL; //Local time

  /* Struttura conversione da Unix Time */
  tmElements_t utc;
  breakTime(t, utc);
  hh = utc.Hour; // Returns the hour for the given time t
  mn = utc.Minute; // Returns the minute for the given time t
  ss = utc.Second; // Returns the second for the given time t
  dd = utc.Day; // The day for the given time t
  mm = utc.Month; // The month for the given time t
  yy = utc.Year; // The year for the given time t

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*! Compute solar system object for a specific date, time and location on earth (if location has been initialized first). */
  SolarSystemObject moon = Ephemeris::solarSystemObjectAtDateAndTime(EarthsMoon, dd, mm, yy, hh, mn, ss);

  /*! Convert floating hours to integer hours, minutes, seconds. */
  Ephemeris::floatingHoursToHoursMinutesSeconds(Ephemeris::floatingHoursWithUTCOffset(moon.rise, timezone), &mrhours, &mrminutes, &mrseconds);

  /*! Convert floating hours to integer hours, minutes, seconds. */
  Ephemeris::floatingHoursToHoursMinutesSeconds(Ephemeris::floatingHoursWithUTCOffset(moon.rise, timezone), &mshours, &msminutes, &msseconds);

  /*! Convert integer hours, minutes, seconds to floating hours. */
  tmoon = Ephemeris::hoursMinutesSecondsToFloatingHours(hh, mn, ss);

  /*    Avvio gestione fasi lunari  */
  MoonPhase::moonData luna = MoonPhase.getInfo( yy, mm, dd, hh); // Calcolo fase lunare da data

  /* Avvio gestione conversione gradi Kelvin in colori RGB secondo parabola oraria  */
  float lunamedia = mapFloat(tmoon, moon.rise, moon.set, 0.0, 3.1415);
  int lunatemp = (sin(lunamedia) * 8000.0);

  /* Avvio calcolo luminosità secondo parabola oraria  */
  int lunabrigh = mapFloat(luna.percentLit, 0.0, 0.999, 0.0, 100.0); // 7.5 luna piena -> 1 lux
  lunabrigh = lunatemp / (float)log(10000);

  /* Funzione convertitore Kelvin in RGB e luminosità */

  Kelvin2RGB Luna(lunatemp, lunabrigh);

  /* Assign modified values to the pwm outputs for each colour led */
  analogWrite(RED_LUNA, Luna.Red);
  analogWrite(GRN_LUNA, Luna.Green);
  analogWrite(BLU_LUNA, Luna.Blue);

  Serial.println("_____________________________________");
  Serial.print("  Moonrise: ");
  Serial.print(mrhours);
  Serial.print("h");
  Serial.print(mrminutes);
  Serial.print("m");
  Serial.print(mrseconds, 0);
  Serial.println("s");
  Serial.print(" ");
  Serial.println("Unix: ");
  Serial.print(moon.rise, 10);
  Serial.print("  Moonset: ");
  Serial.print(mshours);
  Serial.print("h");
  Serial.print(msminutes);
  Serial.print("m");
  Serial.print(msseconds, 0);
  Serial.println("s");
  Serial.print(" ");
  Serial.println("Unix: ");
  Serial.print(moon.set, 10);
  Serial.println("_____________________________________");
  Serial.println(" ");
  Serial.print("Moon KELVIN : ");
  Serial.print(lunatemp);
  Serial.println(" ");
  Serial.print("Moon LUMINOSITA : ");
  Serial.print(lunabrigh);
  Serial.println(" ");
  Serial.print("Moon Frac ");
  Serial.print(Luna.Red);
  Serial.print(" ");
  Serial.print(Luna.Green);
  Serial.print(" ");
  Serial.print(Luna.Blue);
  Serial.println("_____________________________________");

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void sole()
{
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //Variabili locali
  int srhours, srminutes;
  float srseconds;
  int sshours, ssminutes;
  float ssseconds;
  float tsun;
  t1 = now();
  t1 = t1 - 3600UL; //Local time

  /* Struttura conversione da Unix Time */
  tmElements_t utc;
  breakTime(t1, utc);
  hh = utc.Hour; // Returns the hour for the given time t
  mn = utc.Minute; // Returns the minute for the given time t
  ss = utc.Second; // Returns the second for the given time t
  dd = utc.Day; // The day for the given time t
  mm = utc.Month; // The month for the given time t
  yy = utc.Year; // The year for the given time t

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*! Compute solar system object for a specific date, time and location on earth (if location has been initialized first). */
  SolarSystemObject sun = Ephemeris::solarSystemObjectAtDateAndTime(Sun, dd, mm, yy, hh, mn, ss);

  /*! Convert floating hours to integer hours, minutes, seconds. */
  Ephemeris::floatingHoursToHoursMinutesSeconds(Ephemeris::floatingHoursWithUTCOffset(sun.rise, timezone), &srhours, &srminutes, &srseconds);

  /*! Convert floating hours to integer hours, minutes, seconds. */
  Ephemeris::floatingHoursToHoursMinutesSeconds(Ephemeris::floatingHoursWithUTCOffset(sun.set, twilight), &sshours, &ssminutes, &ssseconds);

  /*! Convert integer hours, minutes, seconds to floating hours. */
  tsun = Ephemeris::hoursMinutesSecondsToFloatingHours(hh, mn, ss);

  /* Avvio gestione conversione gradi Kelvin in colori RGB secondo parabola oraria  */
  float solemedia = mapFloat(tsun, sun.rise, sun.set, 0.0, 3.1415);
  int soletemp = (sin(solemedia) * 16000.0);
  int solebrigh = soletemp / (float)log(10000);

  /* Funzione convertitore Kelvin in RGB e luminosità */

  Kelvin2RGB Sole(soletemp, solebrigh);

  /* Assign modified values to the pwm outputs for each colour led */
  analogWrite(RED_SOLE, Sole.Red);
  analogWrite(GRN_SOLE, Sole.Green);
  analogWrite(BLU_SOLE, Sole.Blue);

  Serial.println("_____________________________________");
  Serial.print("  Sunrise: ");
  Serial.print(srhours);
  Serial.print("h");
  Serial.print(srminutes);
  Serial.print("m");
  Serial.print(srseconds, 0);
  Serial.println("s");
  Serial.println(" ");
  Serial.println("Unix: ");
  Serial.print(sun.rise);
  Serial.print("  Sunset: ");
  Serial.print(sshours);
  Serial.print("h");
  Serial.print(ssminutes);
  Serial.print("m");
  Serial.print(ssseconds, 0);
  Serial.println("s");
  Serial.println(" ");
  Serial.println("Unix: ");
  Serial.print(sun.set);
  Serial.println("_____________________________________");
  Serial.println(" ");
  Serial.print("Sun KELVIN : ");
  Serial.print(soletemp);
  Serial.println(" ");
  Serial.print("Sun LUMINOSITA : ");
  Serial.print(solebrigh);
  Serial.println(" ");
  Serial.print("Sun Frac ");
  Serial.print(Sole.Red);
  Serial.print(" ");
  Serial.print(Sole.Green);
  Serial.print(" ");
  Serial.print(Sole.Blue);
  Serial.println("_____________________________________");
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void loop() {

  events();
  luna();
  sole();

}
