#!/bin/bash
echo 61 > /sys/class/gpio/export
echo 66 > /sys/class/gpio/export
echo 67 > /sys/class/gpio/export
echo 68 > /sys/class/gpio/export
echo 69 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio61/direction
echo out > /sys/class/gpio/gpio66/direction
echo out > /sys/class/gpio/gpio67/direction
echo out > /sys/class/gpio/gpio68/direction
echo out > /sys/class/gpio/gpio69/direction
echo cape-bone-iio > /sys/devices/bone_capemgr.*/slots
find /sys/ -name '*AIN*'
