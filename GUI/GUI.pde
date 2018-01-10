import controlP5.*;
import processing.serial.*;

Serial port;

ControlP5 cp5;
PFont font;

void setup() {
  size(300, 400);
  
  printArray(Serial.list()); 
  
  port = new Serial(this, "COM3", 9600);
  
  cp5 = new ControlP5(this);
  font = createFont("calibri light", 20);
  
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
}

void draw() {
  background(150, 0, 0);
  fill(255, 255, 255);
  textFont(font);
  text("The Zumanator 9000", 65, 30);
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