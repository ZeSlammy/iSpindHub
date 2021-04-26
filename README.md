# **iSpindHub**

  

<!-- TOC -->autoauto- [**iSpindHub**](#ispindhub)autoauto<!-- /TOC -->

  
# Overall Project Information

This project is widely inspired by [**TiltBridge**](https://www.tiltbridge.com/) and [**Nautilis' iSpindel Relay**](http://www.nautilis.eu/en/portfolio-item/nautilis-ispindel-relay/).


It is basically a receiver to get information from your iSpindels and display it allowing you to use your iSpindel(s) even if you do not have WiFi available in your brew cave.

It also acts as a WiFi repetiter and will push iSpindel information to various services (BrewFather, LittleBock, CraftBeerPi etc).

![Image of a iSpindhub Display](https://raw.githubusercontent.com/ZeSlammy/iSpindHub/master/pictures/iSpindHub2.jpg)

  
  

It's a work in Progress :D

# Build your Own !
## Generic stuff
I will try and list the various screens I have tested and am sure are working (this might lead to multiple releases unless I find an elegant way to have this as a selectable option)
I might have to rewrite my code to use a generic library and not a specific one ...

## 1.4 TFT Screen
This one is an easy to find on Ali Express and is nice because it plugs directly onto a D1 Mini.
| Marking on screen  |  D1 Mini PIN  |  Alternative names  | Comments  |
| :------------: | :------------: | :------------: | :------------: |
| LED  | 3.3V  | LCD | Could be on 5V if your screen is tolerant  |
|  GND | GND  |  G |   |
|  RST | RESET  |   |   |
| DC  | D3  |   |   |
| MOSI  | D7  | SDI  |   |
| SCK  | D5  |   |   |
| 3V  | 3V3  |   |   |
| CS  | D4  |   |   |

![Back of the Red Tab 1.4 TFT](https://github.com/ZeSlammy/iSpindHub/blob/master/pictures/1_4_TFT_RedTab_Back.jpg?raw=true "Back of the Red Tab 1.4 TFT")
![Front of the Red Tab TFT](https://github.com/ZeSlammy/iSpindHub/blob/master/pictures/1_4_TFT_RedTab_Front.jpg?raw=true "Front of the Red Tab TFT")

## To Do List

- [x] HomePage showing info about iSpindels

- [ ] Configuration Page for Little Bock

- [ ] Configuration Page for BrewFather

- [ ] Configuration Page for URL

- [ ] Jobs that send info to LittleBock / BrewFather / etc

- [ ] Page to manage log files (Delete / Trim)

- [ ] Historical graphs

- [ ] Handle multiple screens through multiple releases

- [ ] Handle multiple screens through options in configuration