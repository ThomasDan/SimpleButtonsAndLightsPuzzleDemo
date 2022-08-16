/*
  A very simple lights-and-buttons puzzle, as a demonstration, for some young teenagers, of what we might make on my current education.
  All lights are turned off to begin with. The goal is to turn them all on. As it is a Very simple puzzle of this sort, it can be solved with just 2 button presses.

  Every button affects the lights in pre-determined way (programs).

  Coded for 3 LED lights and 3 buttons. Each button having 2 'programs'.

  It wouldn't be too much work to add more lights and buttons, so long as all buttons have an equal number of programs.

*/

const int numberOfLights = 3;
const int numberOfButtons = 3;
const int numberOfPrograms = 2;

class Light{
  public:
    int pinNumber; // The pinNumber of the input for the LED light
    bool state; // Keeps track of whether the LED is supposed to be ON (True) or OFF (False)

    // This the Constructor for the Light class
    Light(int _pinNumber){
      pinNumber = _pinNumber;
      state = false; // Light should be off by default
      pinMode(pinNumber, OUTPUT); // Pin initialized
      digitalWrite(pinNumber, LOW); // Pin set to Off
    }

    // This is used for the buttons' programs. Each Program has an Action for a Light to perform.
    PerformAction(int action){
      switch(action){
        case 0: // Turns the light OFF
          state = false;
          break;
        case 1: // Turns the light ON
          state = true;
          break;
        default: // Flips the light's state from ON to OFF, or OFF to ON. 2 should be the only case, but default is never a bad idea!
          state = !state;
          break;
      }
      digitalWrite(pinNumber, state ? HIGH : LOW);
    }
};

// Perhaps a somewhat misleading name, this is used by the Button class to determien what the buttons do when pressed.
class Program{
  public: 
    int lightIndex; // The index of the Light in the lights array.
    // Action (0: SET TO OFF, 1: SET TO ON, 2: FLIP)
    int action; // What action that the Light must perform.
    
    Program(){
      // This empty constructor is Necessary, as the buttons must be initialized with empty programs.
    }
    Program(int _light, int _action){
      lightIndex = _light;
      action = _action;
    }
};

class Button{
  public: 
    int pinNumber;
    Program programs[numberOfPrograms] = { Program(), Program() }; // I wanted to make this array of dynamic sizing, but alas, such is bad practice with arduino.
    
    Button(){
      // This empty constructor is necessary, as the array of buttons requires that it is initialized with empty buttons.
    }
    Button(int _pinNumber, Program prog1, Program prog2){
      pinNumber = _pinNumber;
      pinMode(pinNumber, INPUT);
      programs[0] = prog1;
      programs[1] = prog2;
    }
    
};

const Light lights[numberOfLights] = {
  Light(7), Light(6), Light(5)
};

// Initialization with empty Button()
const Button buttons[numberOfButtons] = { Button(), Button(), Button() };

void setup() {
  Serial.begin(9600);

  // Button 1 programming: (L1 ON, L2 FLIP)
  Program b1L1 = Program(0, 1); // Light 1 (on index 0 in the lights array), turn ON (program.action = 1)
  Program b1L2 = Program(1, 2); // Light 2 (on index 1 in the lights array), Flip state (program.action = 2)
  buttons[0] = Button(4, b1L1, b1L2); // Button(pinNumber, Program, Program)

  // Button 2 programming: (Flip L1 & L3)
  Program b2L1 = Program(0, 2); // Light 1 (index 0), Flip state (action 2)
  Program b2L3 = Program(2, 2); // Light 3 (index 2), Flip state (action 2)
  buttons[1] = Button(3, b2L1, b2L3);

  // Button 3 proramming: (L2 OFF, Flip L3)
  Program b3L2 = Program(1, 0); // Light 2 (index 1), turn off (action 0)
  Program b3L3 = Program(2, 2); // Light 3 (index 2), Flip state (action 2)
  buttons[2] = Button(2, b3L2, b3L3);
}

void loop() {
  bool gameOver = false;
  
  while(!gameOver){
    // Here we check if any of the buttons are being pressed
    for(int i = 0; i < numberOfButtons; i++){
      if(digitalRead(buttons[i].pinNumber) == 1){
        runButtonPrograms(buttons[i]);
        gameOver = checkVictory();
        delay(350);
        break;
      }
    }
    delay(125);
  }
  // Victory Lightshow!
  victoryShow();
  // Set all the lights to off, just prior to restarting the game
  lightsOut();
}

// Applies all of a button's programs to the lights
void runButtonPrograms(Button btn){
  for(int i = 0; i < numberOfPrograms; i++){
    Program prog = btn.programs[i];
    // NULL check, to ensure this Program is actually used for something, and is not merely leftover from the empty initialization.
    if(prog.action != NULL && prog.lightIndex != NULL){
      // We call on the Light of this Program's lightIndex, to perform this Program's action
      lights[prog.lightIndex].PerformAction(prog.action);
    }
  }
}

// Returns true if all lights are on
bool checkVictory(){
  for(int i = 0; i < numberOfLights; i++){
    if(!lights[i].state){
      // If a single Light is state false (Off), the puzzle has not been solved, and there is no reason to run through the rest of the lights.
      return false;
    }
  }
  return true;
}

// Turns off all the lights
void lightsOut(){
  for(int i = 0; i < numberOfLights; i++){
    lights[i].PerformAction(0); // Turn the Light off
  }
}

// A little light show to celebrate user having solved the puzzle
void victoryShow(){
  delay(1125);
  lightsOut();
  delay(300);
  // From left-to-right, turning the lights on.
  for(int i = 0; i < numberOfLights; i++){
    lights[i].PerformAction(1);
    delay(250);
  }
  delay(100);
  lightsOut();
  // From right-to-left, turning the lights on
  for(int i = numberOfLights-1; i > -1; i--){
    lights[i].PerformAction(1);
    delay(250);
  }
  delay(100);
  // Rapidly simultaneously turning even and uneven lights on and off, 10 times over about 2 seconds.
  for(int i = 0; i < 10; i++){
    lightsOut();
    // Lights on even index are turned on
    for(int j = 0; j < numberOfLights; j++){
      if(j % 2 == 0){
        lights[j].PerformAction(1);
      }
    }
    delay(100);
    lightsOut();
    // Lights on uneven indexes are turned off
    for(int k = 0; k < numberOfLights; k++){
      if(k % 2 != 0){
        lights[k].PerformAction(1);
      }
    }
    delay(100);
  }
  lightsOut();
  // All lights are turned on one last time, to better indicate that the puzzle has been reset
  for(int i = 0; i < numberOfLights; i++){
    lights[i].PerformAction(1);
  }
  delay(1000);
}
