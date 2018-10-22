import processing.serial.*;
import cc.arduino.*;
int X = 50;
int connect = -1, r = 5;
boolean val, mPressed;
Arduino arduino;
PFont font;
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
  surface.setResizable(false);
  font = createFont("arial", 20);
  textFont(font);
}

void draw() 
{
  stroke(on);
  background(white);
  if (val)
  {
    
    stroke(on);
    arduino.analogWrite(4, 2);
    textSize(18);
    for (int i = 0; i <= 8; i++)
    {
      fill(on);
      rect(0 + X, 10 + i * 50, 255, 25);
      fill(brown);
      switch (i) 
      {
      case 0:
        fill(red);
        break;
      case 1:
        fill(purple);
        break;
      case 2:
        fill(blue);
        break;
      case 3:
        fill(green);
        break;
      case 4:
        fill(yellow);
        break;
      }
      rect(0 + X, 10 + i * 50, arduino.analogRead(i), 25);
      fill(black);
      text(arduino.analogRead(i), 120 + X, 29 + 50 * i);
    }
    
    
    fill(on);
    for (int i = 0; i <= 3; i++)
    {
      rect(-40 + X, 10 + i * 50, 25, 25, r);
      rect(270 + X, 10 + i * 50, 25, 25, r);
    }
    fill(red);
    rect(405 + X, 405, 35, 35, r);
    fill(yellow);
    for(int i = 0; i <= 2; i++)
    {
      rect(370 + X, 10 + 50 * i, 70, 25, r);
    }
    
    for(int i = 0; i <= 3; i++)
    {
      if (mouseX >= 270 + X && mouseX <= 295 + X && mouseY >= 10 + i * 50 && mouseY <= 35 + i * 50)
      {
        fill(mouse_on);
        rect(270 + X, 10 + i * 50, 25, 25, r);
      }
      else if (mouseX >= -40 + X && mouseX <= -15 + X && mouseY >= 10 + i * 50 && mouseY <= 35 + i * 50)
      {
        fill(mouse_on);
        rect(-40 + X, 10 + i * 50, 25, 25, r);
      }
      
    }
    for(int i = 0; i <= 2; i++)
    {
      if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 10 + 50 * i && mouseY <= 35 + 50 * i)
      {
        fill(mouse_yellow);
        rect(370 + X, 10 + 50 * i, 70, 25, r);
      }
    }
    fill(black);
    textSize(25);
    for(int i = 0; i <= 3; i++)
    {
      text("-", -32 + X, 31 + 50 * i);
      text("+", 275 + X, 32 + 50 * i);
    }
    textSize(20);
    text("SAVE", 378 + X, 30);
    text("RESET", 373 + X, 80);
    text("CALIB", 377 + X, 130);
    textSize(30);
    text("X", 413 + X, 434);
  }
  else
  {
    for(int i = 0; i < Arduino.list().length; i ++)
    {
      
      fill(on);
      rect(155, 190 + i * 35, 70, 25, r);
      fill(black);
      textSize(18);
      text(Arduino.list()[i], 160, 210 + i * 35);
    }
    if(connect != -1)
    {
      try
        {
          arduino = new Arduino(this, Arduino.list()[connect], 115200);
          val = true;
        }
        catch(java.lang.RuntimeException e)
        {
          println("Port is busy");
        }
      connect = -1;
    }
  }
}
void mousePressed()
{
  if(val)
  {
    fill(black);
    for (int i = 0; i < 4; i++)
    {
      if (mouseX >= -40 + X && mouseX <= -15 + X && mouseY >= 10 + i * 50 && mouseY <= 35 + i * 50)
      {
        rect(-40 + X, 10 + i * 50, 25, 25, r);
        arduino.analogWrite(i, 0);
        break;
      }
      else if (mouseX >= 270 + X && mouseX <= 295 + X && mouseY >= 10 + i * 50 && mouseY <= 35 + i * 50)
      {
        rect(270 + X, 10 + i * 50, 25, 25, r);
        arduino.analogWrite(i, 2);
        break;
      }
    }
    if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 10 && mouseY <= 35)
    {
      fill(red);
      rect(370 + X, 10, 70, 25, r);
      arduino.analogWrite(4, 0);
    }
    else if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 60 && mouseY <= 85)
    {
      fill(red);
      rect(370 + X, 60, 70, 25, r);
      arduino.analogWrite(4, 1);
    }
    else if (mouseX >= 370 + X && mouseX <= 440 + X && mouseY >= 110 && mouseY <= 135)
    {
      fill(red);
      rect(370 + X, 110, 70, 25, r);
      arduino.analogWrite(5, 1);
    }
    else if (mouseX >= 405 + X && mouseX <= 440 + X && mouseY >= 405 && mouseY <= 440)
    {
      fill(yellow);
      rect(405 + X, 405, 35, 35, r);
      connect = -1;
      val = false;
      arduino.dispose();
    }
  }
  else
  {
    for(int i = 0; i < Arduino.list().length; i ++)
    {
      
      if (mouseX >= 155 && mouseX <= 225 && mouseY >= 190 + i * 35 && mouseY <= 215 + i * 35)
      {
        fill(mouse_on);
        rect(155, 190 + i * 35, 70, 25, r);
        fill(black);
        textSize(18);
        text(Arduino.list()[i], 160, 210 + i * 35);
        connect = i;
      }
    }
  }
}
