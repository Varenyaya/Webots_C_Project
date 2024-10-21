#include <webots/robot.h>
#include <webots/light_sensor.h>
#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <stdio.h>
#include <stdbool.h>

#define TIME_STEP 64  // Time step for simulation in ms
#define MAX_SPEED 6.28 // Maximum velocity of motor

int main(int argc, char **argv) {
  wb_robot_init();

  // Initialize motors
  WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
  WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
   // Initialize sensors with corrected names
  WbDeviceTag ds_left = wb_robot_get_device("ps0");  // Replace with the actual name from your world file
  WbDeviceTag ds_right = wb_robot_get_device("ps1");  // Replace with the actual name from your world file
  WbDeviceTag light_sensor = wb_robot_get_device("ls0");  // Replace with the actual name from your world file  
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);


  // Initialize proximity sensors
  WbDeviceTag prox_sensors[8];
  char prox_sensor_name[50];
  for (int i = 0; i < 8; i++) {
    sprintf(prox_sensor_name, "ps%d", i);
    prox_sensors[i] = wb_robot_get_device(prox_sensor_name);
    wb_distance_sensor_enable(prox_sensors[i], TIME_STEP);
  }

  // Enable the sensors
  wb_light_sensor_enable(light_sensor, TIME_STEP);
  wb_distance_sensor_enable(ds_left, TIME_STEP);
  wb_distance_sensor_enable(ds_right, TIME_STEP);

  // Print sensor tags for debugging
  printf("Left Distance Sensor Tag: %d, Right Distance Sensor Tag: %d\n", ds_left, ds_right);
  printf("Light Sensor Tag: %d\n", light_sensor);

  double left_speed = MAX_SPEED;
  double right_speed = MAX_SPEED;

  // Main loop
  while (wb_robot_step(TIME_STEP) != -1) {
    // Read the distance sensor values
    bool left_wall = wb_distance_sensor_get_value(prox_sensors[5]) > 100;  // Adjust threshold as needed
    bool left_corner = wb_distance_sensor_get_value(prox_sensors[6]) > 100;  // Adjust threshold as needed
    bool front_wall = wb_distance_sensor_get_value(prox_sensors[7]) > 100;  // Adjust threshold as needed

    double ds_left_value = wb_distance_sensor_get_value(ds_left);
    double ds_right_value = wb_distance_sensor_get_value(ds_right);
    printf("%lf", ds_left_value);
    printf("Left Sensor: %lf, Right Sensor: %lf\n", ds_left_value, ds_right_value);

    // Read the light sensor value
    double light_value = wb_light_sensor_get_value(light_sensor);
    printf("Light Sensor Value: %f\n", light_value);

    // Determine motor speeds based on sensor readings
    if (front_wall) {
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED;
    } else {
      if (left_wall) {
        left_speed = MAX_SPEED;
        right_speed = MAX_SPEED;
      } else {
        left_speed = MAX_SPEED / 8;
        right_speed = MAX_SPEED;
      }
      if (left_corner) {
        left_speed = MAX_SPEED;
        right_speed = MAX_SPEED / 8;
      }
    }

    // Set the motor speeds
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
  }

  // Cleanup the Webots API
  wb_robot_cleanup();

  return 0;
}


