#include "piano.h"
#define NOTAS_LOOP	384
#define NOTAS_EXPL	5
#define NOTAS_MENU	354

//http://tabnabber.com/view_Tab.asp?tabID=17601&sArtist=NYAN+CAT&sName=NYAN+VAT



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
		G5
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


unsigned long theme_menu[]=
{
		
		d4,no,no,no,d4,no,d4,no,no,no,d4,no,d4,no,no,no,d4,no,d4,no,d4,no,d4,no,d4,no,no,no,d4,no,d4,no,no,no,d4,no,d4,no,no,no,d4,no,d4,no,d4,no,d4,no,d4,no,no,no,d4,no,d4,no,no,no,d4,no,d4,no,no,no,d4,no,d4,no,a3,no,c4,no,

		d4,no,no,no,d4,no,no,no,d4,no,e4,no,f4,no,no,no,f4,no,no,no,f4,no,g4,no,e4,no,no,no,e4,no,no,no,d4,no,c4,no,c4,no,d4,no,no,no,no,no,a4,no,c4,no,d4,no,no,no,d4,no,no,no,d4,no,e4,no,f4,no,no,no,f4,no,no,no,f4,no,g4,no,

		e4,no,no,no,e4,no,no,no,d4,no,c3,no,d3,no,no,no,no,no,no,no,a3,no,d4,no,d4,no,no,no,d4,no,no,no,d4,no,f4,no,g4,no,no,no,g4,no,no,no,g4,no,a4,no,A4,no,no,no,A4,no,no,no,a4,no,g4,no,a4,no,d4,no,no,no,no,no,d3,no,e4,no,

		f4,no,no,no,f4,no,no,no,g4,no,no,no,a4,no,d4,no,no,no,no,no,d4,no,f4,no,e4,no,no,no,e4,no,no,no,f4,no,d4,no,e4,no,no,no,no,no,no,no,a4,no,c5,no,d5,no,no,no,d5,no,no,no,d5,no,e5,no,f5,no,no,no,f5,no,no,no,f5,no,g5,no,

		e5,no,no,no,e5,no,no,no,d5,no,c5,no,c5,no,d5,no,no,no,no,no,a4,no,c5,no,d5,no,no,no,d5,no,no,no,d5,no,e5,no,f5,no,no,no,f5,no,no,no,f5,no,g5,no,e5,no,no,no,e5,no,no,no,d5,no,c5,no,  no,no,no,no,no,no

};
/*
72
72
72
72
60

*/
