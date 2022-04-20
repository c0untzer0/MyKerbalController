void controlsInit() {
    pinMode(pTHROTTLE, INPUT);
    pinMode(pTX, INPUT);
    pinMode(pTY, INPUT);
    pinMode(pTZ, INPUT);
    pinMode(pRX, INPUT);
    pinMode(pRY, INPUT);
    pinMode(pRZ, INPUT);
    pinMode(pPOWER, INPUT_PULLUP);
    //pinMode(pTB, INPUT_PULLUP);
    b_TB.attach(pTB, INPUT_PULLUP);
    b_TB.interval(50);
    //pinMode(pRB, INPUT_PULLUP);
    b_RB.attach(pRB, INPUT_PULLUP);
    b_RB.interval(50);
    pinMode(pMODE, INPUT_PULLUP);
    pinMode(pLCDx, INPUT_PULLUP);
    pinMode(pLCDy, INPUT_PULLUP);
    pinMode(pLCDz, INPUT_PULLUP);
    //pinMode(pSASBTN, INPUT_PULLUP);
    b_SASBTN.attach(pSASBTN, INPUT_PULLUP);
    b_SASBTN.interval(50);
    pinMode(pSAS, INPUT_PULLUP);
    pinMode(pRCS, INPUT_PULLUP);
    pinMode(pABORT, INPUT);
    //pinMode(pABORTBTN, INPUT_PULLUP);
    b_ABORTBTN.attach(pABORTBTN, INPUT_PULLUP);
    b_ABORTBTN.interval(50);
    pinMode(pABORTBTNLED, OUTPUT);
    pinMode(pARM, INPUT);
    //pinMode(pSTAGE, INPUT_PULLUP);
    b_STAGE.attach(pSTAGE, INPUT_PULLUP);
    b_STAGE.interval(50);
    pinMode(pSTAGELED, OUTPUT);
    //pinMode(pLIGHTS, INPUT_PULLUP);
    b_LIGHTS.attach(pLIGHTS, INPUT_PULLUP);
    b_LIGHTS.interval(50);
    pinMode(pLIGHTSLED, OUTPUT);
    //pinMode(pLADDER, INPUT_PULLUP);
    b_LADDER.attach(pLADDER, INPUT_PULLUP);
    b_LADDER.interval(50);
    pinMode(pLADDERLED, OUTPUT);
    //pinMode(pSOLAR, INPUT_PULLUP);
    b_SOLAR.attach(pSOLAR, INPUT_PULLUP);
    b_SOLAR.interval(50);
    pinMode(pSOLARLED, OUTPUT);
    //pinMode(pCHUTES, INPUT_PULLUP);
    b_CHUTES.attach(pCHUTES, INPUT_PULLUP);
    b_CHUTES.interval(50);
    pinMode(pCHUTESLED, OUTPUT);
    //pinMode(pGEARS, INPUT_PULLUP);
    b_GEARS.attach(pGEARS, INPUT_PULLUP);
    b_GEARS.interval(50);
    pinMode(pGEARSLED, OUTPUT);
    //pinMode(pBRAKES, INPUT_PULLUP);
    b_BRAKES.attach(pBRAKES, INPUT_PULLUP);
    b_BRAKES.interval(50);
    pinMode(pBRAKESLED, OUTPUT);
    //pinMode(pACTION1, INPUT_PULLUP);
    b_ACTION1.attach(pACTION1, INPUT_PULLUP);
    b_ACTION1.interval(50);
    pinMode(pACTION1LED, OUTPUT);
    //pinMode(pACTION2, INPUT_PULLUP);
    b_ACTION2.attach(pACTION2, INPUT_PULLUP);
    b_ACTION2.interval(50);
    pinMode(pACTION2LED, OUTPUT);
    //pinMode(pACTION3, INPUT_PULLUP);
    b_ACTION3.attach(pACTION3, INPUT_PULLUP);
    b_ACTION3.interval(50);
    pinMode(pACTION3LED, OUTPUT);
    //pinMode(pACTION4, INPUT_PULLUP);
    b_ACTION4.attach(pACTION4, INPUT_PULLUP);
    b_ACTION4.interval(50);
    pinMode(pACTION4LED, OUTPUT);
    //pinMode(pACTION5, INPUT_PULLUP);
    b_ACTION5.attach(pACTION5, INPUT_PULLUP);
    b_ACTION5.interval(50);
    pinMode(pACTION5LED, OUTPUT);
    //pinMode(pACTION6, INPUT_PULLUP);
    b_ACTION6.attach(pACTION6, INPUT_PULLUP);
    b_ACTION6.interval(50);
    pinMode(pACTION6LED, OUTPUT);
    //pinMode(pACTION7, INPUT_PULLUP);
    b_ACTION7.attach(pACTION7, INPUT_PULLUP);
    b_ACTION7.interval(50);
    pinMode(pACTION7LED, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(sasLEDLatch, OUTPUT);
    pinMode(sasLEDData, OUTPUT);
    pinMode(sasLEDClock, OUTPUT);
    pinMode(sasClockPin, INPUT);
    pinMode(sasDataPin, INPUT);
    //pinMode(pCAMMODEUP, INPUT_PULLUP);
    b_CAMMODEUP.attach(pCAMMODEUP, INPUT_PULLUP);
    b_CAMMODEUP.interval(50);
    //pinMode(pCAMMODEDOWN, INPUT_PULLUP);
    b_CAMMODEDOWN.attach(pCAMMODEDOWN, INPUT_PULLUP);
    b_CAMMODEDOWN.interval(50);
    pinMode(pSASONLED, OUTPUT);
    pinMode(pRCSONLED, OUTPUT);
    sasPreviousCLK = digitalRead(sasClockPin);
    sasPreviousDATA = digitalRead(sasDataPin);
}

void testLEDS(int testdelay) {
    digitalWrite(pSTAGELED, HIGH);
    delay(testdelay);
    digitalWrite(pSTAGELED, LOW);
    digitalWrite(pABORTBTNLED, HIGH);
    delay(testdelay);
    digitalWrite(pABORTBTNLED, LOW);
    digitalWrite(pLIGHTSLED, HIGH);
    delay(testdelay);
    digitalWrite(pLIGHTSLED, LOW);
    digitalWrite(pLADDERLED, HIGH);
    delay(testdelay);
    digitalWrite(pLADDERLED, LOW);
    digitalWrite(pSOLARLED, HIGH);
    delay(testdelay);
    digitalWrite(pSOLARLED, LOW);
    digitalWrite(pCHUTESLED, HIGH);
    delay(testdelay);
    digitalWrite(pCHUTESLED, LOW);
    digitalWrite(pGEARSLED, HIGH);
    delay(testdelay);
    digitalWrite(pGEARSLED, LOW);
    digitalWrite(pBRAKESLED, HIGH);
    delay(testdelay);
    digitalWrite(pBRAKESLED, LOW);
    digitalWrite(pACTION1LED, HIGH);
    delay(testdelay);
    digitalWrite(pACTION1LED, LOW);
    digitalWrite(pACTION2LED, HIGH);
    delay(testdelay);
    digitalWrite(pACTION2LED, LOW);
    digitalWrite(pACTION3LED, HIGH);
    delay(testdelay);
    digitalWrite(pACTION3LED, LOW);
    digitalWrite(pACTION4LED, HIGH);
    delay(testdelay);
    digitalWrite(pACTION4LED, LOW);
    digitalWrite(pACTION5LED, HIGH);
    delay(testdelay);
    digitalWrite(pACTION5LED, LOW);
    digitalWrite(pACTION6LED, HIGH);
    delay(testdelay);
    digitalWrite(pACTION6LED, LOW);
    digitalWrite(pACTION7LED, HIGH);
    delay(testdelay);
    digitalWrite(pACTION7LED, LOW);
    digitalWrite(pSASONLED, HIGH);
    delay(testdelay);
    digitalWrite(pSASONLED, LOW);
    digitalWrite(pRCSONLED, HIGH);
    delay(testdelay);
    digitalWrite(pRCSONLED, LOW);

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
        Serial.println(inputByte);
        shiftOut(dataPin, clockPin, MSBFIRST, inputByte);
    }

    //latch the values in when done shifting
    digitalWrite(latchPin, HIGH);

    delay(testdelay);

    //prepare the shift register
    digitalWrite(dataPin, LOW);
    digitalWrite(clockPin, LOW);
    digitalWrite(latchPin, LOW);

    inputBytes[0] = B00000000;
    inputBytes[1] = B00000000;
    inputBytes[2] = B00000000;
    inputBytes[3] = B00000000;
    inputBytes[4] = B00000000;
    inputBytes[5] = B00000000;
    inputBytes[6] = B00000000;
    inputBytes[7] = B00000000;
    inputBytes[8] = B00000000;
    inputBytes[9] = B00000000;
    //loop through the input bytes
    for (int j = 0; j <= 9; j++) {
        byte inputByte = inputBytes[j];
        Serial.println(inputByte);
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
        Serial.println(sasInputByte);
        shiftOut(sasLEDData, sasLEDClock, MSBFIRST, sasInputByte);
    }

    //latch the values in when done shifting
    digitalWrite(sasLEDLatch, HIGH);

    delay(testdelay);

    //prepare the SAS LED shift register
    digitalWrite(sasLEDData, LOW);
    digitalWrite(sasLEDClock, LOW);
    digitalWrite(sasLEDLatch, LOW);

    sasInputBytes[0] = B00000000;
    sasInputBytes[1] = B00000000;

    //loop through the SAS LED input bytes
    for (int j = 0; j <= 1; j++) {
        byte sasInputByte = sasInputBytes[j];
        Serial.println(sasInputByte);
        shiftOut(sasLEDLatch, sasLEDClock, MSBFIRST, sasInputByte);
    }

    //latch the values in when done shifting
    digitalWrite(sasLEDLatch, HIGH);

}

