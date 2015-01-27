add_custom_target(mpu9250bin DEPENDS RTIMULib)

SET( mpu9250_sample_SRCS 
	${mpu9250_sample_SOURCE_DIR}/src/IMU.cpp
	${mpu9250_sample_SOURCE_DIR}/src/main.cpp
)

SET( mpu9250_sample_HDRS 
	${mpu9250_sample_SOURCE_DIR}/include/IMU.h
)

INCLUDE_DIRECTORIES( ../thirdparty/RTIMU )
ADD_EXECUTABLE( mpu9250_sample ${mpu9250_sample_SRCS} )
TARGET_LINK_LIBRARIES( mpu9250_sample m RTIMU pthread )

INSTALL(TARGETS mpu9250_sample DESTINATION bin)
