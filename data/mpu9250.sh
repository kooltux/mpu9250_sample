#!/bin/sh
echo "**************** MPU9250 LAUNCHER ****************"
echo ""

MOD=`lsmod |grep low_speed_spidev`
DELAY=10
LOW_WARN_LVL=20
STRONG_WARN_LVL=45

case "$1" in
  start)
	if [[ -z "$MOD" ]]; then
            insmod /usr/bin/low_speed_spidev.ko
	fi
	/usr/bin/mpu9250_sample -d $DELAY -l $LOW_WARN_LVL -h $STRONG_WARN_LVL
	;;
  stop)
        log_daemon_msg "Stopping mpu9250_sample..." "mpu9250_sample"
        if start-stop-daemon --stop --quiet --oknodo --pidfile /tmp/.mpu9250.pid; then
            log_end_msg 0
        else
            log_end_msg 1
        fi
        ;;
  restart)
        log_daemon_msg "Restarting mpu9250_sample..." "mpu9250_sample"
        start-stop-daemon --stop --quiet --oknodo --retry 30 --pidfile /tmp/.mpu9250.pid
	sleep 1        
        if start-stop-daemon --start --quiet --pidfile /tmp/.mpu9250.pid --exec /usr/bin/mpu9250_sample; then
            log_end_msg 0
        else
            log_end_msg 1
        fi
        ;;
  *)
	log_action_msg "Usage: /etc/init.d/mpu9250.sh {start|stop|restart}"
	exit 1
esac

exit 0
