#include <stdio.h>
#include <webots/camera.h>
#include <webots/camera_recognition_object.h>
#include <webots/motor.h>
#include <webots/robot.h>

#define SPEED 5
#define TIME_STEP 64

int main() {
  WbDeviceTag camera, left_motor, right_motor;
  wb_robot_init();


  /* Get the camera device, enable it and the recognition */
  camera = wb_robot_get_device("camera");
  wb_camera_enable(camera, TIME_STEP);
  wb_camera_recognition_enable(camera, TIME_STEP);

  /* get a handler to the motors and set target position to infinity (speed control). */
  left_motor = wb_robot_get_device("left wheel motor");
  right_motor = wb_robot_get_device("right wheel motor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);

  /* Main loop */
  while (wb_robot_step(TIME_STEP) != -1) {
    /* Get current number of object recognized */
    int num = wb_camera_recognition_get_number_of_objects(camera);
    const WbCameraRecognitionObject *objects = wb_camera_recognition_get_objects(camera);
    if (num==0)
    {
        wb_motor_set_velocity(left_motor, SPEED);
        wb_motor_set_velocity(right_motor, SPEED);
    }
    else
    {
      if (objects[0].colors[1] > 0.5 && objects[0].colors[1] < 0.9)
      {
        wb_motor_set_velocity(left_motor, -SPEED);
        wb_motor_set_velocity(right_motor, SPEED);
      }
      else if (objects[0].colors[0] > 0.3 && objects[0].colors[1] < 0.9)
      {
        wb_motor_set_velocity(left_motor, SPEED);
        wb_motor_set_velocity(right_motor, -SPEED);
      }
      else
      {
        if (objects[0].colors[0] >= 0.95)
        {
        wb_motor_set_velocity(left_motor, 0.0);
        wb_motor_set_velocity(right_motor, 0.0);
        printf("\n Ball found! \n");
        break;
        }
      }
     }

}
  wb_robot_cleanup();

  return 0;
}
