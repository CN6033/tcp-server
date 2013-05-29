/* strncat example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str1[20];
  char str2[20];
  strcpy (str1,"Tobe");
  strcpy (str2,"ornottobe");
  strncat (str1, str2, 1);
  puts (str1);
  return 0;
}
