local Trait = {}

Trait.Name = "Trait.Greedy"

Trait.OwnedTags = { "Generic", "Wealth" }
Trait.RequiredTags = { "Wealth" }

Trait.Biases = {
	{
		RequiredTag = "Wealth",
		WeightMultiplier = 2.0
	}
}

return Trait;