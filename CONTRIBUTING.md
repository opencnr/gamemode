# Contributing

This contribution guideline contains information about the project structure.

## File structure

- gm-name
    - package-name
        - package-name.inc
        - package-name-module-name.inc
- gm-name.pwn
- gm-name.amx

### Packages

Packages are used to organize source-code for better re-usability and readability. For example : Code that's specifically related to vehicles (Vehicle initialization, vehicle manipulation API) has it's own `vehicles` package.

### Modules

Each package has at least one module. A module is an `.inc` file containing the actual code. The main module of a package is named after the package itself. For example : If the package is named `vehicles`, the main module is also named `vehicles`.

Modules that aren't main modules are called sub-modules. These modules must be namespaced with the package name. For example : `vehicles-surfing` (A module that prevents the player from surfing on a vehicle). Sub-modules should be included by the main module.

## Code structure
- Description of package or module
- External Packages
    - External packages are all non-game-mode packages (Includes, Plugins) that this package depends on.
- Declarations
    - All variable, enums and defines should be declared here. Both static and non-static.
    - There is a sub-section for player specific variables (Ex. `static playerRole[MAX_PLAYERS];`)
        - `// Player variables`
    - There is a sub-section for NativeHook variables (PawnPlus).
        - `// Hooks`
- Internal Packages
    - Internal packages are all packages from the game-mode itself that this package depends on and vice versa.
- API
    - This part contains code that’ll be used by other packages. 
    - All functions and forwards must be documented.
    - All declared functions must prefix with `stock`. 
    - Custom callbacks, must be declared with `forward` and called with `CallLocalFunction`.
- Internal
    - Actual code (That is only used by the package itself) goes here.
    - All declared functions in this part must be prefixed with `static`.
    - Include all internal modules here (These are modules that are within the same package) as well as `y_hooks` (Optional of course. It has to be included last to generate new function names).

Code structure sections must be specified as follows :
```CPP
// -
// Section Name
// -
```
Sub-sections are specified with a simple comment : 
```CPP
// Player variables
```

## Code style

K&R and Allman are both fine, but Allman is recommended.

### K&R
```CPP
FunctionName() {

}
```

### Allman
```CPP
FunctionName()
{

}
```

## Naming convention


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
