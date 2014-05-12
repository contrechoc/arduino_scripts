int letterAndByte(int num){
  
  int newNum = 0;
  //numbers
  if ( num == 1 )  newNum = B00000110;//1
  if ( num == 2 )  newNum =  B01011011;//2
  if ( num == 3 )  newNum =  B01001111;//3
  if ( num == 4 )  newNum =  B01100110;//4
  if ( num == 5 )  newNum =  B01101101;//5
  if ( num == 6 )  newNum =  B01111101;//6
  if ( num == 7 )  newNum =  B00000111;//7 
  if ( num == 8 )  newNum =  B01111111;//8
  if ( num == 9 )  newNum =  B01101111;//9
  if ( num == 0 )  newNum =  B00111111;//0
  //comma
  if ( num == 10 )  newNum = B10000000;//comma
  //some letters
  if ( num == 11 )  newNum =  B01110111;//A
  if ( num == 12 )  newNum =  B01111111;//B like 8
  if ( num == 13 )  newNum =  B00111001;//C
  if ( num == 14 )  newNum =  B00111101;//D small like 6 without upper stroke
  if ( num == 15 )  newNum =  B01111001;//E
  if ( num == 16 )  newNum =  B01111000;//F
  if ( num == 17 )  newNum =  B01101111;//small G like 9
  if ( num == 18 )  newNum =  B01110110;//H
  if ( num == 19 )  newNum =  B00000110;//I like 1
  if ( num == 20 )  newNum =  B00011110;//J
  // K?
  if ( num == 21 )  newNum =  B00111000;//L
  //M
  //N
  if ( num == 22 )  newNum =  B00111111;//O like 0
  if ( num == 23 )  newNum =  B01110011;//P
  if ( num == 24 )  newNum =  B10111111;//Q like 0.
  //R
  if ( num == 25 )  newNum =  B01101101;//S like 5
  if ( num == 26 )  newNum =  B01111001;//t small
  if ( num == 27 )  newNum =  B00111110;//U
  //V
  //W
  //X
  //Y
  //Z

  //special signs
  if ( num == 40 )  newNum =  B01011100;//under circle
  if ( num == 41 )  newNum =  B01100011;//upper circle
  if ( num == 42 )  newNum =  B00000000;//empty
}
