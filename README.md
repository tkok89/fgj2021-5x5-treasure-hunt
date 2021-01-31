# 5x5 Treasure Hunt
A game about friendship and other treasures. You are trying to find the greatest treasure in the world. Connect with your friend and start the Hunt!

# Controls
Play with Arraw keys. Use mouse to use buttons at top left corner to host, join or single play.


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