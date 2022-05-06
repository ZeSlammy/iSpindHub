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
  - Temperature #temp
  - Angle of the iSpindel #angle
  - Voltage of the battery #battery
  - Name of the iSpindel #name
  - Duration since last seen #lastseen
  - Local IP adress of the iSpindHub #IP

#### Global
- In here you'll define everyting that'll be true for the whole screen
  - Background Color (back_color)
  - Font Color (font_color)
  - Whether or not the text is wrapped (text_wrap)
  - Generic/Default Font (default_font)
  - Screen Orientation (rotation)

