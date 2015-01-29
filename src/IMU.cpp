#include "IMU.h"

static pthread_attr_t attr;
sched_param paramThreadProcs;
static pthread_t threadIMU;
void* thread_IMU(void* data);


IMU::IMU()
{
	imu_init();
}

IMU::~IMU()
{

}

bool IMU::imu_init()
{
	m_mutexIMU = PTHREAD_MUTEX_INITIALIZER;

	// default warning lvl values
	imu_low_warn = 20;
	imu_strong_warn = 45;

	// Initialize IMU
	imu_settings = new RTIMUSettings((char *)"/usr/share/mpu9250/RTIMULib.ini");
	imu_imu = RTIMU::createIMU(imu_settings);
	if(!imu_imu)
	{
		fprintf(stderr,"Cannot create IMU\n");
		exit(1);
	}
	imu_imu->setAccelEnable(true);
	imu_imu->setGyroEnable(true);
	imu_imu->setCompassEnable(false);

	if(!imu_imu->IMUInit())
	{
		fprintf(stderr,"Cannot initialize IMU\n");
		exit(1);
	}

	// Config and launch grab thread
	pthread_attr_init(&attr);
	int newprio = 20;
	paramThreadProcs.sched_priority = newprio;
	pthread_attr_setschedparam (&attr, &paramThreadProcs);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	pthread_create(&threadIMU, &attr, thread_IMU, this);

	return true;
}

void* thread_IMU(void* data)
{
	IMU *imu = (IMU *)data;
	while(1)
	{
		pthread_mutex_lock(imu->getMutex());

		imu->readData();

		pthread_mutex_unlock(imu->getMutex());
		usleep(10*1000);
	}
}


void IMU::setReadTempo(int us)
{
	if(us < 10)
		imu_us_delay = 10;
	else
		imu_us_delay = us;
}

void IMU::setLowWarningLvl(int llvl)
{
	imu_low_warn = llvl;
}

void IMU::setStrongWarningLvl(int slvl)
{
	imu_strong_warn = slvl;
}

int IMU::getLowWarningLvl()
{
	return imu_low_warn;
}

int IMU::getStrongWarningLvl()
{
	return imu_strong_warn;
}

int IMU::getTempo()
{
	return imu_us_delay * 1000;
}

void IMU::readData()
{
	imu_imu->IMURead();
}

void IMU::computePose()
{
	pthread_mutex_trylock(&m_mutexIMU);

	double pose[3] 	= {0.0, 0.0, 0.0};
	double acc[3] 	= {0.0, 0.0, 0.0};
	double gyr[3] 	= {0.0, 0.0, 0.0};
	double mag[3] 	= {0.0, 0.0, 0.0};
	int warnings[2] = {0, 0};

	imu_data	= imu_imu->getIMUData();
	RTVector3 v = imu_imu->getMeasuredPose();
	RTVector3 a = imu_imu->getAccel();
	RTVector3 g = imu_imu->getGyro();
	RTVector3 m = imu_imu->getCompass();

	acc[0] = a.x(); acc[1] = a.y(); acc[2] = a.z();
	gyr[0] = g.x(); gyr[1] = g.y(); gyr[2] = g.z();
	mag[0] = m.x(); mag[1] = m.y(); mag[2] = m.z();

	// Converting rad to degree
	pose[0] = 180.0 * v.x() / _PI_;
	pose[1] = 180.0 * v.y() / _PI_;
	pose[2] = 180.0 * v.z() / _PI_;

	if(pose[0] > 0)
		pose[0] = -(pose[0] -180.0);
	else
		pose[0] = -(pose[0] + 180.0);

	// Warning Analisis
	if( abs(pose[0]) > getStrongWarningLvl() )
		warnings[0] = 2;
	else if (abs(pose[0]) > getLowWarningLvl())
		warnings[0] = 1;

	if( abs(pose[1]) > getStrongWarningLvl() )
		warnings[1] = 2;
	else if (abs(pose[1]) > getLowWarningLvl())
		warnings[1] = 1;

	printf("{\"p\":{\"x\":%f,\"y\":%f,\"z\":%f},\"a\":{\"x\":%f,\"y\":%f,\"z\":%f},\"g\":{\"x\":%f,\"y\":%f,\"z\":%f},\"m\":{\"x\":%f, \"y\":%f,\"z\":%f},\"w\":{\"x\":%d,\"y\":%d}}\n",
		pose[0], pose[1], pose[2],
		acc[0], acc[1], acc[2],
		gyr[0], gyr[1], gyr[2],
		mag[0], mag[1], mag[2],
		warnings[0], warnings[1]);
	fflush(stdout);

	pthread_mutex_unlock(&m_mutexIMU);
}
