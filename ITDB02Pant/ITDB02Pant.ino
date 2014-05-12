#include <ITDB02.h>

ITDB02 lcd(0,1,2,3,4,5,6,7,19,18,17,16); // 8 bit mode
//ITDB02 lcd(8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,19,18,17,16);

void setup()
{
  lcd.Initial(0);
  
   lcd.Pant(0,0,239,300,0x0);
 //delay(500);
 lcd.Pant(0,0,239,200,0xff00);
// delay(500);
 lcd.Pant(0,0,239,100,0x00ff);
 //delay(500);
  lcd.Pant(0,0,239,50,0xffff);
 //delay(500);
  lcd.Pant(0,0,50,319,0xf000);
    lcd.Pant(180,0,239,319,0xf000);
    
  int add=10;
  lcd.Setcolor(0xff00,0x00ff);
  lcd.Dispshowchar(add,10,'C');
  add+=7;
 // lcd.Setcolor(0xaa00,0x00ff);
  lcd.Dispshowchar(add,10,'O');
  add+=7;
//  lcd.Setcolor(0xaacc,0x00ff);
  lcd.Dispshowchar(add,10,'N');
  add+=7;
//  lcd.Setcolor(0xffcc,0x00ff);
  lcd.Dispshowchar(add,10,'T');
  add+=7;
 // lcd.Setcolor(0xadf0,0x00ff);
  lcd.Dispshowchar(add,10,'R');
  add+=7;
 // lcd.Setcolor(0xcfea,0x00ff);
  lcd.Dispshowchar(add,10,'E');
  add+=7;
 // lcd.Setcolor(0xaa00,0x00ff);
  lcd.Dispshowchar(add,10,'C');
  add+=7;
 // lcd.Setcolor(0xdf30,0x00ff);
  lcd.Dispshowchar(add,10,'H');
  add+=7;
 // lcd.Setcolor(0xc83f,0x00ff);
  lcd.Dispshowchar(add,10,'O');
  add+=7;
 // lcd.Setcolor(0x087c,0x00ff);
  lcd.Dispshowchar(add,10,'C');


  lcd.Setcolor(0x00,0xffff);
  lcd.Dispshowstr(80,300,"www.contrechoc.com");
}

void loop()
{

  
}
