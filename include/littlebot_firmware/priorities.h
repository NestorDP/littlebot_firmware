#ifndef __PRIORITIES_H__
#define __PRIORITIES_H__


/* The priorities of the tasks. */
#define PRIORITY_SERIAL_COMMUNICATION 1
#define PRIORITY_LEFT_MOTOR_TASK      2
#define PRIORITY_RIGHT_MOTOR_TASK     3

/* Size of queues and items. */
#define VELOCITY_ITEM_SIZE  sizeof(float)
#define VELOCITY_QUEUE_SIZE 5

#endif // __PRIORITIES_H__
