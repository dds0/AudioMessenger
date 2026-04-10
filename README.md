# AudioMessenger
Tested on Ubuntu 22.04.5 LTS(localhost)
Used qt: 5.15.3

App shows 2 tab settings:
- network settings
- audio settings

For using network tab, you have to give app admin rules
If you want to startup it on localhost, I'll recommend you multicast
Also it is very important to set unique "Device id"(network tab) in every Audio Messenger App 

I have prepared dissector for wireshark. If you want to change ports, 
don't remember change in dissector too

P.S. AMCDP - Audio Messenger Control & Data Protocol

App diagram
![Screenshot](https://github.com/dds0/AudioMessenger/blob/process/Images/appDiagram.png)

AMDCP_Client - wrapper for protocol and net communication

Audio Queue - solution for jitter problem

Voice Audio Detecter - delete some noises from audio flows

Self-listening switch - feature for testing micro and sound output before dialog

Can't work together with call session

In/Out gateway - giving permission sending/receiving for in / out audio flows

Connection Logic - encapsulates communication rules 

App states
![Screenshot](https://github.com/dds0/AudioMessenger/blob/process/Images/appStates.png)

Active - Correct connection

Denied - Сall was cancelled / disconnected

Missing - Problem with internet connection

Ended - Call ended sucessfully

Idle - Ready for IncomingCall / OutgoingCall

IncomingCall - When someone is calling you

OutgoingCall - When you are calling to someone

Notes:
1. I don't want to use AudioOutput::setVolume method due to my experience
It doens't work properly and predictable on previous versions (5.12.x)

2. I wrote Audio Mixer for working with multiple audio flows, 
but in the end it doesn't have to be there

P.P.S. Sorry for mistakes, my English still need some improvements.
