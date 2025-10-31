local Goal = {}

Goal.Key = "Goal.AcquireWealth"
Goal.OwnedTags = { "Generic", "Wealth" }
Goal.RequiredTags = { "Wealth" }
Goal.PreconditionFnKey = "Invalid"
Goal.Considerations = {
    _MakeNeedConsideration(ENeedType.Wealth, 1),
}

return Goal