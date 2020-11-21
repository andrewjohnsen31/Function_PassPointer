

void Phase_Cut_Setup(){
  pinMode(Triac_Driver_Pin, OUTPUT);
  digitalWrite(Triac_Driver_Pin, LOW);

  pinMode (Zero_Cross_Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Zero_Cross_Pin), zero_cross_detect, RISING);    // Attach an Interupt to Pin D2 (interupt 0) for Zero Cross Detection

  // Design Targets: under 128 interupts per 120 hz period => must be faster than 15,360 Hz
  // Main clock set for 32Mhz
  // Prescale by 32 => 2Mhz
  // Period counter of 170dec (AAH) => 15,151hz 

  //======== double check range 0 to 110 count ==========

  /* note commented section is used by servo, so we are force to use this combined GLK
    // Set up the generic clock (GCLK4) used to clock timers
    REG_GCLK_GENDIV = GCLK_GENDIV_DIV(3) |          // Divide the 48MHz clock source by divisor 3: 48MHz/3=16MHz
                    GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                     GCLK_GENCTRL_GENEN |         // Enable GCLK4
                     GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                     GCLK_GENCTRL_ID(4);          // Select GCLK4
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // Feed GCLK4 to TC4 and TC5
    REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
                     GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                     GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
  */
  REG_TC5_CTRLA |= TC_CTRLA_MODE_COUNT8;           // Set the counter to 8-bit mode
  while (TC5->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  REG_TC5_COUNT8_CC0 = 0x55;                      // Set the TC4 CC0 register to some arbitary value
  while (TC5->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC5_COUNT8_CC1 = 0xAA;                      // Set the TC4 CC1 register to some arbitary value
  while (TC5->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  REG_TC5_COUNT8_PER = 0xAA;                      // Set the PER (period) register to its maximum value
  while (TC5->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

  //NVIC_DisableIRQ(TC5_IRQn);        // not sure if needed
  //NVIC_ClearPendingIRQ(TC5_IRQn);   // not sure if needed
  NVIC_SetPriority(TC5_IRQn, 1);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC5 to 0 (highest)
  NVIC_EnableIRQ(TC5_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  REG_TC5_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        // Clear the interrupt flags
  REG_TC5_INTENSET = TC_INTENSET_MC1 | TC_INTENSET_MC0 | TC_INTENSET_OVF;     // Enable TC4 interrupts
  // REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;     // Disable TC4 interrupts

  REG_TC5_CTRLA |= TC_CTRLA_PRESCALER_DIV16 |     // Set prescaler to 32 => 1Mhz counting rate
                   TC_CTRLA_ENABLE;               // Enable TC4
  while (TC5->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

}

void zero_cross_detect () {
  zero_cross = true;               // set the boolean to true to tell our Fan_Speedming function that a zero cross has occured
  Fan_Speed_i = 0;                           // intialize number of steps to 0
  digitalWrite(Triac_Driver_Pin, LOW);       // turn off TRIAC (and AC)
}

void TC5_Handler()                              
{
  // Check for overflow (OVF) interrupt
  if (TC5->COUNT8.INTFLAG.bit.OVF && TC5->COUNT8.INTENSET.bit.OVF)
  {

    if (zero_cross == true)
    {
      if (Fan_Speed_i >= Fan_Speed)
      {
        digitalWrite(Triac_Driver_Pin, HIGH); // turn on triac
        Fan_Speed_i = 0;                            // reset time step counter
        zero_cross = false; //reset zero cross detection
      }
      else {
        Fan_Speed_i++; // increment time step counter
      }
    }

    REG_TC5_INTFLAG = TC_INTFLAG_OVF;         // Clear the OVF interrupt flag
  }

  // Check for match counter 0 (MC0) interrupt
  if (TC5->COUNT8.INTFLAG.bit.MC0 && TC5->COUNT8.INTENSET.bit.MC0)
  {


    REG_TC5_INTFLAG = TC_INTFLAG_MC0;         // Clear the MC0 interrupt flag
  }

  // Check for match counter 1 (MC1) interrupt
  if (TC5->COUNT8.INTFLAG.bit.MC1 && TC5->COUNT8.INTENSET.bit.MC1)
  {
    // Put your counter compare 1 (CC1) code here:
    // ...

    REG_TC5_INTFLAG = TC_INTFLAG_MC1;        // Clear the MC1 interrupt flag
  }
}
