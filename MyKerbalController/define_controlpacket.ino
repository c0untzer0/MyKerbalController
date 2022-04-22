//Define what controls to send to KSP

void rotary_helper(bool clockwise)
{
	if (clockwise)
	{
		if (temp_sas_mode > 9) { temp_sas_mode = 0; }
		if ((temp_sas_mode == 7 || temp_sas_mode == 8) && (!sas_target_set))
		{
			temp_sas_mode = 9;
		}
		if ((temp_sas_mode == 9) && !sas_maneuver_set) { temp_sas_mode = 0; }
	}
	else
	{
		if (temp_sas_mode < 0) { temp_sas_mode = 9; }
		if ((temp_sas_mode == 9) && !sas_maneuver_set) { temp_sas_mode = 8; }
		if ((temp_sas_mode == 7 || temp_sas_mode == 8) && (!sas_target_set))
		{
			temp_sas_mode = 6;
		}
	}
}


void check_rotary() {
	sasCurrentCLK = digitalRead(sasClockPin);

	/*
	if (sasCurrentCLK != sasPreviousCLK)
	{
		if (digitalRead(sasDataPin) != sasCurrentCLK) {
			temp_sas_mode++;
			mySimpit.printToKSP("SAS Rotary movement CW");
			rotary_helper(true);
		}
		else
		{
			temp_sas_mode--;
			mySimpit.printToKSP("SAS Rotary movement CCW");
			rotary_helper(false);
		}
	}
	*/

	/*
	if ((sasPreviousCLK == 0) && (sasPreviousDATA == 1)) {
		if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 0)) {
			mySimpit.printToKSP("SAS Rotary movement CW");
			temp_sas_mode++;
			rotary_helper(true);
		}
		else if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 1)) {
			mySimpit.printToKSP("SAS Rotary movement CCW");
			temp_sas_mode--;
			rotary_helper(false);
		}
	} else if ((sasPreviousCLK == 1) && (sasPreviousDATA == 0)) {
		if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 1)) {
			mySimpit.printToKSP("SAS Rotary movement CW");
			temp_sas_mode++;
			rotary_helper(true);
		}
		else if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 0)) {
			mySimpit.printToKSP("SAS Rotary movement CCW");
			temp_sas_mode--;
			rotary_helper(false);
		}
	} else if ((sasPreviousCLK == 1) && (sasPreviousDATA == 1)) {
		if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 1)) {
			mySimpit.printToKSP("SAS Rotary movement CW");
			temp_sas_mode++;
			rotary_helper(true);
		}
		else if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 0)) {
			mySimpit.printToKSP("SAS Rotary movement CCW");
			temp_sas_mode--;
			rotary_helper(false);
		}
	} else if ((sasPreviousCLK == 0) && (sasPreviousDATA == 0)) {
		if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 0)) {
			mySimpit.printToKSP("SAS Rotary movement CW");
			temp_sas_mode++;
			rotary_helper(true);
		}
		else if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 1)) {
			mySimpit.printToKSP("SAS Rotary movement CCW");
			temp_sas_mode--;
			rotary_helper(false);
		}
	}
	*/
	//Each stop in the encoder is two steps
	if ((sasPreviousCLK == 1) && (sasPreviousDATA == 1)) {
		if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 1)) {
			mySimpit.printToKSP("SAS Rotary movement CW");
			temp_sas_mode++;
			rotary_helper(true);
		}
		else if ((digitalRead(sasClockPin) == 0) && (digitalRead(sasDataPin) == 0)) {
			mySimpit.printToKSP("SAS Rotary movement CCW");
			temp_sas_mode--;
			rotary_helper(false);
		}
	}
	else if ((sasPreviousCLK == 0) && (sasPreviousDATA == 0)) {
		if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 0)) {
			mySimpit.printToKSP("SAS Rotary movement CW");
			temp_sas_mode++;
			rotary_helper(true);
		}
		else if ((digitalRead(sasClockPin) == 1) && (digitalRead(sasDataPin) == 1)) {
			mySimpit.printToKSP("SAS Rotary movement CCW");
			temp_sas_mode--;
			rotary_helper(false);
		}
	}
	
	
}

void check_and_send_data() {

	//Check SAS Encoder
	// If enough time has passed check the rotary encoder
	if (((millis() - sasTimeOfLastDebounce) > sasDelayofDebounce) && sas_is_on) {

		check_rotary();  // Rotary Encoder check routine below

		sasPreviousCLK = digitalRead(sasClockPin);
		sasPreviousDATA = digitalRead(sasDataPin);

		sasTimeOfLastDebounce = millis();  // Set variable to current millis() timer
	}

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
	if (digitalRead(pSAS) && !sas_is_on) {
		mySimpit.activateAction(SAS_ACTION);
		sas_led_on = true;
		sas_is_on = true;
		temp_sas_mode = 0;
	}
	else if (!digitalRead(pSAS) && sas_is_on) {
		mySimpit.deactivateAction(SAS_ACTION);
		sas_led_on = false;
		sas_is_on = false;
		temp_sas_mode = 255;
	}
	if (digitalRead(pRCS) && !rcs_is_on) {
		mySimpit.activateAction(RCS_ACTION);
		rcs_led_on = true;
	}
	else if (!digitalRead(pRCS) && rcs_is_on) {
		mySimpit.deactivateAction(RCS_ACTION);
		rcs_led_on = false;
	}

	//momentary abort button
	if (b_ABORTBTN.pressed() && digitalRead(pABORT)) {
		mySimpit.activateAction(ABORT_ACTION);
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
		lights_on = !lights_on;
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
	throttle_msg.throttle = constrain(map(analogRead(pTHROTTLE), 30, 990, INT16_MAX, 0), 0, INT16_MAX);
	if (old_throttle != throttle_msg.throttle) {
		mySimpit.send(THROTTLE_MESSAGE, throttle_msg);
		old_throttle = throttle_msg.throttle;
	}

	//rotation joystick button toggles flight control modes
	if (!digitalRead(pRB))
	{
		rb_on = !rb_on;
	}

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
		if (analogRead(pRX) >= 530) { yaw = constrain(map(analogRead(pRX), 550, 1019, 0, INT16_MIN), INT16_MIN, 0); }
		else if (analogRead(pRX) <= 470) { yaw = constrain(map(analogRead(pRX), 0, 450, INT16_MAX, 0), 0, INT16_MAX); }
		if (analogRead(pRY) >= 530) { pitch = constrain(map(analogRead(pRY), 530, 1019, 0, INT16_MAX), 0, INT16_MAX); }
		else if (analogRead(pRY) <= 470) { pitch = constrain(map(analogRead(pRY), 0, 470, INT16_MIN, 0), INT16_MIN, 0); }
		if (analogRead(pRZ) <= 470) { roll = constrain(map(analogRead(pRZ), 0, 470, INT16_MIN, 0), INT16_MIN, 0); }
		else if (analogRead(pRZ) >= 530) { roll = constrain(map(analogRead(pRZ), 530, 1019, 0, INT16_MAX), 0, INT16_MAX); }

		if (analogRead(pTX) >= 530) { tr_x = constrain(map(analogRead(pTX), 550, 1019, 0, INT16_MAX), 0, INT16_MAX); }
		else if (analogRead(pTX) <= 470) { tr_x = constrain(map(analogRead(pTX), 0, 490, INT16_MIN, 0), INT16_MIN, 0); }
		if (analogRead(pTY) >= 530) { tr_y = constrain(map(analogRead(pTY), 1019, 530, INT16_MIN, 0), INT16_MIN, 0); }
		else if (analogRead(pTY) <= 470) { tr_y = constrain(map(analogRead(pTY), 470, 0, 0, INT16_MAX), 0, INT16_MAX); }
		if (analogRead(pTZ) <= 470) { tr_z = constrain(map(analogRead(pTZ), 0, 470, INT16_MIN, 0), INT16_MIN, 0); }
		else if (analogRead(pTZ) >= 530) { tr_z = constrain(map(analogRead(pTZ), 530, 1019, 0, INT16_MAX), 0, INT16_MAX); }
	}
	else {
		//airplane mode
		if (analogRead(pRX) >= 530) { roll = constrain(map(analogRead(pRX), 1019, 550, INT16_MAX, 0), 0, INT16_MAX); }
		else if (analogRead(pRX) <= 470) { roll = constrain(map(analogRead(pRX), 450, 0, 0, INT16_MIN), INT16_MIN, 0); }
		if (analogRead(pRY) >= 530) { pitch = constrain(map(analogRead(pRY), 530, 1019, 0, INT16_MAX), 0, INT16_MAX); }
		else if (analogRead(pRY) <= 470) { pitch = constrain(map(analogRead(pRY), 0, 470, INT16_MIN, 0), INT16_MIN, 0); }
		if (analogRead(pTX) >= 530) { yaw = constrain(map(analogRead(pTX), 1019, 530, INT16_MIN, 0), INT16_MIN, 0); }
		else if (analogRead(pTX) <= 470) { yaw = constrain(map(analogRead(pTX), 470, 0, 0, INT16_MAX), 0, INT16_MAX); }
	}
	rot_msg.setPitchRollYaw(pitch, roll, yaw);
	tra_msg.setXYZ(tr_x, tr_y, tr_z);
	mySimpit.send(ROTATION_MESSAGE, rot_msg);
	mySimpit.send(TRANSLATION_MESSAGE, tra_msg);

	//translation joystick button switches to next vessel
	if (b_TB.pressed())
	{
		//Next vessel switch behavior instead
		keyboardEmulatorMessage keystroke_down(0xDD);
		mySimpit.send(KEYBOARD_EMULATOR, keystroke_down);
		//cameraRotationMessage cam_rot;
		//cam_rot.setPitchRollYawZoom(0, 0, 0, 0);
		//mySimpit.send(CAMERA_ROTATION_MESSAGE, cam_rot);
	}


	//SAS Selector Button sends SAS mode selected
	if (b_SASBTN.pressed()) {
  // Eventually change this behavior to a sas setting timer
		if ((temp_sas_mode != sas_mode) && sas_is_on)
		{
			mySimpit.setSASMode(temp_sas_mode);
		}
	}

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
	}
	if (b_CAMMODEDOWN.pressed())
	{
		//Change camera mode if switch is toggled down
		if (cameraMode == 6) {
			cameraMode = 1;
		}
		else {
			cameraMode++;
		}
		mySimpit.setCameraMode(cameraMode);
	}
	
}
