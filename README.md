# LoopHero Mods
A general landing page for all things Loop Hero modding.

> [!TIP]
> You can find a list of mods developed by me [here](https://github.com/sam-k0/LoopHero_Mods/blob/master/mods.md).

> [!TIP]
> Some notes on developing LH-specific mods [here](https://github.com/sam-k0/LoopHero_Mods/blob/master/devnotes.md).
> A template project for creating mods can be found [here](https://github.com/sam-k0/LoopHero-TemplateMod)

# Where to get the mods

Each mod lives in its own repository and is downloaded from that repository's Releases page.
The [mod list](https://github.com/sam-k0/LoopHero_Mods/blob/master/mods.md) links to all of them.<br>
The only mod still released from this repository is Borderless Window (Borderless Hero), see the Releases tab on the right.

> [!IMPORTANT]
> The current mod releases require [YYTK for Loop Hero](https://github.com/sam-k0/YYToolkit_LoopHero/releases) **2026.10.14** or newer **and** [Callback Core](https://github.com/sam-k0/LoopHeroCallbackCore/releases) **2026.10.14** or newer, due to breaking changes.<br>
> If you are updating from an older setup, replace `YYToolkit.dll`, `dinput8.dll` and `LoopHeroCallbackCore.dll` as well.

# Installation of the modloader

Go to <br>
https://github.com/sam-k0/YYToolkit_LoopHero/releases/ <br>
and download the modified version of YYTK (The modding framework / Modloader)
- Download "YYToolkit.dll"
- Download "dinput8.dll"
- Create folders like shown in the graphic below:
  - Move "dinput8.dll" into your main game directory (where the .exe is)
  - Create "NativeMods" folder and put "YYToolkit.dll" there
  - Create "autoexec" folder, this is where you'll put the mods that depend on YYTK

Then go to <br>
https://github.com/sam-k0/LoopHeroCallbackCore/releases/ <br>
and download "LoopHeroCallbackCore.dll" (the Callback Core).
- Put it into the "autoexec" folder
- All mods depend on it, they will not work without it

### Your directory should look like this:
```
├── Loop Hero.exe
├── data.win
├── dinput8.dll                  <-- the mod loader
├── NativeMods/
│   └── YYToolkit.dll            <-- the mod framework
├── autoexec/
│   ├── LoopHeroCallbackCore.dll <-- the core mod, required by all other mods
│   └── *other mod files*
└── *other game files*
```

→ If you are using a SteamDeck (Linux) you need to follow these extra steps [to get proton recognize the dinput8.dll](https://www.reddit.com/r/linux_gaming/comments/y08u34/comment/irqlpkr/?utm_source=share&utm_medium=mweb3x&utm_name=mweb3xcss&utm_term=1&utm_content=share_button).

# Installation of the actual mods

Download the mods you want from the [mod list](https://github.com/sam-k0/LoopHero_Mods/blob/master/mods.md) and put the `.dll` files into the "autoexec" folder.<br>
If a mod comes with an `Assets` folder, create it in the game directory as well and put its contents (e.g. image files) there.

# Credits

For loading the mods, I use YellowAfterlife's TinyModInjector.<br>
Can be found [here](https://github.com/YAL-Game-Tools/TinyModInjector "github.com/YAL-Game-Tools/TinyModInjector").

The used framework is obv YYTK developed by Archie.
