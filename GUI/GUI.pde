import controlP5.*;
import processing.serial.*;

Serial port;

ControlP5 cp5;
PFont font;

Textarea arduinoComms;

void setup() {
  size(400, 400);
  
  printArray(Serial.list()); 
  
  port = new Serial(this, "COM4", 9600);
  
  cp5 = new ControlP5(this);
  font = createFont("calibri light", 20);
  
  arduinoComms = cp5.addTextarea("txt")
                  .setPosition(220, 250)
                  .setSize(600, 600);
  
  cp5.addButton("Forward")
     .setPosition(100, 50)
     .setSize(100, 80)
     .setFont(font);
     
  cp5.addButton("Backward")
     .setPosition(100, 250)
     .setSize(100, 80)
     .setFont(font);
     
  cp5.addButton("Left")
     .setPosition(30, 150)
     .setSize(100, 80)
     .setFont(font);
     
  cp5.addButton("Right")
     .setPosition(170, 150)
     .setSize(100, 80)
     .setFont(font);
     
  cp5.addButton("Stop")
     .setPosition(100, 350)
     .setSize(100, 30)
     .setFont(font);
     
  cp5.addButton("Override")
      .setPosition(300, 50);
     
  cp5.addButton("Room")
      .setPosition(300, 75);
  
  cp5.addButton("Corridor")
      .setPosition(300, 100);
  
  cp5.addButton("EndOfMaze")
      .setPosition(300, 125);
  
  cp5.addButton("LeftOfCorridor")
      .setPosition(300, 150);
  
  cp5.addButton("RightOfCorridor")
      .setPosition(300, 175);
  
  cp5.addButton("AutomaticMode")
      .setPosition(300, 200);
}

void draw() {
  background(150, 0, 0);
  fill(255, 255, 255);
  textFont(font);
  text("The Zumanator 9000", 65, 30);
  String val = port.readString();
  if (val != null) {
    arduinoComms.setText(val);
  }
}

void Forward()
{
  port.write('w');
}

void Backward()
{
  port.write('x'); 
}

void Left()
{
  port.write('a');
}

void Right()
{
  port.write('d');
}

void Stop()
{
  port.write('s');
}

void Override()
{
  port.write('v');
}

void EndOfMaze()
{
  port.write('e');
}

void Room() {
  port.write('r');
}

void Corridor() {
  port.write('c');
}

void LeftOfCorridor() {
  port.write('l');
}

void RightOfCorridor() {
  port.write('r');
}

void AutomaticMode() {
  port.write('b');
}