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
	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
	byte currentActionStatus;
    switch (messageType) {
	//Action Groups Messages
    case ACTIONSTATUS_MESSAGE:
        // Checking if the message is the size we expect is a very basic
        // way to confirm if the message was received properly.
		mySimpit.printToKSP("ACTIONSTATUS_MESSAGE received at Arduino...");
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
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("ACTIONSTATUS_MESSAGE processed at Arduino...");
		mySimpit.printToKSP((String)"STATE: S:"+sas_is_on+" R:"+rcs_is_on+" B:"+brakes_on+" G:"+gears_on+" L:"+lights_on);
        break;
	//Custom Action Group Messages
    case CAGSTATUS_MESSAGE:
		mySimpit.printToKSP("CAGSTATUS_MESSAGE received at Arduino...");
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
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("CAGSTATUS_MESSAGE processed at Arduino...");
		mySimpit.printToKSP((String)"CAGSTATE: C:" + chutes_on + " S:" + solar_on + " L:" + ladder_on + " 1:" + action1_on + " 2:" + action2_on);
        break;
	//SAS Mode Messages
    case SAS_MODE_INFO_MESSAGE:
		mySimpit.printToKSP("SAS_MODE_INFO_MESSAGE received at Arduino...");
        if (msgSize == sizeof(SASInfoMessage)) {
            SASInfoMessage SASModeInfo;
            SASModeInfo = parseMessage<SASInfoMessage>(msg);
            sas_mode = SASModeInfo.currentSASMode;
        }
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("SAS_MODE_INFO_MESSAGE processed at Arduino...");
        break;
	//Fuel Messages
    case LF_MESSAGE:
		mySimpit.printToKSP("LF_MESSAGE received at Arduino...");
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage liquidFuelInfo;
            liquidFuelInfo = parseMessage<resourceMessage>(msg);
			VData.LiquidFuel = liquidFuelInfo.available;
			VData.LiquidFuelTot = liquidFuelInfo.total;
        }
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("LF_MESSAGE processed at Arduino...");
        break;
    case SF_MESSAGE:
		mySimpit.printToKSP("SF_MESSAGE received at Arduino...");
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage solidFuelInfo;
            solidFuelInfo = parseMessage<resourceMessage>(msg);
			VData.SolidFuel = solidFuelInfo.available;
			VData.SolidFuelTot = solidFuelInfo.total;
        }
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("SF_MESSAGE processed at Arduino...");
        break;
    case OX_MESSAGE:
		mySimpit.printToKSP("OX_MESSAGE received at Arduino...");
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage oxidizerInfo;
            oxidizerInfo = parseMessage<resourceMessage>(msg);
			VData.Oxidizer = oxidizerInfo.available;
			VData.OxidizerTot = oxidizerInfo.total;
        }
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("OX_MESSAGE processed at Arduino...");
        break;
    case ELECTRIC_MESSAGE:
		mySimpit.printToKSP("ELECTRIC_MESSAGE received at Arduino...");
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage ecInfo;
            ecInfo = parseMessage<resourceMessage>(msg);
			VData.ecAvailable = ecInfo.available;
			VData.ecTotal = ecInfo.total;
        }
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("ELECTRIC_MESSAGE processed at Arduino...");
        break;
    case MONO_MESSAGE:
		mySimpit.printToKSP("MONO_MESSAGE received at Arduino...");
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage monoPropInfo;
            monoPropInfo = parseMessage<resourceMessage>(msg);
			VData.monoPropAvailable = monoPropInfo.available;
			VData.monoPropTotal = monoPropInfo.total;
        }
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("MONO_MESSAGE processed at Arduino...");
        break;
    case XENON_GAS_MESSAGE:
		mySimpit.printToKSP("XENON_GAS_MESSAGE received at Arduino...");
        if (msgSize == sizeof(resourceMessage)) {
            resourceMessage xenonGasInfo;
            xenonGasInfo = parseMessage<resourceMessage>(msg);
			VData.xenonGasAvailable = xenonGasInfo.available;
			VData.xenonGasTotal = xenonGasInfo.total;
        }
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("XENON_GAS_MESSAGE processed at Arduino...");
        break;
    case CUSTOM_RESOURCE_1_MESSAGE:
		mySimpit.printToKSP("CUSTOM_RESOURCE_1_MESSAGE received at Arduino...");
        if (msgSize == sizeof(CustomResourceMessage)) {
            CustomResourceMessage customResourceInfo;
            customResourceInfo = parseMessage<CustomResourceMessage>(msg);
			VData.customResource1Available = customResourceInfo.currentResource1;
			VData.customResource1Total = customResourceInfo.maxResource1;
			VData.customResource2Available = customResourceInfo.currentResource2;
			VData.customResource2Total = customResourceInfo.maxResource2;
        }
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("CUSTOM_RESOURCE_1_MESSAGE processed at Arduino...");
        break;
    //Altitude Message
	case ALTITUDE_MESSAGE:
		mySimpit.printToKSP("ALTITUDE_MESSAGE received at Arduino...");
		if (msgSize == sizeof(altitudeMessage)) {
			altitudeMessage altitudeInfo;
			altitudeInfo = parseMessage<altitudeMessage>(msg);
			VData.Alt= altitudeInfo.sealevel;
			VData.RAlt = altitudeInfo.surface;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("ALTITUDE_MESSAGE processed at Arduino...");
		break;
	//Apoapsis and Periapsis Message
	case APSIDES_MESSAGE:
		mySimpit.printToKSP("APSIDES_MESSAGE received at Arduino...");
		if (msgSize == sizeof(apsidesMessage)) {
			apsidesMessage apsidesInfo;
			apsidesInfo = parseMessage<apsidesMessage>(msg);
			VData.AP = apsidesInfo.apoapsis;
			VData.PE = apsidesInfo.periapsis;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("APSIDES_MESSAGE processed at Arduino...");
		break;
	//Velocity Message
	case VELOCITY_MESSAGE:
		mySimpit.printToKSP("VELOCITY_MESSAGE received at Arduino...");
		if (msgSize == sizeof(velocityMessage)) {
			velocityMessage velocityInfo;
			velocityInfo = parseMessage<velocityMessage>(msg);
			VData.VOrbit = velocityInfo.orbital;
			VData.Vsurf = velocityInfo.surface;
			VData.VVI = velocityInfo.vertical;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("VELOCITY_MESSAGE processed at Arduino...");
		break;
	//Times to apoapsis and periapsis
    case APSIDESTIME_MESSAGE:
		mySimpit.printToKSP("APSIDESTIME_MESSAGE received at Arduino...");
		if (msgSize == sizeof(apsidesTimeMessage)) {
			apsidesTimeMessage apsidesTimeInfo;
			apsidesTimeInfo = parseMessage<apsidesTimeMessage>(msg);
			VData.TAp = apsidesTimeInfo.apoapsis;
			VData.TPe = apsidesTimeInfo.periapsis;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("APSIDESTIME_MESSAGE processed at Arduino...");
		break;
    //Target info message
	case TARGETINFO_MESSAGE:
		mySimpit.printToKSP("TARGETINFO_MESSAGE received at Arduino...");
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
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("TARGETINFO_MESSAGE processed at Arduino...");
		break;
    //Orbit message
	case ORBIT_MESSAGE:
		mySimpit.printToKSP("ORBIT_MESSAGE received at Arduino...");
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
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("ORBIT_MESSAGE processed at Arduino...");
		break;
    //Flight status message
	case FLIGHT_STATUS_MESSAGE:
		mySimpit.printToKSP("FLIGHT_STATUS_MESSAGE received at Arduino...");
		if (msgSize == sizeof(flightStatusMessage)) {
			flightStatusMessage flightInfo;
			flightInfo = parseMessage<flightStatusMessage>(msg);
			VData.CurrentStage = flightInfo.currentStage;
			VData.TWI = flightInfo.currentTWIndex;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("FLIGHT_STATUS_MESSAGE processed at Arduino...");
		break;
	//Airspeed message
	case AIRSPEED_MESSAGE:
		mySimpit.printToKSP("AIRSPEED_MESSAGE received at Arduino...");
		if (msgSize == sizeof(airspeedMessage)) {
			airspeedMessage airspeedInfo;
			airspeedInfo = parseMessage<airspeedMessage>(msg);
			VData.IAS = airspeedInfo.IAS;
			VData.MachNumber = airspeedInfo.mach;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("AIRSPEED_MESSAGE processed at Arduino...");
		break;
	//Rotation message
	case ROTATION_DATA:
		mySimpit.printToKSP("ROTATION_DATA received at Arduino...");
		if (msgSize == sizeof(vesselPointingMessage)) {
			vesselPointingMessage pointingInfo;
			pointingInfo = parseMessage<vesselPointingMessage>(msg);
			VData.Heading = pointingInfo.heading;
			VData.Pitch = pointingInfo.pitch;
			VData.Roll = pointingInfo.roll;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("ROTATION_DATA processed at Arduino...");
		break;
	//DeltaV message
	case DELTAV_MESSAGE:
		mySimpit.printToKSP("DELTAV_MESSAGE received at Arduino...");
		if (msgSize == sizeof(deltaVMessage)) {
			deltaVMessage deltaVInfo;
			deltaVInfo = parseMessage<deltaVMessage>(msg);
			VData.StageDeltaV = deltaVInfo.stageDeltaV;
			VData.TotalDeltaV = deltaVInfo.totalDeltaV;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("DELTAV_MESSAGE processed at Arduino...");
		break;
	//Burn Time message
	case BURNTIME_MESSAGE:
		mySimpit.printToKSP("BURNTIME_MESSAGE received at Arduino...");
		if (msgSize == sizeof(burnTimeMessage)) {
			burnTimeMessage burnTInfo;
			burnTInfo = parseMessage<burnTimeMessage>(msg);
			VData.BurnTime = burnTInfo.totalBurnTime;
			VData.BurnTimeS = burnTInfo.stageBurnTime;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("BURNTIME_MESSAGE processed at Arduino...");
		break;
	//Temperature Limits message
	case TEMP_LIMIT_MESSAGE:
		mySimpit.printToKSP("TEMP_LIMIT_MESSAGE received at Arduino...");
		if (msgSize == sizeof(tempLimitMessage)) {
			tempLimitMessage tempLimitInfo;
			tempLimitInfo = parseMessage<tempLimitMessage>(msg);
			VData.MaxOverHeat = tempLimitInfo.tempLimitPercentage;
			VData.MaxSkinOverheat = tempLimitInfo.skinTempLimitPercentage;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("TEMP_LIMIT_MESSAGE processed at Arduino...");
		break;
	//Atmospheric conditions message
	case ATMO_CONDITIONS_MESSAGE:
		mySimpit.printToKSP("ATMO_CONDITIONS_MESSAGE received at Arduino...");
		if (msgSize == sizeof(atmoConditionsMessage)) {
			atmoConditionsMessage atmoInfo;
			atmoInfo = parseMessage<atmoConditionsMessage>(msg);
			VData.Density = atmoInfo.airDensity;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("ATMO_CONDITIONS_MESSAGE processed at Arduino...");
		break;
	//Maneuver message
	case MANEUVER_MESSAGE:
		mySimpit.printToKSP("MANEUVER_MESSAGE received at Arduino...");
		if (msgSize == sizeof(maneuverMessage)) {
			maneuverMessage maneuverInfo;
			maneuverInfo = parseMessage<maneuverMessage>(msg);
			VData.MNTime = maneuverInfo.timeToNextManeuver;
			VData.MNDeltaV = maneuverInfo.deltaVNextManeuver;
			VData.MNDuration = maneuverInfo.durationNextManeuver;
			VData.MNDeltaVTotal = maneuverInfo.deltaVTotal;
		}
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("MANEUVER_MESSAGE processed at Arduino...");
		break;
	//SOI message
	case SOI_MESSAGE:
		mySimpit.printToKSP("SOI_MESSAGE received at Arduino...");
		VData.SOI = msg[0];
		mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
		mySimpit.printToKSP("SOI_MESSAGE processed at Arduino...");
		break;
    }

}

//define what to do with the vessel data here, e.g. turn on LED's, display text on the LCD
void define_vessel_data_display() {

	//Fuel LED bar charts - NEED TO USE A SHIFT REGISTER to drive the LED bar charts!
	// to be implemented
	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
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
		strVsurf += " m/s";
		strVsurf.toCharArray(bufferVsurf, 17);
		writeLCD(bufferVsurf);
		/* Unable to display acceleration for now, as KSimpit does not provide it
		//Acceleration (G)
		jumpToLineTwo();
		char bufferGee[17];
		String strGee = "Accel: ";
		strGee += String(VData.G, 0);
		strGee += " G";
		strGee.toCharArray(bufferGee, 17);
		writeLCD(bufferGee);
		*/
		//Radar Altitude (m)
		jumpToLineTwo();
		char bufferRAlt[17];
		String strRAlt = "Alt: ";
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
		mySimpit.printToKSP("Speed calculations done at Arduino...");
	}

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

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

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

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

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

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

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	if (digitalRead(pLCDx) && digitalRead(pLCDy) && !digitalRead(pLCDz)) {
		//MODE 4: Re-Entry Mode
		//MaxOverHeat
		clearLCD();
		char t[3];
		dtostrf(VData.MaxOverHeat, 3, 0, t);
		writeLCD("Heat: ");
		writeLCD(t);
		writeLCD("%");
		/* Unable to display acceleration for now, as KSimpit does not provide it
		//Acceleration (G)
		jumpToLineTwo();
		char bufferGee[17];
		String strGee = "Decel: ";
		strGee += String(VData.G, 0);
		strGee += " G";
		strGee.toCharArray(bufferGee, 17);
		writeLCD(bufferGee);
		*/
		//Radar Altitude (m)
		jumpToLineTwo();
		char bufferRAlt[17];
		String strRAlt = "Alt: ";
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

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	if (!digitalRead(pLCDx) && digitalRead(pLCDy) && !digitalRead(pLCDz)) {
		//MODE 5: Flying Mode
		//Alt
		clearLCD();
		char bufferAtl[17];
		String strAlt = "Alt:  ";
		strAlt += String(VData.Alt, 0);
		strAlt += " m/s";
		strAlt.toCharArray(bufferAtl, 17);
		writeLCD(bufferAtl);
		//Mach Number
		jumpToLineTwo();
		char bufferMachNumber[17];
		String strMach = "Mach:";
		strMach += String(VData.MachNumber, 0);
		strMach.toCharArray(bufferMachNumber, 17);
		writeLCD(bufferMachNumber);
	}

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	if (digitalRead(pLCDx) && !digitalRead(pLCDy) && !digitalRead(pLCDz)) {
		//MODE 6: Landing Mode
		//RAlt
		clearLCD();
		char bufferRAtl[17];
		String strRAlt = "RAlt: ";
		strRAlt += String(VData.RAlt, 0);
		strRAlt += " m/s";
		strRAlt.toCharArray(bufferRAtl, 17);
		writeLCD(bufferRAtl);
		//Vertical Velocity
		jumpToLineTwo();
		char bufferVVI[17];
		String strVVI = "VVI:  ";
		strVVI += String(VData.VVI, 0);
		strVVI += " m/s";
		strVVI.toCharArray(bufferVVI, 17);
		writeLCD(bufferVVI);
	}

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	if (!digitalRead(pLCDx) && !digitalRead(pLCDy) && !digitalRead(pLCDz)) {
		//MODE 7: Extra Mode
		clearLCD();
		writeLCD("KerbalController");
	}

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	//get in-game status for updating the LED statuses on the controller  
	//lights_on = ControlStatus(AGLight);
	//gears_on = ControlStatus(AGGears);
	//brakes_on = ControlStatus(AGBrakes);
	//action1_on = ControlStatus(AGCustom01);
	//action2_on = ControlStatus(AGCustom02);
	//action3_on = ControlStatus(AGCustom03);
	//action4_on = ControlStatus(AGCustom04);
	//action5_on = ControlStatus(AGCustom05);
	//action6_on = ControlStatus(AGCustom06);
	//action7_on = ControlStatus(AGCustom07);
	//ladder_on = ControlStatus(AGCustom08);
	//solar_on = ControlStatus(AGCustom09);
	//chutes_on = ControlStatus(AGCustom10);

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

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	//Fuel Gauges
	if (VData.SolidFuel != 0)
	{
		vSF = 100 * VData.SolidFuel / VData.SolidFuelTot; //percentage of solid fuel remaining
	}
	else { vSF = 0; }
	if (VData.LiquidFuelTot != 0) {
		vLF = 100 * VData.LiquidFuel / VData.LiquidFuelTot; //percentage of liquid fuel remaining in current stage
	}
	else { vLF = 0; }
	if (VData.OxidizerTot != 0) {
		vOX = 100 * VData.Oxidizer / VData.OxidizerTot; //percentage of oxidized remaining in current stage
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
	mySimpit.printToKSP("Fuel calculations done at Arduino");

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	//scale down to 0-9 for binary calculations
	SF = constrain(map(vSF, 100, 0, 0, 9), 0, 9);
	LF = constrain(map(vLF, 100, 0, 0, 9), 0, 9);
	OX = constrain(map(vOX, 100, 0, 0, 9), 0, 9);
	EL = constrain(map(vEL, 100, 0, 0, 9), 0, 9);
	MP = constrain(map(vMP, 100, 0, 0, 9), 0, 9);
	XE = constrain(map(vXE, 100, 0, 0, 9), 0, 9);
	LI = constrain(map(vLI, 100, 0, 0, 9), 0, 9);
	XX = constrain(map(vXX, 100, 0, 0, 9), 0, 9);

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	//calculate the power of 2. Now each value in binary is all zeroes and a single 1. we can use that to light one LED in each LED bar (dot mode)
	int powOX = 0.1 + pow(2, OX);
	int powEL = 0.1 + pow(2, EL);
	int powMP = 0.1 + pow(2, MP);
	int powSF = 0.1 + pow(2, SF);
	int powLF = 0.1 + pow(2, LF);
	int powXE = 0.1 + pow(2, XE);
	int powLI = 0.1 + pow(2, LI);
	int powXX = 0.1 + pow(2, XX);

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	mySimpit.printToKSP((String)"Arduino SF Fuel at: " + powSF);
	mySimpit.printToKSP((String)"Arduino LF Fuel at: " + powLF);
	mySimpit.printToKSP((String)"Arduino OX Fuel at: " + powOX);
	mySimpit.printToKSP((String)"Arduino MP Fuel at: " + powMP);
	mySimpit.printToKSP((String)"Arduino EL Fuel at: " + powEL);
	mySimpit.printToKSP((String)"Arduino XE Fuel at: " + powXE);
	mySimpit.printToKSP((String)"Arduino LI Fuel at: " + powLI);
	mySimpit.printToKSP((String)"Arduino XX Fuel at: " + powXX);

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

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

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

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

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
	mySimpit.printToKSP("Fuel display calculations done at Arduino");

	//Prepare to light up corresponding SAS LED
	switch (sas_mode) {
	case SMOFF:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00000000;
		break;
	case SMSAS:
		sasInputBytes[0] = B00100000;
		sasInputBytes[1] = B00000000;
		break;
	case SMPrograde:
		sasInputBytes[0] = B01000000;
		sasInputBytes[1] = B00000000;
		break;
	case SMRetroGrade:
		sasInputBytes[0] = B00000010;
		sasInputBytes[1] = B00000000;
		break;
	case SMNormal:
		sasInputBytes[0] = B10000000;
		sasInputBytes[1] = B00000000;
		break;
	case SMAntinormal:
		sasInputBytes[0] = B00000100;
		sasInputBytes[1] = B00000000;
		break;
	case SMRadialIn:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00000001;
		break;
	case SMRadialOut:
		sasInputBytes[0] = B00001000;
		sasInputBytes[1] = B00000000;
		break;
	case SMTarget:
		sasInputBytes[0] = B00000000;
		sasInputBytes[1] = B00000010;
		break;
	case SMAntiTarget:
		sasInputBytes[0] = B00010000;
		sasInputBytes[1] = B00000000;
		break;
	case SMManeuverNode:
		sasInputBytes[0] = B00000001;
		sasInputBytes[1] = B00000000;
		break;
	}

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

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

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);

	//latch the values in when done shifting
	digitalWrite(sasLEDLatch, HIGH);

	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
	mySimpit.printToKSP("SAS display calculations done at Arduino");
}

int get_vessel_data() {
	mySimpit.printToKSP((String)"DBG: " + __LINE__ + ":" + __FUNCTION__);
	define_vessel_data_display();
	int returnValue = 0;
	return returnValue;
}


// No longer used either
/*
//Enumeration of ActionGroups (includes main controls and custom action groups)
#define AGSAS      0
#define AGRCS      1
#define AGLight    2
#define AGGears    3
#define AGBrakes   4
#define AGAbort    5
#define AGCustom01 6
#define AGCustom02 7
#define AGCustom03 8
#define AGCustom04 9
#define AGCustom05 10
#define AGCustom06 11
#define AGCustom07 12
#define AGCustom08 13
#define AGCustom09 14
#define AGCustom10 15

//get the current state of main controls and custom action groups using enumeration above, e.g. ControlStatus(AGBrakes);
byte ControlStatus(byte n)
{
  return ((VData.ActionGroups >> n) & 1) == 1;
}

//get the current SAS Mode. Can be compared to enum values, e.g. if(getSASMode() == SMPrograde){//do stuff}
byte getSASMode() {
  return VData.NavballSASMode & B00001111; // leaves alone the lower 4 bits of; all higher bits set to 0.
}

//get the current navball mode. Can be compared to enum values, e.g. if (getNavballMode() == NAVBallTARGET){//do stuff}
byte getNavballMode() {
  return VData.NavballSASMode >> 4; // leaves alone the higher 4 bits of; all lower bits set to 0.
}

//get the current vessel view. Can be compared to enum values, e.g. if (getCameraMode() == STAGEVIEW){//do stuff}
byte getCameraMode() {
  return VData.CameraMode;
}
*/




