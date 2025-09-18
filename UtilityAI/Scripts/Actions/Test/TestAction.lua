local Action = {}

function WoodScore(ctx, data)
    return math.min(ctx:GetNeed("Survival"), 1.0)
end

function ChopWoodEffect(ctx, data)
   print("Has wood!")
end

Action.Key = "ChopWood"
Action.Tags = {"Gathering"}

Action.Considerations = {
    {
        Key = "WoodLow",
        Data = { Target="Wood", Raw=0, MinRaw=0, MaxRaw=10, Weight=0.5, Priority=1 },
        EvalRawScoreFn = WoodScore
    }
}

Action.Effects = {
    {
        Key = "HasWood",
        ConsiderationKey = "Wood",
        Data = { Target="Wood", Raw=2, MinRaw=0, MaxRaw=10, Weight=1.0, Priority=1 },
        bIsConsideration = true,
        EffectFn = ChopWoodEffect
    }
}

return Action
