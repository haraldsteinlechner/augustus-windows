///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   © Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////

bool useAIEnemyTo = true;

void idleEnvironmentAndAI();
void kiGegnerNachdenkProcedure();

int kiIdle(void *unused)
{
	int denkrate = kiGegnerDenkclockrate/kiClockRate;
	int circle = 0;

	SDL_Delay(2000);
	
	for(;!killThreadsInNextRate;)
	{
		idleEnvironmentAndAI();

		if(useAIEnemyTo)
		if(circle%denkrate==0) kiGegnerNachdenkProcedure();
	

		circle++;
		SDL_Delay(kiClockRate);
	}
	
	return 0;
}

int particleSystemIdle(void *unused)
{
	for(;!killThreadsInNextRate;)
	{ 
		particleSystemHandler->update();

		SDL_Delay(particleSystemRate);
	}
	
	return 0;
}

int timeIdle(void *unused)
{
	for(;!killThreadsInNextRate;)
	{
		mainTime.update();
		
		#ifdef GCCCompile
		#warning Zeitupdate
		#endif
		float vergangeneSekunden = (mainTime.stunde*3600 + mainTime.minute*60 + mainTime.sekunde - (startTime.stunde*3600 + startTime.minute*60 + startTime.sekunde))*zeitfaktor;
		playedTime = awx_time(vergangeneSekunden);
		
		SDL_Delay(clockTimerRate);
	}
	
	return 0;
}

void startParticleSystemThread()
{
	particleSystemTimer = SDL_CreateThread(particleSystemIdle,NULL);
}

void killParticleSystemThread()
{
	cout << "kill particleSystemThread..." << endl;
	SDL_KillThread(particleSystemTimer);
}


void startAIAndIdleThread()
{
	kiTimer = SDL_CreateThread(kiIdle,NULL);
}

void killAIAndIdleThread()
{
	cout << "kill kiTimerThread.." << endl;
	SDL_KillThread(kiTimer);
}

void startTimerIdleThread()
{
	clockTimer = SDL_CreateThread(timeIdle,NULL);
}

void killTimerIdleThread()
{
	cout << "kill killClockTimerThread..." << endl;
	SDL_KillThread(clockTimer);
}

void waitAllUserThreads()
{
	killThreadsInNextRate=true;
	int status;
	
	SDL_WaitThread(kiTimer,&status);
	cout << "kiTimer: " << status << endl;
	SDL_WaitThread(clockTimer,&status);		
	cout << "clockTimer: " << status << endl;
	SDL_WaitThread(particleSystemTimer,&status);	
	cout << "particleSystemTimer: " << status << endl;
	
}