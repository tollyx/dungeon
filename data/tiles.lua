local tiles = {
    dungeon_wall = {
        glyph = '#', -- texture id for the tile, usually ascii
        fg = {0.1,0.1,0.3,1.0}, -- what color to tint the tile with, {R,G,B,A}
        bg = {0.6,0.6,0.9,1.0}, -- what color to paint the rect behind the tile
        passable = false, -- wether entities can pass through the tile
        opaque = true, -- wether actors can see through the tile
        wall = true, -- wether we should render the tile as a wall (only used in generation yet)
        description = "A stone wall", -- description for the tile
        tags = { "dungeon", "stone", "wall" } -- tags that help map generators find the tile
    },
    dungeon_wall_alt = {
        glyph = '#',
        fg = {0.01,0.01,0.1,1.0},
        bg = {0.4,0.4,0.8,1.0},
        passable = false, 
        opaque = true, 
        wall = true, 
        description = "A stone wall", 
        tags = { "dungeon", "stone", "wall" } 
    },
    dungeon_door = {
        glyph = '+',
        fg = {0.1,0.05,0.05,1.0},
        bg = {0.4,0.2,0.1,1.0},
        passable = true,
        opaque = true,
        wall = true,
        description = "A wooden door",
        tags = { "dungeon", "wood", "door" }
    },
    dungeon_floor = {
        glyph = '.',
        fg = {0.8,0.8,1.0,1.0},
        bg = {0.1,0.1,0.2,1.0},
        passable = true,
        opaque = false,
        wall = false,
        description = "A stone floor",
        tags = { "dungeon", "stone", "floor" }
    },
    dungeon_floor_alt = {
        glyph = '.',
        fg = {0.6,0.6,0.9,1.0},
        bg = {0.1,0.1,0.2,1.0},
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
        fg = {0.8,0.8,1.0,1.0},
        bg = {0.1,0.1,0.2,1.0},
        passable = true,
        opaque = false,
        wall = false,
        description = "Stone stairs going upwards",
        tags = { "dungeon", "stone", "entrance" }
    },
    dungeon_exit = {
        glyph = '<',
        fg = {0.8,0.8,1.0,1.0},
        bg = {0.1,0.1,0.2,1.0},
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
        bg = {0.0,0.0,0.0,1.0},
        fg = {i/dijkstra_debug_amount,1-i/dijkstra_debug_amount,0.0,1.0},
        passable = true,
        opaque = false,
        wall = false,
        description = "Debug: "..tostring(i),
        tags = { "debug", "floor" }
    }
    tiles["dijkstra_debug_floor_"..tostring(i)] = debugtile
end

return tiles;