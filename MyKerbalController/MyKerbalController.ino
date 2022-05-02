  /*
 Name:		MyKerbalController.ino
 Created:	4/19/2022 3:07:09 PM
 Author:	morgoth
*/

#include <Bounce2.h>
#include <KerbalSimpit.h>
#include <avr/wdt.h>

//Analog pins
const int pTHROTTLE = A8; //slide pot
const int pTX = A2;       //translation x-axis
const int pTY = A1;       //translation y-axis
const int pTZ = A0;       //translation z-axis
const int pRX = A6;       //rotation x-axis
const int pRY = A5;       //rotation y-axis
const int pRZ = A4;       //rotation z-axis

//Digital pins
const int pPOWER = 53;      //power switch
const int pTB = 57;         //translation joystick button
const int pRB = 61;         //rotation joystick button
const int latchPin = 5;     //ST_CP - green
const int dataPin = 7;      //DS - yellow
const int clockPin = 6;     //SH_CP - blue
const int sasClockPin = 21; //SAS Selector Clock Pin Connector
const int sasDataPin = 20;  //SAS Selector Data Pin Connector
const int pSASBTN = 19;      //SAS Selector Switch
const int sasLEDLatch = 4;  //SAS LED indicator latch pin (Latch and Data pins inverted on controller)
const int sasLEDData = 2;   //SAS LED indicator data pin (Latch and Data pins inverted on controller)
const int sasLEDClock = 3;  //SAS LED indicator clock pin
const int pMODE = 69;       //mode switch (used for debug mode)
const int pLCDx = 39;       //toggle switch x (used for LCD display modes)
const int pLCDy = 41;       //toggle switch y (used for LCD display modes)
const int pLCDz = 43;       //toggle switch z (used for LCD display modes)
const int pSAS = 63;        //SAS switch
const int pRCS = 64;        //RCS switch
const int pABORT = 8;       //Abort switch (safety switch, active high)
const int pABORTBTN = 10;   //Abort button
const int pABORTBTNLED = 9; //Abort button LED
const int pARM = 11;        //Arm switch (safety switch, active high)
const int pSTAGE = 13;      //Stage button
const int pSTAGELED = 12;   //Stage button LED
const int pLIGHTS = 26;     //Lights button
const int pLIGHTSLED = 34;  //Lights button LED
const int pLADDER = 28;     //Ladder button (action group 8)
const int pLADDERLED = 36;  //Ladder button LED
const int pSOLAR = 30;      //Solar button (action group 9)
const int pSOLARLED = 38;   //Solar button LED
const int pCHUTES = 32;     //Chutes button (action group 10)
const int pCHUTESLED = 40;  //Chutes button LED
const int pGEARS = 22;      //Gears button
const int pGEARSLED = 42;   //Gears button LED
const int pBRAKES = 24;     //Brakes button
const int pBRAKESLED = 44;  //Brakes button LED
const int pACTION1 = 23;    //Action Group 1 button
const int pACTION1LED = 31; //Action Group 1 button LED
const int pACTION2 = 25;    //Action Group 2 button
const int pACTION2LED = 33; //Action Group 2 button LED
const int pACTION3 = 27;    //Action Group 3 button
const int pACTION3LED = 35; //Action Group 3 button LED
const int pACTION4 = 29;    //Action Group 4 button
const int pACTION4LED = 37; //Action Group 4 button LED
const int pACTION5 = 45;    //Action Group 5 button
const int pACTION5LED = 46; //Action Group 5 button LED
const int pACTION6 = 47;    //Action Group 6 button
const int pACTION6LED = 48; //Action Group 6 button LED
const int pACTION7 = 49;    //Action Group 7 button
const int pACTION7LED = 50; //Action Group 7 button LED
const int pCAMMODEUP = 65;  //View mode toggle switch up
const int pCAMMODEDOWN = 66;//View mode toggle switch down
const int pSASONLED = 67;   //SAS On LED indicator
const int pRCSONLED = 68;   //RCS On LED indicator

//Setup Bounce2 objects
Bounce2::Button b_TB = Bounce2::Button();
Bounce2::Button b_RB = Bounce2::Button();
Bounce2::Button b_SASBTN = Bounce2::Button();
Bounce2::Button b_ABORTBTN = Bounce2::Button();
Bounce2::Button b_STAGE = Bounce2::Button();
Bounce2::Button b_LIGHTS = Bounce2::Button();
Bounce2::Button b_LADDER = Bounce2::Button();
Bounce2::Button b_SOLAR = Bounce2::Button();
Bounce2::Button b_CHUTES = Bounce2::Button();
Bounce2::Button b_GEARS = Bounce2::Button();
Bounce2::Button b_BRAKES = Bounce2::Button();
Bounce2::Button b_ACTION1 = Bounce2::Button();
Bounce2::Button b_ACTION2 = Bounce2::Button();
Bounce2::Button b_ACTION3 = Bounce2::Button();
Bounce2::Button b_ACTION4 = Bounce2::Button();
Bounce2::Button b_ACTION5 = Bounce2::Button();
Bounce2::Button b_ACTION6 = Bounce2::Button();
Bounce2::Button b_ACTION7 = Bounce2::Button();
Bounce2::Button b_CAMMODEUP = Bounce2::Button();
Bounce2::Button b_CAMMODEDOWN = Bounce2::Button();


//In-game state used to update button LEDs
bool lights_on = false;
bool ladder_on = false;
bool solar_on = false;
bool gears_on = false;
bool brakes_on = false;
bool chutes_on = false;
bool stage_on = false;
bool abort_on = false;
bool action1_on = false;
bool action2_on = false;
bool action3_on = false;
bool action4_on = false;
bool action5_on = false;
bool action6_on = false;
bool action7_on = false;

//SAS and RCS LEDs state
bool sas_led_on = false;
bool rcs_led_on = false;

//Toggle button states
bool tb_on = true;
bool rb_on = true;
bool sasb_on = true;

//Previous button state
bool tb_prev = false;
bool rb_prev = false;
bool sasb_prev = false;

//Stage and Abort LEDs state
bool stage_led_on = false;
bool abort_led_on = false;

//Input value variables
int throttle_value;
int tx_value;
int ty_value;
int tz_value;
int rx_value;
int ry_value;
int rz_value;

//Vessel view mode variable
byte cameraMode;

//SAS mode variables
int temp_sas_mode = 255;
int sas_mode = 255;
int16_t sas_available_modes = 1023;
int16_t bit_sas;
int prev_sas_mode = 255;
bool sas_is_on = false;
bool rcs_is_on = false;

//Store old Throttle and joysticks values to avoid sending unnecessary data
int16_t old_throttle;

//SAS rotary encoder variables
int sasPreviousCLK;
int sasPreviousDATA;
int sasCurrentCLK;
long sasTimeOfLastDebounce = 0;
int sasDelayofDebounce = 30;

//SAS LEDs variable
byte sasInputBytes[2];

//Variables used for fuel guages
byte inputBytes[10];
int vSF, vLF, vOX, vEL, vMP, vXE, vLI, vXX, SF, LF, OX, EL, MP, XE, LI, XX;

//Keep track of how often to update LCD
int lcd_update_timer = 0;

//Keep Vessel Data in an object
struct VesselData
{
    float AP;                       //2   apoapsis (m to sea level)
    float PE;                       //3   periapsis (m to sea level)
    float SemiMajorAxis;            //4   orbit semi major axis (m)
    float SemiMinorAxis;            //5   orbit semi minor axis (m)
    float VVI;                      //6   vertical velocity (m/s)
    float e;                        //7   orbital eccentricity (unitless, 0 = circular, > 1 = escape)
    float inc;                      //8   orbital inclination (degrees)
    //float G;                        //9   acceleration (Gees)
    byte TWI;                       //XX  Thrust to Weight Index
    int32_t TAp;                    //10  time to AP (seconds)
    int32_t TPe;                    //11  time to PE (seconds)
    float PEArg;                    //XX  argument of periapsis (degrees)
    float TrueAnomaly;              //12  orbital true anomaly (degrees)
    float MeanAnomaly;              //XX  orbital mean anomaly (degrees)
    float LongAscNode;              //XX  Longitude of Ascending Node
    float Density;                  //13  air density (presumably kg/m^3, 1.225 at sea level)
    long period;                    //14  orbital period (seconds)
    float RAlt;                     //15  radar altitude (m)
    float Alt;                      //16  altitude above sea level (m)
    float Vsurf;                    //17  surface velocity (m/s)
    //float Lat;                      //18  Latitude (degree)
    //float Lon;                      //19  Longitude (degree)
    //float LiquidFuelTot;            //20  Liquid Fuel Total
    //float LiquidFuel;               //21  Liquid Fuel remaining
    float LiquidFuelSTot;            //20  Liquid Fuel Total
    float LiquidFuelS;               //21  Liquid Fuel remaining
    //float OxidizerTot;              //22  Oxidizer Total
    //float Oxidizer;                 //23  Oxidizer remaining
    float ecTotal;                  //24  Electric Charge Total
    float ecAvailable;              //25  Electric Charge remaining
    float monoPropTotal;            //26  Mono Propellant Total
    float monoPropAvailable;        //27  Mono Propellant remaining
    //float IntakeAirTot;             //28  Intake Air Total
    //float IntakeAir;                //29  Intake Air remaining
    float SolidFuelTot;             //30  Solid Fuel Total
    float SolidFuel;                //31  Solid Fuel remaining
    //float solidFuelSAvailable;      //XX  Solid Fuel Remaining (stage)
    //float solidFuelSTotal;          //XX  Solid Fuel Total (stage)
    float customResource1Available; //XX  Custom Resource 1 Available (Lithium)
    float customResource1Total;     //XX  Custom Resource 1 Total (Lithium)
    float customResource2Available; //XX  Custom Resource 2 Available (Air)
    float customResource2Total;     //XX  Custom Resource 2 Total (Air)
    float xenonGasTotal;           //32  Xenon Gas Total
    float xenonGasAvailable;       //33  Xenon Gas remaining
    //float liquidFuelSTotal;         //34  Liquid Fuel Total (stage)
    //float liquidFuelSAvailable;     //35  Liquid Fuel remaining (stage)
    float oxidizerSTotal;           //36  Oxidizer Total (stage)
    float oxidizerSAvailable;       //37  Oxidizer remaining (stage)
    //float Lithium;              //38  Lithium Fuel Remaining
    //float LithiumTot;           //39  Lithium Fuel Total
    //uint32_t MissionTime;           //40  Time since launch (s)
    //float deltaTime;                //41  Time since last packet (s)
    float VOrbit;                   //42  Orbital speed (m/s)
    float MNTime;                   //43  Time to next node (s) [0 when no node]
    float MNDeltaV;                 //44  Delta V for next node (m/s) [0 when no node]
    float MNDuration;               //XX  Duration of next maneuver node (s)
    float MNDeltaVTotal;            //XX  Total DeltaV of planned nodes 
    float StageDeltaV;              //XX  Delta V in current stage
    float TotalDeltaV;              //XX  Delta V in vessel
    float Pitch;                    //45  Vessel Pitch relative to surface (degrees)
    float Roll;                     //46  Vessel Roll relative to surface (degrees)
    float Heading;                  //47  Vessel Heading relative to surface (degrees)
    //uint16_t ActionGroups;          //48  status bit order:SAS, RCS, Light, Gears, Brakes, Abort, Custom01 - 10
    //byte SOINumber;                 //49  SOI Number (decimal format: sun-planet-moon e.g. 130 = kerbin, 131 = mun)
    char SOI;                       //XX  English name of the body being orbited
    byte MaxOverHeat;               //50  Max part overheat (% percent)
    byte MaxSkinOverheat;           //XX  Max part skin overheat (% percent)
    float MachNumber;               //51  Mach number
    float IAS;                      //52  Indicated Air Speed
    byte CurrentStage;              //53  Current stage number
    //byte TotalStage;                //54  TotalNumber of stages
    float TargetDist;               //55  Distance to targeted vessel (m)
    float TargetV;                  //56  Target vessel relative velocity
    float TargetPitch;              //XX  Target Pitch relative
    float TargetHeading;            //XX  Target heading relative
    float TargetPitchV;             //XX  Target Pitch velocity relative
    float TargetHeadingV;           //XX  Target heading velocity relative
    //float DeltaVActual;             //57  Vessel DeltaV in flight
    float BurnTime;                 //58  Vessel Total burn time
    float BurnTimeS;                //XX  Stage burn time
    //byte CameraMode;                //59  Current Vessel Camera Mode
    byte Temp;                      //Temp assignment of values
    //No longer in use
    //byte NavballSASMode;    //60  Combined byte for navball target mode and SAS mode
                                // First four bits indicate AutoPilot mode:
                                // 0 SAS is off  //1 = Regular Stability Assist //2 = Prograde
                                // 3 = RetroGrade //4 = Normal //5 = Antinormal //6 = Radial In
                                // 7 = Radial Out //8 = Target //9 = Anti-Target //10 = Maneuver node
                                // Last 4 bits set navball mode. (0=ignore,1=ORBIT,2=SURFACE,3=TARGET)
};

//create an instance of a VesselData object
VesselData VData;

//debug variable
bool debug = false;
bool serial_initialized = false;

//Timing
const int IDLETIMER = 20000;        //if no message received from KSP for more than 20s, go idle (default 2000)
const int CONTROLREFRESH = 10;      //send control packet every 10 ms (default 25)
const int stageLedBlinkTime = 500;  //blink staging LED when armed every 500 ms
const int abortLedBlinkTime = 500;  //blink abort LED when armed every 500 ms

//Variables used in timing
unsigned long deadtime, deadtimeOld, controlTime, controlTimeOld, stageLedTime, stageLedTimeOld, abortLedTime, abortLedTimeOld;
unsigned long now;


// the following variables are unsigned long's because the time, measured
// in miliseconds, will quickly become a bigger number than can be stored
// in an int.
//unsigned long lastDebounceTime = 0;  // the last time the output pin
                                     // was toggled
//unsigned long debounceDelay = 50;    // the debounce time; increase
                                     // if the output flickers

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

void reboot() {
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while (1) {}
}

void setup() {

    Serial.begin(115200);
    Serial2.begin(9600); //LCD connection
    setHighLCDRate();
    delay(200);
    Serial2.begin(38400);
    delay(500);           //wait for LCD boot

    //write to LCD
    clearLCD();
    writeLCD("KerbalController");
    jumpToLineTwo();
    writeLCD("booting...");
    delay(100);

    //Initialize
    controlsInit();   //set all pin modes and rotation of sas rotary switch
    testLEDS(100);     //blink every LED once to test (with a delay of 100 ms)

    sasPreviousCLK = digitalRead(sasClockPin);
    sasPreviousDATA = digitalRead(sasDataPin);

    //Check initial debug mode
    if (!digitalRead(pMODE)) { debug = true; }
    else
    {
        debug = false;
    }

    if (!debug) {
        // This loop continually attempts to handshake with the plugin.
        // It will keep retrying until it gets a successful handshake.
        while (!mySimpit.init()) {
            delay(100);
            clearLCD();
            writeLCD("KerbalController");
            jumpToLineTwo();
            writeLCD("connecting...");
        }
        serial_initialized = true;
        //InitTxPackets();  //initialize the serial communication
        
        // Sets our callback function. The KerbalSimpit library will
        // call this function every time a packet is received.
        mySimpit.inboundHandler(messageHandler);
        // Send a message to the plugin registering for the Action status channel.
        // The plugin will now regularly send Action status  messages while the
        // flight scene is active in-game.
        mySimpit.registerChannel(ACTIONSTATUS_MESSAGE);
        // Send registration for Custom Action Groups
        mySimpit.registerChannel(CAGSTATUS_MESSAGE);
        // Send registration for SAS Info
        mySimpit.registerChannel(SAS_MODE_INFO_MESSAGE);
        // Send registration for Fuel types
        //mySimpit.registerChannel(LF_MESSAGE);
        mySimpit.registerChannel(LF_STAGE_MESSAGE);
        mySimpit.registerChannel(SF_MESSAGE);
        //mySimpit.registerChannel(OX_MESSAGE);
        mySimpit.registerChannel(OX_STAGE_MESSAGE);
        mySimpit.registerChannel(ELECTRIC_MESSAGE);
        mySimpit.registerChannel(MONO_MESSAGE);
        mySimpit.registerChannel(XENON_GAS_MESSAGE);
        //Register for altitude,AP/PE,velocity and time to AP/PE info
        mySimpit.registerChannel(ALTITUDE_MESSAGE);
        mySimpit.registerChannel(APSIDES_MESSAGE);
        mySimpit.registerChannel(VELOCITY_MESSAGE);
        mySimpit.registerChannel(APSIDESTIME_MESSAGE);
        mySimpit.registerChannel(ORBIT_MESSAGE);
        mySimpit.registerChannel(AIRSPEED_MESSAGE);
        //mySimpit.registerChannel(DELTAV_MESSAGE);
        mySimpit.registerChannel(TEMP_LIMIT_MESSAGE);
        //mySimpit.registerChannel(ATMO_CONDITIONS_MESSAGE);
        mySimpit.registerChannel(MANEUVER_MESSAGE);
        mySimpit.registerChannel(TARGETINFO_MESSAGE);
        mySimpit.registerChannel(CUSTOM_RESOURCE_1_MESSAGE);
        /* Economizing subscriptions
        mySimpit.registerChannel(FLIGHT_STATUS_MESSAGE);
        mySimpit.registerChannel(SOI_MESSAGE);
        mySimpit.registerChannel(ROTATION_DATA);
        mySimpit.registerChannel(BURNTIME_MESSAGE);
         */
    }
}

void loop() {

    //Check mode
    if (!digitalRead(pMODE)) { debug = true; }
    else
    {
	    debug = false;
        if (!debug && !serial_initialized) { reboot(); }
    }

    if (debug) {
        //Debug mode does not communicate with KSPSerialIO, but simply displays the button states on the LCD.
        //Select analog input using LCDx/y/z. Xyz = Throttle. xYz = Translation. xyZ = Rotation.

        //Previous state tracking only used in debug
        bool stage_prev = false;
        bool abort_prev = false;
        bool chutes_prev = false;
        bool action1_prev = false;
        bool action2_prev = false;
        bool action3_prev = false;
        bool action4_prev = false;
        bool action5_prev = false;
        bool action6_prev = false;
        bool action7_prev = false;
        bool lights_prev = false;
        bool ladder_prev = false;
        bool solar_prev = false;
        bool gears_prev = false;
        bool brakes_prev = false;

        //Clear the LCD 
        clearLCD();

        //Toggle switches
        if (digitalRead(pSAS)) { writeLCD("S"); sas_led_on = true; }
        else { writeLCD("s"); sas_led_on = false; }
        if (digitalRead(pRCS)) { writeLCD("R"); rcs_led_on = true; }
        else { writeLCD("r"); rcs_led_on = false; }
        if (digitalRead(pABORT)) { writeLCD("A"); }
        else { writeLCD("a"); } //note abort switch is active high
        if (digitalRead(pARM)) { writeLCD("A"); }
        else { writeLCD("a"); }   //note arm switch is active high
        digitalWrite(pSASONLED, sas_led_on);
        digitalWrite(pRCSONLED, rcs_led_on);
        //In debug mode, handle all buttons as toggle buttons

        if (!digitalRead(pSTAGE) && !stage_prev) { stage_on = !stage_on; stage_prev = true; }
        if (digitalRead(pSTAGE) && stage_prev) { stage_prev = false; }
        if (stage_on) { writeLCD("S"); }
        else { writeLCD("s"); }
        digitalWrite(pSTAGELED, stage_on);

        if (!digitalRead(pABORTBTN) && !abort_prev) { abort_on = !abort_on; abort_prev = true; }
        if (digitalRead(pABORTBTN) && abort_prev) { abort_prev = false; }
        if (abort_on) { writeLCD("A"); }
        else { writeLCD("a"); }
        digitalWrite(pABORTBTNLED, abort_on);

        if (!digitalRead(pLIGHTS) && !lights_prev) { lights_on = !lights_on; lights_prev = true; }
        if (digitalRead(pLIGHTS) && lights_prev) { lights_prev = false; }
        if (lights_on) { writeLCD("L"); }
        else { writeLCD("l"); }
        digitalWrite(pLIGHTSLED, lights_on);

        if (!digitalRead(pLADDER) && !ladder_prev) { ladder_on = !ladder_on; ladder_prev = true; }
        if (digitalRead(pLADDER) && ladder_prev) { ladder_prev = false; }
        if (ladder_on) { writeLCD("L"); }
        else { writeLCD("l"); }
        digitalWrite(pLADDERLED, ladder_on);

        if (!digitalRead(pSOLAR) && !solar_prev) { solar_on = !solar_on; solar_prev = true; }
        if (digitalRead(pSOLAR) && solar_prev) { solar_prev = false; }
        if (solar_on) { writeLCD("S"); }
        else { writeLCD("s"); }
        digitalWrite(pSOLARLED, solar_on);

        if (!digitalRead(pCHUTES) && !chutes_prev) { chutes_on = !chutes_on; chutes_prev = true; }
        if (digitalRead(pCHUTES) && chutes_prev) { chutes_prev = false; }
        if (chutes_on) { writeLCD("C"); }
        else { writeLCD("c"); }
        digitalWrite(pCHUTESLED, chutes_on);

        if (!digitalRead(pGEARS) && !gears_prev) { gears_on = !gears_on; gears_prev = true; }
        if (digitalRead(pGEARS) && gears_prev) { gears_prev = false; }
        if (gears_on) { writeLCD("G"); }
        else { writeLCD("g"); }
        digitalWrite(pGEARSLED, gears_on);

        if (!digitalRead(pBRAKES) && !brakes_prev) { brakes_on = !brakes_on; brakes_prev = true; }
        if (digitalRead(pBRAKES) && brakes_prev) { brakes_prev = false; }
        if (brakes_on) { writeLCD("B"); }
        else { writeLCD("b"); }
        digitalWrite(pBRAKESLED, brakes_on);

        if (!digitalRead(pACTION1) && !action1_prev) { action1_on = !action1_on; action1_prev = true; }
        if (digitalRead(pACTION1) && action1_prev) { action1_prev = false; }
        if (action1_on) { writeLCD("A"); }
        else { writeLCD("a"); }
        digitalWrite(pACTION1LED, action1_on);

        if (!digitalRead(pACTION2) && !action2_prev) { action2_on = !action2_on; action2_prev = true; }
        if (digitalRead(pACTION2) && action2_prev) { action2_prev = false; }
        if (action2_on) { writeLCD("A"); }
        else { writeLCD("a"); }
        digitalWrite(pACTION2LED, action2_on);

        if (!digitalRead(pACTION3) && !action3_prev) { action3_on = !action3_on; action3_prev = true; }
        if (digitalRead(pACTION3) && action3_prev) { action3_prev = false; }
        if (action3_on) { writeLCD("A"); }
        else { writeLCD("a"); }
        digitalWrite(pACTION3LED, action3_on);

        if (!digitalRead(pACTION4) && !action4_prev) { action4_on = !action4_on; action4_prev = true; }
        if (digitalRead(pACTION4) && action4_prev) { action4_prev = false; }
        if (action4_on) { writeLCD("A"); }
        else { writeLCD("a"); }
        digitalWrite(pACTION4LED, action4_on);

        if (!digitalRead(pACTION5) && !action5_prev) { action5_on = !action5_on; action5_prev = true; }
        if (digitalRead(pACTION5) && action5_prev) { action5_prev = false; }
        if (action5_on) { writeLCD("A"); }
        else { writeLCD("a"); }
        digitalWrite(pACTION5LED, action5_on);

        if (!digitalRead(pACTION6) && !action6_prev) { action6_on = !action6_on; action6_prev = true; }
        if (digitalRead(pACTION6) && action6_prev) { action6_prev = false; }
        if (action6_on) { writeLCD("A"); }
        else { writeLCD("a"); }
        digitalWrite(pACTION6LED, action6_on);

        if (!digitalRead(pACTION7) && !action7_prev) { action7_on = !action7_on; action7_prev = true; }
        if (digitalRead(pACTION7) && action7_prev) { action7_prev = false; }
        if (action7_on) { writeLCD("A"); }
        else { writeLCD("a"); }
        digitalWrite(pACTION7LED, action7_on);

        if (!digitalRead(pTB) && !tb_prev) { tb_on = !tb_on; tb_prev = true; }
        if (digitalRead(pTB) && tb_prev) { tb_prev = false; }
        if (tb_on) { writeLCD("T"); }
        else { writeLCD("t"); }

        if (!digitalRead(pRB) && !rb_prev) { rb_on = !rb_on; rb_prev = true; }
        if (digitalRead(pRB) && rb_prev) { rb_prev = false; }
        if (rb_on) { writeLCD("R"); }
        else { writeLCD("r"); }

        //prepare the shift register
        digitalWrite(dataPin, LOW);
        digitalWrite(clockPin, LOW);
        digitalWrite(latchPin, LOW);

        inputBytes[0] = B11111111;
        inputBytes[1] = B11111111;
        inputBytes[2] = B11111111;
        inputBytes[3] = B11111111;
        inputBytes[4] = B11111111;
        inputBytes[5] = B11111111;
        inputBytes[6] = B11111111;
        inputBytes[7] = B11111111;
        inputBytes[8] = B11111111;
        inputBytes[9] = B11111111;
        //loop through the input bytes
        for (int j = 0; j <= 9; j++) {
            byte inputByte = inputBytes[j];
            shiftOut(dataPin, clockPin, MSBFIRST, inputByte);
        }

        //latch the values in when done shifting
        digitalWrite(latchPin, HIGH);

        //prepare the SAS LEDs shift register
        digitalWrite(sasLEDLatch, LOW);
        digitalWrite(sasLEDData, LOW);
        digitalWrite(sasLEDClock, LOW);

        sasInputBytes[0] = B11111111;
        sasInputBytes[1] = B11111111;

        //loop through the SAS LED input bytes
        for (int j = 0; j <= 1; j++) {
            byte sasInputByte = sasInputBytes[j];
            shiftOut(sasLEDData, sasLEDClock, MSBFIRST, sasInputByte);
        }

        //latch the values in when done shifting
        digitalWrite(sasLEDLatch, HIGH);

        //Analog inputs
        if (!digitalRead(pLCDx) && digitalRead(pLCDy) && digitalRead(pLCDz)) {
            throttle_value = analogRead(pTHROTTLE);
            char throttle_char[5];
            itoa(throttle_value, throttle_char, 10);
            writeLCD(throttle_char);
            writeLCD(" ");
        }
        if (digitalRead(pLCDx) && !digitalRead(pLCDy) && digitalRead(pLCDz)) {
            tx_value = analogRead(pTX);
            char tx_char[5];
            itoa(tx_value, tx_char, 10);
            writeLCD(tx_char);
            writeLCD(" ");
            ty_value = analogRead(pTY);
            char ty_char[5];
            itoa(ty_value, ty_char, 10);
            writeLCD(ty_char);
            writeLCD(" ");
            tz_value = analogRead(pTZ);
            char tz_char[5];
            itoa(tz_value, tz_char, 10);
            writeLCD(tz_char);
            writeLCD(" ");
        }

        if (digitalRead(pLCDx) && digitalRead(pLCDy) && !digitalRead(pLCDz)) {
            rx_value = analogRead(pRX);
            char rx_char[5];
            itoa(rx_value, rx_char, 10);
            writeLCD(rx_char);
            writeLCD(" ");
            ry_value = analogRead(pRY);
            char ry_char[5];
            itoa(ry_value, ry_char, 10);
            writeLCD(ry_char);
            writeLCD(" ");
            rz_value = analogRead(pRZ);
            char rz_char[5];
            itoa(rz_value, rz_char, 10);
            writeLCD(rz_char);
        }
        //End debug mode
    }
    else {

        //KSP mode
        //Send and Receive data
        mySimpit.update();
        get_vessel_data();
        //send_control_data();
        check_and_send_data();
    }
}
