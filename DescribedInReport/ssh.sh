
#!/bin/bash
echo "Launching SSH..."
#sudo ssh turtlebot@192.168.0.100



sshpass -p 'napelturbot'  ssh -T turtlebot@192.168.0.100 
roslaunch turtlebot_bringup minimal.launch

