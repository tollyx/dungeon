return {
    hero = {                            -- actor id 
        name = "Hero",                  -- default name of the actor
        health = "6",                   -- starting & max health
        strength = "2",                 -- strength
        glyph_id = '@',                 -- sprite id for the glyph, chars gets converted to ints
        glyph_color = {0.1, 0.4, 0.9, 1.0},  -- color tint for the glyph
    },
    goblin = {
        name = "Goblin",
        health = "4",
        strength = "1",
        glyph_id = 'g',
        glyph_color = {0.1, 0.7, 0.2, 1.0},
    },
    shaman = {
        name = "Shaman",
        health = "2",
        strength = "1",
        glyph_id = 'g',
        glyph_color = {0.1, 0.7, 0.5, 1.0},
    },
}