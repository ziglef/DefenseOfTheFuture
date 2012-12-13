#include "piano.h"
#define NOTAS_TOTAL 430
#define NOTAS_LOOP	384
#define NOTAS_EXPL	5

//http://tabnabber.com/view_Tab.asp?tabID=17601&sArtist=NYAN+CAT&sName=NYAN+VAT

unsigned long theme_song[]=
{
		D5,no,e5,F5,no,no,b5,no,no,D5,no,e5,F5,no,b5,C6,no,D6,C6,no,A5,b5,no,no,F5,no,no,D5,no,e5,F5,no,no,b5,no,no,C6,no,A5,b5,no,C6,e6,no,D6,e6,no,C6,F5,no,no,G5,no,no,D5,no,D5,F3,no,b4,d5,no,C5,b4,no,no,b4,no,no,C5,no,no,d5,no,no,

		d5,no,C5,b4,no,C5,D5,no,F5,G5,no,D5,F5,no,C5,D5,no,b4,C5,no,b4,D5,no,no,F5,no,no,G5,no,D5,F5,no,C5,D5,no,b4,d5,no,D5,d5,no,C5,b4,no,C5,d5,no,no,b4,no,C5,D5,no,F5,C5,no,D5,C5,no,b4,C5,no,no,b4,no,no,C5,no,no,F5,no,no,G5,no,no,

		D5,no,D5,F3,no,b4,d5,no,C5,b4,no,no,b4,no,no,C5,no,no,d5,no,no,d5,no,C5,b4,no,C5,D5,no,F5,G5,no,D5,F5,no,C5,D5,no,b4,C5,no,b4,D5,no,no,F5,no,no,G5,no,D5,F5,no,C5,D5,no,b4,d5,no,D5,d5,no,C5,b4,no,C5,d5,no,no,b4,no,C5,D5,no,F5,

		C5,no,D5,C5,no,b4,C5,no,no,b4,no,no,C5,no,no,b4,no,no,F4,no,G4,b4,no,no,F4,no,G4,b4,no,C5,D5,no,b4,e5,no,D5,e5,no,F5,b4,no,no,b4,no,no,F4,no,G4,b4,no,F4,e5,no,D5,C5,no,b4,F4,no,D4,e4,no,F4,b4,no,no,F4,no,G4,b4,no,no,F4,no,G4,

		b4,no,b4,C5,no,D5,b4,no,F4,G4,no,F4,b4,no,no,b4,no,A4,b4,no,F4,G4,no,b4,e5,no,D5,e5,no,F5,b4,no,no,A4,no,no,b4,no,no,F4,no,G4,b4,no,no,F4,no,G4,b4,no,C5,D5,no,b4,e5,no,D5,e5,no,F5,b4,no,no,b4,no,no,F4,no,G4,b4,no,F4,e5,no,D5,

		C5,no,b4,F4,no,D4,e4,no,F4,b4,no,no,F4,no,G4,b4,no,no,F4,no,G4,b4,no,b4,C5,no,D5,b4,no,F4,G4,no,F4,b4,no,no,b4,no,A4,b4,no,F4,G4,no,b4,e5,no,D5,e5,no,F5,b4,no,no,C5

};
/*
75
75
75
75
75
55
total
430
*/

unsigned long theme_loop[]=
{
		F5,no,no,G5,no,no,D5,no,D5,F3,no,b4,d5,no,C5,b4,no,no,b4,no,no,C5,no,no,d5,no,no,

		d5,no,C5,b4,no,C5,D5,no,F5,G5,no,D5,F5,no,C5,D5,no,b4,C5,no,b4,D5,no,no,F5,no,no,G5,no,D5,F5,no,C5,D5,no,b4,d5,no,D5,d5,no,C5,b4,no,C5,d5,no,no,b4,no,C5,D5,no,F5,C5,no,D5,C5,no,b4,C5,no,no,b4,no,no,C5,no,no,F5,no,no,G5,no,no,

		D5,no,D5,F3,no,b4,d5,no,C5,b4,no,no,b4,no,no,C5,no,no,d5,no,no,d5,no,C5,b4,no,C5,D5,no,F5,G5,no,D5,F5,no,C5,D5,no,b4,C5,no,b4,D5,no,no,F5,no,no,G5,no,D5,F5,no,C5,D5,no,b4,d5,no,D5,d5,no,C5,b4,no,C5,d5,no,no,b4,no,C5,D5,no,F5,

		C5,no,D5,C5,no,b4,C5,no,no,b4,no,no,C5,no,no,b4,no,no,F4,no,G4,b4,no,no,F4,no,G4,b4,no,C5,D5,no,b4,e5,no,D5,e5,no,F5,b4,no,no,b4,no,no,F4,no,G4,b4,no,F4,e5,no,D5,C5,no,b4,F4,no,D4,e4,no,F4,b4,no,no,F4,no,G4,b4,no,no,F4,no,G4,

		b4,no,b4,C5,no,D5,b4,no,F4,G4,no,F4,b4,no,no,b4,no,A4,b4,no,F4,G4,no,b4,e5,no,D5,e5,no,F5,b4,no,no,A4,no,no,b4,no,no,F4,no,G4,b4,no,no,F4,no,G4,b4,no,C5,D5,no,b4,e5,no,D5,e5,no,F5,b4,no,no,b4,no,no,F4,no,G4,b4,no,F4,e5,no,D5,

		C5,no,b4,F4,no,D4,e4,no,F4,b4,no,no,F4,no,G4,b4,no,no,F4,no,G4,b4,no,b4,C5,no,D5,b4,no,F4,G4,no,F4,b4,no,no,b4,no,A4,b4,no,F4,G4,no,b4,e5,no,D5,e5,no,F5,b4,no,no,C5,no,no

};
/*
27
75
75
75
75
55
382
  */

unsigned long shots[]=
{
		G7
};

unsigned long explosion1[]=
{
		c3,a2,g2,e2,c2
};
//duraçao: 0.5s

unsigned long explosion2[]=
{
		c3,no,a2,no,f2
};


unsigned long explosion3[]=
{
		c3,no,f2,no,c2
};
