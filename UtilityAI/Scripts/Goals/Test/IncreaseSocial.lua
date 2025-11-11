local Goal = {}

Goal.Name = "Goal.IncreaseSocial"
Goal.OwnedTags = { "Generic", "Social" }
Goal.RequiredTags = { "Social" }
Goal.PreconditionFnKey = "Invalid"
Goal.Considerations = {
    _MakeNeedConsideration(ENeedType.Social, 1),
}

return Goal
