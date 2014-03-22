----------------------------------------------------------------------------------------
-- Place custom Lua functions, to be made accessible during Interactive Mode.
-- Any functions placed in the global namespace will, thanks to LuaREPL, be 
-- avaible via tab-selection.
----------------------------------------------------------------------------------------

-- Startup commands:

long_print_on()

local builtin_entities = entities

-- Allows for live function addition/change:
function reload_functions()
    entities = builtin_entities
    dofile "custom_functions.lua"
end

append = table.insert -- Less verbose

-- Helpers for filtering tables:
function filter(table, attr, f)
    local new_table = {}
    for _,v in pairs(table) do
        if f(v[attr]) then
            append(new_table, v)
        end
    end
    return new_table
end

function filter_less(table, attr, val)
    return filter(table, attr, function(x) return x < val end)
end

function filter_greater(table, attr, val)
    return filter(table, attr, function(x) return x > val end)
end

function filter_equal(table, attr, val)
    return filter(table, attr, function(x) return x == val end)
end

-- Tweet analysis functions:
function tweets_of_gen(gen)
    local t = tweets()
    return filter_equal(t, "generation", gen)
end

function tweet_author(t)
    return entity(t.id_original_author)
end

function tweet_tweeter(t)
    return entity(t.id_tweeter)
end

function entities(t)
    if t == nil then
        return builtin_entities()
    else
        local tab = {}
        for _,id in ipairs(t) do
            append(tab, entity(id))
        end
        return tab
    end
end

function followings_e(id)
    return entities(followings(id))
end

function followers_e(id)
    return entities(followers(id))
end
