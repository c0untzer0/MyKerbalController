void setHighLCDRate()
{
  mySerial.write(0x7C);
  mySerial.write(0x10);
}

void jumpToStart()
{
	//Jump to the start of the first line on the LCD
	mySerial.write(254);
	mySerial.write(128);
}

void clearLCD()
{
	//Clear the LCD by writing all spaces
	jumpToStart();
	mySerial.write("                ");
	mySerial.write("                ");
	jumpToStart();
}

void jumpToLineTwo()
{
	//Jump to the start of the second line on the LCD
	mySerial.write(254);
	mySerial.write(192);
}

void writeLCD(char myText[])
{
	//Write text to the LCD
	mySerial.write(myText);
}
