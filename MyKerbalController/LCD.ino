void setHighLCDRate()
{
  Serial2.write(0x7C);
  Serial2.write(0x10);
}

void jumpToStart()
{
	//Jump to the start of the first line on the LCD
	Serial2.write(254);
	Serial2.write(128);
}

void clearLCD()
{
	//Clear the LCD by writing all spaces
	jumpToStart();
	Serial2.write("                ");
	Serial2.write("                ");
	jumpToStart();
}

void jumpToLineTwo()
{
	//Jump to the start of the second line on the LCD
	Serial2.write(254);
	Serial2.write(192);
}

void writeLCD(char myText[])
{
	//Write text to the LCD
	Serial2.write(myText);
}
