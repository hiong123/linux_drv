#include <stdio.h>


int str_int(char *str)
{
      int time;
      time = (str[0]-'0')*10+str[1]-'0';
      time = time*60+(str[3]-'0')*10+str[4]-'0';
      return time;
}
void int_str(char *str,int time)
{
      int h,m;
      h = time/60;
      m = time%60;
      str[0] = h/10+'0';
      str[1] = h%10+'0';
      str[2] = ':';
      str[3] = m/10+'0';
      str[4] = m%10+'0';
      str[5] = '\0';
      //printf("%d:%d\n",h,m);
      printf("%s\n",str);
}

int main(void)
{
      char str[10];
       int time;
      scanf("%s",str);
       time = str_int(str);
      printf("%s 转换成整数为:%d\n",str,str_int(str));
      printf("%d 转换成整数为:",time);
      int_str(str,time);
      return 0;
}
