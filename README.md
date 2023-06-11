# LoopHero_Mods

This repo contains my mods for Loop Hero.

# Installation of the modloader

Go to the Releases tab on the right and download your desired mod.

Go to <br>
https://github.com/sam-k0/YYToolkit_LoopHero/releases/ <br>
and download the modified version of YYTK (The modding framework / Modloader)
- Download "YYToolkit.dll"
- Download "dinput8.dll"
- Create folders like shown in the graphic here:
  - Move "dinput8.dll" into your main game directory (where the .exe is)
  - Create "NativeMods" folder and put "YYToolkit.dll" there
  - Create "autoexec" folder, this is where you'll put the mods that depend on YYTK
  
### Your directory should look like this:
```
├── Loop Hero.exe
├── data.win
├── dinput8.dll            <-- the mod loader
├── NativeMods/
│   ├── YYToolkit.dll      <-- the mod framework
│   └── *other mod files*
├── autoexec/
│   └── *other mod files*
└── *other game files*
```
# Installation of the actual mods
The total list of mods is here:<br>
https://github.com/sam-k0/LoopHero_Mods/releases


## Discord Presence mod
This mod will display a status on your discord profile saying "Playing Loop Hero" and then depending if you're in camp, "In Camp" or "On Expedition"
<br>
**Download Instructions:**
- Download the "Discord Presence"-release from above mentioned release page
- Move the "discord-rpc.dll" into the main game directory
- Move the "DiscordPresence.dll" into the "autoexec" folder
### Your directory should look like this:<br>
```
├── Loop Hero.exe
├── data.win
├── dinput8.dll            <-- the mod loader
├── discord-rpc.dll        <-- ! This is the official discord api dll!
├── NativeMods/
│   ├── YYToolkit.dll      <-- the mod framework
│   └── *other mod files*
├── autoexec/
│   ├── DiscordPresence.dll <-- The actual mod that calls the official discord dll
│   └── *other mod files*
└── *other game files*
```


## Borderless Hero (Borderless window mod)

### Your directory should look like this:<br>
! BorderlessHero is a native mod and doesn't depend on the YYToolkit, so make sure to move it to the "NativeMods" folder instead of the "autoexec" folder!

```
├── Loop Hero.exe
├── data.win
├── dinput8.dll            <-- the mod loader
├── NativeMods/
│   ├── YYToolkit.dll      <-- the mod framework
│   ├── BorderlessHero.dll <-- the actual mod
│   └── *other mod files*
├── autoexec/
│   └── *other mod files*
└── *other game files*
```

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
