> [!NOTE]  
> Interested in making mods? Check this repository: [here](https://github.com/sam-k0/LoopHero-TemplateMod)

A list of all mods made by me is shown [below](#available-mods).<br>
You can click on the little arrow (⮞) to see details. <br>

All mods follow more or less the same [install procedure](https://github.com/sam-k0/LoopHero_Mods).

> [!IMPORTANT]
> The current releases of all mods listed here require [YYTK for Loop Hero](https://github.com/sam-k0/YYToolkit_LoopHero/releases) **2026.10.14** or newer **and** [Callback Core](https://github.com/sam-k0/LoopHeroCallbackCore/releases) **2026.10.14** or newer, due to breaking changes.<br>
> If you are updating from an older setup, replace `YYToolkit.dll`, `dinput8.dll` and `LoopHeroCallbackCore.dll` as well.

# Quick install overview
Install `YYTK for Loop Hero` (2026.10.14 or newer) from [here](https://github.com/sam-k0/YYToolkit_LoopHero/releases).
You need to download both `dlls`, `YYToolkit.dll` and `dinput8.dll`, and place `dinput8.dll` in the same folder as the game executable.
For more info on why I use this, you can read this cool guide here [here](https://github.com/YAL-Game-Tools/TinyModInjector).
For the `YYToolkit.dll`, make a new directory called `NativeMods` and put it there.

Next, install the [core callback mod](https://github.com/sam-k0/LoopHeroCallbackCore/releases) (2026.10.14 or newer) before trying to run the game with the below listed mods.
After downloading, create a new directory called `autoexec` and put it there.
It will look something like this: 

```
├── Loop Hero.exe          <-- the game executable
├── data.win
├── dinput8.dll            <-- the mod injector (makes the game load the mod framework), loads dll files in the "NativeMods" folder
├── NativeMods/
│   └── YYToolkit.dll      <-- the mod framework (Loads the actual mods)
├── autoexec/              <-- the folder where you put mods
│   └── LoopHeroCallbackCore.dll <-- the callback core / core mod
└── *other game files*
```
If you download a mod that uses the `Assets` folder, create it also in the game directory and put all contents like jpg files there.

→ If you are using a SteamDeck (Linux) you need to follow these extra steps [to get proton recognize the dinput8.dll](https://www.reddit.com/r/linux_gaming/comments/y08u34/comment/irqlpkr/?utm_source=share&utm_medium=mweb3x&utm_name=mweb3xcss&utm_term=1&utm_content=share_button).

<a id="available-mods"></a>
# Available mods

## Core Mod (Callback Core)
You **need** this mod installed. The other mods depend on this and not installing it will cause the other mods to not work.<br>
 
<details>
  <summary>Details</summary>
  Dependency for all mods, manages callbacks and provides some shared utilities.
  Requires YYTK for Loop Hero 2026.10.14 or newer.
</details>

[Download](https://github.com/sam-k0/LoopHeroCallbackCore/releases) [![Release](https://img.shields.io/github/v/release/sam-k0/LoopHeroCallbackCore)](https://github.com/sam-k0/LoopHeroCallbackCore/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/LoopHeroCallbackCore/total)](https://github.com/sam-k0/LoopHeroCallbackCore/releases)

## Explorer
A must-have utility for datamining and investigating game behavior<br>
<details>
  <summary>Details</summary>
  Adds various tools to the game, such as a (global & instance) variable explorer, running GML commands, profiling performance and dumping game events.
</details>

[Download](https://github.com/sam-k0/LoopHero-Explorer/releases) [![Release](https://img.shields.io/github/v/release/sam-k0/LoopHero-Explorer)](https://github.com/sam-k0/LoopHero-Explorer/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/LoopHero-Explorer/total)](https://github.com/sam-k0/LoopHero-Explorer/releases)

## Snowy Hero 
Adds a cozy snow effect to the game<br>
<details>
  <summary>Details</summary>
  Does what the captions says. Works well with the winter hero asset pack

</details>

[Download](https://github.com/sam-k0/LoopHero-Snow/releases) [![Release](https://img.shields.io/github/v/release/sam-k0/LoopHero-Snow)](https://github.com/sam-k0/LoopHero-Snow/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/LoopHero-Snow/total)](https://github.com/sam-k0/LoopHero-Snow/releases)

## BattleInfo 
Displays extended information during battle<br>
<details>
  <summary>Details</summary>
  Displays more detailed information for enemies during battle.
  Configuration can be changed in `options.ini` file.

</details>

[Download](https://github.com/sam-k0/LoopHero-BattleInfo/releases) [![Release](https://img.shields.io/github/v/release/sam-k0/LoopHero-BattleInfo?include_prereleases)](https://github.com/sam-k0/LoopHero-BattleInfo/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/LoopHero-BattleInfo/total)](https://github.com/sam-k0/LoopHero-BattleInfo/releases)

## Chroma Hero (Custom Color Mod)
Adds the option to choose a custom color for the hero on the map<br>
 
<details>
  <summary>Details</summary>
  Go to the settings menu where you normally choose your color and click the mini-preview once. The picker will appear. Change Color and Brightness using the arrow keys:
  - Up / Down arrow: Cycle color
  - Left / Right: Change brightness

</details>

[Download](https://github.com/sam-k0/Chroma-Hero/releases/) [![Release](https://img.shields.io/github/v/release/sam-k0/Chroma-Hero)](https://github.com/sam-k0/Chroma-Hero/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/Chroma-Hero/total)](https://github.com/sam-k0/Chroma-Hero/releases)

## Discord Presence for Loop Hero
Adds support for Discord Rich Presence to the game<br>
<details>
  <summary>Details</summary>
  This mod will display a status on your discord profile saying "Playing Loop Hero" and then depending if you're in camp, "In Camp" or "On Expedition".
</details>

[Download](https://github.com/sam-k0/LoopHero-DiscordPresence/releases/) [![Release](https://img.shields.io/github/v/release/sam-k0/LoopHero-DiscordPresence)](https://github.com/sam-k0/LoopHero-DiscordPresence/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/LoopHero-DiscordPresence/total)](https://github.com/sam-k0/LoopHero-DiscordPresence/releases)

## Borderless Window (Borderless Hero)
Allows to change the window to borderless fullscreen<br>
<details>
  <summary>Details</summary>
Reactivates the "maximize" button on the game window, and clicking it will enable borderless fullscreen.
This can be really useful if you want to use your taskbar while playing in fullscreen or if you have multiple monitors.
</details>

[Download](https://github.com/sam-k0/LoopHero_Mods/releases) [![Release](https://img.shields.io/github/v/release/sam-k0/LoopHero_Mods)](https://github.com/sam-k0/LoopHero_Mods/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/LoopHero_Mods/total)](https://github.com/sam-k0/LoopHero_Mods/releases)

## Custom Sprite Replacer
Lets you change game sprites (example included)<br>
<details>
  <summary>Details</summary>
This mod allows you to replace sprites during runtime. Unlike replacing the sprites in the game files like when using [UndertaleModTool] for example, you don't need to back-up the game files before modifying them, as these don't get changed on your hard disk.
Also you can't break your game installation using this.
</details>

[Download](https://github.com/sam-k0/AssetLoader/releases) [![Release](https://img.shields.io/github/v/release/sam-k0/AssetLoader)](https://github.com/sam-k0/AssetLoader/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/AssetLoader/total)](https://github.com/sam-k0/AssetLoader/releases) - 
[Usage instructions](https://github.com/sam-k0/AssetLoader/blob/master/README.md)

## Scheduler Fix
Fixes lags caused by a bottleneck between CPU <-> GPU<br>
<details>
  <summary>Details</summary>
    Fixes the problem described <a href="https://forum.gamemaker.io/index.php?threads/fps-issue-getting-wierd-fps-drops-sometimes.14564/">here</a>.
    Basically if your game lags for no reason (i.e. also in the main menu) even though your GPU and CPU are not anywhere near max capacity, this mod may fix the game for you.
</details>

[Download](https://github.com/sam-k0/LoopHero_SchedulerFix/releases) [![Release](https://img.shields.io/github/v/release/sam-k0/LoopHero_SchedulerFix)](https://github.com/sam-k0/LoopHero_SchedulerFix/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/LoopHero_SchedulerFix/total)](https://github.com/sam-k0/LoopHero_SchedulerFix/releases)

## Music Maker Unlocked
Unlocks the hidden *Music Maker*.<br>
<details>
  <summary>Details</summary>
    This unlocks the probably least well known game feature,
    a music maker "level" hidden in the games files, inaccessible by default.
    There is a button in the main menu that will take you there.
</details>

[Download](https://github.com/sam-k0/LoopHero-MusicMaker/releases) [![Release](https://img.shields.io/github/v/release/sam-k0/LoopHero-MusicMaker)](https://github.com/sam-k0/LoopHero-MusicMaker/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/LoopHero-MusicMaker/total)](https://github.com/sam-k0/LoopHero-MusicMaker/releases)

## Pi Calculator
Calculates pi (the funny 3.1415 number) while you play.<br>
<details>
  <summary>Details</summary>
    Does what the title says. Pretty useless but if you like funny numbers then this is for you.
</details>

[Download](https://github.com/sam-k0/LoopHero-PiCalculator/releases) [![Release](https://img.shields.io/github/v/release/sam-k0/LoopHero-PiCalculator)](https://github.com/sam-k0/LoopHero-PiCalculator/releases) [![Downloads](https://img.shields.io/github/downloads/sam-k0/LoopHero-PiCalculator/total)](https://github.com/sam-k0/LoopHero-PiCalculator/releases)

