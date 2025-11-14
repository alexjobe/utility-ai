local Action = {}

_RegisterEffectFunction("RaidVillage", function (Ctx, Data)
   print("Village was raided!")
   -- Do something with context
end)

_RegisterScoreFunction("SuccessChanceScore", function (Ctx, Data)
    local Strength = Ctx:GetStat("Strength")
    local Endurance = Ctx:GetStat("Endurance")
    local Chance = (0.6 * (Strength / 20) + 0.4 * (Endurance / 20))
    print("Success Chance: " .. Chance)
    return Chance
end)

_RegisterCurveFunction("TimeScoreCurve", function (X)
    return 1.0 - X
end)

Action.Name = "Action.Raid"
Action.OwnedTags = { "Wealth", "Aggressive", "Generic" }

Action.Considerations = {
    {
        Key = "SuccessChance",
        Data = { BaseWeight=1.5 },
        RawScoreFnKey = "SuccessChanceScore"
    },
    {
        Key = "TimeCost",
        Data = { Target="TimeCost", Raw=4, MinRaw=1, MaxRaw=5, BaseWeight=0.5 },
        ScoreCurveFnKey = "TimeScoreCurve"
    }
}

Action.Effects = {
    _MakeNeedEffect(ENeedType.Wealth, 30, 1),
    _MakeNeedEffect(ENeedType.Survival, -10, 1),
    {
        Key = "Effect.RaidVillage",
        bIsConsideration = false,
        EffectFnKey = "RaidVillage"
    },
}

return Action
