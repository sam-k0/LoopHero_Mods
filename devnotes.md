Notes for developing mods.

### Built In constants

These values have a special meaning:

```c++
#define INSTANCE_GLOBAL -5.0
#define INSTANCE_NOONE -4.0
#define INSTANCE_ALL -3.0
#define INSTANCE_OTHER -2.0
#define INSTANCE_SELF -1.0
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
if (selfInst->i_spriteindex == buttonRef.As<double>())
{...}
```


### Variables

Getting / setting variables is safe, what is not safe is querying for variables and then getting the names.
For some reason arrays are completely cooked in this YYG runner version, so ``array_get`` will make the game unstable and lead to a crash.

For this reason, I first dumped all possible varnames for every object I know in order to circumvent having to call `variable_instance_get_names`. Call it crazy, I do too, but its the only way.

For checking if an instance has a variable set, use `variable_instance_exists`, if you want to check if a global variable is set, use `variable_global_exists`. Using the first with -5 as instance id will not work.

To get built in object variables like `sprite_index`, it is perfectly fine to use `variable_instance_get`.