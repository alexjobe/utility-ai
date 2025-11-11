local Goal = {}

Goal.Name = "Goal.IncreaseWealth"
Goal.OwnedTags = { "Generic", "Wealth" }
Goal.RequiredTags = { "Wealth" }
Goal.PreconditionFnKey = "Invalid"
Goal.Considerations = {
    _MakeNeedConsideration(ENeedType.Wealth, 1),
}

return Goal