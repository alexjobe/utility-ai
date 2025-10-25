local Action = {}

_RegisterEffectFunction("RaidVillage", function (Ctx, Data)
   print("Village was raided!")
   -- Do something with context
end)

_RegisterScoreFunction("SuccessChanceScore", function (Ctx, Data)
    local Strength = Ctx:GetStat("Strength")
    local Endurance = Ctx:GetStat("Endurance")
    local Chance = (0.6 * Strength + 0.4 * Endurance)
    print("Success Chance: " .. Chance)
    return Chance
end)

_RegisterCurveFunction("TimeScoreCurve", function (X)
    return 1.0 - X
end)

Action.Key = "Action.Raid"
Action.OwnedTags = { "Wealth", "Aggressive" }

Action.Considerations = {
    {
        Key = "SuccessChance",
        Data = { Weight=1.5 }
    },
    {
        Key = "TimeCost",
        Data = { Raw=4, MinRaw=1, MaxRaw=5, Weight=0.5 },
        ScoreCurveFn = TimeScoreCurve
    }
}

Action.Effects = {
    _MakeNeedEffect(ENeedType.Wealth, 30),
    _MakeNeedEffect(ENeedType.Survival, -10),
    {
        Key = "Effect.RaidVillage",
        bIsConsideration = false,
        EffectFnKey = "RaidVillage"
    },
}

return Action
