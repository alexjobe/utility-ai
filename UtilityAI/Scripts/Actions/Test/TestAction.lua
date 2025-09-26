local Action = {}

function RaidVillage(Ctx, Data)
   print("Village was raided!")
end

function SuccessChanceScore(Ctx, Data)
    local Strength = Ctx:GetStat("Strength")
    local Endurance = Ctx:GetStat("Endurance")
    local Chance = (0.6 * Strength + 0.4 * Endurance)
    print("Success Chance: " .. Chance)
    return Chance
end

Action.Key = "Action.Raid"
Action.Tags = { "Wealth", "Aggressive" }

Action.Considerations = {
    {
        Key = "SuccessChance",
        Data = { Target="", Raw=0, MinRaw=0, MaxRaw=0, Weight=1.5, Priority=1 },
        EvalRawScoreFn = SuccessChanceScore
    }
}

Action.Effects = {
    _MakeNeedEffect(ENeedType.Wealth, 30),
    _MakeNeedEffect(ENeedType.Survival, -10),
    {
        Key = "Effect.RaidVillage",
        EffectFn = RaidVillage,
        bIsConsideration = false
    },
}

return Action
