#include "littlebot_firmware/serialization.h"


void SerializationConstruct(Serialization *self) {
  self->ReceiveMessage = fcReceiveMessage;
  self->Encode = fcEncode;

  // SerialInterfaceContruct(&self->serial, 115200);
}


void fcReceiveMessage (Serialization *self, float *left_vel, float *right_vel) {
  char serialized_msg[200];
  char var_left[4];
  char var_right[4];

  // self->serial.Read (&self->serial, serialized_msg, sizeof(serialized_msg));

  strncpy (var_left, serialized_msg, 4);
  strncpy (var_right, serialized_msg + 4, 4);

  left_vel = (float *)var_left;
  right_vel = (float *)var_right;
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


void FloaToStr(float n, char *str, int afterpoint) {
      // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
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


int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
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