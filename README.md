# UTFT_7seg
A 7 segment display that uses geometry, for super fast updates even at large sizes.

## Example

    // Setup LCD
    UTFT myGLCD(SSD1963_800480,38,39,40,41);  
    
    // Setup geometry
    UTFT_Geometry geo(&myGLCD);
    
    // Setup7 seg
    UTFT_7seg my7seg(&myGLCD,&geo);


    void setup() {
        // Initialise the LCD
        myGLCD.InitLCD();
        myGLCD.clrScr();

        // Initialise the 7seg
        my7seg.begin(10,10,3,8,true);  // x co-ord, y co-ord, num digits, size, fist digit is a 1

        myGLCD.setColor(0,255,255);
    }

    void loop() {
        my7seg.draw( floor(millis()) );
    }

## Concept

This module uses UTFT Geometry to draw a 7 seg display. The draw method is very efficient, it only draws elements of the 7seg that have changed since the last draw.
This means that it can be called as often as you like and it will only take up cycles as and when it needs to.

You can use the UTFT_7seg.value() function to update the value without redrawing the 7 seg. This is useful if your value changes more frequently than you need ot update the display.

You can enter any size value above 0. A size of 1 results in a digit that is 16 pixels wide. Size 10 is 160 pixels wide.

For aesthetic reasons, the display will also render the un-illuminated segments of the display. The colour of the display (and the unilluminated segments) is set
by the last UTFT.setColor() command.

You can display any positive integer upto the number of digits ont he display. If you try to display a number that is too large or the value -1 then the 
display will be blank (not 0). Any value between 0 and maxValue will be displayed on the rightmost digits of the display without 
any leading 0s.

If you set the "fist digit is a one" flag to true, then the first digit can only be a 1 or blank. This is commonly used on speedometer
displays, where the max value is 199. 
