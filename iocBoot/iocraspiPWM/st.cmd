#!../../bin/linux-aarch64/raspiPWM

epicsEnvSet("ENGINEER","HZB Student")

cd "${TOP}"

## TODO
## Define the instructions to set the correct env variables, 
## configure the device and load the record database

cd "${TOP}/iocBoot/${IOC}"
iocInit

