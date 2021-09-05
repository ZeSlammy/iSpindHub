# **iSpindHub**
- [**iSpindHub**](#ispindhub)
- [Overall Project Information](#overall-project-information)
- [Major Thanks](#major-thanks)
- [Documentation is in progress](#documentation-is-in-progress)
- [Build your Own !](#build-your-own-)
  - [Generic stuff](#generic-stuff)
  - [1.4 TFT Screen](#14-tft-screen)
  - [1.77" TFT Screen](#177-tft-screen)
  - [2" TFT Screen](#2-tft-screen)
- [To Do List](#to-do-list)
 

  
# Overall Project Information

This project is widely inspired by [**TiltBridge**](https://www.tiltbridge.com/) and [**Nautilis' iSpindel Relay**](http://www.nautilis.eu/en/portfolio-item/nautilis-ispindel-relay/).

Easiest way to install is through [**BrewFlasher**](https://github.com/thorrak/brewflasher/releases), project is iSpindHub.


It is basically a receiver to get information from your iSpindels and display it allowing you to use your iSpindel(s) even if you do not have WiFi available in your brew cave.

It also acts as a WiFi repetiter and will push iSpindel information to various services (BrewFather, LittleBock, CraftBeerPi etc).

![Image of a iSpindhub Display](https://raw.githubusercontent.com/ZeSlammy/iSpindHub/master/pictures/iSpindHub2.jpg)

    

It's a work in Progress :D

# Major Thanks
I cannot thank enough [**Thorrak**](https://github.com/thorrak) and [**lbussy**](https://github.com/lbussy) for helping me during my dumb moments.

# Documentation is in progress
Check it on in the [**docs folder**](https://www.github.com/ZeSlammy/iSpindHub/docs/) !

# Build your Own !
## Generic stuff
I will try and list the various screens I have tested and am sure are working (this might lead to multiple releases unless I find an elegant way to have this as a selectable option)
I might have to rewrite my code to use a generic library and not a specific one ...

## 1.4 TFT Screen
This one is an easy to find on Ali Express and is nice because it plugs directly onto a D1 Mini.
Display is square 128*128

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


[**find it on Amazon.com**](https://www.amazon.com/1-44Inch-Display-Resolution-Peripheral-Interface/dp/B08135JYP4/)

[**find it on Amazon.fr**](https://amzn.to/3DwjqVb) REFERRAL LINK


## 1.77" TFT Screen
Display is 160*128
| Marking on screen  |  D1 Mini PIN  |  Alternative names  | Comments  |
| :------------: | :------------: | :------------: | :------------: |
| LEDA  | D4  | 8 | Could be on 5V if your screen is tolerant  |
|  GND | GND  |  1 |   |
|  RES | D2  |  5 |   |
| RS  | D1  | 6  |   |
| SDA  | D7  | 4  |   |
| SCK  | D5  |  3 |   |
| VCC  | 3V3  | 2  |   |
| CS  | D8  | 7  |   |

[**Find it on AliExpress**](https://www.aliexpress.com/item/32265216471.html?spm=a2g0s.9042311.0.0.27424c4dJhxqbL)

[**Find it on Amazon.fr**](https://amzn.to/3gOuxPC) REFERRAL LINK

## 2" TFT Screen
Red PCB with Red Tab
Display is 176*220


| Marking on screen  |  D1 Mini PIN  |  Alternative names  | Comments  |
| :------------: | :------------: | :------------: | :------------: |
| LED  | D4  | LCD | Could be on 5V if your screen is tolerant  |
|  GND | GND  |  G |   |
|  RST | D2  |   |   |
| DC  | D1  | RS  |   |
| MOSI  | D7  | SDI  |   |
| SCK  | D5  |   |   |
| 3V  | 3V3  |   |   |
| CS  | D8  |   |   |

[**Find it on AliExpress**](https://www.aliexpress.com/item/32842202564.html?spm=a2g0s.9042311.0.0.27424c4dJhxqbL)

[**Find it on Amazon.fr**](https://amzn.to/3mOLFIX) REFERRAL LINK
# To Do List

- [x] HomePage showing info about iSpindels

- [ ] Configuration Page for Little Bock

- [ ] Configuration Page for BrewFather

- [ ] Configuration Page for URL

- [ ] Jobs that send info to LittleBock / BrewFather / etc

- [ ] Page to manage log files (Delete / Trim)

- [ ] Historical graphs

- [ ] Handle multiple screens through multiple releases

- [ ] Handle multiple screens through options in configuration