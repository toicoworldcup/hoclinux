1.make
2.dtc -@ -I dts -O dtb -o led_overlay.dtbo led_overlay.dts
3.sudo dtoverlay led_overlay.dtbo
4.sudo insmod hieu_driver.ko
5.chmod 666 /dev/hieu_driver

//sudo iwconfig wlan0 power off
