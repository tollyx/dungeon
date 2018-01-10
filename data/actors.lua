return {
    hero = {                            -- actor id 
        name = "Hero",                  -- default name of the actor
        player_controlled = true,       -- wether it is controlled by the player
        health = "6",                   -- starting & max health
        strength = "2",                 -- strength
        glyph_id = '@',                 -- sprite id for the glyph, chars gets converted to ints
        glyph_color = {.1, .4, .9, 1},  -- color tint for the glyph
    },
    goblin = {
        name = "Goblin",
        player_controlled = false,
        health = "4",
        strength = "1",
        glyph_id = 'g',
        glyph_color = {.1, .7, .2, 1},
    },
    shaman = {
        name = "Shaman",
        player_controlled = false,
        health = "2",
        strength = "1",
        glyph_id = 'g',
        glyph_color = {.1, .7, .5, 1},
    },
}