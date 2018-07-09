# M24LR
Arduino library for the M24LR RFID energy harvesting chip from STMicroelectronics
https://www.st.com/resource/en/datasheet/m24lr04e-r.pdf
https://www.st.com/en/evaluation-tools/m24lr-discovery.html

This is an arduino library for interfacing with the M24LR discovery board, it does not utilize the CR95HF board, though I suppose you could use it as an RF source. 

This was tested with a Nexus 5 using the M24LR reader available in the android store. It also works with the ST25 app also available by ST Microelectronics. I recommend not mucking around with the "lock sector" and password settings; you may lock yourself out of your chip. 

This is tested and is working on an Arduino Uno, which ironically, draws too much power for the M24LR to handle. Sue me. Plan on testing on ATtiny, Pro Unos, and Teensy. Those should have lower power requirements without all the bloated HW included on the UNO. 

This library does not utilize all functions available from the chip, but was written to get you up and running. 

Additional functions for future development:
-Lock sector memory (R/W)
-Set I2C password
-Set RF password
-Unlock Sector memory
-and many moooore...
