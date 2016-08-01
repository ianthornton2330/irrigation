Start Watering Every 24 Hours {
    Turn Pump On and Open Valve 1 for 15 Mins &log ()
    Close Valve 1 and Open Valve 2 for 15 Mins &log ()
    Close Valve 2 and Open Valve 3 for 15 Mins &log ()
    Close Valve 3 and Open Valve 4 for 5 Minutes &log ()
    Close Valve 4 and Turn Off Pump &log ()
    Wait 23 Hours 10 Minutes &log ()
                    }

/* 2 types of statements:
 1: Do X Every Y Mins
 2: Do X For Y Mins */

/* Line 1 would be Type 1, Lines 2-7 would be Type 2 */

/* Eventual goals:
    SUPER IMPORTANT - I need a way, preferably a momentary button to turn the pump on and off so I can use the
        hose without having to open an app or anything.
    I also need a water tight, UV rated box. The sun is gonna beat down on it daily, and it's imperative that the electronics don't get hit by those rays.
 
 Lesser Importance - but cool integrations
    GPRS connectivity - via Arduino GPRS Shield or using an Electron/Dash (preferably, because of the IDE/platform/and the pre-existing app, AND LOGGING)
    Integrated Hygrometers to measure watering (and integreting the prevention of over/underwatering)
    Water Metering - and logging how much water is being used per valve (not sure if there's an arduino compatible meter)
    IFTTT notifications of overages on water usages (anything outside suitable hygrometer ranges, water meter usages/etc.) -- this will help cut down on maintenance and finding problems.
    Wind/Temperature metering and logging (Weather station? - we get really high winds pretty consistently)
    Solar charging logging every...say...1 min (- Current Amps/Voltage the panel is putting out)
    Power consumption levels during different waterings, standby, etc.
    Current battery level would be nice too, if I could just open my app and see where it's at, and a notification if it drops below 25%.
 
*/