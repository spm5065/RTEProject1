# RTEProject1
Prints histogram distribution of a periodic squarewave using Hardware Interrupts and Timers

Uses an STM32L476 Discovery board

This takes in a periodic square wave (typically from a function generator), and measures the time between pulses within a command line configurable 100 microsecond window.
It then prints this information in a histogram form.
