#!/bin/sh
# build-run.sh - check for changes, then build and run
# Cheat a little and if radetect.pro.user exists, assume
# we've had an update

LOG=/tmp/build-run.log
BUILD_LOG=/tmp/build-run.logtemp
DIR=/home/pi/radetect
BUSYSEM=/tmp/build-run.sem
PROJ=radetect

MYPID=$$
[ -r ${BUSYSEM} ] &&  { echo "Already busy (pid $(cat ${BUSYSEM}))" >> ${LOG}; exit 0; }
echo ${MYPID} > ${BUSYSEM}

[ -r ${DIR}/${PROJ}.pro.user ] || {
 [ -x ${DIR}/radetect.exe ] && {
   RPID=$(pidof radetect.exe)
   [ "${RPID}" ] || {
     echo "Apparently first time, starting radetect.exe" >> ${LOG}
     cd ${DIR}
     ./radetect.exe >> radetect.log 2>&1 &
   }
 }
 rm ${BUSYSEM}
 exit 0
}

echo "${PROJ}.pro.user detected at $(date), waiting to rebuild..." >> ${LOG}
sleep 5

cd ${DIR}
QMAKE_OK=0
echo "Rebuilding" | tee -a ${LOG} > ${BUILD_LOG}
[ -r Makefile ] && make clean >> ${BUILD_LOG} 2>&1
qmake -r ${PROJ}.pro >> ${BUILD_LOG} 2>&1 && QMAKE_OK=1
MAKE_OK=0
[ ${QMAKE_OK} = 1 ] && make >> ${BUILD_LOG} 2>&1 && MAKE_OK=1
if [ ${MAKE_OK} = 1 ]
then
	echo "Build successful, restarting..." >> ${LOG}
	killall radetect radetect.exe
        sleep 1
	cp radetect radetect.exe
	rm -f ${PROJ}.pro.user
	./radetect.exe >> radetect.log 2>&1 &
else
	echo "Build failed:" >> ${LOG}
	cat ${BUILD_LOG} >> ${LOG}
fi

rm ${BUSYSEM}
