class Button {
  private:
    /*
    * Configs
    */
    
    // If the button is inverted
    bool inverted;

    // Button event handler for events
    void (*onButtonEvent)(bool toggleDown);

    // Button-pin on the board
    byte pin;



    

    // Time when the button got detected as changed
    long detectedTime;

    // Previously detected state of the button
    bool prevState, evtTriggert,activeState;

  public:

    Button(void (*onButtonEvent)(bool toggleDown),byte pin,bool isInverted = false){
      this->onButtonEvent = onButtonEvent;
      this->pin = pin;
      this->inverted = isInverted;

      // Sets the pinmode
      pinMode(pin,INPUT);
    }

    bool isPressed(){
      // Inverter is done using an != (XOR)
      return this->activeState != this->inverted;
    }

    void update(){
      // Reads in the button
      bool btnState = digitalRead(this->pin);

      // Checks if the button is equal
      if(this->prevState == btnState){
        // Checks if the event already triggert
        if(this->evtTriggert)
          return;

        // Checks if the timer has been reached
        if(millis() - this->detectedTime > 200){
          // Triggers the event
          this->evtTriggert=true;
          this->activeState=this->prevState;

          // Checks if a button event is set
          if(this->onButtonEvent != nullptr)
            this->onButtonEvent(this->isPressed());
        }
        return;
      }

      // Updates the button state (And resets the time and evt-triggert flag)
      this->prevState=btnState;
      this->detectedTime = millis();
      this->evtTriggert = false;
    }
    
};
