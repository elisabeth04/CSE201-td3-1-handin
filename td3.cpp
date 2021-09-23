#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    double* allocate = new double[new_size];
    for(int i=0; i<new_size; i++){
        if (i<length){
            *(allocate+i) = *(array+i);
        } else {
            *(allocate+i) = 0;
        }
    }
    delete array;
    return allocate;
}

double* shrink_array(double* array, int length, int new_size) {
    double* allocate = new double[new_size];
    for(int i=0; i<new_size; i++){
        if (i<length){
            *(allocate+i) = *(array+i);
        }
    }
    delete array;
    return allocate;
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
    if (current_size == max_size){
        array = extend_array(array, current_size, max_size+5);
        max_size = max_size+5;
    }

    *(array+current_size) = element;
    current_size++;


    return array;
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
    if (current_size <= max_size - 4){
        array = shrink_array(array, current_size, max_size-5);
        max_size = max_size-5;
    }

    current_size--;


    return array;
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {

      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
    telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);
  }
  return hit_target;
}

void sort(double *obstacles, const int num_obstacles) {
    for(int i=0;i<num_obstacles;i++){
        for(int j=i+1;j<num_obstacles;j++){
            if (obstacles[3*i] > obstacles[3*j]){
                double temp1 = obstacles[3*i];
                double temp2 = obstacles[3*i+1];
                double temp3 = obstacles[3*i+2];
                obstacles[3*i] = obstacles[3*j];
                obstacles[3*i+1] = obstacles[3*j+1];
                obstacles[3*i+2] = obstacles[3*j+2];
                obstacles[3*j] = temp1;
                obstacles[3*j+1] = temp2;
                obstacles[3*j+2] = temp3;
            }
        }
    }
    for (int i = 0; i < num_obstacles; i++){
        std::cout << obstacles[i] << std::endl;
    }
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    for (int i=0; i<tot_telemetries; i++){
        for (int j=0; j<telemetries_sizes[i]; j++){
            //std::cout << telemetries[i][j] << std::endl;
            global_telemetry = append_to_array(telemetries[i][j], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
        }
    }
    //for (int i = 0; i < global_telemetry_max_size; i++){
        //std::cout << global_telemetry[i] << std::endl;
    //}
    //std::cout << global_telemetry << std::endl;
    sort(global_telemetry, global_telemetry_current_size);

}
















