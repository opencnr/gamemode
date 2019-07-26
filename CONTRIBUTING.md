# Contributing

This contribution guideline contains information about the projects file and code structure and it's conventions.

## File structure

- gamemode-name
    - package-name
        - package-name.inc
        - package-name-module-name.inc
- gamemode-name.pwn
- gamemode-name.amx

### Packages

**What are packages?** Packages are folders that are used to organize source-code. For example : Code that's specifically related to vehicles (Vehicle initialization, vehicle manipulation API) has it's own `vehicles` package.

### Modules

**What are modules?** A module is an `.inc` file containing the actual code. Each package has at least one module. The main module of a package is named after the package itself. For example : If the package is named `vehicles`, the main module is also named `vehicles.inc`.

Modules that aren't main modules are called **sub-modules**. These modules must be namespaced with the package name. For example : `vehicles-surfing` (A module that prevents the player from surfing on a vehicle). Sub-modules should be included by the main module in the Internal section.

## Module structure
- **Description of the module**
    - This section is not structured like other sections. It's simply a comment.
    - `// module-name does X, Y and Z`
- **External Packages**
    - External packages are all non-game-mode packages (Includes, Plugins) that this package depends on.
    - If you're planning to use `y_hooks`, do not include it here. See internal section.
- **Internal Packages**
    - Internal packages are all packages from the game-mode itself that this package depends on for it's API.
- **Declarations**
    - All static variables, enums should be declared here.
    - There is a **sub-section** for player specific variables (Ex. `static playerRole[MAX_PLAYERS];`)
        - `// - Player variables`
    - There is a **sub-section** for NativeHook variables (PawnPlus).
        - `// - Hooks`
- **API**
    - This section contains code and variables that’ll be used by other packages. 
    - Everything MUST be documented.
    - Define pre-compiler definitions here. See *Pre-compiler definitions* to see how.
    - All declared functions must prefix with `stock`. 
    - Custom callbacks, must be declared with `forward` and called elsewhere with `CallLocalFunction`.
- **Internal**
    - Code that is only used by the package itself goes here.
    - Include all internal modules here (These are modules that are within the same package).
    - Include external package that extend from / depend on this package here. (Only in main module)
    - All declared functions in this part must be prefixed with `static`.
    - All declared functions must be named as follows
        - `stock FunctionName(arg1, arg2, arg3)`
    - Documenting functions is optional.
    - There is a **sub-section** for y_timers.
        - `// - Timers`
    - There is a **sub-section** for y_hooks. Include `<YSI\y_hooks>` here.
        - `// - Hooks`
    - There is a **sub-section** for commands.
        - `// - Commands`

Code structure sections must be specified as follows :
```CPP
// -
// Section Name
// -
```
Sub-sections are specified with a simple comment and a dash : 
```CPP
// - Player variables
```

## Code style

This project uses the Allman code style for all of it's code.

### What to do (Allman)
```CPP
FunctionName()
{

}
```

### What NOT to do (K&R)
```CPP
FunctionName() {

}
```

## Documentation

**Documenting functions**

```C
// FunctionName does X, Y and Z. Returns 1 always.
static FunctionName(arg1, arg2, arg3)
{
    return 1;
}
```

**Documenting forwards**
```C
// OnSomethingHappening is called {before|after} X, Y and Z.
forward OnSomethingHappening(arg1, arg2, arg3);
```

**Documenting variables**
```C
// someVar stores X, Y and Z.
static someVar;
```

**Documenting pre-compiler definitions**
```C
// SOME_THING is X.
#if !defined SOME_THING
    #define SOME_THING 123
#endif
```

## Naming convention

- **Variables**
    - Variables should be named using camelCase.
- **Enumerators**
    - The enum itself should be named as followed : E_X
    - It’s values should be named as followed : E_X_Y
```CPP
enum E_PLAYER_POSITION
{
     E_PLAYER_POSITION_X,
     E_PLAYER_POSITION_Y,
     E_PLAYER_POSITION_Z
};
```
-
    - If the name is not mandatory, do the following (Notice that in this case the `E_` prefix is not necessary.) :
```CPP
enum /* Vehicle Parameters */
{
    VEHICLE_PARAMETER_ENGINE,
    VEHICLE_PARAMETER_LIGHTS,
    VEHICLE_PARAMETER_ALARM,
    VEHICLE_PARAMETER_DOORS,
    VEHICLE_PARAMETER_BONNET
};
```
- **PawnPlus hooks**
    - NativeHook variables must be named as follows : `hookv_NativeName`
    - NativeHook functions must be named as follows : `hookf_NativeName`
- **Static functions**
    - As mentioned before in *Module Structure > Internal*, static functions should be named in PascalCase.

## Pre-compiler definitions

Pre-compiler definitions should be declared in the API section as follows :

```C
#if !defined DB_CONNECTION_ATTEMPTS
    #define DB_CONNECTION_ATTEMPTS 25
#endif
```

This way they can be modified in `config.inc`.

## Native Hooks

**What are Native Hooks?** Native Hooks are functions that overwrite native functions. (Ex. `SetPlayerPos`) Each native can only be hooked once.

OpenCnR uses PawnPlus to hook natives during run-time rather than hooking them during build-time.

**How do I set up a Native Hook?**

1. Create a NativeHook variable in the Hooks sub-section of the Declarations section.
    ```C
    // -
    // Declarations
    // -

    // - Hooks

    static NativeHook:hookv_NativeName;
    ```
2. Create a NativeHook function in the Hooks sub-section of the Internal section.
    ```C
    // -
    // Internal
    // -
    
    // - Hooks

    forward hookf_NativeName(arg1, arg2, arg3);
    public hookf_NativeName(arg1, arg2, arg3)
    {
        // Do something
        return something;
    }
    ```
For more information about Native Hooks, check the PawnPlus documentation...

## Storage

All interactions made with the database must be done through the `storage` package. The `storage` package must do nothing aside from retrieving, serving and setting database data.

All API functions in the `storage` package must do the following :
    - Take a DB handle as it's first argument.
    - Return 0 if the MySQL has failed, 1 if it succeeded.

Each database table has it's own module.
