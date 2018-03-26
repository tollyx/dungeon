local util = {}

function util.deepcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopy(orig_key)] = deepcopy(orig_value)
        end
        setmetatable(copy, deepcopy(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end

function util.merge(bot, top)
    local p_type = type(parent)
    local c_type = type(child)
    assert(p_type == 'table', 'expected table, found '..p_type..' instead.')
    assert(c_type == 'table', 'expected table, found '..c_type..' instead.')

    for k,v in pairs(top) do
        if type(k) == 'number' and type(bot[1]) ~= nil and type(top[1]) ~= nil then
            table.insert(bot, v)
        elseif type(v) == 'table' then 
            if type(bot[k]) == 'table' then
                merge(bot[k], v)
            else
                bot[k] = v
            end
        else
            bot[k] = v
        end
    end
end

function util.inherit(parent, child)
    local p_type = type(parent)
    local c_type = type(child)
    assert(p_type == 'table', 'expected table, found '..p_type..' instead.')
    assert(c_type == 'table', 'expected table, found '..c_type..' instead.')

    local result = util.deepcopy(parent)

    merge(result, child)

    return result
end