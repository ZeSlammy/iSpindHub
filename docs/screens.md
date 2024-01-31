# All About Screens

## Why is there a screen format ?
Instead of having people tring to find their way through my mucky code, I thought it'd be easier to have a screen format allowing for everybody to design the perfect screen layout for them.
Here it is.

## Overall Format

### It's JSON DUH
Sounded like the easiest way to have a clean structure and also I finally know how to parse JSON on an ESP8266

### The Structure
- 2 types of entries
  - global
  Gives generic information about the screen (orientation, background color,fonts etc)
  - lines
  Doesn't have to be A line but it's the best I could came up with. For those, there are 4 possible types (at the moment) that I'll detail just after.
  Lines do not have to be put in the order they will appear on the screen but it's easier I guess
- For the lines, various variables are available
  - Specific Gravity #SG
  - Signal Strength in dB #RSSI
  - Temperature #Temp
  - Angle of the iSpindel #Angle
  - Voltage of the battery #Voltage
  - Name of the iSpindel #deviceName
  - Duration since last seen #LastSeen
  - Local IP adress of the iSpindHub #IP

#### Global
- In here you'll define everyting that'll be true for the whole screen
  - Background Color (bc)
  - Font Color (fc)
  - Whether or not the text is wrapped (tw)
  - Generic/Default Font (default_font)
  - Screen Orientation (r)

#### Generic for Lines
- When passing a position (x or y), you can use the keyword "MAX" and use x_delta or y_delta. This will replace the MAX par TFT_WIDTH and TFT_HEIGHT respectfully and apply the delta to it (say you want it 20 pixels from the bottom, you'll write "y" : "MAX" and "y_delta" : -20 in the JSON Template)
- Same applies for lines (2 points so x_0/y_0 and x_1,y_1)

#### Text Lines
- To define a text line you need :
  - a font (f)
    - Can be empty to use System font
  - a color (c)
    - can be empty
  - a position (x and y)
  - a centered (ctr) attribute
  - a text (obviously)
- You can have the color changing based on values of your variable.
  - You need a var key in which you put the name of the variable you want to use (ex : "var" : "#RSSI")
  - You need to build a "cs" entry that list in order the 3 couples threshold/color you want to use (low/medium/high)
    - A couple is defined by a "val" and a "col" attribute
  - You need a default color (def_col)

