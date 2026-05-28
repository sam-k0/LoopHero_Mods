# LoopHero Mods
A general landing page for all things Loop Hero modding.

> [!TIP]
> You can find a list of mods developed by me [here](https://github.com/sam-k0/LoopHero_Mods/blob/master/mods.md).


> [!TIP]
> Some notes on developing LH-specific mods [here](https://github.com/sam-k0/LoopHero_Mods/blob/master/devnotes.md).
> A template project for creating mods can be found [here](https://github.com/sam-k0/LoopHero-TemplateMod)

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

You can find a list of mods developed by me [here](https://github.com/sam-k0/LoopHero_Mods/blob/master/mods.md).

# Credits

For loading the mods, I use YellowAfterlife's TinyModInjector.<br>
Can be found [here](https://github.com/YAL-Game-Tools/TinyModInjector "github.com/YAL-Game-Tools/TinyModInjector").

The used framework is obv YYTK developed by Archie.
