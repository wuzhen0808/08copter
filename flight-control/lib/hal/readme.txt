hal is the abstract layer to isolate the underlying hardware.
hal is located on the middle or the bottom of the whole system.

what should be on top of hal? 
1. any code want to access hardware.for instance to display a message on a monitor.

what should be on the bottom of hal?
1. hardware driver.
2. native os.
3. simulation env.

what should not be on the bottom of hal?
1. any code no need to access a hardware.
2. any code that doesn't care the difference of hardwares.




