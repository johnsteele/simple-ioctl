
#ifndef _SIMPLE_IOCTL_H_
#define _SIMPLE_IOCTL_H_

#ifndef MY_MAJOR
#define MY_MAJOR 0
#endif

#ifndef MY_MINOR 
#define MY_MINOR 0
#endif

#ifndef MY_NUM_DEVICES
#define MY_NUM_DEVICES 1
#endif

extern int my_major;
extern int my_minor;
extern int my_num_devices;

#endif /* _SIMPLE_IOCTL_H_ */
