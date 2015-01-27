#ifndef IMU_H
#define IMU_H

#include <RTIMUMPU9250.h>
#include <pthread.h>

#define _PI_	3.14159265

class IMU
{
	public:
	IMU();
	virtual ~IMU();
	
	void setReadTempo(int us);
	void setLowWarningLvl(int llvl);
	void setStrongWarningLvl(int slvl);
	
	int getLowWarningLvl();
	int getStrongWarningLvl();
	
	int getTempo();
	void readData();
	void computePose();
	pthread_mutex_t *getMutex(){return &m_mutexIMU;};
	
	private:
	
	bool imu_init();
	
	int				 imu_us_delay;
	int				 imu_low_warn;
	int				 imu_strong_warn;
	RTIMUSettings 	*imu_settings;
	RTIMU		 	*imu_imu;
	RTIMU_DATA 		 imu_data;
	pthread_mutex_t  m_mutexIMU;
};

#endif // IMU_H
