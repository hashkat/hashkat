local json = (require "json")

----------------------------------------------------------------------------------
-- Helpers for saving and loading Lua objects.
----------------------------------------------------------------------------------

function save_json(fname, obj) 
    local file = io.open(fname, "wb") 
    if not file then error("Could not open " .. fname) end
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

    local normal_run, run_after_loading = "tests/TEST_observables1.json", "tests/TEST_observables2.json"

    if create_observables then
        save_json(normal_run, obj)
    elseif load_network then
        -- We have loaded the network. Test that all our observables are the same.
        local loaded = load_json(normal_run)

        -- We load the current network observations from JSON as well, to isolate effects of serialization to JSON (ie, rounding)
        save_json(run_after_loading, obj)
        local current = load_json(run_after_loading)

        pretty("Normal run observations:", loaded)
        pretty("Run with loading interruption observations:", current)

        assert(loaded.n_entities == current.n_entities, "n_entities mismatch!")
        assert(loaded.max_entities == current.max_entities, "max_entities mismatch!")
        assert(loaded.rate_total == current.rate_total, "rate_total mismatch!")
        assert(loaded.rate_add == current.rate_add, "rate_add mismatch!")
        assert(loaded.rate_follow == current.rate_follow, "rate_follow mismatch!")
        assert(loaded.rate_retweet == current.rate_retweet, "rate_retweet mismatch!")
        assert(loaded.rate_tweet == current.rate_tweet, "rate_tweet mismatch!")
        assert(loaded.analysis_step == current.analysis_step, "analysis_step mismatch!")
    end
end

-- Interaction (ie, what happens when ctrl-C is pressed)

function on_interact() -- Return true to continue, false to exit
    print("Tests should not be interrupted.")
end
