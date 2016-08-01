# Irrigation

This is the start of Ian’s Farm irrigation project. 
Utilizes Particle.io Electron platform.
Data is hosted via an Adafruit.io feed, and data writes are triggered through IFTTT

# To Do:
- Implement control from Adafruit.io
- Implement scheduling based on real time
	- Implement backup system for no-connectivity failover schedule maintenance
- Implement RGB-backlit LED 

# Remote functions are: 
**relayTest** - Tests all relays. 
**startPhase(**phaseNumber**)** - Start the provided phase.
**nextPhase** - Skip the current phase and continue to the next.

# Remote variables are:
anemoValue
windspeed
hygro1
hygro2
soilAvg

# Current setup:
Delay-based, but with an onboard RTC, so accurate to real time. Every 12 hours, perform the following phases:
- Phase 1: Enable pump, begin watering Row 1 for x minutes
- Phase 2: Close Row 1, begin watering Row 2 for x minutes
- Phase 3: Close Row 2, begin Sprinklers for x minutes
- Phase 4: Stop sprinklers, stop Pump.
- Phase 5: Idle for 12 hours.