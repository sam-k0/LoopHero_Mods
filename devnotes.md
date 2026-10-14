Notes for developing mods.

All mods are YYTK plugins that depend on the [Callback Core](https://github.com/sam-k0/LoopHeroCallbackCore).
The easiest way to start is the [template mod](https://github.com/sam-k0/LoopHero-TemplateMod).

### Calling builtins

Almost everything is done by calling GML builtin functions. The examples below use these small helpers:

```cpp
namespace Binds {
    inline YYRValue CallBuiltinA(const std::string name, const std::vector<YYRValue> args)
    {
        YYRValue var;
        CallBuiltin(var, name, nullptr, nullptr, args);
        return var;
    }
}
```

Numbers are always passed as `double`, so cast enums and ints:

```cpp
YYRValue exists = Binds::CallBuiltinA("instance_exists", { (double)LHObjectEnum::o_hero });
if ((bool)exists)
{...}
```

### Printing

Never pass text as the format string to `PrintMessage`, game strings contain `%` (e.g. "% chance for") and can crash the game.

```cpp
PrintMessage(CLR_GREEN, "%s", text.c_str()); // good
PrintMessage(CLR_GREEN, text.c_str());       // bad
```

### Mod entry and the Callback Core

Multiple mods can't hook the same game event, so the Callback Core does the hooking and forwards every code event to the mods.
Your mod has to wait until the core is loaded before installing its callbacks:

```cpp
DllExport YYTKStatus PluginEntry(YYTKPlugin* PluginObject)
{
    LHCore::CoreReadyPack* pack = new LHCore::CoreReadyPack(PluginObject, InstallPatches); // InstallPatches runs once the core is ready
    PluginObject->PluginUnload = PluginUnload;
    CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LHCore::ResolveCore, (LPVOID)pack, 0, NULL));
    return YYTK_OK;
}

void InstallPatches()
{
    LHCore::pInstallPrePatch(CodePrePatch);
    LHCore::pInstallPostPatch(CodePostPatch);
}

YYTKStatus PluginUnload()
{
    LHCore::pUnregisterModule(gPluginName);
    return YYTK_OK;
}
```

Other functions the core exports (`API_GetWindowHandle`, `API_HttpGetRequest`, ...) are imported with `PmGetExported`, see the core's README.

### Code events

Pre-patches run before the game's code, post-patches after it.
Returning `YYTK_DONTCALL` from a pre-patch cancels the original code.
Post-patches are still called when the original was cancelled, check `ccAttr->call` if that matters:

```cpp
int CodePostPatch(YYTKCodeEvent* codeEvent, void* p_rawCCAttr)
{
    CCode* codeObj = std::get<CCode*>(codeEvent->Arguments());
    CInstance* selfInst = std::get<0>(codeEvent->Arguments());
    CallbackCoreAttributes* ccAttr = static_cast<CallbackCoreAttributes*>(p_rawCCAttr);

    if (!codeObj || !codeObj->i_pName)
        return YYTK_INVALIDARG;

    if (ccAttr->call == OriginalCall::CANCELLED) // another mod cancelled it
        return YYTK_OK;

    if (strcmp(codeObj->i_pName, "gml_Object_o_hero_Step_0") == 0)
    {...}

    return YYTK_OK;
}
```

Event names follow the GameMaker scheme `gml_Object_<object>_<event>_<number>`. The useful ones:

| Event | Meaning |
| --- | --- |
| `Create_0` | Instance was created |
| `Step_0` / `Step_1` / `Step_2` | Step / Begin Step / End Step |
| `Draw_0` | Draw |
| `Draw_64` | Draw GUI |
| `Draw_73` | Draw End |
| `Mouse_4` | Left mouse pressed on the instance |
| `Mouse_10` / `Mouse_11` | Mouse enter / leave |
| `Alarm_<n>` | Alarm n fired |
| `Other_62` | Async HTTP |

Rooms fire `gml_Room_<room>_Create` when they are entered.

Scripts (`gml_Script_...`) don't fire code events because the game is compiled with YYC. Hook the object event that calls the script instead,
e.g. `gml_Script___item_generate` is only called from `gml_Object_o_item_Alarm_0`.

Comparing strings every event is slow-ish, so some mods hash the names at compile time and `switch` on them:

```cpp
constexpr uint32_t Hash(const char* str) // FNV-1a
{
    uint32_t hash = 2166136261u;
    while (*str)
    {
        hash ^= (uint8_t)*str++;
        hash *= 16777619u;
    }
    return hash;
}

constexpr uint32_t HEVT_HERO_STEP = Hash("gml_Object_o_hero_Step_0");

switch (Hash(codeObj->i_pName))
{
case HEVT_HERO_STEP:
    ...
    break;
}
```

### Built In constants

These values have a special meaning:

```c++
#define INSTANCE_GLOBAL -5.0
#define INSTANCE_NOONE -4.0
#define INSTANCE_ALL -3.0
#define INSTANCE_OTHER -2.0
#define INSTANCE_SELF -1.0
```

For example, `instance_find` returns `noone` if there is no instance:

```cpp
double hero = Binds::CallBuiltinA("instance_find", { (double)LHObjectEnum::o_hero, 0. });
if (hero == INSTANCE_NOONE)
{...}
```

### Rooms

These are all the game's rooms:
The main menu seems to be `rm_intro`, which gets immediately switched to after `rm_load` after starting the game.
`rm_camp` is the camp, and `rm_game` is the main game with the path and fights.
`rm_music_maker` is the hidden music maker.

The other rooms have yet to be investigated.

```c++
namespace LHRooms {

	enum LHRoom {
		rm_titles = 0,
		rm_creater_dialog = 1,
		rm_piramidka = 2,
		rm_load = 3,
		rm_intro = 4,
		rm_camp = 5,
		rm_tutor = 6,
		rm_game = 7,
		rm_cutscenes = 8,
		rm_music_maker = 9
	};

	static const std::unordered_map<std::string, LHRoom> LHRoomMap = {
	{"rm_titles", rm_titles},
	{"rm_creater_dialog", rm_creater_dialog},
	{"rm_piramidka", rm_piramidka},
	{"rm_load", rm_load},
	{"rm_intro", rm_intro},
	{"rm_camp", rm_camp},
	{"rm_tutor", rm_tutor},
	{"rm_game", rm_game},
	{"rm_cutscenes", rm_cutscenes},
	{"rm_music_maker", rm_music_maker}
	};

}
```

Track the current room by listening for the room creation events:

```cpp
if (strcmp(codeObj->i_pName, "gml_Room_rm_camp_Create") == 0)
{
    gCurrentRoom = LHRooms::rm_camp;
}
```

Change the room with `room_goto`:

```cpp
Binds::CallBuiltinA("room_goto", { (double)LHRooms::rm_music_maker });
```

Instances you created are gone after a room change (unless they are persistent), so reset your state in `gml_Room_` events.

### Buttons

Instantiate a new object of `o_continue_button` and remove the press-callback like this:

```cpp
buttonRef = Binds::CallBuiltinA("instance_create_depth", { 480., 320., -200., (double)LHObjectEnum::o_continue_button });
Binds::CallBuiltinA("variable_instance_set", { buttonRef, "click_event", -1. });
```

The button text is changed like this:

```cpp
Binds::CallBuiltinA("variable_instance_set", { buttonRef, "text", "Test" });
Binds::CallBuiltinA("variable_instance_set", { buttonRef, "fa_ltext", "Test"});
```

The event that is triggered when clicking a button is `gml_Object_o_base_button_Mouse_4`.

Check if it is your button by comparing the instance ids:

```cpp
if (selfInst->i_id == buttonRef.As<double>())
{...}
```

Menu buttons (`o_menu_button`) also run `gml_Object_o_menu_button_Alarm_6` after being clicked, which changes the language. Cancel it in a pre-patch for your own button:

```cpp
if (strcmp(codeObj->i_pName, "gml_Object_o_menu_button_Alarm_6") == 0 && selfInst->i_id == buttonRef.As<double>())
    return YYTK_DONTCALL;
```

Check that the button still exists before touching it, e.g. after a room change:

```cpp
if ((bool)Binds::CallBuiltinA("instance_exists", { buttonRef }))
{...}
```

### Variables

Getting / setting variables is safe:

```cpp
YYRValue hp = Binds::CallBuiltinA("variable_instance_get", { (double)selfInst->i_id, "hp" });
Binds::CallBuiltinA("variable_instance_set", { (double)selfInst->i_id, "hp", 100. });
```

For checking if an instance has a variable set, use `variable_instance_exists`, if you want to check if a global variable is set, use `variable_global_exists`. Using the first with -5 as instance id will not work.

```cpp
if ((bool)Binds::CallBuiltinA("variable_global_exists", { "lang" }))
{
    YYRValue lang = Binds::CallBuiltinA("variable_global_get", { "lang" });
}
```

To get built in object variables like `sprite_index`, it is perfectly fine to use `variable_instance_get`.

```cpp
YYRValue sprite = Binds::CallBuiltinA("variable_instance_get", { (double)selfInst->i_id, "sprite_index" });
```

### Arrays

Arrays used to be completely cooked: `array_get` and `variable_instance_get_names` made the game unstable and led to a crash.
For this reason, I first dumped all possible varnames for every object I know in order to avoid calling `variable_instance_get_names`.

The runner wasn't the problem, the SDK was. `YYRValue` copied and freed arrays and strings itself, but arrays are GC objects whose refcount is not where the SDK expected it, and strings live on the runner's heap.
So every copy or destructor of a `YYRValue` holding an array corrupted memory, and the crash came later at some random point.
Since the SDK in **YYTK 2026.10.14** `YYRValue` uses the runner's own functions to copy, create and free values, so arrays work. Make sure your mod is built with that SDK.

Read an array with `array_length_1d` and `array_get`:

```cpp
YYRValue names;
CallBuiltin(names, "variable_instance_get_names", nullptr, nullptr, { (double)selfInst->i_id });

int count = (int)Binds::CallBuiltinA("array_length_1d", { names });
for (int i = 0; i < count; i++)
{
    YYRValue item = Binds::CallBuiltinA("array_get", { names, (double)i });
    std::string name = static_cast<std::string>(item); // copies, empty if not a string
}
```

Check the type with `typeof` before treating a value as an array, nested arrays work the same way:

```cpp
YYRValue type = Binds::CallBuiltinA("typeof", { value });
if (static_cast<std::string>(type) == "array")
{...}
```

A `const char*` you get from a `YYRValue` points into that value's string, so it is only valid while the value is alive and unchanged, and it is `nullptr` if the value isn't a string.
Convert to `std::string` instead, especially when reusing the same `YYRValue` inside a loop.

### Sprites

Load sprites at runtime with `sprite_add`. Use the full path, the working directory is the game directory.
The returned id can be used like any game sprite, e.g. for `sprite_index`:

```cpp
std::string path = Filesys::GetCurrentDir() + "\\Assets\\my_sprite.png";
// file, subimages, removeback, smooth, xorig, yorig
double spriteRef = Binds::CallBuiltinA("sprite_add", { path, 1., false, false, 0., 0. });
Binds::CallBuiltinA("variable_instance_set", { buttonRef, "sprite_index", spriteRef });
```

`sprite_add` returns `-1` if the file could not be loaded, so check if the file exists first.
Game sprites can be swapped with `sprite_replace`, using the ids from `LHSpriteEnum`.

### Drawing

Draw calls only work inside draw events (`Draw_0`, `Draw_64`, `Draw_73`), so run them from a post-patch of one of those.
Colours are BGR, not RGB.

The game doesn't reset the draw state, so save the font / colour / alpha / alignment before drawing and restore it afterwards or the game's own text will look wrong:

```cpp
if (strcmp(codeObj->i_pName, "gml_Object_o_controller_Draw_73") == 0)
{
    YYRValue oldColour = Binds::CallBuiltinA("draw_get_colour", {});
    Binds::CallBuiltinA("draw_set_colour", { (double)0x0000FF }); // red
    Binds::CallBuiltinA("draw_text", { 10., 10., "Hello" });
    Binds::CallBuiltinA("draw_set_colour", { oldColour });
}
```
