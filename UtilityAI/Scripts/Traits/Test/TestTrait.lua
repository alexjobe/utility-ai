local Trait = {}

Trait.Key = "Trait.Greedy"

Trait.OwnedTags = { "Generic", "Wealth" }
Trait.RequiredTags = { "Wealth" }

Trait.Considerations = {
	{
		Key = "Need.Wealth",
		Data = { Weight = 2 }
	}
}

return Trait;