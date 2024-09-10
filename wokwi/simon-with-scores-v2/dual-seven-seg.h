#ifndef __DUALSEVENSEG__
#define __DUALSEVENSEG__

#include <Arduino.h>

class DualSevenSeg final {
  public:
    // constructor

    //! Initialize a DualSevenSeg object, setting the pinout and dance flag
    //!
    //! @param dataPin - the number of the digital pin connected to the data pin of the first shift register
    //! @param latchPin - the number of the digital pin connected to the latch pin of the shift registers
    //! @param clockPin - the number of the digital pin connected to the clock pin of the shift registers
    //! @param dance - determines if a dance animation should be played before displaying a value. Set to false by default.
    DualSevenSeg(int dataPin, int latchPin, int clockPin, bool dance);

    // methods

    //! Displays a number between 0 and 99 in decimal on the dual seven segment display
    //! 
    //! @param number - the number to be displayed. Values equal to or higher than 100 won't be displayed.
    void displayNumber(byte number);

    //! Displays a number between 0 and FF (= 255) in hexadecimal on the dual seven segment display
    //! 
    //! @param number - the number to be displayed.
    void displayNumberHex(byte number);
  
  private:
    // methods
    void sendDigits(byte tens, byte units);
    void dance();
};

#endif // __SEVENSEG__
