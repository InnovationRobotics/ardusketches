1. Use arduino ide after you:
	sudo usermod -a -G dialout sload
	reboot
2. In order to download, stop the node
3. The node to run is:
	rosrun rosserial_python serial_node.py /dev/ttyACM1

4. Install: follow http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup
