local json = (require "json").generate -- Defined in dependencies/lua-misc/json.lua

local function gexf_writer(fname)
    -- Create the object:
    local O = {}

    -- Open file for writing:
    local file = io.open(fname, "wb") 

    -- Define the methods:
    local function write_obj(o)
        file:write(json(o) .. '\n')
    end

    function O.close()
        file:close()
    end

    function O.add_node(node) 
        write_obj { t = node.t, an = { 
            [node.name] = {
                label = node.label, 
                size = (node.size or 1)
            }
        }}
    end

    function O.add_edge(node) 
        write_obj { t = node.t, ae = { 
            [node.name] = { 
                source = node.source,
                target = node.target,
                directed = node.directed or true,
                weight = node.weight or 1,
                label = node.label
            }
        }}
    end

    function O.change_node(node) 
        write_obj { t = node.t, cn = { 
            [node.name] = { 
                size = node.size,
                label = node.label,
                weight = node.weight
            }
        }}
    end

    function O.change_edge(node) 
        write_obj { t = node.t, ce = { 
            [node.name] = { 
                label = node.label,
                weight = node.weight
            }
        }}
    end

    function O.delete_edge(node) 
        write_obj { t = node.t, de = { 
            [node.name] = { 
            }
        }}
    end

    function O.delete_node(node) 
        write_obj { t = node.t, dn = { 
            [node.name] = { 
            }
        }}
    end

    -- Return the object:
    return O
end

return {
    new = gexf_writer
}
