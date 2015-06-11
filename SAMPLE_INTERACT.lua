
-- Default interaction script. You may edit this directly, or
-- create additional files to point to.
--
-- Called every time interactive mode starts again.
-- Keep state with global variables.
--
-- One way of using the 'interact' method is
-- a custom callback that occurs every time Ctrl-C is pressed.
--
-- It is recommended that you keep utility functions in custom_functions.lua.


local gexf = require "gexf" -- Graph serialization minilibrary, found in src/gexf.lua

--
-- Configuration
--

long_print_on() -- Print objects over multiple lines in repl_run()

--
-- Event loggers
--

function on_new_network(id) 

end

function on_save_network(id) 

end

function on_load_network(id) 

end

local graph = gexf.new("pregraph.json")
function on_add(id) 
    local e = agent(id)
    local size,weight = 1,1
    if e.agent_type == "Celebrity" then 
        size,weight = 100,100
    end
    graph.add_node {
        name = tostring(id),
        label = e.agent_type,
        size = size, weight = weight,
        t = time
    }
end

function on_unfollow(id1, id2) 
    graph.delete_edge {
        name = id1.."->"..id2,
        t = time
    }
end

function on_follow(id1, id2) 
    graph.add_edge {
        name = id1.."->"..id2,
        source = tostring(id1),
        target = tostring(id2),
        t = time
    }
end

function on_tweet(id1, id2) 
end

function on_exit() 
    graph:close()
end


--
-- Interaction (ie, what happens when ctrl-C is pressed)
--

function on_interact() -- Return true to continue, false to exit
    print("Welcome to Simulator Interactive Mode.")
    print("Type exit() or quit() to (gracefully) finish the simulation.")
    return repl_run() 
end
