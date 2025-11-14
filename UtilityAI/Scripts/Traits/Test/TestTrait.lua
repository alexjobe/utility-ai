local Trait = {}

Trait.Name = "Trait.Greedy"

Trait.OwnedTags = { "Generic", "Wealth" }

Trait.Biases = {
	{
		Target = "Wealth",
		WeightMultiplier = 2.0
	}
}

return Trait;