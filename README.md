Features: 
  - Automatically navigate between two walls
  - Recognises when it hits a wall directly and allow user to guide it around corners
  - Search a room for any objects within it
  - Navigate down side-corridors
  - Can be controlled from the GUI via the Xbee
  - Has a vague idea of corridor/sub-corridor/room-IDs
  - Can realise it is at the end of the maze 

Key issues: 
  - Calibration, it was found to be a huge issue with regards to getting the reflectance
  sensors to play nicely. This became a major problem when trying to get the zumo to navigate
  it's way back out of the maze with no user intervention, as it could quickly go wrong and
  require a user override. Most of the time spent on this was trying to find ways around this/tune the problem
  away.
  - Using CP5 as a means to make the GUI. Although it works fantastically for sending data to
  the Arduino, even via Xbee (and is fantastically simple). Due to an issue with time management
  I was unable to find a solution to display communication back from the arduino.
  - Ideally, if given another shot, rather than using timings/speeds to turn the Arduino
  precisely, I would have used something similar to the compass example, however this was discovered
  too late into the process (better to have something working badly than not at all!).
  
Sources/Inspiration of code was from the Zumo example libraries.
