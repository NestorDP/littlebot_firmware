/* Copyright (c) 2022-2025 Nestor D. Pereira Neto

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INCLUDE_LITTLEBOT_FIRMWARE_TYPES_H__
#define INCLUDE_LITTLEBOT_FIRMWARE_TYPES_H__

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Motor wheel data structure containing velocity and control information
 */
typedef struct {
    float command_velocity;   /**< Commanded velocity from user/controller */
    float status_velocity;    /**< Actual measured velocity from encoder */
    float status_position;    /**< Actual measured position from encoder */
} WheelData_t;

/**
 * @brief Complete robot wheels data for both left and right wheels
 */
typedef struct {
    WheelData_t left;         /**< Left wheel data */
    WheelData_t right;        /**< Right wheel data */
} WheelsData_t;


/**
 * @brief PID controller parameters
 */
typedef struct {
    float kp;                 /**< Proportional gain */
    float ki;                 /**< Integral gain */
    float kd;                 /**< Derivative gain */
    float max_output;         /**< Maximum output limit */
    float min_output;         /**< Minimum output limit */
} PidParams_t;

/**
 * @brief System status enumeration
 */
typedef enum {
    SYSTEM_IDLE = 0,
    SYSTEM_RUNNING,
    SYSTEM_ERROR,
    SYSTEM_CALIBRATING
} SystemStatus_t;

/**
 * @brief Motor side enumeration (for consistency with existing code)
 */
typedef enum {
    MOTOR_RIGHT = 0,
    MOTOR_LEFT = 1
} MotorSide_t;

#endif // INCLUDE_LITTLEBOT_FIRMWARE_TYPES_H__