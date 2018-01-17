local tiles = {
    dungeon_wall = {
        glyph = '#', -- texture id for the tile, usually ascii
        fg = {.1,.1,.3,1}, -- what color to tint the tile with, {R,G,B,A}
        bg = {.6,.6,.9,1}, -- what color to paint the rect behind the tile
        passable = false, -- wether entities can pass through the tile
        opaque = true, -- wether actors can see through the tile
        wall = true, -- wether we should render the tile as a wall (only used in generation yet)
        description = "A stone wall", -- description for the tile
        tags = { "dungeon", "stone", "wall" } -- tags that help generators find the tile
    },
    dungeon_door = {
        glyph = '+',
        fg = {.1,.05,.05,1},
        bg = {.4,.2,.1,1},
        passable = true,
        opaque = true,
        wall = true,
        description = "A wooden door",
        tags = { "dungeon", "wood", "door" }
    },
    dungeon_floor = {
        glyph = '.',
        fg = {.8,.8,.95,1},
        bg = {.1,.1,.2,1},
        passable = true,
        opaque = false,
        wall = false,
        description = "A stone floor",
        tags = { "dungeon", "stone", "floor" }
    },
    -- dungeon_floor2 = {
    --     glyph = '.',
    --     fg = {.85,.85,.9,1},
    --     bg = {.15,.15,.25,1},
    --     passable = true,
    --     opaque = false,
    --     wall = false,
    --     description = "A stone floor",
    --     tags = { "dungeon", "stone", "floor" }
    -- },
    -- dungeon_floor3 = {
    --     glyph = '.',
    --     fg = {.75,.75,.95,1},
    --     bg = {.1,.1,.25,1},
    --     passable = true,
    --     opaque = false,
    --     wall = false,
    --     description = "A stone floor",
    --     tags = { "dungeon", "stone", "floor" }
    -- },
    dungeon_entrance = {
        glyph = '>',
        fg = {.8,.8,1,1},
        bg = {.1,.1,.2,1},
        passable = true,
        opaque = false,
        wall = false,
        description = "Stone stairs going upwards",
        tags = { "dungeon", "stone", "entrance" }
    },
    dungeon_exit = {
        glyph = '<',
        fg = {.8,.8,1,1},
        bg = {.1,.1,.2,1},
        passable = true,
        opaque = false,
        wall = false,
        description = "Stone stairs going downwards",
        tags = { "dungeon", "stone", "exit" }
    },
    -- grass = {
    --     glyph = '"',
    --     fg = {.4,.95,.4,1},
    --     bg = {.1,.15,.2,1},
    --     passable = true,
    --     opaque = false,
    --     wall = false,
    --     description = "A patch of grass",
    --     tags = { "dungeon", "floor", "grass" }
    -- },
    -- moss = {
    --     glyph = '.',
    --     fg = {.4,.95,.4,1},
    --     bg = {.1,.15,.2,1},
    --     passable = true,
    --     opaque = false,
    --     wall = false,
    --     description = "A patch of moss",
    --     tags = { "dungeon", "floor", "moss" }
    -- },
}

local dijkstra_debug_amount = 100

for i=1,dijkstra_debug_amount do
    local debugtile = {
        glyph = tostring(math.floor(i/10)),
        bg = {0,0,0,1},
        fg = {i/dijkstra_debug_amount,1-i/dijkstra_debug_amount,0,1},
        passable = true,
        opaque = false,
        wall = false,
        description = "Debug: "..tostring(i),
        tags = { "debug", "floor" }
    }
    tiles["dijkstra_debug_floor_"..tostring(i)] = debugtile
end

return tiles;