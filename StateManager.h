

class StateManager{

  private:
    long timerState;
    int state;
    bool updChange;
  
  public:
  
    StateManager(){
      this->reset();
    }
  
  
    /*
     * Checks if the current state is set and updates to the given next if the amount of millis has been reached.
     * If no nextState state has been defined, the current one only increments.
     *
     * To reset back, just put 0 for the nextState-variable
     *
     * Returns true ONCE! if the current state is set and otherwise false.
     */
    bool isState(int checkState,int deltaMillis, int nextState = -1){
      // Checks at which checkpoint (State) the manager currently is
      if(this->state != checkState)
        return false;
  
      // Checks if the state just changed
      if(this->updChange){
        // Inverses the change
        this->updChange=false;
        return true;
      }
  
      // Checks if the state changed
      if(millis() - this->timerState > deltaMillis){
        // Switches to the next state
      this->state=nextState == -1 ? (checkState+1) : nextState;
      // Updates the just-changed flag
      this->updChange=true;
      // Resets the timer
      this->timerState = millis();
    }

    return false;
  }

  /*
   * Resets the state-manager
   */
  void reset(){
    this->timerState = millis();
    this->state=0;
    this->updChange=true;
  }



};






/*

  After 1 do x

  After 2 do y

  After 3 do z


*/
