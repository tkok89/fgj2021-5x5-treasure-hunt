# 5x5 Treasure Hunt
A game about friendship and other treasures. You are trying to find the greatest treasure in the world. Connect with your friend and start the Hunt!

# Controls
Move your character with Arrow keys. Use mouse to start game with buttons at the top left corner (host or join multiplayer or play single player game).


# Building on Linux
Just works I guess

# Building on Windows
Open "local folder" like a root
Should notice cmake automatically
Select from solution Explorer "switch views" > cmake target
Select ggj21 (executable) Add debug config and add  "currentDir": "${projectDir}" under "configuration" array
Then select ggj21 (executable) > generate cache
Then you can select ggj21.exe as a startup item and go on pressing Debug-buttons


# Building on a Mac

- You need to install SMFL as a framework as pointed in https://www.sfml-dev.org/tutorials/2.5/start-osx.php
- You can find XCode project file inside 
```
\util\fgj2021-lost-and-found\fgj2021-lost-and-found.xcodeproj folder
```
- You might need to sign Frameworks, then use codesigning https://developer.apple.com/library/archive/documentation/Security/Conceptual/CodeSigningGuide/Procedures/Procedures.html
```
codesign -s <identity> <code-path>
```


- You might be able to build without xcode, but I don't know how :)

tominDayroll325