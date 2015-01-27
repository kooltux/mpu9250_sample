#include "IMU.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


int main(int argc, char *argv[])
{
	// intialize default values
	int opt = 0;
	int delay = 10;
	int low_warning = 20;
	int strong_warning = 45;
	char *cmd = NULL;
	
	// parsing command options
	while ((opt = getopt(argc, argv, "d:l:h:")) != -1) {
		switch(opt) {
			
			case 'd':
			cmd = optarg;
			printf("Delay set to %s usec\n", cmd);
			delay = atoi(cmd);
			break;
			
			case 'l':
			cmd = optarg;
			printf("Low Warning set to +/-%s deg\n", cmd);
			low_warning = atoi(cmd);
			break;
			
			case 'h':
			cmd = optarg;
			printf("Strong Warning set to +/-%s deg\n", cmd);
			strong_warning = atoi(cmd);
			break;
			
			case '?':
			
			break;
		}
	}
	
	// Setting IMU Parameters
	IMU *imu = new IMU();
	imu->setReadTempo(delay);
	imu->setLowWarningLvl(low_warning);
	imu->setStrongWarningLvl(strong_warning);
	
	while(1)
	{
		usleep(imu->getTempo());
		imu->computePose();
	}
	
	return 0;
}
