#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"

#define RAD2DEG(x) ((x)*180./M_PI)	//Global defined Function to convert radian to degree														

ros::Publisher cmd_vel_pub_;	//Setting Published															

bool obstacle(const sensor_msgs::LaserScan::ConstPtr& scan){
	float count=0;
	count = scan->scan_time / scan->time_increment; //Setting the number of values to be checked in for loop         

		for(int i = 0; i < count; i++){	//Loop for each points of range //FOR1

            float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i); //Convertion to have the angle for each point in degree 

            if ((degree > 150) and (degree < 210)){ //Setting the field of View of the robot //IF1

				if(scan->ranges[i] < 0.30000){
					return true;
				}
			}
			else if ((degree > 160) and (degree < 200) and scan->ranges[i] > 0.30000) {return false;}
		}
}

void ScanMove(const sensor_msgs::LaserScan::ConstPtr& scan){
	
	geometry_msgs::Twist base_cmd;
	bool bolle=true;	
	
	while(bolle){
		
		if(obstacle(scan) == false){
			base_cmd.linear.x = 0.25;												
			base_cmd.angular.z = 0;							
			cmd_vel_pub_.publish(base_cmd);
			ros::Duration(0.1).sleep();		
		}
		else{
			while(obstacle(scan) == true){
				base_cmd.linear.x = 0;												
				base_cmd.angular.z = 0.4;							
				cmd_vel_pub_.publish(base_cmd);
				ros::Duration(0.2).sleep();	
				break;
			}					
		}

		
		bolle=false;			
	}
	
}




void ScanAndMove(const sensor_msgs::LaserScan::ConstPtr& scan)                                                                                         
{
	geometry_msgs::Twist base_cmd;															
    base_cmd.linear.x=base_cmd.linear.y=base_cmd.angular.z=0;  //Setting Initial values                                                                                    
    bool status = true; //Boolean to check if the robot have to move                                        
    int available = 1; //Variable to loop	
    int count; //Counter
    int savedi=0;
    double saveang;	
    


	while (available < 2) //while loop used to run program without interrupts	//WHILE1	
	{

		status = 1;

        count = scan->scan_time / scan->time_increment; //Setting the number of values to be checked in for loop         
		//float maxtemp=scan->ranges[0]; //Setting temporary value of the distance between robot and walls

		for(int i = 0; i < count; i++){	//Loop for each points of range //FOR1

            float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i); //Convertion to have the angle for each point in degree 

            if ((degree > 150) and (degree < 210)){ //Setting the field of View of the robot //IF1

				if(scan->ranges[i] < 0.30000){ //Looking if obstacle if nearest than 40cm //IF2

                status = 0;	//Setting boolean to say that there is and obstacle	

				}//ENDIF2

			}//ENDIF1

			//if(scan->ranges[i]>maxtemp){ //Loop Used to set the farest point   //IF3

				//float saveang=degree;//Save the angle corresponding to the max distance
			//	int savedi=i;//Save index of the max value
			//	maxtemp = scan->ranges[i];
				
				

			//}//ENDIF3

			

		}//ENDFOR1

        if (status == 1){//IF4

	            base_cmd.linear.x = 0.25;//Setting Speed													
				base_cmd.angular.z = 0;//Rotation so 0 to move in line								
				cmd_vel_pub_.publish(base_cmd);//Publishing

		}//ENDIF4 THEN ELSE1
		else{//ELSE1

	        //base_cmd.linear.x = 0;	//Speed to 0

			//if(scan->ranges[savedi] > 0){ //Impossible case that makes move the robot to the right we only use farest point but this can be used also //IF5

                //base_cmd.angular.z = -0.4; //Turn Right							
				//ROS_INFO ("Right Rotation"); //Information displayed in cmd

			//}//ENDIF5 THEN ELSE2
			//else{//ELSE2
			//while(){
            //    base_cmd.angular.z = -0.4; //Set angle to point to the farest way												
             //   ROS_INFO ("Going to the Farest Point");	//Information displayed in cmd
            //}    

			//}//ENDELSE2
			
			
			//REPRISE
			
			
			int randNumber = rand() % 1000 + 0;
			float rotSpeed=0;
			
			if(randNumber % 2 == 0){
			
				rotSpeed=-0.2; 
			
			}else{
				
				rotSpeed=0.2;
				
			}
	int obs=3;
			while( !status ){
				
				ros::Duration(0.4).sleep();
				ROS_INFO ("LOOPING");	
				
					for(int i = 0; i < count; i++){	//Loop for each points of range //FOR1
						float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i); //Convertion to have the angle for each point in degree 
						if (((degree > 150) and (degree < 210)) and (scan->ranges[i] < 0.30000)){ //Setting the field of View of the robot //IF1
						

							
						 //Looking if obstacle if nearest than 40cm 

							status = 0;	//Setting boolean to say that there is and obstacle	
							obs=1;
							
							ROS_INFO ("OBSTACLE");
							break;

						
						}else if (((degree > 150) and (degree < 210)) and (scan->ranges[i] > 0.30000)){obs=0;ROS_INFO ("NOOOOOBSTACLE");break;}

					}
				
				if(obs==1){
							base_cmd.angular.z = rotSpeed;
							cmd_vel_pub_.publish(base_cmd);													
							ros::Duration(0.4).sleep();	
							ROS_INFO ("Rotation");
							obs=3;
							break;
				}
				else{
							status=1;
							rotSpeed=0;
							base_cmd.angular.z = rotSpeed;
							cmd_vel_pub_.publish(base_cmd);													
							ros::Duration(0.8).sleep();	
							ROS_INFO ("STOP Rotation");
							obs=3;
							available++;
							ros::Duration(0.1).sleep();	
							status=1;
							base_cmd.linear.x = 0.25;//Setting Speed													
							base_cmd.angular.z = 0;//Rotation so 0 to move in line								
							cmd_vel_pub_.publish(base_cmd);//Publishing
							ros::Duration(0.2).sleep();
							ROS_INFO ("PBLISH");
							obs=3;
							available++; //Condidition to end while loop without it the loop stop before because of publishing to topics	
							break;

				}
				

				
			}
            
			
		}//ENDELSE1
		

	}
}//ENDwHILE1


int main(int argc, char **argv)																
{
	

	ROS_INFO ("Starting the Autonomous Mapping");	

    ros::init(argc, argv, "rplidar_node_client");    

    ros::NodeHandle nh_;																
    ros::Subscriber sub;																
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi", 1);                                                               
    sub = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 1000, ScanMove); 

	ros::spin();

    return 0;
}
