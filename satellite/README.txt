To compile, run 

make build

Please, make sure ncurses is installed by doing apt-get install libncurses5 libncurses5-dev

The code will be build in build/satellite and needs to be run as root. Before running the code for the first time, and after each restart of the BeagleBone, please run setupGPIO.sh as root

When the main process is running, a user may send a thrust command by creating a file with a single line representing the bits for a thrust command as specified in the software specifications.  Eg to command a 2 second thrust at 0.5 magnitude to a single thruster try:

echo 0x181 > ~/earth2satellie

The user should notice that a single gpio bit gets toggled at a 0.5 second rate for a duration of 2 seconds. 
