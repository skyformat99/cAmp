#pragma once

#define MP   MAX_PATH+60  //path+file name
#define ExtLen     5  // max ext

#define NamePls   24  // max plst tab name
#define NameSrch  32  // max search name

const INT64 smax = INT64(MAXDWORD)+1;


//  enum CTrk type
const byte
	TY_DIR_unv=0, TY_DIR=1,  // dir unvisited
	TY_FILE=2, TY_AUDIO=3;   // files


//  rating   = -- -  ` ^ ~ + *
const int cR0= 3/*- */, cR1= 5/*+*/, chRall=cR0+cR1+1;
const char chRates[chRall] = {'=',150,'-',' ','`','^','~','+','*'};  // 176 o  183 . 215 x 149 o* 150 - 151 --
const char chDRates[chRall] = {'=',33,34,' ',39,40,41,'+','*'};  //cfdig =,33-150,34-,39` 40^ 41~ +*

//  rating  in file name
const int  chFnAll = 7;  //special -2 -- 
const char cFnCharRates[chFnAll] = {'=','-', '`','^','~','+','#'};
const char cFnNumRates[chFnAll] = {-3,-1, 1,2,3,4,5};
const static char chFRate[chRall][3] = {"=","--","-","","`","^","~","+","#"};  //rename add
