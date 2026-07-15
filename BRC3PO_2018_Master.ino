//2018 version using FastLED library - PDG
// refactored using RGBset features
// Set up for Teensy and Octo adapter---no specific pin declarations

#define USE_OCTOWS2811
#include<OctoWS2811.h>
#include <FastLED.h>


FASTLED_USING_NAMESPACE
//////////////////////////////
// forward function declarations

void LetterColorShift(int ihue) ;
void setLetterColor(CRGB color) ;
void First_Show() ;
void FillUFFromPaletteColors( uint8_t colorIndex) ;
void FillLettersFromPalette( uint8_t index) ;
void ShiftLeds( int istart ,int numleds, int shift) ;
void ShiftLetters(int shift ) ;
void dance(int ncycles) ;
void Second_Show()  ;
void ColorWave(int n) ;
void ChangePalettePeriodically( ) ;
void ColorDrop(int cdx) ;
void DiscoStrobe()  ;
void discoWorker( 
    uint8_t dashperiod, uint8_t dashwidth, int8_t  dashmotionspeed,
    uint8_t stroberepeats,
    uint8_t huedelta ) ;
static void drawRainbowDashes( 
  uint8_t startpos, uint16_t lastpos, uint8_t period, uint8_t width, 
  uint8_t huestart, uint8_t huedelta, uint8_t saturation, uint8_t value ) ;
static void delayToSyncFrameRate( uint8_t framesPerSecond ) ;

 
// Potentiometers
  #define POT_COLOR 3
  #define POT_BRIGHTNESS 5


//////////////////////////////
 #define NUM_STRIPS 8
 #define NUM_LEDS_PER_STRIP 144
 // must be at least as long as the longest strip
 
 #define NUM_LEDS  NUM_STRIPS * NUM_LEDS_PER_STRIP

///////////////////////////
//generalized LED counts 
const int NUM_B = 141 ;
const int NUM_R = 119 ;
const int NUM_C =  79 ;
const int NUM_3 =  94 ;
const int NUM_P = 104 ;
const int NUM_O =  128 ;
const int NUM_U = 126 ;
const int NUM_F = 143 ;
// set limit for copying array
const int Nmax = 144 ;

// array offsets
/* these are determined by the physical connections between the Teensy and
 *  the strip connectors at the patch box. The connections denoted as 1 and 2 must correspond between the control box and the patch box
 *  but the cables are interchangable. They are labeled for convenience during hookup.
 */
const int THREE_START = 0 ;
const int C_START = NUM_LEDS_PER_STRIP ;
const int R_START = 2 * NUM_LEDS_PER_STRIP ;
const int B_START = 3 * NUM_LEDS_PER_STRIP ;
const int F_START = 4 * NUM_LEDS_PER_STRIP ;
const int U_START = 5 * NUM_LEDS_PER_STRIP ;
const int O_START = 6 * NUM_LEDS_PER_STRIP ;
const int P_START = 7 * NUM_LEDS_PER_STRIP ;

///////////////////////
#define LED_TYPE NEOPIXEL

#define BRIGHTNESS 96 //set initial brightness
#define MAX_BRIGHTNESS 240 //set maximum brightness


//#define FRAMES_PER_SECOND 120

// set up led array
//Array size is governed by the longest strip length
CRGBArray <NUM_LEDS> leds ;

// individual sets, one for each strip

CRGBSet Strip_B(leds (B_START, B_START + NUM_B - 1 )) ;
CRGBSet Strip_R(leds (R_START, R_START + NUM_R - 1 )) ;
CRGBSet Strip_C(leds (C_START, C_START + NUM_C - 1 )) ;
CRGBSet Strip_3(leds (THREE_START, THREE_START + NUM_3 - 1 )) ;
CRGBSet Strip_P(leds (P_START, P_START + NUM_P - 1 )) ;
CRGBSet Strip_O(leds (O_START, O_START + NUM_O - 1 )) ;
CRGBSet Strip_U(leds (U_START, U_START + NUM_U - 1 )) ;
CRGBSet Strip_F(leds (F_START, F_START + NUM_F - 1 )) ;

// copying array
CRGB CopyLeds[Nmax] ;

//list of colors used in colordrop
CRGB colorList[15] =  {CRGB::Blue , CRGB::Yellow , CRGB::Red, CRGB::Purple,
                  CRGB::Green , CRGB::Pink , CRGB::Teal , CRGB::Orange ,
                  CRGB::Cyan , CRGB::Magenta , CRGB::DarkBlue, CRGB::Gold,
                  CRGB::Violet , CRGB::SeaGreen , CRGB::DarkRed   } ;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;                  


///////////////////////

// enable the power indicator led on the teensy for debugging
const int powerPin =  13 ;


// startup display flag
boolean StartUpOn = true ;
// animation toggle
boolean ShowFlag = true ;

// Underline NeoPixels...set up scanner eye  
  unsigned long prevTime, timex;
  unsigned long ulDelay = 20;     // Rate of chase
  int pos = 0, dir = 1; // Position, direction of "eye"
  int eyePadding = 10;      // Number of pixels surrounding "eye" per side
  CRGB ulColor = CRGB::White ;
  CRGB flagColor = CRGB::Red ;
  
// define color and brightness for pot control  
  uint8_t letter_hue;
  uint8_t current_brightness;
CRGB currentColor ; 
#define ARRAY_SIZE(A) (sizeof(A) / sizeof(A[0]))

//   int ntest = 0 ;

// set up a timing interval for the main loop in millis
  int Frame_Length = 100 ;
  int NFrames = 60000 / Frame_Length ;
  long nfr ;
  
// duration of while loops in seconds
  int duration = 120 ;

  
void setup() {
   Serial.begin(9600);
  Serial.println("Connected");
// turn on the led on the teensy to show that we have power  
  pinMode (powerPin, OUTPUT) ;
  digitalWrite (powerPin, HIGH) ;
  
////     define the entire array of leds using the "one array, many strips" protocol  
    LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);

    
// set master brightness to initialize...will be linked to brightness pot
  LEDS.setBrightness(BRIGHTNESS) ;   

  // initialize the leds array to all off
       fill_solid(leds, NUM_LEDS, CRGB::Black) ;  
       
  prevTime = millis();
}


void loop() {
// test section ////////

      
///////////////////////////////////////////  

 // update letter color and brightness from pots///////// 
 
  letter_hue = map(analogRead(POT_COLOR), 0, 1023, 0, 255);
  current_brightness = map(analogRead(POT_BRIGHTNESS), 0, 1023, 0, MAX_BRIGHTNESS);
  CHSV read_color(letter_hue, 255, current_brightness) ;
  currentColor = read_color ;
  setLetterColor( currentColor) ;
  LEDS.setBrightness(current_brightness) ;
  
  
///set flag color in case it has changed  /////////////
  fill_solid(Strip_F, NUM_F , flagColor) ;
  
  LEDS.show() ;
  
  //Underline Animation
    timex = millis();
  if ((timex - prevTime) >= ulDelay) {
    
// Rather than being sneaky and erasing just the tail pixel,
// it's easier to erase it all and draw a new one next time.   
    if ( (pos >=   2) &&  ( pos <= NUM_U - 3 ) ) {
    fill_solid(Strip_U, NUM_U, CRGB::Black) ;
    }
    
    //Pad "eye"
    // pos is the relative position along the strip, between 0 and NUM_U - 1
    leds[U_START + pos] = ulColor ;
    for (int i = 0; i < eyePadding; i++) {
       // set all of the eye to the brightest color
        if (pos - i >= 0) { leds[U_START + pos - i] = ulColor ; }
        if (pos + i < NUM_U - 1 ) { leds[U_START + pos + i] = ulColor ; }    
        // Center pixel is brightest, others are dimmed 
      if (pos + i + 1 <= NUM_U - 1 ) {
          leds [U_START + pos + i + 1] =leds [U_START + pos + i + 1].nscale8(200) ;
      } 
      if (pos - i - 1 >= 0) {
          leds [U_START + pos - i - 1 ] = leds [U_START + pos - i - 1 ].nscale8(200) ;
      }
    }
    
    // Bounce off ends of strip
    pos += dir;
    if (pos < 0) {
      pos = 1;
      dir = -dir;
    } else if (pos >= NUM_U - 1) {
      pos = NUM_U - 2;
      dir = -dir;
    }
  } 
    //Update elapsed time
    prevTime = timex;
    
    LEDS.show() ;
    LEDS.delay(Frame_Length) ;
    
  // startup display runs both animation sequences once at power up    
  if(StartUpOn == true) { 
// initialize the leds array to all off
       fill_solid(leds, NUM_LEDS, CRGB::Black) ;       
       
      First_Show() ;

      Second_Show() ; 
//    Serial.println( "end startup" ) ;  
   }  
      StartUpOn = false ;
///// end of startup section ///////////////////////
  
/* new section using every_N_minutes timers and a new toggle flag    
 *  to run the First_Show and Second_Show animation sequences alternately
 */
   EVERY_N_MINUTES( 10 ) {
      if( ShowFlag) { First_Show() ;}
      else {Second_Show() ;}
      ShowFlag = !ShowFlag ;
  }
  
}
// end of main loop




void setLetterColor(CRGB color) {
  fill_solid (Strip_B, NUM_B, color) ;
  fill_solid (Strip_R, NUM_R, color) ;
  fill_solid (Strip_C, NUM_C, color) ;
  fill_solid (Strip_3, NUM_3, color) ;
  fill_solid (Strip_P, NUM_P, color) ;
  fill_solid (Strip_O, NUM_O, color) ; 
}

// first set of animations

void First_Show() {
 currentBlending = LINEARBLEND ;
 
//initialize loop timer 
unsigned long  SUinit = millis() ;
unsigned long  SUtimer = SUinit ;
int ntick =0 ;
// start loop
 unsigned long duration = 120*1000L ; 
  while ( (SUtimer - SUinit) < duration) {   // run for 2 min
// underline and flag use rainbow
  currentPalette = RainbowColors_p ;
    static uint8_t startIndex = 0;
    FillUFFromPaletteColors( startIndex); 
// letters use partycolors
    currentPalette = PartyColors_p ;
    FillLettersFromPalette(startIndex) ;
  startIndex += 1 ;
  LEDS.show() ;
  LEDS.delay(200) ;


// update timer  
  SUtimer = millis() ; 
  ntick++ ;
  } 
// end loop


// reset colors for flag and underline. 
// read pots for color and brightness
 // update letter color and brightness from pots///////// 
 
  letter_hue = map(analogRead(POT_COLOR), 0, 1023, 0, 255);
  current_brightness = map(analogRead(POT_BRIGHTNESS), 0, 1023, 0, MAX_BRIGHTNESS);
  CHSV new_color(letter_hue, 255, current_brightness) ;
  currentColor = new_color ;
  setLetterColor( currentColor) ;
  LEDS.setBrightness(current_brightness) ;


fill_solid (Strip_F, NUM_F, CRGB::Red) ;
fill_solid (Strip_U, NUM_U ,CRGB::White) ;
  LEDS.show() ;
  LEDS.delay(200) ;
  
//dance 10 cycles
  dance(10) ;
// reset underline to black as background for the cylon
  fill_solid(Strip_U, NUM_U, CRGB::Black) ;
}

void FillUFFromPaletteColors( uint8_t colorIndex) {

    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_U; i++) {
        Strip_U[i] = ColorFromPalette( currentPalette, colorIndex , brightness, currentBlending);
        colorIndex += 3;
    }
    
    for( int i = 0; i < NUM_F; i++) {
        Strip_F[ i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void FillLettersFromPalette( uint8_t index) {
 CRGB color = ColorFromPalette(currentPalette, index, 255 , currentBlending) ;
  fill_solid (Strip_B, NUM_B, color) ;
  color = ColorFromPalette(currentPalette, index + 6, 255 , currentBlending) ;
  fill_solid (Strip_R, NUM_R, color) ;
   color = ColorFromPalette(currentPalette, index + 12, 255 , currentBlending) ; 
  fill_solid (Strip_C, NUM_C, color) ;
    color = ColorFromPalette(currentPalette, index + 18, 255 , currentBlending) ;
  fill_solid (Strip_3, NUM_3, color) ;
    color = ColorFromPalette(currentPalette, index + 24, 255 , currentBlending) ;
  fill_solid (Strip_P, NUM_P, color) ;
    color = ColorFromPalette(currentPalette, index + 30, 255 , currentBlending) ;
  fill_solid (Strip_O, NUM_O, color) ;
}

// shift fn assumes shift is + or - 1 for the ends
// need to generalize for arbitrary length of n < numleds

void ShiftLeds( int istart ,int numleds, int shift) {
  if( shift > 0) {
    leds[istart + numleds] = leds[istart] ;
    for (int j = istart + numleds - 1 ; j > istart ; j--) {
      leds[j] = leds[j - shift] ;
    }
  }
    else {
      leds[istart] = leds[istart + numleds] ;
      for (int j = istart + 1 ; j < istart + numleds ; j++){
        leds[j] = leds[j - shift] ; 
      }
    }
}
void ShiftLetters(int shift ) {
// shift all letters by the same number of pixels

ShiftLeds( B_START, NUM_B, shift) ;
ShiftLeds( R_START, NUM_R, shift) ;
ShiftLeds( C_START, NUM_C, shift) ;
ShiftLeds( THREE_START, NUM_3, shift) ;
ShiftLeds( P_START, NUM_P, shift) ;
ShiftLeds( O_START, NUM_O, shift) ;
}

void dance(int ncycles) {
    
// black out every other pixel on all letters

  for (int i = 0 ; i < NUM_B;  i = i +2) {
    leds[B_START + i ] = CRGB::Black ;
  }
  for (int i = 0 ; i < NUM_R;  i = i +2) {
    leds[R_START + i ] = CRGB::Black ;
  } 
  for (int i = 0 ; i < NUM_C;  i = i +2) {
    leds[C_START + i ] = CRGB::Black ;
  }
  for (int i = 0; i < NUM_3;  i = i +2) {
    leds[THREE_START + i ] = CRGB::Black ;
  }  
  for (int i = 0 ; i < NUM_P;  i = i +2) {
    leds[P_START + i ] = CRGB::Black ;
  } 
  for (int i = 0 ; i < NUM_O;  i = i +2) {
    leds[O_START + i ] = CRGB::Black ;
  }
  LEDS.show() ;
  LEDS.delay( 1000) ;
 //shift the pixels back and forth ncycles times 
   for (int i=1 ; i <= ncycles ; i++) {
      ShiftLetters( 1 ) ;
        LEDS.show() ;
        LEDS.delay( 300) ;
      ShiftLetters( 1 ) ;
        LEDS.show() ;
        LEDS.delay( 300 ) ;
      ShiftLetters( -1 ) ;
        LEDS.show() ;
        LEDS.delay( 300 ) ;
      ShiftLetters( -1 ) ;
        LEDS.show() ;
        LEDS.delay( 300) ;
    }
    
}


void Second_Show() {
//    Serial.println ("in Second_Show" ) ;
// the stuff that happens in the second animation group

  
// black out everything for a half second
 setLetterColor(CRGB::Black) ;
 fill_solid (Strip_F, NUM_F, CRGB::Black) ;
 fill_solid (Strip_U, NUM_U, CRGB::Black) ;
 LEDS.show() ;
 LEDS.delay(500) ;
 
// begin with 3 white letter flashes
for(int i=1; i < 4 ; i++) {
  setLetterColor(CRGB::White) ;
  LEDS.show() ;
  LEDS.delay(200) ;
  setLetterColor(CRGB::Black) ;
  LEDS.show() ;
  LEDS.delay(400) ;
}

  
// begin the timing loop for the Second Show (S_2) animations
int ipalx = 0 ;  //palette index to pass to LetterColorShift
unsigned long S_2start = millis() ;
unsigned long S_2timer = S_2start ;
unsigned long S_2duration = 120*1000L ; // 2 minuies
#define FRAMES_PER_SECOND   100

#define ZOOMING_BEATS_PER_MINUTE 122

while( (S_2timer - S_2start) < (60 * 1000L) ) { //duration is 1 min for testing
    Serial.println(" in S2_timer loop") ;
static int ntick = 0 ;
static int cindex = 0 ;
 
  EVERY_N_SECONDS (10) { // change color index
    cindex+= 3 ;
  }
    if(cindex > 12) {cindex = 0 ;}
    
// each function changes the colors in a portion of the overall leds array
    ColorDrop(cindex) ;
    LetterColorShift(ipalx) ;
    DiscoStrobe() ;
 LEDS.show();

// delay just enough to keep a steady frame rate, e.g 100 FPS
  delayToSyncFrameRate( FRAMES_PER_SECOND);
  
// update timer
  ntick++ ;
    EVERY_N_SECONDS(1) {
    ipalx++ ;
    }
  if(ipalx > 15 ) {ipalx = 0 ;}
  S_2timer = millis() ;
}
// end with 5 white flashes
for(int i=1; i < 6 ; i++) {
  setLetterColor(CRGB::White) ;
  LEDS.show() ;
  LEDS.delay(200) ;
  setLetterColor(CRGB::Black) ;
  LEDS.show() ;
  LEDS.delay(400) ;
  }

// end of Second Show   
}


void ChangePalettePeriodically( ) {

    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = LavaColors_p;         currentBlending = LINEARBLEND; }

        if( secondHand == 12)  { currentPalette = OceanColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 24)  { currentPalette = RainbowColors_p;            currentBlending = LINEARBLEND; }

        if( secondHand == 36)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 48)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }

    }
}


void ColorDrop(int cdx) {
//  Serial.println("in ColorDrop " ) ;
// set up position counters and colors  
  int topled = 103 ;
//  int nfp = n % topled ; // not needed? if needed add n to arg list
static  int pos1 = topled ;
static  int pos2 = pos1 + 33 ;
static  int pos3 = pos2 + 33 ;

int cdx2 = cdx + 1 ;
int cdx3 = cdx + 2 ;
  CRGB color1 = colorList[cdx] ;
  CRGB color2 = colorList[ cdx + 1 ] ;
  CRGB color3 = colorList[ cdx + 2 ];

// controls for updating the leds
  
  fadeToBlackBy( Strip_F, NUM_F, 10);
  
  Strip_F[pos1] = CRGB::White ;
  if( pos1 < topled ) {
    Strip_F[ pos1 + 1] = color1 ;
  }
  pos1 = pos1 -1 ;
  if(pos1 < 0) { pos1 = topled ; 
  Strip_F[0] = color1 ;
  color1 = colorList[cdx +3] ;
}
  
// check if pos2 is in range for next color  
  pos2 = pos2 - 1 ;
  if (pos2 < 0) {      // reset to top and change color
  pos2 = topled ;
  Strip_F[0] = color2 ;
  color2 = colorList[cdx2 + 3] ;
}
  if (pos2 < topled ) {
    Strip_F[ pos2 ] = CRGB::White ;
  }
    if(pos2 < topled - 1) {
      Strip_F[ pos2 +1] = color2 ;
    }
    
  // check if pos3 is in range for next color
  pos3 = pos3 - 1 ;
  if (pos3 < 0 ) {      // reset to top and change color
  pos3 = topled ;
  Strip_F[0] = color3 ;
  color3 = colorList[cdx3 + 3] ;
}
  if (pos3 < topled ) {
    Strip_F[ pos3 ] = CRGB::White ;
  }
    if(pos3 < topled - 1) {
      Strip_F[ pos3 +1] = color3 ;
    }

}
void DiscoStrobe() {
// Adapted from Mark Kriegsman's DiscoStrobe code Aug 2015. PG
//    Serial.println(" in DiscoStrobe ") ;
  // First, we black out all the LEDs
  fill_solid(Strip_U, NUM_U, CRGB::Black);

  // To achive the strobe effect, we actually only draw lit pixels
  // every Nth frame (e.g. every 4th frame).  
  // sStrobePhase is a counter that runs from zero to kStrobeCycleLength-1,
  // and then resets to zero.  
  const uint8_t kStrobeCycleLength = 4; // light every Nth frame
  static uint8_t sStrobePhase = 0;
  sStrobePhase = sStrobePhase + 1;
  if( sStrobePhase >= kStrobeCycleLength ) { 
    sStrobePhase = 0; 
  }

  // We only draw lit pixels when we're in strobe phase zero; 
  // in all the other phases we leave the LEDs all black.
  if( sStrobePhase == 0 ) {

    // The dash spacing cycles from 4 to 9 and back, 8x/min (about every 7.5 sec)
    uint8_t dashperiod= beatsin8( 8/*cycles per minute*/, 4,10);
    // The width of the dashes is a fraction of the dashperiod, with a minimum of one pixel
    uint8_t dashwidth = (dashperiod / 4) + 1;
    
    // The distance that the dashes move each cycles varies 
    // between 1 pixel/cycle and half-the-dashperiod/cycle.
    // At the maximum speed, it's impossible to visually distinguish
    // whether the dashes are moving left or right, and the code takes
    // advantage of that moment to reverse the direction of the dashes.
    // So it looks like they're speeding up faster and faster to the
    // right, and then they start slowing down, but as they do it becomes
    // visible that they're no longer moving right; they've been 
    // moving left.  Easier to see than t o explain.
    //
    // The dashes zoom back and forth at a speed that 'goes well' with
    // most dance music, a little faster than 120 Beats Per Minute.  You
    // can adjust this for faster or slower 'zooming' back and forth.
    uint8_t zoomBPM = ZOOMING_BEATS_PER_MINUTE;
    int8_t  dashmotionspeed = beatsin8( (zoomBPM /2), 1,dashperiod);
    // This is where we reverse the direction under cover of high speed
    // visual aliasing.
    if( dashmotionspeed >= (dashperiod/2)) { 
      dashmotionspeed = 0 - (dashperiod - dashmotionspeed );
    }

    
    // The hueShift controls how much the hue of each dash varies from 
    // the adjacent dash.  If hueShift is zero, all the dashes are the 
    // same color. If hueShift is 128, alterating dashes will be two
    // different colors.  And if hueShift is range of 10..40, the
    // dashes will make rainbows.
    // Initially, I just had hueShift cycle from 0..130 using beatsin8.
    // It looked great with very low values, and with high values, but
    // a bit 'busy' in the middle, which I didnt like.
    //   uint8_t hueShift = beatsin8(2,0,130);
    //
    // So instead I layered in a bunch of 'cubic easings'
    // (see http://easings.net/#easeInOutCubic )
    // so that the resultant wave cycle spends a great deal of time
    // "at the bottom" (solid color dashes), and at the top ("two
    // color stripes"), and makes quick transitions between them.
    uint8_t cycle = beat8(2); // two cycles per minute
    uint8_t easedcycle = ease8InOutCubic( ease8InOutCubic( cycle));
    uint8_t wavecycle = cubicwave8( easedcycle);
    uint8_t hueShift = scale8( wavecycle,130);


    // Each frame of the animation can be repeated multiple times.
    // This slows down the apparent motion, and gives a more static
    // strobe effect.  After experimentation, I set the default to 1.
    uint8_t strobesPerPosition = 1; // try 1..4


    // Now that all the parameters for this frame are calculated,
    // we call the 'worker' function that does the next part of the work.
    discoWorker( dashperiod, dashwidth, dashmotionspeed, strobesPerPosition, hueShift);
  }  
}


// discoWorker updates the positions of the dashes, and calls the draw function
//
void discoWorker( 
    uint8_t dashperiod, uint8_t dashwidth, int8_t  dashmotionspeed,
    uint8_t stroberepeats,
    uint8_t huedelta)
 {
  static uint8_t sRepeatCounter = 0;
  static int8_t sStartPosition = 0;
  static uint8_t sStartHue = 0;

  // Always keep the hue shifting a little
  sStartHue += 1;

  // Increment the strobe repeat counter, and
  // move the dash starting position when needed.
  sRepeatCounter = sRepeatCounter + 1;
  if( sRepeatCounter>= stroberepeats) {
    sRepeatCounter = 0;
    
    sStartPosition = sStartPosition + dashmotionspeed;
    
    // These adjustments take care of making sure that the
    // starting hue is adjusted to keep the apparent color of 
    // each dash the same, even when the state position wraps around.
    if( sStartPosition >= dashperiod ) {
      while( sStartPosition >= dashperiod) { sStartPosition -= dashperiod; }
      sStartHue  -= huedelta;
    } else if( sStartPosition < 0) {
      while( sStartPosition < 0) { sStartPosition += dashperiod; }
      sStartHue  += huedelta;
    }
  }

  // draw dashes with full brightness (value), and somewhat
  // desaturated (whitened) so that the LEDs actually throw more light.
  const uint8_t kSaturation = 208;
  const uint8_t kValue = 255;

  // call the function that actually just draws the dashes now
  drawRainbowDashes( sStartPosition, NUM_U-1, 
                     dashperiod, dashwidth, 
                     sStartHue, huedelta, 
                     kSaturation, kValue);
}


// drawRainbowDashes - draw rainbow-colored 'dashes' of light along the led strip:
//   starting from 'startpos', up to and including 'lastpos'
//   with a given 'period' and 'width'
//   starting from a given hue, which changes for each successive dash by a 'huedelta'
//   at a given saturation and value.
//
//   period = 5, width = 2 would be  _ _ _ X X _ _ _ Y Y _ _ _ Z Z _ _ _ A A _ _ _ 
//                                   \-------/       \-/
//                                   period 5      width 2
//
static void drawRainbowDashes( 
  uint8_t startpos, uint16_t lastpos, uint8_t period, uint8_t width, 
  uint8_t huestart, uint8_t huedelta, uint8_t saturation, uint8_t value)
{
  uint8_t hue = huestart;
  for( uint16_t i = startpos; i <= lastpos; i += period) {
    CRGB color = CHSV( hue, saturation, value);
    
    // draw one dash
    uint16_t pos = i;
    for( uint8_t w = 0; w < width; w++) {
      leds[U_START +  pos ] = color;
      pos++;
      if( pos >= NUM_U) {
        break;
      }
    }
    
    hue += huedelta;
  }
}

// delayToSyncFrameRate - delay how many milliseconds are needed
//   to maintain a stable frame rate.
static void delayToSyncFrameRate( uint8_t framesPerSecond)
{
  static uint32_t msprev = 0;
  uint32_t mscur = millis();
  uint16_t msdelta = mscur - msprev;
  uint16_t mstargetdelta = 1000 / framesPerSecond;
  if( msdelta < mstargetdelta) {
    delay( mstargetdelta - msdelta);
  }
  msprev = mscur;
}


//////////////////////////////


void LetterColorShift(int ihue) 
{
// function gets a color from the current palette, rotates it through the letter strips

//    Serial.println( " in LetterColorShift ") ;
  int indx2 = 0 ;
  int indx3 = 0 ;
  int indx4 = 0 ;
  int indx5 = 0 ;
  int indx6 = 0 ;
  
      currentPalette = RainbowColors_p ;
  CRGB c1 = CRGB::Black ;
  CRGB c2 = CRGB::Black ;
  CRGB c3 = CRGB::Black ;
  CRGB c4 = CRGB::Black ;
  CRGB c5 = CRGB::Black ;
  CRGB c6 = CRGB::Black ;
  
      indx2 = (ihue +1) % 15 ;
      indx3 = (ihue +2) % 15 ;
      indx4 = (ihue +3) % 15 ;
      indx5 = (ihue +4) % 15 ;
      indx6 = (ihue +5) % 15 ;           
      c1 = currentPalette[ihue] ;      
      c2 = currentPalette[ indx2 ] ;
      c3 = currentPalette[ indx3 ] ;    
      c4 = currentPalette[ indx4 ] ;
      c5 = currentPalette[ indx5 ] ; 
      c6 = currentPalette[ indx6 ] ;
              
      fill_solid(Strip_B, NUM_B , c1) ;
      fill_solid(Strip_R, NUM_R , c2) ;
      fill_solid(Strip_C, NUM_C , c3) ;
      fill_solid(Strip_3, NUM_3 , c4) ;
      fill_solid(Strip_P, NUM_P , c5) ;
      fill_solid(Strip_O, NUM_O , c6) ;
   
  
}






  



 






