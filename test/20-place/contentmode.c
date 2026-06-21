#include <mulle-layout/mulle-layout.h>

#include <stdio.h>


int   main()
{
   unsigned int    i, j;

   for( j = 0; j <= 0xF0; j +=16)
   {
      for( i = 0; i <= 0xF; i++)
      {
         mulle_printf( "0x%02x: %s\n", i + j, CGContentModeIsValid( i + j) ? "valid" : "invalid");
      }
      mulle_printf( "\n");
   }
   return( 0);
}
