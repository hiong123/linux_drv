#include <stdio.h>
#include "print.h"

int main(void)
{
#if 0
	 printf(RED"***********************\n"NONE);
	 printf(LIGHT_RED"***********************\n"NONE);
	 printf(GREEN"***********************\n"NONE);
	 printf(LIGHT_GREEN "***********************\n"NONE);
	 printf(BLUE"***********************\n"NONE);
	 printf(LIGHT_BLUE"***********************\n"NONE);
	 printf(DARY_GRAY"***********************\n"NONE);
	 printf(CYAN"***********************\n"NONE);
	 printf(LIGHT_CYAN"***********************\n"NONE);
	 printf(PURPLE"***********************\n"NONE);
	 printf(LIGHT_PURPLE"***********************\n"NONE);
	 printf(BROWN"***********************\n"NONE);
	 printf(YELLOW"***********************\n"NONE);
	 printf(LIGHT_GRAY "***********************\n"NONE);

	 printf(WHITE "***********************\n"NONE);
	 printf(HIGH "***********************\n"NONE);
	 printf(LINE "***********************\n"NONE);
	 printf(LIGHT"***********************\n"NONE);
	 printf(F1"***********************\n"NONE);
	 printf(F2"***********************\n"NONE);
#endif
      /*
	 printf(fore1"***********************\n"NONE);
	 printf(fore2"***********************\n"NONE);
	 printf(fore3"***********************\n"NONE);
	 printf(fore4"***********************\n"NONE);
	 printf(fore5"***********************\n"NONE);
	 printf(fore6"***********************\n"NONE);
	 printf(fore7"***********************\n"NONE);
	 printf(fore8"***********************\n"NONE);

	 printf(back1"***********************\n");
	 printf(back2"***********************\n");
	 printf(back3"***********************\n");
	 printf(back4"***********************\n");
	 printf(back5"***********************\n");
	 printf(back6"***********************\n");
	 printf(back7"***********************\n");
	 printf(back8"***********************\n"NONE);
       */
#if 1
      int i1,i2,i3,i4,i,j,n = 6;
      for(i = 11,j = 0; j < n; i--,j++){
	    for(i1 = 0;i1 <= i-2*j;i1++){
		  printf(RIGHT RED"#"NONE);
		  fflush(stdout);
		  usleep(100000);
	    }
	    for(i2=0;i2 <= i-2*j-1;i2++){
		  printf(LEFT1 DOWN BLUE"#"NONE);
		  fflush(stdout);
		  usleep(100000);
	    }
	    for(i3 = 0;i3 <= i-2*j-1;i3++){
		  printf(LEFT GREEN"#"NONE);
		  fflush(stdout);
		  usleep(100000);
	    }
	    for(i4=0; i4 < i-2*j;i4++){
		  printf(LEFT1 UP YELLOW"#"NONE);
		  fflush(stdout);
		  usleep(100000);
	    }
	    printf(DOWN RIGHT);
      }
#endif
      /*
	 printf(UP"####################\n");
	 printf("***********************\n");
	 printf(RIGHT"***********************\n");
	 printf(LEFT "***********************\n");
	 printf(F3 "***********************\n");
	 printf(CLEAR"***********************\n");
	 printf(CLEAN"***********************\n");
	 printf(F4"***********************\n");
	 printf(F5"***********************\n");
	 printf(F6"***********************\n");
	 printf(F7"***********************\n");
       */
#if 0
      for(i = 0; i < 8; i++)
	    printf(LEFT1  DOWN);
      printf(LEFT);
#endif
      return 0;
}
