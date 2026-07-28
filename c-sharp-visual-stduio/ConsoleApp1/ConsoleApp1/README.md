# Program.cs
## PSEUDOCODE / PLAN (detailed):
1. Show a simple menu that maps integer indices to "run" actions:
   - 7 -> run behavior from `Car.cs` (instantiates `Car1`, calls methods, prints fields)
   - 8 -> run behavior from `Vehical.cs` (instantiates `Vehical`, calls methods, prints fields)
   - 0 -> exit program
   - other -> print message and re-show menu
2. Read user input using `Console.ReadLine`.
3. Validate input using `int.TryParse`.
4. Use a `switch` on the parsed index:
   - case 7:
       a. Create `Car1 car = new();`
       b. Call `car.activateKey();`
       c. Call `car.Honk();`
       d. Print `car.modelName` and `car.brand`
   - case 8:
       a. Create `Vehical veh = new();`
       b. Call `veh.activateKey();`
       c. Call `veh.Honk();`
       d. Print `veh.brand`
   - case 0:
       a. break loop and exit
   - default:
       a. Inform user index is unknown
5. Wrap the menu loop so user can run multiple choices until they choose to exit.
6. Provide clear prompts and handle invalid input gracefully.
7. Keep implementation minimal and compatible with C# 12 / .NET 8.

IMPLEMENTATION NOTES:
- Use the existing `Vehical` and `Car1` signatures from the project.
- Keep console interactions simple and synchronous.
