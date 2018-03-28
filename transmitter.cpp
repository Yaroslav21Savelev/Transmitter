import processing.serial.*;
import cc.arduino.*;
import controlP5.*;
int X = 50;
int connect, val, r = 5;
Arduino arduino;
ControlP5 cp5;
PImage photo;
String[] textfieldNames = {"Enter COM port:"};
Textfield myTextfield;
color on = color(234, 230, 230);
color mouse_on = color(190, 190, 190);
color mouse_yellow = color(240, 240, 0);
color black = color(0, 0, 0);
color white = color(255, 255, 255);
color red = color(255, 51, 51);
color blue = color(51, 51, 255);
color purple = color(153, 0, 153);
color green = color(51, 255, 51);
color yellow = color(255, 255, 51);
color brown = color(153, 76, 0);

void setup() 
{
  size(500, 450);
  surface.setResizable(true);
  photo = loadImage("background.png");
  PFont font = createFont("arial",20);
  cp5 = new ControlP5(this);
  int y=400;
  int spacing = 60;
  for(String name: textfieldNames){
    cp5.addTextfield("Enter COM")
       .setPosition(10, 210)
       .setSize(25,25)
       .setFont(font)
       .setFocus(true)
       .setColor(on)
       ;
     y += spacing;
  }

  textFont(font);
background(black);
}

void draw() 
{
  stroke(on);
  if (val==1)
  {
    background(white);
    stroke(on);
    arduino.analogWrite(4, 2);
    fill(on);
    for (int i = 0; i <= 8; i++)
    {
      rect(0 + X, 10 + i * 50, 255, 25);
    }
    for (int i = 0; i <= 8; i++)
    {
      fill(brown);
      switch (i) 
      {
        case 0:fill(red);break;
        case 1:fill(purple);break;
        case 2:fill(blue);break;
        case 3:fill(green);break;
        case 4:fill(yellow);break;
      }
      rect(0 + X, 10 + i * 50, arduino.analogRead(i), 25);
    }
    fill(black);
    textSize(18);
    for (int i = 0; i <= 8; i++)
    {
      text(arduino.analogRead(i), 120 + X, 29 + 50 * i);
    }
  
    fill(on);
    for (int i = 0; i < 4; i++)
    {
      rect(-40 + X, 10 + i * 50, 25, 25, r);
    }
    for (int i = 0; i < 4; i++)
    {
      rect(270 + X, 10 + i * 50, 25, 25, r);
    }
    fill(yellow);
    rect(370 + X, 10, 70, 25, r);
    rect(370 + X, 60, 70, 25, r);
    for (int i = 0; i < 4; i++)
    {
      if (mouseX >= -40 + X && mouseX <= -15 + X && mouseY >= 10 + i * 50 && mouseY <= 35 + i * 50)
      {
        fill(mouse_on);
        rect(-40 + X, 10 + i * 50, 25, 25, r);
      }
    }
    for (int i = 0; i < 4; i++)
    {
      if (mouseX >= 270 + X && mouseX <= 295 + X && mouseY >= 10 + i * 50 && mouseY <= 35 + i * 50)
      {
        fill(mouse_on);
        rect(270 + X, 10 + i * 50, 25, 25, r);
      }
    }

  
    if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 10 && mouseY <= 35)
    {
      fill(mouse_yellow);
      rect(370 + X, 10, 70, 25, r);
    }
    if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 60 && mouseY <= 85)
    {
      fill(mouse_yellow);
      rect(370 + X, 60, 70, 25, r);
    }
    fill(black);
    textSize(25);
    text("-", -32 + X, 31);
    text("-", -32 + X, 81);
    text("-", -32 + X, 131);
    text("-", -32 + X, 181);
    text("+", 275 + X, 32);
    text("+", 275 + X, 82);
    text("+", 275 + X, 132);
    text("+", 275 + X, 182);
    textSize(20);
    text("SAVE", 378 + X, 30);
    text("RESET", 373 + X, 80);
    
  }
  else
  {
    image(photo, 0, 0);
  }
}
void mousePressed()
{
  fill(black);
  for (int i = 0; i < 4; i++)
  {
    if (mouseX >= -40 + X && mouseX <= -15 + X && mouseY >= 10 + i * 50 && mouseY <= 35 + i * 50)
    {
      rect(-40 + X, 10 + i * 50, 25, 25, r);
      arduino.analogWrite(i, 0);
    }
  }
  for (int i = 0; i < 4; i++)
  {
    if (mouseX >= 270 + X && mouseX <= 295 + X && mouseY >= 10 + i * 50 && mouseY <= 35 + i * 50)
    {
      rect(270 + X, 10 + i * 50, 25, 25, r);
      arduino.analogWrite(i, 2);
    }
  }
  if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 10 && mouseY <= 35)
  {
    fill(red);
    rect(370 + X, 10, 70, 25, r);
    arduino.analogWrite(4, 0);
  }
  if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 60 && mouseY <= 85)
  {
    fill(red);
    rect(370 + X, 60, 70, 25, r);
    arduino.analogWrite(4, 1);
  }
}
void controlEvent(ControlEvent theEvent) {
  if(theEvent.isAssignableFrom(Textfield.class)) {
    println(theEvent.getStringValue());
    arduino = new Arduino(this,"COM"+theEvent.getStringValue(), 115200);
    val=1;
  }
}