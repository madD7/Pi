#include "appInclude.h"
#include "serial.h"

static PFsword Fd;					// File Descriptor
static PFdword Txcnt=0, Rxcnt=0;

static PFchar Rxbuf[256]={0};
static PFchar Txbuf[256]={0};


struct termios TtyN; 			// For TTY New settings





PFEnStatus pfSerialOpen(PFpCfgUart pser)
{	
	struct termios ttyo;				// For Old TTY settings
	char portname[50];
	
	memset(portname, 0, sizeof(portname));
	strcpy(portname, "/dev/tty");
	#if		USB_COMPORT_USED
	strcat(portname, "USB");
	#endif
	sprintf(&portname[strlen(portname)], "%d", (int)pser->portnum);
	
	printf("Portname = %s\n", portname);
	memset (&TtyN, 0, sizeof(TtyN));

	
	/* Setting other Port Stuff */
	/*
	CS8		: 8n1 (8bit,no parity,1 stopbit)
	CLOCAL  : local connection, no modem contol
        CREAD   : enable receiving characters
	*/

	TtyN.c_cflag    &= ~CRTSCTS;								// Hardware flow control Disabled
	TtyN.c_cflag    &= ~CSIZE;								// Character Size mask

	switch(pser->parity)
	{
		default:	
		case enUartParityNone:
			TtyN.c_cflag    &= ~PARENB;						// Disable Parity
		break;

		case enUartParityOdd:
			TtyN.c_cflag	|= PARENB | PARODD;
		break;
		
		case enUartParityEven:
			TtyN.c_cflag	|= PARENB;
		break;
	}

	switch(pser->stpbits)
	{
		default:
		case enUartStopBits_1:
			TtyN.c_cflag	&= ~CSTOPB;
		break;
		
		case enUartStopBits_2:
			TtyN.c_cflag	|= CSTOPB;
		break;
	}

	TtyN.c_cflag    |= CREAD | CLOCAL;
	TtyN.c_cflag	|= pser->csize;

	TtyN.c_cc[VMIN]   =  0;									// read doesn't block
	TtyN.c_cc[VTIME]  =  1;									// 0.5 seconds read timeout

	cfsetospeed (&TtyN, (speed_t)pser->baudrate);						// Set Output Speed (Baudrate)
	cfsetispeed (&TtyN, (speed_t)pser->baudrate);						// Set Input Speed (Baudrate)

	TtyN.c_iflag |= IGNPAR;									// Ignore Parity Errors
	
	TtyN.c_oflag = 0;									// 
	TtyN.c_lflag = 0;									// Non-canonical mode No Echo. Most Important. 

	//Fd = open( comPort, O_RDWR| O_NONBLOCK | O_NDELAY );
	Fd = open(portname, O_RDWR);
		
	printf("Initialized with File Descriptor %d",(int)Fd);
	if ((int) Fd < 0 )
	{
		printf("\n !!!!! ERROR: Unable to Initialize the Port %d\n",(int)pser->portnum);
		return enStatusError;
	}
	
	tcgetattr(Fd, &ttyo);										// Copy Old Settings.
	
	tcflush(Fd, TCIFLUSH);										// Flush Input before setting the new attribute values
	if (tcsetattr ( Fd, TCSANOW, &TtyN) != 0)
	{
		printf("!!!!! Warning: Could not Set tc Attributes \n");
	}
	return enStatusSuccess;
}



PFEnStatus pfSerialClose()
{
	Txcnt = -1;
	Rxcnt = -1;
	
	if(pfSerialTxBufferFlush() == enStatusError)
	{
		return enStatusError;
	}
	
	if(pfSerialRxBufferFlush() == enStatusError)
	{
		return enStatusError;
	}

	if(!close(Fd))
	{
		return enStatusSuccess;
	}
	else 
	{
		printf("!!!!! Error: Could not Purge Comm Port \n");
		return enStatusError;
	}
}


PFEnStatus pfSerialWrite(PFbyte* pdata, PFdword size)
{	
	PFdword wrbytes=0;
	
	tcflush(Fd, TCOFLUSH);
	if ( tcsetattr (Fd, TCSANOW, &TtyN) != 0)
	{
		printf("!!!!! Warning: Could not Set tc Attributes \n");
	}
	
	wrbytes  = write(Fd, pdata, size);
	if(wrbytes <0)
    {
        printf("!!!!! Error: Could not Write to Output Buffer\n");
		return enStatusError;
    }
	
	Txcnt = wrbytes;
	if(wrbytes == size)
	{
		return enStatusSuccess;
	} else 
	{
		return enStatusError;
	}
}


PFEnStatus pfSerialWriteByte(PFbyte data)
{
	return pfSerialWrite(&data, 1);
}


PFEnStatus pfSerialWriteString(PFbyte* pdata)
{
	Txcnt=0;
	while(*pdata != '\0')
	{
		pfSerialWriteByte(*pdata++);
		Txcnt++;
	}

	return enStatusSuccess;
}



PFEnStatus pfSerialRead(PFbyte* pdata, PFdword maxsize, PFdword* prdbytes)
{	
	PFdword bytesread=0;
	
	while(bytesread != maxsize)
	{
		pfSerialReadByte(pdata);
		pdata++;
		bytesread++;
	}
	
	Rxcnt = bytesread;
	*prdbytes = bytesread;
	return enStatusSuccess;
}


PFEnStatus pfSerialReadByte(PFbyte* pdata)
{
	PFdword bytesread=0, attempt;
	
	while(bytesread == 0)
	{
		attempt=0;

		bytesread = read(Fd, pdata, 1);
		if(bytesread < 0)
		{
			attempt++;
			printf("!!!!! Warning: Could not Read from Input Buffer\n");
			
			if(attempt>10)
				return enStatusError;
		}
		
		tcflush(Fd, TCIFLUSH );
		if (tcsetattr (Fd, TCSANOW, &TtyN) != 0)
		{
			printf("!!!!! Warning: Could not Set tc Attributes \n");
		}
	}
	Rxcnt = 0;
	return enStatusSuccess;
}



PFEnStatus pfSerialGetTxBufferSize(PFdword* value)
{
	*value  = sizeof(Txbuf);
	return enStatusSuccess;
}


PFEnStatus pfSerialGetTxBufferCount(PFdword* value)
{
	*value  = Txcnt;
	return enStatusSuccess;
}


PFEnStatus pfSerialGetRxBufferSize(PFdword* value)
{
	*value  = sizeof(Rxbuf);
	return enStatusSuccess;
}


PFEnStatus pfSerialGetRxBufferCount(PFdword* value)
{
	*value  = Rxcnt;
	return enStatusSuccess;
}


PFEnStatus pfSerialTxBufferFlush()
{
	tcflush(Fd, TCOFLUSH);
	if ( tcsetattr ( Fd, TCSANOW, &TtyN) != 0)
	{
		printf("!!!!! Warning: Could not Set tc Attributes \n");
		return enStatusError;
	}
	
	memset(Txbuf, 0, sizeof(Txbuf));
	Txcnt = 0;
	
	return enStatusSuccess;
}


PFEnStatus pfSerialRxBufferFlush()
{
	tcflush( Fd, TCIFLUSH );
	if ( tcsetattr ( Fd, TCSANOW, &TtyN) != 0)
	{
		printf("!!!!! Warning: Could not Set tc Attributes \n");
		return enStatusError;
	}
	
	memset(Rxbuf, 0, sizeof(Rxbuf));
	Rxcnt = 0;
	
	return enStatusSuccess;
}


