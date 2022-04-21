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
	if (sas_mode != prev_sas_mode) { mySimpit.setSASMode(sas_mode); }
	mySimpit.printToKSP("SAS mode sent from Arduino...");
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

	mySimpit.printToKSP("Bounce switches updated at Arduino...");

	//toggle switches
	if (!digitalRead(pSAS) && !sas_is_on) {
		mySimpit.activateAction(SAS_ACTION);
		mySimpit.printToKSP("SAS active sent from Arduino...");
	}
	else if (sas_is_on) {
		mySimpit.deactivateAction(SAS_ACTION);
		mySimpit.printToKSP("SAS deactive sent from Arduino...");
	}
	if (!digitalRead(pRCS) && !rcs_is_on) {
		mySimpit.activateAction(RCS_ACTION);
		mySimpit.printToKSP("RCS active sent from Arduino...");
	}
	else if (rcs_is_on) {
		mySimpit.deactivateAction(RCS_ACTION);
		mySimpit.printToKSP("RCS deactive sent from Arduino...");
	}

	//momentary abort button
	if (b_ABORTBTN.pressed() && digitalRead(pABORT)) {
		mySimpit.activateAction(ABORT_ACTION);
		mySimpit.printToKSP("Abort active sent from Arduino...");
	}
	//else { mySimpit.deactivateAction(ABORT_ACTION); }
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
	if (b_STAGE.pressed() && digitalRead(pARM))
	{
		mySimpit.activateAction(STAGE_ACTION);
		mySimpit.printToKSP("Stage active sent from Arduino...");
	}
	//else { mySimpit.deactivateAction(STAGE_ACTION); }
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
	if (b_LIGHTS.pressed()) {
		mySimpit.toggleAction(LIGHT_ACTION);
		mySimpit.printToKSP("Lights toggle sent from Arduino...");
	}
	if (b_GEARS.pressed()) { mySimpit.toggleAction(GEAR_ACTION); }
	if (b_BRAKES.pressed()) { mySimpit.toggleAction(BRAKES_ACTION); }
	if (b_ACTION1.pressed()) { mySimpit.toggleCAG(1); }
	if (b_ACTION2.pressed()) { mySimpit.toggleCAG(2); }
	if (b_ACTION3.pressed()) { mySimpit.toggleCAG(3); }
	if (b_ACTION4.pressed()) { mySimpit.toggleCAG(4); }
	if (b_ACTION5.pressed()) { mySimpit.toggleCAG(5); }
	if (b_ACTION6.pressed()) { mySimpit.toggleCAG(6); }
	if (b_ACTION7.pressed()) { mySimpit.toggleCAG(7); }
	if (b_LADDER.pressed()) { mySimpit.toggleCAG(8); }
	if (b_SOLAR.pressed()) { mySimpit.toggleCAG(9); }
	if (b_CHUTES.pressed()) { mySimpit.toggleCAG(10); }

	//throttle
	throttleMessage throttle_msg;
	throttle_msg.throttle = map(analogRead(pTHROTTLE), 30, 990, INT16_MAX, 0);
	if (old_throttle != throttle_msg.throttle) {
		mySimpit.send(THROTTLE_MESSAGE, throttle_msg);
		mySimpit.printToKSP("Throttle sent from Arduino...");
		old_throttle = throttle_msg.throttle;
	}
	
	//rotation joystick button toggles flight control modes
	if (b_RB.pressed() && !rb_prev) { rb_on = !rb_on; rb_prev = true; }
	if (!b_RB.pressed() && rb_prev) { rb_prev = false; }

	//Send Joysticks info
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
	mySimpit.printToKSP("Joysticks info sent from Arduino...");
	

	//translation joystick button resets camera view to neutral
	if (b_TB.pressed() && !tb_prev) { tb_on = !tb_on; tb_prev = true; }
	if (!b_TB.pressed() && tb_prev) { tb_prev = false; }
	if (tb_on)
	{
		cameraRotationMessage cam_rot;
		cam_rot.setPitchRollYawZoom(0, 0, 0, 0);
	}
	mySimpit.printToKSP("Joysticks button1 info sent from Arduino...");

	/* No Target modes on KSimpit
	//SAS Selector Button toggles SAS Target modes
	if (b_SASBTN.pressed() && !sasb_prev) { sasb_on = !sasb_on; sasb_prev = true; }
	if (!b_SASBTN.pressed() && sasb_prev) { sasb_prev = false; }
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
	*/

	//SAS Selector Button cycles camera views
	if (b_SASBTN.pressed()) {
		if (cameraMode == 6) {
			cameraMode = 1;
		}
		else {
			cameraMode++;
		}
		mySimpit.setCameraMode(cameraMode);
	}

	//SAS Selector Rotary Encoder for changing SAS modes
	// If enough time has passed check the rotary encoder
	if ((millis() - sasTimeOfLastDebounce) > sasDelayofDebounce) {
		if (sas_mode != 255) { check_rotary(); }  // Rotary Encoder check routine defined above
		sasPreviousCLK = digitalRead(sasClockPin);
		sasPreviousDATA = digitalRead(sasDataPin);
		sasTimeOfLastDebounce = millis();  // Set variable to current millis() timer
	}
	mySimpit.printToKSP("SAS button info sent from Arduino...");

	/*Changing the behavior of camera switch until I have better integration with API
	//Toggle switch for selecting camera mode
	cameraMode = 0;
	if (b_CAMMODEUP.pressed()) {
		cameraMode = 1;
	}
	else if (b_CAMMODEDOWN.pressed()) {
		cameraMode = -1;
	}
	setCameraMode(cameraMode);
	*/
	//Set the View toggle switch actions
	if (b_CAMMODEUP.pressed())
	{
		//Map view toggle if the switch is toggled up
		keyboardEmulatorMessage keystroke_up(0x4D);
		mySimpit.send(KEYBOARD_EMULATOR, keystroke_up);
		mySimpit.printToKSP("View button1 info sent from Arduino...");
	}
	if (b_CAMMODEDOWN.pressed())
	{
		//Next vessel switch if switch is toggled down
		keyboardEmulatorMessage keystroke_down(0xDD);
		mySimpit.send(KEYBOARD_EMULATOR, keystroke_down);
		mySimpit.printToKSP("View button2 info sent from Arduino...");
	}
}

/* Maybe not needed?
//check if it is time to send control data
void send_control_data() {
    now = millis();
    controlTime = now - controlTimeOld;
    if (controlTime > CONTROLREFRESH) {
        controlTimeOld = now;
        check_and_send_data();
    }
}
*/






