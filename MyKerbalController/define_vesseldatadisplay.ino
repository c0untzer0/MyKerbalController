//grab info from KSP here (VData object) and write out results to the Arduino pins


//Enumeration of SAS Modes
#define SMOFF          255
#define SMSAS          0
#define SMPrograde     1
#define SMRetroGrade   2
#define SMNormal       3
#define SMAntinormal   4
#define SMRadialIn     5
#define SMRadialOut    6
#define SMTarget       7
#define SMAntiTarget   8
#define SMManeuverNode 9

//Message Handler function
void messageHandler(byte messageType, byte msg[], byte msgSize) {
	byte currentActionStatus;
    switch (messageType) {
	//Action Groups Messages
    case ACTIONSTATUS_MESSAGE:
        // Checking if the message is the size we expect is a very basic
        // way to confirm if the message was received properly.
        if (msgSize == 1) {
            currentActionStatus = msg[0];

            if (currentActionStatus & LIGHT_ACTION) {
                lights_on = true;
            }
            else {
                lights_on = false;
            }
            if (currentActionStatus & GEAR_ACTION) {
                gears_on = true;
            }
            else {
                gears_on = false;
            }
            if (currentActionStatus & BRAKES_ACTION) {
                brakes_on = true;
            }
            else {
                brakes_on = false;
            }
            if (currentActionStatus & SAS_ACTION) {
                sas_led_on = true;
				sas_is_on = true;
            }
            else {
                sas_led_on = false;
				sas_is_on = false;
            }
            if (currentActionStatus & RCS_ACTION) {
                rcs_led_on = true;
				rcs_is_on = true;
            }
            else {
                rcs_led_on = false;
				rcs_is_on = false;
            }
        }
        break;
	//Custom Action Group Messages
    case CAGSTATUS_MESSAGE:
        if (msgSize == sizeof(cagStatusMessage)) {
            cagStatusMessage cagStatus;
            cagStatus = parseMessage<cagStatusMessage>(msg);
            if (cagStatus.is_action_activated(1)) {
                action1_on = true;
            }
            else {
                action1_on = false;
            }
            if (cagStatus.is_action_activated(2)) {
                action2_on = true;
            }
            else {
                action2_on = false;
            }
            if (cagStatus.is_action_activated(3)) {
                action3_on = true;
            }
            else {
                action3_on = false;
            }
            if (cagStatus.is_action_activated(4)) {
                action4_on = true;
            }
            else {
                action4_on = false;
            }
            if (cagStatus.is_action_activated(5)) {
                action5_on = true;
            }
            else {
                action5_on = false;
            }
            if (cagStatus.is_action_activated(6)) {
                action6_on = true;
            }
            else {
                action6_on = false;
            }
            if (cagStatus.is_action_activated(7)) {
                action7_on = true;
            }
            else {
                action7_on = false;
            }
            if (cagStatus.is_action_activated(8)) {
                ladder_on = true;
            }
            else {
                ladder_on = false;
            }
            if (cagStatus.is_action_activated(9)) {
                solar_on = true;
            }
            else {
                solar_on = false;
            }
            if (cagStatus.is_action_activated(10)) {
                chutes_on = true;
            }
            else {
                chutes_on = false;
            }
        }
        break;
	//SAS Mode Messages
    case SAS_MODE_INFO_MESSAGE:
		mySimpit.printToKSP("SAS Mode received");
        if (msgSize == sizeof(SASInfoMessage)) {
            SASInfoMessage SASModeInfo;
            SASModeInfo = parseMessage<SASInfoMessage>(msg);
            sas_mode = SASModeInfo.currentSASMode;
			sas_available = SASModeInfo.SASModeAvailability;
			mySimpit.printToKSP((String)"Available SAS modes: " + sas_available);
        }
        break;
	//Fuel Messages
    case LF_STAGE_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage liquidFuelSInfo;
            liquidFuelSInfo = parseMessage<resourceMessage>(msg);
			//Do some sanity checks in case I'm overflowing the buffers
			if (liquidFuelSInfo.available >= 0) {
				VData.LiquidFuelS = liquidFuelSInfo.available;
			}
			if (liquidFuelSInfo.total >= 0) {
				VData.LiquidFuelSTot = liquidFuelSInfo.total;
			}
        }
        break;
    case SF_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage solidFuelInfo;
            solidFuelInfo = parseMessage<resourceMessage>(msg);
			//Do some sanity checks in case I'm overflowing the buffers
			if (solidFuelInfo.available >= 0) {
				VData.SolidFuel = solidFuelInfo.available;
			}
			if (solidFuelInfo.total >= 0) {
				VData.SolidFuelTot = solidFuelInfo.total;
			}
        }
        break;
    case OX_STAGE_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage oxidizerSInfo;
            oxidizerSInfo = parseMessage<resourceMessage>(msg);
			//Do some sanity checks in case I'm overflowing the buffers
			if (oxidizerSInfo.available >= 0){
				VData.oxidizerSAvailable = oxidizerSInfo.available;
			}
			if (oxidizerSInfo.total >= 0){
				VData.oxidizerSTotal = oxidizerSInfo.total;
			}
        }
        break;
    case ELECTRIC_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage ecInfo;
            ecInfo = parseMessage<resourceMessage>(msg);
			//Do some sanity checks in case I'm overflowing the buffers
			if (ecInfo.available >= 0) {
				VData.ecAvailable = ecInfo.available;
			}
			if (ecInfo.total >= 0) {
				VData.ecTotal = ecInfo.total;
			}
        }
        break;
    case MONO_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage monoPropInfo;
            monoPropInfo = parseMessage<resourceMessage>(msg);
			//Do some sanity checks in case I'm overflowing the buffers
			if (monoPropInfo.available >= 0) {
				VData.monoPropAvailable = monoPropInfo.available;
			}
			if (monoPropInfo.total >= 0) {
				VData.monoPropTotal = monoPropInfo.total;
			}
        }
        break;
    case XENON_GAS_MESSAGE:
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage xenonGasInfo;
            xenonGasInfo = parseMessage<resourceMessage>(msg);
			//Do some sanity checks in case I'm overflowing the buffers
			if (xenonGasInfo.available >= 0) {
				VData.xenonGasAvailable = xenonGasInfo.available;
			}
			if (xenonGasInfo.total >= 0) {
				VData.xenonGasTotal = xenonGasInfo.total;
			}
        }
        break;
    case CUSTOM_RESOURCE_1_MESSAGE:
        if (msgSize == sizeof(CustomResourceMessage)) {
            CustomResourceMessage customResourceInfo;
            customResourceInfo = parseMessage<CustomResourceMessage>(msg);
			//Do some sanity checks in case I'm overflowing the buffers
			if (customResourceInfo.currentResource1 >= 0) {
				VData.customResource1Available = customResourceInfo.currentResource1;
			}
			if (customResourceInfo.maxResource1 >= 0) {
				VData.customResource1Total = customResourceInfo.maxResource1;
			}
			if (customResourceInfo.currentResource2 >= 0) {
				VData.customResource2Available = customResourceInfo.currentResource2;
			}
			if (customResourceInfo.maxResource2 >= 0) {
				VData.customResource2Total = customResourceInfo.maxResource2;
			}
        }
        break;
    //Altitude Message
	case ALTITUDE_MESSAGE:
		if (msgSize == sizeof(altitudeMessage)) {
			altitudeMessage altitudeInfo;
			altitudeInfo = parseMessage<altitudeMessage>(msg);
			VData.Alt= altitudeInfo.sealevel;
			VData.RAlt = altitudeInfo.surface;
		}
		break;
	//Apoapsis and Periapsis Message
	case APSIDES_MESSAGE:
		if (msgSize == sizeof(apsidesMessage)) {
			apsidesMessage apsidesInfo;
			apsidesInfo = parseMessage<apsidesMessage>(msg);
			VData.AP = apsidesInfo.apoapsis;
			VData.PE = apsidesInfo.periapsis;
		}
		break;
	//Velocity Message
	case VELOCITY_MESSAGE:
		if (msgSize == sizeof(velocityMessage)) {
			velocityMessage velocityInfo;
			velocityInfo = parseMessage<velocityMessage>(msg);
			VData.VOrbit = velocityInfo.orbital;
			VData.Vsurf = velocityInfo.surface;
			VData.VVI = velocityInfo.vertical;
		}
		break;
	//Times to apoapsis and periapsis
    case APSIDESTIME_MESSAGE:
		if (msgSize == sizeof(apsidesTimeMessage)) {
			apsidesTimeMessage apsidesTimeInfo;
			apsidesTimeInfo = parseMessage<apsidesTimeMessage>(msg);
			VData.TAp = apsidesTimeInfo.apoapsis;
			VData.TPe = apsidesTimeInfo.periapsis;
		}
		break;
	//Target info message
	case TARGETINFO_MESSAGE:
		if (msgSize == sizeof(targetMessage)) {
			targetMessage targetInfo;
			targetInfo = parseMessage<targetMessage>(msg);
			VData.TargetDist = targetInfo.distance;
			VData.TargetV = targetInfo.velocity;
			VData.TargetPitch = targetInfo.pitch;
			VData.TargetHeading = targetInfo.heading;
			VData.TargetPitchV = targetInfo.velocityPitch;
			VData.TargetHeadingV = targetInfo.velocityHeading;
		}
		break;
    //Orbit message
	case ORBIT_MESSAGE:
		if (msgSize == sizeof(orbitInfoMessage)) {
			orbitInfoMessage orbitInfo;
			orbitInfo = parseMessage<orbitInfoMessage>(msg);
			VData.PEArg = orbitInfo.argPeriapsis;
			VData.e = orbitInfo.eccentricity;
			VData.TrueAnomaly = orbitInfo.trueAnomaly;
			VData.MeanAnomaly = orbitInfo.meanAnomaly;
			VData.inc = orbitInfo.inclination;
			VData.period = orbitInfo.period;
			VData.SemiMajorAxis = orbitInfo.semiMajorAxis;
			VData.LongAscNode = orbitInfo.longAscendingNode;
		}
		break;
	//Flight status message
	case FLIGHT_STATUS_MESSAGE:
		if (msgSize == sizeof(flightStatusMessage)) {
			flightStatusMessage flightInfo;
			flightInfo = parseMessage<flightStatusMessage>(msg);
			VData.CurrentStage = flightInfo.currentStage;
			VData.TWI = flightInfo.currentTWIndex;
		}
		break;
	//Airspeed message
	case AIRSPEED_MESSAGE:
		if (msgSize == sizeof(airspeedMessage)) {
			airspeedMessage airspeedInfo;
			airspeedInfo = parseMessage<airspeedMessage>(msg);
			VData.IAS = airspeedInfo.IAS;
			VData.MachNumber = airspeedInfo.mach;
		}
		break;
	//Rotation message
	case ROTATION_DATA:
		if (msgSize == sizeof(vesselPointingMessage)) {
			vesselPointingMessage pointingInfo;
			pointingInfo = parseMessage<vesselPointingMessage>(msg);
			VData.Heading = pointingInfo.heading;
			VData.Pitch = pointingInfo.pitch;
			VData.Roll = pointingInfo.roll;
		}
		break;
	//DeltaV message
	case DELTAV_MESSAGE:
		if (msgSize == sizeof(deltaVMessage)) {
			deltaVMessage deltaVInfo;
			deltaVInfo = parseMessage<deltaVMessage>(msg);
			VData.StageDeltaV = deltaVInfo.stageDeltaV;
			VData.TotalDeltaV = deltaVInfo.totalDeltaV;
		}
		break;
	//Burn Time message
	case BURNTIME_MESSAGE:
		if (msgSize == sizeof(burnTimeMessage)) {
			burnTimeMessage burnTInfo;
			burnTInfo = parseMessage<burnTimeMessage>(msg);
			VData.BurnTime = burnTInfo.totalBurnTime;
			VData.BurnTimeS = burnTInfo.stageBurnTime;
		}
		break;
	//Temperature Limits message
	case TEMP_LIMIT_MESSAGE:
		if (msgSize == sizeof(tempLimitMessage)) {
			tempLimitMessage tempLimitInfo;
			tempLimitInfo = parseMessage<tempLimitMessage>(msg);
			VData.MaxOverHeat = tempLimitInfo.tempLimitPercentage;
			VData.MaxSkinOverheat = tempLimitInfo.skinTempLimitPercentage;
		}
		break;
	//Atmospheric conditions message
	case ATMO_CONDITIONS_MESSAGE:
		if (msgSize == sizeof(atmoConditionsMessage)) {
			atmoConditionsMessage atmoInfo;
			atmoInfo = parseMessage<atmoConditionsMessage>(msg);
			VData.Density = atmoInfo.airDensity;
		}
		break;
	//Maneuver message
	case MANEUVER_MESSAGE:
		if (msgSize == sizeof(maneuverMessage)) {
			maneuverMessage maneuverInfo;
			maneuverInfo = parseMessage<maneuverMessage>(msg);
			VData.MNTime = maneuverInfo.timeToNextManeuver;
			VData.MNDeltaV = maneuverInfo.deltaVNextManeuver;
			VData.MNDuration = maneuverInfo.durationNextManeuver;
			VData.MNDeltaVTotal = maneuverInfo.deltaVTotal;
		}
		break;
	//SOI message
	case SOI_MESSAGE:
		VData.SOI = msg[0];
		break;
    }

}

//Define what toi display to LCD on a regular basis
void display_to_lcd()
{

	//LCD Display Modes
	// 0 xyz TakeOff Mode:     Suface Velocity / Acceleration (G)
	// 1 Xyz Orbit Mode:       Apoapsis + Time to Apoapsis / Periapsis + Time to Periapsis
	// 2 xYz Maneuver Mode:    Time to next maneuver node / Remaining Delta-V for next maneuver node
	// 3 XYz Rendezvouz Mode:  Distance to target / Velocity relative to target
	// 4 xyZ Re-Entry Mode:    Percentage overheating (max) /   Deceleration (G)
	// 5 XyZ Flying Mode:      Altitude / Mach number
	// 6 xYZ Landing Mode:     Radar Altitude / Vertical Velocity
	// 7 XYZ Extra Mode:       Not implemented yet. Possibly DeltaV left on Vessel / Total Burn Time?

	if (digitalRead(pLCDx) && digitalRead(pLCDy) && digitalRead(pLCDz)) {
		//MODE 0 : TakeOff Mode
		//Vsurf
		clearLCD();
		char bufferVsurf[17];
		String strVsurf = "Vsurf: ";
		strVsurf += String(VData.Vsurf, 0);
		//mySimpit.printToKSP((String)"VSurf to display: " + strVsurf);
		strVsurf += " m/s";
		strVsurf.toCharArray(bufferVsurf, 17);
		writeLCD(bufferVsurf);

		// Unable to display acceleration for now, as KSimpit does not provide it
		////Acceleration (G)
		//jumpToLineTwo();
		//char bufferGee[17];
		//String strGee = "Accel: ";
		//strGee += String(VData.G, 0);
		//strGee += " G";
		//strGee.toCharArray(bufferGee, 17);
		//writeLCD(bufferGee);

		//Radar Altitude (m)
		jumpToLineTwo();
		char bufferRAlt[17];
		String strRAlt = "RAlt: ";
		if (VData.RAlt < 10000 && VData.RAlt > -10000) {
			strRAlt += String(VData.RAlt, 0);
			strRAlt += "m ";
		}
		else if ((VData.RAlt >= 10000 && VData.RAlt < 10000000) || (VData.RAlt <= -10000 && VData.RAlt > -10000000)) {
			strRAlt += String((VData.RAlt / 1000.0), 0);
			strRAlt += "km ";
		}
		else if ((VData.RAlt >= 10000000 && VData.RAlt < 10000000000) || (VData.RAlt <= -10000000 && VData.RAlt > -10000000000)) {
			strRAlt += String((VData.RAlt / 1000000.0), 0);
			strRAlt += "Mm ";
		}
		else {
			strRAlt += String((VData.RAlt / 1000000000.0), 0);
			strRAlt += "Gm ";
		}
		strRAlt.toCharArray(bufferRAlt, 17);
		writeLCD(bufferRAlt);

	}


	if (!digitalRead(pLCDx) && digitalRead(pLCDy) && digitalRead(pLCDz)) {
		//MODE 1: Orbit Mode
		clearLCD();

		//Apoapsis
		char bufferAP[17];
		String strApo = "AP: ";
		if (VData.AP < 10000 && VData.AP > -10000) {
			strApo += String(VData.AP, 0);
			strApo += "m ";
		}
		else if ((VData.AP >= 10000 && VData.AP < 10000000) || (VData.AP <= -10000 && VData.AP > -10000000)) {
			strApo += String((VData.AP / 1000), 0);
			strApo += "km ";
		}
		else if ((VData.AP >= 10000000 && VData.AP < 10000000000) || (VData.AP <= -10000000 && VData.AP > -10000000000)) {
			strApo += String((VData.AP / 1000000), 0);
			strApo += "Mm ";
		}
		else {
			strApo += String((VData.AP / 1000000000), 0);
			strApo += "Gm ";
		}
		strApo += String(VData.TAp); //time to apoapsis
		strApo += "s";
		strApo.toCharArray(bufferAP, 17);
		writeLCD(bufferAP);

		//Periapsis
		char bufferPE[17];
		String strPeri = "PE: ";
		if (VData.PE < 10000 && VData.PE > -10000) {
			strPeri += String(VData.PE, 0);
			strPeri += "m ";
		}
		else if ((VData.PE >= 10000 && VData.PE < 10000000) || (VData.PE <= -10000 && VData.PE > -10000000)) {
			strPeri += String((VData.PE / 1000.0), 0);
			strPeri += "km ";
		}
		else if ((VData.PE >= 10000000 && VData.PE < 10000000000) || (VData.PE <= -10000000 && VData.PE > -10000000000)) {
			strPeri += String((VData.PE / 1000000.0), 0);
			strPeri += "Mm ";
		}
		else {
			strPeri += String((VData.PE / 1000000000.0), 0);
			strPeri += "Gm ";
		}
		strPeri += String(VData.TPe); //time to periapsis
		strPeri += "s";
		strPeri.toCharArray(bufferPE, 17);
		jumpToLineTwo();
		writeLCD(bufferPE);
	}

	if (digitalRead(pLCDx) && !digitalRead(pLCDy) && digitalRead(pLCDz)) {
		//MODE 2: Maneuver Mode
		//MNTime
		clearLCD();
		char t[10];
		dtostrf(VData.MNTime, 8, 0, t);
		writeLCD("Tnode: ");
		writeLCD(t);
		writeLCD("s");
		//MNDeltaV
		jumpToLineTwo();
		char bufferMNDeltaV[17];
		String strMNDeltaV = "dV: ";
		strMNDeltaV += String(VData.MNDeltaV, 0);
		strMNDeltaV += " m/s";
		strMNDeltaV.toCharArray(bufferMNDeltaV, 17);
		writeLCD(bufferMNDeltaV);
	}

	if (!digitalRead(pLCDx) && !digitalRead(pLCDy) && digitalRead(pLCDz)) {
		//MODE 3: Rendezvouz Mode
		//Target Distance
		clearLCD();
		char bufferTargetDist[17];
		String strTargetDist = "TDist: ";
		strTargetDist += String(VData.TargetDist, 0);
		strTargetDist += " m";
		strTargetDist.toCharArray(bufferTargetDist, 17);
		writeLCD(bufferTargetDist);
		//Target Velocity
		jumpToLineTwo();
		char bufferTargetV[17];
		String strTargetV = "TVel: ";
		strTargetV += String(VData.TargetV, 0);
		strTargetV += " m/s";
		strTargetV.toCharArray(bufferTargetV, 17);
		writeLCD(bufferTargetV);
	}

	if (digitalRead(pLCDx) && digitalRead(pLCDy) && !digitalRead(pLCDz)) {
		//MODE 4: Re-Entry Mode
		//MaxOverHeat
		clearLCD();
		char t[3];
		dtostrf(VData.MaxOverHeat, 3, 0, t);
		writeLCD("Heat: ");
		writeLCD(t);
		writeLCD("%");

		// Unable to display acceleration for now, as KSimpit does not provide it
		////Acceleration (G)
		//jumpToLineTwo();
		//char bufferGee[17];
		//String strGee = "Decel: ";
		//strGee += String(VData.G, 0);
		//strGee += " G";
		//strGee.toCharArray(bufferGee, 17);
		//writeLCD(bufferGee);

		//Radar Altitude (m)
		jumpToLineTwo();
		char bufferRAlt[17];
		String strRAlt = "RAlt: ";
		if (VData.RAlt < 10000 && VData.RAlt > -10000) {
			strRAlt += String(VData.RAlt, 0);
			strRAlt += "m ";
		}
		else if ((VData.RAlt >= 10000 && VData.RAlt < 10000000) || (VData.RAlt <= -10000 && VData.RAlt > -10000000)) {
			strRAlt += String((VData.RAlt / 1000.0), 0);
			strRAlt += "km ";
		}
		else if ((VData.RAlt >= 10000000 && VData.RAlt < 10000000000) || (VData.RAlt <= -10000000 && VData.RAlt > -10000000000)) {
			strRAlt += String((VData.RAlt / 1000000.0), 0);
			strRAlt += "Mm ";
		}
		else {
			strRAlt += String((VData.RAlt / 1000000000.0), 0);
			strRAlt += "Gm ";
		}
		strRAlt.toCharArray(bufferRAlt, 17);
		writeLCD(bufferRAlt);
	}

	if (!digitalRead(pLCDx) && digitalRead(pLCDy) && !digitalRead(pLCDz)) {
		//MODE 5: Flying Mode
		//Alt
		clearLCD();
		char bufferAlt[17];
		String strAlt = "Alt:  ";
		if (VData.Alt < 10000 && VData.Alt > -10000) {
			strAlt += String(VData.Alt, 0);
			strAlt += "m ";
		}
		else if ((VData.Alt >= 10000 && VData.Alt < 10000000) || (VData.Alt <= -10000 && VData.Alt > -10000000)) {
			strAlt += String((VData.Alt / 1000.0), 0);
			strAlt += "km ";
		}
		else if ((VData.Alt >= 10000000 && VData.Alt < 10000000000) || (VData.Alt <= -10000000 && VData.Alt > -10000000000)) {
			strAlt += String((VData.Alt / 1000000.0), 0);
			strAlt += "Mm ";
		}
		else {
			strAlt += String((VData.Alt / 1000000000.0), 0);
			strAlt += "Gm ";
		}
		strAlt.toCharArray(bufferAlt, 17);
		writeLCD(bufferAlt);
		//Mach Number
		jumpToLineTwo();
		char bufferMachNumber[17];
		String strMach = "Mach:";
		strMach += String(VData.MachNumber, 0);
		strMach.toCharArray(bufferMachNumber, 17);
		writeLCD(bufferMachNumber);
	}

	if (digitalRead(pLCDx) && !digitalRead(pLCDy) && !digitalRead(pLCDz)) {
		//MODE 6: Landing Mode
		//RAlt
		clearLCD();
		char bufferRAlt[17];
		String strRAlt = "RAlt: ";
		if (VData.RAlt < 10000 && VData.RAlt > -10000) {
			strRAlt += String(VData.RAlt, 0);
			strRAlt += "m ";
		}
		else if ((VData.RAlt >= 10000 && VData.RAlt < 10000000) || (VData.RAlt <= -10000 && VData.RAlt > -10000000)) {
			strRAlt += String((VData.RAlt / 1000.0), 0);
			strRAlt += "km ";
		}
		else if ((VData.RAlt >= 10000000 && VData.RAlt < 10000000000) || (VData.RAlt <= -10000000 && VData.RAlt > -10000000000)) {
			strRAlt += String((VData.RAlt / 1000000.0), 0);
			strRAlt += "Mm ";
		}
		else {
			strRAlt += String((VData.RAlt / 1000000000.0), 0);
			strRAlt += "Gm ";
		}
		strRAlt.toCharArray(bufferRAlt, 17);
		writeLCD(bufferRAlt);
		//Vertical Velocity
		jumpToLineTwo();
		char bufferVVI[17];
		String strVVI = "VVI:  ";
		strVVI += String(VData.VVI, 0);
		strVVI += " m/s";
		strVVI.toCharArray(bufferVVI, 17);
		writeLCD(bufferVVI);
	}

	if (!digitalRead(pLCDx) && !digitalRead(pLCDy) && !digitalRead(pLCDz)) {
		//MODE 7: Extra Mode
		clearLCD();
		writeLCD("KerbalController");
	}
}

//define what to do with the vessel data here, e.g. turn on LED's, display text on the LCD
void define_vessel_data_display() {

	//update button LEDs based on in-game status
	digitalWrite(pLIGHTSLED, lights_on);
	digitalWrite(pGEARSLED, gears_on);
	digitalWrite(pBRAKESLED, brakes_on);
	digitalWrite(pACTION1LED, action1_on);
	digitalWrite(pACTION2LED, action2_on);
	digitalWrite(pACTION3LED, action3_on);
	digitalWrite(pACTION4LED, action4_on);
	digitalWrite(pACTION5LED, action5_on);
	digitalWrite(pACTION6LED, action6_on);
	digitalWrite(pACTION7LED, action7_on);
	digitalWrite(pLADDERLED, ladder_on);
	digitalWrite(pSOLARLED, solar_on);
	digitalWrite(pCHUTESLED, chutes_on);
	digitalWrite(pSASONLED, sas_led_on);
	digitalWrite(pRCSONLED, rcs_led_on);


	//Fuel Gauges
	if (VData.SolidFuel != 0)
	{
		vSF = 100 * VData.SolidFuel / VData.SolidFuelTot; //percentage of solid fuel remaining
	}
	else { vSF = 0; }
	if (VData.LiquidFuelSTot != 0) {
		vLF = 100 * VData.LiquidFuelS / VData.LiquidFuelSTot; //percentage of liquid fuel remaining in current stage
	}
	else { vLF = 0; }
	if (VData.oxidizerSTotal != 0) {
		vOX = 100 * VData.oxidizerSAvailable / VData.oxidizerSTotal; //percentage of oxidized remaining in current stage
	}
	else { vOX = 0; }
	if (VData.ecTotal != 0) {
		vEL = 100 * VData.ecAvailable / VData.ecTotal; //percentage of electric charge remaining
	}
	else { vEL = 0; }
	if (VData.monoPropTotal != 0) {
		vMP = 100 * VData.monoPropAvailable / VData.monoPropTotal; //percentage of monopropellant remaining
	}
	else { vMP = 0; }
	if (VData.xenonGasTotal != 0) {
		vXE = 100 * VData.xenonGasAvailable / VData.xenonGasTotal; //percentage of Xenon Gas remaining
	}
	else { vXE = 0; }
	if (VData.customResource1Total != 0) {
		vLI = 100 * VData.customResource1Available / VData.customResource1Total; //percentage of Xenon Gas remaining
	}
	else { vLI = 0; }
	if (VData.customResource2Total != 0) {
		vXX = 100 * VData.customResource2Available / VData.customResource2Total; //percentage of Xenon Gas remaining
	}
	else { vXX = 0; }


	//scale down to 0-9 for binary calculations
	SF = constrain(map(vSF, 100, 0, 0, 9), 0, 9);
	LF = constrain(map(vLF, 100, 0, 0, 9), 0, 9);
	OX = constrain(map(vOX, 100, 0, 0, 9), 0, 9);
	EL = constrain(map(vEL, 100, 0, 0, 9), 0, 9);
	MP = constrain(map(vMP, 100, 0, 0, 9), 0, 9);
	XE = constrain(map(vXE, 100, 0, 0, 9), 0, 9);
	LI = constrain(map(vLI, 100, 0, 0, 9), 0, 9);
	XX = constrain(map(vXX, 100, 0, 0, 9), 0, 9);


	//calculate the power of 2. Now each value in binary is all zeroes and a single 1. we can use that to light one LED in each LED bar (dot mode)
	int powOX = 0.1 + pow(2, OX);
	int powEL = 0.1 + pow(2, EL);
	int powMP = 0.1 + pow(2, MP);
	int powSF = 0.1 + pow(2, SF);
	int powLF = 0.1 + pow(2, LF);
	int powXE = 0.1 + pow(2, XE);
	int powLI = 0.1 + pow(2, LI);
	int powXX = 0.1 + pow(2, XX);


	//map the 8-bit 595 shift registers to the 10-bit LED bars, specific to the way I wired them
	inputBytes[0] = powSF >> 2;
	inputBytes[1] = (powSF << 6) | (powLF >> 4);
	inputBytes[2] = (powLF << 4) | (powOX >> 6);
	inputBytes[3] = (powOX << 2) | (powMP >> 8);
	inputBytes[4] = powMP;
	inputBytes[5] = powEL >> 2;
	inputBytes[6] = (powEL << 6) | (powXE >> 4);
	inputBytes[7] = (powXE << 4) | (powLI >> 6);
	inputBytes[8] = (powLI << 2) | (powXX >> 8);
	inputBytes[9] = powXX;


	//prepare the shift register
	digitalWrite(dataPin, LOW);
	digitalWrite(clockPin, LOW);
	digitalWrite(latchPin, LOW);

	//loop through the input bytes
	for (int j = 0; j <= 9; j++) {
		byte inputByte = inputBytes[j];
		Serial.println(inputByte);
		shiftOut(dataPin, clockPin, MSBFIRST, inputByte);
	}

	//latch the values in when done shifting
	digitalWrite(latchPin, HIGH);


	//Prepare to light up corresponding SAS LED
	switch (temp_sas_mode) {
	case SMOFF:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00000000;
		break;
	case SMSAS:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00100000;
		break;
	case SMPrograde:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B01000000;
		break;
	case SMRetroGrade:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00000010;
		break;
	case SMNormal:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B10000000;
		break;
	case SMAntinormal:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00000100;
		break;
	case SMRadialIn:
		sasInputBytes[0] = B00000001;
		sasInputBytes[1] = B00000000;
		break;
	case SMRadialOut:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00001000;
		break;
	case SMTarget:
		sasInputBytes[0] = B00000010;
		sasInputBytes[1] = B00000000;
		break;
	case SMAntiTarget:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00010000;
		break;
	case SMManeuverNode:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00000001;
		break;
	}


	//prepare the SAS LEDs shift register
	digitalWrite(sasLEDLatch, LOW);
	digitalWrite(sasLEDData, LOW);
	digitalWrite(sasLEDClock, LOW);

	//loop through the SAS LED input bytes
	for (int j = 0; j <= 1; j++) {
		byte sasInputByte = sasInputBytes[j];
		Serial.println(sasInputByte);
		shiftOut(sasLEDData, sasLEDClock, MSBFIRST, sasInputByte);
	}


	//latch the values in when done shifting
	digitalWrite(sasLEDLatch, HIGH);

	lcd_update_timer++;
	if (lcd_update_timer > 79)
	{
		display_to_lcd();
		lcd_update_timer = 0;
	}

}

int get_vessel_data() {
	define_vessel_data_display();
	int returnValue = 0;
	return returnValue;
}
