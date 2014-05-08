local json = (require "json")

----------------------------------------------------------------------------------
-- Helpers for saving and loading Lua objects.
----------------------------------------------------------------------------------

function save_json(fname, obj) 
    local file = io.open(fname, "wb") 
    file:write(json.generate(obj))
    file:close()
end

function load_json(fname) 
    local file = io.open(fname, "rb") 
    local as_string = file:read("*all")
    local ok, obj = json.parse(as_string)
    assert(ok, "Not a valid JSON object! (Got " .. as_string .. ")")
    return obj
end

----------------------------------------------------------------------------------
-- Network hooks.
----------------------------------------------------------------------------------

-- Event loggers

test_id = (tonumber(os.getenv "HASHKAT_TEST_ID") == 1)
save_network = (tonumber(os.getenv "HASHKAT_SAVE_NETWORK") == 1)
load_network = (tonumber(os.getenv "HASHKAT_LOAD_NETWORK") == 1)
create_observables = (tonumber(os.getenv "HASHKAT_CREATE_OBSERVABLES") == 1)
analysis_step_save_point = tonumber(os.getenv "HASHKAT_ANALYSIS_STEP_SAVE_POINT")

function on_new_network() 
    if load_network then
        load_network_state "tests/TEST.sav"
    end
end

function on_save_network() 
end

function on_load_network() 
end

function on_step_analysis() 
    if save_network and (analysis_step > analysis_step_save_point) then
        save_network_state "tests/TEST.sav"
        os.exit()
    end
end

function on_add(id) 
end

function on_unfollow(id1, id2) 
end

function on_follow(id1, id2) 
end

function on_tweet(id_tweeter, id_tweet) 
end

function on_exit() 
    local obj = {
        n_entities = n_entities,
        max_entities = max_entities,
        rate_total = rate_total,
        rate_add = rate_add,
        rate_follow = rate_follow,
        rate_retweet = rate_retweet,
        rate_tweet = rate_tweet,
        analysis_step = analysis_step
    }

    if create_observables then
        save_json("tests/TEST_observables.json", obj)
    elseif load_network then
        pretty_print("Previous object:", obj)
        -- We have loaded the network. Test that all our observables are the same.
        local loaded = load_json("tests/TEST_observables.json")
        pretty_print("Post-loading object:", loaded)
        assert(loaded.n_entities == n_entities)
        assert(loaded.max_entities == max_entities)
        assert(loaded.rate_total == rate_total)
        assert(loaded.rate_add == rate_add)
        assert(loaded.rate_follow == rate_follow)
        assert(loaded.rate_retweet == rate_retweet)
        assert(loaded.rate_tweet == rate_tweet)
        assert(loaded.analysis_step == analysis_step)
    end
end

-- Interaction (ie, what happens when ctrl-C is pressed)

function on_interact() -- Return true to continue, false to exit
    print("Tests should not be interrupted.")
end
