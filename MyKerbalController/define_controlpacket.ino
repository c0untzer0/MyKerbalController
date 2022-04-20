//Define what controls to send to KSP

//Define the SAS Rotary encoder checks and actions
void check_rotary() {

    int temp_sas_mode = sas_mode;
    //Remove the step logic for middle steps, as two steps make one SAS change
    //if ((sasPreviousCLK == 0) && (sasPreviousDATA == 1)) {
    //  if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 0)) {
    //    sas_mode ++;
    //  }
    //  if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 1)) {
    //    sas_mode --;
    //  }
    //}

    //if ((sasPreviousCLK == 1) && (sasPreviousDATA == 0)) {
    //  if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 1)) {
    //    sas_mode ++;
    //  }
    //  if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 0)) {
    //    sas_mode --;
    //  }
    //}

    if ((sasPreviousCLK == 1) && (sasPreviousDATA == 1)) {
        if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 1)) {
            temp_sas_mode++;
        }
        if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 0)) {
            temp_sas_mode--;
        }
    }

    if ((sasPreviousCLK == 0) && (sasPreviousDATA == 0)) {
        if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 0)) {
            temp_sas_mode++;
        }
        if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 1)) {
            temp_sas_mode--;
        }
    }
    if (temp_sas_mode > 10) {
        temp_sas_mode = 1;
    }
    else if (temp_sas_mode < 1) {
        temp_sas_mode = 10;
    }
    sas_mode = temp_sas_mode;
}

void check_and_send_data() {

	//here we define what controls to send when which pins are manipulated

	//First, update all temporary switches using Bounce2
	b_TB.update();
	b_RB.update();
	b_SASBTN.update();
	b_ABORTBTN.update();
	b_STAGE.update();
	b_LIGHTS.update();
	b_LADDER.update();
	b_SOLAR.update();
	b_CHUTES.update();
	b_GEARS.update();
	b_BRAKES.update();
	b_ACTION1.update();
	b_ACTION2.update();
	b_ACTION3.update();
	b_ACTION4.update();
	b_ACTION5.update();
	b_ACTION6.update();
	b_ACTION7.update();
	b_CAMMODEUP.update();
	b_CAMMODEDOWN.update();

	//toggle switches
	if (digitalRead(pSAS)) {
		mySimpit.activateAction(SAS_ACTION);
	}
	else {
		mySimpit.deactivateAction(SAS_ACTION);
	}
	if (digitalRead(pRCS)) {
		mySimpit.activateAction(RCS_ACTION);
	}
	else {
		mySimpit.deactivateAction(RCS_ACTION);
	}

	//momentary abort button
	if (!digitalRead(pABORTBTN) && digitalRead(pABORT)) { mySimpit.activateAction(ABORT_ACTION); }
	else { mySimpit.deactivateAction(ABORT_ACTION); }
	if (digitalRead(pABORT)) {
		now = millis();
		abortLedTime = now - abortLedTimeOld;
		if (abortLedTime > abortLedBlinkTime) {
			abortLedTimeOld = now;
			abort_led_on = !abort_led_on;
		}
	}
	else { abort_led_on = false; }
	digitalWrite(pABORTBTNLED, abort_led_on);

	//momentary stage button
	if (!digitalRead(pSTAGE) && digitalRead(pARM)) { mySimpit.activateAction(STAGE_ACTION); }
	else { mySimpit.deactivateAction(STAGE_ACTION); }
	if (digitalRead(pARM)) {
		now = millis();
		stageLedTime = now - stageLedTimeOld;
		if (stageLedTime > stageLedBlinkTime) {
			stageLedTimeOld = now;
			stage_led_on = !stage_led_on;
		}
	}
	else { stage_led_on = false; }
	digitalWrite(pSTAGELED, stage_led_on);

	//toggle buttons
	if (!digitalRead(pLIGHTS)) { mySimpit.toggleAction(LIGHT_ACTION); }
	if (!digitalRead(pGEARS)) { mySimpit.toggleAction(GEAR_ACTION); }
	if (!digitalRead(pBRAKES)) { mySimpit.toggleAction(BRAKES_ACTION); }
	if (!digitalRead(pACTION1)) { mySimpit.toggleCAG(1); }
	if (!digitalRead(pACTION2)) { mySimpit.toggleCAG(2); }
	if (!digitalRead(pACTION3)) { mySimpit.toggleCAG(3); }
	if (!digitalRead(pACTION4)) { mySimpit.toggleCAG(4); }
	if (!digitalRead(pACTION5)) { mySimpit.toggleCAG(5); }
	if (!digitalRead(pACTION6)) { mySimpit.toggleCAG(6); }
	if (!digitalRead(pACTION7)) { mySimpit.toggleCAG(7); }
	if (!digitalRead(pLADDER)) { mySimpit.toggleCAG(8); }
	if (!digitalRead(pSOLAR)) { mySimpit.toggleCAG(9); }
	if (!digitalRead(pCHUTES)) { mySimpit.toggleCAG(10); }

	//throttle
	throttleMessage throttle_msg;
	throttle_msg.throttle = map(analogRead(pTHROTTLE), 30, 990, INT16_MAX, 0);
	mySimpit.send(THROTTLE_MESSAGE, throttle_msg);
	
	//rotation joystick button toggles flight control modes
	if (!digitalRead(pRB) && !rb_prev) { rb_on = !rb_on; rb_prev = true; }
	if (digitalRead(pRB) && rb_prev) { rb_prev = false; }
	rotationMessage rot_msg;
	translationMessage tra_msg;
	int16_t pitch = 0;
	int16_t roll = 0;
	int16_t yaw = 0;
	int16_t tr_x = 0;
	int16_t tr_y = 0;
	int16_t tr_z = 0;
	if (rb_on) {
		//rocket mode
		if (analogRead(pRX) >= 530) { yaw = map(analogRead(pRX), 1019, 530, INT16_MIN, 0); }
		else if (analogRead(pRX) <= 470) { yaw = map(analogRead(pRX), 470, 0, 0, INT16_MAX); }
		if (analogRead(pRY) >= 530) { pitch = map(analogRead(pRY), 530, 1019, 0, INT16_MAX); }
		else if (analogRead(pRY) <= 470) { pitch = map(analogRead(pRY), 0, 470, INT16_MIN, 0); }
		if (analogRead(pRZ) <= 470) { roll = map(analogRead(pRZ), 0, 470, INT16_MIN, 0); }
		else if (analogRead(pRZ) >= 530) { roll = map(analogRead(pRZ), 530, 1019, 0, INT16_MAX); }

		if (analogRead(pTX) >= 530) { tr_x = map(analogRead(pTX), 1019, 530, 0, INT16_MAX); }
		else if (analogRead(pTX) <= 470) { tr_x = map(analogRead(pTX), 470, 0, INT16_MIN, 0); }
		if (analogRead(pTY) >= 530) { tr_y = map(analogRead(pTY), 1019, 530, INT16_MIN, 0); }
		else if (analogRead(pTY) <= 470) { tr_y = map(analogRead(pTY), 470, 0, 0, INT16_MAX); }
		if (analogRead(pTZ) <= 470) { tr_z = map(analogRead(pTZ), 0, 470, INT16_MIN, 0); }
		else if (analogRead(pTZ) >= 530) { tr_z = map(analogRead(pTZ), 530, 1019, 0, INT16_MAX); }
	}
	else {
		//airplane mode
		if (analogRead(pRX) >= 530) { roll = map(analogRead(pRX), 1019, 530, INT16_MIN, 0); }
		else if (analogRead(pRX) <= 470) { roll = map(analogRead(pRX), 470, 0, 0, INT16_MAX); }
		if (analogRead(pRY) >= 530) { pitch = map(analogRead(pRY), 530, 1019, 0, INT16_MAX); }
		else if (analogRead(pRY) <= 470) { pitch = map(analogRead(pRY), 0, 470, INT16_MIN, 0); }
		if (analogRead(pTX) >= 530) { yaw = map(analogRead(pTX), 1019, 530, INT16_MIN, 0); }
		else if (analogRead(pTX) <= 470) { yaw = map(analogRead(pTX), 470, 0, 0, INT16_MAX); }
	}
	rot_msg.setPitchRollYaw(pitch, roll, yaw);
	tra_msg.setXYZ(tr_x, tr_y, tr_z);

	//translation joystick button resets camera view to neutral
	if (!digitalRead(pTB) && !tb_prev) { tb_on = !tb_on; tb_prev = true; }
	if (digitalRead(pTB) && tb_prev) { tb_prev = false; }
	if (tb_on)
	{
		cameraRotationMessage cam_rot;
		cam_rot.setPitchRollYawZoom(0, 0, 0, 0);
	}

	//SAS Selector Button toggles SAS Target modes
	if (!digitalRead(pSASBTN) && !sasb_prev) { sasb_on = !sasb_on; sasb_prev = true; }
	if (digitalRead(pSASBTN) && sasb_prev) { sasb_prev = false; }
	if (sasb_on) {
		target_mode = getNavballMode();
		if (target_mode == 3) {
			target_mode = 0;
		}
		else {
			target_mode++;
		}
		setNavballMode(target_mode);
	}

	//SAS Selector Rotary Encoder for changing SAS modes
	// If enough time has passed check the rotary encoder
	if ((millis() - sasTimeOfLastDebounce) > sasDelayofDebounce) {
		check_rotary();  // Rotary Encoder check routine defined above
		sasPreviousCLK = digitalRead(sasClockPin);
		sasPreviousDATA = digitalRead(sasDataPin);
		sasTimeOfLastDebounce = millis();  // Set variable to current millis() timer
	}

	//Toggle switch for selecting camera mode
	cameraMode = 0;
	if (digitalRead(pCAMMODEUP)) {
		cameraMode = 1;
	}
	else if (digitalRead(pCAMMODEDOWN)) {
		cameraMode = -1;
	}
	setCameraMode(cameraMode);

	//send the control packet to the KSPSerialIO plugin
	KSPBoardSendData(details(CPacket));
}


//check if it is time to send control data
void send_control_data() {
    now = millis();
    controlTime = now - controlTimeOld;
    if (controlTime > CONTROLREFRESH) {
        controlTimeOld = now;
        check_and_send_data();
    }
}

//define the structure of a control packet
struct ControlPacket {
    //the following controls can be sent to the KSPSerialIO plugin (defined by the plugin)
    byte id;
    byte MainControls;                  //SAS RCS Lights Gears Brakes Precision Abort Stage (see enum)
    byte Mode;                          //0 = stage, 1 = docking, 2 = map
    int CameraModeSwitch;                     //Camera Mode (0 = No change, 1 = Mode Up, -1 = Mode down)
    unsigned int ControlGroup;          //action groups 1-10
    byte NavballSASMode;                //AutoPilot mode
    byte AdditionalControlByte1;
    int Pitch;                          //-1000 -> 1000
    int Roll;                           //-1000 -> 1000
    int Yaw;                            //-1000 -> 1000
    int TX;                             //-1000 -> 1000
    int TY;                             //-1000 -> 1000
    int TZ;                             //-1000 -> 1000
    int WheelSteer;                     //-1000 -> 1000
    int Throttle;                       //    0 -> 1000
    int WheelThrottle;                  //    0 -> 1000
};

//Create an instance of a control packet
ControlPacket CPacket;

//macro used to generate the control packet (also used for the handshake packet)
#define details(name) (uint8_t*)&name,sizeof(name)

//Control groups (action groups) uses an integer to refer to a custom action group, e.g. ControlGroup(5,true);
void ControlGroups(byte n, boolean s) {
    if (s)
        CPacket.ControlGroup |= (1 << n);       // forces nth bit of x to be 1.  all other bits left alone.
    else
        CPacket.ControlGroup &= ~(1 << n);      // forces nth bit of x to be 0.  all other bits left alone.
}

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

//Vessel camera mode 
void setCameraMode(int c) {
    CPacket.CameraModeSwitch = c;
}




