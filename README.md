# LoopHero_Mods

This repo contains my mods for Loop Hero.

Right now, only the "Borderless Window" mod works, <br>
as YYToolkit (ver. 2.1.2 as of 2023/04/03) crashes when entering the battle room.<br>

# Installation

Download the latest release from the release tab on the right --><br>
Unzip the .zip file so the contents are in the game directory<br>
where `Loop Hero.exe` resides.<br>

### Your directory should look like this:<br>
```
├── Loop Hero.exe
├── data.win
├── dinput8.dll
├── NativeMods/
│   ├── mod dlls go here!
│   └── BorderlessHero.dll
└── *other game files*
```
# Controls
## Borderless Hero (Borderless window mod)
![grafik](https://user-images.githubusercontent.com/56673835/229794941-73488396-fcb6-4894-8698-a7bfeba6b8e8.png)
<br>
The mod enables the "maximize button" of the game window ^^
- Click the "maximize" button of the window to enable borderless fullscreen (Or press F11 on your keyboard)
- To change back to normal fullscreen:
  - 1. Click "fullscreen"
  - 2. Click "window x2"

![grafik](https://user-images.githubusercontent.com/56673835/229796072-790944ee-ef0e-457e-ad41-eee2d4c77832.png)

### The magic

First of all, Loop Hero natively deactivates the maximize button, which I re-enabled with the mod.<br>
I then hook the Maximize-Window-Event to trigger my "Make the window borderless fullscreen"-magic.<br>
The mod gets loaded automatically by using "DLL-Hijacking", the game thinks the `dinput8.dll` is the real one,<br>but all it does is load the DLLs inside the "Native Mods" folder, and then forward all calls to the original DLL.<br>



# Credits

For loading the mods, I use YellowAfterlife's TinyModInjector.<br>
Can be found [here](https://github.com/YAL-Game-Tools/TinyModInjector "github.com/YAL-Game-Tools/TinyModInjector").
