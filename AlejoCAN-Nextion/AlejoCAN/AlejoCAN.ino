
#include <SPI.h>
//GPS
#include <TinyGPSPlus.h>

// CAN receiving
#include <mcp2515.h>

//EGT
#define MAX6675_CS   50
#define MAX6675_SO   52
#define MAX6675_SCK  48

struct can_frame canMsg;
struct can_frame canMsg1;
MCP2515 mcp2515(9);  // SPI CS Pin 9

// The TinyGPSPlus object
TinyGPSPlus gps;
//int cantxValue = 0;
int SPEED; //GPS Variables

// megasquirt variables
int MAP, RPM, TPS, CLT, BATT, BARO, MAT, EGO1, EGO2, EGOC, FUELP, OILP, OILT, KNOCK, ETH, VVT, VSS1, SENSORS1, SENSORS2, SENSORS3, SENSORS4, SENSORS5, SENSORS6, SENSORS7, SENSORS8, SENSORS9, SENSORS10, SENSORS11, SENSORS12, SENSORS13, SENSORS14, SENSORS15, SENSORS16;

// can variables
unsigned long previousMillis = 0;
int delayPeriod = 1000;


void setup() {
  // Serial INIT
  Serial.begin(115200); //Send CAN Data to Serial Monitor
  Serial1.begin(115200); //Nextion Screen
  Serial2.begin(115200); //GPS

  //CAN INIT
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();
  
}

void loop() {  

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");
    
    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
      Serial.println(canMsg.data[i],HEX);
      Serial.print(" ");
    }
  }
//Create CAN Message
  uint8_t data[8];
  float speed = gps.speed.mph();
  //uint32_t gpsSpeed = *(uint32_t*)(&speed2);  


//CAN Messages for Sensor 4,5 and 6
  canMsg1.can_id  = 0x001; //ECU ID
  canMsg1.can_dlc = 8; // Bits
  canMsg1.data[1] = speed; //Sensor 1
  //canMsg1.data[3] = 51; //Sensor 2
  //canMsg1.data[5] = 17;  //Sensor 3
   mcp2515.sendMessage(&canMsg1); //Send Data to CAN Bus


while (Serial2.available() > 0) // Read GPS ninfo
    gps.encode(Serial2.read());

   {
    switch (canMsg.can_id) {
      case 1520:  // group 0 - RPM, ECU Time
        //SECONDS = (float)(word(canMsg.data[0], canMsg.data[1]));
        //PW1 = (float)(word(canMsg.data[2], canMsg.data[3]));
        //PW2 = (float)(word(canMsg.data[4], canMsg.data[5]));
        RPM = (float)(word(canMsg.data[6], canMsg.data[7]));
        break;

      case (1520 + 1):  // AFR Target
        //ADV_DEG = (float)(word(canMsg.data[0], canMsg.data[1]));
        //SQUIRT = (float)(word(canMsg.data[2]));
        //ENGINE = (float)(word(canMsg.data[3]));
        //AFRTGT1 = (float)(word(canMsg.data[4]));
        //AFRTGT1 = (AFRTGT1 / 10);
        //AFRTGT2 = (float)(word(canMsg.data[5]));
        //AFRTGT2 = (AFRTGT2 / 10);

      case (1520 + 2):  // BARO, MAT, CLT,
        BARO = (float)(word(canMsg.data[0], canMsg.data[1]));
        BARO = (BARO / 10);
        MAP = (float)(word(canMsg.data[2], canMsg.data[3]));
        MAP = (MAP / 10 );
        MAT = (float)(word(canMsg.data[4], canMsg.data[5]));
        MAT = (MAT / 10);
        CLT = (float)(word(canMsg.data[6], canMsg.data[7]));
        CLT = (CLT / 10);
        break;

      case (1520 + 3):  // TPS, BATT, EGO
        TPS = (float)(word(canMsg.data[0], canMsg.data[1]));
        TPS = (TPS / 10);
        BATT = (float)(word(canMsg.data[2], canMsg.data[3]));
        BATT = (BATT / 10);
        EGO1 = (float)(word(canMsg.data[4], canMsg.data[5]));
        EGO1 = (EGO1 / 10);
        //EGO2 = (float)(word(canMsg.data[6], canMsg.data[7]));
        //EGO2 = (EGO2 / 10);
        break;

      case (1520 + 4):  // EGO Correction / Knock Sensor
        KNOCK = (float)(word(canMsg.data[0], canMsg.data[1]));
        KNOCK = (KNOCK / 10);      
        //EGOC1 = (float)(word(canMsg.data[2], canMsg.data[3]));
        //EGOC1 = (EGOC1 / 10);
        //EGOC2 = (float)(word(canMsg.data[4], canMsg.data[5]));
        //EGOC2 = (EGOC2 / 10);        
        //AIRCOR = (float)(word(canMsg.data[5], canMsg.data[6]));
        //AIRCOR = (AIRCOR / 10); 
        break;

      case (1520 + 5):  // EGO Correction / Knock Sensor
        //WARMCOR = (float)(word(canMsg.data[0], canMsg.data[1]));
        //WARMCOR = (WARMCOR / 10);      
        //TPSACCEL = (float)(word(canMsg.data[2], canMsg.data[3]));
        //TPSACCEL = (TPSACCEL / 10);
        //TPSFUELCUT = (float)(word(canMsg.data[4], canMsg.data[5]));
        //TPSFUELCUT = (TPSFUELCUT / 10);
        //BAROCOR = (float)(word(canMsg.data[6], canMsg.data[7]));
        //BAROCOR = (BAROCOR / 10);        
        break;

      case (1520 + 6):  // EGO Correction / Knock Sensor
        //TOTALCOR = (float)(word(canMsg.data[0], canMsg.data[1]));
        //TOTALCOR = (TOTALCOR / 10);      
        //VE1 = (float)(word(canMsg.data[2], canMsg.data[3]));
        //VE1 = (VE1 / 10);
        //VE2 = (float)(word(canMsg.data[4], canMsg.data[5]));
        //VE2 = (VE2 / 10);
        //IACSTEP = (float)(word(canMsg.data[6], canMsg.data[7]));
        //IACSTEP = (IACSTEP * 392 / 1000);                

        break;
      case (1520 + 7):  // EGO Correction / Knock Sensor
        //COL_ADV_DEGR = (float)(word(canMsg.data[0], canMsg.data[1]));
        //COL_ADV_DEG = (COL_ADV_DEG / 10);      
        //TPSDOT = (float)(word(canMsg.data[2], canMsg.data[3]));
        //TPSDOT = (TPSDOT / 10);
        //MAPDOT = (float)(word(canMsg.data[4], canMsg.data[5]));
        //RPMSDOT = (float)(word(canMsg.data[7], canMsg.data[7]));
        //RPMDOT = (RPMSDOT * 10 );                

        break;    
      case (1520 + 8):  // EGO Correction / Knock Sensor
        //MAFLOAD = (float)(word(canMsg.data[0], canMsg.data[1]));
        //MAFLOAD = (MAFLOAD / 10);      
        //TPSDOT = (float)(word(canMsg.data[2], canMsg.data[3]));
        //TPSDOT = (TPSDOT / 10);
        //FUELCOR = (float)(word(canMsg.data[4], canMsg.data[5]));
        //FUELCOR = (FUELCOR / 10);                      

        break;    
      case (1520 + 9):  // EGO Correction / Knock Sensor
        //DWELL = (float)(word(canMsg.data[4], canMsg.data[5]));
        //DWELL = (DWELL * 10 );      

        break;                      
      case (1520 + 13):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        FUELP = (float)(word(canMsg.data[0], canMsg.data[1]));
        FUELP = (FUELP / 10); //SENSORS1
        OILP = (float)(word(canMsg.data[2], canMsg.data[3]));
        OILP = (OILP / 10); //SENSORS2
        OILT = (float)(word(canMsg.data[4], canMsg.data[5]));
        OILT = (OILT / 10); //SENSORS3
        //SENSORS4 = (float)(word(canMsg.data[6], canMsg.data[7]));
        //SENSORS4 = (SENSORS4 / 10);
        break;

      case (1520 + 14):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        //SENSORS5 = (float)(word(canMsg.data[0], canMsg.data[1]));
        //SENSORS5 = (SENSORS5 / 10);
        //SENSORS6 = (float)(word(canMsg.data[2], canMsg.data[3]));
        //SENSORS6 = (SENSORS6 / 10);
        //SENSORS7 = (float)(word(canMsg.data[4], canMsg.data[5]));
        //SENSORS7 = (SENSORS7 / 10);
        //SENSORS8 = (float)(word(canMsg.data[6], canMsg.data[7]));
        //SENSORS8 = (SENSORS8 / 10);
        break;

      case (1520 + 15):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        //SENSORS9 = (float)(word(canMsg.data[0], canMsg.data[1]));
        //SENSORS9 = (SENSORS9 / 10);
        //SENSORS10 = (float)(word(canMsg.data[2], canMsg.data[3]));
        //SENSORS10 = (SENSORS10 / 10);
        //SENSORS11 = (float)(word(canMsg.data[4], canMsg.data[5]));
        //SENSORS11 = (SENSORS11 / 10);
        //SENSORS12 = (float)(word(canMsg.data[6], canMsg.data[7]));
        //SENSORS12 = (SENSORS12 / 10);
        break;

      case (1520 + 16):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        //SENSORS13 = (float)(word(canMsg.data[0], canMsg.data[1]));
        //SENSORS13 = (SENSORS13 / 10);
        //SENSORS14 = (float)(word(canMsg.data[2], canMsg.data[3]));
        //SENSORS14 = (SENSORS14 / 10);
        //SENSORS15 = (float)(word(canMsg.data[4], canMsg.data[5]));
        //SENSORS15 = (SENSORS15 / 10);
        //SENSORS16 = (float)(word(canMsg.data[6], canMsg.data[7]));
        //SENSORS16 = (SENSORS16 / 10);
        break;
      case (1520 + 17):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        //BOOST_TARG_1 = (float)(word(canMsg.data[0], canMsg.data[1]));
        //BOOST_TARG_1 = (BOOST_TARG_1 / 10);
        //BOOSTDUTY = (float)(word(canMsg.data[4]));
        break;        
      case (1520 + 28):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        //CL_IDLE_TARG_RPM = (float)(word(canMsg.data[0], canMsg.data[1]));
        break;     
      case (1520 + 42):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        VSS1 = (float)(word(canMsg.data[0], canMsg.data[1]));
        VSS1 = (VSS1 / 10);
        //VSS2 = (float)(word(canMsg.data[2], canMsg.data[3]));
        //VSS2 = (VSS2 / 10);
        //VSS3 = (float)(word(canMsg.data[4], canMsg.data[5]));
        //VSS3 = (VSS3 / 10);
        //VSS4 = (float)(word(canMsg.data[6], canMsg.data[7]));
        //VSS4 = (VSS4 / 10);
        break;           
      case (1520 + 44):  // VVT Angle 1
        VVT = (float)(word(canMsg.data[0], canMsg.data[1]));
        VVT = (VVT / 10);
        break;
      case (1520 + 46):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        //VVTDUTY = (float)(word(canMsg.data[0]));
        //VVTDUTY = (VVTDUTY * 392 / 1000);
        break;             
      case (1520 + 47):  // Ethanol Content %
        ETH = (float)(word(canMsg.data[0], canMsg.data[1]));
        ETH = (ETH / 10);   
      case (1520 + 48):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        //KNOCK_CYL1 = (float)(word(canMsg.data[0]));
        //KNOCK_CYL1 = (KNOCK_CYL1 * 4 / 10);
        //KNOCK_CYL2 = (float)(word(canMsg.data[1]));
        //KNOCK_CYL2 = (KNOCK_CYL2 * 4 / 10);
        //KNOCK_CYL3 = (float)(word(canMsg.data[2]));
        //KNOCK_CYL3 = (KNOCK_CYL3 * 4 / 10);
        //KNOCK_CYL4 = (float)(word(canMsg.data30]));
        //KNOCK_CYL4 = (KNOCK_CYL4 * 4 / 10);
        break;    
      case (1520 + 52):  // Fuel Pressure, Oil Pressure, Oil Temperature - Generic Sensors 1,2,3
        //KNK_RTD = (float)(word(canMsg.data[3]));
        //KNK_RTD = (KNK_RTD / 10);
        //FUELFLOW = (float)(word(canMsg.data[4], canMsg.data[5]));
        //FUELCONS = (float)(word(canMsg.data[6], canMsg.data[7]));
        break;                          
    }

    previousMillis = 0;   // reset no data timer
  } 
  
  {                       // no CAN bus data coming in
    unsigned long currentMillis = millis();
    if (previousMillis == 0) 
    
    {
      previousMillis = currentMillis;                           // entered no data timer
    } else if (currentMillis - previousMillis > delayPeriod) 
    
    {  // no data timer expired
      previousMillis = currentMillis;
      MAP = -999; //Fake data to test gauge even if data is being received
      RPM = -999; //Fake data to test gauge even if data is being received
      TPS = -999; //Fake data to test gauge even if data is being received
      CLT = -999; //Fake data to test gauge even if data is being received
      BATT = -999; //Fake data to test gauge even if data is being received
      BARO = -999;  //Fake data to test gauge even if data is being received
      MAT = -999; //Fake data to test gauge even if data is being received
      EGO1 = -999; //Fake data to test gauge even if data is being received
      EGOC = -999;  //Fake data to test gauge even if data is being received
      FUELP = -999;  //Fake data to test gauge even if data is being received
      OILP = -999; //Fake data to test gauge even if data is being received
      OILT = -999; //Fake data to test gauge even if data is being received
      KNOCK = -999;  //Fake data to test gauge even if data is being received
      ETH = -999; //Fake data to test gauge even if data is being received
      VVT = -999; //Fake data to test gauge even if data is being received
      SPEED = -999; //Fake data to test gauge even if data is being received
    }
  }    

    //Serial1.print(F("page: main."));
    Serial1.print(F("rpm.txt=\""));
    Serial1.print(RPM);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);

    Serial1.print(F("mat.txt=\""));
    Serial1.print(MAT);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);

    Serial1.print(F("map.txt=\""));
    Serial1.print(MAP);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);    

    Serial1.print(F("tps.val="));
    Serial1.print(TPS);
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);

    Serial1.print(F("clt.txt=\""));
    Serial1.print(CLT);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);

    Serial1.print(F("ego.txt=\""));
    Serial1.print(EGO1);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);

    Serial1.print(F("fuelp.txt=\""));
    Serial1.print(FUELP);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);

    Serial1.print(F("oilp.txt=\""));
    Serial1.print(OILP);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);

    Serial1.print(F("oilt.txt=\""));
    Serial1.print(OILT);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);

    Serial1.print(F("batt.txt=\""));
    Serial1.print(BATT);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);

    Serial1.print(F("knock.txt=\""));
    Serial1.print(KNOCK);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);    

    Serial1.print(F("eth.txt=\""));
    Serial1.print(ETH);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);     

    Serial1.print(F("vvt.txt=\""));
    Serial1.print(VVT);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);   

    //Serial1.print(F("hum.txt=\"999"));
    //Serial1.println(h);
    //Serial1.print(F("\""));
    //Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);     
    
    //Serial1.print(F("temp.txt=\"999"));
    //Serial1.println(tf);
    //Serial1.print(F("\""));
    //Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);     
    
    Serial1.print(F("mph.txt=\""));
    Serial1.println(speed);
    Serial1.print(F("\""));
    Serial1.write(0xff); Serial1.write(0xff); Serial1.write(0xff);  

    //EGT    
    Serial.print(readThermocouple());
    Serial.println('c');
    delay(1500);
}

double readThermocouple() {

  uint16_t v;
  pinMode(MAX6675_CS, OUTPUT);
  pinMode(MAX6675_SO, INPUT);
  pinMode(MAX6675_SCK, OUTPUT);
  
  digitalWrite(MAX6675_CS, LOW);
  delay(1);

  // Read in 16 bits,
  //  15    = 0 always
  //  14..2 = 0.25 degree counts MSB First
  //  2     = 1 if thermocouple is open circuit  
  //  1..0  = uninteresting status
  
  v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  v <<= 8;
  v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  
  digitalWrite(MAX6675_CS, HIGH);
  if (v & 0x4) 
  {    
    // Bit 2 indicates if the thermocouple is disconnected
    return NAN;     
  }

  // The lower three bits (0,1,2) are discarded status bits
  v >>= 3;

  // The remaining bits are the number of 0.25 degree (C) counts
  return v*0.25;
}