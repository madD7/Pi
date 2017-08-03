/*
 * Terminal.cpp
 *
 *  Created on: 05-May-2016
 *      Author: Dhaval Solanki
 */
#include "Terminal.hpp"




bool Terminal::instanceFlag = false;
Terminal* Terminal::pTerm = NULL;

Terminal::Terminal()
{
	portnum = 0;
	baudrate = 0;
	rxflg = false;
	txflg = false;
	txEchoflg = false;
	logtofileflg = false;
	printAsciiflg = true;
	rtsstatus = true;
	dtrstatus = true;

	memset(chnname, 0, sizeof(chnname));

	Util.log(DEVELOPER , "Terminal Constructor\n");
}



Terminal::~Terminal()
{
	if(instanceFlag)
	{
		instanceFlag = false;

		Util.log(DEVELOPER , "Closing Terminal\n");
		Util.message("Closing Terminal\n");

		#ifdef COMPILE_FOR_LINUX
			tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
		#endif

		delete port;
		port = NULL;
	}

	Util.log(DEVELOPER , "Terminal Destructor\n");
}



Terminal* Terminal::getInstance()
{
	if(!instanceFlag)
	{
		pTerm = new Terminal();
		instanceFlag = true;
	}

	return pTerm;
}


int Terminal::parsePortNumber()
{
	CliParser *pcli = CliParser::getInstance();

	char temp[4];
	int val=-1;

	pcli->getParamValue(PARAM_N, temp, sizeof(temp)-1);
	if(!pcli->checkParamVal(temp))
	{
		Util.message("No input parameter value specified for Port Number. \n");
		Util.message(3, "Specify Serial Port Number with -n parameter. Eg: -n0 \n");
		Util.log("No input parameter value specified for Port Number. \n");
		Util.log(3, "Specify Serial Port Number with -n parameter. Eg: -n0 \n");
		throw ErrorManager::getRecord(NoInputSpecified);
	}
	else
	{
		if(!isdigit(temp[0]))
		{
			Util.message( "Expected a numeric input for Port number \n");
			Util.log( "Expected a numeric input for Port number \n");
			throw ErrorManager::getRecord(InvalidInput);
		}
		val = atoi(temp);
	}
	return val;
}



unsigned long Terminal::parseBaudrate()
{
	CliParser *pcli = CliParser::getInstance();

	char temp[10];
	unsigned long val=0;

	pcli->getParamValue(PARAM_B, temp, sizeof(temp)-1);
	if(!pcli->checkParamVal(temp))
	{
		Util.message( "Using default value for Serial-baudrate/Jtag-frequency\n");
		Util.message( "Default Baudrate = 115200 \n");
		val = 115200;
	}
	else
	{
		if(!isdigit(temp[0]))
		{
			Util.message( "Expected a numeric input for baudrate/frequency.\n");
			Util.message( "Using default value for Serial-baudrate/Jtag-frequency\n");
			Util.message( "Default Baudrate = 115200 \n");
			val = 115200;
		}
		else
		{
			val = atoi(temp);
		}
	}
	return val;
}



void Terminal::setChannelName()
{
	CliParser *pcli = CliParser::getInstance();
	char temp[15];
	char printbuf[50]={};

	pcli->getParamValue(PARAM_C,temp, sizeof(temp)-1);
	if(!pcli->checkParamVal(temp))
	{
		Util.message("No input parameter value specified for Channel Name. ");
		Util.message(3, "Specify channel name with -c parameter. Eg: -cSerial or -cJtag\n");
		Util.log("No input parameter value specified for Channel Name. ");
		Util.log(3, "Specify channel name with -c parameter. Eg: -cSerial or -cJtag\n");
		throw ErrorManager::getRecord(NoInputSpecified);
	}
	else
	{
		strcpy(chnname, temp);
		sprintf(printbuf, "Channel %s was selected\n",temp);
		Util.log(DEVELOPER, printbuf);
	}
}



char Terminal::parseAsciiPrint()
{
	CliParser *pcli = CliParser::getInstance();
	char temp[10];

	pcli->getParamValue(PARAM_A, temp, 1);
	if(!pcli->checkParamVal(temp))
	{
		Util.message("Default: Ascii print on terminal \n");
		Util.log("Default: Ascii print on terminal \n");
	}
	else
	{
		if(temp[0] == '1')
		{
			Util.message("Ascii print enabled on terminal\n");
			Util.log("Ascii print enabled on terminal\n");
			return true;
		}
		else if(temp[0] == '0')
		{
			Util.message("Hex print enabled on terminal\n");
			Util.log("Hex print enabled on terminal\n");
			return false;
		}
		else
		{
			Util.message("Invalid input. Default: Ascii print on terminal \n");
			Util.log("Invalid input. Default: Ascii print on terminal \n");
		}
	}

	return true;
}


void Terminal::parseEchoFlag()
{
	CliParser *pcli = CliParser::getInstance();
	char temp[5];

	pcli->getParamValue(PARAM_E, temp, 1);
	if(!pcli->checkParamVal(temp))
	{
		Util.message( "Transmission echo disabled\n");
	}
	else
	{
		if(temp[0] == '1')
		{
			Util.message("Transmission echo enabled\n");
			txEchoflg = true;

		}
		else
		{
			Util.message("Transmission echo disabled\n");
			Util.message(3,"-E parameter not '1'. Hence Transmission echo disabled\n");
			txEchoflg = false;
		}
	}
}



void Terminal::setFilename()
{
	CliParser *pcli = CliParser::getInstance();

	filename = pcli->getParamValue(PARAM_F);
	if(!pcli->checkParamVal(filename))
	{
		Util.message("No input parameter value specified for Filename. Logging data to file disabled\n");
		Util.message(3, "Specify Filename with -f parameter. Eg: -F../<relative path>/<filename>.<extension> \n");
		Util.log("No input parameter value specified for Filename. Logging data to file disabled\n");
		Util.log(3, "Specify Filename with -f parameter. Eg: -F../<relative path>/<filename>.<extension> \n");
		logtofileflg=false;
	}
	else
		logtofileflg = true;

}


void Terminal::setVerbosityLevel()
{
	CliParser *pcli = CliParser::getInstance();

	char temp[5];

	pcli->getParamValue(PARAM_V, temp, sizeof(temp)-1);
	if(!pcli->checkParamVal(temp))
	{
		Util.log(DEVELOPER, "Verbosity level not specified. Setting Verbosity level to default \n");
	}
	else
	{
		if(isdigit(temp[0]))
		{
			Util.setVerbosity(atoi(temp));
		}
		else
			Util.log(DEVELOPER,"Verbosity level value not numeric. Setting Verbosity level to minimum \n");
	}
}

void Terminal::checkValidParameter()
{
	char printbuf[50]={};
	char xtens[10]={};

	setVerbosityLevel();
	setChannelName();

	portnum=parsePortNumber();
	setFilename();


	if(logtofileflg)
	{
		int filenamelen = strlen(filename);
		std::string pstr(filename);
		int dotpos = pstr.find_last_of( "." ); // Search for last og / or \.

		if( (dotpos == string::npos)) // Not found?
		{
			//pstr.assign( "." ); // The current directory is the AVROSP EXE path also.
			Util.message( "File Extension not specified \n");
			Util.log( "File Extension not specified \n");
			throw ErrorManager::getRecord(InvalidInput);
		}

		++dotpos;
		if( strlen(&filename[dotpos]) > 3)											// File extension string length limited to 3
		{
			sprintf(printbuf, "Extension '%s' exceeded expected length\n", &filename[dotpos]);
			Util.message(printbuf);
			Util.log(printbuf);
			throw ErrorManager::getRecord(UnexpectedLength);
		}
		else
			strncpy(xtens, &filename[dotpos], sizeof(xtens)-1);

		if(!(strlen(xtens)))
		{
			Util.message( "File Extension not specified \n");
			Util.log( "File Extension not specified \n");
			throw ErrorManager::getRecord(InvalidInput);
		}

		memset(filename, 0, filenamelen);
		strncpy(filename, pstr.c_str(), dotpos-1);

		if(strcasecmp(xtens, _TXT_))
		{
			Util.message("File with Extension '.");
			Util.message(xtens);
			Util.message("' not supported\n");
			Util.log("File with Extension '.");
			Util.log(xtens);
			Util.log("' not supported\n");
			throw ErrorManager::getRecord(InvalidInput);
		}
	}

	baudrate=parseBaudrate();
	printAsciiflg=parseAsciiPrint();
	parseEchoFlag();

	/*
	 *  This is currently hard-coded. The flags can be updated by passing some parameter from command line, whenever features would be expanded.
	 */
	rxflg = true;
	txflg = false;
}


int Terminal::getPressedKey()
{
	int key = 0;
#ifdef COMPILE_FOR_WINDOWS
	if(kbhit())
	{
		key = getch();
		if(key==0)
		{
			key += 256;
			key += getch();
		}
	}
#endif

#ifdef COMPILE_FOR_LINUX
	fd_set set;
	struct timeval t;

	t.tv_sec = 0;
	t.tv_usec = 0;

	FD_ZERO( &set );
	FD_SET( fileno( stdin ), &set );

	int res = select( fileno( stdin )+1, &set, NULL, NULL, &t);

	if( res > 0 )
	{
		read( fileno( stdin ), &key, 1 );

		if(key == 27)						// Esc or any special function key
		{
			int i;
			int sum=0;

			sum+=key;
			for(i=0; i<4; i++)
			{
				// If select function is not called here then, it requires more than one press to detect 'Esc' key
				int ret = select( fileno( stdin )+1, &set, NULL, NULL, &t);
				if(ret > 0)
				{
					read( fileno( stdin ), &key, 1 );
					sum += key;
				}
			}

			switch(sum)
			{
				case 27:
					key = 27;
					break;

				case 346:			// Function Key 'F5'. key value sequence - 27 91 49 53 126
					key = 319;
					break;

				case 349:			// Function Key 'F7'. key value sequence - 27 91 49 56 126
					key = 321;
					break;

				case 350:			// Function Key 'F8'. key value sequence - 27 91 49 57 126
					key = 322;
					break;

				case 342:			// Function Key 'F9'. key value sequence - 27 91 50 48 126
					key = 323;
					break;
			}
		}
	}
	else if( res < 0 )
	{
		perror( "select error" );
		key =0;
	}
#endif

	return key;
}




void Terminal::simpleTerminal()
{
	char printbuf[256]={};
	int key=0;
	int bytesreceived=0;
	unsigned char printcharcnt=0;
	FileIO file;
	FileIO hexoutfile;

	port = CommManageFactory::createNew(chnname);
	sprintf(printbuf, "%s port", chnname);
	if(port)
	{
		strcat(printbuf, " created successfully \n");
		Util.message(5, printbuf);
		Util.log(5, printbuf);
	}
	else if (port == NULL)
	{
		strcat(printbuf, " Factory creation failed \n");
		throw ErrorManager::getRecord(CommChFactoryCreatnFailed);
	}

	port->setPortNum(portnum);
	port->setDataRate(baudrate);
	port->openChannel();
	port->flushTX();
	port->flushRX();

	// Receive Retries set to 2 only for serial port.
	SerialPort *pChn = dynamic_cast<SerialPort*>(port);
	pChn->setRxRetries(1);

	// Because in linux, on opening a port, DTR & RTS are high
	controlModemLines(1, 1);
	Util.message("Toggle RTS & DTR using 'F8' & 'F7' key respectively\n");
	Util.log("Toggle RTS & DTR using 'F8' & 'F7' key respectively\n");

	if(logtofileflg)
	{
		char *hexoutfilename;
		hexoutfilename = new char[strlen(filename)+4+4];			// 4 characters for "_hex" and next 4 characters for ".txt"
		strcpy(hexoutfilename, filename);
		strcat(hexoutfilename, "_hex");
		strcat(filename, ".");
		strcat(hexoutfilename, ".");
		strcat(filename, _TXT_);
		strcat(hexoutfilename, _TXT_);

		file.open(filename, enFileOutput);
		hexoutfile.open(hexoutfilename, enFileOutput);

		Util.message("Logging to file enabled\n");
		Util.log("Logging to file enabled\n");
	}

	Util.message("Press "
					"\t'Esc' key \tTo Quit terminal\n"
					"\t'F7'  key \tTo Toggle DTR modem signal\n"
					"\t'F8'  key \tTo Toggle RTS modem signal\n"
					"\t'F9'  key \tTo print current status of RTS & DTR signals\n"
					"\n");

	#ifdef COMPILE_FOR_LINUX

	struct termios newSettings;
	newSettings = oldSettings;
	newSettings.c_iflag |= IGNBRK;
	newSettings.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
	newSettings.c_lflag &= ~(ICANON | ECHO );
	newSettings.c_lflag |= (ECHONL );
	tcsetattr( fileno( stdin ), TCSANOW, &newSettings );

	#endif

	memset(printbuf, 0, sizeof(printbuf));
	while(1)
	{
		bytesreceived = port->receive((unsigned char *)printbuf, 1);

		#if 0
		if(printbuf[0] >= 0x20 && printbuf[0] <= 0x7e) /*isprint?*/
		{
			sprintf(printbuf, "%c", printbuf[0]);
		}
		else
		{
			char temp = printbuf[0];
			sprintf(printbuf, "(%02X)", printbuf[0]);
			expects
			if(temp == '\r' || temp=='\n')
				strcat(printbuf, "\n");
		}
		#endif

//		Util.message("Total bytes read ");
//		Util.message(bytesreceived);
//		Util.message("\n");

		int cntr=0;
		for(cntr=0; cntr<bytesreceived; cntr++)
		{
			unsigned char chr = printbuf[cntr];
			char hexchr[5]={};

			sprintf(hexchr, "%02X ", (unsigned char)chr);

			if(chr == '\r')
				chr = '.';

			if(chr == '\n')
				strcat(hexchr, "\n");

			if(chr == '\t')
				strcat(hexchr, "\t");

			if(chr == 0)
				chr = ' ';

			if(logtofileflg)
			{
				file.writeFile(chr);
				hexoutfile.writeFile(hexchr);
			}

			if(printAsciiflg)
			{
				Util.message(chr);
			}
			else
			{
				Util.message(hexchr);
			}

			printcharcnt++;

			if((printcharcnt>=MAX_PRINT_CHARACTERS) || chr=='\n')
			{
				Util.message("\n");

				if(logtofileflg)
				{
					file.writeFile("\n");
					hexoutfile.writeFile("\n");
				}
				printcharcnt = 0;
			}
		}

		bytesreceived=0;

		fflush(stdout);
		memset(printbuf, 0, 10);

		key = getPressedKey();
		switch(key)
		{
			case 27:						// Escape Key Pressed
				if(logtofileflg)
				{
					file.close();
					hexoutfile.close();
				}
				Util.message("\n");
				Util.log("\n");
				return;

			case 319:								// Function Key 'F5'. This is not mentioned in user manual purposely
				if(printAsciiflg == true)
				{
					printAsciiflg = false;
					Util.message("\nAscii print disabled \n");
				}
				else
				{
					printAsciiflg = true;
					Util.message("\nAscii print enabled \n");
				}
				break;

			case 321:								// Function Key 'F7'
				if(dtrstatus)
				{
					controlModemLines(0, rtsstatus);
				}
				else
				{
					controlModemLines(1, rtsstatus);
				}
				break;

			case 322:								// Function Key 'F8'
				if(rtsstatus)
				{
					controlModemLines(dtrstatus, 0);
				}
				else
				{
					controlModemLines(dtrstatus, 1);
				}
				break;

			case 323:								// Function Key 'F9'
				controlModemLines(dtrstatus, rtsstatus);
				break;

			default:
				if((key<128) && key)
				{
					port->send((unsigned char*) &key, 1);

					if(txEchoflg)
					{
						char hexkey[5]={};
						char charkey[5]={};			// Only used for display purpose

						sprintf(hexkey, "%02X ", (unsigned char)key);
						sprintf(charkey, "%c", (unsigned char)key);

						if(key == '\r')
						{
							strcat(charkey, "\n");
						}

						if(key == '\n')
						{
							strcat(hexkey, "\n");
							strcat(charkey, "\n");
							printcharcnt = 0;
						}

						if(key == '\t')
						{
							strcat(hexkey, "\t");
						}

						if(key == 0)
							key = ' ';

						if(printAsciiflg)
						{
							Util.message(charkey);
						}
						else
						{
							Util.message(hexkey);
						}
					}

					if(key == '\t')
						printcharcnt += 8;
					else
						printcharcnt++;

					if(printcharcnt>=MAX_PRINT_CHARACTERS)
					{
						Util.message("\n");
						printcharcnt = 0;
					}
				}
		}
		key = 0;
	}
}


void Terminal::Run()
{
    #ifdef COMPILE_FOR_LINUX
	tcgetattr( fileno( stdin ), &oldSettings );
    #endif
    
	checkValidParameter();

	if(!strcasecmp(chnname, _SERIAL_))
	{
		if(rxflg)
		{
			simpleTerminal();
		}
	}
	else //if(!strcasecmp(chnname, _JTAG_))
	{
		Util.message(chnname);
		Util.message(" channel is not supported in current software version \n");
		Util.log(chnname);
		Util.log(" channel is not supported in current software version \n");
		throw ErrorManager::getRecord(InvalidInput);
	}
}


void Terminal::controlModemLines(unsigned char DTR, unsigned char RTS)
{

	bool olddtr = dtrstatus;
	bool oldrts = rtsstatus;
	SerialPort *pChannel = dynamic_cast<SerialPort*>(port);

	if(DTR == 1)
		dtrstatus = true;
	else
		dtrstatus = false;

	if(RTS == 1)
		rtsstatus = true;
	else
		rtsstatus = false;

	if(pChannel->controlModemLines(dtrstatus, rtsstatus) == false)
	{
		dtrstatus = olddtr;
		rtsstatus = oldrts;
	}

	if(dtrstatus == 1)
		Util.message("\n DTR=1 ");
	else
		Util.message("\n DTR=0 ");

	if(rtsstatus == 1)
		Util.message("RTS=1 \n");
	else
		Util.message("RTS=0 \n");
}


