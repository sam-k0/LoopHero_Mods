Notes for developing mods.

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

