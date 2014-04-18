
-- Default interact file. You may edit this directly, or
-- create additional files to point to.
--
-- Called every time interactive mode starts again.
-- Keep state with global variables.
--
-- One way of using the 'interact' method is
-- a custom callback that occurs every time Ctrl-C is pressed.
--
-- It is recommended that you keep utility functions in custom_functions.lua.

function interact()
    print("Welcome to Simulator Interactive Mode.")
    print("Type exit() or quit() to (gracefully) finish the simulation.")
    return repl_run()
end
