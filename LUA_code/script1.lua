
-- Pin definition 
local pin = 16            --  GPIO13
local status = gpio.LOW
local duration = 1000    -- 1 second duration for timer

-- Initialising pin
gpio.mode(pin, gpio.OUTPUT)
gpio.write(pin, status)

-- Create an interval
tmr.alarm(0, duration, 1, function ()
    if status == gpio.LOW then
        status = gpio.HIGH
    else
        status = gpio.LOW
    end

    gpio.write(pin, status)
end)


-- connect to WiFi access point
--wifi.setmode(wifi.STATION)
--wifi.sta.config{ssid="Nuria's Hizzie 2", pwd="roundwind557"}
