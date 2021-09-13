const uint16_t DEBOUNCE_INTERVAL = 10;  // Timer interval between successive SW1 reads in milliseconds
const uint8_t GREEN_LED = 13;         // GREEN_LED is connected to D13
const uint8_t YELLOW_LED = 12;        // YELLOW_LED is connected to D12
const uint8_t RED_LED = 11;           // RED_LED is connected to D11
const uint8_t SW1 = 10;               // SW1 is connected to D10
const uint8_t SW2 = 9;                // SW2 is connected to D9
const uint8_t POT = 0;                // POT wiper is connected to A0

enum states {                         // Define enumerated type for state machine states
  NONE, 
  ALL_ON, 
  ALL_FLASHING, 
  ALL_OFF,
  ALTERNATE_FLASHING,
  WAVING_LIGHT, 
};

states prior_state, state;            // Global variables to store the prior and current states
uint32_t flashing_time;                 // Global variable to store the time that YELLOW_LED last changed state
uint16_t flashing_count;                // Global variable to store the number of times YELLOW_LED has changed 
                                      //   states
uint32_t debounce_time;               // Global variable to store the time since SW1 or SW2 was read
uint16_t BLINK_INTERVAL;           // Global variable to store the blink interval in milliseconds

bool SW1_went_back_low;               // Global variable to indicate that SW1 returned low after going high
bool SW2_went_back_low;               // Global variable to indicate that SW2 returned low after going high
bool SW1_high;                        // Global variable to store whether SW1 is high
bool SW2_high;                        // Global variable to store whether SW is high


void all_on() {
  uint32_t t;                           // Local variable to store the current value of the millis timer

  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;              
    digitalWrite(GREEN_LED, HIGH);    // Turn all of the LEDs on
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
  }

  // Perform state tasks (there are no state tasks for this mode because all of the LEDs just stay on)
  // Check for state transitions
  t = millis();                     // Get the current value of the millis timer
  if (t >= debounce_time + DEBOUNCE_INTERVAL) { // Only sample the tactile switch at a DEBOUNCE_INTERVAL
    SW1_high = digitalRead(SW1) == HIGH;        //   millisecond interval
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {        // If SW1 has been pressed, move to the ALL_FLASHING state
      state = ALL_FLASHING;
      SW1_went_back_low = false;
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = WAVING_LIGHT;                   // If SW2 has been pressed, move to the WAVING_LIGHT state
      SW2_went_back_low = false;
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;                          // Set debounce_time to the current value of the millis timer
  }
  
  if (state != prior_state) {         // If we are leaving the state, do clean up stuff 
    digitalWrite(GREEN_LED, LOW);     //    (in this case turn LEDs all off)
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}

void all_flashing() {
  uint32_t t;                           // Local variable to store the current value of the millis timer

  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);    // Turn all of the LEDs on
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
    flashing_time = millis();        // Set flashing_time to the current value of the millis timer
  }

  // Perform state tasks
  t = millis();
  if (t >= flashing_time + BLINK_INTERVAL) {              // If BLINK_INTERVAL milliseconds have elapsed
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));     //   since blink_time, toggle the state of all 
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));   //   of the LEDs and set flashing_time to the 
    digitalWrite(RED_LED, !digitalRead(RED_LED));         //   current value of the millis timer.
    flashing_time = t;
  }

  // Check for state transitions
  if (t >= debounce_time + DEBOUNCE_INTERVAL) {          // Only sample the tactile switch at a 
    SW1_high = digitalRead(SW1) == HIGH;                 //   DEBOUNCE_INTERVAL millisecond interval
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {
      state = ALL_OFF;                                  // If SW1 has been pressed, move to ALL_OFF state
      SW1_went_back_low = false;
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = ALL_ON;                                   // If SW2 has been pressed, move to ALL_ON state
      SW2_went_back_low = false;
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;                                  // Set debounce_time to the current value of the 
  }                                                     //    millis timer
  

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff 
    digitalWrite(GREEN_LED, LOW);     //    (in this case, turn all of the LEDs off)
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}

void all_off() {
  uint32_t t;                           // Local variable to store the current value of the millis timer

  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, LOW);      // Turn all of the LEDs on
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }

  // Perform state tasks (in this case there are no state tasks because the LEDs stay off)


  // Check for state transitions
  t = millis();
  if (t >= debounce_time + DEBOUNCE_INTERVAL) {      // Only sample the tactile switch at a 
    SW1_high = digitalRead(SW1) == HIGH;             //     DEBOUNCE_INTERVAL millisecond interval
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {
      state = ALTERNATE_FLASHING;                     // If SW1 has been pressed, move to 
      SW1_went_back_low = false;                      //    ALTERNATE_FLASHING state
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = ALL_FLASHING;                           // If SW2 has been pressed, move to
      SW2_went_back_low = false;                      //     ALL_FLASHING state
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;                              // Set debounce_time to the current value 
  }                                                 // of the millis timer

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff 
    digitalWrite(GREEN_LED, LOW);     // (in this case, turn all LEDs off)
    digitalWrite(YELLOW_LED, LOW);  
    digitalWrite(RED_LED, LOW);
  }
}

void alternate_flashing() {
  uint32_t t;                           // Local variable to store the current value of the
                                        //     millis timer
  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;
    digitalWrite(GREEN_LED, HIGH);   //Turn the green and red LEDs on, and turn the yellow LED off
    digitalWrite(YELLOW_LED, LOW);   // this will make it so that when we toggle the states of all
    digitalWrite(RED_LED, HIGH);     // of the LEDs, the LEDs will alternate in their flashing
    flashing_time = millis();
  }

  // Perform state tasks
  t = millis();                               // Get the current value of the millis timer
  if (t >= flashing_time + BLINK_INTERVAL) {             // If BLINK_INTERVAL milliseconds have
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));    //  elapsed since blink_time, toggle the 
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));  //  state of all the LEDs and set 
    digitalWrite(RED_LED, !digitalRead(RED_LED));        //  flashing_time to the current value 
    flashing_time = t;                                   //  of the millis timer
  }

  // Check for state transitions
  if (t >= debounce_time + DEBOUNCE_INTERVAL) {   // Only sample the tactile switch at a 
    SW1_high = digitalRead(SW1) == HIGH;          //    DEBOUNCE_INTERVAL millisecond interval
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {
      state = WAVING_LIGHT;                      // If SW1 has been pressed, move to 
      SW1_went_back_low = false;                 //   WAVING_LIGHT state
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = ALL_OFF;                              // If SW1 has been pressed, move to 
      SW2_went_back_low = false;                    // ALL_OFF state
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;                             // Set debounce_time to the current value 
  }                                                //   of the millis timer
  

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff 
    digitalWrite(GREEN_LED, LOW);     //    (in this case, turn all LEDs off)
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}

void waving_light() {
  uint32_t t;                           // Local variable to store the current value of the millis timer

  if (state != prior_state) {         // If we are entering the state, do initialization stuff
    prior_state = state;              //  in this case we are turning the green LED on and turning
    digitalWrite(GREEN_LED, HIGH);    // the yellow and red LEDs off. In addition we are setting
    digitalWrite(YELLOW_LED, LOW);    // flashing_time to the current value of the millis timer
    digitalWrite(RED_LED, LOW);       // and initializing the flashing count to 1
    flashing_time = millis();
    flashing_count = 1;               // The flashing count will be used to determine which LEDs to toggle.
  }                                   //    In doing this, we can have different phases of LED toggling and 
                                      //    allow the lights to "wave" from green to red.
  // Perform state tasks
  t = millis();                                                       // Get the current value of the millis 
  if (t >= flashing_time + BLINK_INTERVAL && flashing_count%3 == 1) { //    timer. If BLINK_INTERVAL milliseconds 
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));                 //    have elapsed since blink_time, and 
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));               //    we're in the first of the three phases 
    flashing_time = t;                                                //    of LED toggling, toggle the state of 
    flashing_count++;                                                 //    the green and yellow LEDs, set and
  }                                                                   //    increase the flashing count by one
  if (t >= flashing_time + BLINK_INTERVAL && flashing_count%3 == 2) {   
    digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));                //  If we're in the 2nd of the 3 phases of 
    digitalWrite(RED_LED, !digitalRead(RED_LED));                      //     LED toggling, toggle the state of the
    flashing_time = t;                                                 //     yellow and red LEDs,  and increase the 
    flashing_count++;                                                  //     flashing count by one
  }
  if (t >= flashing_time + BLINK_INTERVAL && flashing_count%3 == 0) { // If we're in the third of the three phases of 
    digitalWrite(RED_LED, !digitalRead(RED_LED));                     //    LED toggling,  toggle the state of the red 
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));                 //    and green LEDs, set flashing_time to the 
    flashing_time = t;                                                //    current value of the millis timer
    flashing_count++;                                                 //    and increase the flashing count by one
  }


  if (t >= debounce_time + DEBOUNCE_INTERVAL) {     // Only sample the tactile switch at a DEBOUNCE_INTERVAL 
    SW1_high = digitalRead(SW1) == HIGH;            //    millisecond interval
    SW2_high = digitalRead(SW2) == HIGH;
    if (SW1_went_back_low && SW1_high) {
      state = ALL_ON;                               // If SW1 has been pressed, move to ALL_ON state
      SW1_went_back_low = false;
    } else if (!SW1_went_back_low && !SW1_high) {
      SW1_went_back_low = true;
    } else if (SW2_went_back_low && SW2_high) {
      state = ALTERNATE_FLASHING;                   // If SW2 has been pressed, move to ALTERNATE_FLASHING state
      SW2_went_back_low = false;
    } else if (!SW2_went_back_low && !SW2_high) {
      SW2_went_back_low = true;
    }
    debounce_time = t;                              // Set debounce_time to the current value of the millis timer
  }

  if (state != prior_state) {         // If we are leaving the state, do clean up stuff (turn all LEDs off)
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}
void setup() {
  pinMode(GREEN_LED, OUTPUT);         // Configure GREEN_LED pin as a digital output
  pinMode(YELLOW_LED, OUTPUT);        // Configure YELLOW_LED pin as a digital output
  pinMode(RED_LED, OUTPUT);           // Configure RED_LED pin as a digital output
  pinMode(SW1, INPUT);                // Configure SW1 pin as a digital input
  pinMode(SW2, INPUT);                // Configure SW2 pin as a digital input
  
  digitalWrite(GREEN_LED, LOW);       // Set GREEN_LED low initially
  digitalWrite(YELLOW_LED, LOW);      // Set YELLOW_LED low initially
  digitalWrite(RED_LED, LOW);         // Set RED_LED low initially

  debounce_time = millis();
  BLINK_INTERVAL = analogRead(POT);   // Convert potentiometer wiper voltage to initialize blink_interval
                                      //   this way, by turning the potentiometer, we can change the rate
  SW1_went_back_low = true;           //   at which the lights will flash at whenever the current state involves
  SW2_went_back_low = true;           //   flashing LEDs
  
  prior_state = NONE;
  state = ALL_ON;
}

void loop() {
  switch (state) {
    case ALL_ON:
      all_on();
      break;
    case ALL_FLASHING:
      all_flashing();
      break;
    case ALL_OFF:
      all_off();
      break;
    case ALTERNATE_FLASHING:
      alternate_flashing();
      break;
    case WAVING_LIGHT:
      waving_light();
      break;
  }
}
