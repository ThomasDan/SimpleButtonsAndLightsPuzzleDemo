/*
  A very simple lights-and-buttons puzzle, as a demonstration for some young teenagers of what we might make on my current education.
  All lights are turned off to begin with. The goal is to turn them all on. It can be done with just 2 button presses.

  Every button affects the lights in pre-determined way.

  Coded for 3 LED lights and 3 buttons. Each button having 2 'programs'.

  It wouldn't be too much work to add more lights and buttons, so long as all buttons have an equal number of programs.

*/

class Light{
  public:
    int pinNumber;
    bool state;
    // This the Constructor for the Light class
    Light(int _pinNumber){
      pinNumber = _pinNumber;
      state = false;
      pinMode(pinNumber, OUTPUT);
      digitalWrite(pinNumber, LOW);
    }

    PerformAction(int action){
      // If turn off, and not already turned off
      if(action == 0 && state){
        state = false;
      } // -||-, but on
      else if(action == 1 && !state){
        state = true;
      } // Flip the Light's state
      else if(action == 2){
        state = !state;
      }
      digitalWrite(pinNumber, state ? HIGH : LOW);
    }
};

class Program{
  public: 
    int lightIndex;
    // Action (0: SET TO OFF, 1: SET TO ON, 2: FLIP)
    int action;
    
    Program(){

    }
    Program(int _light, int _action){
      lightIndex = _light;
      action = _action;
    }
};

class Button{
  public: 
    int pinNumber;
    Program programs[2] = { Program(), Program() }; // I wanted to make this array of dynamic sizing, but alas, such is bad practice with arduino.
    
    Button(){

    }
    Button(int _pinNumber, Program prog1, Program prog2){
      pinNumber = _pinNumber;
      pinMode(pinNumber, INPUT);
      programs[0] = prog1;
      programs[1] = prog2;
    }
    
};

const Light lights[3] = {
  Light(7), Light(6), Light(5)
};

// Initialization with empty Button()
const Button buttons[3] = { Button(), Button(), Button() };

void setup() {
  Serial.begin(9600);

  // Button 1 programming: (L1 ON, Flip L2)
  Program b1L1 = Program(0, 1);
  Program b1L2 = Program(1, 2);
  buttons[0] = Button(4, b1L1, b1L2);

  // Button 2 programming: (Flip L1 & L3)
  Program b2L1 = Program(0, 2);
  Program b2L3 = Program(2, 2);
  buttons[1] = Button(3, b2L1, b2L3);

  // Button 3 proramming: (L2 OFF, Flip L3)
  Program b3L2 = Program(1, 0);
  Program b3L3 = Program(2, 2);
  buttons[2] = Button(2, b3L2, b3L3);
}

void loop() {
  bool gameOver = false;
  
  
  while(!gameOver){
    // It is important to update the bX variables like so
    int b1 = digitalRead(buttons[0].pinNumber);
    int b2 = digitalRead(buttons[1].pinNumber);
    int b3 = digitalRead(buttons[2].pinNumber);
    if(b1 == 1){
      gameOver = buttonClicked(buttons[0]);
    }
    else if(b2 == 1){
      gameOver = buttonClicked(buttons[1]);
    }
    else if(b3 == 1){
      gameOver = buttonClicked(buttons[2]);
    }

    delay(125);
  }

  delay(1500);
  // Victory Lightshow!
  victoryShow();
  delay(1500);
  lightsOut();}

bool buttonClicked(Button btn){
  Serial.println((String)"Button pin: " + btn.pinNumber);
  runButtonPrograms(btn);
  delay(500);
  return checkVictory();
}

void runButtonPrograms(Button btn){
  for(int i = 0; i < 2; i++){
    Program prog = btn.programs[i];
    // We call on the Light of this Program's lightIndex, to perform this Program's action
    lights[prog.lightIndex].PerformAction(prog.action);
  }
}

bool checkVictory(){
  return lights[0].state && lights[1].state && lights[2].state;
}

void lightsOut(){or(int i = 0; i < 3; i++){
    lights[i].PerformAction(0); // Turn the Light off
  }
}

void victoryShow(){
  lightsOut();
  delay(500);
  for(int i = 0; i < 3; i++){
    lights[i].PerformAction(1);
    delay(250);
  }
  delay(100);
  lightsOut();
  for(int i = 2; i > -1; i--){
    lights[i].PerformAction(1);
    delay(250);
  }
  delay(100);
  for(int i = 0; i < 10; i++){
    lightsOut();
    lights[0].PerformAction(1);
    lights[2].PerformAction(1);
    delay(100);
    lightsOut();
    lights[1].PerformAction(1);
    delay(100);
  }
  lightsOut();
  lights[0].PerformAction(1);
  lights[1].PerformAction(1);
  lights[2].PerformAction(1);
  delay(350);
}
