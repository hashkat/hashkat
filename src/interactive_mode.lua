local repl = require 'repl.console'
for _, plugin in ipairs { 'linenoise', 'history', 'completion', 'autoreturn' } do
    repl:loadplugin(plugin)
end

local M = {}

--- Get a  human-readable string from a lua value. The resulting value is generally valid lua.
-- Note that the paramaters should typically not used directly, except for perhaps 'packed'.
-- @param val the value to pretty-print
-- @param tabs <i>optional, default 0</i>, the level of indentation
-- @param packed <i>optional, default false</i>, if true, minimal spacing is used
-- @param quote_strings <i>optional, default true</i>, whether to print strings with spaces
function pretty_tostring(val, --[[Optional]] tabs, --[[Optional]] packed, --[[Optional]] quote_strings)
    tabs = tabs or 0
    quote_strings = (quote_strings == nil) or quote_strings

    local tabstr = ""

    if not packed then
        for i = 1, tabs do
            tabstr = tabstr .. "  "
        end
    end
    if type(val) == "string" then val = val:gsub('\n','\\n') end
    if type(val) == "string" and quote_strings then
        return tabstr .. "\"" .. val .. "\""
    end

    local meta = getmetatable(val) 
    if (meta and meta.__tostring) or type(val) ~= "table" then
        return tabstr .. tostring(val)
    end

    local parts = {"{", --[[sentinel for remove below]] ""}

    for k,v in pairs(val) do
        table.insert(parts, packed and "" or "\n") 

        if type(k) == "number" then
            table.insert(parts, pretty_tostring(v, tabs+1, packed))
        else 
            table.insert(parts, pretty_tostring(k, tabs+1, packed, false))
            table.insert(parts, " = ")
            table.insert(parts, pretty_tostring(v, type(v) == "table" and tabs+1 or 0, packed))
        end

        table.insert(parts, ", ")
    end

    parts[#parts] = nil -- remove comma or sentinel

    table.insert(parts, (packed and "" or "\n") .. tabstr .. "}");

    return table.concat(parts)
end

function pretty_tostring_compact(v)
    return pretty_tostring(v, nil, true)
end

local function pretty_s(val)
    if type(val) ~= "function" then
        return pretty_tostring_compact(val)
    end
    local info = debug.getinfo(val)
    local ups = "{" ; for i=1,info.nups do 
        local k, v = debug.getupvalue(val,i) ; ups = ups .. k .."="..tostring(v)..","
    end
    return "function " .. info.source .. ":" .. info.linedefined .. "-" .. info.lastlinedefined .. ups .. '}'
end

-- Convenience print-like function:
function pretty(...)
    local args = {}
    for i=1,select("#", ...) do
        args[i] = pretty_s(select(i, ...))
    end
    print(unpack(args))
end

local long_print = false
local first_run = true
function long_print_on() 
    long_print = true
end
function long_print_off() 
    long_print = false
end

-- Exported global function for starting the interactive interpreter.
function repl_run()
    local keep_going = true
    local R = repl:clone()

    function exit() 
        keep_going = false 
    end
    quit = exit

    -- Override
    function R:run()
        self:prompt(1)
        for line in self:lines() do
            local level = self:handleline(line)
            self:prompt(level)
            if not keep_going then 
                break 
            end
        end
        self:shutdown()
    end

    function R:displayresults(results)
        if results.n == 0 then return end
        if long_print then
            for i=1,results.n do
                print(pretty_tostring(results[i]))
            end
        else
            pretty(unpack(results, 1, results.n))
        end
    end

    R:run()
    return keep_going
end

----------------------------------------------------------------------------------------
-- Place custom Lua functions, to be made accessible during Interactive Mode.
-- Any functions placed in the global namespace will, thanks to LuaREPL, be 
-- avaible via tab-selection.
----------------------------------------------------------------------------------------

-- Startup commands:

local builtin_agents = agents

-- Allows for live function addition/change:
function reload_functions()
    agents = builtin_agents
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
    return agent(t.id_original_author)
end

function tweet_tweeter(t)
    return agent(t.id_tweeter)
end

-- Resolve agent id's and return actual agent objects
function agents(t)
    if t == nil then
        return builtin_agents()
    else
        local tab = {}
        for _,id in ipairs(t) do
            append(tab, agent(id))
        end
        return tab
    end
end

function followings_e(id)
    return agents(followings(id))
end

function followers_e(id)
    return agents(followers(id))
end
