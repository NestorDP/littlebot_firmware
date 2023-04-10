#include "littlebot_firmware/serialization.h"


void SerializationConstruct(Serialization *self) {
  self->Decode = fcDecode;
  self->Encode = fcEncode;
}


void fcDecode (Serialization *self, char *msg, float *left_vel, float *right_vel) {
  char serialized_msg[80];
  char *split_msg;

  split_msg = strtok(msg, "#");
  *left_vel = atof(split_msg);

  split_msg = strtok(NULL, "#");
  *right_vel = atof(split_msg);
}


void fcEncode (Serialization *self, char *msg, float *left_vel, float *right_vel) {
  char str_left[20];
  char str_right[20];

  FloaToStr(*left_vel, str_left, 2);
  FloaToStr(*right_vel, str_right, 2);

  strcpy(msg, str_left);
  strcat(msg, "#");
  strcat(msg, str_right);
}


void FloaToStr(float num, char *str, int afterpoint) {
      // Extract integer part
    int ipart = (int)num;
 
    // Extract floating part
    float fpart = num - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, str, 0);
 
    // check for display option after point
    if (afterpoint != 0) {
        str[i] = '.'; // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, str + i + 1, afterpoint);
    }
}


int intToStr(int num, char *str, int d)
{
    int i = 0;
    while (num) {
        str[i++] = (num % 10) + '0';
        num = num / 10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}


// Reverses a string 'str' of length 'len'
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}