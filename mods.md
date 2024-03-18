# Available mods

A list of all mods made by me is shown below.<br>
You can click on the little arrow (⮞) to see details. <br>
The (⚙) denotes the required modloader (`YYTK for Loop Hero`) framework version, **not** the `YYTK` version.<br>
All mods follow more or less the same [install procedure](https://github.com/sam-k0/LoopHero_Mods).
# Quick install overview
Install the `YYTK for Loop Hero` from [here](https://github.com/sam-k0/YYToolkit_LoopHero/releases).
You need to download both `dlls`, `YYToolkit.dll` and `dinput8.dll`, and place `dinput8.dll` in the same folder as the game executable.
For more info on why I use this, you can read this cool guide here [here](https://github.com/YAL-Game-Tools/TinyModInjector).
For the `YYToolkit.dll`, make a new directory called `NativeMods` and put it there.

Next, install the [core callback mod](https://github.com/sam-k0/LoopHeroCallbackCore) before trying to run the game with the below listed mods.
After downloading, create a new directory called `autoexec` and put it there.
It will look something like this: 

```
├── Loop Hero.exe          <-- the game executable
├── data.win
├── dinput8.dll            <-- the mod injector (makes the game load the mod framework), loads dll files in the "NativeMods" folder
├── NativeMods/
│   └── YYToolkit.dll      <-- the mod framework (Loads the actual mods)
├── autoexec/              <-- the folder where you put mods
│   └── Loop Hero Callback Core.dll <-- the callback core / core mod
└── *other game files*
```
If you download a mod that uses the `Assets` folder, create it also in the game directory and put all contents like jpg files there.

→ If you are using a SteamDeck (Linux) you need to follow these extra steps [to get proton recognize the dinput8.dll](https://www.reddit.com/r/linux_gaming/comments/y08u34/comment/irqlpkr/?utm_source=share&utm_medium=mweb3x&utm_name=mweb3xcss&utm_term=1&utm_content=share_button).


## Core Mod (Callback Core)
You **need** this mod installed. The other mods depend on this and not installing it will cause the other mods to not work.<br>
<span style="color: cyan;">⚙ supported by </span>``≥ 2.1.6``
<details>
  <summary>Details</summary>
  Go to the camp, open the settings menu and click on the "mods" button to manage mods.
</details>

## Chroma Hero (Custom Color Mod)
Adds the option to choose a custom color for the hero on the map<br>
<span style="color: cyan;">⚙ supported by </span>``≥ 2.1.6``
<details>
  <summary>Details</summary>
  Go to the settings menu where you normally choose your color and click the mini-preview once. The picker will appear. Change Color and Brightness using the arrow keys:

  - Up / Down arrow: Cycle color
  - Left / Right: Change brightness

</details>

[Download](https://github.com/sam-k0/Chroma-Hero/releases/)

## Discord Presence for Loop Hero
Adds support for Discord Rich Presence to the game<br>
<span style="color: cyan;">⚙ supported by </span>``≥ 2.1.6``
<details>
  <summary>Details</summary>
  This mod will display a status on your discord profile saying "Playing Loop Hero" and then depending if you're in camp, "In Camp" or "On Expedition".
</details>

[Download](https://github.com/sam-k0/LoopHero_Mods/releases)

## Borderless Window (Borderless Hero)
Allows to change the window to borderless fullscreen<br>
<span style="color: cyan;">⚙ supported by </span>``≥ 2.1.6``
<details>
  <summary>Details</summary>
Reactivates the "maximize" button on the game window, and clicking it will enable borderless fullscreen.
This can be really useful if you want to use your taskbar while playing in fullscreen or if you have multiple monitors.
</details>

[Download](https://github.com/sam-k0/LoopHero_Mods/releases)

## Custom Sprite Replacer
Lets you change game sprites (example included)<br>
<span style="color: cyan;">⚙ supported by </span>``≥ 2.1.6``
<details>
  <summary>Details</summary>
This mod allows you to replace sprites during runtime. Unlike replacing the sprites in the game files like when using [UndertaleModTool] for example, you don't need to back-up the game files before modifying them, as these don't get changed on your hard disk.
Also you can't break your game installation using this.
</details>

[Download](https://github.com/sam-k0/AssetLoader/releases) - 
[Usage instructions](https://github.com/sam-k0/AssetLoader/blob/master/README.md)

## Scheduler Fix
Fixes lags caused by a bottleneck between CPU <-> GPU<br>
<span style="color: cyan;">⚙ supported by </span>``≥ 2.1.6``
<details>
  <summary>Details</summary>
    Fixes the problem described <a href="https://forum.gamemaker.io/index.php?threads/fps-issue-getting-wierd-fps-drops-sometimes.14564/">here</a>.
    Basically if your game lags for no reason (i.e. also in the main menu) even though your GPU and CPU are not anywhere near max capacity, this mod may fix the game for you.
</details>

[Download](https://github.com/sam-k0/LoopHero_SchedulerFix/releases)

## Music Maker Unlocked
Unlocks the hidden *Music Maker*.<br>
<span style="color: green;">⚙ supported by </span>``≥ 2.1.3``
<details>
  <summary>Details</summary>
    This unlocks the probably least well known game feature,
    a music maker "level" hidden in the games files, inaccessible by default.
    Pressing [F1] key in main menu will take you there.
</details>

[Download](https://github.com/sam-k0/LoopHero-MusicMaker/releases)

## Pi Calculator
Calculates pi (the funny 3.1415 number) while you play.<br>
<span style="color: green;">⚙ supported by </span>``≥ 2.1.3``
<details>
  <summary>Details</summary>
    Does what the title says. Pretty useless but if you like funny numbers then this is for you.
</details>

[Download](https://github.com/sam-k0/LoopHero-PiCalculator/releases)

