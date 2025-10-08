local Trait = {}

Trait.Key = "Trait.Greedy"

Trait.Tags = { "Wealth" }

Trait.Considerations = {
	{
		Key = "Need.Wealth",
		Data = { Weight = 2 }
	}
}

return Trait;